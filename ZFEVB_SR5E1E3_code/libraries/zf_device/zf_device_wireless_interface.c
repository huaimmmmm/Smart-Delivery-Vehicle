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
* 文件名称          zf_device_wireless_interface
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

/*********************************************************************************************************************
* 本文件的作用是方便 WIRELESS 的任意平台适配
* 内容为将 WIRELESS 相关的接口进行解耦整合
* 在移植到新的平台时 只需要实现了本文件的接口
* 就可以直接适配 SeekFree 所有的 V4 库 WIRELESS 器件
********************************************************************************************************************/

// 自身头文件
#include "zf_device_wireless_interface.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
AT_ZF_LIB_SECTION_START
#if WIRELESS_INTERFACE_BOARD_UART_INTERFACE
AT_ZF_LIB_SECTION wireless_interface_obj_struct wireless_interface_uart_default_obj = 
{
    .interface_type         = WIRELESS_INTERFACE_TYPE_UNKNOW,
    .communication_index    = 0,
    .rts_pin                = PIN_NULL,
    .rst_pin                = PIN_NULL,
    .int_pin                = EXTI1_CH0_A0,
    .fifo_obj               =
    {
        .base_ptr           = 0,
        .tail_ptr           = 0,
        .write_ptr          = 0,
        .read_ptr           = 0,
        .step               = FIFO_DATA_8BIT,
        .state              = 0
    }
};
#endif
#if WIRELESS_INTERFACE_BOARD_SPI_INTERFACE
AT_ZF_LIB_SECTION wireless_interface_obj_struct wireless_interface_spi_default_obj = 
{
    .interface_type         = WIRELESS_INTERFACE_TYPE_UNKNOW,
    .communication_index    = 0,
    .rts_pin                = PIN_NULL,
    .rst_pin                = PIN_NULL,
    .int_pin                = EXTI1_CH0_A0,
    .fifo_obj               =
    {
        .base_ptr           = 0,
        .tail_ptr           = 0,
        .write_ptr          = 0,
        .read_ptr           = 0,
        .step               = FIFO_DATA_8BIT,
        .state              = 0
    }
};
#endif
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 WIRELESS 相关的接口实现 这里允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// //-------------------------------------------------------------------------------------------------------------------
// // 函数简介     WIRELESS 中断处理
// // 参数说明     event               回调事件
// // 参数说明     *ptr                标志位指针
// // 返回参数     void
// // 使用示例     
// // 备注信息     
// //-------------------------------------------------------------------------------------------------------------------
// static void wireless_interface_callbakc_defalut (uint32 event, void *ptr)
// {
//     (void)event;
//     (void)ptr;
// }

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
#if WIRELESS_INTERFACE_USE_ZF_COMMON_DEBUG
#else
uint8 wireless_interface_assert_handler (char *file, int line, char *tips)
{
    printf("\r\n WIRELESS OPERATION ASSERT!\r\n");
    printf("\r\n file: %s.\r\n", file);
    printf("\r\n line: %d.\r\n", line);
    if(NULL != tips)
    {
        printf("\r\n tips: %s.\r\n", tips);
    }
    while(1);
    return ZF_ERROR;
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS 延时
// 参数说明     ms              毫秒数
// 返回参数     void
// 使用示例     wireless_interface_delay_ms(ms);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#if WIRELESS_INTERFACE_USE_ZF_DIRVER_DELAY
#else
void wireless_interface_delay_ms (uint32 ms)
{
    for(uint32 loop_count1 = ms    ; loop_count1 --; )
    for(uint32 loop_count2 = 0xFFFF; loop_count2 --; );
}
#endif


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS 输出 8bit 数据
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     *send_data          发送数据缓冲区
// 参数说明     send_len            发送数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_interface_write_8bit_array(interface, send_data, send_len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_interface_write_8bit_array (wireless_interface_obj_struct *interface, const uint8 *send_data, uint32 send_len)
{
    wireless_interface_operation_state_enum  return_state    =   WIRELESS_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = WIRELESS_INTERFACE_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(WIRELESS_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = WIRELESS_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        return_state = WIRELESS_INTERFACE_OPERATION_DONE;
        if(WIRELESS_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
            zf_spi_transfer_8bit_array(interface->spi_index, send_data, NULL, send_len);
            zf_gpio_high(interface->spi_cs_pin);
        }
        else
        {
            zf_uart_write_buffer(interface->uart_index, send_data, send_len);
        }

    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS 接收 8bit 数据
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     *receive_data       接收数据缓冲区
// 参数说明     receive_len         接收数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_interface_read_8bit_array(interface, receive_data, receive_len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_interface_read_8bit_array (wireless_interface_obj_struct *interface, uint8 *receive_data, uint32 receive_len)
{
    wireless_interface_operation_state_enum  return_state    =   WIRELESS_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = WIRELESS_INTERFACE_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(WIRELESS_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = WIRELESS_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        return_state = WIRELESS_INTERFACE_OPERATION_DONE;
        if(WIRELESS_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
            zf_spi_transfer_8bit_array(interface->spi_index, NULL, receive_data, receive_len);
            zf_gpio_high(interface->spi_cs_pin);
        }
        else
        {
            zf_uart_read_buffer(interface->uart_index, receive_data, receive_len);
        }

    }while(0);

    return return_state;
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS 双向传输 8bit 数据
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     *send_data          发送数据缓冲区
// 参数说明     send_len            发送数据长度
// 参数说明     *receive_data       接收数据缓冲区
// 参数说明     receive_len         接收数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_interface_transfer_8bit_array(interface, send_data, send_len, receive_data, receive_len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_interface_transfer_8bit_array (wireless_interface_obj_struct *interface, const uint8 *send_data, uint32 send_len, uint8 *receive_data, uint32 receive_len)
{
    wireless_interface_operation_state_enum  return_state    =   WIRELESS_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = WIRELESS_INTERFACE_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(WIRELESS_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = WIRELESS_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        return_state = WIRELESS_INTERFACE_OPERATION_DONE;
        if(WIRELESS_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
            if(NULL != send_data && 0 != send_len)
            {
                zf_spi_transfer_8bit_array(interface->spi_index, send_data, NULL, send_len);
            }
            if(NULL != receive_data && 0 != receive_len)
            {
                zf_spi_transfer_8bit_array(interface->spi_index, NULL, receive_data, receive_len);
            }
            zf_gpio_high(interface->spi_cs_pin);
        }
        else
        {
            if(NULL != send_data && 0 != send_len)
            {
                zf_uart_write_buffer(interface->uart_index, send_data, send_len);
            }
            if(NULL != receive_data && 0 != receive_len)
            {
                zf_uart_read_buffer(interface->uart_index, receive_data, receive_len);
            }
        }

    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS 接口注销初始化
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_interface_deinit(interface);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_interface_deinit (wireless_interface_obj_struct *interface)
{
    wireless_interface_operation_state_enum  return_state    =   WIRELESS_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = WIRELESS_INTERFACE_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(WIRELESS_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = WIRELESS_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(WIRELESS_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_exti_channel_deinit(interface->int_pin);
            zf_spi_deinit(interface->spi_index);
            zf_gpio_deinit(interface->spi_cs_pin);
            zf_gpio_deinit(interface->rst_pin);
        }
        else
        {
            zf_uart_deinit(interface->uart_index);
            zf_gpio_deinit(interface->rts_pin);
        }

        interface->interface_type       = WIRELESS_INTERFACE_TYPE_UNKNOW;
        interface->spi_index            = 0                             ;
        interface->spi_cs_pin           = PIN_NULL                      ;
        interface->rst_pin              = PIN_NULL                      ;
        interface->int_pin              = 0                             ;

        interface->fifo_obj.base_ptr    = 0                 ;
        interface->fifo_obj.tail_ptr    = 0                 ;
        interface->fifo_obj.write_ptr   = 0                 ;
        interface->fifo_obj.read_ptr    = 0                 ;
        interface->fifo_obj.step        = FIFO_DATA_8BIT    ;
        interface->fifo_obj.state       = 0                 ;

        return_state = WIRELESS_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS SPI 通信 接口初始化
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     mode                SPI 模式        (详见 zf_driver_spi.h 内 zf_spi_mode_enum 定义)
// 参数说明     baudrate            设置 SPI 的波特率 不超过系统时钟的一半 会被适配成相近的速率
// 参数说明     sck_pin             选择 SCK 引脚   (详见 zf_driver_spi.h 内 zf_spi_sck_pin_enum 定义)
// 参数说明     mosi_pin            选择 MOSI 引脚  (详见 zf_driver_spi.h 内 zf_spi_mosi_pin_enum 定义)
// 参数说明     miso_pin            选择 MISO 引脚  (详见 zf_driver_spi.h 内 zf_spi_miso_pin_enum 定义)
// 参数说明     cs_pin              使用软件 CS 控制 所以此处填写 GPIO 索引
// 参数说明     rst_pin             RST 复位引脚
// 参数说明     int_pin             INT 中断引脚    (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 参数说明     *data_buffer        数据缓冲区
// 参数说明     buffer_size         数据缓冲区大小
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_interface_spi_init(interface, spi_index, mode, baudrate, sck_pin, mosi_pin, miso_pin, cs_pin, rst_pin, int_pin, data_buffer, buffer_size, call_back, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_interface_spi_init (wireless_interface_obj_struct *interface,
    zf_spi_index_enum spi_index, zf_spi_mode_enum mode, uint32 baudrate,
    zf_spi_sck_pin_enum sck_pin, zf_spi_mosi_pin_enum mosi_pin, zf_spi_miso_pin_enum miso_pin, zf_gpio_pin_enum cs_pin,
    zf_gpio_pin_enum rst_pin, zf_exti_channel_pin_enum int_pin, uint8 *data_buffer, uint32 buffer_size,
    void_callback_uint32_ptr call_back, void *ptr)
{
    wireless_interface_operation_state_enum  return_state    =   WIRELESS_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(     NULL == interface   || NULL == call_back    ||  NULL == ptr
            ||  NULL == data_buffer || 0 == buffer_size)
        {
            return_state = WIRELESS_INTERFACE_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(WIRELESS_INTERFACE_TYPE_UNKNOW != interface->interface_type)
        {
            return_state = WIRELESS_INTERFACE_ERROR_INTERFACE_OCCUPIED;
            break;
        }

        if(zf_spi_init(spi_index, mode, baudrate, sck_pin, mosi_pin, miso_pin, SPI_CS_NULL))
        {
            return_state = WIRELESS_INTERFACE_ERROR_INTERFACE_OCCUPIED;
            break;
        }
        zf_gpio_init(cs_pin, GPO_PUSH_PULL, GPIO_HIGH);
        zf_gpio_init(rst_pin, GPO_PUSH_PULL, GPIO_HIGH);
        zf_exti_channel_init(int_pin, EXTI_TRIGGER_TYPE_FALLING, call_back, ptr);
        zf_fifo_init(&(interface->fifo_obj), FIFO_DATA_8BIT, data_buffer, buffer_size);

        interface->interface_type   = WIRELESS_INTERFACE_TYPE_UART  ;
        interface->spi_index        = spi_index                     ;
        interface->spi_cs_pin       = cs_pin                        ;
        interface->rst_pin          = rst_pin                       ;
        interface->int_pin          = int_pin                       ;

        return_state = WIRELESS_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS UART 通信 接口初始化
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     baudrate            UART 波特率
// 参数说明     tx_pin              UART 发送引脚   (详见 zf_driver_uart.h 内 zf_uart_tx_pin_enum 定义)
// 参数说明     rx_pin              UART 接收引脚   (详见 zf_driver_uart.h 内 zf_uart_rx_pin_enum 定义)
// 参数说明     rts_pin             RTS 流控引脚
// 参数说明     *data_buffer        数据缓冲区
// 参数说明     buffer_size         数据缓冲区大小
// 参数说明     callback            回调函数 为无返回值 uint32 ptr 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_interface_uart_init(interface, uart_index, baudrate, tx_pin, rx_pin, rts_pin, data_buffer, buffer_size, call_back, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_interface_uart_init (wireless_interface_obj_struct *interface,
    zf_uart_index_enum uart_index, uint32 baudrate, zf_uart_tx_pin_enum tx_pin, zf_uart_rx_pin_enum rx_pin,
    zf_gpio_pin_enum rts_pin, uint8 *data_buffer, uint32 buffer_size, void_callback_uint32_ptr call_back, void *ptr)
{
    wireless_interface_operation_state_enum  return_state    =   WIRELESS_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(     NULL == interface   || NULL == call_back    ||  NULL == ptr
            ||  NULL == data_buffer || 0 == buffer_size)
        {
            return_state = WIRELESS_INTERFACE_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(WIRELESS_INTERFACE_TYPE_UNKNOW != interface->interface_type)
        {
            return_state = WIRELESS_INTERFACE_ERROR_INTERFACE_OCCUPIED;
            break;
        }

        if(zf_uart_init(uart_index, baudrate, tx_pin, rx_pin))
        {
            return_state = WIRELESS_INTERFACE_ERROR_INTERFACE_OCCUPIED;
            break;
        }
        zf_uart_set_interrupt_callback(uart_index, call_back, ptr);
        zf_uart_set_interrupt_config(uart_index, UART_INTERRUPT_CONFIG_RX_ENABLE);

        zf_gpio_init(rts_pin, GPI_PULL_UP, GPIO_HIGH);
        zf_fifo_init(&(interface->fifo_obj), FIFO_DATA_8BIT, data_buffer, buffer_size);

        interface->interface_type   = WIRELESS_INTERFACE_TYPE_UART  ;
        interface->uart_index       = uart_index                    ;
        interface->rts_pin          = rts_pin                       ;

        return_state = WIRELESS_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
