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
* 文件名称          zf_driver_pit
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
#include "zf_driver_system.h"

// 自身头文件
#include "zf_driver_pit.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 用于固定值或指向的内容 主要是用少量存储占用来减少栈开销
static const zf_interrupt_index_enum pit_irq_index_list[PIT_NUM_MAX] =
{
    INTERRUPT_INDEX_TIM1_PIT ,  INTERRUPT_INDEX_TIM8_PIT ,
    INTERRUPT_INDEX_TIM2_PIT ,  INTERRUPT_INDEX_TIM5_PIT ,
    INTERRUPT_INDEX_TIM3_PIT ,  INTERRUPT_INDEX_TIM4_PIT ,
    INTERRUPT_INDEX_TIM15_PIT,  INTERRUPT_INDEX_TIM16_PIT,
    INTERRUPT_INDEX_TIM6_PIT ,  INTERRUPT_INDEX_TIM7_PIT 
};

static void zf_pit_callbakc_defalut (uint32 event, void *ptr);

// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION zf_pit_obj_struct pit_obj_list[PIT_NUM_MAX] =
{
    {.timer_obj = &timer_obj_list[TIM_1 ], .period_system_cycles = 0},
    {.timer_obj = &timer_obj_list[TIM_8 ], .period_system_cycles = 0},
    {.timer_obj = &timer_obj_list[TIM_2 ], .period_system_cycles = 0},
    {.timer_obj = &timer_obj_list[TIM_5 ], .period_system_cycles = 0},
    {.timer_obj = &timer_obj_list[TIM_3 ], .period_system_cycles = 0},
    {.timer_obj = &timer_obj_list[TIM_4 ], .period_system_cycles = 0},
    {.timer_obj = &timer_obj_list[TIM_15], .period_system_cycles = 0},
    {.timer_obj = &timer_obj_list[TIM_16], .period_system_cycles = 0},
    {.timer_obj = &timer_obj_list[TIM_6 ], .period_system_cycles = 0},
    {.timer_obj = &timer_obj_list[TIM_7 ], .period_system_cycles = 0}
};

// 周期中断 回调函数第一个参数固定为 0 第二个参数是用户指针
AT_ZF_LIB_SECTION zf_interrupt_callback_struct timer_pit_callback[PIT_NUM_MAX] =
{
    {zf_pit_callbakc_defalut, NULL},   {zf_pit_callbakc_defalut, NULL},
    {zf_pit_callbakc_defalut, NULL},   {zf_pit_callbakc_defalut, NULL},
    {zf_pit_callbakc_defalut, NULL},   {zf_pit_callbakc_defalut, NULL},
    {zf_pit_callbakc_defalut, NULL},   {zf_pit_callbakc_defalut, NULL},
    {zf_pit_callbakc_defalut, NULL},   {zf_pit_callbakc_defalut, NULL}
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的外部重载函数 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM1  与 TIM16 的中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              pit_set_priority(pit_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_TIM1_UP_HANDLER (void)
{
    if(pit_obj_list[PIT_TIM1].period_system_cycles)
    {
        TIM1->SR &= ~TIM_SR_UIF;
        timer_pit_callback[PIT_TIM1].callback(0, timer_pit_callback[PIT_TIM1].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __tim_serve_interrupt(&DRV_TIM1);
        IRQ_EPILOGUE();
    }
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM8  的中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              pit_set_priority(pit_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_TIM8_UP_HANDLER (void)
{
    if(pit_obj_list[PIT_TIM8].period_system_cycles)
    {
        TIM8->SR &= ~TIM_SR_UIF;
        timer_pit_callback[PIT_TIM8].callback(0, timer_pit_callback[PIT_TIM8].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __tim_serve_interrupt(&DRV_TIM8);
        IRQ_EPILOGUE();
    }
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM2  的中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              pit_set_priority(pit_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_TIM2_HANDLER (void)
{
    if(pit_obj_list[PIT_TIM2].period_system_cycles)
    {
        TIM2->SR &= ~TIM_SR_UIF;
        timer_pit_callback[PIT_TIM2].callback(0, timer_pit_callback[PIT_TIM2].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __tim_serve_interrupt(&DRV_TIM2);
        IRQ_EPILOGUE();
    }
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM5  的中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              pit_set_priority(pit_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_TIM5_HANDLER (void)
{
    if(pit_obj_list[PIT_TIM5].period_system_cycles)
    {
        TIM5->SR &= ~TIM_SR_UIF;
        timer_pit_callback[PIT_TIM5].callback(0, timer_pit_callback[PIT_TIM5].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __tim_serve_interrupt(&DRV_TIM5);
        IRQ_EPILOGUE();
    }
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM3  的中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              pit_set_priority(pit_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_TIM3_HANDLER (void)
{
    if(pit_obj_list[PIT_TIM3].period_system_cycles)
    {
        TIM3->SR &= ~TIM_SR_UIF;
        timer_pit_callback[PIT_TIM3].callback(0, timer_pit_callback[PIT_TIM3].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __tim_serve_interrupt(&DRV_TIM3);
        IRQ_EPILOGUE();
    }
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM4  的中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              pit_set_priority(pit_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_TIM4_HANDLER (void)
{
    if(pit_obj_list[PIT_TIM4].period_system_cycles)
    {
        TIM4->SR &= ~TIM_SR_UIF;
        timer_pit_callback[PIT_TIM4].callback(0, timer_pit_callback[PIT_TIM4].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __tim_serve_interrupt(&DRV_TIM4);
        IRQ_EPILOGUE();
    }
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM15 的中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              pit_set_priority(pit_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_TIM15_HANDLER (void)
{
    if(pit_obj_list[PIT_TIM15].period_system_cycles)
    {
        TIM15->SR &= ~TIM_SR_UIF;
        timer_pit_callback[PIT_TIM15].callback(0, timer_pit_callback[PIT_TIM15].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __tim_serve_interrupt(&DRV_TIM15);
        IRQ_EPILOGUE();
    }
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM17 的中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              pit_set_priority(pit_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_TIM16_HANDLER (void)
{
    if(pit_obj_list[PIT_TIM16].period_system_cycles)
    {
        TIM16->SR &= ~TIM_SR_UIF;
        timer_pit_callback[PIT_TIM16].callback(0, timer_pit_callback[PIT_TIM16].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __tim_serve_interrupt(&DRV_TIM16);
        IRQ_EPILOGUE();
    }
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM6  的中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              pit_set_priority(pit_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_TIM6_HANDLER (void)
{
    if(pit_obj_list[PIT_TIM6].period_system_cycles)
    {
        TIM6->SR &= ~TIM_SR_UIF;
        timer_pit_callback[PIT_TIM6].callback(0, timer_pit_callback[PIT_TIM6].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __tim_serve_interrupt(&DRV_TIM6);
        IRQ_EPILOGUE();
    }
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM7  的中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              pit_set_priority(pit_index, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_TIM7_HANDLER (void)
{
    if(pit_obj_list[PIT_TIM7].period_system_cycles)
    {
        TIM7->SR &= ~TIM_SR_UIF;
        timer_pit_callback[PIT_TIM7].callback(0, timer_pit_callback[PIT_TIM7].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __tim_serve_interrupt(&DRV_TIM7);
        IRQ_EPILOGUE();
    }
    ZF_DSB();
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断默认回调函数
// 参数说明     event               触发中断的事件 此处默认为 0
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void zf_pit_callbakc_defalut (uint32 event, void *ptr)
{
    (void)event;
    (void)ptr;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断设置回调函数
// 参数说明     pit_index           PIT 外设模块号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 参数说明     callback            回调函数 为无返回值 uint32 ptr 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / PIT_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pit_set_interrupt_callback(pit_index, callback, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pit_set_interrupt_callback (zf_pit_index_enum pit_index, void_callback_uint32_ptr callback, void *ptr)
{
    zf_pit_operation_state_enum return_state = PIT_OPERATION_DONE;

    do
    {
        if(zf_pit_assert(NULL != callback))                                     // 检查 回调函数
        {
            // 此处如果断言报错 那么证明回调函数为空指针 是不允许的
            // 中断必须设置有效的回调函数 否则会导致程序异常
            return_state = PIT_ERROR_INTERRUPT_CALLBACK_ILLEGAL;
            break;
        }

        // 修改回调函数需要先禁止中断触发
        // 避免在修改过程中触发中断导致程序异常
        zf_pit_disable(pit_index);
        timer_pit_callback[pit_index].callback      = callback;
        timer_pit_callback[pit_index].parameter_ptr = ptr;
        zf_pit_enable(pit_index);

        return_state = PIT_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断设置优先级
// 参数说明     pit_index           PIT 外设模块号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 参数说明     priority            中断优先级
// 返回参数     void
// 使用示例     zf_pit_set_interrupt_priority(pit_index, priority);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_pit_set_interrupt_priority (zf_pit_index_enum pit_index, uint8 priority)
{
    zf_interrupt_set_priority(pit_irq_index_list[pit_index], priority);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断使能
// 参数说明     pit_index           PIT 外设模块号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / PIT_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pit_enable(pit_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pit_enable (zf_pit_index_enum pit_index)
{
    zf_pit_operation_state_enum return_state = PIT_OPERATION_DONE;

    do
    {
        if(zf_pit_assert(pit_obj_list[pit_index].period_system_cycles))         // 检查 初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = PIT_ERROR_MODULE_NOT_INIT;
            break;
        }

        zf_timer_clear((zf_timer_index_enum)pit_index);
        zf_timer_enable((zf_timer_index_enum)pit_index);

        return_state = PIT_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断禁止
// 参数说明     pit_index           PIT 外设模块号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / PIT_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pit_disable(pit_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pit_disable (zf_pit_index_enum pit_index)
{
    zf_pit_operation_state_enum return_state = PIT_OPERATION_DONE;

    do
    {
        if(zf_pit_assert(pit_obj_list[pit_index].period_system_cycles))         // 检查 初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = PIT_ERROR_MODULE_NOT_INIT;
            break;
        }

        zf_timer_disable((zf_timer_index_enum)pit_index);

        return_state = PIT_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 注销初始化
// 参数说明     pit_index           PIT 外设模块号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / PIT_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pit_deinit(pit_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pit_deinit (zf_pit_index_enum pit_index)
{
    zf_pit_operation_state_enum return_state = PIT_OPERATION_DONE;

    do
    {
        if(zf_pit_assert(pit_obj_list[pit_index].period_system_cycles))         // 检查 初始化
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = PIT_ERROR_MODULE_NOT_INIT;
            break;
        }

        pit_obj_list[pit_index].period_system_cycles = 0;
        zf_timer_deinit((zf_timer_index_enum)pit_index);

        return_state = PIT_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 初始化 一般调用 pit_ms_init 或 pit_us_init
// 参数说明     pit_index           PIT 外设模块号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 参数说明     period              PIT 周期 一般是芯片或者模块时钟频率计算
// 参数说明     callback            回调函数 为无返回值 uint32 ptr 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / PIT_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pit_init(pit_index, period, period, callback, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pit_init (zf_pit_index_enum pit_index, uint32 period, void_callback_uint32_ptr callback, void *ptr)
{
    zf_pit_operation_state_enum return_state = PIT_OPERATION_DONE;

    do
    {
        if(zf_pit_assert(!zf_timer_funciton_check(                              // 检查 模块是否占用
                (zf_timer_index_enum)pit_index,                                 // 当前模块
                TIMER_FUNCTION_PIT)))
        {
            // 此处如果断言报错 那么证明 _PIT 所依赖的 TIMER 已经被占用
            // _PIT 索引对应 TIMER 索引
            return_state = PIT_ERROR_DEPENDS_TIMER_OCCUPIED;
            break;
        }
        if(zf_pit_assert(!pit_obj_list[pit_index].period_system_cycles))        // 检查 初始化
        {
            // 此处如果断言报错 那么证明本通道已经初始化过了 是不允许覆盖初始化的
            // 如果要更改本通道的引脚 应该先调用注销初始化 之后再重新初始化新的通道
            return_state = PIT_ERROR_MODULE_OCCUPIED;
            break;
        }
        if(zf_pit_assert(period))                                               // 检查 周期设置
        {
            // 此处如果断言报错 那么证明设置的中断周期不合理
            return_state = PIT_ERROR_PERIOD_ILLEGAL;
            break;
        }

        zf_timer_clock_enable((zf_timer_index_enum)pit_index);                  // 使能时钟

        uint16 freq_div = (period >> 15);                                       // 计算预分频
        uint16 period_temp = (period / (freq_div + 1));                         // 计算自动重装载值

        pit_obj_list[pit_index].timer_obj->tim_ptr->PSC = freq_div;
        pit_obj_list[pit_index].timer_obj->tim_ptr->ARR = period_temp;

        pit_obj_list[pit_index].period_system_cycles = period;
        pit_obj_list[pit_index].timer_obj->div     = (freq_div + 1);
        pit_obj_list[pit_index].timer_obj->period  = 0xFFFF;
        pit_obj_list[pit_index].timer_obj->mode    = 3;

        pit_obj_list[pit_index].timer_obj->tim_ptr->DIER   = TIM_DIER_UIE;
        pit_obj_list[pit_index].timer_obj->tim_ptr->CNT    = 0;

        zf_interrupt_enable(pit_irq_index_list[pit_index]);

        return_state = zf_pit_set_interrupt_callback(pit_index, callback, ptr);

        return_state = PIT_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM PIT 中断初始化 ms 周期
// 参数说明     pit_index           使用的 PIT 编号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 参数说明     period_ms           PIT 周期 ms 级别
// 参数说明     callback            回调函数 为无返回值 uint32 ptr 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / PIT_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pit_ms_init(pit_index, period_ms, period, callback, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pit_ms_init (zf_pit_index_enum pit_index, uint32 period_ms, void_callback_uint32_ptr callback, void *ptr)
{
    return zf_pit_init(pit_index, (period_ms) * (pit_obj_list[pit_index].timer_obj->clock_input / 1000), callback, ptr);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM PIT 中断初始化 us 周期
// 参数说明     pit_index           使用的 PIT 编号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 参数说明     period_us           PIT 周期 us 级别
// 参数说明     callback            回调函数 为无返回值 uint32 ptr 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / PIT_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pit_us_init(pit_index, period_us, period, callback, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pit_us_init (zf_pit_index_enum pit_index, uint32 period_us, void_callback_uint32_ptr callback, void *ptr)
{
    return zf_pit_init(pit_index, (period_us) * (pit_obj_list[pit_index].timer_obj->clock_input / 1000000), callback, ptr);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
