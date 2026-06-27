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

#ifndef _zf_driver_uart_h_
#define _zf_driver_uart_h_

// SDK 底层驱动
#include <uart.h>

// zf_common 层引用
#include "zf_common_typedef.h"

// zf_driver 层 类型定义 引用
#include "zf_driver_gpio.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_uart_write_byte                                                           // UART 字节输出
// zf_uart_write_buffer                                                         // UART 发送数组
// zf_uart_write_string                                                         // UART 发送字符串

// zf_uart_read_byte                                                            // UART 读取接收的数据（阻塞式 whlie等待）
// zf_uart_query_byte                                                           // UART 读取接收的数据（查询接收）

// zf_uart_set_interrupt_callback                                               // UART 中断设置回调函数
// zf_uart_set_interrupt_config                                                 // UART 设置中断配置
// zf_uart_set_interrupt_priority                                               // UART 设置中断优先级

// zf_uart_set_config                                                           // UART 配置设置
// zf_uart_set_baudrate                                                         // UART 设置波特率

// zf_uart_enable                                                               // UART 使能
// zf_uart_disable                                                              // UART 禁止

// zf_uart_deinit                                                               // UART 注销初始化
// zf_uart_init                                                                 // UART 初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 UART 相关的结构体数据构成细节 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define     UART_NUM_MAX            (  3     )                                  // 最多 UART_NUM_MAX 个 UART

typedef enum                                                                    // 枚举 UART 索引 此枚举定义不允许用户修改
{
    UART_1  ,   UART_2  ,   UART_3  ,
}zf_uart_index_enum;

// 定义引脚要用的宏定义 根据各单片机不同 可以自由定义需要什么信息
// bit[11:0 ] 固定为引脚索引
// bit[15:12] 固定为引脚复用
// bit[19:16] 在 UART 模块中为 UART 索引

#define     UART_PIN_INDEX_OFFSET   ( 0      )                                  // bit[11:0 ] 存储 GPIO 的索引号
#define     UART_PIN_INDEX_MASK     ( 0x0FFF )                                  // 宽度 12bit 因此掩码为 0x0FFF

#define     UART_PIN_AF_OFFSET      ( 12     )                                  // bit[15:12] 存储 GPIO 的复用功能索引
#define     UART_PIN_AF_MASK        ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0x0F

#define     UART_INDEX_OFFSET       ( 16     )                                  // bit[19:16] 存储 UART 索引
#define     UART_INDEX_MASK         ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0x0F

typedef enum                                                                    // 枚举 UART 引脚 此枚举定义不允许用户修改
{
    // UART_1 <TX> 引脚 具体的复用值通过芯片手册查看确定
    UART1_TX_F3     = ((UART_1 << UART_INDEX_OFFSET) | (GPIO_AF7  << UART_PIN_AF_OFFSET) | (F3 )),

    // UART_2 <TX> 引脚 具体的复用值通过芯片手册查看确定
    UART2_TX_A7     = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF7  << UART_PIN_AF_OFFSET) | (A7 )),
    UART2_TX_D8     = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF7  << UART_PIN_AF_OFFSET) | (D8 )),

    // UART_3 <TX> 引脚 具体的复用值通过芯片手册查看确定
    // 需要注意 E9 /E10 选作默认的 Debug UART    谨慎使用
    UART3_TX_E10    = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF7  << UART_PIN_AF_OFFSET) | (E10)),
    UART3_TX_G15    = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF7  << UART_PIN_AF_OFFSET) | (G15)),

}zf_uart_tx_pin_enum;

typedef enum                                                                    // 枚举 UART 引脚 此枚举定义不允许用户修改
{
    // UART_1 <RX> 引脚 具体的复用值通过芯片手册查看确定
    UART1_RX_F2     = ((UART_1 << UART_INDEX_OFFSET) | (GPIO_AF7  << UART_PIN_AF_OFFSET) | (F2 )),

    // UART_2 <RX> 引脚 具体的复用值通过芯片手册查看确定
    UART2_RX_A6     = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF7  << UART_PIN_AF_OFFSET) | (A6 )),
    UART2_RX_D7     = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF7  << UART_PIN_AF_OFFSET) | (D7 )),

    // UART_3 <RX> 引脚 具体的复用值通过芯片手册查看确定
    // 需要注意 E9 /E10 选作默认的 Debug UART    谨慎使用
    UART3_RX_E9     = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF7  << UART_PIN_AF_OFFSET) | (E9 )),
    UART3_RX_G14    = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF7  << UART_PIN_AF_OFFSET) | (G14)),

}zf_uart_rx_pin_enum;

typedef enum                                                                    // 枚举 UART 中断配置 此枚举定义不允许用户修改
{
    UART_INTERRUPT_CONFIG_TX_DISABLE  ,                                         // 禁止 发送中断
    UART_INTERRUPT_CONFIG_RX_DISABLE  ,                                         // 禁止 接收中断
    UART_INTERRUPT_CONFIG_ALL_DISABLE ,                                         // 禁止 所有中断

    UART_INTERRUPT_CONFIG_TX_ENABLE   ,                                         // 使能 发送中断
    UART_INTERRUPT_CONFIG_RX_ENABLE   ,                                         // 使能 接收中断
    UART_INTERRUPT_CONFIG_ALL_ENABLE  ,                                         // 使能 所有中断
}zf_uart_interrupt_config_enum;

typedef enum                                                                    // 枚举 UART 中断状态 此枚举定义不允许用户修改
{
    UART_INTERRUPT_STATE_NONE   =   0x00    ,

    UART_INTERRUPT_STATE_RX     =   0x01    ,
    UART_INTERRUPT_STATE_TX     =   0x02    ,

    UART_INTERRUPT_STATE_ALL    =   0x03    ,
}zf_uart_interrupt_state_enum;

typedef enum                                                                    // 枚举 UART 数据位宽 此枚举定义不允许用户修改
{
    UART_DATA_WIDTH_7BIT                ,                                       // 7bit 数据位宽
    UART_DATA_WIDTH_8BIT                ,                                       // 8bit 数据位宽
}zf_uart_data_width_enum;

typedef enum                                                                    // 枚举 UART 停止位宽 此枚举定义不允许用户修改
{
    UART_STOP_WIDTH_0BIT_5              ,                                       // 0.5  bit 停止位宽
    UART_STOP_WIDTH_1BIT                ,                                       // 1    bit 停止位宽
    UART_STOP_WIDTH_1BIT_5              ,                                       // 1.5  bit 停止位宽
    UART_STOP_WIDTH_2BIT                ,                                       // 2    bit 停止位宽
}zf_uart_stop_width_enum;

typedef enum                                                                    // 枚举 UART 校验模式 此枚举定义不允许用户修改
{
    UART_PARITY_MODE_NONE               ,                                       // 无校验
    UART_PARITY_MODE_EVEN               ,                                       // 奇校验
    UART_PARITY_MODE_ODD                ,                                       // 偶校验
}zf_uart_parity_mode_enum;

typedef enum                                                                    // 枚举 UART 流控模式 此枚举定义不允许用户修改
{
    UART_HWCONTROL_MODE_NONE            ,                                       // 无流控
    UART_HWCONTROL_MODE_RTS             ,                                       // RTS 流控
    UART_HWCONTROL_MODE_CTS             ,                                       // CTS 流控
    UART_HWCONTROL_MODE_ALL             ,                                       // RTS + CTS 流控
}zf_uart_hwcontrol_mode_enum;

typedef enum                                                                    // 枚举 UART 流控模式 此枚举定义不允许用户修改
{
    UART_CONFIG_TYPE_DATA_WIDTH         ,                                       // 数据位宽配置
    UART_CONFIG_TYPE_STOP_WIDTH         ,                                       // 停止位宽配置
    UART_CONFIG_TYPE_PARITY_MODE        ,                                       // 校验模式配置
    UART_CONFIG_TYPE_HWCONTROL_MODE     ,                                       // 流控模式配置
}zf_uart_config_type_enum;

// 定义模块配置要用的宏定义
// bit[3 :0 ] 在 UART 模块中为 UART 配置数据索引
// bit[7 :4 ] 在 UART 模块中为 UART 配置类别索引

#define     UART_CONFIG_VALUE_OFFSET    ( 0     )                               // bit[3 :0 ] 存储 UART 配置数据索引
#define     UART_CONFIG_VALUE_MASK      ( 0x0F  )                               // 宽度 4bit 因此掩码为 0xF

#define     UART_CONFIG_TYPE_OFFSET     ( 4     )                               // bit[7 :4 ] 存储 UART 配置类别索引
#define     UART_CONFIG_TYPE_MASK       ( 0x0F  )                               // 宽度 4bit 因此掩码为 0xF

typedef enum                                                                    // 枚举 UART 配置选项 此枚举定义不允许用户修改
{
    UART_CONFIG_DATA_WIDTH_7BIT     = ((UART_CONFIG_TYPE_DATA_WIDTH     << UART_CONFIG_TYPE_OFFSET) | (UART_DATA_WIDTH_7BIT     )),
    UART_CONFIG_DATA_WIDTH_8BIT     = ((UART_CONFIG_TYPE_DATA_WIDTH     << UART_CONFIG_TYPE_OFFSET) | (UART_DATA_WIDTH_8BIT     )),

    UART_CONFIG_STOP_BIT_0BIT_5     = ((UART_CONFIG_TYPE_STOP_WIDTH     << UART_CONFIG_TYPE_OFFSET) | (UART_STOP_WIDTH_0BIT_5   )),
    UART_CONFIG_STOP_BIT_1BIT       = ((UART_CONFIG_TYPE_STOP_WIDTH     << UART_CONFIG_TYPE_OFFSET) | (UART_STOP_WIDTH_1BIT     )),
    UART_CONFIG_STOP_BIT_1BIT_5     = ((UART_CONFIG_TYPE_STOP_WIDTH     << UART_CONFIG_TYPE_OFFSET) | (UART_STOP_WIDTH_1BIT_5   )),
    UART_CONFIG_STOP_BIT_2BIT       = ((UART_CONFIG_TYPE_STOP_WIDTH     << UART_CONFIG_TYPE_OFFSET) | (UART_STOP_WIDTH_2BIT     )),

    UART_CONFIG_PARITY_BIT_NONE     = ((UART_CONFIG_TYPE_PARITY_MODE    << UART_CONFIG_TYPE_OFFSET) | (UART_PARITY_MODE_NONE    )),
    UART_CONFIG_PARITY_BIT_EVEN     = ((UART_CONFIG_TYPE_PARITY_MODE    << UART_CONFIG_TYPE_OFFSET) | (UART_PARITY_MODE_EVEN    )),
    UART_CONFIG_PARITY_BIT_ODD      = ((UART_CONFIG_TYPE_PARITY_MODE    << UART_CONFIG_TYPE_OFFSET) | (UART_PARITY_MODE_ODD     )),

    UART_CONFIG_HWCONTROL_NONE      = ((UART_CONFIG_TYPE_HWCONTROL_MODE << UART_CONFIG_TYPE_OFFSET) | (UART_HWCONTROL_MODE_NONE )),
    UART_CONFIG_HWCONTROL_RTS       = ((UART_CONFIG_TYPE_HWCONTROL_MODE << UART_CONFIG_TYPE_OFFSET) | (UART_HWCONTROL_MODE_RTS  )),
    UART_CONFIG_HWCONTROL_CTS       = ((UART_CONFIG_TYPE_HWCONTROL_MODE << UART_CONFIG_TYPE_OFFSET) | (UART_HWCONTROL_MODE_CTS  )),
    UART_CONFIG_HWCONTROL_ALL       = ((UART_CONFIG_TYPE_HWCONTROL_MODE << UART_CONFIG_TYPE_OFFSET) | (UART_HWCONTROL_MODE_ALL  )),
}zf_uart_config_item_enum;

#define     UART_DEFAULT_TIMEOUT_COUNT      ( 0x001FFFFF )                      // 超时计数不能太短 否则会直接导致通信异常

typedef enum                                                                    // 枚举 UART 操作状态   此枚举定义不允许用户修改
{
    UART_OPERATION_DONE             = ZF_NO_ERROR   ,                           // UART 操作完成
    UART_ERROR_UNKNOW               = ZF_ERROR      ,                           // UART 未知错误 操作无法进行 正常情况下不会出现此错误

    UART_WARNING_MODULE_DISABLE     = 0x10          ,                           // UART 模块失能禁用 操作中断退出
    UART_WARNING_OPERATION_TIMEOUT                  ,                           // UART 操作超时退出 操作中断退出
    UART_WARNING_NO_DATA                            ,                           // UART 查询数据为空 操作无法进行

    UART_ERROR_MODULE_OCCUPIED      = 0x20          ,                           // UART 模块已被占用 操作无法进行
    UART_ERROR_MODULE_NOT_INIT                      ,                           // UART 模块未初始化 操作无法进行

    UART_ERROR_TX_PIN_MISMATCH                      ,                           // UART 引脚不匹配 操作无法进行
    UART_ERROR_RX_PIN_MISMATCH                      ,                           // UART 引脚不匹配 操作无法进行

    UART_ERROR_BAUDRATE_UNSUPPORTED                 ,                           // UART 传输速率错误 操作无法进行
    UART_ERROR_INTERRUPT_CALLBACK_NOT_SET           ,                           // UART 中断回调未设 操作无法进行
    UART_ERROR_INTERRUPT_CALLBACK_ILLEGAL           ,                           // UART 中断回调异常 操作无法进行

    UART_ERROR_DATA_BUFFER_NULL                     ,                           // UART 数据指针异常 操作无法进行
}zf_uart_operation_state_enum;

typedef struct                                                                  // UART 管理对象模板 用于存储 UART 的信息
{
    // 配置信息包括 初始化 使能 数据位宽 停止位 校验 流控
    // 中断模式 自身索引 按照实际情况看如何确定
    // 总配置信息所占位宽不限制

    // 需要使用指针指向对应模块寄存器

    uint32              baudrate        ;                                       // 当前 UART 波特率
    UART_TypeDef        *uart_ptr       ;                                       // 当前 UART 模块的寄存器

    zf_gpio_pin_enum    tx_pin          ;                                       // 存储 UART TX 引脚
    zf_gpio_pin_enum    rx_pin          ;                                       // 存储 UART RX 引脚

    union
    {
        uint32          config_info     ;                                       // 配置整体数据
        struct
        {
            uint32      enable      : 4 ;                                       // 使能

            uint32      data_width  : 4 ;                                       // 传输位宽
            uint32      stop_width  : 4 ;                                       // 停止位
            uint32      parity      : 4 ;                                       // 校验
            uint32      hwcontrol   : 4 ;                                       // 流控

            uint32      tx_interrupt: 4 ;                                       // 中断
            uint32      rx_interrupt: 4 ;                                       // 中断

            uint32      self_index  : 4 ;                                       // 自身索引
        };
    };
}zf_uart_obj_struct;
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
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
uint8 zf_uart_assert_handler (char *file, int line, char *tips);
#define zf_uart_assert(status)      ((status) ? (0) : zf_uart_assert_handler(__FILE__, __LINE__, NULL))
#define zf_uart_log(status, tips)   ((status) ? (0) : zf_uart_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 字节输出
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     data                需要发送的字节
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_write_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_write_byte (zf_uart_index_enum uart_index, const uint8 data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 发送数组
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     *buff               要发送的数组地址
// 参数说明     len                 发送长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_write_buffer(uart_index, buff, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_write_buffer (zf_uart_index_enum uart_index, const uint8 *buff, uint32 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 发送字符串
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     *str                要发送的字符串地址
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_write_string(uart_index, str); 
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_write_string (zf_uart_index_enum uart_index, const char *str);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 读取接收的数据（阻塞式 whlie 等待 会超时退出）
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
//                                  UART_WARNING_OPERATION_TIMEOUT 标识超过读取时间限制
// 使用示例     zf_uart_read_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_read_byte (zf_uart_index_enum uart_index, uint8 *data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 读取接收的数据（查询接收）
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     art_query_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_query_byte (zf_uart_index_enum uart_index, uint8 *data);

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
uint8 zf_uart_read_buffer (zf_uart_index_enum uart_index, uint8 *buff, uint32 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 中断设置回调函数
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     callback            回调函数 为无返回值 uint32 加 void * 参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_set_interrupt_callback(uart_index, callback, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_set_interrupt_callback (zf_uart_index_enum uart_index, void_callback_uint32_ptr callback, void *ptr);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 设置中断配置
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     config              中断配置        (详见 zf_driver_uart.h 内 zf_uart_interrupt_config_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_set_interrupt_config(uart_index, config);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_set_interrupt_config (zf_uart_index_enum uart_index, zf_uart_interrupt_config_enum config);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 中断设置优先级
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     priority            中断优先级
// 返回参数     void
// 使用示例     zf_uart_set_interrupt_priority(uart_index, priority);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_uart_set_interrupt_priority (zf_uart_index_enum uart_index, uint8 priority);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 配置设置
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     item                UART 配置       (详见 zf_driver_uart.h 内 zf_uart_config_item_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_set_config(uart_index, item);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_set_config (zf_uart_index_enum uart_index, zf_uart_config_item_enum item);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 设置波特率
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 参数说明     baudrate            波特率
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_set_baudrate(uart_index, baudrate);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_set_baudrate (zf_uart_index_enum uart_index, uint32 baudrate);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 使能
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_enable(uart_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_enable (zf_uart_index_enum uart_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 禁止
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_disable(uart_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_disable (zf_uart_index_enum uart_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 注销初始化
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 zf_uart_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / UART_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_uart_deinit(uart_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_uart_deinit (zf_uart_index_enum uart_index);

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
uint8 zf_uart_init (zf_uart_index_enum uart_index, uint32 baudrate, zf_uart_tx_pin_enum tx_pin, zf_uart_rx_pin_enum rx_pin);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
