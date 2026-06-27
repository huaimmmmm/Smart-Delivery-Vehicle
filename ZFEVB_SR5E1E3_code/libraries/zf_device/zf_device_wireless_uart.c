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

// 自身头文件
#include "zf_device_wireless_uart.h"

// 此处定义 MT9V03X 相关的接口资源 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define WIRELESS_UART_TIMEOUT_COUNT     ( 0x64 )
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WRIELESS_UART 模块 发送数据 框架接口
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     data                8bit 数据
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_func_send_byte(interface, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static uint8 wireless_uart_func_send_byte (wireless_interface_obj_struct *interface, const uint8 data)
{
    wireless_uart_operation_state_enum  return_state    = WIRELESS_UART_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = WIRELESS_UART_ERROR_DATA_BUFFER_NULL;
            break;
        }

        uint16 time_count = WIRELESS_UART_TIMEOUT_COUNT;
        while(time_count)
        {
            if(!zf_gpio_get_level(interface->rts_pin))
            {
                wireless_interface_write_8bit_array(interface, &data, 1);
                break;
            }
            time_count --;
            wireless_interface_delay_ms(1);
        }

        return_state = (0 < time_count) ? (WIRELESS_UART_OPERATION_DONE) : (WIRELESS_UART_WARNING_OPERATION_TIMEOUT);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WRIELESS_UART 模块 发送数据块 框架接口
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     *buff               发送数据
// 参数说明     *len                发送数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_func_send_buffer(interface, buff, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static uint8 wireless_uart_func_send_buffer (wireless_interface_obj_struct *interface, const uint8 *buff, uint32 *len)
{
    wireless_uart_operation_state_enum  return_state    = WIRELESS_UART_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = WIRELESS_UART_ERROR_DATA_BUFFER_NULL;
            break;
        }

        uint16 time_count = WIRELESS_UART_TIMEOUT_COUNT;
        while(0 != *len && time_count)
        {
            if(!zf_gpio_get_level(wireless_interface_uart_default_obj.rts_pin)) // 如果RTS为低电平 则继续发送数据
            {
                if(30 <= *len)                                                  // 数据分 30byte 每包发送
                {
                    wireless_interface_write_8bit_array(&wireless_interface_uart_default_obj, buff, 30);
                    buff += 30;                                                 // 地址偏移
                    *len -= 30;                                                 // 数量
                    time_count = WIRELESS_UART_TIMEOUT_COUNT;
                }
                else                                                            // 不足 30byte 的数据一次性发送完毕
                {
                    wireless_interface_write_8bit_array(&wireless_interface_uart_default_obj, buff, *len);
                    *len = 0;
                    break;
                }
            }
            else                                                                // 如果RTS为高电平 则模块忙
            {
                time_count --;
                wireless_interface_delay_ms(1);
            }
        }

        return_state = (0 < time_count) ? (WIRELESS_UART_OPERATION_DONE) : (WIRELESS_UART_WARNING_OPERATION_TIMEOUT);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WRIELESS_UART 模块 发送字符串 框架接口
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     *string             发送字符串
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_func_send_string(interface, string);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static uint8 wireless_uart_func_send_string (wireless_interface_obj_struct *interface, const char *string)
{
    wireless_uart_operation_state_enum  return_state    = WIRELESS_UART_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = WIRELESS_UART_ERROR_DATA_BUFFER_NULL;
            break;
        }

        uint16 time_count = WIRELESS_UART_TIMEOUT_COUNT;
        uint32 len = strlen(string);
        while(0 != len && time_count)
        {
            if(!zf_gpio_get_level(wireless_interface_uart_default_obj.rts_pin)) // 如果RTS为低电平 则继续发送数据
            {
                if(30 <= len)                                                   // 数据分 30byte 每包发送
                {
                    wireless_interface_write_8bit_array(&wireless_interface_uart_default_obj, (const uint8 *)string, 30);
                    string += 30;                                               // 地址偏移
                    len -= 30;                                                  // 数量
                    time_count = WIRELESS_UART_TIMEOUT_COUNT;
                }
                else                                                            // 不足 30byte 的数据一次性发送完毕
                {
                    wireless_interface_write_8bit_array(&wireless_interface_uart_default_obj, (const uint8 *)string, len);
                    len = 0;
                    break;
                }
            }
            else                                                                // 如果RTS为高电平 则模块忙
            {
                time_count --;
                wireless_interface_delay_ms(1);
            }
        }

        return_state = (0 < time_count) ? (WIRELESS_UART_OPERATION_DONE) : (WIRELESS_UART_WARNING_OPERATION_TIMEOUT);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WRIELESS_UART 模块 读取缓冲 框架接口
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     *buff               发送数据
// 参数说明     *len                发送数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_func_read_buffer(interface, buff, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static uint8 wireless_uart_func_read_buffer (wireless_interface_obj_struct *interface, uint8 *buff, uint32 *len)
{
    wireless_uart_operation_state_enum  return_state    = WIRELESS_UART_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = WIRELESS_UART_ERROR_DATA_BUFFER_NULL;
            break;
        }

        uint32 data_len = *len;
        zf_fifo_read_buffer(&wireless_interface_uart_default_obj.fifo_obj, buff, len, FIFO_READ_WITH_CLEAN);

        return_state = (data_len != *len) ? (WIRELESS_UART_OPERATION_DONE) : (WIRELESS_UART_WARNING_OPERATION_TIMEOUT);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无线转串口模块 串口中断回调函数
// 参数说明     void
// 返回参数     void
// 使用示例     wireless_uart_callback();
// 备注信息     该函数在 ISR 文件 串口中断程序被调用
//              由串口中断服务函数调用 wireless_module_uart_handler() 函数
//              再由 wireless_module_uart_handler() 函数调用本函数
//-------------------------------------------------------------------------------------------------------------------
static void wireless_uart_callback (uint32 event, void *ptr)
{
    (void)event;
    uint8   wireless_uart_data  = 0;

    wireless_interface_obj_struct *interface = (wireless_interface_obj_struct *)ptr;
    wireless_interface_read_8bit_array(interface, &wireless_uart_data, 1);
    zf_fifo_write_element(&(interface->fifo_obj), wireless_uart_data);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WRIELESS_UART 模块 发送数据
// 参数说明     data                8bit 数据
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_send_byte(data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_send_byte (const uint8 data)
{
    return wireless_uart_func_send_byte(&wireless_interface_uart_default_obj, data);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无线转串口模块 发送数据块
// 参数说明     *buff               发送数据
// 参数说明     *len                发送数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_send_buffer(buff, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_send_buffer (const uint8 *buff, uint32 *len)
{
    return wireless_uart_func_send_buffer(&wireless_interface_uart_default_obj, buff, len);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无线转串口模块 发送字符串
// 参数说明     *string             要发送的字符串地址
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_send_string(string);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_send_string (const char *string)
{
    return wireless_uart_func_send_string(&wireless_interface_uart_default_obj, string);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无线转串口模块 读取缓冲
// 参数说明     *buff               接收缓冲区
// 参数说明     *len                读取数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_read_buffer(buff, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_read_buffer (uint8 *buff, uint32 *len)
{
    return wireless_uart_func_read_buffer(&wireless_interface_uart_default_obj, buff, len);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WRIELESS_UART 模块 初始化
// 参数说明     *data_buffer        数据缓冲区
// 参数说明     buffer_size         数据缓冲区大小
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_uart_init(data_buffer, buffer_size);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_init (uint8 *data_buffer, uint32 buffer_size)
{
    wireless_uart_operation_state_enum  return_state    = WIRELESS_UART_ERROR_UNKNOW_ERROR;

    do
    {
        if(wireless_interface_assert(NULL != data_buffer || 0 == buffer_size))
        {
            return_state = WIRELESS_UART_ERROR_DATA_BUFFER_NULL;
            break;
        }

        uint32  temp_value                      = 0;
        uint8   rts_init_status                 = 0;
        uint8   wireless_auto_baud_data[3]      = {0, 1, 2};

        wireless_interface_uart_default_init(&wireless_interface_uart_default_obj, data_buffer, buffer_size, wireless_uart_callback, &wireless_interface_uart_default_obj);
        rts_init_status = zf_gpio_get_level(wireless_interface_uart_default_obj.rts_pin);
        zf_gpio_init(wireless_interface_uart_default_obj.rts_pin, GPO_PUSH_PULL, rts_init_status);

        wireless_interface_delay_ms(5);                                         // 模块上电之后需要延时等待
        zf_gpio_set_level(wireless_interface_uart_default_obj.rts_pin, !rts_init_status);   // RTS引脚拉高，进入自动波特率模式
        wireless_interface_delay_ms(100);                                       // RTS拉高之后必须延时20ms
        zf_gpio_toggle_level(wireless_interface_uart_default_obj.rts_pin);      // RTS引脚取反

        do
        {
            zf_fifo_clear(&wireless_interface_uart_default_obj.fifo_obj);
            wireless_interface_write_8bit_array(&wireless_interface_uart_default_obj, wireless_auto_baud_data, 3);
            wireless_interface_delay_ms(20);

            if(3 != zf_fifo_used(&wireless_interface_uart_default_obj.fifo_obj))// 检验自动波特率是否完成
            {
                break;                                                          // 如果程序进入到此语句内 说明自动波特率失败了
            }

            temp_value = 3;
            zf_fifo_read_buffer(&wireless_interface_uart_default_obj.fifo_obj, wireless_auto_baud_data, &temp_value, FIFO_READ_WITH_CLEAN);
            if( 0xa5 != wireless_auto_baud_data[0] &&                           // 检验自动波特率是否正确
                0xff != wireless_auto_baud_data[1] &&                           // 检验自动波特率是否正确
                0xff != wireless_auto_baud_data[2] )                            // 检验自动波特率是否正确
            {
                break;                                                          // 如果程序进入到此语句内 说明自动波特率失败了
            }
            zf_fifo_clear(&wireless_interface_uart_default_obj.fifo_obj);

            zf_gpio_init(wireless_interface_uart_default_obj.rts_pin, GPI_PULL_UP, 0);  // 初始化流控引脚
            wireless_interface_delay_ms(10);                                    // 延时等待 模块准备就绪
        }while(0);

        return_state = WIRELESS_UART_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
