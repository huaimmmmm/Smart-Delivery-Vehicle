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
* 文件名称          zf_driver_exti
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
#include "zf_driver_system.h"

// 自身头文件
#include "zf_driver_exti.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 用于固定值或指向的内容 主要是用少量存储占用来减少栈开销
static const zf_interrupt_index_enum exti_irq_index_list[EXTI_MODULE_NUM_MAX][EXTI_CHANNEL_NUM_MAX] =
{
    {
        INTERRUPT_INDEX_EXTI0       ,   INTERRUPT_INDEX_EXTI1       ,   INTERRUPT_INDEX_EXTI2       ,   INTERRUPT_INDEX_EXTI3       ,
        INTERRUPT_INDEX_EXTI4       ,   INTERRUPT_INDEX_EXTI9_5     ,   INTERRUPT_INDEX_EXTI9_5     ,   INTERRUPT_INDEX_EXTI9_5     ,
        INTERRUPT_INDEX_EXTI9_5     ,   INTERRUPT_INDEX_EXTI9_5     ,   INTERRUPT_INDEX_EXTI15_10   ,   INTERRUPT_INDEX_EXTI15_10   ,
        INTERRUPT_INDEX_EXTI15_10   ,   INTERRUPT_INDEX_EXTI15_10   ,   INTERRUPT_INDEX_EXTI15_10   ,   INTERRUPT_INDEX_EXTI15_10
    }
};

static void zf_exti_callbakc_defalut (uint32 event, void *ptr);

// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION_START
// 外部中断 回调函数第一个参数固定为 0 第二个参数是用户指针
AT_ZF_LIB_SECTION zf_interrupt_callback_struct exti_callback[EXTI_MODULE_NUM_MAX][EXTI_CHANNEL_NUM_MAX] = 
{
    {
        {zf_exti_callbakc_defalut, NULL},   {zf_exti_callbakc_defalut, NULL},
        {zf_exti_callbakc_defalut, NULL},   {zf_exti_callbakc_defalut, NULL},
        {zf_exti_callbakc_defalut, NULL},   {zf_exti_callbakc_defalut, NULL},
        {zf_exti_callbakc_defalut, NULL},   {zf_exti_callbakc_defalut, NULL},
        {zf_exti_callbakc_defalut, NULL},   {zf_exti_callbakc_defalut, NULL},
        {zf_exti_callbakc_defalut, NULL},   {zf_exti_callbakc_defalut, NULL},
        {zf_exti_callbakc_defalut, NULL},   {zf_exti_callbakc_defalut, NULL},
        {zf_exti_callbakc_defalut, NULL},   {zf_exti_callbakc_defalut, NULL}
    }
};

AT_ZF_LIB_SECTION zf_exti_obj_struct exti_obj_list[EXTI_MODULE_NUM_MAX] =
{
    {
        .exti_ptr = EXTI,
        .pin_list =
        {
            PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,
            PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,
            PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL,
            PIN_NULL,   PIN_NULL,   PIN_NULL,   PIN_NULL
        },
        .config_info = 0
    },
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的外部重载函数 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断默认回调函数
// 参数说明     event               触发中断的事件 此处默认为 0
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void zf_exti_callbakc_defalut (uint32 event, void *ptr)
{
    (void)event;
    (void)ptr;
}

#define EXTI_CALC_INDEX     ( (TEMP_EXTI_INDEX >> EXTI_INDEX_OFFSET     ) & EXTI_INDEX_MASK     )
#define EXTI_CALC_CHANNEL   ( (TEMP_EXTI_INDEX >> EXTI_CHANNEL_OFFSET   ) & EXTI_CHANNEL_MASK   )

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI0 线 的串口中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              zf_exti_set_interrupt_priority(pin, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_EXTI0_HANDLER (void)
{
    #define TEMP_EXTI_INDEX     ( EXTI1_CH0_A0 )
    exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
    if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
    {
        uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
        switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
        {
            case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
            case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
            case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
        }
        exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __exti_serve_interrupt(EXTI_IRQ_LINE0);
        IRQ_EPILOGUE();
    }
    #undef  TEMP_EXTI_INDEX
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI1 线 的串口中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              zf_exti_set_interrupt_priority(pin, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_EXTI1_HANDLER (void)
{
    #define TEMP_EXTI_INDEX     ( EXTI1_CH1_A1 )
    exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
    if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
    {
        uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
        switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
        {
            case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
            case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
            case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
        }
        exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __exti_serve_interrupt(EXTI_IRQ_LINE1);
        IRQ_EPILOGUE();
    }
    #undef  TEMP_EXTI_INDEX
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI2 线 的串口中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              zf_exti_set_interrupt_priority(pin, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_EXTI2_HANDLER (void)
{
    #define TEMP_EXTI_INDEX     ( EXTI1_CH2_A2 )
    exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
    if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
    {
        uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
        switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
        {
            case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
            case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
            case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
        }
        exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __exti_serve_interrupt(EXTI_IRQ_LINE2);
        IRQ_EPILOGUE();
    }
    #undef  TEMP_EXTI_INDEX
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI3 线 的串口中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              zf_exti_set_interrupt_priority(pin, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_EXTI3_HANDLER (void)
{
    #define TEMP_EXTI_INDEX     ( EXTI1_CH3_A3 )
    exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
    if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
    {
        uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
        switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
        {
            case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
            case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
            case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
        }
        exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __exti_serve_interrupt(EXTI_IRQ_LINE3);
        IRQ_EPILOGUE();
    }
    #undef  TEMP_EXTI_INDEX
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI4 线 的串口中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              zf_exti_set_interrupt_priority(pin, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_EXTI4_HANDLER (void)
{
    #define TEMP_EXTI_INDEX     ( EXTI1_CH4_A4 )
    exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
    if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
    {
        uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
        switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
        {
            case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
            case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
            case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
        }
        exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
    }
    else
    {
        IRQ_PROLOGUE();
        __exti_serve_interrupt(EXTI_IRQ_LINE4);
        IRQ_EPILOGUE();
    }
    #undef  TEMP_EXTI_INDEX
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI5-9 线 的串口中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              zf_exti_set_interrupt_priority(pin, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_EXTI9_5_HANDLER (void)
{
    if(EXTI->C1PR1 & (0x00000001 << 5))
    {
        #define TEMP_EXTI_INDEX     ( EXTI1_CH5_A5 )
        exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
        if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
        {
            uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
            switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
            {
                case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
                case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
                case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
            }
            exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
        }
        else
        {
            IRQ_PROLOGUE();
            __exti_serve_interrupt(EXTI_IRQ_LINE9_5);
            IRQ_EPILOGUE();
        }
        #undef  TEMP_EXTI_INDEX
        ZF_DSB();
    }
    if(EXTI->C1PR1 & (0x00000001 << 6))
    {
        #define TEMP_EXTI_INDEX     ( EXTI1_CH6_A6 )
        exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
        if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
        {
            uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
            switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
            {
                case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
                case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
                case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
            }
            exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
        }
        else
        {
            IRQ_PROLOGUE();
            __exti_serve_interrupt(EXTI_IRQ_LINE9_5);
            IRQ_EPILOGUE();
        }
        #undef  TEMP_EXTI_INDEX
        ZF_DSB();
    }
    if(EXTI->C1PR1 & (0x00000001 << 7))
    {
        #define TEMP_EXTI_INDEX     ( EXTI1_CH7_A7 )
        exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
        if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
        {
            uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
            switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
            {
                case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
                case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
                case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
            }
            exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
        }
        else
        {
            IRQ_PROLOGUE();
            __exti_serve_interrupt(EXTI_IRQ_LINE9_5);
            IRQ_EPILOGUE();
        }
        #undef  TEMP_EXTI_INDEX
        ZF_DSB();
    }
    if(EXTI->C1PR1 & (0x00000001 << 8))
    {
        #define TEMP_EXTI_INDEX     ( EXTI1_CH8_A8 )
        exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
        if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
        {
            uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
            switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
            {
                case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
                case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
                case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
            }
            exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
        }
        else
        {
            IRQ_PROLOGUE();
            __exti_serve_interrupt(EXTI_IRQ_LINE9_5);
            IRQ_EPILOGUE();
        }
        #undef  TEMP_EXTI_INDEX
        ZF_DSB();
    }
    if(EXTI->C1PR1 & (0x00000001 << 9))
    {
        #define TEMP_EXTI_INDEX     ( EXTI1_CH9_A9 )
        exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
        if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
        {
            uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
            switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
            {
                case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
                case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
                case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
            }
            exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
        }
        else
        {
            IRQ_PROLOGUE();
            __exti_serve_interrupt(EXTI_IRQ_LINE9_5);
            IRQ_EPILOGUE();
        }
        #undef  TEMP_EXTI_INDEX
        ZF_DSB();
    }
    ZF_DSB();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI10-15 线 的串口中断服务函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     启动 .s 文件定义 不允许修改函数名称
//              zf_exti_set_interrupt_priority(pin, priority);
//-------------------------------------------------------------------------------------------------------------------
void IRQ_EXTI15_10_HANDLER (void)
{
    if(EXTI->C1PR1 & (0x00000001 << 10))
    {
        #define TEMP_EXTI_INDEX     ( EXTI1_CH10_A10 )
        exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
        if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
        {
            uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
            switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
            {
                case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
                case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
                case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
            }
            exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
        }
        else
        {
            IRQ_PROLOGUE();
            __exti_serve_interrupt(EXTI_IRQ_LINE15_10);
            IRQ_EPILOGUE();
        }
        #undef  TEMP_EXTI_INDEX
        ZF_DSB();
    }
    if(EXTI->C1PR1 & (0x00000001 << 11))
    {
        #define TEMP_EXTI_INDEX     ( EXTI1_CH11_A11 )
        exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
        if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
        {
            uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
            switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
            {
                case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
                case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
                case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
            }
            exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
        }
        else
        {
            IRQ_PROLOGUE();
            __exti_serve_interrupt(EXTI_IRQ_LINE15_10);
            IRQ_EPILOGUE();
        }
        #undef  TEMP_EXTI_INDEX
        ZF_DSB();
    }
    if(EXTI->C1PR1 & (0x00000001 << 12))
    {
        #define TEMP_EXTI_INDEX     ( EXTI1_CH12_A12 )
        exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
        if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
        {
            uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
            switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
            {
                case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
                case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
                case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
            }
            exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
        }
        else
        {
            IRQ_PROLOGUE();
            __exti_serve_interrupt(EXTI_IRQ_LINE15_10);
            IRQ_EPILOGUE();
        }
        #undef  TEMP_EXTI_INDEX
        ZF_DSB();
    }
    if(EXTI->C1PR1 & (0x00000001 << 13))
    {
        #define TEMP_EXTI_INDEX     ( EXTI1_CH13_A13 )
        exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
        if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
        {
            uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
            switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
            {
                case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
                case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
                case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
            }
            exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
        }
        else
        {
            IRQ_PROLOGUE();
            __exti_serve_interrupt(EXTI_IRQ_LINE15_10);
            IRQ_EPILOGUE();
        }
        #undef  TEMP_EXTI_INDEX
        ZF_DSB();
    }
    if(EXTI->C1PR1 & (0x00000001 << 14))
    {
        #define TEMP_EXTI_INDEX     ( EXTI1_CH14_A14 )
        exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
        if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
        {
            uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
            switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
            {
                case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
                case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
                case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
            }
            exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
        }
        else
        {
            IRQ_PROLOGUE();
            __exti_serve_interrupt(EXTI_IRQ_LINE15_10);
            IRQ_EPILOGUE();
        }
        #undef  TEMP_EXTI_INDEX
        ZF_DSB();
    }
    if(EXTI->C1PR1 & (0x00000001 << 15))
    {
        #define TEMP_EXTI_INDEX     ( EXTI1_CH15_A15 )
        exti_obj_list[EXTI_CALC_INDEX].exti_ptr->C1PR1 |= (0x00000001 << EXTI_CALC_CHANNEL);
        if(PIN_NULL != exti_obj_list[EXTI_CALC_INDEX].pin_list[EXTI_CALC_CHANNEL])
        {
            uint32 event_temp = EXTI_INTERRUPT_STATE_NONE;
            switch((exti_obj_list[EXTI_CALC_INDEX].config_info >> (EXTI_CALC_CHANNEL * 2)) & 0x03)
            {
                case EXTI_TRIGGER_TYPE_RISING   :   event_temp = EXTI_INTERRUPT_STATE_RISING    ;   break;
                case EXTI_TRIGGER_TYPE_FALLING  :   event_temp = EXTI_INTERRUPT_STATE_FALLING   ;   break;
                case EXTI_TRIGGER_TYPE_BOTH     :   event_temp = EXTI_INTERRUPT_STATE_BOTH      ;   break;
            }
            exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].callback(event_temp, exti_callback[EXTI_CALC_INDEX][EXTI_CALC_CHANNEL].parameter_ptr);
        }
        else
        {
            IRQ_PROLOGUE();
            __exti_serve_interrupt(EXTI_IRQ_LINE15_10);
            IRQ_EPILOGUE();
        }
        #undef  TEMP_EXTI_INDEX
        ZF_DSB();
    }
    ZF_DSB();
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断使能
// 参数说明     channel             选择 EXTI 引脚  (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / EXTI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_enable(channel);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_enable (zf_exti_channel_pin_enum channel)
{
    zf_exti_operation_state_enum return_state = EXTI_ERROR_UNKNOW;

    uint8   exti_index      = ((channel >> EXTI_INDEX_OFFSET) & EXTI_INDEX_MASK);
    uint8   exti_channel    = ((channel >> EXTI_CHANNEL_OFFSET) & EXTI_CHANNEL_MASK);

    do
    {
        zf_gpio_pin_enum pin = (zf_gpio_pin_enum)(channel & EXTI_PIN_INDEX_MASK);

        if(zf_exti_assert(PIN_NULL !=                                           // 检查 对应通道是否初始化
            exti_obj_list[exti_index].pin_list[exti_channel]))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = EXTI_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_exti_assert(pin ==                                                // 检查 对应通道是否初始化
            exti_obj_list[exti_index].pin_list[exti_channel]))
        {
            // 此处如果断言报错 那么证明本通道原本初始化引脚与注销的引脚不匹配
            // 初始化用什么引脚 注销就用什么引脚 不要随便乱填
            return_state = EXTI_ERROR_PIN_MISMATCH;
            break;
        }

        exti_obj_list[exti_index].exti_ptr->C1IMR1 |= (0x00000001 << (pin & GPIO_PIN_INDEX_MASK));

        return_state = EXTI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断失能
// 参数说明     channel             选择 EXTI 引脚  (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / EXTI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_disable(channel);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_disable (zf_exti_channel_pin_enum channel)
{
    zf_exti_operation_state_enum return_state = EXTI_ERROR_UNKNOW;

    uint8   exti_index      = ((channel >> EXTI_INDEX_OFFSET) & EXTI_INDEX_MASK);
    uint8   exti_channel    = ((channel >> EXTI_CHANNEL_OFFSET) & EXTI_CHANNEL_MASK);

    do
    {
        zf_gpio_pin_enum pin = (zf_gpio_pin_enum)(channel & EXTI_PIN_INDEX_MASK);

        if(zf_exti_assert(PIN_NULL !=                                           // 检查 对应通道是否初始化
            exti_obj_list[exti_index].pin_list[exti_channel]))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = EXTI_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_exti_assert(pin ==                                                // 检查 对应通道是否初始化
            exti_obj_list[exti_index].pin_list[exti_channel]))
        {
            // 此处如果断言报错 那么证明本通道原本初始化引脚与注销的引脚不匹配
            // 初始化用什么引脚 注销就用什么引脚 不要随便乱填
            return_state = EXTI_ERROR_PIN_MISMATCH;
            break;
        }

        exti_obj_list[exti_index].exti_ptr->C1IMR1 &= ~(0x00000001 << (pin & GPIO_PIN_INDEX_MASK));

        return_state = EXTI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断设置回调函数
// 参数说明     channel             选择 EXTI 引脚  (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 参数说明     callback            回调函数 为无返回值 uint32 void* 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / EXTI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_set_callback(channel, callback, NULL);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_set_callback (zf_exti_channel_pin_enum channel, void_callback_uint32_ptr callback, void *ptr)
{
    zf_exti_operation_state_enum return_state = EXTI_ERROR_UNKNOW;

    uint8   exti_index      = ((channel >> EXTI_INDEX_OFFSET) & EXTI_INDEX_MASK);
    uint8   exti_channel    = ((channel >> EXTI_CHANNEL_OFFSET) & EXTI_CHANNEL_MASK);

    do
    {
        if(zf_exti_assert(PIN_NULL !=                                           // 检查 对应通道是否初始化
            exti_obj_list[exti_index].pin_list[exti_channel]))
        {
            // 此处如果断言报错 那么证明本通道已经初始化过了 是不允许覆盖初始化的
            // 如果要更改本通道的引脚 应该先调用注销初始化 之后再重新初始化新的通道
            return_state = EXTI_ERROR_MODULE_OCCUPIED;
            break;
        }
        if(zf_exti_assert(NULL != callback))                                    // 检查 回调函数
        {
            // 此处如果断言报错 那么证明回调函数为空指针 是不允许的
            // 中断必须设置有效的回调函数 否则会导致程序异常
            return_state = EXTI_ERROR_INTERRUPT_CALLBACK_ILLEGAL;
            break;
        }

        // 修改回调函数需要先禁止中断触发
        // 避免在修改过程中触发中断导致程序异常
        zf_exti_disable(channel);
        exti_callback[exti_index][exti_channel].callback      = callback;
        exti_callback[exti_index][exti_channel].parameter_ptr = ptr;
        zf_exti_enable(channel);

        return_state = EXTI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断设置优先级
// 参数说明     channel             选择 EXTI 引脚  (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 参数说明     priority            中断优先级
// 返回参数     uint8               操作状态 ZF_NO_ERROR / EXTI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_set_interrupt_priority(channel, priority);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_set_interrupt_priority (zf_exti_channel_pin_enum channel, uint8 priority)
{
    zf_exti_operation_state_enum return_state = EXTI_ERROR_UNKNOW;

    do
    {
        uint8   exti_index      = ((channel >> EXTI_INDEX_OFFSET) & EXTI_INDEX_MASK);
        uint8   exti_channel    = ((channel >> EXTI_CHANNEL_OFFSET) & EXTI_CHANNEL_MASK);

        zf_interrupt_set_priority(exti_irq_index_list[exti_index][exti_channel], priority);

        return_state = EXTI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断注销初始化
// 参数说明     channel             选择 EXTI 引脚  (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / EXTI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_deinit(channel);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_channel_deinit (zf_exti_channel_pin_enum channel)
{
    zf_exti_operation_state_enum return_state = EXTI_ERROR_UNKNOW;

    uint8   exti_index      = ((channel >> EXTI_INDEX_OFFSET) & EXTI_INDEX_MASK);
    uint8   exti_channel    = ((channel >> EXTI_CHANNEL_OFFSET) & EXTI_CHANNEL_MASK);

    do
    {
        zf_gpio_pin_enum pin = (zf_gpio_pin_enum)(channel & EXTI_PIN_INDEX_MASK);

        if(zf_exti_assert(PIN_NULL !=                                           // 检查 对应通道是否初始化
            exti_obj_list[exti_index].pin_list[exti_channel]))
        {
            // 此处如果断言报错 那么证明本通道没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = EXTI_ERROR_MODULE_NOT_INIT;
            break;
        }
        if(zf_exti_assert(pin ==                                                // 检查 对应通道是否初始化
            exti_obj_list[exti_index].pin_list[exti_channel]))
        {
            // 此处如果断言报错 那么证明本通道原本初始化引脚与注销的引脚不匹配
            // 初始化用什么引脚 注销就用什么引脚 不要随便乱填
            return_state = EXTI_ERROR_PIN_MISMATCH;
            break;
        }

        exti_obj_list[exti_index].pin_list[0]    = PIN_NULL;
        exti_obj_list[exti_index].config_info   &= ~(0x00000003 << (exti_channel * 2));

        zf_exti_disable(channel);
        zf_gpio_deinit(pin);

        switch(exti_channel)
        {
            default :
            case    EXTI_CH_0   :  zf_interrupt_disable(exti_irq_index_list[exti_index][0]);    break;
            case    EXTI_CH_1   :  zf_interrupt_disable(exti_irq_index_list[exti_index][1]);    break;
            case    EXTI_CH_2   :  zf_interrupt_disable(exti_irq_index_list[exti_index][2]);    break;
            case    EXTI_CH_3   :  zf_interrupt_disable(exti_irq_index_list[exti_index][3]);    break;
            case    EXTI_CH_4   :  zf_interrupt_disable(exti_irq_index_list[exti_index][4]);    break;
            case    EXTI_CH_5   :
            case    EXTI_CH_6   :
            case    EXTI_CH_7   :
            case    EXTI_CH_8   :
            case    EXTI_CH_9   :
            {
                if(     PIN_NULL == exti_obj_list[exti_index].pin_list[5]
                    &&  PIN_NULL == exti_obj_list[exti_index].pin_list[6]
                    &&  PIN_NULL == exti_obj_list[exti_index].pin_list[7]
                    &&  PIN_NULL == exti_obj_list[exti_index].pin_list[8]
                    &&  PIN_NULL == exti_obj_list[exti_index].pin_list[9])
                    {
                        zf_interrupt_disable(exti_irq_index_list[exti_index][5]);
                    }
            }break;
            case    EXTI_CH_10  :
            case    EXTI_CH_11  :
            case    EXTI_CH_12  :
            case    EXTI_CH_13  :
            case    EXTI_CH_14  :
            case    EXTI_CH_15  :
            {
                if(     PIN_NULL == exti_obj_list[exti_index].pin_list[10]
                    &&  PIN_NULL == exti_obj_list[exti_index].pin_list[11]
                    &&  PIN_NULL == exti_obj_list[exti_index].pin_list[12]
                    &&  PIN_NULL == exti_obj_list[exti_index].pin_list[13]
                    &&  PIN_NULL == exti_obj_list[exti_index].pin_list[14]
                    &&  PIN_NULL == exti_obj_list[exti_index].pin_list[15])
                    {
                        zf_interrupt_disable(exti_irq_index_list[exti_index][10]);
                    }
            }break;
        }

        return_state = EXTI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断初始化
// 参数说明     channel             选择 EXTI 引脚  (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 参数说明     trigger             触发信号方式    (详见 zf_driver_exti.h 内 zf_exti_trigger_enum 定义)
// 参数说明     callback            回调函数 为无返回值 uint32 ptr 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / EXTI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_channel_init(channel, trigger, callback, ptr);
// 备注信息     初始化 A0 使用完之后 应当注销 A0 初始化
//              之后再进行 B0 的初始化 直接就进行 B0 初始化会断言报错
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_channel_init (zf_exti_channel_pin_enum channel, zf_exti_trigger_enum trigger, void_callback_uint32_ptr callback, void *ptr)
{
    zf_exti_operation_state_enum return_state = EXTI_ERROR_UNKNOW;

    uint8   exti_index      = ((channel >> EXTI_INDEX_OFFSET) & EXTI_INDEX_MASK);
    uint8   exti_channel    = ((channel >> EXTI_CHANNEL_OFFSET) & EXTI_CHANNEL_MASK);

    do
    {
        if(zf_exti_assert(PIN_NULL ==                                           // 检查 对应通道是否初始化
            exti_obj_list[exti_index].pin_list[exti_channel]))
        {
            // 此处如果断言报错 那么证明本通道已经初始化过了 是不允许覆盖初始化的
            // 如果要更改本通道的引脚 应该先调用注销初始化 之后再重新初始化新的通道
            return_state = EXTI_ERROR_MODULE_OCCUPIED;
            break;
        }
        if(zf_exti_assert(NULL != callback))                                    // 检查 回调函数
        {
            // 此处如果断言报错 那么证明回调函数为空指针 是不允许的
            // 中断必须设置有效的回调函数 否则会导致程序异常
            return_state = EXTI_ERROR_INTERRUPT_CALLBACK_ILLEGAL;
            break;
        }

        zf_gpio_pin_enum    pin             = (zf_gpio_pin_enum)(channel & EXTI_PIN_INDEX_MASK);
        uint8               io_group        = (pin >> GPIO_GROUP_INDEX_OFFSET); // 提取IO分组
        uint8               io_pin          = (pin & GPIO_PIN_INDEX_MASK);      // 提取IO引脚下标
        uint32              register_temp   = 0;

        exti_obj_list[exti_index].pin_list[exti_channel]     =  pin;
        exti_obj_list[exti_index].config_info               |=  (((uint32)trigger)   << (exti_channel * 2));

        zf_rcc_clock_enable(RCC_INDEX_SYSCFG);

        register_temp  = SYSCFG->EXTICR[io_pin / 4];
        register_temp &= ~(0x0000000F << ((io_pin % 4) * 4));
        register_temp |= (io_group << ((io_pin % 4) * 4));
        SYSCFG->EXTICR[io_pin / 4] = register_temp;

        zf_gpio_init(pin, GPI_PULL_UP, GPIO_HIGH);

        uint32 register_rtsr1 = exti_obj_list[exti_index].exti_ptr->RTSR1;
        uint32 register_ftsr1 = exti_obj_list[exti_index].exti_ptr->FTSR1;
        switch(trigger)
        {
            case EXTI_TRIGGER_TYPE_RISING:
            {
                register_rtsr1 |=  (0x00000001 << io_pin);
                register_ftsr1 &= ~(0x00000001 << io_pin);
            }break;
            case EXTI_TRIGGER_TYPE_FALLING:
            {
                register_rtsr1 &= ~(0x00000001 << io_pin);
                register_ftsr1 |=  (0x00000001 << io_pin);
            }break;
            case EXTI_TRIGGER_TYPE_BOTH:
            {
                register_rtsr1 |=  (0x00000001 << io_pin);
                register_ftsr1 |=  (0x00000001 << io_pin);
            }break;
        }
        exti_obj_list[exti_index].exti_ptr->RTSR1 = register_rtsr1;
        exti_obj_list[exti_index].exti_ptr->FTSR1 = register_ftsr1;

        zf_exti_set_callback(channel, callback, ptr);
        zf_exti_enable(channel);

        zf_interrupt_enable(exti_irq_index_list[exti_index][io_pin]);

        return_state = EXTI_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
