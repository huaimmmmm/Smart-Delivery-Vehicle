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

#ifndef _zf_device_wireless_interface_h_
#define _zf_device_wireless_interface_h_

// 如果移植到其他平台 这里就替换成自己的资源头文件
#include "zf_common_headfile.h"
#include "zf_driver_headfile.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// wireless_interface_assert                                                    //WIRELESS 断言处理
// wireless_interface_log                                                       //WIRELESS Log 输出
// wireless_interface_delay_ms                                                  //WIRELESS 延时

// wireless_interface_write_8bit_register                                       //WIRELESS 写寄存器 8bit
// wireless_interface_write_8bit_registers                                      //WIRELESS 写寄存器 8bit
// wireless_interface_read_8bit_register                                        //WIRELESS 读寄存器 8bit
// wireless_interface_read_8bit_registers                                       //WIRELESS 读数据 8bit
// wireless_interface_transfer_8bit_array                                       //WIRELESS 双向传输 8bit 数据

// wireless_interface_deinit                                                    //WIRELESS 接口注销初始化

// wireless_interface_spi_init                                                  //WIRELESS SPI 通信 接口初始化
// wireless_interface_soft_iic_init                                             //WIRELESS SOFT_IIC 通信 接口初始化

// wireless_interface_spi_default_init                                          //WIRELESS 使用默认引脚进行 SPI 通信 接口初始化
// wireless_interface_soft_iic_default_init                                     //WIRELESS 使用默认引脚进行 SOFT_IIC 通信 接口初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 WIRELESS 相关的接口资源 这里允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// 首先需要进行此处设置 如果主板有两类无线模块接口 那就都设置为 ZF_TRUE
// 如果对应硬件只支持一类接口的模块 例如只支持串口的模块 就将 UART 的宏定义设置为 ZF_TRUE 另一个设置为 ZF_FALSE
#define WIRELESS_INTERFACE_BOARD_UART_INTERFACE     ( ZF_TRUE  )                // 硬件是否支持 UART 接口的无线模块
#define WIRELESS_INTERFACE_BOARD_SPI_INTERFACE      ( ZF_FALSE )                // 硬件是否支持 SPI  接口的无线模块

// 在 逐飞科技 开源库中默认支持多种驱动模式
// 自行移植的话看情况是否需要此处的多方式驱动结构
//====================================================硬件 UART 驱动====================================================
#if WIRELESS_INTERFACE_BOARD_UART_INTERFACE
#ifdef _zf_driver_uart_h_

#define WIRELESS_INTERFACE_UART_INDEX               ( UART_2        )           // 硬件 UART 号
#define WIRELESS_INTERFACE_UART_SPEED               ( 115200        )           // 硬件 UART 速率
#define WIRELESS_INTERFACE_TX_PIN                   ( UART2_RX_A6   )           // 硬件 UART RX 引脚    连接 WIRELESS 的 TX 引脚
#define WIRELESS_INTERFACE_RX_PIN                   ( UART2_TX_A7   )           // 硬件 UART TX 引脚    连接 WIRELESS 的 RX 引脚

#define WIRELESS_INTERFACE_RTS_PIN                  ( B1            )           // RTS 流控引脚         连接 WIRELESS 的 RTS 引脚

#endif
#endif
//====================================================硬件 UART 驱动====================================================

//====================================================硬件 SPI 驱动====================================================
#if WIRELESS_INTERFACE_BOARD_SPI_INTERFACE
#ifdef _zf_driver_spi_h_

#define WIRELESS_INTERFACE_SPI_INDEX                ( SPI_3             )       // 硬件 SPI 号
#define WIRELESS_INTERFACE_SPI_SPEED                ( 10 * 1000 * 1000  )       // 硬件 SPI 速率
#define WIRELESS_INTERFACE_SPC_PIN                  ( SPI3_SCK_D10      )       // 硬件 SPI SCK  引脚   连接 WIRELESS 的 SPC 引脚
#define WIRELESS_INTERFACE_SDI_PIN                  ( SPI3_MOSI_D12     )       // 硬件 SPI MOSI 引脚   连接 WIRELESS 的 SDI 引脚
#define WIRELESS_INTERFACE_SDO_PIN                  ( SPI3_MISO_D11     )       // 硬件 SPI MISO 引脚   连接 WIRELESS 的 SDO 引脚

#define WIRELESS_INTERFACE_CS_PIN                   ( D7                )       // CS  片选引脚         连接 WIRELESS 的 CS 引脚

#define WIRELESS_INTERFACE_INT_PIN                  ( EXTI1_CH11_G11    )       // INT 中断引脚         连接 WIRELESS 的 INT 引脚
#define WIRELESS_INTERFACE_RST_PIN                  ( G12               )       // RST 复位引脚         连接 WIRELESS 的 RST 引脚

#endif
#endif
//====================================================硬件 SPI 驱动====================================================

// 独立内存管理部分 默认使用 逐飞科技 开源库中的内存分段定义
// 如果移植到其它平台后此处报错 可以将 WIRELESS_INTERFACE_USE_ZF_COMMON_MEMORY 修改为 0
// 当 WIRELESS_INTERFACE_USE_ZF_COMMON_MEMORY 为 0 会禁止内存分段指定 通过编译器随机分配
#define     WIRELESS_INTERFACE_USE_ZF_COMMON_MEMORY     ( 1 )
#if         WIRELESS_INTERFACE_USE_ZF_COMMON_MEMORY
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
// 如果移植到其它平台后此处报错 可以将 WIRELESS_INTERFACE_USE_ZF_COMMON_DEBUG 修改为 0
// 当 WIRELESS_INTERFACE_USE_ZF_COMMON_DEBUG 为 0 会通过 wireless_assert 使用 printf 输出报错
#define     WIRELESS_INTERFACE_USE_ZF_COMMON_DEBUG      ( 1 )
#if         WIRELESS_INTERFACE_USE_ZF_COMMON_DEBUG
#ifndef _zf_common_debug_h_
#define zf_assert(pass)         ((pass) ? (0) : (1))
#define zf_log(pass, str)       ((pass) ? (0) : (1))
#endif
#endif

// 延时定向部分 默认使用 逐飞科技 开源库中的延时接口
// 如果移植到其它平台后此处报错 可以将 WIRELESS_INTERFACE_USE_ZF_DIRVER_DELAY 修改为 0
// 当 WIRELESS_INTERFACE_USE_ZF_DIRVER_DELAY 为 0 会通过 wireless 的软延时实现
#define     WIRELESS_INTERFACE_USE_ZF_DIRVER_DELAY      ( 1 )
#if         WIRELESS_INTERFACE_USE_ZF_DIRVER_DELAY
#ifndef _zf_driver_delay_h_
#define zf_delay_ms(time)   for(vuint32 i = time * 1000000; i --; )
#endif
#endif

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 WIRELESS 相关的控制对象格式 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef enum
{
    WIRELESS_INTERFACE_TYPE_UNKNOW                  = 0             ,

    WIRELESS_INTERFACE_TYPE_UART                                    ,
    WIRELESS_INTERFACE_TYPE_SPI                                     ,
}wireless_interface_type_enum;

typedef enum
{
    WIRELESS_INTERFACE_OPERATION_DONE               = ZF_NO_ERROR   ,           // 操作完成
    WIRELESS_INTERFACE_ERROR_UNKNOW_ERROR           = ZF_ERROR      ,           // 异常错误 这个错误理论上不会被返回

    WIRELESS_INTERFACE_WARNING_MODULE_DISABLE       = 0x10          ,           // 模块禁止警告 模块未使能
    WIRELESS_INTERFACE_WARNING_OPERATION_TIMEOUT                    ,           // 模块超时警告 通信或者操作超时

    WIRELESS_INTERFACE_ERROR_INTERFACE_OCCUPIED     = 0x20          ,           // 通信接口占用错误 不能重复初始化同一个接口
    WIRELESS_INTERFACE_ERROR_INTERFACE_NOT_INIT                     ,           // 接口未初始化错误 不能未初始化就调用接口

    WIRELESS_INTERFACE_ERROR_DATA_BUFFER_NULL                       ,           // 数据缓冲区错误 不可以传入空指针
}wireless_interface_operation_state_enum;

typedef struct                                                                  // WIRELESS 模块化解耦结构
{
    wireless_interface_type_enum            interface_type          ;           // 必须要的接口类型
    union
    {
        zf_uart_index_enum                  uart_index              ;           // 硬件 UART 索引
        zf_spi_index_enum                   spi_index               ;           // 硬件 SPI  索引
        uint8                               communication_index     ;
    };

    union
    {
        zf_gpio_pin_enum                    rts_pin                 ;           // 模块引脚 RTS 流控引脚
        zf_gpio_pin_enum                    spi_cs_pin              ;           // 模块引脚 SPI CS 片选
    };

    zf_gpio_pin_enum                        rst_pin                 ;           // 模块引脚 INT 中断引脚
    zf_exti_channel_pin_enum                int_pin                 ;           // 模块引脚 RST 复位引脚

    zf_fifo_obj_struct                      fifo_obj                ;           // 无线模块默认使用 FIFO 进行数据接收缓冲
}wireless_interface_obj_struct;

//typedef uint8   (*wireless_func_get_acc      )   (wireless_interface_obj_struct *interface, void *result);
//typedef uint8   (*wireless_func_get_gyro     )   (wireless_interface_obj_struct *interface, void *result);
//typedef uint8   (*wireless_func_get_mag      )   (wireless_interface_obj_struct *interface, void *result);
//typedef uint8   (*wireless_func_set_config   )   (wireless_interface_obj_struct *interface, uint8 item);
//typedef uint8   (*wireless_func_ckeck_id     )   (wireless_interface_obj_struct *interface);
//typedef uint8   (*wireless_func_init         )   (wireless_interface_obj_struct *interface);

//typedef struct
//{
//    wireless_func_get_acc        get_acc     ;
//    wireless_func_get_gyro       get_gyro    ;
//    wireless_func_get_mag        get_mag     ;
//    wireless_func_set_config     set_config  ;
//    wireless_func_ckeck_id       ckeck_id    ;
//    wireless_func_init           init        ;
//}wireless_module_func_list;

#if WIRELESS_INTERFACE_BOARD_UART_INTERFACE
extern wireless_interface_obj_struct wireless_interface_uart_default_obj;
#endif
#if WIRELESS_INTERFACE_BOARD_SPI_INTERFACE
extern wireless_interface_obj_struct wireless_interface_spi_default_obj;
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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
#define wireless_interface_assert(pass)     zf_assert(pass)
#else
uint8 wireless_interface_assert_handler (char *file, int line, char *tips);
#define wireless_interface_assert(status)       ((status) ? (0) : wireless_interface_assert_handler(__FILE__, __LINE__, NULL))
#define wireless_interface_log(status, tips)    ((status) ? (0) : wireless_interface_assert_handler(__FILE__, __LINE__, (tips)))
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS 延时
// 参数说明     ms                  毫秒数
// 返回参数     void
// 使用示例     wireless_interface_delay_ms(ms);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#if WIRELESS_INTERFACE_USE_ZF_DIRVER_DELAY
#define wireless_interface_delay_ms(ms)     zf_delay_ms(ms)
#else
void wireless_interface_delay_ms (uint32 ms);
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
uint8 wireless_interface_write_8bit_array (wireless_interface_obj_struct *interface, const uint8 *send_data, uint32 send_len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS 接收 8bit 数据
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     *receive_data       接收数据缓冲区
// 参数说明     receive_len         接收数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_interface_read_8bit_array(interface, receive_data, receive_len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_interface_read_8bit_array (wireless_interface_obj_struct *interface, uint8 *receive_data, uint32 receive_len);

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
uint8 wireless_interface_transfer_8bit_array (wireless_interface_obj_struct *interface, const uint8 *send_data, uint32 send_len, uint8 *receive_data, uint32 receive_len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS 接口注销初始化
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_interface_deinit(interface);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_interface_deinit (wireless_interface_obj_struct *interface);

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
// 参数说明     callback            回调函数 为无返回值 uint32 ptr 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_interface_spi_init(interface, spi_index, mode, baudrate, sck_pin, mosi_pin, miso_pin, cs_pin, rst_pin, int_pin, data_buffer, buffer_size, call_back, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_interface_spi_init (wireless_interface_obj_struct *interface,
    zf_spi_index_enum spi_index, zf_spi_mode_enum mode, uint32 baudrate,
    zf_spi_sck_pin_enum sck_pin, zf_spi_mosi_pin_enum mosi_pin, zf_spi_miso_pin_enum miso_pin, zf_gpio_pin_enum cs_pin,
    zf_gpio_pin_enum rst_pin, zf_exti_channel_pin_enum int_pin, uint8 *data_buffer, uint32 buffer_size,
    void_callback_uint32_ptr call_back, void *ptr);

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
    zf_gpio_pin_enum rts_pin, uint8 *data_buffer, uint32 buffer_size, void_callback_uint32_ptr call_back, void *ptr);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS 使用默认资源进行 SPI 通信 接口初始化
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     mode                SPI 模式        (详见 zf_driver_spi.h 内 zf_spi_mode_enum 定义)
// 参数说明     *data_buffer        数据缓冲区
// 参数说明     buffer_size         数据缓冲区大小
// 参数说明     callback            回调函数 为无返回值 uint32 ptr 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_interface_spi_default_init(interface, mode, data_buffer, buffer_size, call_back, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define wireless_interface_spi_default_init(interface, mode, data_buffer, buffer_size, call_back, ptr)  \
    wireless_interface_spi_init(interface, WIRELESS_INTERFACE_SPI_INDEX, mode, WIRELESS_INTERFACE_SPI_SPEED, \
        WIRELESS_INTERFACE_SPC_PIN, WIRELESS_INTERFACE_SDI_PIN, WIRELESS_INTERFACE_SDO_PIN, WIRELESS_INTERFACE_CS_PIN, \
        WIRELESS_INTERFACE_RST_PIN, WIRELESS_INTERFACE_INT_PIN, data_buffer, buffer_size, call_back, ptr)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIRELESS 使用默认资源进行 UART 通信 接口初始化
// 参数说明     *interface          接口对象 WIRELESS 系列通用的接口对象
// 参数说明     *data_buffer        数据缓冲区
// 参数说明     buffer_size         数据缓冲区大小
// 参数说明     callback            回调函数 为无返回值 uint32 ptr 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / WIRELESS_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     wireless_interface_uart_default_init(interface, data_buffer, buffer_size, call_back, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define wireless_interface_uart_default_init(interface, data_buffer, buffer_size, call_back, ptr)   \
    wireless_interface_uart_init(interface, WIRELESS_INTERFACE_UART_INDEX, \
        WIRELESS_INTERFACE_UART_SPEED, WIRELESS_INTERFACE_RX_PIN, WIRELESS_INTERFACE_TX_PIN,\
        WIRELESS_INTERFACE_RTS_PIN, data_buffer, buffer_size, call_back, ptr)
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#endif
