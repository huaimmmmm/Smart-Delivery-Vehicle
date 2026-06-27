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
* 文件名称          zf_driver_uart
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

// zf_common 层引用
#include "zf_common_debug.h"
#include "zf_common_fifo.h"
#include "zf_common_memory.h"

// zf_driver 层引用
#include "zf_driver_interrupt.h"
#include "zf_driver_rcc.h"

// 自身头文件
#include "zf_driver_uart.h"

// 此处定义 本文件用使用的模块管理数据 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define UART_INPUT_CLOCK    ( 60000000 )

static void zf_uart_callbakc_defalut (uint32 event, void *ptr);

// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION zf_uart_obj_struct uart_obj_list[UART_NUM_MAX] = 
{
    {.baudrate = 0, .uart_ptr = UART1, .tx_pin = PIN_NULL, .rx_pin = PIN_NULL, .config_info = 0},
    {.baudrate = 0, .uart_ptr = UART2, .tx_pin = PIN_NULL, .rx_pin = PIN_NULL, .config_info = 0},
    {.baudrate = 0, .uart_ptr = UART3, .tx_pin = PIN_NULL, .rx_pin = PIN_NULL, .config_info = 0},
};

// 串口中断 回调函数第一个参数代表触发类型 发送空中断或接收有效中断 第二个参数是用户指针
AT_ZF_LIB_SECTION zf_interrupt_callback_struct uart_callback[UART_NUM_MAX] = 
{
    {zf_uart_callbakc_defalut, &uart_obj_list[0]},
    {zf_uart_callbakc_defalut, &uart_obj_list[1]},
    {zf_uart_callbakc_defalut, &uart_obj_list[2]},
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件用使用的静态或常亮数据 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 用于固定值或指向的内容 主要是用少量存储占用来减少栈开销
static const zf_interrupt_index_enum uart_irq_index_list[UART_NUM_MAX] =
{
    INTERRUPT_INDEX_UART1, INTERRUPT_INDEX_UART2, INTERRUPT_INDEX_UART3,
};

static const zf_rcc_index_enum uart_rcc_index_list[UART_NUM_MAX] = 
{
    RCC_INDEX_UART1, RCC_INDEX_UART2, RCC_INDEX_UART3,
};
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的外部重载函数 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 中断默认回调函数
// 参数说明     event               触发中断的事件 此处默认为 0
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void zf_uart_callbakc_defalut (uint32 event, void *ptr)
{
	(void)event;
	(void)ptr;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART1 的串口中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              uart_set_interrupt_priority(uart_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_UART1_HANDLER (void)
{
    #define TEMP_UART_INDEX         UART1
    #define TEMP_UART_CALLBACK      uart_callback[UART_1]

    if(uart_obj_list[UART_1].baudrate)
    {
        uint32 event_temp = UART_INTERRUPT_STATE_NONE;
        if((TEMP_UART_INDEX->CR1 & UART_CR1_RXNEIE_RXFNEIE))
        {
            event_temp |= (TEMP_UART_INDEX->ISR & UART_ISR_RXNE_RXFNE) ? (UART_INTERRUPT_STATE_RX) : (UART_INTERRUPT_STATE_NONE);
        }
        if((TEMP_UART_INDEX->CR1 & UART_CR1_TXEIE_TXFNFIE))
        {
            event_temp |= (TEMP_UART_INDEX->ISR & UART_ISR_TXE_TXFNF) ? (UART_INTERRUPT_STATE_TX) : (UART_INTERRUPT_STATE_NONE);
        }

        if(UART_INTERRUPT_STATE_NONE != event_temp)
        {
            TEMP_UART_CALLBACK.callback(event_temp, TEMP_UART_CALLBACK.parameter_ptr);
        }
        TEMP_UART_INDEX->ICR |= 0xFFFFFFFF;
    }
    else
    {
        IRQ_PROLOGUE();
        __uart_serve_interrupt(&DRV_UART1);
        IRQ_EPILOGUE();
    }

    #undef  TEMP_UART_INDEX 
    #undef  TEMP_UART_CALLBACK 

    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART2 的串口中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              uart_set_interrupt_priority(uart_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_UART2_HANDLER (void)
{
    #define TEMP_UART_INDEX         UART2
    #define TEMP_UART_CALLBACK      uart_callback[UART_2]

    if(uart_obj_list[UART_2].baudrate)
    {
        uint32 event_temp = UART_INTERRUPT_STATE_NONE;
        if((TEMP_UART_INDEX->CR1 & UART_CR1_RXNEIE_RXFNEIE))
        {
            event_temp |= (TEMP_UART_INDEX->ISR & UART_ISR_RXNE_RXFNE) ? (UART_INTERRUPT_STATE_RX) : (UART_INTERRUPT_STATE_NONE);
        }
        if((TEMP_UART_INDEX->CR1 & UART_CR1_TXEIE_TXFNFIE))
        {
            event_temp |= (TEMP_UART_INDEX->ISR & UART_ISR_TXE_TXFNF) ? (UART_INTERRUPT_STATE_TX) : (UART_INTERRUPT_STATE_NONE);
        }

        if(UART_INTERRUPT_STATE_NONE != event_temp)
        {
            TEMP_UART_CALLBACK.callback(event_temp, TEMP_UART_CALLBACK.parameter_ptr);
        }
        TEMP_UART_INDEX->ICR |= 0xFFFFFFFF;
    }
    else
    {
        IRQ_PROLOGUE();
        __uart_serve_interrupt(&DRV_UART2);
        IRQ_EPILOGUE();
    }

    #undef  TEMP_UART_INDEX 
    #undef  TEMP_UART_CALLBACK 

    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART3 的串口中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              uart_set_interrupt_priority(uart_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_UART3_HANDLER (void)
{
    #define TEMP_UART_INDEX         UART3
    #define TEMP_UART_CALLBACK      uart_callback[UART_3]

    if(uart_obj_list[UART_3].baudrate)
    {
        uint32 event_temp = UART_INTERRUPT_STATE_NONE;
        if((TEMP_UART_INDEX->CR1 & UART_CR1_RXNEIE_RXFNEIE))
        {
            event_temp |= (TEMP_UART_INDEX->ISR & UART_ISR_RXNE_RXFNE) ? (UART_INTERRUPT_STATE_RX) : (UART_INTERRUPT_STATE_NONE);
        }
        if((TEMP_UART_INDEX->CR1 & UART_CR1_TXEIE_TXFNFIE))
        {
            event_temp |= (TEMP_UART_INDEX->ISR & UART_ISR_TXE_TXFNF) ? (UART_INTERRUPT_STATE_TX) : (UART_INTERRUPT_STATE_NONE);
        }

        if(UART_INTERRUPT_STATE_NONE != event_temp)
        {
            TEMP_UART_CALLBACK.callback(event_temp, TEMP_UART_CALLBACK.parameter_ptr);
        }
        TEMP_UART_INDEX->ICR |= 0xFFFFFFFF;
    }
    else
    {
        IRQ_PROLOGUE();
        __uart_serve_interrupt(&DRV_UART3);
        IRQ_EPILOGUE();
    }

    #undef  TEMP_UART_INDEX 
    #undef  TEMP_UART_CALLBACK 

    ZF_DSB();
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 串口中断处理函数 isr.c 中对应串口中断服务函数调用
// 参数说明     event               中断事件
// 参数说明     *ptr                默认传入是一个 fifo 对象
// 返回参数     void
// 使用示例     debug_interrupr_handler();
// 备注信息     本函数需要开启 DEBUG_UART_USE_INTERRUPT 宏定义才可使用
//-------------------------------------------------------------------------------------------------------------------
void debug_uart_interrupr_handler (uint32 event, void *ptr)
{
	(void)event;
    uint8 debug_uart_data;
    zf_uart_query_byte(DEBUG_UART_INDEX, &debug_uart_data);                     // 读取串口数据
    zf_fifo_write_element((zf_fifo_obj_struct *)ptr, debug_uart_data);          // 存入 FIFO
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 串口字符输出接口
// 参数说明     *str                需要输出的字符串
// 返回参数     void
// 使用示例     debug_uart_str_output_handler("Log message");
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
void debug_uart_str_output_handler (const char *str)
{
    zf_uart_write_string(DEBUG_UART_INDEX, str);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 串口输出接口
// 参数说明     *buff           数据地址
// 参数说明     *length         长度
// 返回参数     uint32          剩余未发送数据长度
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
uint8 debug_uart_write_buffer (const uint8 *buff, uint32 *length)
{
	return zf_uart_write_buffer(DEBUG_UART_INDEX, buff, *length);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 串口读取接口
// 参数说明     *buff           数据地址
// 参数说明     *length         长度
// 返回参数     void
// 使用示例     debug_uart_str_output_handler("Log message");
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
uint8 debug_uart_read_buffer (uint8 *buff, uint32 *length)
{
    if(DEBUG_UART_USE_INTERRUPT)
    {
        debug_read_ring_buffer(buff, *length);
    }
    else
    {
        zf_uart_read_buffer(DEBUG_UART_INDEX, buff, *length);
    }
    return ZF_NO_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 串口传输接口
// 参数说明     *out                输出数据缓冲区
// 参数说明     out_len             输出长度
// 参数说明     *in                 接收数据缓冲区
// 参数说明     in_len              接收长度
// 返回参数     void
// 使用示例     debug_uart_transfer_handler(out, out_len, in, in_len);
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
void debug_uart_transfer_handler (const uint8 *out, uint32 out_len, uint8 *in, uint32 in_len)
{
    if(NULL != out)
    {
        zf_uart_write_buffer(DEBUG_UART_INDEX, out, out_len);
    }
    if(NULL != in)
    {
        if(DEBUG_UART_USE_INTERRUPT)
        {
            while(!debug_read_ring_buffer(in, in_len));
        }
        else
        {
            zf_uart_read_buffer(DEBUG_UART_INDEX, in, in_len);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 串口初始化操作
// 参数说明     *obj                Debug 对象指针
// 参数说明     callback            回调函数 为无返回值 uint32 加 void * 参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void
// 使用示例     debug_uart_init_handler(obj, callback, ptr);
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
void debug_uart_init_handler (debug_obj_struct *obj, void_callback_uint32_ptr callback, void *ptr)
{
    zf_uart_init(
        DEBUG_UART_INDEX,                                                       // 在 zf_common_debug.h 中查看对应值
        DEBUG_UART_BAUDRATE,                                                    // 在 zf_common_debug.h 中查看对应值
        DEBUG_UART_TX_PIN,                                                      // 在 zf_common_debug.h 中查看对应值
        DEBUG_UART_RX_PIN);                                                     // 在 zf_common_debug.h 中查看对应值

    obj->device_index = DEBUG_UART_INDEX;
    if(NULL != ptr)
    {
        obj->type_index = DEBUG_OBJ_TYPE_UART_INTERRUPT;
        zf_uart_set_interrupt_callback(DEBUG_UART_INDEX, callback, ptr);
        zf_uart_set_interrupt_config(DEBUG_UART_INDEX, UART_INTERRUPT_CONFIG_RX_ENABLE);
    }

}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 字节输出
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     data                需要发送的字节
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_write_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_write_byte (zf_uart_index_enum uart_index, const uint8 data)
{
    zf_uart_operation_state_enum     return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }

        if(ZF_DISABLE == uart_obj_list[uart_index].enable)
        {
            return_state = UART_WARNING_MODULE_DISABLE;                         // UART 模块失能禁用 操作中断退出
            break;
        }

        vuint32 timeout_count   = 0;
        uart_obj_list[uart_index].uart_ptr->TDR = data;
        while(  timeout_count ++ <= UART_DEFAULT_TIMEOUT_COUNT
            &&  !(uart_obj_list[uart_index].uart_ptr->ISR & UART_ISR_TXE_TXFNF));   // 等待发送完成
        if(timeout_count >= UART_DEFAULT_TIMEOUT_COUNT)
        {
            return_state = UART_WARNING_OPERATION_TIMEOUT;                      // UART 操作超时退出 操作中断退出
            break;
        }

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 发送数组
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     *buff               要发送的数组地址
// 参数说明     len                 发送长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_write_buffer(uart_index, buff, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_write_buffer (zf_uart_index_enum uart_index, const uint8 *buff, uint32 len)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }
        if(zf_uart_assert(NULL != buff))                                        // 检查 数据缓冲
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = UART_ERROR_DATA_BUFFER_NULL;                         // UART 数据指针异常 操作无法进行
            break;
        }

        if(ZF_DISABLE == uart_obj_list[uart_index].enable)
        {
            return_state = UART_WARNING_MODULE_DISABLE;                         // UART 模块失能禁用 操作中断退出
            break;
        }

        return_state = UART_WARNING_OPERATION_TIMEOUT;
        while(len --)                                                           // 循环到发送完
        {
            vuint32 timeout_count   = 0;
            uart_obj_list[uart_index].uart_ptr->TDR = *buff ++;
            while(  timeout_count ++ <= UART_DEFAULT_TIMEOUT_COUNT
                &&  !(uart_obj_list[uart_index].uart_ptr->ISR & UART_ISR_TXE_TXFNF));   // 等待发送完成
            if(timeout_count >= UART_DEFAULT_TIMEOUT_COUNT)
            {
                return_state = UART_WARNING_OPERATION_TIMEOUT;                  // UART 操作超时退出 操作中断退出
                break;
            }
        }

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 发送字符串
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     *str                要发送的字符串地址
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_write_string(uart_index, str); 
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_write_string (zf_uart_index_enum uart_index, const char *str)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }
        if(zf_uart_assert(NULL != str))                                         // 检查 数据缓冲
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = UART_ERROR_DATA_BUFFER_NULL;                         // UART 数据指针异常 操作无法进行
            break;
        }

        if(ZF_DISABLE == uart_obj_list[uart_index].enable)
        {
            return_state = UART_WARNING_MODULE_DISABLE;                         // UART 模块失能禁用 操作中断退出
            break;
        }

        return_state = UART_WARNING_OPERATION_TIMEOUT;
        while(*str)                                                             // 一直循环到结尾
        {
            vuint32 timeout_count   = 0;
            uart_obj_list[uart_index].uart_ptr->TDR = *str ++;
            while(  timeout_count ++ <= UART_DEFAULT_TIMEOUT_COUNT
                &&  !(uart_obj_list[uart_index].uart_ptr->ISR & UART_ISR_TXE_TXFNF));   // 等待发送完成
            if(timeout_count >= UART_DEFAULT_TIMEOUT_COUNT)
            {
                return_state = UART_WARNING_OPERATION_TIMEOUT;                  // UART 操作超时退出 操作中断退出
                break;
            }
        }

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 读取接收的数据（阻塞式 whlie 等待 会超时退出）
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
//                                  UART_WARNING_OPERATION_TIMEOUT 标识超过读取时间限制
// 使用示例     zf_uart_read_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_read_byte (zf_uart_index_enum uart_index, uint8 *data)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }
        if(zf_uart_assert(NULL != data))                                        // 检查 数据缓冲
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = UART_ERROR_DATA_BUFFER_NULL;                         // UART 数据指针异常 操作无法进行
            break;
        }

        if(ZF_DISABLE == uart_obj_list[uart_index].enable)
        {
            return_state = UART_WARNING_MODULE_DISABLE;                         // UART 模块失能禁用 操作中断退出
            break;
        }

        vuint32 timeout_count   = 0;
        while(  timeout_count ++ <= UART_DEFAULT_TIMEOUT_COUNT
            &&  !(uart_obj_list[uart_index].uart_ptr->ISR & UART_ISR_RXNE_RXFNE));  // 等待读取到一个数据
        if(timeout_count >= UART_DEFAULT_TIMEOUT_COUNT)
        {
            return_state = UART_WARNING_OPERATION_TIMEOUT;                      // UART 操作超时退出 操作中断退出
            break;
        }
        *data = uart_obj_list[uart_index].uart_ptr->RDR;

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 读取接收的数据（查询接收）
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     art_query_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_query_byte (zf_uart_index_enum uart_index, uint8 *data)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }
        if(zf_uart_assert(NULL != data))                                        // 检查 数据缓冲
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = UART_ERROR_DATA_BUFFER_NULL;                         // UART 数据指针异常 操作无法进行
            break;
        }

        if(ZF_DISABLE == uart_obj_list[uart_index].enable)
        {
            return_state = UART_WARNING_MODULE_DISABLE;                         // UART 模块失能禁用 操作中断退出
            break;
        }

        return_state = UART_WARNING_NO_DATA;
        if(uart_obj_list[uart_index].uart_ptr->ISR & UART_ISR_RXNE_RXFNE)       // 读取到一个数据
        {
            *data = uart_obj_list[uart_index].uart_ptr->RDR;                    // 存储一个数据
            return_state = UART_OPERATION_DONE;
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 读取接收的数据（阻塞式 whlie 等待 会超时退出）
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     *data               数据存放缓冲区
// 参数说明     len                 读取长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
//                                  UART_WARNING_OPERATION_TIMEOUT 标识超过读取时间限制
// 使用示例     zf_uart_read_buffer(uart_index, buff, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_read_buffer (zf_uart_index_enum uart_index, uint8 *buff, uint32 len)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }
        if(zf_uart_assert(NULL != buff))                                        // 检查 数据缓冲
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = UART_ERROR_DATA_BUFFER_NULL;                         // UART 数据指针异常 操作无法进行
            break;
        }

        if(ZF_DISABLE == uart_obj_list[uart_index].enable)
        {
            return_state = UART_WARNING_MODULE_DISABLE;                         // UART 模块失能禁用 操作中断退出
            break;
        }

        while(len -- > 0)
        {
            vuint32 timeout_count   = 0;
            while(  timeout_count ++ <= UART_DEFAULT_TIMEOUT_COUNT
                &&  !(uart_obj_list[uart_index].uart_ptr->ISR & UART_ISR_RXNE_RXFNE));  // 等待读取到一个数据
            if(timeout_count >= UART_DEFAULT_TIMEOUT_COUNT)
            {
                return_state = UART_WARNING_OPERATION_TIMEOUT;                  // UART 操作超时退出 操作中断退出
                break;
            }
            *buff ++ = uart_obj_list[uart_index].uart_ptr->RDR;
        }

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 中断设置回调函数
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     callback            回调函数 为无返回值 uint32 加 void * 参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_set_interrupt_callback(uart_index, callback, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_set_interrupt_callback (zf_uart_index_enum uart_index, void_callback_uint32_ptr callback, void *ptr)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(NULL != callback))                                    // 检查 回调函数
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = UART_ERROR_INTERRUPT_CALLBACK_ILLEGAL;               // UART 中断回调异常 操作无法进行
            break;
        }

        // 修改回调函数需要先禁止中断触发
        // 避免在修改过程中触发中断导致程序异常

        // 由于本模块中断并不是绑定初始化
        // 因此不能确定设置中断回调时是否初始化过模块
        // 所以不能直接操作模块的寄存器来开关中断
        // 那么只能去操作 NVIC 来直接关掉中断
        if(uart_obj_list[uart_index].tx_interrupt || uart_obj_list[uart_index].rx_interrupt)
        {
        	zf_interrupt_disable(uart_irq_index_list[uart_index]);
        }
        uart_callback[uart_index].callback      = callback;
        uart_callback[uart_index].parameter_ptr = ptr;
        if(uart_obj_list[uart_index].tx_interrupt || uart_obj_list[uart_index].rx_interrupt)
        {
        	zf_interrupt_enable(uart_irq_index_list[uart_index]);
        }

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 设置中断配置
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     config              中断配置        (详见 zf_driver_uart.h 内 zf_uart_interrupt_config_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_set_interrupt_config(uart_index, config);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_set_interrupt_config (zf_uart_index_enum uart_index, zf_uart_interrupt_config_enum config)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }
        if(zf_uart_assert(                                                      // Debug 输出检测查
                UART_INTERRUPT_CONFIG_TX_ENABLE <= config                       // 开启中断
            &&  NULL != uart_callback[uart_index].callback                      // 回调不能为空
            &&  zf_uart_callbakc_defalut != uart_callback[uart_index].callback))// 回调不能为默认
        {
            // 此处如果断言报错 那么证明你想要开启中断 但是没有设置中断回调函数
            // 必须在开启中断之前先设置好中断回调函数
            return_state = UART_ERROR_INTERRUPT_CALLBACK_NOT_SET;               // UART 中断回调未设 操作无法进行
            break;
        }

        zf_interrupt_disable(uart_irq_index_list[uart_index]);
        uint32 register_temp = uart_obj_list[uart_index].uart_ptr->CR1;
        switch(config)
        {
            case UART_INTERRUPT_CONFIG_TX_DISABLE:
            {
                uart_obj_list[uart_index].tx_interrupt = ZF_DISABLE;
                register_temp &= ~(UART_CR1_TXEIE_TXFNFIE);
            }break;
            case UART_INTERRUPT_CONFIG_RX_DISABLE:
            {
                uart_obj_list[uart_index].rx_interrupt = ZF_DISABLE;
                register_temp &= ~(UART_CR1_RXNEIE_RXFNEIE);
            }break;
            case UART_INTERRUPT_CONFIG_ALL_DISABLE:
            {
                uart_obj_list[uart_index].tx_interrupt = ZF_DISABLE;
                uart_obj_list[uart_index].rx_interrupt = ZF_DISABLE;
                register_temp &= ~(UART_CR1_TXEIE_TXFNFIE |UART_CR1_RXNEIE_RXFNEIE);
            }break;
            case UART_INTERRUPT_CONFIG_TX_ENABLE:
            {
                uart_obj_list[uart_index].tx_interrupt = ZF_ENABLE;
                register_temp |= UART_CR1_TXEIE_TXFNFIE;
            }break;
            case UART_INTERRUPT_CONFIG_RX_ENABLE:
            {
                uart_obj_list[uart_index].rx_interrupt = ZF_ENABLE;
                register_temp |= UART_CR1_RXNEIE_RXFNEIE;
            }break;
            case UART_INTERRUPT_CONFIG_ALL_ENABLE:
            {
                uart_obj_list[uart_index].tx_interrupt = ZF_ENABLE;
                uart_obj_list[uart_index].rx_interrupt = ZF_ENABLE;
                register_temp |= (UART_CR1_TXEIE_TXFNFIE |UART_CR1_RXNEIE_RXFNEIE);
            }break;
        }
        uart_obj_list[uart_index].uart_ptr->CR1 = register_temp;
        if((UART_CR1_TXEIE_TXFNFIE | UART_CR1_RXNEIE_RXFNEIE) & register_temp)
        {
        	zf_interrupt_enable(uart_irq_index_list[uart_index]);
        }

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 中断设置优先级
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     priority            中断优先级
// 返回参数     void
// 使用示例     zf_uart_set_interrupt_priority(uart_index, priority);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_uart_set_interrupt_priority (zf_uart_index_enum uart_index, uint8 priority)
{
	zf_interrupt_set_priority(uart_irq_index_list[uart_index], priority);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 配置设置
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     item                UART 配置       (详见 zf_driver_uart.h 内 zf_uart_config_item_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_set_config(uart_index, item);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_set_config (zf_uart_index_enum uart_index, zf_uart_config_item_enum item)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }

        uint8   enable  = (uart_obj_list[uart_index].uart_ptr->CR1 & UART_CR1_UE) ? (1) : (0);
        uint32  register_temp = 0;

        zf_interrupt_disable(uart_irq_index_list[uart_index]);
        uart_obj_list[uart_index].uart_ptr->CR1 &= ~UART_CR1_UE;

        switch((item >> UART_CONFIG_TYPE_OFFSET) & UART_CONFIG_TYPE_MASK)
        {
            case UART_CONFIG_TYPE_DATA_WIDTH:
            {
                uart_obj_list[uart_index].data_width = (item & UART_CONFIG_VALUE_MASK);
                register_temp = uart_obj_list[uart_index].uart_ptr->CR1;
                register_temp &= ~0x10001000;
                switch(item & UART_CONFIG_VALUE_MASK)
                {
                    case UART_DATA_WIDTH_7BIT   :   register_temp |= 0x10000000;    break;
                    case UART_DATA_WIDTH_8BIT   :                                   break;
                }
                uart_obj_list[uart_index].uart_ptr->CR1 = register_temp;
            }break;
            case UART_CONFIG_TYPE_STOP_WIDTH:
            {
                uart_obj_list[uart_index].stop_width = (item & UART_CONFIG_VALUE_MASK);
                register_temp = uart_obj_list[uart_index].uart_ptr->CR2;
                register_temp &= ~0x00003000;
                switch(item & UART_CONFIG_VALUE_MASK)
                {
                    case UART_STOP_WIDTH_0BIT_5 :   register_temp |= 0x00001000;    break;
                    case UART_STOP_WIDTH_1BIT   :                                   break;
                    case UART_STOP_WIDTH_1BIT_5 :   register_temp |= 0x00003000;    break;
                    case UART_STOP_WIDTH_2BIT   :   register_temp |= 0x00002000;    break;
                }
                uart_obj_list[uart_index].uart_ptr->CR2 = register_temp;
            }break;
            case UART_CONFIG_TYPE_PARITY_MODE:
            {
                uart_obj_list[uart_index].parity = (item & UART_CONFIG_VALUE_MASK);
                register_temp = uart_obj_list[uart_index].uart_ptr->CR1;
                register_temp &= ~0x00000600;
                switch(item & UART_CONFIG_VALUE_MASK)
                {
                    case UART_PARITY_MODE_NONE  :
                    {
                        register_temp &= ~0x10001000;
                        if(UART_DATA_WIDTH_7BIT == uart_obj_list[uart_index].data_width)
                        {
                            register_temp |= 0x10000000;
                        }
                    }break;
                    case UART_PARITY_MODE_EVEN  :
                    {
                        register_temp &= ~0x10001000;
                        if(UART_DATA_WIDTH_8BIT == uart_obj_list[uart_index].data_width)
                        {
                            register_temp |= 0x00001000;
                        }
                        register_temp |= 0x00000400;
                    }break;
                    case UART_PARITY_MODE_ODD   :
                    {
                        register_temp &= ~0x10001000;
                        if(UART_DATA_WIDTH_8BIT == uart_obj_list[uart_index].data_width)
                        {
                            register_temp |= 0x00001000;
                        }
                        register_temp |= 0x00000600;
                    }break;
                }
                uart_obj_list[uart_index].uart_ptr->CR1 = register_temp;
            }break;
            case UART_CONFIG_TYPE_HWCONTROL_MODE:
            {
                uart_obj_list[uart_index].hwcontrol = (item & UART_CONFIG_VALUE_MASK);
                register_temp = uart_obj_list[uart_index].uart_ptr->CR3;
                register_temp &= ~0x00000300;
                switch(item & UART_CONFIG_VALUE_MASK)
                {
                    case UART_HWCONTROL_MODE_NONE   :                                   break;
                    case UART_HWCONTROL_MODE_RTS    :   register_temp |= 0x00000100;    break;
                    case UART_HWCONTROL_MODE_CTS    :   register_temp |= 0x00000200;    break;
                    case UART_HWCONTROL_MODE_ALL    :   register_temp |= 0x00000300;    break;
                }
                uart_obj_list[uart_index].uart_ptr->CR3 = register_temp;
            }break;
        }

        uart_obj_list[uart_index].uart_ptr->CR1 |= (enable) ? (UART_CR1_UE) :(0);
        uart_obj_list[uart_index].uart_ptr->ICR = UART_INTERRUPT_STATE_ALL;
        if(uart_obj_list[uart_index].tx_interrupt || uart_obj_list[uart_index].rx_interrupt)
        {
            zf_interrupt_enable(uart_irq_index_list[uart_index]);
        }

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 设置波特率
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     baudrate            波特率
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_set_baudrate(uart_index, baudrate);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_set_baudrate (zf_uart_index_enum uart_index, uint32 baudrate)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }
        if(zf_uart_assert(0 != baudrate))
        {
            // 此处如果断言报错 那么证明输入波特率为 0 是不合理的
            return_state = UART_ERROR_BAUDRATE_UNSUPPORTED;
            break;
        }

        zf_interrupt_disable(uart_irq_index_list[uart_index]);
        uart_obj_list[uart_index].uart_ptr->CR1 &= ~UART_CR1_UE;

        uint8 freq_div = 1;
        for(freq_div = 1; 12 > freq_div; freq_div ++)
        {
            if(0xFFFF >= (UART_INPUT_CLOCK / baudrate / freq_div))
            {
                break;
            }
        }

        uart_obj_list[uart_index].baudrate = baudrate;
        uart_obj_list[uart_index].uart_ptr->PRESC = (freq_div - 1);
        uart_obj_list[uart_index].uart_ptr->BRR = (UART_INPUT_CLOCK / baudrate / freq_div);

        uart_obj_list[uart_index].baudrate  =
                UART_INPUT_CLOCK
            /   uart_obj_list[uart_index].uart_ptr->BRR
            /   (uart_obj_list[uart_index].uart_ptr->PRESC + 1) ;

        uart_obj_list[uart_index].uart_ptr->CR1 |= UART_CR1_UE;
        uart_obj_list[uart_index].uart_ptr->ICR = UART_INTERRUPT_STATE_ALL;
        if(uart_obj_list[uart_index].tx_interrupt || uart_obj_list[uart_index].rx_interrupt)
        {
            zf_interrupt_enable(uart_irq_index_list[uart_index]);
        }

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 使能
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_enable(uart_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_enable (zf_uart_index_enum uart_index)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }

        uart_obj_list[uart_index].uart_ptr->CR1 |= UART_CR1_UE;
        uart_obj_list[uart_index].enable = ZF_ENABLE;

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 禁止
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_disable(uart_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_disable (zf_uart_index_enum uart_index)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }

        uart_obj_list[uart_index].uart_ptr->CR1 &= ~UART_CR1_UE;
        uart_obj_list[uart_index].enable = ZF_DISABLE;

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 注销初始化
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_deinit(uart_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_deinit (zf_uart_index_enum uart_index)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(uart_obj_list[uart_index].baudrate))                  // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = UART_ERROR_MODULE_NOT_INIT;                          // UART 模块未初始化 操作无法进行
            break;
        }

        zf_gpio_deinit(uart_obj_list[uart_index].tx_pin);
        zf_gpio_deinit(uart_obj_list[uart_index].rx_pin);

        zf_rcc_peripheral_reset(uart_rcc_index_list[uart_index]);
        zf_rcc_clock_disable(uart_rcc_index_list[uart_index]);

        zf_interrupt_disable(uart_irq_index_list[uart_index]);
        uart_callback[uart_index].callback      = zf_uart_callbakc_defalut;
        uart_callback[uart_index].parameter_ptr = &uart_obj_list[uart_index];

        uart_obj_list[uart_index].tx_pin        = PIN_NULL;
        uart_obj_list[uart_index].rx_pin        = PIN_NULL;
        uart_obj_list[uart_index].baudrate      = 0;
        uart_obj_list[uart_index].config_info   = 0;

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 初始化
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     baudrate            UART 波特率
// 参数说明     tx_pin              UART 发送引脚   (详见 zf_driver_uart.h 内 zf_uart_tx_pin_enum 定义)
// 参数说明     rx_pin              UART 接收引脚   (详见 zf_driver_uart.h 内 zf_uart_rx_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_init(uart_index, baudrate, tx_pin, rx_pin);
// 备注信息     已经初始化过的 UART 不可以反复初始化 可以通过接口修改波特率 或者通过 deinit 后再调用 init
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_init (zf_uart_index_enum uart_index, uint32 baudrate, zf_uart_tx_pin_enum tx_pin, zf_uart_rx_pin_enum rx_pin)
{
    zf_uart_operation_state_enum    return_state    =   UART_ERROR_UNKNOW;

    do
    {
        if(zf_uart_assert(!uart_obj_list[uart_index].baudrate))                 // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块已经被初始化过一次 重复初始化是不允许的
            // 否则应当先注销初始化之后再重新初始化
            return_state = UART_ERROR_MODULE_OCCUPIED;                          // UART 模块未初始化 操作无法进行
            break;
        }
        if(zf_uart_assert(0 != baudrate))
        {
            // 此处如果断言报错 那么证明输入波特率为 0 是不合理的
            return_state = UART_ERROR_BAUDRATE_UNSUPPORTED;
            break;
        }
        if(zf_uart_assert(
            uart_index == ((tx_pin >> UART_INDEX_OFFSET) & UART_INDEX_MASK)))                                                     // 输出错误码 具体问题查看函数上方注释
        {
            // 此处如果断言报错 那么证明 TX 引脚跟 UART 索引不匹配
            return_state = UART_ERROR_TX_PIN_MISMATCH;
            break;
        }
        if(zf_uart_assert(
            uart_index == ((rx_pin >> UART_INDEX_OFFSET) & UART_INDEX_MASK)))                                                     // 输出错误码 具体问题查看函数上方注释
        {
            // 此处如果断言报错 那么证明 RX 引脚跟 UART 索引不匹配
            return_state = UART_ERROR_RX_PIN_MISMATCH;
            break;
        }
        zf_rcc_clock_enable(uart_rcc_index_list[uart_index]);

        uart_obj_list[uart_index].tx_interrupt  = ZF_DISABLE;
        uart_obj_list[uart_index].rx_interrupt  = ZF_DISABLE;

        uart_obj_list[uart_index].data_width    = (UART_CONFIG_DATA_WIDTH_8BIT  & UART_CONFIG_VALUE_MASK);
        uart_obj_list[uart_index].stop_width    = (UART_CONFIG_STOP_BIT_1BIT    & UART_CONFIG_VALUE_MASK);
        uart_obj_list[uart_index].parity        = (UART_CONFIG_PARITY_BIT_NONE  & UART_CONFIG_VALUE_MASK);
        uart_obj_list[uart_index].hwcontrol     = (UART_CONFIG_HWCONTROL_NONE   & UART_CONFIG_VALUE_MASK);

        uart_obj_list[uart_index].tx_pin        = (zf_gpio_pin_enum)(tx_pin & UART_PIN_INDEX_MASK);
        uart_obj_list[uart_index].rx_pin        = (zf_gpio_pin_enum)(rx_pin & UART_PIN_INDEX_MASK);
        zf_afio_init(uart_obj_list[uart_index].tx_pin,
            GPO_AF_PUSH_PULL,
            (zf_gpio_af_enum)((tx_pin >> UART_PIN_AF_OFFSET) & UART_PIN_AF_MASK));
        zf_afio_init(uart_obj_list[uart_index].rx_pin,
            GPO_AF_OPEN_DRAIN,
            (zf_gpio_af_enum)((rx_pin >> UART_PIN_AF_OFFSET) & UART_PIN_AF_MASK));

        {
            uint8 freq_div = 1;
            for(freq_div = 1; 12 > freq_div; freq_div ++)
            {
                if(0xFFFF >= (UART_INPUT_CLOCK / baudrate / freq_div))
                {
                    break;
                }
            }

            uart_obj_list[uart_index].baudrate = baudrate;
            uart_obj_list[uart_index].uart_ptr->PRESC = (freq_div - 1);
            uart_obj_list[uart_index].uart_ptr->BRR = (UART_INPUT_CLOCK / baudrate / freq_div);

            uart_obj_list[uart_index].baudrate  =
            		UART_INPUT_CLOCK
                /   uart_obj_list[uart_index].uart_ptr->BRR
                /   (uart_obj_list[uart_index].uart_ptr->PRESC + 1) ;

            uint32 register_temp = UART_CR1_RE | UART_CR1_TE;
            uart_obj_list[uart_index].uart_ptr->CR1 = register_temp;
            uart_obj_list[uart_index].uart_ptr->CR1 |= UART_CR1_UE;
        }

        uart_obj_list[uart_index].self_index = uart_index;
        uart_obj_list[uart_index].enable = ZF_ENABLE;

        return_state = UART_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
