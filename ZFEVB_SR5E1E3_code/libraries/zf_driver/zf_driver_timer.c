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
* 文件名称          zf_driver_timer
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
#include "zf_driver_rcc.h"
#include "zf_driver_system.h"

// 自身头文件
#include "zf_driver_timer.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 用于固定值或指向的内容 主要是用少量存储占用来减少栈开销
static const zf_rcc_index_enum timer_rcc_index_list[TIM_NUM_MAX] =
{
    RCC_INDEX_TIM1 , RCC_INDEX_TIM8 ,
    RCC_INDEX_TIM2 , RCC_INDEX_TIM5 ,
    RCC_INDEX_TIM3 , RCC_INDEX_TIM4 ,
    RCC_INDEX_TIM15, RCC_INDEX_TIM16,
    RCC_INDEX_TIM6 , RCC_INDEX_TIM7 ,
};

// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION zf_timer_obj_struct timer_obj_list[TIM_NUM_MAX] =
{
    {.tim_ptr = TIM1 , .clock_input = 300000000, .div = 0, .period = 0, .config_info = 0},
    {.tim_ptr = TIM8 , .clock_input = 300000000, .div = 0, .period = 0, .config_info = 0},
    {.tim_ptr = TIM2 , .clock_input = 150000000, .div = 0, .period = 0, .config_info = 0},
    {.tim_ptr = TIM5 , .clock_input = 300000000, .div = 0, .period = 0, .config_info = 0},
    {.tim_ptr = TIM3 , .clock_input = 150000000, .div = 0, .period = 0, .config_info = 0},
    {.tim_ptr = TIM4 , .clock_input = 300000000, .div = 0, .period = 0, .config_info = 0},
    {.tim_ptr = TIM15, .clock_input = 300000000, .div = 0, .period = 0, .config_info = 0},
    {.tim_ptr = TIM16, .clock_input = 300000000, .div = 0, .period = 0, .config_info = 0},
    {.tim_ptr = TIM6 , .clock_input = 150000000, .div = 0, .period = 0, .config_info = 0},
    {.tim_ptr = TIM7 , .clock_input = 150000000, .div = 0, .period = 0, .config_info = 0}
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_assert_handler(file, line, tips);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 外设确认功能状态 库内部调用
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 参数说明     mode                功能模块        (详见 zf_driver_timer.h 内 zf_timer_function_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_funciton_check(index, mode);
// 备注信息     本函数是提供给其他基于 TIM 模块实现的功能模块进行互斥控制的
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_funciton_check (zf_timer_index_enum timer_index, zf_timer_function_enum mode)
{
    zf_timer_operation_state_enum return_state = TIM_ERROR_UNKNOW;

    if(TIMER_FUNCTION_INIT == timer_obj_list[timer_index].function)
    {
        timer_obj_list[timer_index].function = mode;
        return_state = TIM_OPERATION_DONE;
    }
    else if(timer_obj_list[timer_index].function == mode)
    {
        return_state = TIM_OPERATION_DONE;
    }
    else
    {
        return_state = TIM_ERROR_MODULE_OCCUPIED;
    }

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 时钟使能 库内部调用
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 返回参数     void
// 使用示例     zf_timer_clock_enable(timer_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_timer_clock_enable (zf_timer_index_enum timer_index)
{
    zf_rcc_clock_enable(timer_rcc_index_list[timer_index]);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 开始计时
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_enable(timer_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_enable (zf_timer_index_enum timer_index)
{
    zf_timer_operation_state_enum return_state = TIM_ERROR_UNKNOW;

    do
    {
        if(zf_timer_assert(timer_obj_list[timer_index].period))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = TIM_ERROR_MODULE_NOT_INIT;
            break;
        }

        timer_obj_list[timer_index].tim_ptr->CR1  |=  TIM_CR1_CEN;

        return_state = TIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 停止计时
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_disable(timer_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_disable (zf_timer_index_enum timer_index)
{
    zf_timer_operation_state_enum return_state = TIM_ERROR_UNKNOW;

    do
    {
        if(zf_timer_assert(timer_obj_list[timer_index].period))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = TIM_ERROR_MODULE_NOT_INIT;
            break;
        }

        timer_obj_list[timer_index].tim_ptr->CR1  &= ~TIM_CR1_CEN;

        return_state = TIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 获取计时器计数
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint16              计数值
// 使用示例     zf_timer_get(timer_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_get (zf_timer_index_enum timer_index, uint16 *data)
{
    zf_timer_operation_state_enum return_state = TIM_ERROR_UNKNOW;

    do
    {
        if(zf_timer_assert(timer_obj_list[timer_index].period))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = TIM_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_timer_assert(NULL != data))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = TIM_ERROR_DATA_BUFFER_NULL;
            break;
        }

        uint16 return_value = timer_obj_list[timer_index].tim_ptr->CNT;
        if(TIMER_MS == timer_obj_list[timer_index].mode)
        {
            return_value = return_value / 10;
        }
        *data = return_value;

        return_state = TIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 清除计时器计数
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_clear(timer_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_clear (zf_timer_index_enum timer_index)
{
    zf_timer_operation_state_enum return_state = TIM_ERROR_UNKNOW;

    do
    {
        if(zf_timer_assert(timer_obj_list[timer_index].period))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = TIM_ERROR_MODULE_NOT_INIT;
            break;
        }

        timer_obj_list[timer_index].tim_ptr->CNT = 0;

        return_state = TIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 注销初始化
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_deinit(timer_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_deinit (zf_timer_index_enum timer_index)
{
    zf_timer_operation_state_enum return_state = TIM_ERROR_UNKNOW;

    do
    {
        if(zf_timer_assert(timer_obj_list[timer_index].period))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = TIM_ERROR_MODULE_NOT_INIT;
            break;
        }

        zf_rcc_peripheral_reset(timer_rcc_index_list[timer_index]);
        zf_rcc_clock_disable(timer_rcc_index_list[timer_index]);

        timer_obj_list[timer_index].config_info = 0;
        timer_obj_list[timer_index].div         = 0;
        timer_obj_list[timer_index].period      = 0;

        return_state = TIM_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 计时器初始化
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 参数说明     mode                计时方式        (详见 zf_driver_timer.h 内 zf_timer_mode_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_init(timer_index, mode);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_init (zf_timer_index_enum timer_index, zf_timer_mode_enum mode)
{
    zf_timer_operation_state_enum return_state = TIM_ERROR_UNKNOW;

    do
    {
        if(zf_timer_assert(!zf_timer_funciton_check(timer_index, TIMER_FUNCTION_TIMER)))
        {
            // 如果程序输出了断言信息 并且提示出错位置在这里
            // 就去查看你在什么地方调用这个函数 检查你的传入参数
            // 这里是检查是否有重复使用定时器
            // 比如初始化了 TIM1_PWM 然后又初始化成 TIM_1 这种用法是不允许的
            return_state = TIM_ERROR_MODULE_OCCUPIED;
            break;
        }
        zf_timer_clock_enable(timer_index);                                     // 使能时钟

        uint8 clock_div = (RCC_INDEX_APB1L_BUS_INDEX == (timer_rcc_index_list[timer_index] & RCC_BUS_INDEX_MASK)) ? (2) : (1);
        timer_obj_list[timer_index].clock_input = zf_system_clock / clock_div;
        switch(mode)
        {
            case TIMER_US:  timer_obj_list[timer_index].tim_ptr->PSC = timer_obj_list[timer_index].clock_input / 1000000 - 1;  break;
            case TIMER_MS:  timer_obj_list[timer_index].tim_ptr->PSC = timer_obj_list[timer_index].clock_input / 10000 - 1;    break;
            default:        timer_obj_list[timer_index].tim_ptr->PSC = 0;                           break;
        }
        timer_obj_list[timer_index].tim_ptr->ARR = 0xFFFF;

        timer_obj_list[timer_index].div     = (timer_obj_list[timer_index].tim_ptr->PSC + 1);
        timer_obj_list[timer_index].period  = 0xFFFF;
        timer_obj_list[timer_index].mode    = mode;

        zf_timer_disable(timer_index);
        zf_timer_clear(timer_index);

        return_state = TIM_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
