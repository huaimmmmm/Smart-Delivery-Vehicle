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
* 文件名称          zf_driver_hrtim
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

// zf_common 层引用
#include "zf_common_debug.h"
#include "zf_common_memory.h"

// zf_driver 层引用
#include "zf_driver_interrupt.h"
#include "zf_driver_rcc.h"

// 自身头文件
#include "zf_driver_hrtim.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的

#define     HRTIM_PWM_CHANNEL_PIN_DEFAULT   \
{   \
    PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,\
    PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL\
}

AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION zf_hrtim_obj_struct hrtim_obj_list[HRTIM_NUM_MAX] =
{
    {.hrtimer_obj = HRTIM1, .pin_list = HRTIM_PWM_CHANNEL_PIN_DEFAULT, .clock_input = 9600000000, .freq = 0, .period = 0, .config_info = 0},
    {.hrtimer_obj = HRTIM2, .pin_list = HRTIM_PWM_CHANNEL_PIN_DEFAULT, .clock_input = 9600000000, .freq = 0, .period = 0, .config_info = 0},
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件用使用的静态或常亮数据 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 用于固定值或指向的内容 主要是用少量存储占用来减少栈开销
// static const zf_interrupt_index_enum hrtim_irq_index_list[HRTIM_NUM_MAX][HRTIM_INTERRUPT_NUM_MAX] =
// {
//     {
//         INTERRUPT_INDEX_HRTIM1_MASTER,
//         INTERRUPT_INDEX_HRTIM1_TIMA, INTERRUPT_INDEX_HRTIM1_TIMB, INTERRUPT_INDEX_HRTIM1_TIMC,
//         INTERRUPT_INDEX_HRTIM1_TIMD, INTERRUPT_INDEX_HRTIM1_TIME, INTERRUPT_INDEX_HRTIM1_TIMF
//     },
//     {
//         INTERRUPT_INDEX_HRTIM2_MASTER,
//         INTERRUPT_INDEX_HRTIM2_TIMA, INTERRUPT_INDEX_HRTIM2_TIMB, INTERRUPT_INDEX_HRTIM2_TIMC,
//         INTERRUPT_INDEX_HRTIM2_TIMD, INTERRUPT_INDEX_HRTIM2_TIME, INTERRUPT_INDEX_HRTIM2_TIMF
//     },
// };

static const zf_rcc_index_enum hrtim_rcc_index_list[HRTIM_NUM_MAX] = 
{
    RCC_INDEX_HRTIMER1, RCC_INDEX_HRTIMER2,
};
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_assert_handler(file, line, tips);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 更新占空比
// 参数说明     pin                 选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_positive_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_set_duty(pin, duty);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_set_duty (zf_hrtim_pwm_positive_channel_enum pin, const uint32 duty)
{
    zf_hrtim_operation_state_enum return_state = HRTIM_ERROR_UNKNOW;

    do
    {
        zf_hrtim_index_enum             hrtim_index     = (zf_hrtim_index_enum)((pin >> HRTIM_PWM_INDEX_OFFSET) & HRTIM_PWM_INDEX_MASK);
        zf_hrtim_pwm_channel_index_enum channel_index   = (zf_hrtim_pwm_channel_index_enum)((pin >> HRTIM_PWM_CHANNEL_OFFSET) & HRTIM_PWM_CHANNEL_MASK);

        if(zf_hrtim_assert(hrtim_obj_list[hrtim_index].freq))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = HRTIM_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_hrtim_assert(PIN_NULL != hrtim_obj_list[hrtim_index].pin_list[channel_index]))
        {
            // 此处如果断言报错 那么证明引脚所在通道已经被初始化过 不能重复初始化占用
            return_state = HRTIM_ERROR_CHANNEL_NOT_INIT;
        }
        if(zf_hrtim_assert(HRTIM_DUTY_MAX >= duty))
        {
            // 此处如果断言报错 那么证明占空比超过了限定最大值
            return_state = HRTIM_ERROR_DUTY_ILLEGAL;
            break;
        }

        // 如果 100% 占空比 那么这里就是 match_temp = period
        // 如果 0%   占空比 那么这里就是 match_temp = 0
        // 设计是 CMP1 匹配拉高 CMP2 匹配拉低
        uint16 period_temp = hrtim_obj_list[hrtim_index].period;
        if(duty >= HRTIM_DUTY_MAX / 100 * 98)
        {
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP1xR = period_temp / 2;
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP2xR = 0;
        }
        else if(duty <= HRTIM_DUTY_MAX / 100 * 2)
        {
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP1xR = period_temp;
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP2xR = period_temp / 2;
        }
        else
        {
            uint16 match_temp  = (uint16)(period_temp / 2 * ((float)duty / HRTIM_DUTY_MAX));
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP1xR = (period_temp / 2 - match_temp);
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP2xR = (period_temp / 2 + match_temp);
        }

        return_state = HRTIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 模块设置频率
// 参数说明     hrtim_index         选择 HRTIM 模块     (详见 zf_driver_hrtim.h 内 zf_hrtim_index_enum 定义)
// 参数说明     freq                设置频率 同个模块只有最后一次设置生效
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_set_freq(hrtim_index, freq);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_set_freq (zf_hrtim_index_enum hrtim_index, const uint32 freq)
{
    zf_hrtim_operation_state_enum return_state = HRTIM_ERROR_UNKNOW;

    do
    {
        if(zf_hrtim_assert(hrtim_obj_list[hrtim_index].freq))
        {
            // 此处如果断言报错 那么证明本模块已经被初始化过一次 重复初始化是不允许的
            // 否则应当先注销初始化之后再重新初始化
            return_state = HRTIM_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_hrtim_assert(freq))
        {
            // 此处如果断言报错 那么证明输入的频率为 0
            return_state = HRTIM_ERROR_FREQ_ILLEGAL;
            break;
        }

        hrtim_obj_list[hrtim_index].hrtimer_obj->sMasterRegs.MCR = 0;
        hrtim_obj_list[hrtim_index].hrtimer_obj->sMasterRegs.MCNTR = 0;
        for(uint8 i = 0; i < 6; i ++)
        {
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[i].CNTxR = 0;
        }

        uint32 register_temp    = 0;
        uint16 freq_div         = 0;
        uint16 period_temp      = 0;

        for(uint8 i = 1; i <= 128; i *= 2)
        {
            if(hrtim_obj_list[hrtim_index].clock_input / i / freq < 0xFF00)
            {
                period_temp = hrtim_obj_list[hrtim_index].clock_input / i / freq;
                period_temp = period_temp + period_temp % 2;
                hrtim_obj_list[hrtim_index].freq = hrtim_obj_list[hrtim_index].clock_input / i / period_temp;
                break;
            }
            freq_div += 1;
        }

        hrtim_obj_list[hrtim_index].hrtimer_obj->sMasterRegs.MPER = period_temp - 1;
        for(uint8 i = 0; i < 6; i ++)
        {
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[i].PERxR = period_temp - 1;
        }
        hrtim_obj_list[hrtim_index].period = period_temp;

        register_temp = freq_div | 0x00000008;
        for(uint8 i = 0; i < 6; i ++)
        {
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[i].TIMxCR = register_temp;
        }
        hrtim_obj_list[hrtim_index].hrtimer_obj->sMasterRegs.MCR = register_temp | 0x007F0000;

        return_state = HRTIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 注销互补初始化
// 参数说明     positive_pin        选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_positive_channel_enum 定义)
// 参数说明     negative_pin        选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_negative_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_complementary_deinit(positive_pin, negative_pin);
// 备注信息     必须先调用 zf_hrtim_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_complementary_deinit (zf_hrtim_pwm_positive_channel_enum positive_pin, zf_hrtim_pwm_negative_channel_enum negative_pin)
{
    zf_hrtim_operation_state_enum return_state = HRTIM_ERROR_UNKNOW;

    do
    {
        zf_hrtim_index_enum             positive_hrtim_index    = (zf_hrtim_index_enum)((positive_pin >> HRTIM_PWM_INDEX_OFFSET) & HRTIM_PWM_INDEX_MASK);
        zf_hrtim_pwm_channel_index_enum positive_channel_index  = (zf_hrtim_pwm_channel_index_enum)((positive_pin >> HRTIM_PWM_CHANNEL_OFFSET) & HRTIM_PWM_CHANNEL_MASK);
        zf_hrtim_index_enum             negative_hrtim_index    = (zf_hrtim_index_enum)((negative_pin >> HRTIM_PWM_INDEX_OFFSET) & HRTIM_PWM_INDEX_MASK);
        zf_hrtim_pwm_channel_index_enum negative_channel_index  = (zf_hrtim_pwm_channel_index_enum)((negative_pin >> HRTIM_PWM_CHANNEL_OFFSET) & HRTIM_PWM_CHANNEL_MASK);

        if(zf_hrtim_assert(positive_hrtim_index == negative_hrtim_index))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = HRTIM_ERROR_MOUDLE_MISMATCH;
            break;
        }
        if(zf_hrtim_assert((positive_channel_index + 6) == negative_channel_index))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = HRTIM_ERROR_COMPLEMENTARY_MISMATCH;
            break;
        }

        if(zf_hrtim_assert(hrtim_obj_list[positive_hrtim_index].freq))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = HRTIM_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_hrtim_assert(
                hrtim_obj_list[positive_hrtim_index].channel_state & (0x0001 << positive_channel_index)
            &&  hrtim_obj_list[negative_hrtim_index].channel_state & (0x0001 << negative_channel_index)))
        {
            // 此处如果断言报错 那么证明引脚所在通道已经被初始化过 不能重复初始化占用
            return_state = HRTIM_ERROR_COMPLEMENTARY_NOT_INIT;
        }
        if(zf_hrtim_assert(
            hrtim_obj_list[positive_hrtim_index].pin_list[positive_channel_index]
            == (zf_gpio_pin_enum)(positive_pin & HRTIM_PWM_PIN_INDEX_MASK)))
        {
            // 此处如果断言报错 那么证明引脚所在通道已经被初始化过 不能重复初始化占用
            return_state = HRTIM_ERROR_PIN_MISMATCH;
        }
        if(zf_hrtim_assert(
            hrtim_obj_list[positive_hrtim_index].pin_list[negative_channel_index]
            == (zf_gpio_pin_enum)(negative_pin & HRTIM_PWM_PIN_INDEX_MASK)))
        {
            // 此处如果断言报错 那么证明引脚所在通道已经被初始化过 不能重复初始化占用
            return_state = HRTIM_ERROR_PIN_MISMATCH;
        }

        hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sCommonRegs.OENR &= ~(0x00000003 << ((positive_channel_index % 6) * 2));
        hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[positive_channel_index % 6].SETx1R = 0;
        hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[positive_channel_index % 6].RSTx1R = 0;
        hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[negative_channel_index % 6].SETx2R = 0;
        hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[negative_channel_index % 6].RSTx2R = 0;
        zf_gpio_deinit(hrtim_obj_list[positive_hrtim_index].pin_list[positive_channel_index]);
        zf_gpio_deinit(hrtim_obj_list[positive_hrtim_index].pin_list[negative_channel_index]);
        hrtim_obj_list[positive_hrtim_index].pin_list[positive_channel_index] = PIN_NULL;
        hrtim_obj_list[positive_hrtim_index].pin_list[negative_channel_index] = PIN_NULL;
        hrtim_obj_list[positive_hrtim_index].channel_state &= ~(0x0001 << positive_channel_index);
        hrtim_obj_list[positive_hrtim_index].channel_state &= ~(0x0001 << negative_channel_index);

        return_state = HRTIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 通道销初始化
// 参数说明     pin                 选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_positive_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_channel_deinit(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_channel_deinit (zf_hrtim_pwm_positive_channel_enum pin)
{
    zf_hrtim_operation_state_enum return_state = HRTIM_ERROR_UNKNOW;

    do
    {
        zf_hrtim_index_enum             hrtim_index     = (zf_hrtim_index_enum)((pin >> HRTIM_PWM_INDEX_OFFSET) & HRTIM_PWM_INDEX_MASK);
        zf_hrtim_pwm_channel_index_enum channel_index   = (zf_hrtim_pwm_channel_index_enum)((pin >> HRTIM_PWM_CHANNEL_OFFSET) & HRTIM_PWM_CHANNEL_MASK);

        if(zf_hrtim_assert(hrtim_obj_list[hrtim_index].freq))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = HRTIM_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_hrtim_assert(PIN_NULL != hrtim_obj_list[hrtim_index].pin_list[channel_index]))
        {
            // 此处如果断言报错 那么证明引脚所在通道已经被初始化过 不能重复初始化占用
            return_state = HRTIM_ERROR_CHANNEL_NOT_INIT;
        }

        hrtim_obj_list[hrtim_index].hrtimer_obj->sCommonRegs.OENR &= ~(0x00000001 << ((channel_index % 6) * 2));
        hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].SETx1R = 0;
        hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].RSTx1R = 0;
        zf_gpio_deinit(hrtim_obj_list[hrtim_index].pin_list[channel_index]);
        hrtim_obj_list[hrtim_index].pin_list[channel_index] = PIN_NULL;
        hrtim_obj_list[hrtim_index].channel_state &= ~(0x0001 << channel_index);

        return_state = HRTIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 模块注销初始化
// 参数说明     hrtim_index         选择 HRTIM 模块     (详见 zf_driver_hrtim.h 内 zf_hrtim_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_module_deinit(hrtim_index);
// 备注信息     模块注销初始化会连带注销当前模块下所有引脚
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_module_deinit (zf_hrtim_index_enum hrtim_index)
{
    zf_hrtim_operation_state_enum return_state = HRTIM_ERROR_UNKNOW;

    do
    {
        if(zf_hrtim_assert(hrtim_obj_list[hrtim_index].freq))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = HRTIM_ERROR_MODULE_NOT_INIT;
            break;
        }

        hrtim_obj_list[hrtim_index].hrtimer_obj->sCommonRegs.OENR = 0;
        for(zf_hrtim_pwm_channel_index_enum i = PWM_CHA1; i <= PWM_CHF2; i ++)
        {
            if(PIN_NULL != hrtim_obj_list[hrtim_index].pin_list[i])
            {
                zf_gpio_deinit(hrtim_obj_list[hrtim_index].pin_list[i]);
                hrtim_obj_list[hrtim_index].pin_list[i] = PIN_NULL;
            }
        }
        hrtim_obj_list[hrtim_index].freq = 0;
        hrtim_obj_list[hrtim_index].period = 0;
        hrtim_obj_list[hrtim_index].config_info = 0;

        return_state = HRTIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 互补初始化
// 参数说明     positive_pin        选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_positive_channel_enum 定义)
// 参数说明     negative_pin        选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_negative_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_complementary_init(positive_pin, negative_pin, duty);
// 备注信息     必须先调用 zf_hrtim_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_complementary_init (zf_hrtim_pwm_positive_channel_enum positive_pin, zf_hrtim_pwm_negative_channel_enum negative_pin, const uint32 duty)
{
    zf_hrtim_operation_state_enum return_state = HRTIM_ERROR_UNKNOW;

    do
    {
        zf_hrtim_index_enum             positive_hrtim_index    = (zf_hrtim_index_enum)((positive_pin >> HRTIM_PWM_INDEX_OFFSET) & HRTIM_PWM_INDEX_MASK);
        zf_hrtim_pwm_channel_index_enum positive_channel_index  = (zf_hrtim_pwm_channel_index_enum)((positive_pin >> HRTIM_PWM_CHANNEL_OFFSET) & HRTIM_PWM_CHANNEL_MASK);
        zf_hrtim_index_enum             negative_hrtim_index    = (zf_hrtim_index_enum)((negative_pin >> HRTIM_PWM_INDEX_OFFSET) & HRTIM_PWM_INDEX_MASK);
        zf_hrtim_pwm_channel_index_enum negative_channel_index  = (zf_hrtim_pwm_channel_index_enum)((negative_pin >> HRTIM_PWM_CHANNEL_OFFSET) & HRTIM_PWM_CHANNEL_MASK);

        if(zf_hrtim_assert(positive_hrtim_index == negative_hrtim_index))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = HRTIM_ERROR_MOUDLE_MISMATCH;
            break;
        }
        if(zf_hrtim_assert((positive_channel_index + 6) == negative_channel_index))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = HRTIM_ERROR_COMPLEMENTARY_MISMATCH;
            break;
        }

        if(zf_hrtim_assert(hrtim_obj_list[positive_hrtim_index].freq))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = HRTIM_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_hrtim_assert(
            !(      hrtim_obj_list[positive_hrtim_index].channel_state
                &   (0x0001 << positive_channel_index))))
        {
            // 此处如果断言报错 那么证明引脚所在通道已经被初始化过 不能重复初始化占用
            return_state = HRTIM_ERROR_CHANNEL_OCCUPIED;
        }
        if(zf_hrtim_assert(
            !(      hrtim_obj_list[negative_hrtim_index].channel_state
                &   (0x0001 << negative_channel_index))))
        {
            // 此处如果断言报错 那么证明引脚所在通道已经被初始化过 不能重复初始化占用
            return_state = HRTIM_ERROR_CHANNEL_OCCUPIED;
        }
        if(zf_hrtim_assert(HRTIM_DUTY_MAX >= duty))
        {
            // 此处如果断言报错 那么证明占空比超过了限定最大值
            return_state = HRTIM_ERROR_DUTY_ILLEGAL;                              // PWM 占空比值异常 操作无法进行
            break;
        }

        // 如果 100% 占空比 那么这里就是 match_temp = period
        // 如果 0%   占空比 那么这里就是 match_temp = 0
        // 设计是 CMP1 匹配拉高 CMP2 匹配拉低
        uint16 period_temp = hrtim_obj_list[positive_hrtim_index].period;
        if(duty >= HRTIM_DUTY_MAX / 100 * 98)
        {
            hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[positive_channel_index % 6].CMP1xR = period_temp / 2;
            hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[positive_channel_index % 6].CMP2xR = 0;
        }
        else if(duty <= HRTIM_DUTY_MAX / 100 * 2)
        {
            hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[positive_channel_index % 6].CMP1xR = 0;
            hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[positive_channel_index % 6].CMP2xR = period_temp / 2;
        }
        else
        {
            uint16 match_temp  = (uint16)(period_temp / 2 * ((float)duty / HRTIM_DUTY_MAX));
            hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[positive_channel_index % 6].CMP1xR = (period_temp / 2 - match_temp);
            hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[positive_channel_index % 6].CMP2xR = (period_temp / 2 + match_temp);
        }

        hrtim_obj_list[positive_hrtim_index].channel_state |= (0x0001 << positive_channel_index);
        hrtim_obj_list[positive_hrtim_index].channel_state |= (0x0001 << negative_channel_index);
        hrtim_obj_list[positive_hrtim_index].pin_list[positive_channel_index] = (zf_gpio_pin_enum)(positive_pin & HRTIM_PWM_PIN_INDEX_MASK);
        hrtim_obj_list[positive_hrtim_index].pin_list[negative_channel_index] = (zf_gpio_pin_enum)(negative_pin & HRTIM_PWM_PIN_INDEX_MASK);
        zf_afio_init(
            (zf_gpio_pin_enum)(positive_pin & HRTIM_PWM_PIN_INDEX_MASK),
            GPO_AF_PUSH_PULL,
            (zf_gpio_af_enum)((positive_pin >> HRTIM_PWM_PIN_AF_OFFSET) & HRTIM_PWM_PIN_AF_MASK));
        zf_afio_init(
            (zf_gpio_pin_enum)(negative_pin & HRTIM_PWM_PIN_INDEX_MASK),
            GPO_AF_PUSH_PULL,
            (zf_gpio_af_enum)((negative_pin >> HRTIM_PWM_PIN_AF_OFFSET) & HRTIM_PWM_PIN_AF_MASK));

        hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[positive_channel_index % 6].SETx1R |= 0x00000008;
        hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[positive_channel_index % 6].RSTx1R |= 0x00000010;
        hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[negative_channel_index % 6].SETx2R |= 0x00000010;
        hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sTimerxRegs[negative_channel_index % 6].RSTx2R |= 0x00000008;
        hrtim_obj_list[positive_hrtim_index].hrtimer_obj->sCommonRegs.OENR |= (0x00000003 << ((positive_channel_index % 6) * 2));

        return_state = HRTIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 通道初始化
// 参数说明     pin                 选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_positive_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_channel_init(pin, duty);
// 备注信息     必须先调用 zf_hrtim_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_channel_init (zf_hrtim_pwm_positive_channel_enum pin, const uint32 duty)
{
    zf_hrtim_operation_state_enum return_state = HRTIM_ERROR_UNKNOW;

    do
    {
        zf_hrtim_index_enum             hrtim_index     = (zf_hrtim_index_enum)((pin >> HRTIM_PWM_INDEX_OFFSET) & HRTIM_PWM_INDEX_MASK);
        zf_hrtim_pwm_channel_index_enum channel_index   = (zf_hrtim_pwm_channel_index_enum)((pin >> HRTIM_PWM_CHANNEL_OFFSET) & HRTIM_PWM_CHANNEL_MASK);

        if(zf_hrtim_assert(hrtim_obj_list[hrtim_index].freq))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = HRTIM_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_hrtim_assert(
            !(      hrtim_obj_list[hrtim_index].channel_state
                &   (0x0001 << channel_index))))
        {
            // 此处如果断言报错 那么证明引脚所在通道已经被初始化过 不能重复初始化占用
            return_state = HRTIM_ERROR_CHANNEL_OCCUPIED;
        }
        if(zf_hrtim_assert(HRTIM_DUTY_MAX >= duty))
        {
            // 此处如果断言报错 那么证明占空比超过了限定最大值
            return_state = HRTIM_ERROR_DUTY_ILLEGAL;                              // PWM 占空比值异常 操作无法进行
            break;
        }

        // 如果 100% 占空比 那么这里就是 match_temp = period
        // 如果 0%   占空比 那么这里就是 match_temp = 0
        // 设计是 CMP1 匹配拉高 CMP2 匹配拉低
        uint16 period_temp = hrtim_obj_list[hrtim_index].period;
        if(duty >= HRTIM_DUTY_MAX / 100 * 98)
        {
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP1xR = period_temp / 2;
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP2xR = 0;
        }
        else if(duty <= HRTIM_DUTY_MAX / 100 * 2)
        {
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP1xR = period_temp;
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP2xR = period_temp / 2;
        }
        else
        {
            uint16 match_temp  = (uint16)(period_temp / 2 * ((float)duty / HRTIM_DUTY_MAX));
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP1xR = (period_temp / 2 - match_temp);
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].CMP2xR = (period_temp / 2 + match_temp);
        }

        hrtim_obj_list[hrtim_index].channel_state |= (0x0001 << channel_index);
        hrtim_obj_list[hrtim_index].pin_list[channel_index] = (zf_gpio_pin_enum)(pin & HRTIM_PWM_PIN_INDEX_MASK);
        zf_afio_init(
            (zf_gpio_pin_enum)(pin & HRTIM_PWM_PIN_INDEX_MASK),
            GPO_AF_PUSH_PULL,
            (zf_gpio_af_enum)((pin >> HRTIM_PWM_PIN_AF_OFFSET) & HRTIM_PWM_PIN_AF_MASK));

        hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].SETx1R |= 0x00000008;
        hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[channel_index % 6].RSTx1R |= 0x00000010;
        hrtim_obj_list[hrtim_index].hrtimer_obj->sCommonRegs.OENR |= (0x00000001 << ((channel_index % 6) * 2));

        return_state = HRTIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 模块初始化
// 参数说明     hrtim_index         选择 HRTIM 模块     (详见 zf_driver_hrtim.h 内 zf_hrtim_index_enum 定义)
// 参数说明     freq                设置频率 同个模块只有最后一次设置生效
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_module_init(hrtim_index, freq);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_module_init (zf_hrtim_index_enum hrtim_index, const uint32 freq)
{
    zf_hrtim_operation_state_enum return_state = HRTIM_ERROR_UNKNOW;

    do
    {
        if(zf_hrtim_assert(!hrtim_obj_list[hrtim_index].freq))
        {
            // 此处如果断言报错 那么证明本模块已经被初始化过一次 重复初始化是不允许的
            // 否则应当先注销初始化之后再重新初始化
            return_state = HRTIM_ERROR_MODULE_OCCUPIED;
            break;
        }
        if(zf_hrtim_assert(freq))
        {
            // 此处如果断言报错 那么证明输入的频率为 0
            return_state = HRTIM_ERROR_FREQ_ILLEGAL;
            break;
        }

        zf_rcc_clock_enable(hrtim_rcc_index_list[hrtim_index]);

        hrtim_obj_list[hrtim_index].hrtimer_obj->sCommonRegs.DLLCR = 0X00000002;

        hrtim_obj_list[hrtim_index].enable      = ZF_ENABLE;
        hrtim_obj_list[hrtim_index].self_index  = hrtim_index;

        uint32 register_temp    = 0;
        uint16 freq_div         = 0;
        uint16 period_temp      = 0;

        for(uint8 i = 1; i <= 128; i *= 2)
        {
            if(hrtim_obj_list[hrtim_index].clock_input / i / freq < 0xFF00)
            {
                period_temp = hrtim_obj_list[hrtim_index].clock_input / i / freq;
                period_temp = period_temp + period_temp % 2;
                hrtim_obj_list[hrtim_index].freq = hrtim_obj_list[hrtim_index].clock_input / i / period_temp;
                break;
            }
            freq_div += 1;
        }

        hrtim_obj_list[hrtim_index].hrtimer_obj->sMasterRegs.MPER = period_temp - 1;
        for(uint8 i = 0; i < 6; i ++)
        {
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[i].PERxR = period_temp - 1;
        }
        hrtim_obj_list[hrtim_index].period = period_temp;

        register_temp = freq_div | 0x00000008;
        for(uint8 i = 0; i < 6; i ++)
        {
            hrtim_obj_list[hrtim_index].hrtimer_obj->sTimerxRegs[i].TIMxCR = register_temp;
        }
        hrtim_obj_list[hrtim_index].hrtimer_obj->sMasterRegs.MCR = register_temp | 0x007F0000;

        return_state = HRTIM_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
