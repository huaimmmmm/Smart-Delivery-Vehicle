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
* 文件名称          zf_driver_encoder
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
#include "zf_driver_encoder.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION zf_encoder_obj_struct encoder_obj_list[ENCODER_NUM_MAX] = 
{
    {.timer_obj = &timer_obj_list[TIM_1 ], .a_plus_pin = PIN_NULL, .b_dir_pin = PIN_NULL, .mode = 0},
    {.timer_obj = &timer_obj_list[TIM_8 ], .a_plus_pin = PIN_NULL, .b_dir_pin = PIN_NULL, .mode = 0},
    {.timer_obj = &timer_obj_list[TIM_2 ], .a_plus_pin = PIN_NULL, .b_dir_pin = PIN_NULL, .mode = 0},
    {.timer_obj = &timer_obj_list[TIM_5 ], .a_plus_pin = PIN_NULL, .b_dir_pin = PIN_NULL, .mode = 0},
    {.timer_obj = &timer_obj_list[TIM_3 ], .a_plus_pin = PIN_NULL, .b_dir_pin = PIN_NULL, .mode = 0},
    {.timer_obj = &timer_obj_list[TIM_4 ], .a_plus_pin = PIN_NULL, .b_dir_pin = PIN_NULL, .mode = 0},
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ENCODER 接口获取编码器计数
// 参数说明     encoder_index       ENCODER 模块号  (详见 zf_driver_encoder.h 内 zf_encoder_index_enum 定义)
// 参数说明     *data               数据读取地址 int16 * 类型指针
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_encoder_get_count(encoder_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_encoder_get_count (zf_encoder_index_enum encoder_index, int16 *data)
{
    zf_encoder_operation_state_enum return_state = ENCODER_ERROR_UNKNOW;

    int16 return_value = 0;

    do
    {
        if(zf_encoder_assert(encoder_obj_list[encoder_index].mode))             // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = ENCODER_ERROR_MODULE_NOT_INIT;                       // ENCODER 模块未初始化 操作无法进行
            break;
        }
        if(zf_encoder_assert(NULL != data))                                     // 检查 数据存储地址
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = ENCODER_ERROR_DATA_BUFFER_NULL;                      // ENCODER 数据指针异常 操作无法进行
            break;
        }

        // 根据不同单片机来具体实现
        // 有的单片机需要根据 DIR 来判断正负 有的兼容方向编码器不需要自行判断
        // 数值需要统一成转一圈 1024 个数值 因为编码器是 1024 线的 转一圈就应该是 1024
        // 至于为什么单片机计数是 2048 或者 4096 那是因为有的是计边沿 这样就会数值翻倍
        if(ENCODER_MODE_QUADRATURE == encoder_obj_list[encoder_index].mode)
        {
            return_value = (int16)encoder_obj_list[encoder_index].timer_obj->tim_ptr->CNT / 4;
        }
        else
        {
            return_value = (int16)encoder_obj_list[encoder_index].timer_obj->tim_ptr->CNT;
            if(!zf_gpio_get_level(encoder_obj_list[encoder_index].b_dir_pin))
            {
                return_value = -return_value;
            }
        }
        *data = return_value;

        return_state = ZF_NO_ERROR;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ENCODER 清空编码器计数
// 参数说明     encoder_index       ENCODER 模块号  (详见 zf_driver_encoder.h 内 zf_encoder_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_encoder_clear_count(encoder_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_encoder_clear_count (zf_encoder_index_enum encoder_index)
{
    zf_encoder_operation_state_enum return_state = ENCODER_ERROR_UNKNOW;

    do
    {
        if(zf_encoder_assert(encoder_obj_list[encoder_index].mode))             // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = ENCODER_ERROR_MODULE_NOT_INIT;                       // ENCODER 模块未初始化 操作无法进行
            break;
        }

        encoder_obj_list[encoder_index].timer_obj->tim_ptr->CNT = 0;            // 清空计数器数值 没啥好说的

        return_state = ZF_NO_ERROR;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ENCODER 接口注销初始化
// 参数说明     encoder_index       ENCODER 模块号  (详见 zf_driver_encoder.h 内 zf_encoder_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_encoder_deinit(encoder_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_encoder_deinit (zf_encoder_index_enum encoder_index)
{
    zf_encoder_operation_state_enum return_state = ENCODER_ERROR_UNKNOW;

    do
    {
        if(zf_encoder_assert(encoder_obj_list[encoder_index].mode))             // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = ENCODER_ERROR_MODULE_NOT_INIT;                       // ENCODER 模块未初始化 操作无法进行
            break;
        }

        encoder_obj_list[encoder_index].mode        = 0;                        // 注销时需要先把模式清空 这样避免中断触发读取异常

        zf_gpio_deinit(encoder_obj_list[encoder_index].a_plus_pin);             // 接着把引脚注销
        zf_gpio_deinit(encoder_obj_list[encoder_index].b_dir_pin);              // 接着把引脚注销

        encoder_obj_list[encoder_index].a_plus_pin  = PIN_NULL;                 // 随后把引脚复位成空引脚占位
        encoder_obj_list[encoder_index].b_dir_pin   = PIN_NULL;                 // 随后把引脚复位成空引脚占位

        zf_timer_deinit((zf_timer_index_enum)encoder_index);                    // 最后把对应的定时器注销初始化

        return_state = ZF_NO_ERROR;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ENCODER 接口初始化
// 参数说明     encoder_index       ENCODER 模块号  (详见 zf_driver_encoder.h 内 zf_encoder_index_enum 定义)
// 参数说明     mode                ENCODER 模式    (详见 zf_driver_encoder.h 内 zf_encoder_mode_enum 定义)
// 参数说明     a_plus_pin          A 相或脉冲引脚  (详见 zf_driver_encoder.h 内 zf_encoder_channel_a_plus_enum 定义)
// 参数说明     b_dir_pin           B 相或方向引脚  (详见 zf_driver_encoder.h 内 zf_encoder_channel_b_dir_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_encoder_init(encoder_index, mode, a_plus_pin, b_dir_pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_encoder_init (zf_encoder_index_enum encoder_index, zf_encoder_mode_enum mode, zf_encoder_channel_a_plus_enum a_plus_pin, zf_encoder_channel_b_dir_enum b_dir_pin)
{
    zf_encoder_operation_state_enum return_state = ENCODER_ERROR_UNKNOW;

    do
    {
        if(zf_encoder_assert(!zf_timer_funciton_check(                          // 检查 模块是否占用
                (zf_timer_index_enum)encoder_index,                             // 当前模块
                TIMER_FUNCTION_ENCODER)))                                       // 编码器功能匹配
        {
            // 此处如果断言报错 那么证明 ENCODER 所依赖的 TIMER 已经被占用
            // ENCODER 索引对应 TIMER 索引
            return_state = ENCODER_ERROR_DEPENDS_TIMER_OCCUPIED;
            break;
        }
        if(zf_encoder_assert(!encoder_obj_list[encoder_index].mode))            // 检查 模块初始化
        {
            // 此处如果断言报错 那么证明本模块已经被初始化过一次 重复初始化是不允许的
            // 否则应当先注销初始化之后再重新初始化
            return_state = ENCODER_ERROR_MODULE_OCCUPIED;                       // ENCODER 模块已被占用 操作无法进行
            break;
        }
        if(zf_encoder_assert(encoder_index ==                                   // a_plus_pin 与 encoder_index 是否匹配
            ((a_plus_pin >> ENCODER_INDEX_OFFSET) & ENCODER_INDEX_MASK)))                                                     // 输出错误码 具体问题查看函数上方注释
        {
            // 此处如果断言报错 那么证明 ENCODER 编号跟 A 相或者 PLUS 引脚不匹配
            return_state = ENCODER_ERROR_A_PLUS_PIN_MISMATCH;                   // ENCODER 引脚不匹配 操作无法进行
            break;
        }
        if(zf_encoder_assert(encoder_index ==                                   // b_dir_pin 与 encoder_index 是否匹配
            ((b_dir_pin >> ENCODER_INDEX_OFFSET) & ENCODER_INDEX_MASK)))
        {
            // 此处如果断言报错 那么证明 ENCODER 编号跟 B 相或者 DIR 引脚不匹配
            return_state = ENCODER_ERROR_B_DIR_PIN_MISMATCH;                    // ENCODER 引脚不匹配 操作无法进行
            break;
        }

        // 先把数据都保存下来
        encoder_obj_list[encoder_index].a_plus_pin          = (zf_gpio_pin_enum)(a_plus_pin & ENCODER_PIN_INDEX_MASK);
        encoder_obj_list[encoder_index].b_dir_pin           = (zf_gpio_pin_enum)(b_dir_pin  & ENCODER_PIN_INDEX_MASK);
        encoder_obj_list[encoder_index].mode                = mode;
        encoder_obj_list[encoder_index].timer_obj->div      = 1;
        encoder_obj_list[encoder_index].timer_obj->period   = 0xFFFF;
        encoder_obj_list[encoder_index].timer_obj->mode     = 3;

        // 然后初始化对应的引脚 这里按照实际单片机来确定最终初始化的方案
        // 支持两种模式的单片机就都初始化成复用功能
        // 需要 GPIO 读取方向的就在方向模式下初始化成一个 GPIO 和一个复用功能
        zf_afio_init(
            encoder_obj_list[encoder_index].a_plus_pin,
            GPO_AF_OPEN_DRAIN,
            (zf_gpio_af_enum)((a_plus_pin >> ENCODER_PIN_AF_OFFSET) & ENCODER_PIN_AF_MASK));
        if(ENCODER_MODE_QUADRATURE == mode)
        {
            zf_afio_init(
                encoder_obj_list[encoder_index].b_dir_pin,
                GPO_AF_OPEN_DRAIN,
                (zf_gpio_af_enum)((b_dir_pin >> ENCODER_PIN_AF_OFFSET) & ENCODER_PIN_AF_MASK));
        }
        else
        {
            zf_gpio_init(
                encoder_obj_list[encoder_index].b_dir_pin,
                GPI_FLOATING_IN,
                GPIO_HIGH);
        }

        // 最后初始化对应编码器模块
        zf_timer_clock_enable((zf_timer_index_enum)encoder_index);              // 使能时钟

        {
            encoder_obj_list[encoder_index].timer_obj->tim_ptr->PSC      =  0;
            encoder_obj_list[encoder_index].timer_obj->tim_ptr->ARR      =  0xFFFF;
            encoder_obj_list[encoder_index].timer_obj->tim_ptr->CNT      =  0;
            encoder_obj_list[encoder_index].timer_obj->tim_ptr->SMCR     =  (ENCODER_MODE_QUADRATURE == mode) ? 0x00000003 : 0x00000057;

            uint32 register_temp = 0;
            register_temp = encoder_obj_list[encoder_index].timer_obj->tim_ptr->CCMR1;
            register_temp |= (TIM_CCMR1_CC1S_0);
            register_temp |= (TIM_CCMR1_CC2S_0);
            encoder_obj_list[encoder_index].timer_obj->tim_ptr->CCMR1 = register_temp;
            encoder_obj_list[encoder_index].timer_obj->tim_ptr->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);
        }

        zf_timer_enable((zf_timer_index_enum)encoder_index);

        return_state = ZF_NO_ERROR;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
