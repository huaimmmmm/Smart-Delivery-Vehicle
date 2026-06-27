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
* 文件名称          zf_driver_adc
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
#include "zf_driver_system.h"
#include "zf_driver_rcc.h"

// 自身头文件
#include "zf_driver_adc.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 用于固定值或指向的内容 主要是用少量存储占用来减少栈开销
static const zf_rcc_index_enum adc_rcc_index[ADC_NUM_MAX]  =
{
    RCC_INDEX_ADC12, RCC_INDEX_ADC12, RCC_INDEX_ADC345, RCC_INDEX_ADC345, RCC_INDEX_ADC345
};

#define     ADC_CHANNEL_PIN_DEFAULT     \
{   \
    PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,\
    PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,\
    PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL \
}

// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION zf_adc_obj_struct adc_obj_list[ADC_NUM_MAX] =                 // 通道使能状态
{
    {.adc_ptr = ADC1, .pin_list = ADC_CHANNEL_PIN_DEFAULT, .config_info = 0},
    {.adc_ptr = ADC2, .pin_list = ADC_CHANNEL_PIN_DEFAULT, .config_info = 0},
    {.adc_ptr = ADC3, .pin_list = ADC_CHANNEL_PIN_DEFAULT, .config_info = 0},
    {.adc_ptr = ADC4, .pin_list = ADC_CHANNEL_PIN_DEFAULT, .config_info = 0},
    {.adc_ptr = ADC5, .pin_list = ADC_CHANNEL_PIN_DEFAULT, .config_info = 0}
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 获取对应通道转换数据
// 参数说明     channel             选择 ADC 通道   (详见 zf_driver_adc.h 中枚举 zf_adc_channel_enum 定义)
// 参数说明     *data               ADC 转换存储地址指针 uint16 * 类型
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_get_convert_data(channel, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_get_convert_data (zf_adc_channel_enum channel, uint16 *data)
{
    zf_adc_operation_state_enum return_state = ADC_ERROR_UNKNOW;

    uint8   adc_index   = ((channel >> ADC_INDEX_OFFSET) & ADC_INDEX_MASK);
    uint8   adc_channel = ((channel >> ADC_CHANNEL_OFFSET) & ADC_CHANNEL_MASK);

    do
    {
        if(zf_adc_assert(adc_obj_list[adc_index].init))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = ADC_ERROR_MODULE_NOT_INIT;                           // ADC 模块未初始化 操作无法进行
            break;
        }
        if(zf_adc_assert(NULL != data))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = ADC_ERROR_DATA_BUFFER_NULL;                          // ADC 数据指针异常 操作无法进行
            break;
        }
        if(zf_adc_assert(adc_obj_list[adc_index].config_info
            &   (1U << adc_channel)))
        {
            // 通道未初始化 无法进行通道数据转换
            return_state = ADC_ERROR_CHANNEL_NOT_INIT;                          // ADC 通道未初始化 操作无法进行
            break;                                                              // 如果没有使能就退出去
        }

        if(!adc_obj_list[adc_index].enable)                                     // 模块是否使能
        {
            // 通道禁用 直接退出
            return_state = ADC_WARNING_MODULE_DISABLE;                          // ADC 模块失能禁用 操作中断退出
            break;                                                              // 如果没有使能就退出去
        }

        uint32 register_temp = adc_obj_list[adc_index].adc_ptr->SQR1;
        register_temp &= ~(0x0000001F << 6);
        register_temp |= (adc_channel << 6);
        adc_obj_list[adc_index].adc_ptr->SQR1 = register_temp;

        adc_obj_list[adc_index].adc_ptr->CR |= ADC_CR_ADSTART;
        while(ADC_ISR_EOC != (adc_obj_list[adc_index].adc_ptr->ISR & ADC_ISR_EOC));
        adc_obj_list[adc_index].adc_ptr->ISR |= ADC_ISR_EOC;
        *data = adc_obj_list[adc_index].adc_ptr->DR;

        return_state = ADC_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 获取对应通道转换数据的平均值
// 参数说明     channel             选择 ADC 通道   (详见 zf_driver_adc.h 中枚举 zf_adc_channel_enum 定义)
// 参数说明     count               均值滤波次数
// 参数说明     *data               ADC 转换存储地址指针 uint16 * 类型
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_get_convert_average_data(channel, count, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_get_convert_average_data (zf_adc_channel_enum channel, const uint8 count, uint16 *data)
{
    zf_adc_operation_state_enum return_state = ADC_ERROR_UNKNOW;

    uint8   adc_index       = ((channel >> ADC_INDEX_OFFSET) & ADC_INDEX_MASK);
    uint8   adc_channel     = ((channel >> ADC_CHANNEL_OFFSET) & ADC_CHANNEL_MASK);

    vuint8  loop_count      = 0;                                                // 均值滤波循环变量 volatile 修饰防止优化异常
    vuint32 adc_data_temp   = 0;                                                // 采样数据累加变量 volatile 修饰防止优化异常

    do
    {
        if(zf_adc_assert(adc_obj_list[adc_index].init))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = ADC_ERROR_MODULE_NOT_INIT;                           // ADC 模块未初始化 操作无法进行
            break;
        }
        if(zf_adc_assert(NULL != data))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = ADC_ERROR_DATA_BUFFER_NULL;                          // ENCODER 数据指针异常 操作无法进行
            break;
        }
        if(zf_adc_assert(adc_obj_list[adc_index].config_info
            &   (1U << adc_channel)))
        {
            // 通道未初始化 无法进行通道数据转换
            return_state = ADC_ERROR_CHANNEL_NOT_INIT;                          // ADC 通道未初始化 操作无法进行
            break;                                                              // 如果没有使能就退出去
        }

        if(!adc_obj_list[adc_index].enable)                                     // 模块是否使能
        {
            // 通道禁用 直接退出
            return_state = ADC_WARNING_MODULE_DISABLE;                          // ADC 模块失能禁用 操作中断退出
            break;                                                              // 如果没有使能就退出去
        }

        for(loop_count = 0; loop_count < count; loop_count ++)
        {
            uint32 register_temp = adc_obj_list[adc_index].adc_ptr->SQR1;
            register_temp &= ~(0x0000001F << 6);
            register_temp |= (adc_channel << 6);
            adc_obj_list[adc_index].adc_ptr->SQR1 = register_temp;

            adc_obj_list[adc_index].adc_ptr->CR |= ADC_CR_ADSTART;
            while(ADC_ISR_EOC != (adc_obj_list[adc_index].adc_ptr->ISR & ADC_ISR_EOC));
            adc_obj_list[adc_index].adc_ptr->ISR |= ADC_ISR_EOC;
            adc_data_temp += adc_obj_list[adc_index].adc_ptr->DR;               // 累加采样数据
        }
        *data = (uint16)(adc_data_temp / count);                                // 求均值

        return_state = ADC_OPERATION_DONE;
    }while(0);

    return return_state;                                                        // 输出均值数据
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 通道使能
// 参数说明     adc_index           选择 ADC 模块   (详见 zf_driver_adc.h 中枚举 zf_adc_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_enable(adc_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_enable (zf_adc_index_enum adc_index)
{
    zf_adc_operation_state_enum return_state = ADC_ERROR_UNKNOW;

    do
    {
        if(zf_adc_assert(adc_obj_list[adc_index].init))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = ADC_ERROR_MODULE_NOT_INIT;                           // ADC 模块未初始化 操作无法进行
            break;
        }

        adc_obj_list[adc_index].enable = ZF_ENABLE;

        return_state = ADC_OPERATION_DONE;
    }while(0);

    return return_state;                                                        // 返回状态
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 通道禁止
// 参数说明     adc_index           选择 ADC 模块   (详见 zf_driver_adc.h 中枚举 zf_adc_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_disable(adc_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_disable (zf_adc_index_enum adc_index)
{
    zf_adc_operation_state_enum return_state = ADC_ERROR_UNKNOW;

    do
    {
        if(zf_adc_assert(adc_obj_list[adc_index].init))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = ADC_ERROR_MODULE_NOT_INIT;                           // ADC 模块未初始化 操作无法进行
            break;
        }

        adc_obj_list[adc_index].enable = ZF_DISABLE;

        return_state = ADC_OPERATION_DONE;
    }while(0);

    return return_state;                                                        // 返回状态
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 精度设置
// 参数说明     adc_index           选择 ADC 模块   (详见 zf_driver_adc.h 中枚举 zf_adc_index_enum 定义)
// 参数说明     resolution          选择分辨率      (详见 zf_driver_adc.h 中枚举 zf_adc_resolution_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_set_resolution(adc_index, resolution);
// 备注信息     本函数一定会设置生效 即使没有通道初始化也能设置 会在首个通道初始化时生效
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_set_resolution (zf_adc_index_enum adc_index, zf_adc_resolution_enum resolution)
{
    zf_adc_operation_state_enum return_state = ADC_ERROR_UNKNOW;

    do
    {
        if(zf_adc_assert(adc_obj_list[adc_index].init))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = ADC_ERROR_MODULE_NOT_INIT;                           // ADC 模块未初始化 操作无法进行
            break;
        }

        uint32 adc_cr_temp      = adc_obj_list[adc_index].adc_ptr->CR;
        uint32 adc_cfgr_temp    = adc_obj_list[adc_index].adc_ptr->CFGR;

        adc_obj_list[adc_index].adc_ptr->CR &= ~(0x0000000D);
        adc_cfgr_temp &= ~(0x00000018);
        adc_cfgr_temp |= ((uint32)resolution << 3);
        adc_obj_list[adc_index].adc_ptr->CFGR = adc_cfgr_temp;
        adc_obj_list[adc_index].adc_ptr->CR = adc_cr_temp;

        return_state = ADC_OPERATION_DONE;
    }while(0);

    return return_state;                                                        // 返回状态
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 通道注销初始化
// 参数说明     channel             选择 ADC 通道   (详见 zf_driver_adc.h 中枚举 zf_adc_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_channel_deinit(channel);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_channel_deinit (zf_adc_channel_enum channel)
{
    zf_adc_operation_state_enum return_state = ADC_ERROR_UNKNOW;

    uint8   adc_index       = ((channel >> ADC_INDEX_OFFSET) & ADC_INDEX_MASK);
    uint8   adc_channel     = ((channel >> ADC_CHANNEL_OFFSET) & ADC_CHANNEL_MASK);

    do
    {
        if(zf_adc_assert(adc_obj_list[adc_index].init))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = ADC_ERROR_MODULE_NOT_INIT;                           // ADC 模块未初始化 操作无法进行
            break;
        }
        if(zf_adc_assert(
            adc_obj_list[adc_index].config_info & (1U << adc_channel)))         // 检查 通道初始化
        {
            // 此处如果断言报错 那么证明本通道未被初始化 没初始化怎么注销
            return_state = ADC_ERROR_CHANNEL_NOT_INIT;                          // ADC 通道未初始化 操作无法进行
            break;
        }

        adc_obj_list[adc_index].config_info &= ~(1U << adc_channel);            // 记录对应模块对应通道的占用信息
        if(PIN_NULL != adc_obj_list[adc_index].pin_list[adc_channel])
        {
            zf_gpio_deinit(adc_obj_list[adc_index].pin_list[adc_channel]);
            adc_obj_list[adc_index].pin_list[adc_channel] = PIN_NULL;
        }

        return_state = ADC_OPERATION_DONE;
    }while(0);

    return return_state;                                                        // 返回状态
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 模块注销初始化
// 参数说明     adc_index           选择 ADC 模块   (详见 zf_driver_adc.h 中枚举 zf_adc_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_module_deinit(adc_index);
// 备注信息     模块注销初始化会连带注销当前模块下所有通道
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_module_deinit (zf_adc_index_enum adc_index)
{
    zf_adc_operation_state_enum return_state = ADC_ERROR_UNKNOW;

    do
    {
        if(zf_adc_assert(adc_obj_list[adc_index].init))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = ADC_ERROR_MODULE_NOT_INIT;                           // ADC 模块未初始化 操作无法进行
            break;
        }

        for(uint8 i = 0; 18 > i; i ++)
        {
            if(     (adc_obj_list[adc_index].config_info & (1U << i))           // 判断通道是否初始化了
                &&  (PIN_NULL != adc_obj_list[adc_index].pin_list[i]))          // 在判断通道是否有有效引脚
            {
                zf_gpio_deinit(adc_obj_list[adc_index].pin_list[i]);            // 注销对应 IO 的初始化
            }
            adc_obj_list[adc_index].pin_list[i] = PIN_NULL;
        }
        adc_obj_list[adc_index].config_info = 0;

        if(!adc_obj_list[0].config_info && !adc_obj_list[1].config_info)
        {
            zf_rcc_peripheral_reset(adc_rcc_index[0]);                          // 如果是的话就复位当前模块
            zf_rcc_clock_disable(adc_rcc_index[0]);                             // 并且关闭当前模块时钟
        }
        if(!adc_obj_list[2].config_info && !adc_obj_list[3].config_info && !adc_obj_list[4].config_info)
        {
            zf_rcc_peripheral_reset(adc_rcc_index[2]);                          // 如果是的话就复位当前模块
            zf_rcc_clock_disable(adc_rcc_index[2]);                             // 并且关闭当前模块时钟
        }

        return_state = ADC_OPERATION_DONE;
    }while(0);

    return return_state;                                                        // 返回状态
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 通道初始化
// 参数说明     channel             选择 ADC 通道   (详见 zf_driver_adc.h 中枚举 zf_adc_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_channel_init(channel);
// 备注信息     如果通道已经有一个通道初始化了 再初始化同一个通道的通道就会失败
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_channel_init (zf_adc_channel_enum channel)
{
    zf_adc_operation_state_enum return_state = ADC_ERROR_UNKNOW;

    uint8   adc_index       = ((channel >> ADC_INDEX_OFFSET) & ADC_INDEX_MASK);
    uint8   adc_channel     = ((channel >> ADC_CHANNEL_OFFSET) & ADC_CHANNEL_MASK);

    do
    {
        if(zf_adc_assert(adc_obj_list[adc_index].init))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = ADC_ERROR_MODULE_NOT_INIT;                           // ADC 模块未初始化 操作无法进行
            break;
        }
        if(zf_adc_assert(
            !(adc_obj_list[adc_index].config_info & (1U << adc_channel))))      // 检查 通道初始化
        {
            // 此处如果断言报错 那么证明这个通道已经被初始化过了 不能直接覆盖初始化
            // 应当先注销初始化之后再重新初始化
            return_state = ADC_ERROR_CHANNEL_OCCUPIED;                          // ADC 通道已被占用 操作无法进行
            break;
        }

        adc_obj_list[adc_index].config_info |= (1U << adc_channel);             // 记录对应模块对应通道的占用信息
        adc_obj_list[adc_index].pin_list[adc_channel] = (zf_gpio_pin_enum)(channel & ADC_PIN_INDEX_MASK);
        if(PIN_NULL != adc_obj_list[adc_index].pin_list[adc_channel])
        {
            zf_gpio_init((zf_gpio_pin_enum)(channel & ADC_PIN_INDEX_MASK), GPI_ANAOG_IN, GPIO_HIGH);
        }

        return_state = ADC_OPERATION_DONE;
    }while(0);

    return return_state;                                                        // 返回状态
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 模块初始化
// 参数说明     adc_index           选择 ADC 模块   (详见 zf_driver_adc.h 中枚举 zf_adc_index_enum 定义)
// 参数说明     resolution          选择分辨率      (详见 zf_driver_adc.h 中枚举 zf_adc_resolution_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_module_init(adc_index, resolution);
// 备注信息     本函数一定会设置生效 即使没有通道初始化也能设置 会在首个通道初始化时生效
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_module_init (zf_adc_index_enum adc_index, zf_adc_resolution_enum resolution)
{
    zf_adc_operation_state_enum return_state = ADC_ERROR_UNKNOW;

    do
    {
        if(zf_adc_assert(!adc_obj_list[adc_index].init))
        {
            // 此处如果断言报错 那么证明本模块已经被初始化过一次 重复初始化是不允许的
            // 如果需要修改精度可以调用设置精度接口
            // 否则应当先注销初始化之后再重新初始化
            return_state = ADC_ERROR_MODULE_OCCUPIED;                           // ADC 模块已被占用 操作无法进行
            break;
        }

        zf_rcc_clock_enable(adc_rcc_index[adc_index]);
        adc_obj_list[adc_index].init = ZF_ENABLE;
        adc_obj_list[adc_index].resolution = resolution;

        {
            SYSCFG->ADC_BIASEN = SYSCFG_ADC_BIASEN_GLOBAL_BIAS_EN;              // 它需要 SYSCFG 启用基准时钟

            adc_obj_list[adc_index].adc_ptr->CR = ADC_CR_ADVREGEN | ADC_CR_ADCALDIF;
            adc_obj_list[adc_index].adc_ptr->CR = ADC_CR_ADVREGEN | ADC_CR_ADCALDIF | ADC_CR_ADCAL;
            while ((adc_obj_list[adc_index].adc_ptr->CR & ADC_CR_ADCAL) != 0U);

            adc_obj_list[adc_index].adc_ptr->CR  = ADC_CR_ADVREGEN | ADC_CR_ADEN;
            while ((adc_obj_list[adc_index].adc_ptr->ISR & ADC_ISR_ADRDY) == 0U);

            uint32 adc_cfgr_temp    = adc_obj_list[adc_index].adc_ptr->CFGR;

            adc_cfgr_temp &= ~ADC_CFGR_RES;
            adc_cfgr_temp |= ((uint32)resolution << 3);
            adc_obj_list[adc_index].adc_ptr->CFGR = adc_cfgr_temp;

            adc_obj_list[adc_index].adc_ptr->CR |= ADC_CR_ADEN;
        }

        adc_obj_list[adc_index].self_index = adc_index;
        adc_obj_list[adc_index].enable = ZF_ENABLE;

        return_state = ADC_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
