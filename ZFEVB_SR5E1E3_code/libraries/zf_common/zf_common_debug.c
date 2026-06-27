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
* 文件名称          zf_common_debug
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

// zf_common 层引用
#include "zf_common_fifo.h"
#include "zf_common_memory.h"

// 自身头文件
#include "zf_common_debug.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION static debug_obj_struct    debug_output_obj =
{
    .type_index             = DEBUG_OBJ_TYPE_NULL,
    .display_x_max          = 0,
    .display_y_max          = 0,
    .font_x_size            = 0,
    .font_y_size            = 0,
    .device_obj             = NULL,
    .output_uart            = NULL,
    .output_screen_clear    = NULL
};

#if DEBUG_UART_USE_INTERRUPT                                                    // 如果启用 debug uart 接收中断
AT_ZF_LIB_SECTION static uint8               debug_uart_buffer[DEBUG_RING_BUFFER_LEN] = {0};
AT_ZF_LIB_SECTION static zf_fifo_obj_struct  debug_uart_fifo =
{
    .base_ptr       = 0,
    .tail_ptr       = 0,
    .write_ptr      = 0,
    .read_ptr       = 0,
    .step           = FIFO_DATA_8BIT,
    .state          = 0
};
#endif
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的外部重载函数 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     重定向 printf 到串口
// 参数说明     size                需要打印的长度
// 参数说明     buf                 数据流
// 返回参数     int                 输出的数据数量
// 备注信息     此函数由编译器自带库里的printf所调用
//-------------------------------------------------------------------------------------------------------------------
int _write(int fd, uint8_t *buf, int size)
{
    (void)fd;
    debug_uart_transfer_handler(buf, size, NULL, 0);
    return size;
}

#if !defined(__ICCARM__)

#include <stdio.h>
#include <errno.h>
#include <stdint.h>

void *_sbrk (intptr_t increment)
{
    extern uint8_t __heap_base__;
    extern uint8_t __heap_end__;

    static uint8_t *heap_base = &__heap_base__;
    uint8_t *prev_head_base;

    prev_head_base = heap_base;
    if ((heap_base + increment) > (uint8_t*) &__heap_end__)
    {
        errno = ENOMEM;
        return (void *) -1;
    }
    heap_base += increment;

    return prev_head_base;
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 保护处理 主要是防止断言后出现信号维持而导致硬件失控
// 参数说明     void
// 返回参数     void
// 使用示例     debug_pwm_protective_handler();
// 备注信息     本函数在文件内部调用 用户不用关注
//              本函数由底层在 zf_driver_pwm 中进行重载
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK void debug_pwm_protective_handler (void)
{
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 保护处理 主要是防止断言后出现依旧进行操作的现象
// 参数说明     void
// 返回参数     void
// 使用示例     debug_interrupt_protective_handler();
// 备注信息     本函数在文件内部调用 用户不用关注
//              本函数由底层在 zf_driver_interrupt 中进行重载
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK void debug_interrupt_protective_handler (void)
{
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 串口中断处理函数 isr.c 中对应串口中断服务函数调用
// 参数说明     event               中断事件
// 参数说明     *ptr                默认传入是一个 fifo 对象
// 返回参数     void
// 使用示例     debug_interrupr_handler();
// 备注信息     本函数需要开启 DEBUG_UART_USE_INTERRUPT 宏定义才可使用
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK void debug_uart_interrupr_handler (uint32 event, void *ptr)
{
    (void)event;
    (void)ptr;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 串口字符输出接口
// 参数说明     *str                需要输出的字符串
// 返回参数     void
// 使用示例     debug_uart_str_output_handler("Log message");
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK void debug_uart_str_output_handler (const char *str)
{
    (void)str;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 串口输出接口
// 参数说明     *buff           数据地址
// 参数说明     *length         长度
// 返回参数     uint32          剩余未发送数据长度
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK uint8 debug_uart_write_buffer (const uint8 *buff, uint32 *length)
{
    (void)buff;
    (void)length;
    // 当选择自定义通讯方式时 需要自行完成数据接收功能
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 串口读取接口
// 参数说明     *buff           数据地址
// 参数说明     *length         长度
// 返回参数     void
// 使用示例     debug_uart_str_output_handler("Log message");
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK uint8 debug_uart_read_buffer (uint8 *buff, uint32 *length)
{
    (void)buff;
    (void)length;
    // 当选择自定义通讯方式时 需要自行完成数据接收功能
    return 0;
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
ZF_WEAK void debug_uart_transfer_handler (const uint8 *out, uint32 out_len, uint8 *in, uint32 in_len)
{
    (void)out;
    (void)out_len;
    (void)in;
    (void)in_len;
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
ZF_WEAK void debug_uart_init_handler (debug_obj_struct *obj, void_callback_uint32_ptr callback, void *ptr)
{
    (void)obj;
    (void)callback;
    (void)ptr;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 软延时函数 各单片机需要根据各自时钟测试才能知道具体时长
// 参数说明     void
// 返回参数     void
// 使用示例     debug_delay();
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
static void debug_delay (void)
{
    vuint32 loop_1 = 0, loop_2 = 0;
    for(loop_1 = 0; 0xFF >= loop_1; loop_1 ++)
    {
        for(loop_2 = 0; 0xFFFF >= loop_2; loop_2 ++)
        {
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 输出接口
// 参数说明     *type               log 类型
// 参数说明     *file               文件名
// 参数说明     line                目标行数
// 参数说明     *str                信息
// 返回参数     void
// 使用示例     debug_output("Log message", file, line, str);
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
static void debug_output (char *type, char *file, int line, char *str)
{
    char    *file_str       = NULL          ;
    vuint16 temp_counter_1  = 0             ;
    vuint16 temp_counter_2  = 0             ;
    vint16  len_origin      = strlen(file)  ;
    vint16  show_len        = 0             ;
    vint16  show_line_index = 0             ;

    char    output_buffer[256]              ;
    char    file_path_buffer[64]            ;

    if(DEBUG_OBJ_TYPE_SCREEN == debug_output_obj.type_index)
    {
        debug_output_obj.output_screen_clear(debug_output_obj.device_obj);

        // 需要分行将文件的路径和行数输出
        // <不输出完整路径 只输出一级目录 例如 src/main.c>
        // 输出 line : xxxx

        // 首先输出 Log 类型 预设为断言输出 "Assert error" 调试信息输出 "Log message"
        debug_output_obj.output_screen(debug_output_obj.device_obj, 0, show_line_index ++, type);

        file_str    = file;
        len_origin  = strlen(file);
        show_len    = (debug_output_obj.display_x_max / debug_output_obj.font_x_size);

        // 首先把指针指向路径字符串尾部
        // 因为只取一级目录显示 否则有的 IDE 会输出绝对路径 太长了没必要
        // 只需要直接输出当前工程的相对目录就可以
        while('\0' != *file_str ++);
        for(temp_counter_2 = 0; (2 > temp_counter_2) && (0 <= len_origin); len_origin --)
        {                                                                       // 遍历路径 直到找到两个 '/' 或者遍历整个路径字符串
            file_str --;                                                        // 指针一个字符一个字符往前遍历
            if(('/' == *file_str) || (0x5C == *file_str))                       // 查找两个 '/'
            {
                temp_counter_2 ++;
            }
        }

        // 文件路径保存到数组中
        if(0 <= len_origin)
        {
            file_str ++;
            sprintf(output_buffer, "file: %s", file_str);
        }
        else
        {
            if(0 == temp_counter_2)
            {
                sprintf(output_buffer, "file: mdk/%s", file_str);
            }
            else
            {
                sprintf(output_buffer, "file: %s", file_str);
            }
        }

        // 屏幕显示路径
        // 这里就是根据屏幕的宽度来计算怎么显示
        // 能单行显示完毕的就一行显示完毕了
        // 需要多行显示的就会分成多行截断显示
        for(temp_counter_1 = 0; temp_counter_1 < ((strlen(output_buffer) / show_len) + 1); temp_counter_1 ++)
        {
            for(temp_counter_2 = 0; temp_counter_2 < show_len; temp_counter_2 ++)
            {
                if((uint16)strlen(output_buffer) < (temp_counter_2 + temp_counter_1 * show_len))
                {
                    break;
                }
                file_path_buffer[temp_counter_2] = output_buffer[temp_counter_2 + temp_counter_1 * show_len];
            }
            
            file_path_buffer[temp_counter_2] = '\0';                            // 末尾添加\0
            
            debug_output_obj.output_screen(debug_output_obj.device_obj, 0, debug_output_obj.font_y_size * show_line_index ++, file_path_buffer);
        }

        // 屏幕显示行号
        sprintf(output_buffer, "line: %d", line);
        debug_output_obj.output_screen(debug_output_obj.device_obj, 0, debug_output_obj.font_y_size * show_line_index ++, output_buffer);

        // 屏幕显示 Log 如果有的话
        // 同样是根据屏幕的宽度来计算怎么显示
        // 能单行显示完毕的就一行显示完毕了
        // 需要多行显示的就会分成多行截断显示
        if(NULL != str)
        {
            for(temp_counter_1 = 0; temp_counter_1 < ((strlen(str) / show_len) + 1); temp_counter_1 ++)
            {
                for(temp_counter_2 = 0; temp_counter_2 < show_len; temp_counter_2 ++)
                {
                    if((uint16)strlen(str) < (temp_counter_2 + temp_counter_1 * show_len))
                    {
                        break;
                    }
                    file_path_buffer[temp_counter_2] = str[temp_counter_2 + temp_counter_1 * show_len];
                }
                
                file_path_buffer[temp_counter_2] = '\0';                        // 末尾添加\0
                
                debug_output_obj.output_screen(debug_output_obj.device_obj, 0, debug_output_obj.font_y_size * show_line_index ++, file_path_buffer);
            }
        }
    }
    else
    {
        // 串口输出就没什么顾虑了
        // 最高 256byte 直接怼到串口就完事了
        memset(output_buffer, 0, 256);
        debug_output_obj.output_uart(type);
        if(NULL != str)
        {
            sprintf(output_buffer, "\r\nfile %s line %d: %s.\r\n", file, line, str);
        }
        else
        {
            sprintf(output_buffer, "\r\nfile %s line %d.\r\n", file, line);
        }
        debug_output_obj.output_uart(output_buffer);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 调试信息处理函数
// 参数说明     output_level        输出等级
// 参数说明     pass                条件判断
// 参数说明     *str                输出的信息
// 参数说明     *file               文件名
// 参数说明     line                目标行数
// 返回参数     uint8               返回 pass 判断值
// 使用示例     debug_message_handler(ZF_DEBUG_FILE, 0, "Error", ZF_DEBUG_FILE, ZF_DEBUG_LINE);
// 备注信息     此部分不允许用户更改
//              推荐使用 zf_commmon_debug.h 中的 zf_log(x, str) 接口
//-------------------------------------------------------------------------------------------------------------------
uint8 debug_message_handler (uint8 output_level, uint8 pass, char *str, char *file, int line)
{
    if(!pass)
    {
        switch(output_level)
        {
            case DEBUG_OUTPUT_LEVEL_ASSERT:
            {
                static uint8 assert_nest_index = 0;

                if(0 != assert_nest_index)
                {
                    while(1);
                }
                assert_nest_index ++;

                debug_interrupt_protective_handler();
                debug_pwm_protective_handler();

                while(1)
                {
                    // 如果代码跳转到这里停住了
                    // 一般你的函数参数传递出错了
                    // 或者你自己调用的 zf_assert(x) 接口处报错了

                    // 如果调用了 debug_init 初始化了 log 输出
                    // 就在对应串口输出去查看是哪个文件的哪一行报错

                    // 如果没有初始化 debug
                    // 那就看看这个 file 的字符串值和 line 的行数
                    // 那代表报错的文件路径名称和对应报错行数

                    // 再去调试看看是为什么参数出错

                    debug_output("Assert error", file, line, str);
                    debug_delay();
                }
            }break;
            case DEBUG_OUTPUT_LEVEL_LOG:
            {
                debug_output("Log message", file, line, str);
            }break;
            case DEBUG_OUTPUT_LEVEL_DISABLE:
            default:
            {
            }break;
        }
    }
    return !pass;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     读取 debug 环形缓冲区数据
// 参数说明     *data               读出数据存放的数组指针
// 参数说明     data_len            目标读出数据长度
// 返回参数     uint32              实际读出长度
// 使用示例     debug_read_ring_buffer(data, data_len);
// 备注信息     本函数需要开启 DEBUG_UART_USE_INTERRUPT 宏定义 否则直接返回
//-------------------------------------------------------------------------------------------------------------------
uint32 debug_read_ring_buffer (uint8 *data, uint32 data_len)
{
#if DEBUG_UART_USE_INTERRUPT                                                    // 条件编译 只有在启用串口中断才编译
    zf_fifo_read_buffer(&debug_uart_fifo, data, &data_len, FIFO_READ_WITH_CLEAN);
    return data_len;
#else
    return 0;
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 输出绑定初始化
// 参数说明     *obj                屏幕对象
// 参数说明     dispaly_x           屏幕横轴长
// 参数说明     dispaly_y           屏幕纵轴长
// 参数说明     font_x              字体宽度
// 参数说明     font_y              字体高度
// 参数说明     output_screen       屏幕字符串输出接口
// 参数说明     output_screen_clear 屏幕清空接口
// 返回参数     void
// 使用示例     debug_output_init(info);
// 备注信息     这个函数一般不由用户调用
//-------------------------------------------------------------------------------------------------------------------
void debug_screen_init (void *obj, uint16 dispaly_x, uint16 dispaly_y, uint8 font_x, uint8 font_y, debug_output_screen output_screen, debug_output_screen_clear output_screen_clear)
{
    do
    {
        if(NULL == obj)
        {
            break;
        }

        debug_output_obj.device_obj             = obj;
        debug_output_obj.type_index             = DEBUG_OBJ_TYPE_SCREEN;

        debug_output_obj.display_x_max          = dispaly_x;
        debug_output_obj.display_y_max          = dispaly_y;

        debug_output_obj.font_x_size            = font_x;
        debug_output_obj.font_y_size            = font_y;

        debug_output_obj.output_screen          = output_screen;
        debug_output_obj.output_screen_clear    = output_screen_clear;
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 串口初始化
// 参数说明     void
// 返回参数     void
// 使用示例     debug_init();
// 备注信息     开源库示例默认调用 但默认禁用中断接收
//-------------------------------------------------------------------------------------------------------------------
void debug_init (void)
{
    debug_output_obj.type_index     = DEBUG_OBJ_TYPE_UART;
    debug_output_obj.output_uart    = debug_uart_str_output_handler;

#if DEBUG_UART_USE_INTERRUPT                                                    // 条件编译 只有在启用串口中断才编译
    zf_fifo_init(&debug_uart_fifo, FIFO_DATA_8BIT, debug_uart_buffer, DEBUG_RING_BUFFER_LEN);
    debug_uart_init_handler(&debug_output_obj, debug_uart_interrupr_handler, &debug_uart_fifo);
#else
    debug_uart_init_handler(&debug_output_obj, debug_uart_interrupr_handler, NULL);
#endif
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
