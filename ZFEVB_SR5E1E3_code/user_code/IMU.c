#include "balance_control.h"

cascade_value_struct roll_balance_cascade;          // 俯仰平衡控制参数结构体
cascade_value_struct roll_balance_cascade_resave;   // 俯仰平衡控制参数结构体初始备份
cascade_value_struct pitch_balance_cascade;         // 横滚平衡参数级联结构体
cascade_value_struct pitch_balance_cascade_resave;  // 横滚平衡参数级联结构体初始备份

// 函数简介    计算正切值的反正切（近似计算）
// 返回参数   float - 反正切角度值（单位：度）
// 使用示例   float angle = arctan1(1.0f); // 计算 tan(45°) 的反正切，返回 45.0 度
// 备注信息   采用分段近似公式计算，简化运算，适用于对精度要求不高的场景
static float arctan1(float tan)
{
    // 分段计算反正切近似值，根据 tan 的绝对值是否大于 1 选择不同公式
    float angle = (func_abs(tan) > 1.0f) ? 90.0f - (func_abs(1.0f / tan)) * (45.0f - (func_abs(1.0f / tan) - 1.0f) * (14.0f + 3.83f * func_abs(1.0f / tan))) :
                                          func_abs(tan) * (45.0f - (func_abs(tan) - 1.0f) * (14.0f + 3.83f * func_abs(tan)));
    return (tan > 0) ? angle : -angle;  // 根据 tan 符号确定角度正负
}

// 函数简介    计算二维坐标的反正切（类似 atan2，返回角度）
// 返回参数   float - 角度值（单位：度，范围 -180°~180°）
// 使用示例   float angle = arctan2(1.0f, 1.0f); // 计算 (1,1) 点的角度，返回 45.0 度
// 备注信息   基于 arctan1 实现，处理坐标轴特殊情况（x 或 y 为 0 时）
static float arctan2(float x, float y)
{
    float tan, angle;

    if (x == 0 && y == 0) return 0;    // 原点特殊处理，返回 0 度
    if (x == 0)                         // x 为 0 时，角度为 ±90 度
    {
        if (y > 0) return 90;
        else return -90;
    }
    if (y == 0)                         // y 为 0 时，角度为 0 或 -180 度
    {
        if (x > 0) return 0;
        else return -180.0f;
    }
    tan = y / x;                        // 计算正切值
    angle = arctan1(tan);               // 调用 arctan1 计算基础角度

    if (x < 0 && angle > 0)             // 根据 x 的符号调整角度，确保范围正确
    {
        angle -= 180.0f;
    }
    else if (x < 0 && angle < 0)
    {
        angle += 180.0f;
    }
    return angle;
}

// 函数简介    计算正弦值的反正弦（近似计算）
// 返回参数   float - 反正弦角度值（单位：度）
// 使用示例   float angle = arcsin(1.0f); // 计算 sin(90°) 的反正弦，返回 90.0 度
// 备注信息   基于 arctan1 实现，利用恒等式 arcsin(x) = arctan(x / sqrt(1 - x2))
static float arcsin(float i)
{
    return arctan1(i / sqrt(1 - i * i)); // 转换为反正切计算
}

// 函数简介    加速度计数据低通滤波
// 返回参数   void
// 使用示例   acc_lowpass_filter(&ax, &ay, &az, &fax, &fay, &faz, 0.8f);
// 备注信息   一阶低通滤波，公式：filtered = alpha * filtered + (1 - alpha) * raw, alpha 越大滤波越强
static void acc_lowpass_filter(float *raw_x, float *raw_y, float *raw_z, float *filtered_x, float *filtered_y, float *filtered_z, float alpha)
{
    *filtered_x = alpha * *filtered_x + (1 - alpha) * *raw_x;  // X 轴滤波计算
    *filtered_y = alpha * *filtered_y + (1 - alpha) * *raw_y;  // Y 轴滤波计算
    *filtered_z = alpha * *filtered_z + (1 - alpha) * *raw_z;  // Z 轴滤波计算
}

// 函数简介    加速度计数据归一化（单位化）
// 返回参数   void
// 使用示例   acc_normalize(&ax, &ay, &az);
// 备注信息   将加速度向量归一化为单位向量，当模长过小时（<0.1），默认设为 (0,0,1)
static void acc_normalize(float *ax, float *ay, float *az)
{
    float norm = sqrt(*ax * *ax + *ay * *ay + *az * *az);    // 计算加速度向量模长
    if (norm < 0.1f)                                        // 模长过小，视为无效数据，赋默认值
    {
        *ax = 0.0f;
        *ay = 0.0f;
        *az = 1.0f;
    }
    else                                                    // 模长有效，归一化
    {
        *ax /= norm;
        *ay /= norm;
        *az /= norm;
    }
}

// 函数简介    判断设备是否处于静止状态
// 返回参数   bool - 静止状态返回 true，运动状态返回 false
// 使用示例   bool static_flag = is_static_state(ax_g, ay_g, az_g);
// 备注信息   通过加速度模长判断（静止时加速度接近 1g），模长在 0.9~1.1g 范围内视为静止
static bool is_static_state(float ax_g, float ay_g, float az_g)
{
    float norm = sqrt(ax_g * ax_g + ay_g * ay_g + az_g * az_g);  // 计算加速度模长（g 为单位）
    return (norm >= 0.9f && norm <= 1.1f) ? true : false;       // 判断是否在静止范围内
}

// 函数简介    四元数及姿态角计算
// 返回参数   void
// 使用示例   quaternion_module_calculate(&quaternion, 0.001f); // 以 1 ms 周期调用
// 备注信息   融合陀螺仪和加速度计数据，通过梯度下降法更新四元数，计算旋转矩阵和姿态角
//            输入参数 cycle 为调用周期（单位：秒），需稳定调用以保证计算精度
void quaternion_module_calculate(cascade_value_struct *cascade_value)
{
    static float first_count_time = 0;  // 首次计算时间计数器（用于快速收敛）
    float length;                       // 四元数模长（用于归一化）
    float x, y, z;                      // 陀螺仪角速度（弧度/秒）

    // 陀螺仪数据转换：原始数据 -> (°/s) -> 弧度/秒（先除以 10 再乘 10 做简单滤波）
    x = (float)(GYRO_DATA_X / 10 * 10) / GYRO_TRANSITION_FACTOR * 0.01745329f;  // 0.01745329 为度转弧度系数（π/180）
    y = (float)(GYRO_DATA_Y / 10 * 10) / GYRO_TRANSITION_FACTOR * 0.01745329f;
    z = (float)(GYRO_DATA_Z / 10 * 10) / GYRO_TRANSITION_FACTOR * 0.01745329f;

    // 加速度计数据转换：原始数据 -> g 为单位（1g ≈ 9.8 m/s2）
    float ax_g = (float)ACC_DATA_X / ACC_TRANSITION_FACTOR;
    float ay_g = (float)ACC_DATA_Y / ACC_TRANSITION_FACTOR;
    float az_g = (float)ACC_DATA_Z / ACC_TRANSITION_FACTOR;

    bool static_state = is_static_state(ax_g, ay_g, az_g);  // 判断设备是否静止
    float acc_alpha = static_state ? 0.8f : 0.5f;           // 静止时滤波系数更大（平滑效果更好）

    // 加速度计数据低通滤波
    acc_lowpass_filter(&ax_g, &ay_g, &az_g,
                       &cascade_value->quaternion.pro.acc_filtered[0],
                       &cascade_value->quaternion.pro.acc_filtered[1],
                       &cascade_value->quaternion.pro.acc_filtered[2], acc_alpha);

    // 取滤波后的加速度数据
    float ax = cascade_value->quaternion.pro.acc_filtered[0];
    float ay = cascade_value->quaternion.pro.acc_filtered[1];
    float az = cascade_value->quaternion.pro.acc_filtered[2];

    acc_normalize(&ax, &ay, &az);  // 加速度数据归一化

    // 取出当前四元数 (w, x, y, z)
    float q0 = cascade_value->quaternion.pro.qua[0];
    float q1 = cascade_value->quaternion.pro.qua[1];
    float q2 = cascade_value->quaternion.pro.qua[2];
    float q3 = cascade_value->quaternion.pro.qua[3];

    // 根据当前四元数计算重力向量在机体坐标系中的投影（用于与加速度计数据对比）
    float gx = 2 * (q1 * q3 - q0 * q2);
    float gy = 2 * (q0 * q1 + q2 * q3);
    float gz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    // 计算加速度计与重力投影的误差（梯度误差）
    float ex = ay * gz - az * gy;
    float ey = az * gx - ax * gz;
    float ez = ax * gy - ay * gx;

    // 根据静止状态调整校正系数（运动时减弱校正，避免引入噪声）KI不衰减，保证积分校正力度
    float kp = static_state ? cascade_value->posture_value.correct_kp : cascade_value->posture_value.correct_kp * 0.8f;
    float ki = cascade_value->posture_value.correct_ki;

    // 首次计算的前 0.1 秒，使用大比例系数加速收敛
    if(first_count_time < 0.1f)
    {
        first_count_time += cascade_value->posture_value.call_cycle;  // 累加时间
        kp = 10.0f;  // 强比例校正，快速收敛
    }

    // 动态时积分力度降为1/10
    float integral_gain = static_state ? 1.0f : 0.1f;

    cascade_value->quaternion.parameter.acc_err[0] += (ex * cascade_value->posture_value.call_cycle) * integral_gain;
    cascade_value->quaternion.parameter.acc_err[1] += (ey * cascade_value->posture_value.call_cycle) * integral_gain;
    cascade_value->quaternion.parameter.acc_err[2] += (ez * cascade_value->posture_value.call_cycle) * integral_gain;

    // 积分限幅：防止饱和
    float acc_err_limit = 1.0f;
    cascade_value->quaternion.parameter.acc_err[0] = (cascade_value->quaternion.parameter.acc_err[0] > acc_err_limit) ? acc_err_limit :
                                                      (cascade_value->quaternion.parameter.acc_err[0] < -acc_err_limit) ? -acc_err_limit :
                                                      cascade_value->quaternion.parameter.acc_err[0];
    cascade_value->quaternion.parameter.acc_err[1] = (cascade_value->quaternion.parameter.acc_err[1] > acc_err_limit) ? acc_err_limit :
                                                      (cascade_value->quaternion.parameter.acc_err[1] < -acc_err_limit) ? -acc_err_limit :
                                                      cascade_value->quaternion.parameter.acc_err[1];
    cascade_value->quaternion.parameter.acc_err[2] = (cascade_value->quaternion.parameter.acc_err[2] > acc_err_limit) ? acc_err_limit :
                                                      (cascade_value->quaternion.parameter.acc_err[2] < -acc_err_limit) ? -acc_err_limit :
                                                      cascade_value->quaternion.parameter.acc_err[2];

    // 用误差校正陀螺仪数据（比例+积分校正）
    x += kp * ex + ki * cascade_value->quaternion.parameter.acc_err[0];
    y += kp * ey + ki * cascade_value->quaternion.parameter.acc_err[1];
    z += kp * ez + ki * cascade_value->quaternion.parameter.acc_err[2];

    // 四元数微分方程更新（根据陀螺仪角速度）
    cascade_value->quaternion.pro.qua[0] += ((-q1 * x - q2 * y - q3 * z) * cascade_value->posture_value.call_cycle / 2.0f);
    cascade_value->quaternion.pro.qua[1] += (( q0 * x + q2 * z - q3 * y) * cascade_value->posture_value.call_cycle / 2.0f);
    cascade_value->quaternion.pro.qua[2] += (( q0 * y - q1 * z + q3 * x) * cascade_value->posture_value.call_cycle / 2.0f);
    cascade_value->quaternion.pro.qua[3] += (( q0 * z + q1 * y - q2 * x) * cascade_value->posture_value.call_cycle / 2.0f);

    // 取出当前四元数 (w, x, y, z)
    q0 = cascade_value->quaternion.pro.qua[0];
    q1 = cascade_value->quaternion.pro.qua[1];
    q2 = cascade_value->quaternion.pro.qua[2];
    q3 = cascade_value->quaternion.pro.qua[3];

    // 计算四元数各分量平方（减少重复计算）
    float q0_2 = q0 * q0;
    float q1_2 = q1 * q1;
    float q2_2 = q2 * q2;
    float q3_2 = q3 * q3;

    // 四元数归一化（避免数值漂移）
    length = sqrt(q0_2 + q1_2 + q2_2 + q3_2);  // 计算模长
    if (length > 0.001f)  // 模长有效时才归一化
    {
        cascade_value->quaternion.pro.qua[0] /= length;
        cascade_value->quaternion.pro.qua[1] /= length;
        cascade_value->quaternion.pro.qua[2] /= length;
        cascade_value->quaternion.pro.qua[3] /= length;
    }

    // 根据四元数计算旋转矩阵（用于后续姿态角计算）
    cascade_value->quaternion.data.rot_mat[0][0] = q0_2 + q1_2 - q2_2 - q3_2;
    cascade_value->quaternion.data.rot_mat[0][1] = 2 * (q1 * q2 + q0 * q3);
    cascade_value->quaternion.data.rot_mat[0][2] = 2 * (q1 * q3 - q0 * q2);
    cascade_value->quaternion.data.rot_mat[1][0] = 2 * (q1 * q2 - q0 * q3);
    cascade_value->quaternion.data.rot_mat[1][1] = q0_2 - q1_2 + q2_2 - q3_2;
    cascade_value->quaternion.data.rot_mat[1][2] = 2 * (q2 * q3 + q0 * q1);
    cascade_value->quaternion.data.rot_mat[2][0] = 2 * (q1 * q3 + q0 * q2);
    cascade_value->quaternion.data.rot_mat[2][1] = 2 * (q2 * q3 - q0 * q1);
    cascade_value->quaternion.data.rot_mat[2][2] = q0_2 - q1_2 - q2_2 + q3_2;

    // 根据旋转矩阵计算姿态角（横滚角、俯仰角、偏航角）
    cascade_value->posture_value.rol = arctan2(cascade_value->quaternion.data.rot_mat[2][2], cascade_value->quaternion.data.rot_mat[1][2]);  // 横滚角
    cascade_value->posture_value.pit = -arcsin(cascade_value->quaternion.data.rot_mat[0][2]);                                            // 俯仰角
    cascade_value->posture_value.yaw = arctan2(cascade_value->quaternion.data.rot_mat[0][0], cascade_value->quaternion.data.rot_mat[0][1]);   // 偏航角
}

// 函数简介    PID闭环计算
// 参数说明   pid_cycle        PID参数结构体
// 参数说明   target           目标值
// 参数说明   real             当前值
// 返回参数   void
// 使用示例   pid_control(&roll_balance_cascade.speed_cycle, 0, (left_motor.encoder_data + right_motor.encoder_data) / 2);
// 备注信息
void pid_control (pid_cycle_struct *pid_cycle, float target, float real)
{
    float    proportion_value    = 0;          // 比例量
    float    differential_value  = 0;          // 微分量

    proportion_value = target - real;          // 比例量 = 目标值 - 实际值

    pid_cycle->i_value += (proportion_value * pid_cycle->i_value_pro);  // 积分量 = 积分量 + 比例量 * 积分程度

    pid_cycle->i_value = func_limit_ab(pid_cycle->i_value, -pid_cycle->i_value_max, pid_cycle->i_value_max);  // 积分量限幅

    differential_value = proportion_value - pid_cycle->p_value_last;  // 微分量 = 比例量 - 上一次比例量

    pid_cycle->out = (pid_cycle->p * proportion_value + pid_cycle->i * pid_cycle->i_value + pid_cycle->d * differential_value);  // PID拟合

    pid_cycle->out = func_limit_ab(pid_cycle->out, -pid_cycle->out_max, pid_cycle->out_max);  // PID输出限幅

    pid_cycle->p_value_last = proportion_value;        // 保存比例量
}

// 函数简介    PID闭环计算(增量式)
// 参数说明   pid_cycle        PID参数结构体
// 参数说明   target           目标值
// 参数说明   real             当前值
// 返回参数   void
// 使用示例   pid_control_incremental(&roll_balance_cascade.speed_cycle, 0, (left_motor.encoder_data + right_motor.encoder_data) / 2);
// 备注信息
void pid_control_incremental (pid_cycle_struct *pid_cycle, float target, float real)
{
    float    proportion_value    = 0,          // 比例量
             differential_value  = 0;          // 微分量

    pid_cycle->i_value = target - real;          // 积分量 = 目标值 - 实际值  增量式PID P ---> I

    differential_value = proportion_value - 2 * pid_cycle->incremental_data[0] - pid_cycle->incremental_data[1];  // 微分量 增量式PID I ---> D

    proportion_value  = proportion_value - pid_cycle->incremental_data[0];  // 比例量 增量式PID D ---> P

    pid_cycle->incremental_data[1] = pid_cycle->incremental_data[0];          // 增量式PID 保存参数

    pid_cycle->incremental_data[0] = proportion_value;

    pid_cycle->out += (pid_cycle->p * proportion_value + pid_cycle->i * pid_cycle->i_value + pid_cycle->d * differential_value);  // PID拟合

    pid_cycle->out = func_limit_ab(pid_cycle->out, -pid_cycle->out_max, pid_cycle->out_max);          // PID输出限幅
}

// 函数简介    初始化四元数模块
// 返回参数   void
// 使用示例   quaternion_module_init(&quaternion);
// 备注信息   初始化四元数为单位四元数，姿态角为 0，加速度滤波初始值为当前加速度数据，重置姿态角亦可调用此函数
void quaternion_module_init(cascade_value_struct *cascade_value)
{
    // 初始化四元数为单位四元数 (w=1, x=y=z=0)
    cascade_value->quaternion.pro.qua[0] = 1.0f;
    cascade_value->quaternion.pro.qua[1] = 0.0f;
    cascade_value->quaternion.pro.qua[2] = 0.0f;
    cascade_value->quaternion.pro.qua[3] = 0.0f;

    // 初始化姿态角为 0 度
    cascade_value->posture_value.yaw = 0.0f;
    cascade_value->posture_value.rol = 0.0f;
    cascade_value->posture_value.pit = 0.0f;

    // 初始化加速度滤波值为当前加速度数据（g 为单位）
    cascade_value->quaternion.pro.acc_filtered[0] = (float)ACC_DATA_X / ACC_TRANSITION_FACTOR;
    cascade_value->quaternion.pro.acc_filtered[1] = (float)ACC_DATA_Y / ACC_TRANSITION_FACTOR;
    cascade_value->quaternion.pro.acc_filtered[2] = (float)ACC_DATA_Z / ACC_TRANSITION_FACTOR;

    // 初始化误差积分为 0
    cascade_value->quaternion.parameter.acc_err[0] = 0.0f;
    cascade_value->quaternion.parameter.acc_err[1] = 0.0f;
    cascade_value->quaternion.parameter.acc_err[2] = 0.0f;
}

// 函数简介    串级平衡控制初始化
// 返回参数   void
// 使用示例   balance_cascade_init();
// 备注信息   初始化平衡控制结构体参数，包括姿态校准系数、PID 各环节参数（P/I/D、限幅等）
//            并保存初始状态到备份结构体，最后初始化四元数模块
void balance_cascade_init (void)
{
    // 初始化俯仰平衡控制的姿态参数
    roll_balance_cascade.posture_value.call_cycle        = 0.001;      // 调用周期 0.001s (1ms)
    roll_balance_cascade.posture_value.mechanical_zero  = -4.0f;      // 机械零点初始化为 0
    roll_balance_cascade.posture_value.correct_kp        = 0.4f;       // 姿态校准比例系数 0.4
    roll_balance_cascade.posture_value.correct_ki        = 0.015f;     // 姿态校准积分系数 0.015

    // 初始化俯仰平衡控制的角速度环 PID 限幅参数
    roll_balance_cascade.angular_speed_cycle.i_value_max     = 1000;      // 角速度环积分上限
    roll_balance_cascade.angular_speed_cycle.i_value_pro    = 0.1f;       // 角速度环积分比例
    roll_balance_cascade.angular_speed_cycle.out_max        = 10000;      // 角速度环输出上限

    // 初始化俯仰平衡控制的角度环 PID 限幅参数
    roll_balance_cascade.angle_cycle.i_value_max        = 1000;      // 角度环积分上限
    roll_balance_cascade.angle_cycle.i_value_pro         = 2.0f;       // 角度环积分比例
    roll_balance_cascade.angle_cycle.out_max            = 10000;      // 角度环输出上限

    // 初始化俯仰平衡控制的速度环 PID 限幅参数
    roll_balance_cascade.speed_cycle.i_value_max        = 500;      // 速度环积分上限
    roll_balance_cascade.speed_cycle.i_value_pro         = 0.005f;   // 速度环积分比例
    roll_balance_cascade.speed_cycle.out_max            = 2000;      // 速度环输出上限

    // 俯仰平衡控制各环 PID 的 P/I/D 系数
    roll_balance_cascade.angular_speed_cycle.p    = 1.1f;      // 角速度环 P
    roll_balance_cascade.angular_speed_cycle.i    = 0.0f;      // 角速度环 I
    roll_balance_cascade.angular_speed_cycle.d    = 0.0f;      // 角速度环 D

    roll_balance_cascade.angle_cycle.p    = 700.0f;      // 角度环 P
    roll_balance_cascade.angle_cycle.i    = 1.0f;        // 角度环 I
    roll_balance_cascade.angle_cycle.d    = 50.0f;       // 角度环 D

    roll_balance_cascade.speed_cycle.p    = 5.0f;      // 速度环 P
    roll_balance_cascade.speed_cycle.i    = 0.0f;      // 速度环 I
    roll_balance_cascade.speed_cycle.d    = 0.0f;      // 速度环 D

    // 保存俯仰平衡控制初始参数到备份结构体
    memcpy(&roll_balance_cascade_resave, &roll_balance_cascade, sizeof(roll_balance_cascade_resave));
    // 初始化俯仰平衡控制的四元数模块
    quaternion_module_init(&roll_balance_cascade);

    // 初始化横滚平衡控制的姿态参数
    pitch_balance_cascade.posture_value.call_cycle        = 0.001;      // 调用周期 0.001s (1ms)
    pitch_balance_cascade.posture_value.mechanical_zero  = 0.0f;       // 机械零点初始化为 0
    pitch_balance_cascade.posture_value.correct_kp        = 0.4f;       // 姿态校准比例系数 0.4
    pitch_balance_cascade.posture_value.correct_ki        = 0.015f;     // 姿态校准积分系数 0.015

    // 初始化横滚平衡控制的角速度环 PID 限幅参数
    pitch_balance_cascade.angular_speed_cycle.i_value_max     = 1000;      // 角速度环积分上限
    pitch_balance_cascade.angular_speed_cycle.i_value_pro    = 0.3f;       // 角速度环积分比例
    pitch_balance_cascade.angular_speed_cycle.out_max        = 10000;      // 角速度环输出上限

    // 初始化横滚平衡控制的角度环 PID 限幅参数
    pitch_balance_cascade.angle_cycle.i_value_max        = 300;      // 角度环积分上限
    pitch_balance_cascade.angle_cycle.i_value_pro         = 0.8f;       // 角度环积分比例
    pitch_balance_cascade.angle_cycle.out_max            = 300;      // 角度环输出上限

    // 初始化横滚平衡控制的速度环 PID 限幅参数
    pitch_balance_cascade.speed_cycle.i_value_max        = 4000;      // 速度环积分上限
    pitch_balance_cascade.speed_cycle.i_value_pro         = 0.05f;   // 速度环积分比例
    pitch_balance_cascade.speed_cycle.out_max            = 1500;      // 速度环输出上限

    // 横滚平衡控制各环 PID 的 P/I/D 系数
    pitch_balance_cascade.angular_speed_cycle.p    = 0.0f;      // 角速度环 P
    pitch_balance_cascade.angular_speed_cycle.i    = 0.0f;      // 角速度环 I
    pitch_balance_cascade.angular_speed_cycle.d    = 0.0f;      // 角速度环 D

    pitch_balance_cascade.angle_cycle.p    = 0.0f;      // 角度环 P
    pitch_balance_cascade.angle_cycle.i    = 1.0f;        // 角度环 I
    pitch_balance_cascade.angle_cycle.d    = 0.0f;       // 角度环 D

    pitch_balance_cascade.speed_cycle.p    = 0.0f;      // 速度环 P
    pitch_balance_cascade.speed_cycle.i    = 0.0f;      // 速度环 I
    pitch_balance_cascade.speed_cycle.d    = 0.0f;      // 速度环 D

    // 保存横滚平衡控制初始参数到备份结构体
    memcpy(&pitch_balance_cascade_resave, &pitch_balance_cascade, sizeof(pitch_balance_cascade_resave));
}
