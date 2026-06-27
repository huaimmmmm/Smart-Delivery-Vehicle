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
* 文件名称          zf_driver_pwm
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

// zf_common 层引用
#include "zf_common_debug.h"
#include "zf_common_memory.h"

// 自身头文件
#include "zf_driver_pwm.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的

#define     PWM_CHANNEL_PIN_DEFAULT     \
{   \
    PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,\
    PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,\
}

AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION zf_pwm_obj_struct pwm_obj_list[PWM_NUM_MAX] =
{
    {.timer_obj = &timer_obj_list[PWM_TIM1 ], .pin_list = PWM_CHANNEL_PIN_DEFAULT, .freq = 0, .config_info = 0},
    {.timer_obj = &timer_obj_list[PWM_TIM8 ], .pin_list = PWM_CHANNEL_PIN_DEFAULT, .freq = 0, .config_info = 0},
    {.timer_obj = &timer_obj_list[PWM_TIM2 ], .pin_list = PWM_CHANNEL_PIN_DEFAULT, .freq = 0, .config_info = 0},
    {.timer_obj = &timer_obj_list[PWM_TIM5 ], .pin_list = PWM_CHANNEL_PIN_DEFAULT, .freq = 0, .config_info = 0},
    {.timer_obj = &timer_obj_list[PWM_TIM3 ], .pin_list = PWM_CHANNEL_PIN_DEFAULT, .freq = 0, .config_info = 0},
    {.timer_obj = &timer_obj_list[PWM_TIM4 ], .pin_list = PWM_CHANNEL_PIN_DEFAULT, .freq = 0, .config_info = 0},
    {.timer_obj = &timer_obj_list[PWM_TIM15], .pin_list = PWM_CHANNEL_PIN_DEFAULT, .freq = 0, .config_info = 0},
    {.timer_obj = &timer_obj_list[PWM_TIM16], .pin_list = PWM_CHANNEL_PIN_DEFAULT, .freq = 0, .config_info = 0},
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的外部重载函数 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 更新占空比
// 参数说明     pin                 选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_positive_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_set_duty(pin, duty);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_set_duty (zf_pwm_positive_channel_enum pin, const uint32 duty)
{
    zf_pwm_operation_state_enum return_state = PWM_ERROR_UNKNOW;

    do
    {
        zf_pwm_index_enum           pwm_index       = (zf_pwm_index_enum)((pin >> PWM_INDEX_OFFSET) & PWM_INDEX_MASK);
        zf_pwm_channel_index_enum   channel_index   = (zf_pwm_channel_index_enum)((pin >> PWM_CHANNEL_OFFSET) & PWM_CHANNEL_MASK);

        if(zf_pwm_assert(PIN_NULL != pwm_obj_list[pwm_index].pin_list[channel_index]))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = PWM_ERROR_CHANNEL_NOT_INIT;                          // PWM 通道未初始化 操作无法进行
            break;
        }
        if(zf_pwm_assert(PWM_DUTY_MAX >= duty))
        {
            // 此处如果断言报错 那么证明占空比超过了限定最大值
            return_state = PWM_ERROR_DUTY_ILLEGAL;                              // PWM 占空比值异常 操作无法进行
            break;
        }

        uint16 period_temp = pwm_obj_list[pwm_index].timer_obj->period;         // 获取自动重装载值
        uint16 match_temp = (uint16)(period_temp * ((float)duty / PWM_DUTY_MAX));   // 计算占空比
        if(match_temp == period_temp)
        {
            match_temp +=1;
        }
        switch(((pin >> PWM_CHANNEL_OFFSET) & PWM_CHANNEL_MASK))
        {
            case PWM_CH1:
            case PWM_CH1N:  pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCR1 = match_temp; break;
            case PWM_CH2:
            case PWM_CH2N:  pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCR2 = match_temp; break;
            case PWM_CH3:
            case PWM_CH3N:  pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCR3 = match_temp; break;
            case PWM_CH4:
            case PWM_CH4N:  pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCR4 = match_temp; break;
        }

        return_state = PWM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 模块设置频率
// 参数说明     pwm_index           选择 PWM 模块   (详见 zf_driver_pwm.h 内 zf_pwm_index_enum 定义)
// 参数说明     freq                设置频率 同个模块只有最后一次设置生效
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_set_freq(pwm_index, freq);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_set_freq (zf_pwm_index_enum pwm_index, const uint32 freq)
{
    zf_pwm_operation_state_enum return_state = PWM_ERROR_UNKNOW;

    do
    {
        if(zf_pwm_assert(pwm_obj_list[pwm_index].freq))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = PWM_ERROR_MODULE_NOT_INIT;                           // PWM 模块未初始化 操作无法进行
            break;
        }
        if(zf_pwm_assert(freq))
        {
            // 此处如果断言报错 那么证明输入的频率为 0
            return_state = PWM_ERROR_DUTY_ILLEGAL;                              // PWM 占空比值异常 操作无法进行
            break;
        }

        zf_timer_disable((zf_timer_index_enum)pwm_index);

        uint32 freq_temp    = (pwm_obj_list[pwm_index].alignment ? freq * 2 : freq);
        uint16 freq_div     = ((pwm_obj_list[pwm_index].timer_obj->clock_input / freq_temp) >> 15);         // 计算预分频
        uint16 period_temp  = (pwm_obj_list[pwm_index].timer_obj->clock_input / freq_temp / (freq_div + 1));// 计算自动重装载值

        pwm_obj_list[pwm_index].freq                = pwm_obj_list[pwm_index].timer_obj->clock_input / (freq_div + 1) / period_temp;
        pwm_obj_list[pwm_index].timer_obj->div      = (freq_div + 1);
        pwm_obj_list[pwm_index].timer_obj->period   = period_temp - 1;

        pwm_obj_list[pwm_index].timer_obj->tim_ptr->PSC = freq_div;
        pwm_obj_list[pwm_index].timer_obj->tim_ptr->ARR = period_temp - 1;
        pwm_obj_list[pwm_index].timer_obj->tim_ptr->CNT = 0;

        zf_timer_enable((zf_timer_index_enum)pwm_index);

        return_state = PWM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 注销互补初始化
// 参数说明     positive_pin        选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_positive_channel_enum 定义)
// 参数说明     negative_pin        选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_negative_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_complementary_deinit(positive_pin, negative_pin);
// 备注信息     必须先调用 zf_pwm_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_complementary_deinit (zf_pwm_positive_channel_enum positive_pin, zf_pwm_negative_channel_enum negative_pin)
{
    zf_pwm_operation_state_enum return_state = PWM_ERROR_UNKNOW;

    do
    {
        zf_pwm_index_enum           positive_pwm_index      = (zf_pwm_index_enum)((positive_pin >> PWM_INDEX_OFFSET) & PWM_INDEX_MASK);
        zf_pwm_channel_index_enum   positive_channel_index  = (zf_pwm_channel_index_enum)((positive_pin >> PWM_CHANNEL_OFFSET) & PWM_CHANNEL_MASK);
        zf_pwm_index_enum           negative_pwm_index      = (zf_pwm_index_enum)((negative_pin >> PWM_INDEX_OFFSET) & PWM_INDEX_MASK);
        zf_pwm_channel_index_enum   negative_channel_index  = (zf_pwm_channel_index_enum)((negative_pin >> PWM_CHANNEL_OFFSET) & PWM_CHANNEL_MASK);

        if(zf_pwm_assert(positive_pwm_index == negative_pwm_index))
        {
            // 此处如果断言报错 那么证明输入的互补引脚不属于同一个模块
            // 不是同一个模块的通道怎么能互补？
            return_state = PWM_ERROR_MOUDLE_MISMATCH;
            break;
        }
        if(zf_pwm_assert(positive_channel_index == (negative_channel_index - 4)))
        {
            // 此处如果断言报错 那么证明输入的互补引脚不属于同一个通道
            // 不是同一个模块的通道怎么能互补？
            return_state = PWM_ERROR_COMPLEMENTARY_MISMATCH;
            break;
        }

        if(zf_pwm_assert(
                pwm_obj_list[positive_pwm_index].channel_state & (0x01 << positive_channel_index)
            &&  pwm_obj_list[negative_pwm_index].channel_state & (0x01 << negative_channel_index)))
        {
            // 此处如果断言报错 那么证明输入的互补引脚 未曾初始化过
            // 不是同一个模块的通道怎么能互补？
            return_state = PWM_ERROR_COMPLEMENTARY_NOT_INIT;
            break;
        }
        if(zf_pwm_assert(
                pwm_obj_list[positive_pwm_index].pin_list[positive_channel_index]
            == (zf_gpio_pin_enum)(positive_pin & PWM_PIN_INDEX_MASK)))
        {
            // 此处如果断言报错 那么证明输入的引脚 与之前初始化的引脚不匹配
            return_state = PWM_ERROR_PIN_MISMATCH;
            break;
        }
        if(zf_pwm_assert(
                pwm_obj_list[negative_pwm_index].pin_list[negative_channel_index]
            == (zf_gpio_pin_enum)(negative_pin & PWM_PIN_INDEX_MASK)))
        {
            // 此处如果断言报错 那么证明输入的引脚 与之前初始化的引脚不匹配
            return_state = PWM_ERROR_PIN_MISMATCH;
            break;
        }

        uint32 register_temp = 0;

        register_temp = pwm_obj_list[positive_pwm_index].timer_obj->tim_ptr->CCER;
        register_temp &= ~(TIM_CCER_CC1E << (positive_channel_index / 2 * 4 + positive_channel_index / 4 * 2));
        pwm_obj_list[positive_pwm_index].timer_obj->tim_ptr->CCER = register_temp;

        zf_gpio_deinit(pwm_obj_list[positive_pwm_index].pin_list[positive_channel_index]);
        pwm_obj_list[positive_pwm_index].pin_list[positive_channel_index] = PIN_NULL;

        register_temp = pwm_obj_list[negative_pwm_index].timer_obj->tim_ptr->CCER;
        register_temp &= ~(TIM_CCER_CC1E << (negative_channel_index / 2 * 4 + negative_channel_index / 4 * 2));
        pwm_obj_list[negative_pwm_index].timer_obj->tim_ptr->CCER = register_temp;

        zf_gpio_deinit(pwm_obj_list[negative_pwm_index].pin_list[negative_channel_index]);
        pwm_obj_list[negative_pwm_index].pin_list[negative_channel_index] = PIN_NULL;

        pwm_obj_list[positive_pwm_index].channel_state &= ~(0x01 << positive_channel_index);
        pwm_obj_list[negative_pwm_index].channel_state &= ~(0x01 << negative_channel_index);

        return_state = PWM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 通道销初始化
// 参数说明     pin                 选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_positive_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_channel_deinit(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_channel_deinit (zf_pwm_positive_channel_enum pin)
{
    zf_pwm_operation_state_enum return_state = PWM_ERROR_UNKNOW;

    do
    {
        zf_pwm_index_enum           pwm_index       = (zf_pwm_index_enum)((pin >> PWM_INDEX_OFFSET) & PWM_INDEX_MASK);
        zf_pwm_channel_index_enum   channel_index   = (zf_pwm_channel_index_enum)((pin >> PWM_CHANNEL_OFFSET) & PWM_CHANNEL_MASK);

        if(zf_pwm_assert(PIN_NULL != pwm_obj_list[pwm_index].pin_list[channel_index]))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = PWM_ERROR_CHANNEL_NOT_INIT;                          // PWM 通道未初始化 操作无法进行
            break;
        }

        uint32 register_temp = 0;
        register_temp = pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCER;
        register_temp &= ~(TIM_CCER_CC1E << (channel_index / 2 * 4 + channel_index / 4 * 2));
        pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCER = register_temp;

        zf_gpio_deinit(pwm_obj_list[pwm_index].pin_list[channel_index]);
        pwm_obj_list[pwm_index].pin_list[channel_index] = PIN_NULL;

        pwm_obj_list[pwm_index].channel_state &= ~(0x01 << channel_index);

        return_state = PWM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 模块注销初始化
// 参数说明     pwm_index           选择 PWM 模块   (详见 zf_driver_pwm.h 内 zf_pwm_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_module_deinit(pwm_index);
// 备注信息     模块注销初始化会连带注销当前模块下所有引脚
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_module_deinit (zf_pwm_index_enum pwm_index)
{
    zf_pwm_operation_state_enum return_state = PWM_ERROR_UNKNOW;

    do
    {
        if(zf_pwm_assert(pwm_obj_list[pwm_index].freq))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = PWM_ERROR_MODULE_NOT_INIT;                           // PWM 模块未初始化 操作无法进行
            break;
        }

        for(zf_pwm_channel_index_enum i = PWM_CH1; i <= PWM_CH4N; i ++)
        {
            if(PIN_NULL != pwm_obj_list[pwm_index].pin_list[i])
            {
                uint32 register_temp = 0;
                register_temp = pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCER;
                register_temp &= ~(TIM_CCER_CC1E << (i / 2 * 4 + i / 4 * 2));
                pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCER = register_temp;
                zf_gpio_deinit(pwm_obj_list[pwm_index].pin_list[i]);
                pwm_obj_list[pwm_index].pin_list[i] = PIN_NULL;
            }
        }
        pwm_obj_list[pwm_index].config_info = 0;
        pwm_obj_list[pwm_index].self_index  = pwm_index;
        pwm_obj_list[pwm_index].freq        = 0;
        zf_timer_deinit((zf_timer_index_enum)pwm_index);

        return_state = PWM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 互补初始化
// 参数说明     positive_pin        选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_positive_channel_enum 定义)
// 参数说明     negative_pin        选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_negative_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_complementary_init(positive_pin, negative_pin, duty);
// 备注信息     必须先调用 zf_pwm_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_complementary_init (zf_pwm_positive_channel_enum positive_pin, zf_pwm_negative_channel_enum negative_pin, const uint32 duty)
{
    zf_pwm_operation_state_enum return_state = PWM_ERROR_UNKNOW;

    do
    {
        zf_pwm_index_enum           positive_pwm_index      = (zf_pwm_index_enum)((positive_pin >> PWM_INDEX_OFFSET) & PWM_INDEX_MASK);
        zf_pwm_channel_index_enum   positive_channel_index  = (zf_pwm_channel_index_enum)((positive_pin >> PWM_CHANNEL_OFFSET) & PWM_CHANNEL_MASK);
        zf_pwm_index_enum           negative_pwm_index      = (zf_pwm_index_enum)((negative_pin >> PWM_INDEX_OFFSET) & PWM_INDEX_MASK);
        zf_pwm_channel_index_enum   negative_channel_index  = (zf_pwm_channel_index_enum)((negative_pin >> PWM_CHANNEL_OFFSET) & PWM_CHANNEL_MASK);

        if(zf_pwm_assert(positive_pwm_index == negative_pwm_index))
        {
            // 此处如果断言报错 那么证明输入的互补引脚不属于同一个模块
            // 不是同一个模块的通道怎么能互补？
            return_state = PWM_ERROR_MOUDLE_MISMATCH;
            break;
        }
        if(zf_pwm_assert(positive_channel_index == (negative_channel_index - 4)))
        {
            // 此处如果断言报错 那么证明输入的互补引脚不属于同一个通道
            // 不是同一个模块的通道怎么能互补？
            return_state = PWM_ERROR_COMPLEMENTARY_MISMATCH;
            break;
        }

        if(zf_pwm_assert(pwm_obj_list[positive_pwm_index].freq))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = PWM_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_pwm_assert(
            !(      pwm_obj_list[positive_pwm_index].channel_state
                &   (0x01 << positive_channel_index))))
        {
            // 此处如果断言报错 那么证明引脚所在通道已经被初始化过 不能重复初始化占用
            return_state = PWM_ERROR_CHANNEL_OCCUPIED;
            break;
        }
        if(zf_pwm_assert(
            !(      pwm_obj_list[negative_pwm_index].channel_state
                &   (0x01 << negative_channel_index))))
        {
            // 此处如果断言报错 那么证明引脚所在通道已经被初始化过 不能重复初始化占用
            return_state = PWM_ERROR_CHANNEL_OCCUPIED;
            break;
        }
        if(zf_pwm_assert(PWM_DUTY_MAX >= duty))
        {
            // 此处如果断言报错 那么证明占空比超过了限定最大值
            return_state = PWM_ERROR_DUTY_ILLEGAL;                              // PWM 占空比值异常 操作无法进行
            break;
        }

        uint32 register_temp = 0;

        (       (positive_channel_index % 4 > PWM_CH2)
            ?   (register_temp = pwm_obj_list[positive_pwm_index].timer_obj->tim_ptr->CCMR2)
            :   (register_temp = pwm_obj_list[positive_pwm_index].timer_obj->tim_ptr->CCMR1));
        register_temp &= ~(TIM_CCMR1_CC1S                           << ((positive_channel_index % 2) * 8));
        register_temp |=  ((TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2)    << ((positive_channel_index % 2) * 8));
        register_temp |=  (TIM_CCMR1_OC1PE                          << ((positive_channel_index % 2) * 8));
        register_temp |=  (TIM_CCMR1_OC1CE                          << ((positive_channel_index % 2) * 8));
        (       (positive_channel_index % 4 > PWM_CH2)
            ?   (pwm_obj_list[positive_pwm_index].timer_obj->tim_ptr->CCMR2 = register_temp)
            :   (pwm_obj_list[positive_pwm_index].timer_obj->tim_ptr->CCMR1 = register_temp));

        (       (negative_channel_index % 4 > PWM_CH2)
            ?   (register_temp = pwm_obj_list[negative_pwm_index].timer_obj->tim_ptr->CCMR2)
            :   (register_temp = pwm_obj_list[negative_pwm_index].timer_obj->tim_ptr->CCMR1));
        register_temp &= ~(TIM_CCMR1_CC1S                           << ((negative_channel_index % 2) * 8));
        register_temp |=  ((TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2)    << ((negative_channel_index % 2) * 8));
        register_temp |=  (TIM_CCMR1_OC1PE                          << ((negative_channel_index % 2) * 8));
        register_temp |=  (TIM_CCMR1_OC1CE                          << ((negative_channel_index % 2) * 8));
        (       (negative_channel_index % 4 > PWM_CH2)
            ?   (pwm_obj_list[negative_pwm_index].timer_obj->tim_ptr->CCMR2 = register_temp)
            :   (pwm_obj_list[negative_pwm_index].timer_obj->tim_ptr->CCMR1 = register_temp));


        register_temp = pwm_obj_list[positive_pwm_index].timer_obj->tim_ptr->CCER;
        register_temp |= (TIM_CCER_CC1E << (positive_channel_index % 4 * 4 + positive_channel_index / 4 * 2));
        register_temp |= (TIM_CCER_CC1E << (negative_channel_index % 4 * 4 + negative_channel_index / 4 * 2));
        pwm_obj_list[positive_pwm_index].timer_obj->tim_ptr->CCER = register_temp;

        switch(positive_pwm_index)
        {
            default:        break;
            case TIM_1 :
            case TIM_8 :
            case TIM_15:
            case TIM_16:    pwm_obj_list[positive_pwm_index].timer_obj->tim_ptr->BDTR |= TIM_BDTR_MOE;   break;
        }

        pwm_obj_list[positive_pwm_index].channel_state |= (0x01 << positive_channel_index);
        pwm_obj_list[negative_pwm_index].channel_state |= (0x01 << negative_channel_index);

        pwm_obj_list[positive_pwm_index].pin_list[positive_channel_index] = (zf_gpio_pin_enum)(positive_pin & PWM_PIN_INDEX_MASK);
        pwm_obj_list[negative_pwm_index].pin_list[negative_channel_index] = (zf_gpio_pin_enum)(negative_pin & PWM_PIN_INDEX_MASK);
        zf_pwm_set_duty(positive_pin, duty);
        zf_afio_init(
            (zf_gpio_pin_enum)(positive_pin & PWM_PIN_INDEX_MASK),
            GPO_AF_PUSH_PULL,
            (zf_gpio_af_enum)((positive_pin >> PWM_PIN_AF_OFFSET) & PWM_PIN_AF_MASK));
        zf_afio_init(
            (zf_gpio_pin_enum)(negative_pin & PWM_PIN_INDEX_MASK),
            GPO_AF_PUSH_PULL,
            (zf_gpio_af_enum)((negative_pin >> PWM_PIN_AF_OFFSET) & PWM_PIN_AF_MASK));

        return_state = PWM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 通道初始化
// 参数说明     pin                 选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_positive_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_channel_init(pin, duty);
// 备注信息     必须先调用 zf_pwm_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_channel_init (zf_pwm_positive_channel_enum pin, const uint32 duty)
{
    zf_pwm_operation_state_enum return_state = PWM_ERROR_UNKNOW;

    do
    {
        zf_pwm_index_enum           pwm_index       = (zf_pwm_index_enum)((pin >> PWM_INDEX_OFFSET) & PWM_INDEX_MASK);
        zf_pwm_channel_index_enum   channel_index   = (zf_pwm_channel_index_enum)((pin >> PWM_CHANNEL_OFFSET) & PWM_CHANNEL_MASK);

        if(zf_pwm_assert(pwm_obj_list[pwm_index].freq))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = PWM_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_pwm_assert(
            !(      pwm_obj_list[pwm_index].channel_state
                &   (0x01 << channel_index))))
        {
            // 此处如果断言报错 那么证明引脚所在通道已经被初始化过 不能重复初始化占用
            return_state = PWM_ERROR_CHANNEL_OCCUPIED;
        }
        if(zf_pwm_assert(PWM_DUTY_MAX >= duty))
        {
            // 此处如果断言报错 那么证明占空比超过了限定最大值
            return_state = PWM_ERROR_DUTY_ILLEGAL;                              // PWM 占空比值异常 操作无法进行
            break;
        }

        uint32 register_temp = 0;
        (       (channel_index % 4 > PWM_CH2)
            ?   (register_temp = pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCMR2)
            :   (register_temp = pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCMR1));
        register_temp &= ~(TIM_CCMR1_CC1S                           << ((channel_index % 2) * 8));
        register_temp |=  ((TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2)    << ((channel_index % 2) * 8));
        register_temp |=  (TIM_CCMR1_OC1PE                          << ((channel_index % 2) * 8));
        register_temp |=  (TIM_CCMR1_OC1CE                          << ((channel_index % 2) * 8));
        (       (channel_index % 4 > PWM_CH2)
            ?   (pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCMR2 = register_temp)
            :   (pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCMR1 = register_temp));

        register_temp = pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCER;
        register_temp |= (TIM_CCER_CC1E << (channel_index % 4 * 4 + channel_index / 4 * 2));
        pwm_obj_list[pwm_index].timer_obj->tim_ptr->CCER = register_temp;

        switch(pwm_index)
        {
            default:        break;
            case TIM_1 :
            case TIM_8 :
            case TIM_15:
            case TIM_16:    pwm_obj_list[pwm_index].timer_obj->tim_ptr->BDTR |= TIM_BDTR_MOE;   break;
        }

        pwm_obj_list[pwm_index].channel_state |= (0x01 << channel_index);
        pwm_obj_list[pwm_index].pin_list[channel_index] = (zf_gpio_pin_enum)(pin & PWM_PIN_INDEX_MASK);
        zf_pwm_set_duty(pin, duty);
        zf_afio_init(
            (zf_gpio_pin_enum)(pin & PWM_PIN_INDEX_MASK),
            GPO_AF_PUSH_PULL,
            (zf_gpio_af_enum)((pin >> PWM_PIN_AF_OFFSET) & PWM_PIN_AF_MASK));

        return_state = PWM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 模块初始化
// 参数说明     pwm_index           选择 PWM 模块   (详见 zf_driver_pwm.h 内 zf_pwm_index_enum 定义)
// 参数说明     mode                PWM 对齐模式    (详见 zf_driver_pwm.h 内 zf_pwm_alignment_enum 定义)
// 参数说明     freq                设置频率 同个模块只有最后一次设置生效
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_module_init(pwm_index, mode, freq);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_module_init (zf_pwm_index_enum pwm_index, zf_pwm_alignment_enum mode, const uint32 freq)
{
    zf_pwm_operation_state_enum return_state = PWM_ERROR_UNKNOW;

    do
    {
        if(zf_pwm_assert(!zf_timer_funciton_check(                              // 检查 模块是否占用
                (zf_timer_index_enum)pwm_index,                                 // 当前模块
                TIMER_FUNCTION_PWM)))                                           // 编码器功能匹配
        {
            // 此处如果断言报错 那么证明 PWM 所依赖的 TIMER 已经被占用
            // PWM 索引对应 TIMER 索引
            return_state = PWM_ERROR_DEPENDS_TIMER_OCCUPIED;
            break;
        }
        if(zf_pwm_assert(!pwm_obj_list[pwm_index].freq))
        {
            // 此处如果断言报错 那么证明本模块已经被初始化过一次 重复初始化是不允许的
            // 否则应当先注销初始化之后再重新初始化
            return_state = PWM_ERROR_MODULE_OCCUPIED;
            break;
        }
        if(zf_pwm_assert(freq))
        {
            // 此处如果断言报错 那么证明输入的频率为 0
            return_state = PWM_ERROR_DUTY_ILLEGAL;                              // PWM 占空比值异常 操作无法进行
            break;
        }

        uint32 freq_temp    = (mode ? freq * 2 : freq);
        uint16 freq_div     = ((pwm_obj_list[pwm_index].timer_obj->clock_input / freq_temp) >> 15);         // 计算预分频
        uint16 period_temp  = (pwm_obj_list[pwm_index].timer_obj->clock_input / freq_temp / (freq_div + 1));// 计算自动重装载值
        zf_timer_clock_enable((zf_timer_index_enum)pwm_index);

        pwm_obj_list[pwm_index].enable              = ZF_ENABLE;
        pwm_obj_list[pwm_index].self_index          = pwm_index;
        pwm_obj_list[pwm_index].alignment           = mode;
        pwm_obj_list[pwm_index].freq                = pwm_obj_list[pwm_index].timer_obj->clock_input / (freq_div + 1) / period_temp;
        pwm_obj_list[pwm_index].timer_obj->div      = (freq_div + 1);
        pwm_obj_list[pwm_index].timer_obj->period   = period_temp - 1;
        pwm_obj_list[pwm_index].timer_obj->mode     = 3;

        uint32 register_temp = 0;
        register_temp = pwm_obj_list[pwm_index].timer_obj->tim_ptr->CR1;
        register_temp &= ~(TIM_CR1_CMS_Msk);
        register_temp |=  (mode ? TIM_CR1_CMS_Msk : 0);
        pwm_obj_list[pwm_index].timer_obj->tim_ptr->CR1 = register_temp;

        pwm_obj_list[pwm_index].timer_obj->tim_ptr->PSC = freq_div;
        pwm_obj_list[pwm_index].timer_obj->tim_ptr->ARR = period_temp - 1;
        pwm_obj_list[pwm_index].timer_obj->tim_ptr->CNT = 0;

        zf_timer_enable((zf_timer_index_enum)pwm_index);

        return_state = PWM_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
