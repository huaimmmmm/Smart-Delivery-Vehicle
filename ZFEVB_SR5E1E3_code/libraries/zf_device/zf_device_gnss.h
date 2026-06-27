/*********************************************************************************************************************
* Stellar-SR5E1E3 Opensource Library 即（Stellar-SR5E1E3 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 Stellar-SR5E1E3 开源库的一部分
*
* Stellar-SR5E1E3 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          zf_device_gnss
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

/*********************************************************************************************************************
* 接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   RX                  查看 zf_device_gnss.h 中 GNSS_RX 宏定义
*                   TX                  查看 zf_device_gnss.h 中 GNSS_TX 宏定义
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_gnss_h_
#define _zf_device_gnss_h_

// 需要使用 math 相关函数
#include "math.h"

// 如果移植到其他平台 这里就替换成自己的资源头文件
#include "zf_common_headfile.h"
#include "zf_driver_headfile.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// gnss_get_two_points_distance                                                 // GNSS 计算从第一个点到第二个点的距离
// gnss_get_two_points_azimuth                                                  // GNSS 计算从第一个点到第二个点的方位角

// gnss_data_parse                                                              // GNSS 解析数据

// gnss_init                                                                    // GNSS 初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 GNSS 相关的接口资源 这里允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define GNSS_UART_INDEX     ( UART_1        )
#define GNSS_RX_PIN         ( UART1_TX_F3   )                                   // GPS 模块 RX 串口引脚 对应单片机的 TX
#define GNSS_TX_PIN         ( UART1_RX_F2   )                                   // GPS 模块 TX 串口引脚 对应单片机的 RX

// 独立内存管理部分 默认使用 逐飞科技 开源库中的内存分段定义
// 如果移植到其它平台后此处报错 可以将 GNSS_INTERFACE_USE_ZF_COMMON_MEMORY 修改为 0
// 当 GNSS_INTERFACE_USE_ZF_COMMON_MEMORY 为 0 会禁止内存分段指定 通过编译器随机分配
#define     GNSS_INTERFACE_USE_ZF_COMMON_MEMORY     ( 1 )
#if         GNSS_INTERFACE_USE_ZF_COMMON_MEMORY
#ifndef _zf_common_memory_h_
#define AT_ZF_LIB_SECTION        
#define AT_ZF_LIB_SECTION_START  
#define AT_ZF_LIB_SECTION_END    
#endif
#else
#define AT_ZF_LIB_SECTION        
#define AT_ZF_LIB_SECTION_START  
#define AT_ZF_LIB_SECTION_END    
#endif

// 调试信息输出部分 默认使用 逐飞科技 开源库中的调试接口
// 如果移植到其它平台后此处报错 可以将 GNSS_INTERFACE_USE_ZF_COMMON_DEBUG 修改为 0
// 当 GNSS_INTERFACE_USE_ZF_COMMON_DEBUG 为 0 会通过 imu_assert 使用 printf 输出报错
#define     GNSS_INTERFACE_USE_ZF_COMMON_DEBUG  ( 1 )
#if         GNSS_INTERFACE_USE_ZF_COMMON_DEBUG
#ifndef _zf_common_debug_h_
#define zf_assert(pass)         ((pass) ? (0) : (1))
#define zf_log(pass, str)       ((pass) ? (0) : (1))
#endif
#endif

// 延时定向部分 默认使用 逐飞科技 开源库中的延时接口
// 如果移植到其它平台后此处报错 可以将 GNSS_INTERFACE_USE_ZF_DIRVER_DELAY 修改为 0
// 当 GNSS_INTERFACE_USE_ZF_DIRVER_DELAY 为 0 会通过 imu 的软延时实现
#define     GNSS_INTERFACE_USE_ZF_DIRVER_DELAY  ( 1 )
#if         GNSS_INTERFACE_USE_ZF_DIRVER_DELAY
#ifndef _zf_driver_delay_h_
#define zf_delay_ms(time)   for(vuint32 i = time * 1000000; i --; )
#endif
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 WIRELESS_UART 相关状态与数据格式等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define PI                  ( 3.1415926535898 )
#define ANGLE_TO_RAD(x)     ( (x) * PI / 180.0 )                                // 角度转换为弧度
#define RAD_TO_ANGLE(x)     ( (x) * 180.0 / PI )                                // 弧度转换为角度

typedef enum
{
    GNSS_TYPE_UNKONW        = 0,

    GNSS_TYPE_TAU1201       = 1,                                                // 逐飞科技双频 GPS 模块
    GNSS_TYPE_GN42A         = 1,                                                // 逐飞科技双频 GPS 模块 与 TAU1201 是一样的
    GNSS_TYPE_GN43RFA       = 2,                                                // 逐飞科技三频 RTK 模块
}gnss_type_enum;

typedef struct
{
    uint16      year;  
    uint8       month; 
    uint8       day;
    uint8       hour;
    uint8       minute;
    uint8       second;
}gnss_time_struct;

typedef struct
{
    gnss_time_struct    time;                                                   // 时间
    
    uint8               state;                                                  // 有效状态  1-定位有效  0-定位无效
    
    uint16              latitude_degree;                                        // 度
    uint16              latitude_cent;                                          // 分
    uint16              latitude_second;                                        // 秒    这里的秒是被放大了 100 倍的 主要是避免使用浮点数
    uint16              longitude_degree;                                       // 度
    uint16              longitude_cent;                                         // 分
    uint16              longitude_second;                                       // 秒    这里的秒是被放大了 100 倍的 主要是避免使用浮点数
    
    double              latitude;                                               // 纬度
    double              longitude;                                              // 经度
    
    int8                ns;                                                     // 纬度半球 N-北半球 或 S-南半球
    int8                ew;                                                     // 经度半球 E-东经 或 W-西经
    
    float               speed;                                                  // 速度 公里/每小时
    float               direction;                                              // 地面航向 [000.0~359.9] 度 以真北方为参考基准
    uint8               antenna_direction_state;                                // 双天线测向有效状态 1-测向有效  0-测向无效 无效时 antenna_direction 数据是无效的
    float               antenna_direction;                                      // 主天线指向从天线与真北构成的夹角 [000.0~359.9] 度
    
    // 下面两个个信息从GNGGA语句中获取
    uint8               satellite_used;                                         // 用于定位的卫星数量
    float               height;                                                 // 高度   
}gnss_info_struct;

typedef enum
{
    GNSS_STATE_RECEIVING,                                                       // 正在接收数据
    GNSS_STATE_RECEIVED,                                                        // 数据接收完成
    GNSS_STATE_PARSING,                                                         // 正在解析
}gnss_state_enum;

extern uint8            gnss_flag;                                              // 1-采集完成等待处理数据 0-没有采集完成
extern gnss_info_struct gnss_info;                                              // GNSS 解析之后的数据
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 WIRELESS_UART 相关状态与数据格式等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS 计算从第一个点到第二个点的距离
// 参数说明     latitude1       第一个点的纬度
// 参数说明     longitude1      第一个点的经度
// 参数说明     latitude2       第二个点的纬度
// 参数说明     longitude2      第二个点的经度
// 返回参数     double          返回两点距离
// 使用示例     gnss_get_two_points_distance(latitude1_1, longitude1, latitude2, longitude2);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
double gnss_get_two_points_distance (double latitude1, double longitude1, double latitude2, double longitude2);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS 计算从第一个点到第二个点的方位角
// 参数说明     latitude1       第一个点的纬度
// 参数说明     longitude1      第一个点的经度
// 参数说明     latitude2       第二个点的纬度
// 参数说明     longitude2      第二个点的经度
// 返回参数     double          返回方位角（0至360）
// 使用示例     gnss_get_two_points_azimuth(latitude1_1, longitude1, latitude2, longitude2);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
double gnss_get_two_points_azimuth (double latitude1, double longitude1, double latitude2, double longitude2);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS 解析数据
// 参数说明     void
// 返回参数     uint8           ZF_TRUE-解析成功 ZF_FALSE-数据有问题不能解析
// 使用示例     gnss_data_parse();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 gnss_data_parse (void);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS 初始化
// 参数说明     void
// 返回参数     uint8           操作状态 ZF_NO_ERROR - 完成 其余值为异常
// 使用示例     gps_init();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 gnss_init (gnss_type_enum gnss_type);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
