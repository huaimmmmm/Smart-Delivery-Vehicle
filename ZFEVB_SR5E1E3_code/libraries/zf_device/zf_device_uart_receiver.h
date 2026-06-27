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
* 文件名称          zf_device_uart_receiver
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

/*********************************************************************************************************************
* 接线定义：
*                   ------------------------------------
*                   模块管脚        单片机管脚
*                   TXD             查看 zf_device_uart_receiver.h 中 UART_RECEVIER_RX_PIN 宏定义
*                   GND             电源地
*                   5V              5V电源
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_uart_receiver_h_
#define _zf_device_uart_receiver_h_

// 如果移植到其他平台 这里就替换成自己的资源头文件
#include "zf_common_headfile.h"
#include "zf_driver_headfile.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// uart_receiver_init                                                           // 串口接收机初始化 初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 GNSS 相关的接口资源 这里允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define UART_RECEVIER_UART_INDEX    ( UART_2        )
#define UART_RECEVIER_RX_PIN        ( UART2_TX_A7   )
#define UART_RECEVIER_TX_PIN        ( UART2_RX_A6   )                           // UART_RECEVIER 模块 TX 串口引脚 对应单片机的 RX

#define UART_RECEVIER_TIMER_INDEX   ( TIM_15        )                           // UART_RECEVIER 需要进行接受间隔解析 需要一个定时器

#define UART_RECEVIER_BAUDRATE      ( 100000        )                           // 串口波特率 不可修改
#define UART_RECEVIER_CHANNEL_NUM   ( 6             )                           // 通道数量 不可修改

// 独立内存管理部分 默认使用 逐飞科技 开源库中的内存分段定义
// 如果移植到其它平台后此处报错 可以将 UART_RECEVIER_INTERFACE_USE_ZF_COMMON_MEMORY 修改为 0
// 当 UART_RECEVIER_INTERFACE_USE_ZF_COMMON_MEMORY 为 0 会禁止内存分段指定 通过编译器随机分配
#define     UART_RECEVIER_INTERFACE_USE_ZF_COMMON_MEMORY    ( 1 )
#if         UART_RECEVIER_INTERFACE_USE_ZF_COMMON_MEMORY
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

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 UART_RECEVIER 相关状态与数据格式等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct
{
    uint16 channel[UART_RECEVIER_CHANNEL_NUM];                  // CH1-CH6通道数据
    uint8  state;                                               // 遥控器状态(1表示正常，否则表示失控)
    uint8  finsh_flag;                                          // 1：表示成功接收到一帧遥控器数据
}uart_receiver_struct;

extern uart_receiver_struct uart_receiver;                      // 串口接收机通道数据与状态
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 UART_RECEVIER 相关状态与数据格式等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     串口接收机初始化
// 参数说明     void
// 返回参数     uint8           操作状态 ZF_NO_ERROR - 完成 其余值为异常s
// 使用示例     uart_receiver_init();
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_receiver_init (void);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
