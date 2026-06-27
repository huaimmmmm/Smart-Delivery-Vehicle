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
* 文件名称          zf_driver_soft_spi
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

#ifndef _zf_driver_soft_spi_h_
#define _zf_driver_soft_spi_h_

// zf_common 层 类型定义 引用
#include "zf_common_typedef.h"

// zf_driver 层 类型定义 引用
#include "zf_driver_gpio.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// soft_spi_transfer_8bit_array                                                 // SOFT_SPI 接口传输 8bit 数组 先写后读取
// soft_spi_transfer_16bit_array                                                // SOFT_SPI 接口传输 16bit 数组 先写后读取

// soft_spi_set_config                                                          // SOFT_SPI 接口配置设置
// soft_spi_set_speed                                                           // SOFT_SPI 接口配置速度

// soft_spi_enable                                                              // SOFT_SPI 接口 使能
// soft_spi_disable                                                             // SOFT_SPI 接口 禁止

// soft_spi_deinit                                                              // SOFT_SPI 接口注销初始化
// soft_spi_init                                                                // SOFT_SPI 接口初始化 默认 MASTER 模式 不提供 SLAVE 模式
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 SOFT-SPI 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define         SOFT_SPI_MAX                ( 4 )                               // 支持的最大的 SOFT_SPI 数量

typedef enum                                                                    // 枚举 SOFT_SPI 编号 此枚举定义不允许用户修改
{
    SOFT_SPI_1 = (uint8)0,
    SOFT_SPI_2,
    SOFT_SPI_3,
    SOFT_SPI_4,
}zf_soft_spi_index_enum;

typedef enum                                                                    // 枚举 SOFT_SPI 模式 此枚举定义不允许用户修改
{
    SOFT_SPI_MODE0,
    SOFT_SPI_MODE1,
    SOFT_SPI_MODE2,
    SOFT_SPI_MODE3,
}zf_soft_spi_mode_enum;

typedef enum                                                                    // 枚举 SOFT_SPI 帧格式 此枚举定义不允许用户修改
{
    SOFT_SPI_CONFIG_MSB,
    SOFT_SPI_CONFIG_LSB,
}zf_soft_spi_config_item_enum;

typedef enum                                                                    // 枚举 SOFT_SPI 操作状态   此枚举定义不允许用户修改
{
    SOFT_SPI_OPERATION_DONE                 = ZF_NO_ERROR   ,                   // SOFT_SPI 操作完成
    SOFT_SPI_ERROR_UNKNOW                   = ZF_ERROR      ,                   // SOFT_SPI 未知错误 操作无法进行 正常情况下不会出现此错误

    SOFT_SPI_WARNING_MODULE_DISABLE         = 0x10          ,                   // SOFT_SPI 模块失能禁用 操作中断退出

    SOFT_SPI_ERROR_MODULE_OCCUPIED          = 0x20          ,                   // SOFT_SPI 模块已被占用 操作无法进行
    SOFT_SPI_ERROR_MODULE_NOT_INIT                          ,                   // SOFT_SPI 模块未初始化 操作无法进行

    SOFT_SPI_ERROR_SPEED_UNSUPPORTED                        ,                   // SOFT_SPI 传输速率错误 操作无法进行
    SOFT_SPI_ERROR_SCK_PIN_ILLEGAL                          ,                   // SOFT_SPI 引脚参数错误 操作无法进行
    SOFT_SPI_ERROR_MOSI_PIN_ILLEGAL                         ,                   // SOFT_SPI 引脚参数错误 操作无法进行

    SOFT_SPI_ERROR_DATA_BUFFER_NULL                         ,                   // SOFT_SPI 数据指针异常 操作无法进行
}zf_soft_spi_operation_state_enum;

typedef struct
{
    zf_gpio_pin_enum    sck_pin;                                                // 用于记录对应的引脚编号
    zf_gpio_pin_enum    mosi_pin;                                               // 用于记录对应的引脚编号
    zf_gpio_pin_enum    miso_pin;                                               // 用于记录对应的引脚编号
    zf_gpio_pin_enum    cs_pin;                                                 // 用于记录对应的引脚编号

    uint32              delay;                                                  // 模拟 SPI 软延时时长

    union
    {
        uint32          config_info;                                            // 配置整体数据
        struct
        {
            uint32      mode        : 4 ;                                       // SPI 模式
            uint32      enable      : 4 ;                                       // 使能
            uint32      format      : 4 ;                                       // 帧模式
            uint32      use_miso    : 4 ;                                       // 是否使用 MISO 引脚
            uint32      use_cs      : 4 ;                                       // 是否使用 CS 引脚
            uint32                  : 12;
        };
    };
}zf_soft_spi_obj_struct;

extern zf_soft_spi_obj_struct soft_spi_obj_list[SOFT_SPI_MAX];
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_assert_handler(file, line, tips);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_assert_handler (char *file, int line, char *tips);
#define zf_soft_spi_assert(status)      ((status) ? (0) : zf_soft_spi_assert_handler(__FILE__, __LINE__, NULL))
#define zf_soft_spi_log(status, tips)   ((status) ? (0) : zf_soft_spi_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 8bit 数据传输 发送与接收数据是同时进行的
// 参数说明     soft_spi_index  SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     *write_buffer   发送数据存放缓冲区
// 参数说明     *read_buffer    读取数据存放缓冲区
// 参数说明     len             发送的字节数
// 返回参数     uint8           操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_transfer_8bit_array(soft_spi_index, write_buffer, read_buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_transfer_8bit_array (zf_soft_spi_index_enum soft_spi_index, const uint8 *write_buffer, uint8 *read_buffer, uint32 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 16bit 数据传输 发送与接收数据是同时进行的
// 参数说明     soft_spi_index  SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     *write_buffer   发送数据存放缓冲区
// 参数说明     *read_buffer    读取数据存放缓冲区
// 参数说明     len             发送的字节数
// 返回参数     uint8           操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_transfer_16bit_array(zf_soft_spi_index, write_buffer, read_buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_transfer_16bit_array (zf_soft_spi_index_enum soft_spi_index, const uint16 *write_buffer, uint16 *read_buffer, uint32 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口配置设置
// 参数说明     soft_spi_index  SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     item            SOFT_SPI 参照   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_config_item_enum 定义)
// 返回参数     uint8           操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_set_config(soft_spi_index, item);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_set_config (zf_soft_spi_index_enum soft_spi_index, zf_soft_spi_config_item_enum item);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口配置速度
// 参数说明     soft_spi_index  SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     delay           SOFT_SPI 延时 就是时钟高电平时间 越短 SPI 速率越高 最小值为 1
// 返回参数     uint8           操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_set_config(soft_spi_index, item);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_set_speed (zf_soft_spi_index_enum soft_spi_index, uint32 delay);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口 使能
// 参数说明     soft_spi_index  SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 返回参数     uint8           操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_enable(soft_spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_enable (zf_soft_spi_index_enum soft_spi_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口 禁止
// 参数说明     soft_spi_index  SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 返回参数     uint8           操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_disable(soft_spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_disable (zf_soft_spi_index_enum soft_spi_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口注销初始化
// 参数说明     soft_spi_index  SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 返回参数     uint8           操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_deinit(soft_spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_deinit (zf_soft_spi_index_enum soft_spi_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口初始化
// 参数说明     soft_spi_index  SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     mode            SPI 模式        (详见 zf_driver_soft_spi.h 内 zf_soft_spi_mode_enum 定义)
// 参数说明     delay           SOFT_SPI 延时 就是时钟高电平时间 越短 SPI 速率越高 最小值为 1
// 参数说明     sck_pin         选择 SCK 引脚   (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     mosi_pin        选择 MOSI 引脚  (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     miso_pin        选择 MISO 引脚  如果不需要这个引脚 就填 PIN_NULL 或 MOSI 一样的引脚 (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     cs_pin          选择 CS 引脚    如果不需要这个引脚 就填 PIN_NULL 或 MISO 一样的引脚 (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 返回参数     uint8           操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_init(soft_spi_index, mode, delay, sck_pin, mosi_pin, miso_pin, cs_pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_init (zf_soft_spi_index_enum soft_spi_index, zf_soft_spi_mode_enum mode, uint32 delay, zf_gpio_pin_enum sck_pin, zf_gpio_pin_enum mosi_pin, zf_gpio_pin_enum miso_pin, zf_gpio_pin_enum cs_pin);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
