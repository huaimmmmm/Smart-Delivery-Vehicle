#include "zf_libraries_headfile.h"
#include "zf_device_ips200.h"
#include "zf_device_gnss.h"
#include <stdio.h>

// ==============================================================================
// [1. 系统状态枚举与全局类型定义]
// ==============================================================================
typedef enum {
    STATE_IDLE = 0,         // 待机状态：锁定当前航向，等待指令
    STATE_TEACHING,         // 示教模式：允许手动推车或打点记录坐标
    STATE_NAVIGATING        // 导航模式：闭环自动寻路
} SystemState;

// ==============================================================================
// [2. 全局变量定义]
// ==============================================================================

// --- 系统与交互状态 ---
SystemState current_state = STATE_IDLE; // 系统当前运行状态，上电默认待机
char ui_prompt[30] = "System Ready. ";  // UI 状态提示字符缓存
uint8 key_c7_hold = 0, key_c8_hold = 0, key_c9_hold = 0, key_c10_hold = 0; // 按键长按状态标志
uint8 key_c7_click = 0, key_c8_click = 0, key_c9_click = 0, key_c10_click = 0; // 按键单击触发标志 (需手动清零)

// --- 传感器基础反馈数据 ---
int16 left_pulse = 0, right_pulse = 0;  // 左右轮实际编码器脉冲数反馈
float gyro_z_offset = 0.0f;             // 陀螺仪Z轴零偏漂移值
float yaw_angle = 0.0f;                 // 当前偏航角 (基于陀螺仪积分)
float real_dt = 0.02f;                  // 硬件定时器测得的单次循环周期 (单位: s)
uint32 last_cycles = 0;                 // 上一帧 DWT 计数器快照

// --- 导航与定位解算参数 ---
uint8  has_target = 0;                  // 目标坐标有效标志位 (1: 有效, 0: 无效)
double target_lat = 0.0, target_lon = 0.0; // 目标点绝对经纬度坐标
double nav_distance = 0.0;              // 距目标点直线距离 (单位: m)
double nav_azimuth = 0.0;               // 目标点绝对方位角
double nav_yaw_error = 0.0;             // 航向误差角 (正数右偏，负数左偏)
float  target_yaw = 0.0f;               // 闭环控制的目标航向角
float  turn_Kp = 0.3f, turn_Kd = 0.8f;  // 转向外环 PD 控制参数
float  last_nav_yaw_error = 0.0f;       // 上一帧航向误差 (用于微分计算)

// --- 底盘运动控制指令 ---
int left_target_cmd = 0, right_target_cmd = 0;               // 左右轮期望速度
float left_target_smooth = 0.0f, right_target_smooth = 0.0f; // 经过斜坡缓冲的期望速度
float step_limit = 0.8f;                                     // 斜坡函数步长限制，控制加减速平滑度

// --- 底盘内环速度 PID 参数 ---
float Kp = 150.0, Ki = 15.0, Kd = 20.0;                    // 速度环 PID 参数
int left_last_err = 0, left_integral = 0;                  // 左轮 PID 历史误差与积分器
int right_last_err = 0, right_integral = 0;                // 右轮 PID 历史误差与积分器
int32 left_pwm = 0, right_pwm = 0;                         // 最终输出到电机的 PWM 占空比

// --- 云台控制参数 ---
float ptz_pan_angle = 0.0f;             // 云台水平角度 (Pan: -135.0 ~ 135.0)
float ptz_tilt_angle = 0.0f;            // 云台俯仰角度 (Tilt: -90.0 ~ 90.0)
int32 ptz_center_duty = 750;            // 舵机物理中位脉宽 (默认 1500us 对应 750)

// --- 视觉通信与追踪控制 ---
uint8  vision_type = 0;          // 目标状态标志 (0:丢失, 1:发现色块, 2:距离满足, 4:核验成功)
int8   vision_x_err = 0;         // 视觉画面 X 轴像素偏差
int8   vision_y_err = 0;         // 视觉画面 Y 轴像素偏差
uint8  vision_distance = 0;      // 目标估算距离 (cm)

uint8  ptz_auto_mode = 0;        // 云台自动追踪模式使能标志位
uint8  mission_completed = 0;    // 整体任务完成标志位 (阻断重复触发)
uint8  gps_arrived_flag = 0;     // GPS 导航完成触发标志 (触发视觉模块接管)

// 云台追踪专用 PD 参数
float ptz_Kp = 0.08f;            // 云台追踪比例系数
float ptz_Kd = 0.03f;            // 云台追踪微分系数
int8  last_vision_x_err = 0;
int8  last_vision_y_err = 0;

// ==============================================================================
// [3. 函数声明]
// ==============================================================================
void System_Init(void);
void Sensor_Update(void);
void Navigation_Task(void);
void Chassis_Task(void);
void UI_Update(void);

void set_left_motor(int32 pwm);
void set_right_motor(int32 pwm);
void pit_handler_10ms(uint32 event, void *ptr);
int32 calculate_motor_pwm(float kp_val, float ki_val, float kd_val, int target_cmd, float target_smooth, int16 real_pulse, int *history_integral, int *history_last_err, int32 current_pwm);

void PTZ_Init(void);
void PTZ_Set_Angle(float pan_deg, float tilt_deg);

void uart2_rx_callback(uint32 event, void *ptr);
void Vision_Track_Task(void);

// ==============================================================================
// [4. 主函数循环]
// ==============================================================================
int main (void)
{
    System_Init();

    for( ; ; )
    {
        Sensor_Update();        // 传感器数据采集与状态更新

        Navigation_Task();      // GPS 导航控制逻辑
        Vision_Track_Task();    // 视觉追踪与云台控制逻辑

        Chassis_Task();         // 底盘速度解算与执行
        UI_Update();            // 刷新屏幕显示

        zf_delay_ms(20);        // 维持 50Hz 控制周期
    }
}

// ==============================================================================
// [5. 具体函数实现]
// ==============================================================================

// ------------------------------------------------------------------------------
// 系统基础外设初始化
// ------------------------------------------------------------------------------
void System_Init(void)
{
    zf_system_clock_init(SYSTEM_CLOCK_300M);
    debug_init();

    // DWT 硬件周期计数器初始化
    *(volatile uint32 *)0xE000EDFC |= 0x01000000;
    *(volatile uint32 *)0xE0001004 = 0;
    *(volatile uint32 *)0xE0001000 |= 1;

    zf_delay_ms(100);
    printf("\r\n--- System Booting ---\r\n");

    // GPIO 与驱动 PWM 初始化
    zf_gpio_init(D9, GPO_PUSH_PULL, GPIO_LOW); zf_gpio_init(D8, GPO_PUSH_PULL, GPIO_LOW);
    zf_gpio_init(B1, GPO_PUSH_PULL, GPIO_LOW); zf_gpio_init(C5, GPO_PUSH_PULL, GPIO_LOW);
    zf_gpio_init(C7, GPI_PULL_UP, GPIO_HIGH);  zf_gpio_init(C8, GPI_PULL_UP, GPIO_HIGH);
    zf_gpio_init(C9, GPI_PULL_UP, GPIO_HIGH);  zf_gpio_init(C10,GPI_PULL_UP, GPIO_HIGH);

    zf_pwm_module_init(PWM_TIM3, PWM_ALIGNMENT_EDGE, 13000);
    zf_pwm_channel_init(PWM_TIM3_CH2_E2, 0);
    zf_pwm_channel_init(PWM_TIM3_CH1_E4, 0);

    // 状态指示灯初始化 (默认熄灭)
    zf_pwm_channel_init(PWM_TIM3_CH3_H0, PWM_DUTY_MAX);

    // 编码器与定时器中断初始化
    zf_encoder_init(ENCODER_TIM5, ENCODER_MODE_QUADRATURE, ENCODER_TIM5_A_PLUS_I1, ENCODER_TIM5_B_DIR_I3);
    zf_encoder_init(ENCODER_TIM8, ENCODER_MODE_QUADRATURE, ENCODER_TIM8_A_PLUS_I0, ENCODER_TIM8_B_DIR_I2);
    zf_pit_ms_init(PIT_TIM1, 10, pit_handler_10ms, NULL);

    // 屏幕初始化及静态 UI 框架绘制
    printf("Init IPS200 Display...\r\n");
    ips200_init(IPS200_TYPE_SPI);
    ips200_clear();

    ips200_show_string(0,  32, "Yaw   :");
    ips200_show_string(0,  48, "T_Yaw :");
    ips200_show_string(0,  64, "N_Err :");
    ips200_show_string(0,  80, "G_Dist:");
    ips200_show_string(0,  96, "----------------");

    ips200_show_string(0, 112, "P_Ang :");
    ips200_show_string(0, 128, "T_Ang :");
    ips200_show_string(0, 144, "V_Dist:");
    ips200_show_string(0, 160, "----------------");

    ips200_show_string(0, 176, "L_Spd :");
    ips200_show_string(0, 192, "R_Spd :");

    ips200_show_string(0, 208, "-- GNSS Info --");
    ips200_show_string(0, 224, "State :");
    ips200_show_string(0, 240, "Lon   :");
    ips200_show_string(0, 256, "Lat   :");
    ips200_show_string(0, 272, "Sats  :");
    ips200_show_string(96, 272, "TGT:");

    // 云台 PWM 初始化 (50Hz)
    zf_pwm_module_init(PWM_TIM4,  PWM_ALIGNMENT_EDGE, 50);
    zf_pwm_module_init(PWM_TIM16, PWM_ALIGNMENT_EDGE, 50);
    PTZ_Init();

    // 视觉通信串口初始化
    zf_uart_init(UART_2, 115200, UART2_TX_A7, UART2_RX_A6);
    zf_uart_set_interrupt_callback(UART_2, uart2_rx_callback, NULL);
    zf_uart_set_interrupt_config(UART_2, UART_INTERRUPT_CONFIG_RX_ENABLE);

    // 传感器初始化与零偏校准
    printf("Init GNSS Module...\r\n");
    gnss_init(GNSS_TYPE_TAU1201);

    printf("Init IMU660RB...\r\n");
    imu660rb_init();

    printf("\r\n>>> Calibrating IMU, please keep the vehicle static for 1s <<<\r\n");
    imu660rb_measurement_data_struct temp_raw;
    imu660rb_physical_data_struct temp_phys;
    for(int i = 0; i < 50; i++) {
        imu660rb_get_gyro(&temp_raw);
        imu660rb_get_physical_gyro(&temp_raw, IMU_GYRO_RANGE_DEFAULT, &temp_phys);
        gyro_z_offset += temp_phys.z;
        zf_delay_ms(20);
    }
    gyro_z_offset /= 50.0f;

    last_cycles = *(volatile uint32 *)0xE0001004;
    printf("Ready! PID Closed-Loop System Booted.\r\n");
}

// ------------------------------------------------------------------------------
// UART2 接收中断回调函数 (解析 OpenMV 数据帧)
// ------------------------------------------------------------------------------
void uart2_rx_callback(uint32 event, void *ptr)
{
    (void)event;
    (void)ptr;

    uint8 rx_byte;
    static uint8 state = 0;
    static uint8 buffer[8];
    static uint8 sum = 0;

    while(zf_uart_query_byte(UART_2, &rx_byte) == ZF_NO_ERROR)
    {
        switch(state)
        {
            case 0: if(rx_byte == 0xFF) { state = 1; sum = 0; } break; // Frame Header 1
            case 1: if(rx_byte == 0xFE) { state = 2; } else { state = 0; } break; // Frame Header 2
            case 2: buffer[2] = rx_byte; sum += rx_byte; state = 3; break; // Target Type
            case 3: buffer[3] = rx_byte; sum += rx_byte; state = 4; break; // X Err
            case 4: buffer[4] = rx_byte; sum += rx_byte; state = 5; break; // Y Err
            case 5: buffer[5] = rx_byte; sum += rx_byte; state = 6; break; // Distance
            case 6: buffer[6] = rx_byte; sum += rx_byte; state = 7; break; // Reserved
            case 7:
                if(sum == rx_byte) // Checksum Verify
                {
                    vision_type     = buffer[2];
                    vision_x_err    = (int8)buffer[3];
                    vision_y_err    = (int8)buffer[4];
                    vision_distance = buffer[5];
                }
                state = 0;
                break;
            default: state = 0; break;
        }
    }
}

// ------------------------------------------------------------------------------
// 视觉跟随与云台伺服逻辑
// ------------------------------------------------------------------------------
void Vision_Track_Task(void)
{
    static float scan_dir = 1.0f;
    static float scan_speed = 1.5f;

    static uint32 led_timer = 0;
    static uint8 sweep_count = 0;
    static uint32 spin_timer = 0;

    // 触发条件：按键 C9 触发或 GPS 抵达目标点
    if (key_c9_click || gps_arrived_flag) {

        key_c9_click = 0;
        gps_arrived_flag = 0;

        ptz_auto_mode = !ptz_auto_mode;

        mission_completed = 0;
        vision_type = 0;
        led_timer = 0;
        sweep_count = 0;
        spin_timer = 0;

        if (ptz_auto_mode) {
            sprintf(ui_prompt, "%-20s", "Vision Track: ON ");
        } else {
            sprintf(ui_prompt, "%-20s", "Vision Track: OFF");
        }
    }

    // 任务完成保护逻辑
    if (mission_completed == 1) {
        left_target_cmd = 0;
        right_target_cmd = 0;
        target_yaw = yaw_angle;

        PTZ_Set_Angle(0.0f, 0.0f);

        if (led_timer > 0) {
            zf_pwm_set_duty(PWM_TIM3_CH3_H0, 0); // 点亮蓝灯
            led_timer--;
        } else {
            zf_pwm_set_duty(PWM_TIM3_CH3_H0, PWM_DUTY_MAX); // 熄灭蓝灯
            ptz_auto_mode = 0;
            mission_completed = 0;
            sprintf(ui_prompt, "%-20s", "Vision Track: OFF");
        }
        return;
    }

    if (ptz_auto_mode == 1)
    {
        if (vision_type == 1)
        {
            // 目标追踪状态
            sweep_count = 0;
            spin_timer = 0;

            float pan_comp = (vision_x_err * ptz_Kp) + ((vision_x_err - last_vision_x_err) * ptz_Kd);
            float tilt_comp = (vision_y_err * ptz_Kp) + ((vision_y_err - last_vision_y_err) * ptz_Kd);

            last_vision_x_err = vision_x_err;
            last_vision_y_err = vision_y_err;

            float target_pan = ptz_pan_angle + pan_comp;
            float target_tilt = ptz_tilt_angle + tilt_comp;
            PTZ_Set_Angle(target_pan, target_tilt);

            // 底盘联动逻辑
            float chassis_turn_Kp = 0.2f;
            int turn_comp = (int)(ptz_pan_angle * chassis_turn_Kp);

            if(turn_comp > 15) turn_comp = 15;
            if(turn_comp < -15) turn_comp = -15;

            int base_speed = 0;
            if (vision_distance > 40) {
                base_speed = 8;
            } else if (vision_distance < 20 && vision_distance > 0) {
                base_speed = -6;
            } else {
                base_speed = 0;
            }

            left_target_cmd = base_speed + turn_comp;
            right_target_cmd = base_speed - turn_comp;
            target_yaw = yaw_angle;
        }
        else if (vision_type == 2)
        {
            // 抵近目标状态
            left_target_cmd = 0;
            right_target_cmd = 0;
            target_yaw = yaw_angle;

            PTZ_Set_Angle(0.0f, -30.0f);
            sprintf(ui_prompt, "%-20s", "Look for Face...");
        }
        else if (vision_type == 4)
        {
            // 核验成功状态
            sprintf(ui_prompt, "%-20s", "Mission Complete! ");
            mission_completed = 1;
            led_timer = 150;
        }
        else
        {
            // 目标丢失，全局搜索状态
            last_vision_x_err = 0;
            last_vision_y_err = 0;

            if (spin_timer > 0)
            {
                // 底盘原地旋转搜索
                PTZ_Set_Angle(0.0f, 0.0f);
                left_target_cmd = 8;
                right_target_cmd = -8;

                spin_timer--;
                if (spin_timer == 0) {
                    sweep_count = 0;
                    target_yaw = yaw_angle;
                }
            }
            else
            {
                // 云台水平扫描
                float next_pan = ptz_pan_angle + (scan_dir * scan_speed);
                if (next_pan >= 90.0f) {
                    next_pan = 90.0f;
                    scan_dir = -1.0f;
                    sweep_count++;
                }
                else if (next_pan <= -90.0f) {
                    next_pan = -90.0f;
                    scan_dir = 1.0f;
                    sweep_count++;
                }

                PTZ_Set_Angle(next_pan, 0.0f);

                left_target_cmd = 0;
                right_target_cmd = 0;
                target_yaw = yaw_angle;

                if (sweep_count >= 4) {
                    spin_timer = 50;
                }
            }
        }

        if (led_timer == 0) zf_pwm_set_duty(PWM_TIM3_CH3_H0, PWM_DUTY_MAX);
    }
    else
    {
        PTZ_Set_Angle(0.0f, 0.0f);
        last_vision_x_err = 0;
        last_vision_y_err = 0;
        sweep_count = 0;
        spin_timer = 0;
        zf_pwm_set_duty(PWM_TIM3_CH3_H0, PWM_DUTY_MAX);
    }
}

// ------------------------------------------------------------------------------
// 传感器采集与数据解算
// ------------------------------------------------------------------------------
void Sensor_Update(void)
{
    // 1. 测算实时控制周期
    uint32 current_cycles = *(volatile uint32 *)0xE0001004;
    real_dt = (float)(current_cycles - last_cycles) / 300000000.0f;
    if(real_dt <= 0.0f || real_dt > 0.1f) { real_dt = 0.02f; }
    last_cycles = current_cycles;

    // 2. 读取编码器速度反馈
    int16 left_pulse_raw = 0, right_pulse_raw = 0;
    zf_encoder_get_count(ENCODER_TIM5, &left_pulse_raw);
    zf_encoder_get_count(ENCODER_TIM8, &right_pulse_raw);
    zf_encoder_clear_count(ENCODER_TIM5);
    zf_encoder_clear_count(ENCODER_TIM8);
    left_pulse = -left_pulse_raw;
    right_pulse = right_pulse_raw;

    // 3. IMU 航向角积分
    imu660rb_measurement_data_struct gyro_raw;
    imu660rb_physical_data_struct gyro_phys;
    imu660rb_get_gyro(&gyro_raw);
    imu660rb_get_physical_gyro(&gyro_raw, IMU_GYRO_RANGE_DEFAULT, &gyro_phys);
    yaw_angle -= (gyro_phys.z - gyro_z_offset) * real_dt;

    // 4. 解析 GNSS 报文
    if(gnss_flag) {
        gnss_flag = 0;
        gnss_data_parse();
    }

    // 5. 解算目标方位角与距离
    if (has_target == 1 && gnss_info.state == 1)
    {
        nav_azimuth = gnss_get_two_points_azimuth(gnss_info.latitude, gnss_info.longitude, target_lat, target_lon);
        nav_distance = gnss_get_two_points_distance(gnss_info.latitude, gnss_info.longitude, target_lat, target_lon);

        if ((nav_azimuth != nav_azimuth) || (nav_distance != nav_distance) || nav_distance > 1000000.0 || nav_distance < 0.0) {
        } else {
            double temp_azim = nav_azimuth;
            if(temp_azim > 180.0) temp_azim -= 360.0;
            nav_yaw_error = temp_azim - yaw_angle;

            // 限制误差在 [-180, 180] 范围内
            int loop_guard = 10;
            while (nav_yaw_error > 180.0 && loop_guard-- > 0)  nav_yaw_error -= 360.0;
            loop_guard = 10;
            while (nav_yaw_error < -180.0 && loop_guard-- > 0) nav_yaw_error += 360.0;
        }
    }
}

// ------------------------------------------------------------------------------
// GPS 导航逻辑状态机
// ------------------------------------------------------------------------------
void Navigation_Task(void)
{
    // 若开启视觉或任务已完成，挂起 GPS 导航逻辑
    if (ptz_auto_mode == 1 || mission_completed == 1) return;

    switch (current_state)
    {
        case STATE_IDLE:
            left_target_cmd = 0; right_target_cmd = 0;
            target_yaw = yaw_angle;

            if (key_c8_click) {
                key_c8_click = 0;
                current_state = STATE_TEACHING;
                sprintf(ui_prompt, "%-20s", "Move & Press C7");
            }
            if (key_c7_click) {
                key_c7_click = 0;
                if (has_target == 1) {
                    yaw_angle = 0.0f; target_yaw = 0.0f; // 重置航向基准
                    current_state = STATE_NAVIGATING;
                    sprintf(ui_prompt, "%-20s", "Navigating...");
                } else {
                    sprintf(ui_prompt, "%-20s", "ERR: No Target!");
                }
            }

            if (key_c10_hold){ left_target_cmd = 10; right_target_cmd = 6; target_yaw = yaw_angle; }
            break;

        case STATE_TEACHING:
            left_target_cmd = 0; right_target_cmd = 0;
            target_yaw = yaw_angle;

            if (key_c10_hold){ left_target_cmd = 10; right_target_cmd = 6; target_yaw = yaw_angle; }

            if (key_c7_click) {
                key_c7_click = 0;
                if (gnss_info.state == 1) {
                    target_lat = gnss_info.latitude;
                    target_lon = gnss_info.longitude;
                    has_target = 1;
                    current_state = STATE_IDLE;
                    sprintf(ui_prompt, "%-20s", "TGT Saved!");
                } else {
                    sprintf(ui_prompt, "%-20s", "ERR: Wait GPS!");
                }
            }
            if (key_c8_click) {
                key_c8_click = 0;
                current_state = STATE_IDLE;
                sprintf(ui_prompt, "%-20s", "Teach Cancelled.");
            }
            break;

        case STATE_NAVIGATING:
            if (key_c8_click) {
                key_c8_click = 0;
                current_state = STATE_IDLE;
                sprintf(ui_prompt, "%-20s", "E-STOP Triggered");
                break;
            }

            if (gnss_info.state == 1 && gnss_info.satellite_used > 4) {
                if (nav_distance > 2.5) {
                    // 外环转向 PD 控制
                    int turn_comp = (int)(nav_yaw_error * turn_Kp + (nav_yaw_error - last_nav_yaw_error) * turn_Kd);
                    last_nav_yaw_error = nav_yaw_error;

                    if (turn_comp > 6)  turn_comp = 6;
                    if (turn_comp < -6) turn_comp = -6;

                    int base_speed = 10 - abs(turn_comp);
                    if(base_speed < 0) base_speed = 0;

                    // 航向偏差过大则原地转向
                    if (abs((int)nav_yaw_error) > 80) {
                        base_speed = 0;
                        turn_comp = (nav_yaw_error > 0) ? 8 : -8;
                    }

                    left_target_cmd  = base_speed + turn_comp;
                    right_target_cmd = base_speed - turn_comp;
                } else {
                    left_target_cmd  = 0; right_target_cmd = 0;
                    current_state = STATE_IDLE;
                    sprintf(ui_prompt, "%-20s", "Arrived Target!");

                    // 抵达目标阈值，置位标志位通知视觉模块
                    gps_arrived_flag = 1;
                }
            } else {
                left_target_cmd  = 0; right_target_cmd = 0;
            }
            break;
    }
}

// ------------------------------------------------------------------------------
// 底盘速度解算与执行
// ------------------------------------------------------------------------------
void Chassis_Task(void)
{
    // 1. 斜坡缓冲限幅限制突变
    if (left_target_smooth < left_target_cmd) {
        left_target_smooth += step_limit;
        if (left_target_smooth > left_target_cmd) left_target_smooth = left_target_cmd;
    } else if (left_target_smooth > left_target_cmd) {
        left_target_smooth -= step_limit;
        if (left_target_smooth < left_target_cmd) left_target_smooth = left_target_cmd;
    }

    if (right_target_smooth < right_target_cmd) {
        right_target_smooth += step_limit;
        if (right_target_smooth > right_target_cmd) right_target_smooth = right_target_cmd;
    } else if (right_target_smooth > right_target_cmd) {
        right_target_smooth -= step_limit;
        if (right_target_smooth < right_target_cmd) right_target_smooth = right_target_cmd;
    }

    // 2. 内环速度 PID 计算
    left_pwm = calculate_motor_pwm(
        Kp, Ki, Kd,
        left_target_cmd, left_target_smooth, left_pulse,
        &left_integral, &left_last_err, left_pwm
    );

    right_pwm = calculate_motor_pwm(
        Kp, Ki, Kd,
        right_target_cmd, right_target_smooth, right_pulse,
        &right_integral, &right_last_err, right_pwm
    );

    // 3. 电机堵转保护逻辑
    static int stall_count = 0;
    if ( (abs(left_pwm) > 3000 && abs(left_pulse) < 2) || (abs(right_pwm) > 3000 && abs(right_pulse) < 2) ) {
        stall_count++;
    } else {
        stall_count = 0;
    }

    if (stall_count > 50) {
        left_pwm = 0; right_pwm = 0;
        left_target_smooth = 0; right_target_smooth = 0;
        left_integral = 0; right_integral = 0;
        sprintf(ui_prompt, "%-20s", "FATAL: Motor Locked");
        current_state = STATE_IDLE;
    }

    // 4. 驱动输出
    set_left_motor(left_pwm);
    set_right_motor(right_pwm);
}

// ------------------------------------------------------------------------------
// 屏幕 UI 刷新逻辑
// ------------------------------------------------------------------------------
void UI_Update(void)
{
    static int print_count = 0;
    if(print_count++ % 5 == 0) // 降采样刷新，避免堵塞总线
    {
        if (current_state == STATE_IDLE)           ips200_show_string(0, 0, "[Mode: IDLE]        ");
        else if (current_state == STATE_TEACHING)  ips200_show_string(0, 0, "[Mode: TEACHING]    ");
        else if (current_state == STATE_NAVIGATING)ips200_show_string(0, 0, "[Mode: NAVIGATING]  ");

        ips200_show_string(0, 16, ui_prompt);

        ips200_show_float(72,  32, yaw_angle,  3, 2);
        ips200_show_float(72,  48, target_yaw, 3, 2);
        ips200_show_float(72,  64, nav_yaw_error, 3, 2);
        ips200_show_float(72,  80, nav_distance, 4, 1);

        ips200_show_float(72, 112, ptz_pan_angle,  4, 1);
        ips200_show_float(72, 128, ptz_tilt_angle, 4, 1);
        ips200_show_uint(72,  144, vision_distance, 3);

        ips200_show_int(72,   176, left_pulse,   3);
        ips200_show_int(72,   192, right_pulse,  3);

        if(gnss_info.state == 1) ips200_show_string(64, 224, "Valid  ");
        else                     ips200_show_string(64, 224, "Wait...");

        ips200_show_float(64, 240, gnss_info.longitude, 3, 6);
        ips200_show_float(64, 256, gnss_info.latitude,  2, 6);
        ips200_show_uint(64,  272, gnss_info.satellite_used, 2);
        ips200_show_uint(144, 272, has_target, 1);
    }
}

// ------------------------------------------------------------------------------
// PID 增量式/位置式混合计算
// ------------------------------------------------------------------------------
int32 calculate_motor_pwm(float kp_val, float ki_val, float kd_val, int target_cmd, float target_smooth, int16 real_pulse, int *history_integral, int *history_last_err, int32 current_pwm)
{
    int err = (int)target_smooth - real_pulse;
    int current_integral = *history_integral;

    // 静态死区消除积分
    if (target_cmd == 0 && target_smooth == 0.0f && err >= -1 && err <= 1) {
        current_integral = 0; err = 0;
    } else {
        // 积分抗饱和处理
        if (current_pwm >= 6000 && err > 0) { /* do nothing */ }
        else if (current_pwm <= -6000 && err < 0) { /* do nothing */ }
        else { current_integral += err; }
    }

    if (current_integral > 300) current_integral = 300;
    else if (current_integral < -300) current_integral = -300;

    int32 output_pwm = (int32)(kp_val * err + ki_val * current_integral + kd_val * (err - *history_last_err));

    *history_last_err = err;
    *history_integral = current_integral;

    if (output_pwm > 6000)  output_pwm = 6000;
    else if (output_pwm < -6000) output_pwm = -6000;

    return output_pwm;
}

// ------------------------------------------------------------------------------
// PIT 定时器中断 (10ms 扫描按键状态)
// ------------------------------------------------------------------------------
void pit_handler_10ms(uint32 event, void *ptr)
{
    (void)event; (void)ptr;
    static uint8 last_c7 = 1, last_c8 = 1, last_c9 = 1, last_c10 = 1;
    uint8 cur_c7 = zf_gpio_get_level(C7), cur_c8 = zf_gpio_get_level(C8);
    uint8 cur_c9 = zf_gpio_get_level(C9), cur_c10= zf_gpio_get_level(C10);

    key_c7_hold = (cur_c7 == 0) ? 1 : 0; key_c8_hold = (cur_c8 == 0) ? 1 : 0;
    key_c9_hold = (cur_c9 == 0) ? 1 : 0; key_c10_hold= (cur_c10== 0) ? 1 : 0;

    if (last_c7 == 1 && cur_c7 == 0) key_c7_click = 1;
    if (last_c8 == 1 && cur_c8 == 0) key_c8_click = 1;
    if (last_c9 == 1 && cur_c9 == 0) key_c9_click = 1;
    if (last_c10== 1 && cur_c10== 0) key_c10_click = 1;

    last_c7 = cur_c7; last_c8 = cur_c8; last_c9 = cur_c9; last_c10= cur_c10;
}

// ------------------------------------------------------------------------------
// 驱动输出函数封装
// ------------------------------------------------------------------------------
void set_left_motor(int32 pwm)
{
    if (pwm > 0)      { zf_gpio_set_level(D9, 0); zf_gpio_set_level(D8, 1); }
    else if (pwm < 0) { zf_gpio_set_level(D9, 1); zf_gpio_set_level(D8, 0); pwm = -pwm; }
    else              { zf_gpio_set_level(D9, 0); zf_gpio_set_level(D8, 0); }
    if (pwm > 6000) pwm = 6000;
    zf_pwm_set_duty(PWM_TIM3_CH2_E2, pwm);
}

void set_right_motor(int32 pwm)
{
    if (pwm > 0)      { zf_gpio_set_level(B1, 1); zf_gpio_set_level(C5, 0); }
    else if (pwm < 0) { zf_gpio_set_level(B1, 0); zf_gpio_set_level(C5, 1); pwm = -pwm; }
    else              { zf_gpio_set_level(B1, 0); zf_gpio_set_level(C5, 0); }
    if (pwm > 6000) pwm = 6000;
    zf_pwm_set_duty(PWM_TIM3_CH1_E4, pwm);
}

void PTZ_Init(void)
{
    zf_pwm_channel_init(PWM_TIM4_CH2_H13, ptz_center_duty);
    zf_pwm_channel_init(PWM_TIM16_CH1_E8, ptz_center_duty);

    ptz_pan_angle = 0.0f;
    ptz_tilt_angle = 0.0f;

    zf_delay_ms(1000);
}

void PTZ_Set_Angle(float pan_deg, float tilt_deg)
{
    if(pan_deg > 135.0f)  pan_deg = 135.0f;
    if(pan_deg < -135.0f) pan_deg = -135.0f;

    if(tilt_deg > 81.0f)  tilt_deg = 81.0f;
    if(tilt_deg < -81.0f) tilt_deg = -81.0f;

    int32 pan_duty = ptz_center_duty - (int32)((pan_deg / 135.0f) * 500);
    int32 tilt_duty = ptz_center_duty + (int32)((tilt_deg / 90.0f) * 500);

    zf_pwm_set_duty(PWM_TIM4_CH2_H13, pan_duty);
    zf_pwm_set_duty(PWM_TIM16_CH1_E8, tilt_duty);

    ptz_pan_angle = pan_deg;
    ptz_tilt_angle = tilt_deg;
}

// ==========================================================================================================
// ======================================= 智能派件车 - 引脚备忘录 =======================================
// ==========================================================================================================
//
// [1. 左侧动力组 (1号 TB6612 驱动模块)]
// 左轮方向控制1 | PD9      | GPO_PUSH_PULL (推挽输出)     | 接 AIN2 & BIN1
// 左轮方向控制2 | PD8      | GPO_PUSH_PULL (推挽输出)     | 接 AIN1 & BIN2
// 左轮速度控制  | PE2      | PWM_TIM3_CH2_E2  (定时器3)   |
//
// [2. 右侧动力组 (2号 TB6612/AT8236 驱动模块)]
// 右轮方向控制1 | PB1      | GPO_PUSH_PULL (推挽输出)     | 接 AIN2 & BIN1
// 右轮方向控制2 | PC5      | GPO_PUSH_PULL (推挽输出)     | 接 AIN1 & BIN2
// 右轮速度控制  | PE4      | PWM_TIM3_CH1_E4  (定时器3)   | 接 PWMA & PWMB
//
// [3. 编码器反馈组]
// 左轮编码器 A相| PI1      | TIM5_CH1 (编码器5)
// 左轮编码器 B相| PI3      | TIM5_CH2 (编码器5)
// 右轮编码器 A相| PI0      | TIM8_CH1 (编码器8)
// 右轮编码器 B相| PI2      | TIM8_CH2 (编码器8)
//
// [4. 二维云台控制组]
// 云台左右 (Pan) | PH13     | PWM_TIM4_CH2_H13 (定时器4)
// 云台上下 (Tilt)| PE8      | PWM_TIM16_CH1_E8 (定时器16)
//
// [5. 人机交互组]
// 独立按键 1~4  | PC7~PC10 | GPI_PULL_UP (上拉输入)       |
// RGB 状态灯    | PH0      | PWM_TIM3_CH3_H0  (定时器3)   |
//
// [6. IMU660RB 六轴姿态传感器 (SPI 模式)]
// SPI 时钟      | PD10     | SPI3_SCK
// SPI 数据输出  | PD12     | SPI3_MOSI
// SPI 数据输入  | PD11     | SPI3_MISO
// SPI 片选      | PD7      | CS Pin
//
// [7. IPS200 2.0寸 显示屏 (SPI 模式)]
// SPI 时钟      | PG5      | SPI4_SCK_G5
// SPI 数据线    | PG7      | SPI4_MOSI_G7
// 屏幕复位      | PG6      | RES/RST
// 数据命令选择  | PH1      | DC/RS
// 屏幕片选      | PH12     | CS
// 屏幕背光      | 3.3V 排针| 物理常亮
//
// [8. GNSS 卫星导航模块 (UART 串口模式)]
// 串口发送 TX   | PF2      | UART1_RX_F2 (接模块 TX)
// 串口接收 RX   | PF3      | UART1_TX_F3 (接模块 RX)
//
// [9. OpenMV 视觉通信组 (UART 串口模式)]
// 串口发送 TX   | PA7      | UART2_TX_A7 (接 OpenMV RX)
// 串口接收 RX   | PA6      | UART2_RX_A6 (接 OpenMV TX)
// ==========================================================================================================
