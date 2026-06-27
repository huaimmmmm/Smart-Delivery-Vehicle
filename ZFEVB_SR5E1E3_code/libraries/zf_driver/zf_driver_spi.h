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
* 文件名称          zf_driver_spi
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

#ifndef _zf_driver_spi_h_
#define _zf_driver_spi_h_

// SDK 底层驱动
#include <spi.h>

// zf_common 层引用
#include "zf_common_typedef.h"

// zf_driver 层 类型定义 引用
#include "zf_driver_gpio.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_spi_transfer_8bit_array                                                   // SPI 接口传输 8bit 数组 先写后读取
// zf_spi_transfer_16bit_array                                                  // SPI 接口传输 16bit 数组 先写后读取

// zf_spi_set_config                                                            // SPI 接口配置设置
// zf_spi_set_speed                                                             // SPI 接口配置速度

// zf_spi_enable                                                                // SPI 接口 使能
// zf_spi_disable                                                               // SPI 接口 禁止

// zf_spi_deinit                                                                // SPI 接口注销初始化
// zf_spi_init                                                                  // SPI 接口初始化 默认 MASTER 模式 不提供 SLAVE 模式
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 UART 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define     SPI_NUM_MAX             (  4     )                                  // 总共最多 4 个 SPI

typedef enum                                                                    // 枚举 SPI 此枚举定义不允许用户修改
{
    SPI_1,
    SPI_2,
    SPI_3,
    SPI_4,
}zf_spi_index_enum;

typedef enum                                                                    // 枚举 SPI 模式 此枚举定义不允许用户修改
{
    SPI_MODE0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3,
}zf_spi_mode_enum;

#define     SPI_PIN_INDEX_OFFSET    ( 0      )                                  // 低位开始 先存储 GPIO 的索引号
#define     SPI_PIN_INDEX_MASK      ( 0x0FFF )                                  // 使用低 12bit 存储 GPIO 索引号 因此掩码为 0xFFF

#define     SPI_PIN_AF_OFFSET       ( 12     )                                  // 累计低位的位移后从 12bit 开始
#define     SPI_PIN_AF_MASK         ( 0x0F   )                                  // 同样根据需要添加的引脚服用功能掩码 这里也是 4bit 因此掩码为 0xF

#define     SPI_INDEX_OFFSET        ( 16     )                                  // 累计低位的位移后从 16bit 开始
#define     SPI_INDEX_MASK          ( 0x0F   )                                  // 根据需要定义 SPI 索引掩码 这里是 4bit 因此掩码为 0xF

typedef enum                                                                    // 枚举 SPI SCK 引脚 此枚举定义不允许用户修改
{
    // SPI1 引脚 SCK
    SPI1_SCK_D15        = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (D15)),

    // SPI2 引脚 SCK
    SPI2_SCK_G11        = ((SPI_2 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (G11)),

    // SPI3 引脚 SCK
    SPI3_SCK_D10        = ((SPI_3 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (D10)),

    // SPI4 引脚 SCK
    SPI4_SCK_G5         = ((SPI_4 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (G5 )),

}zf_spi_sck_pin_enum;

typedef enum                                                                    // 枚举 SPI MOSI 引脚 此枚举定义不允许用户修改
{
    // SPI1 引脚 MOSI
    SPI1_MOSI_E2        = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (E2 )),

    // SPI2 引脚 MOSI
    SPI2_MOSI_F3        = ((SPI_2 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (F3 )),

    // SPI3 引脚 MOSI
    SPI3_MOSI_D12       = ((SPI_3 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (D12)),

    // SPI4 引脚 MOSI
    SPI4_MOSI_G7        = ((SPI_4 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (G7 )),

}zf_spi_mosi_pin_enum;

typedef enum                                                                    // 枚举 SPI MISO 引脚 此枚举定义不允许用户修改
{
    SPI_MISO_NULL       = (PIN_NULL),                                           // 不使用硬件 MISO 引脚就填这个

    // SPI1 引脚 MISO
    SPI1_MISO_E1        = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (E1 )),

    // SPI2 引脚 MISO
    SPI2_MISO_F2        = ((SPI_2 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (F2 )),

    // SPI3 引脚 MISO
    SPI3_MISO_D11       = ((SPI_3 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (D11)),

    // SPI4 引脚 MISO
    SPI4_MISO_G6        = ((SPI_4 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (G6 )),

}zf_spi_miso_pin_enum;

typedef enum                                                                    // 枚举 SPI CS 引脚 此枚举定义不允许用户修改
{
    SPI_CS_NULL         = (PIN_NULL),                                           // 不使用硬件 CS 引脚就填这个

    // SPI1 引脚 CS
    SPI1_CS_D14         = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (D14)),
    SPI1_CS_E8          = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (E8 )),

    // SPI2 引脚 CS
    SPI2_CS_E10         = ((SPI_2 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (E10)),
    SPI2_CS_G12         = ((SPI_2 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (G12)),

    // SPI3 引脚 CS
    SPI3_CS_D13         = ((SPI_3 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (D13)),

    // SPI4 引脚 CS
    SPI4_CS_G8          = ((SPI_4 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (G8 )),

}zf_spi_cs_pin_enum;

#define     SPI_CONFIG_TYPE_OFFSET  ( 4     )                                   // 累计低位的位移后从 16bit 开始
#define     SPI_CONFIG_TYPE_MASK    ( 0x0F   )                                  // 同样根据需要添加的引脚服用功能掩码 这里也是 4bit 因此掩码为 0xF

#define     SPI_CONFIG_VALUE_OFFSET ( 0     )                                   // 累计低位的位移后从 16bit 开始
#define     SPI_CONFIG_VALUE_MASK   ( 0x0F   )                                  // 同样根据需要添加的引脚服用功能掩码 这里也是 4bit 因此掩码为 0xF

typedef enum                                                                    // 枚举串口配置选项 此枚举定义不允许用户修改
{
    SPI_CONFIG_DATA_LENGTH_8BIT     = 0x00,                                     // 8bit 传输模式兼容 8/16bit 数据传输
    SPI_CONFIG_DATA_LENGTH_16BIT    = 0x01,                                     // 16bit 传输模式仅保证 16bit 的接口工作正常

    SPI_CONFIG_DATA_FORMAT_MSB      = 0x10,
    SPI_CONFIG_DATA_FORMAT_LSB      = 0x11,
}zf_spi_config_item_enum;

typedef enum                                                                    // 枚举 SPI 操作状态    此枚举定义不允许用户修改
{
    SPI_OPERATION_DONE                  = ZF_NO_ERROR   ,                       // SPI 操作完成
    SPI_ERROR_UNKNOW                    = ZF_ERROR      ,                       // SPI 未知错误 操作无法进行 正常情况下不会出现此错误

    SPI_WARNING_MODULE_DISABLE          = 0x10          ,                       // SPI 模块失能禁用 操作中断退出

    SPI_ERROR_MODULE_OCCUPIED           = 0x20          ,                       // SPI 模块已被占用 操作无法进行
    SPI_ERROR_MODULE_NOT_INIT                           ,                       // SPI 模块未初始化 操作无法进行

    SPI_ERROR_SPEED_UNSUPPORTED                         ,                       // SPI 传输速率错误 操作无法进行
    SPI_ERROR_SCK_PIN_ILLEGAL                           ,                       // SPI 引脚参数错误 操作无法进行
    SPI_ERROR_MOSI_PIN_ILLEGAL                          ,                       // SPI 引脚参数错误 操作无法进行
    SPI_ERROR_MISO_PIN_ILLEGAL                          ,                       // SPI 引脚参数错误 操作无法进行
    SPI_ERROR_CS_PIN_ILLEGAL                            ,                       // SPI 引脚参数错误 操作无法进行

    SPI_ERROR_DATA_BUFFER_NULL                          ,                       // SPI 数据指针异常 操作无法进行
}zf_spi_operation_state_enum;

typedef struct                                                                  // SPI 管理对象模板 用于存储 SPI 的信息
{
    SPI_TypeDef         *spi_ptr        ;
    uint32              clock_input     ;

    zf_gpio_pin_enum    sck_pin         ;                                       // 用于记录对应的引脚编号
    zf_gpio_pin_enum    mosi_pin        ;                                       // 用于记录对应的引脚编号
    zf_gpio_pin_enum    miso_pin        ;                                       // 用于记录对应的引脚编号
    zf_gpio_pin_enum    cs_pin          ;                                       // 用于记录对应的引脚编号

    uint32              baudrate        ;

    union
    {
        uint32          config_info     ;                                       // 配置整体数据
        struct
        {
            uint32      mode        : 4 ;                                       // SPI 模式
            uint32      enable      : 4 ;                                       // 使能

            uint32      width       : 4 ;                                       // 传输位宽
            uint32      format      : 4 ;                                       // 帧模式

            uint32      use_miso    : 4 ;                                       // 是否使用 MISO 引脚
            uint32      use_cs      : 4 ;                                       // 是否使用 CS 引脚

            uint32                  : 8 ;
        };
    };
}zf_spi_obj_struct;

extern zf_spi_obj_struct zf_spi_obj_list[SPI_NUM_MAX];
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_assert_handler (char *file, int line, char *tips);
#define zf_spi_assert(status)       ((status) ? (0) : zf_gpio_assert_handler(__FILE__, __LINE__, NULL))
#define zf_spi_log(status, tips)    ((status) ? (0) : zf_gpio_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 8bit 数据传输 发送与接收数据是同时进行的
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     *write_buffer       发送数据存放缓冲区
// 参数说明     *read_buffer        读取数据存放缓冲区
// 参数说明     len                 发送的字节数
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_transfer_8bit_array(spi_index, write_buffer, read_buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_transfer_8bit_array (zf_spi_index_enum spi_index, const uint8 *write_buffer, uint8 *read_buffer, uint32 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 16bit 数据传输 发送与接收数据是同时进行的
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     *write_buffer       发送数据存放缓冲区
// 参数说明     *read_buffer        读取数据存放缓冲区
// 参数说明     len                 发送的字节数
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_transfer_16bit_array(spi_index, write_buffer, read_buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_transfer_16bit_array (zf_spi_index_enum spi_index, const uint16 *write_buffer, uint16 *read_buffer, uint32 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口 配置设置
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     item                SPI 参照 zf_driver_spi.h 内 zf_spi_config_item_enum 枚举体定义
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_set_config(spi_index, item);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_set_config (zf_spi_index_enum spi_index, zf_spi_config_item_enum item);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口 配置速度
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     baudrate            设置 SPI 的波特率 不超过系统时钟的一半 会被适配成相近的速率
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_set_speed(spi_index, baudrate);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_set_speed (zf_spi_index_enum spi_index, uint32 baudrate);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口  使能
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_enable(spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_enable (zf_spi_index_enum spi_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口  禁止
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_disable(spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_disable (zf_spi_index_enum spi_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口 注销初始化
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_deinit(spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_deinit (zf_spi_index_enum spi_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口初始化
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     mode                SPI 模式        (详见 zf_driver_spi.h 内 zf_spi_mode_enum 定义)
// 参数说明     baudrate            设置 SPI 的波特率 不超过系统时钟的一半 会被适配成相近的速率
// 参数说明     sck_pin             选择 SCK 引脚   (详见 zf_driver_spi.h 内 zf_spi_sck_pin_enum 定义)
// 参数说明     mosi_pin            选择 MOSI 引脚  (详见 zf_driver_spi.h 内 zf_spi_mosi_pin_enum 定义)
// 参数说明     miso_pin            选择 MISO 引脚  (详见 zf_driver_spi.h 内 zf_spi_miso_pin_enum 定义)
// 参数说明     cs_pin              选择 CS 引脚    (详见 zf_driver_spi.h 内 zf_spi_cs_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_init(spi_index, mode, baudrate, sck_pin, mosi_pin, miso_pin, cs_pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_init (zf_spi_index_enum spi_index, zf_spi_mode_enum mode, uint32 baudrate, zf_spi_sck_pin_enum sck_pin, zf_spi_mosi_pin_enum mosi_pin, zf_spi_miso_pin_enum miso_pin, zf_spi_cs_pin_enum cs_pin);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
