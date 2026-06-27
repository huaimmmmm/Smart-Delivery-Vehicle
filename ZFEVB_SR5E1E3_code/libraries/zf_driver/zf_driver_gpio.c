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
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2025-03-18        Teternal            first version
********************************************************************************************************************/

// zf_common 层引用
#include "zf_common_debug.h"
#include "zf_common_memory.h"

// 自身头文件
#include "zf_driver_gpio.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION_START
#define GPIO_INPUT_ANAOG_CFG        ( \
    GPIO_MODE_MODER_ANALOG          | GPIO_MODE_OTYPER_PUSHPULL     | GPIO_MODE_OSPEEDR_VERYHIGH    | \
    GPIO_MODE_PUPDR_FLOATING        | GPIO_MODE_AFR(0U)             | GPIO_MODE_IHYSTR_CMOS         | \
    GPIO_MODE_TRIGENR_IN_DISABLED   | GPIO_MODE_SAFESELR_DISABLED   | GPIO_MODE_SAFEVALR_LOW        )

#define GPIO_INPUT_FLOATING_CFG     ( \
    GPIO_MODE_MODER_INPUT           | GPIO_MODE_OTYPER_PUSHPULL     | GPIO_MODE_OSPEEDR_VERYHIGH    | \
    GPIO_MODE_PUPDR_FLOATING        | GPIO_MODE_AFR(0U)             | GPIO_MODE_IHYSTR_CMOS         | \
    GPIO_MODE_TRIGENR_IN_ENABLED    | GPIO_MODE_SAFESELR_DISABLED   | GPIO_MODE_SAFEVALR_LOW        )

#define GPIO_INPUT_PULLDOWN_CFG     ( \
    GPIO_MODE_MODER_INPUT           | GPIO_MODE_OTYPER_PUSHPULL     | GPIO_MODE_OSPEEDR_VERYHIGH    | \
    GPIO_MODE_PUPDR_PULLDOWN        | GPIO_MODE_AFR(0U)             | GPIO_MODE_IHYSTR_CMOS         | \
    GPIO_MODE_TRIGENR_IN_ENABLED    | GPIO_MODE_SAFESELR_DISABLED   | GPIO_MODE_SAFEVALR_LOW        )

#define GPIO_INPUT_PULLUP_CFG       ( \
    GPIO_MODE_MODER_INPUT           | GPIO_MODE_OTYPER_PUSHPULL     | GPIO_MODE_OSPEEDR_VERYHIGH    | \
    GPIO_MODE_PUPDR_PULLUP          | GPIO_MODE_AFR(0U)             | GPIO_MODE_IHYSTR_CMOS         | \
    GPIO_MODE_TRIGENR_IN_ENABLED    | GPIO_MODE_SAFESELR_DISABLED   | GPIO_MODE_SAFEVALR_LOW        )

#define GPIO_OUTPUT_PUSHPULL_CFG   ( \
    GPIO_MODE_MODER_OUTPUT          | GPIO_MODE_OTYPER_PUSHPULL     | GPIO_MODE_OSPEEDR_VERYHIGH    | \
    GPIO_MODE_PUPDR_PULLUP          | GPIO_MODE_AFR(0U)             | GPIO_MODE_IHYSTR_CMOS         | \
    GPIO_MODE_TRIGENR_IN_ENABLED    | GPIO_MODE_SAFESELR_DISABLED   | GPIO_MODE_SAFEVALR_LOW        )

#define GPIO_OUTPUT_OPENDRAIN_CFG   ( \
    GPIO_MODE_MODER_OUTPUT          | GPIO_MODE_OTYPER_OPENDRAIN    | GPIO_MODE_OSPEEDR_VERYHIGH    | \
    GPIO_MODE_PUPDR_PULLUP          | GPIO_MODE_AFR(0U)             | GPIO_MODE_IHYSTR_CMOS         | \
    GPIO_MODE_TRIGENR_IN_ENABLED    | GPIO_MODE_SAFESELR_DISABLED   | GPIO_MODE_SAFEVALR_LOW        )

#define GPIO_AFR_PUSHPULL_CFG       ( \
    GPIO_MODE_MODER_ALTERNATE       | GPIO_MODE_OTYPER_PUSHPULL     | GPIO_MODE_OSPEEDR_VERYHIGH    | \
    GPIO_MODE_PUPDR_PULLUP          | GPIO_MODE_AFR(0U)             | GPIO_MODE_IHYSTR_CMOS         | \
    GPIO_MODE_TRIGENR_IN_ENABLED    | GPIO_MODE_SAFESELR_DISABLED   | GPIO_MODE_SAFEVALR_LOW        )

#define GPIO_AFR_OPENDRAIN_CFG      ( \
    GPIO_MODE_MODER_ALTERNATE       | GPIO_MODE_OTYPER_OPENDRAIN    | GPIO_MODE_OSPEEDR_VERYHIGH    | \
    GPIO_MODE_PUPDR_PULLUP          | GPIO_MODE_AFR(0U)             | GPIO_MODE_IHYSTR_CMOS         | \
    GPIO_MODE_TRIGENR_IN_ENABLED    | GPIO_MODE_SAFESELR_DISABLED   | GPIO_MODE_SAFEVALR_LOW        )

AT_ZF_LIB_SECTION const uint32 gpio_pin_moed_config[8] = 
{
    GPIO_INPUT_ANAOG_CFG    , GPIO_INPUT_FLOATING_CFG  , GPIO_INPUT_PULLDOWN_CFG, GPIO_INPUT_PULLUP_CFG ,
    GPIO_OUTPUT_PUSHPULL_CFG, GPIO_OUTPUT_OPENDRAIN_CFG, GPIO_AFR_PUSHPULL_CFG  , GPIO_AFR_OPENDRAIN_CFG,
};

AT_ZF_LIB_SECTION GPIO_TypeDef* const gpio_group_list[GPIO_GROUP_MAX] =                           // 这个数组用于定向指向 GPIO 寄存器
{
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
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
uint8 zf_gpio_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 输出设置
// 参数说明     pin                 选择的引脚      (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     dat                 GPIO_LOW - 低电平 / GPIO_HIGH - 高电平
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_gpio_set_level(pin, dat);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_set_level (zf_gpio_pin_enum pin, const uint8 dat)
{
    uint8   return_state    =   ZF_ERROR;

    do
    {
        if(zf_gpio_assert(PIN_NULL != pin))                                     // 检查 引脚是否合法
        {
            break;
        }

        uint8   io_group    = (pin  >>  GPIO_GROUP_INDEX_OFFSET);               // 提取IO分组
        uint8   io_index    = (pin  &   GPIO_PIN_INDEX_MASK);                   // 提取IO引脚下标

        (   (dat) ?
            (gpio_group_list[io_group]->BSRR |= ((uint32)0x00000001 << io_index)) :
            (gpio_group_list[io_group]->BSRR |= ((uint32)0x00010000 << io_index)));

        return_state = ZF_NO_ERROR;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 电平获取
// 参数说明     pin                 选择的引脚      (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 返回参数     uint8               引脚当前电平 GPIO_LOW - 低电平 / GPIO_HIGH - 高电平
// 使用示例     uint8 status = zf_gpio_get_level(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_get_level (zf_gpio_pin_enum pin)
{
    uint8   return_value    =   GPIO_LOW;

    do
    {
        if(zf_gpio_assert(PIN_NULL != pin))                                     // 检查 引脚是否合法
        {
            break;
        }

        uint8   io_group    = (pin  >>  GPIO_GROUP_INDEX_OFFSET);               // 提取IO分组
        uint8   io_index    = (pin  &   GPIO_PIN_INDEX_MASK);                   // 提取IO引脚下标

        return_value = ((gpio_group_list[io_group]->IDR
                    &   (0x00000001 << io_index)) ? (GPIO_HIGH) : (GPIO_LOW));
    }while(0);

    return return_value;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 翻转电平
// 参数说明     pin                 选择的引脚      (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_gpio_toggle_level(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_toggle_level (zf_gpio_pin_enum pin)
{
    uint8   return_state    =   ZF_ERROR;

    do
    {
        if(zf_gpio_assert(PIN_NULL != pin))                                     // 检查 引脚是否合法
        {
            break;
        }

        uint8   io_group    = (pin  >>  GPIO_GROUP_INDEX_OFFSET);               // 提取IO分组
        uint8   io_index    = (pin  &   GPIO_PIN_INDEX_MASK);                   // 提取IO引脚下标

        if((gpio_group_list[io_group]->IDR & (0x00000001 << io_index)))
        {
            (gpio_group_list[io_group]->BSRR |= ((uint32)0x00010000 << io_index));
        }
        else
        {
            (gpio_group_list[io_group]->BSRR |= ((uint32)0x00000001 << io_index));
        }

        return_state = ZF_NO_ERROR;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 方向设置
// 参数说明     pin                 选择的引脚      (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     mode                引脚的模式      (详见 zf_driver_gpio.h 内 zf_gpio_mode_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_gpio_set_dir(pin, mode);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_set_dir (zf_gpio_pin_enum pin, zf_gpio_mode_enum mode)
{
    uint8   return_state    =   ZF_ERROR;

    do
    {
        if(zf_gpio_assert(PIN_NULL != pin))                                     // 检查 引脚是否合法
        {
            break;
        }

        uint32 gpio_pin_mode_set = gpio_pin_moed_config[(mode & GPIO_PIN_CONFIG_MASK) + (mode >> GPIO_PIN_DIR_OFFSET) * 4];
        gpio_set_pin_moder(((pin & 0xFE0) >> 1) | (pin & 0x1F), gpio_pin_mode_set);

        return_state = ZF_NO_ERROR;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 注销初始化 设置为开漏输出 并输出高电平 维持高阻态
// 参数说明     pin                 选择的引脚      (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_gpio_deinit(pin);
// 备注信息     当 GPIOx 没有任何引脚初始化时将会关闭 GPIOx 的时钟降低功耗
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_gpio_deinit (zf_gpio_pin_enum pin)
{
    uint8   return_state    =   ZF_ERROR;

    do
    {
        if(zf_gpio_assert(PIN_NULL != pin))                                     // 检查 引脚是否合法
        {
            break;
        }

        gpio_set_pin_mode(((pin & 0xFE0) >> 1) | (pin & 0x1F), GPIO_OUTPUT_OPENDRAIN_CFG);
        zf_gpio_set_level(pin, GPIO_HIGH);

        return_state = ZF_NO_ERROR;
    }while(0);

    return return_state;
}

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
uint8 zf_gpio_init (zf_gpio_pin_enum pin, zf_gpio_mode_enum mode, const uint8 dat)
{
    uint8   return_state    =   ZF_ERROR;

    do
    {
        if(zf_gpio_assert(PIN_NULL != pin))                                     // 检查 引脚是否合法
        {
            break;
        }

        if(     (mode >> GPIO_PIN_DIR_OFFSET)
            &&  (       0x01 == (pin >> (GPIO_GROUP_INDEX_OFFSET & GPIO_GROUP_INDEX_MASK))
                    ||  0x02 == (pin >> (GPIO_GROUP_INDEX_OFFSET & GPIO_GROUP_INDEX_MASK))))
        {
            if(B1 != pin && C5 !=  pin)
            {
                zf_gpio_assert(0);
                // zf_gpio_log(0, "SR5E1E3 Group B & Group C, can not set output mode.");
            }
        }

        uint32 gpio_pin_mode_set = gpio_pin_moed_config[(mode & GPIO_PIN_CONFIG_MASK) + (mode >> GPIO_PIN_DIR_OFFSET) * 4];
        gpio_set_pin_mode(((pin & 0xFE0) >> 1) | (pin & 0x1F), gpio_pin_mode_set);

        if(mode >> GPIO_PIN_DIR_OFFSET)
        {
            zf_gpio_set_level(pin, dat);                                        // 初始化电平设置
        }

        return_state = ZF_NO_ERROR;
    }while(0);

    return return_state;
}

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
uint8 zf_afio_init (zf_gpio_pin_enum pin, zf_gpio_mode_enum mode, zf_gpio_af_enum af)
{
    uint8   return_state    =   ZF_ERROR;

    do
    {
        if(zf_gpio_assert(PIN_NULL != pin))                                     // 检查 引脚是否合法
        {
            break;
        }

        if(     (mode >> GPIO_PIN_DIR_OFFSET)
            &&  (       0x01 == (pin >> (GPIO_GROUP_INDEX_OFFSET & GPIO_GROUP_INDEX_MASK))
                    ||  0x02 == (pin >> (GPIO_GROUP_INDEX_OFFSET & GPIO_GROUP_INDEX_MASK))))
        {
            if(B1 != pin && C5 !=  pin)
            {
                zf_gpio_assert(0);
                // zf_gpio_log(0, "SR5E1E3 Group B & Group C, can not set output mode.");
            }
        }

        uint32 gpio_pin_mode_set = gpio_pin_moed_config[(mode & GPIO_PIN_CONFIG_MASK) + (mode >> GPIO_PIN_DIR_OFFSET) * 4];
        gpio_pin_mode_set |= GPIO_MODE_AFR(af);
        gpio_set_pin_mode(((pin & 0xFE0) >> 1) | (pin & 0x1F), gpio_pin_mode_set);

        return_state = ZF_NO_ERROR;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
