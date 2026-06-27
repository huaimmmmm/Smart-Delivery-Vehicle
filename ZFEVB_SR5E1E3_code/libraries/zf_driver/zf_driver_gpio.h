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
* 文件名称          zf_driver_gpio
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

#ifndef _zf_driver_gpio_h_
#define _zf_driver_gpio_h_

// SDK 底层驱动
#include <gpio.h>

// zf_common 层 类型定义 引用
#include "zf_common_typedef.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_gpio_idr_8bit_addr                                                        // GPIO 获取输入数据地址  8bit 对齐
// zf_gpio_idr_32bit_addr                                                       // GPIO 获取输入数据地址 32bit 对齐
// zf_gpio_odr_8bit_addr                                                        // GPIO 获取输出数据地址  8bit 对齐
// zf_gpio_odr_32bit_addr                                                       // GPIO 获取输出数据地址 32bit 对齐

// zf_gpio_low                                                                  // GPIO 复位为低电平
// zf_gpio_high                                                                 // GPIO 置位为高电平

// zf_gpio_set_level                                                            // GPIO 输出设置
// zf_gpio_get_level                                                            // GPIO 电平获取
// zf_gpio_toggle_level                                                         // GPIO 翻转电平

// zf_gpio_deinit                                                               // GPIO 注销初始化 设置为开漏输出 并输出高电平 维持高阻态
// zf_gpio_set_dir                                                              // GPIO 方向设置
// zf_gpio_init                                                                 // GPIO 初始化
// zf_afio_init                                                                 // GPIO 复用功能初始化 一般是内部调用
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 UART 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 根据实际的芯片来进行定义
#define     GPIO_GROUP_MAX                  (  9 )                              // 引脚组数量
#define     GPIO_GROUP_PIN_NUMBER_MAX       ( 32 )                              // 每个组引脚上限

// 宏定义函数需要通过枚举来检索 IO 因此需要获取 IO 组别和下标信息 这里就定义偏移量和掩码
#define     GPIO_PIN_INDEX_OFFSET           ( 0    )                            // 将下标信息保存在低位 方便直接使用
#define     GPIO_PIN_INDEX_MASK             ( 0x1F )                            // 由于范围是 [0, 31] 占 5bit 掩码就是 0x1F

#define     GPIO_GROUP_INDEX_OFFSET         ( 5    )                            // 低 5bit 存储了下标 那么位移 5
#define     GPIO_GROUP_INDEX_MASK           ( 0x07 )                            // 这里根据情况来设置掩码

typedef enum                                                                    // 枚举 GPIO 引脚  此枚举定义不允许用户修改
{
    A0 = GPIO_GROUP_PIN_NUMBER_MAX * 0,
            A1 ,    A2 ,    A3 ,
    A4 ,    A5 ,    A6 ,    A7 ,
    A8 ,    A9 ,    A10,    A11,
    A12,    A13,    A14,    A15,

    B0 = GPIO_GROUP_PIN_NUMBER_MAX * 1,
            B1 ,    B2 ,    B3 ,
    B4 ,    B5 ,    B6 ,    B7 ,
    B8 ,    B9 ,    B10,    B11,
    B12,    B13,    B14,    B15,

    C0 = GPIO_GROUP_PIN_NUMBER_MAX * 2,
            C1 ,    C2 ,    C3 ,
    C4 ,    C5 ,    C6 ,    C7 ,
    C8 ,    C9 ,    C10,    C11,
    C12,    C13,    C14,    C15,

    D0 = GPIO_GROUP_PIN_NUMBER_MAX * 3,
            D1 ,    D2 ,    D3 ,
    D4 ,    D5 ,    D6 ,    D7 ,
    D8 ,    D9 ,    D10,    D11,
    D12,    D13,    D14,    D15,

    E0 = GPIO_GROUP_PIN_NUMBER_MAX * 4,
            E1 ,    E2 ,    E3 ,
    E4 ,    E5 ,    E6 ,    E7 ,
    E8 ,    E9 ,    E10,    E11,
    E12,    E13,    E14,    E15,

    F0 = GPIO_GROUP_PIN_NUMBER_MAX * 5,
            F1 ,    F2 ,    F3 ,
    F4 ,    F5 ,    F6 ,    F7 ,
    F8 ,    F9 ,    F10,    F11,
    F12,    F13,    F14,    F15,

    G0 = GPIO_GROUP_PIN_NUMBER_MAX * 6,
            G1 ,    G2 ,    G3 ,
    G4 ,    G5 ,    G6 ,    G7 ,
    G8 ,    G9 ,    G10,    G11,
    G12,    G13,    G14,    G15,

    H0 = GPIO_GROUP_PIN_NUMBER_MAX * 7,
            H1 ,    H2 ,    H3 ,
    H4 ,    H5 ,    H6 ,    H7 ,
    H8 ,    H9 ,    H10,    H11,
    H12,    H13,    H14,    H15,


    I0 = GPIO_GROUP_PIN_NUMBER_MAX * 8,
            I1 ,    I2 ,    I3 ,
    I4 ,    I5 ,    I6 ,    I7 ,
    I8 ,    I9 ,    I10,    I11,
    I12,    I13,    I14,    I15,

    // 空引脚 占位使用 该枚举值无效
    // 在不正确的位置使用本枚举值会导致不可预测的异常
    PIN_NULL    = 0x0FFF,
    // 用户只能在特定条件能使用它
    // 特定条件 1   在 soft_spi 不使用 MISO/CS 引脚时可以使用此枚举值占位
}zf_gpio_pin_enum;

#define     GPIO_PIN_CONFIG_OFFSET          ( 0    )                            // 将下标信息保存在低位 方便直接使用
#define     GPIO_PIN_CONFIG_MASK            ( 0x0F )                            // 这里根据情况来设置掩码

#define     GPIO_PIN_DIR_OFFSET             ( 4    )                            // 将下标信息保存在低位 方便直接使用
#define     GPIO_PIN_DIR_MASK               ( 0x01 )                            // 这里根据情况来设置掩码

typedef enum                                                                    // 枚举端口模式   此枚举定义不允许用户修改
{
    GPI_ANAOG_IN        = ( 0x00 ),                                             // [输入] [定义管脚模拟输入]
    GPI_FLOATING_IN     = ( 0x01 ),                                             // [输入] [定义管脚浮空输入]
    GPI_PULL_DOWN       = ( 0x02 ),                                             // [输入] [定义管脚下拉输入]
    GPI_PULL_UP         = ( 0x03 ),                                             // [输入] [定义管脚上拉输入]

    GPO_PUSH_PULL       = ( 0x10 ),                                             // [输出] [定义管脚推挽输出]
    GPO_OPEN_DRAIN      = ( 0x11 ),                                             // [输出] [定义管脚开漏输出]
    GPO_AF_PUSH_PULL    = ( 0x12 ),                                             // [输出] [定义管脚复用推挽输出]
    GPO_AF_OPEN_DRAIN   = ( 0x13 ),                                             // [输出] [定义管脚复用开漏输出]
}zf_gpio_mode_enum;

typedef enum                                                                    // 枚举端口复用   此枚举定义不允许用户修改
{
    GPIO_AF0            = ( 0x00 ),                                             // 引脚复用功能选项  0
    GPIO_AF1            = ( 0x01 ),                                             // 引脚复用功能选项  1
    GPIO_AF2            = ( 0x02 ),                                             // 引脚复用功能选项  2
    GPIO_AF3            = ( 0x03 ),                                             // 引脚复用功能选项  3
    GPIO_AF4            = ( 0x04 ),                                             // 引脚复用功能选项  4
    GPIO_AF5            = ( 0x05 ),                                             // 引脚复用功能选项  5
    GPIO_AF6            = ( 0x06 ),                                             // 引脚复用功能选项  6
    GPIO_AF7            = ( 0x07 ),                                             // 引脚复用功能选项  7
    GPIO_AF8            = ( 0x08 ),                                             // 引脚复用功能选项  8
    GPIO_AF9            = ( 0x09 ),                                             // 引脚复用功能选项  9
    GPIO_AF10           = ( 0x0A ),                                             // 引脚复用功能选项 10
    GPIO_AF11           = ( 0x0B ),                                             // 引脚复用功能选项 11
    GPIO_AF12           = ( 0x0C ),                                             // 引脚复用功能选项 12
    GPIO_AF13           = ( 0x0D ),                                             // 引脚复用功能选项 13
    GPIO_AF14           = ( 0x0E ),                                             // 引脚复用功能选项 14
    GPIO_AF15           = ( 0x0F ),                                             // 引脚复用功能选项 15
}zf_gpio_af_enum;

typedef enum                                                                    // 枚举端口电平   此枚举定义不允许用户修改
{
    GPIO_LOW            = 0x00,                                                 // 定义管脚初始化电平为低
    GPIO_HIGH           = 0x01,                                                 // 定义管脚初始化电平为高
}zf_gpio_level_enum;

// 本数组之所以不进行 static 修饰 是因为其他文件中使用宏定义函数需要用到它
extern GPIO_TypeDef * const gpio_group_list[GPIO_GROUP_MAX];
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_gpio_assert_handler(file, line, tips);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_assert_handler (char *file, int line, char *tips);
#define zf_gpio_assert(status)      ((status) ? (0) : zf_gpio_assert_handler(__FILE__, __LINE__, NULL))
#define zf_gpio_log(status, tips)   ((status) ? (0) : zf_gpio_assert_handler(__FILE__, __LINE__, (tips)))

// 本部分定义引脚的数据读写地址 以及组间偏移值 用于后续直接对地址读写或 DMA 搬运
#define     GPIO_IDR_ADDR_BASE              ( 0x48000010 )                      // GPIO 的输入数据寄存器地址
#define     GPIO_ODR_ADDR_BASE              ( 0x48000014 )                      // GPIO 的输出数据寄存器地址
#define     GPIO_GROUP_ADDR_OFFSET          ( 0x00000400 )                      // GPIO 组间地址偏移量

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 获取输入数据地址  8bit 对齐
// 参数说明     pin                 选择的引脚 (可选择范围由 zf_driver_gpio.h 内 zf_gpio_pin_enum 枚举值确定)
//                                  不可以选择 PIN_NULL 使用本宏定义函数 否则会导致不可预料的异常
// 返回参数     uint32              32bit 地址
// 使用示例     zf_gpio_idr_8bit_addr(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define zf_gpio_idr_8bit_addr(pin)  ((PIN_NULL != pin) ? ((uint8  *)(GPIO_IDR_ADDR_BASE + ((pin) >> GPIO_GROUP_INDEX_OFFSET) * GPIO_GROUP_ADDR_OFFSET + (((pin) & GPIO_PIN_INDEX_MASK) / 8))) : (zf_gpio_assert(0)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 获取输入数据地址 32bit 对齐
// 参数说明     pin                 选择的引脚 (可选择范围由 zf_driver_gpio.h 内 zf_gpio_pin_enum 枚举值确定)
//                                  不可以选择 PIN_NULL 使用本宏定义函数 否则会导致不可预料的异常
// 返回参数     uint32              32bit 地址
// 使用示例     zf_gpio_idr_32bit_addr(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define zf_gpio_idr_32bit_addr(pin) ((PIN_NULL != pin) ? ((uint32 *)(GPIO_IDR_ADDR_BASE + ((pin) >> GPIO_GROUP_INDEX_OFFSET) * GPIO_GROUP_ADDR_OFFSET)) : (zf_gpio_assert(0)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 获取输出数据地址  8bit 对齐
// 参数说明     pin                 选择的引脚 (可选择范围由 zf_driver_gpio.h 内 zf_gpio_pin_enum 枚举值确定)
//                                  不可以选择 PIN_NULL 使用本宏定义函数 否则会导致不可预料的异常
// 返回参数     uint32              32bit 地址
// 使用示例     zf_gpio_odr_8bit_addr(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define zf_gpio_odr_8bit_addr(pin)  ((PIN_NULL != pin) ? ((uint8 *)(GPIO_ODR_ADDR_BASE + ((pin) >> GPIO_GROUP_INDEX_OFFSET) * GPIO_GROUP_ADDR_OFFSET + (((pin) & GPIO_PIN_INDEX_MASK) / 8))) : (zf_gpio_assert(0)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 获取输出数据地址 32bit 对齐
// 参数说明     pin                 选择的引脚 (可选择范围由 zf_driver_gpio.h 内 zf_gpio_pin_enum 枚举值确定)
//                                  不可以选择 PIN_NULL 使用本宏定义函数 否则会导致不可预料的异常
// 返回参数     uint32              32bit 地址
// 使用示例     zf_gpio_odr_32bit_addr(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define zf_gpio_odr_32bit_addr(pin) ((PIN_NULL != pin) ? ((uint32 *)(GPIO_ODR_ADDR_BASE + ((pin) >> GPIO_GROUP_INDEX_OFFSET) * GPIO_GROUP_ADDR_OFFSET)) : (zf_gpio_assert(0)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     对应 IO 复位为低电平
// 参数说明     pin         选择的引脚 (可选择范围由 zf_driver_gpio.h 内 zf_gpio_pin_enum 枚举值确定)
// 返回参数     void
// 使用示例     zf_gpio_low(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define zf_gpio_low(pin)            ((PIN_NULL != pin) ? (gpio_group_list[((pin) >> GPIO_GROUP_INDEX_OFFSET)]->BSRR |= ((uint32)0x00010000 << ((pin) & GPIO_PIN_INDEX_MASK))) : (zf_gpio_assert(0)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     对应 IO 置位为高电平
// 参数说明     pin         选择的引脚 (可选择范围由 zf_driver_gpio.h 内 zf_gpio_pin_enum 枚举值确定)
// 返回参数     void
// 使用示例     zf_gpio_high(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define zf_gpio_high(pin)           ((PIN_NULL != pin) ? (gpio_group_list[((pin) >> GPIO_GROUP_INDEX_OFFSET)]->BSRR |= ((uint32)0x00000001 << ((pin) & GPIO_PIN_INDEX_MASK))) : (zf_gpio_assert(0)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 输出设置
// 参数说明     pin                 选择的引脚      (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     dat                 GPIO_LOW - 低电平 / GPIO_HIGH - 高电平
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_gpio_set_level(pin, dat);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_set_level (zf_gpio_pin_enum pin, const uint8 dat);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 电平获取
// 参数说明     pin                 选择的引脚      (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 返回参数     uint8               引脚当前电平 GPIO_LOW - 低电平 / GPIO_HIGH - 高电平
// 使用示例     uint8 status = zf_gpio_get_level(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_get_level (zf_gpio_pin_enum pin);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 翻转电平
// 参数说明     pin                 选择的引脚      (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_gpio_toggle_level(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_toggle_level (zf_gpio_pin_enum pin);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 方向设置
// 参数说明     pin                 选择的引脚      (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     mode                引脚的模式      (详见 zf_driver_gpio.h 内 zf_gpio_mode_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_gpio_set_dir(pin, mode);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_set_dir (zf_gpio_pin_enum pin, zf_gpio_mode_enum mode);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 注销初始化 设置为开漏输出 并输出高电平 维持高阻态
// 参数说明     pin                 选择的引脚      (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_gpio_deinit(pin);
// 备注信息     当 GPIOx 没有任何引脚初始化时将会关闭 GPIOx 的时钟降低功耗
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_deinit (zf_gpio_pin_enum pin);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 初始化
// 参数说明     pin                 选择的引脚      (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     mode                引脚的模式      (详见 zf_driver_gpio.h 内 zf_gpio_mode_enum 定义)
// 参数说明     dat                 引脚初始化时设置的电平状态，输出时有效 0：低电平 1：高电平 仅在设置为输出模式时有效
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 操作完成
//                                  GPIO_WARNING_PIN_OVERWRITES_INIT - GPIO 覆盖初始化 原有初始化设置丢失
//                                  其余值为操作异常报错
// 使用示例     zf_gpio_init(pin, mode, dat);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_init (zf_gpio_pin_enum pin, zf_gpio_mode_enum mode, const uint8 dat);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 复用功能初始化 一般是内部调用
// 参数说明     pin                 选择的引脚 (可选择范围由 zf_driver_gpio.h 内 zf_gpio_pin_enum 枚举值确定)
// 参数说明     mode                引脚的模式 (可选择范围由 zf_driver_gpio.h 内 zf_gpio_mode_enum 枚举值确定)
// 参数说明     af                  引脚的功能选择 (可选择范围由 zf_driver_gpio.h 内 zf_gpio_af_enum 枚举值确定)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 操作完成
//                                  GPIO_WARNING_PIN_OVERWRITES_INIT - GPIO 覆盖初始化 原有初始化设置丢失
//                                  其余值为操作异常报错
// 使用示例     zf_afio_init(pin, mode, af);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_afio_init (zf_gpio_pin_enum pin, zf_gpio_mode_enum mode, zf_gpio_af_enum af);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#endif
