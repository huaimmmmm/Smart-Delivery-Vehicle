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

#include "zf_device_uart_receiver.h"

// 此处定义 UART_RECEVIER 相关的接口资源 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define UART_RECEVIER_REV_DATA_LEN                  ( 25   )                    // SBUS帧长
#define UART_RECEVIER_FRAME_STAR                    ( 0x0F )                    // 帧头信息
#define UART_RECEVIER_FRAME_END                     ( 0x00 )                    // 帧尾信息
#define UART_RECEVIER_SBUS_ABNORMAL_STATE           ( 0x04 )                    // 失控状态
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION uart_receiver_struct uart_receiver ;                          // 串口接收机通道数据与状态
AT_ZF_LIB_SECTION static uint8 uart_receiver_data[UART_RECEVIER_REV_DATA_LEN];  // 接收器原始数据
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
//  函数简介     获取两次调用uart_receiver_interval_time函数时间间隔 
//  参数说明     void
//  返回参数     uint16            返回从开始到现在的时间 单位 us
//  使用示例     uart_receiver_interval_time();
//  备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint16 uart_receiver_interval_time (void)
{
    uint16 interval_time;

    zf_timer_get(UART_RECEVIER_TIMER_INDEX, &interval_time);

    zf_timer_clear(UART_RECEVIER_TIMER_INDEX);

    return interval_time;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SBUS数据解析
// 参数说明     *remote_data    接收遥控器数据的地址
// 参数说明     *bufer          原始数据
// 返回参数     void
// 使用示例
// 备注信息     对sbus数据解析进行解析
//-------------------------------------------------------------------------------------------------------------------
static void uart_receiver_analysis (uart_receiver_struct *remote_data,uint8 * buffer)
{
    uint8 num = 0;

    remote_data->channel[num++] = (buffer[1] |buffer[ 2] << 8 ) & 0x07FF;
    remote_data->channel[num++] = (buffer[2] >> 3 | buffer[3] << 5 ) & 0x07FF;
    remote_data->channel[num++] = (buffer[3] >> 6 | buffer[4] << 2 | buffer[5] << 10 ) & 0x07FF;
    remote_data->channel[num++] = (buffer[5] >> 1 | buffer[6] << 7 ) & 0x07FF;
    remote_data->channel[num++] = (buffer[6] >> 4 | buffer[7] << 4 ) & 0x07FF;
    remote_data->channel[num++] = (buffer[7] >> 7 | buffer[8] << 1 | buffer[9] << 9 ) & 0x07FF;

    remote_data->state = 
        (       UART_RECEVIER_SBUS_ABNORMAL_STATE
            ==  (buffer[23] & UART_RECEVIER_SBUS_ABNORMAL_STATE))
        ? 0
        : 1;

    uart_receiver.finsh_flag  = 1;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     串口接收机模块 串口中断回调函数
// 参数说明     void
// 返回参数     void
// 使用示例     uart_receiver_callback();
// 备注信息     该函数在 ISR 文件 串口中断程序通过uart_receiver_uart_handler函数指针被调用
//-------------------------------------------------------------------------------------------------------------------
static void uart_receiver_callback (uint32 event, void *ptr)
{
    (void)event;
    (void)ptr;

    static vuint8 length = 0;

    if(uart_receiver_interval_time() > 3000)
    {
        length = 0;
    }

    if(!zf_uart_query_byte(UART_RECEVIER_UART_INDEX, &uart_receiver_data[length]))
    {
        length ++;
        if(     (UART_RECEVIER_REV_DATA_LEN == length)
            &&  (UART_RECEVIER_FRAME_STAR   == uart_receiver_data[0 ])
            &&  (UART_RECEVIER_FRAME_END    == uart_receiver_data[24]))
        {
            uart_receiver_analysis(&uart_receiver, uart_receiver_data);
        }
    }

}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     串口接收机初始化
// 参数说明     void
// 返回参数     uint8           操作状态 ZF_NO_ERROR - 完成 其余值为异常s
// 使用示例     uart_receiver_init();
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_receiver_init (void)
{
    uint8 return_state = ZF_ERROR;

    zf_uart_init(UART_RECEVIER_UART_INDEX, UART_RECEVIER_BAUDRATE, UART_RECEVIER_RX_PIN, UART_RECEVIER_TX_PIN);

    zf_timer_init(UART_RECEVIER_TIMER_INDEX, TIMER_US);

    zf_uart_set_interrupt_callback(UART_RECEVIER_UART_INDEX, uart_receiver_callback, NULL);
    zf_uart_set_interrupt_config(UART_RECEVIER_UART_INDEX, UART_INTERRUPT_CONFIG_RX_ENABLE);

    zf_timer_enable(UART_RECEVIER_TIMER_INDEX);

    return_state = ZF_NO_ERROR;

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
