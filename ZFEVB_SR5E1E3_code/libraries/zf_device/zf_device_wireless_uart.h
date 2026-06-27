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
* 文件名称          zf_device_wireless_uart
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

/*********************************************************************************************************************
* 接线定义          查看 zf_device_wireless_interface.h 中接口资源定义
********************************************************************************************************************/

#ifndef _zf_device_wireless_uart_h_
#define _zf_device_wireless_uart_h_

// zf_common 层 类型定义 引用
#include "zf_common_typedef.h"

// zf_device 层头文件引用
#include "zf_device_wireless_interface.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// wireless_uart_send_byte                                                      // WRIELESS_UART 模块 发送数据
// wireless_uart_send_buffer                                                    // WRIELESS_UART 模块 发送数据块
// wireless_uart_send_string                                                    // WRIELESS_UART 模块 发送字符串

// wireless_uart_read_buffer                                                    // WRIELESS_UART 模块 读取缓冲

// wireless_uart_init                                                           // WRIELESS_UART 模块 初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 WIRELESS_UART 相关状态与数据格式等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef enum
{
    WIRELESS_UART_OPERATION_DONE            =   WIRELESS_INTERFACE_OPERATION_DONE               ,   // 操作完成
    WIRELESS_UART_ERROR_UNKNOW_ERROR        =   WIRELESS_INTERFACE_ERROR_UNKNOW_ERROR           ,   // 异常错误 这个错误理论上不会被返回

    WIRELESS_UART_WARNING_MODULE_DISABLE    =   WIRELESS_INTERFACE_WARNING_MODULE_DISABLE       ,   // 模块禁止警告 模块未使能
    WIRELESS_UART_WARNING_OPERATION_TIMEOUT =   WIRELESS_INTERFACE_WARNING_OPERATION_TIMEOUT    ,   // 模块超时警告 通信或者操作超时
    WIRELESS_UART_WARNING_TRANSFER_LEN_CHANGE                                                   ,   // 长度变更警告

    WIRELESS_UART_ERROR_INTERFACE_OCCUPIED  =   WIRELESS_INTERFACE_ERROR_INTERFACE_OCCUPIED     ,   // 通信接口占用错误 不能重复初始化同一个接口
    WIRELESS_UART_ERROR_INTERFACE_NOT_INIT  =   WIRELESS_INTERFACE_ERROR_INTERFACE_NOT_INIT     ,   // 接口未初始化错误 不能未初始化就调用接口

    WIRELESS_UART_ERROR_DATA_BUFFER_NULL    =   WIRELESS_INTERFACE_ERROR_DATA_BUFFER_NULL       ,   // 数据缓冲区错误 不可以传入空指针
}wireless_uart_operation_state_enum;
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 WIRELESS_UART 相关状态与数据格式等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WRIELESS_UART 模块 发送数据
// 参数说明     data                8bit 数据
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_send_byte(data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_send_byte (const uint8 data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WRIELESS_UART 模块 发送数据块
// 参数说明     *buff               发送数据
// 参数说明     *len                发送数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_send_buffer(buff, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_send_buffer (const uint8 *buff, uint32 *len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WRIELESS_UART 模块 发送字符串
// 参数说明     *string             要发送的字符串地址
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_send_string(string);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_send_string (const char *string);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WRIELESS_UART 模块 读取缓冲
// 参数说明     *buff               接收缓冲区
// 参数说明     *len                读取数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_read_buffer(buff, 32);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_read_buffer (uint8 *buff, uint32 *len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WRIELESS_UART 模块 初始化
// 参数说明     *data_buffer        数据缓冲区
// 参数说明     buffer_size         数据缓冲区大小
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_init(data_buffer, buffer_size);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_init (uint8 *data_buffer, uint32 buffer_size);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
