#ifndef _BALANCE_CONTROL_H_
#define _BALANCE_CONTROL_H_

#include "zf_common_headfile.h"

// 陀螺仪数据转换（原始数据转换）
#define GYRO_DATA_X              ( imu660ra_gyro_x)        // 陀螺仪 X 轴原始数据
#define GYRO_DATA_Y              (-imu660ra_gyro_y)        // 陀螺仪 Y 轴原始数据（带方向转换）
#define GYRO_DATA_Z              (-imu660ra_gyro_z)        // 陀螺仪 Z 轴原始数据（带方向转换）
#define GYRO_TRANSITION_FACTOR   (16.384f)                 // 陀螺仪数据转换系数（LSB/(°/s)）

// 加速度计数据转换
#define ACC_DATA_X               ( imu660ra_acc_x)         // 加速度计 X 轴原始数据
#define ACC_DATA_Y               (-imu660ra_acc_y)         // 加速度计 Y 轴原始数据（带方向转换）
#define ACC_DATA_Z               (-imu660ra_acc_z)         // 加速度计 Z 轴原始数据（带方向转换）
#define ACC_TRANSITION_FACTOR    (4096.0f)                 // 加速度计数据转换系数（LSB/g）

#define ACC_GRAVITY               (9.80665f)               // 重力加速度参考值（m/s2）

typedef struct quaternion_data
{
    float rot_mat[3][3];                                  // 旋转矩阵
} quaternion_data;

typedef struct quaternion_process
{
    float qua[4];                                         // 四元数数据（w, x, y, z 顺序）
    float acc_filtered[3];                                // 滤波后的加速度（单位：g）
} quaternion_process;

typedef struct quaternion_parameter
{
    float acc_err[3];                                    // 加速度计校准误差值
} quaternion_parameter;

typedef struct quaternion_module
{
    quaternion_process pro;                              // 四元数处理数据
    quaternion_data data;                                // 四元数计算结果数据
    quaternion_parameter parameter;                      // 四元数校准参数
} quaternion_module;

typedef struct
{
    float p;                                             // PID 控制器比例项 P
    float i;                                             // PID 控制器积分项 I
    float d;                                             // PID 控制器微分项 D
    float p_value_last;                                  // 上一次偏差值
    float i_value;                                       // PID 积分值
    float i_value_pro;                                   // PID 积分值的比例（范围 0 - 1，用于限制积分增长速度）
    float i_value_max;                                   // PID 积分值上限
    float out;                                           // PID 控制器输出值
    float out_max;                                       // PID 输出值上限
    float incremental_data[2];                           // 增量式 PID 的偏差历史数据
} pid_cycle_struct;

typedef struct
{
    float correct_kp;                                    // 姿态校准比例系数（范围 0.1 - 0.5）
    float correct_ki;                                    // 姿态校准积分系数（范围 0.001 - 0.01）
    float call_cycle;                                    // 姿态滤波算法的调用周期（单位：s（秒））
    float mechanical_zero;                               // 机械零点
    float yaw;                                           // 偏航角（单位：度）
    float rol;                                           // 横滚角（单位：度）
    float pit;                                           // 俯仰角（单位：度）
} cascade_common_value_struct;

// 级联控制结构体
typedef struct
{
    quaternion_module          quaternion;               // 四元数相关数据
    cascade_common_value_struct posture_value;          // 动态姿态数据
    pid_cycle_struct           angular_speed_cycle;      // 角速度环控制器
    pid_cycle_struct           angle_cycle;              // 角度环控制器
    pid_cycle_struct           speed_cycle;              // 速度环控制器
} cascade_value_struct;

extern cascade_value_struct roll_balance_cascade;         // 俯仰平衡控制参数结构体
extern cascade_value_struct roll_balance_cascade_resave;  // 俯仰平衡控制参数结构体初始备份
extern cascade_value_struct pitch_balance_cascade;        // 横滚平衡参数级联结构体
extern cascade_value_struct pitch_balance_cascade_resave; // 横滚平衡参数级联结构体初始备份

// 函数简介    四元数模块计算（更新姿态数据）
// 参数说明   cascade_value - 级联控制结构体指针（存储四元数及姿态数据）
// 返回参数   void
// 使用示例   quaternion_module_calculate(&balance_cascade);
// 备注信息   该函数通过融合陀螺仪和加速度计数据，更新四元数及旋转矩阵，最终计算出姿态角（横滚角、俯仰角、偏航角）
void quaternion_module_calculate(cascade_value_struct *cascade_value);

// 函数简介    位置式 PID 控制计算
// 参数说明   pid_cycle - PID 控制器结构体指针；target - 目标值；real - 实际测量值
// 返回参数   void
// 使用示例   pid_control(&balance_cascade.speed_cycle, 0, (left_enc + right_enc) / 2);
// 备注信息   计算位置式 PID 的输出，包含比例、积分、微分环节，并对积分和输出进行限幅
void pid_control (pid_cycle_struct *pid_cycle, float target, float real);

// 函数简介    增量式 PID 控制计算
// 参数说明   pid_cycle - PID 控制器结构体指针；target - 目标值；real - 实际测量值
// 返回参数   void
// 使用示例   pid_control_incremental(&balance_cascade.angle_cycle, 0, balance_cascade.posture_value.pit);
// 备注信息   计算增量式 PID 的输出，通过偏差的变化量计算控制量增量，累加后作为输出并限幅
void pid_control_incremental (pid_cycle_struct *pid_cycle, float target, float real);

// 函数简介    四元数模块初始化
// 参数说明   cascade_value - 级联控制结构体指针（需要初始化的四元数模块）
// 返回参数   void
// 使用示例   quaternion_module_init(&balance_cascade);
// 备注信息   初始化四元数为单位四元数，姿态角为 0，加速度滤波初始值设为当前加速度计数据，校准误差清零
void quaternion_module_init (cascade_value_struct *cascade_value);

// 函数简介    平衡级联控制初始化
// 返回参数   void
// 使用示例   balance_cascade_init();
// 备注信息   初始化平衡控制和转向平衡控制的级联结构体参数，包括姿态校准系数、PID 参数等，并保存初始状态
void balance_cascade_init (void);

#endif
