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
* 文件名称          zf_driver_interrupt
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

#ifndef _zf_driver_interrupt_h_
#define _zf_driver_interrupt_h_

// SDK 底层驱动
#include <platform.h>
#include <irq.h>

// zf_common 层引用
#include "zf_common_typedef.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_interrupt_global_enable                                                   // 全局中断使能
// zf_interrupt_global_disable                                                  // 全局中断屏蔽

// zf_interrupt_check                                                           // 确认指定中断使能状态
// zf_interrupt_enable                                                          // 指定中断使能
// zf_interrupt_disable                                                         // 指定中断屏蔽

// zf_interrupt_get_priority                                                    // 指定中断获取优先级
// zf_interrupt_set_priority                                                    // 指定中断设置优先级

// zf_interrupt_init                                                            // 中断组初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件需要的枚举与对象结构等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define     INTERRUPT_PRIORITY_MASK     ( 0x0F )

typedef enum                                                                    // 枚举常用中断索引号   此枚举定义不允许用户修改
{
    INTERRUPT_INDEX_EXTI0           = EXTI0_IRQn                ,
    INTERRUPT_INDEX_EXTI1           = EXTI1_IRQn                ,
    INTERRUPT_INDEX_EXTI2           = EXTI2_IRQn                ,
    INTERRUPT_INDEX_EXTI3           = EXTI3_IRQn                ,
    INTERRUPT_INDEX_EXTI4           = EXTI4_IRQn                ,
    INTERRUPT_INDEX_EXTI9_5         = EXTI9_5_IRQn              ,
    INTERRUPT_INDEX_EXTI15_10       = EXTI15_10_IRQn            ,

    INTERRUPT_INDEX_TIM1_PIT        = TIM1_UP_IRQn              ,
    INTERRUPT_INDEX_TIM8_PIT        = TIM8_UP_IRQn              ,

    INTERRUPT_INDEX_TIM2_PIT        = TIM2_IRQn                 ,
    INTERRUPT_INDEX_TIM5_PIT        = TIM5_IRQn                 ,

    INTERRUPT_INDEX_TIM3_PIT        = TIM3_IRQn                 ,
    INTERRUPT_INDEX_TIM4_PIT        = TIM4_IRQn                 ,

    INTERRUPT_INDEX_TIM15_PIT       = TIM15_IRQn                ,
    INTERRUPT_INDEX_TIM16_PIT       = TIM16_IRQn                ,

    INTERRUPT_INDEX_TIM6_PIT        = TIM6_IRQn                 ,
    INTERRUPT_INDEX_TIM7_PIT        = TIM7_IRQn                 ,

    INTERRUPT_INDEX_TIM_TS          = TIM_TS_IRQn               ,

    INTERRUPT_INDEX_HRTIM1_MASTER   = HRTIM1_MASTER_INT_IRQn    ,
    INTERRUPT_INDEX_HRTIM1_TIMA     = HRTIM1_TIMA_INT_IRQn      ,
    INTERRUPT_INDEX_HRTIM1_TIMB     = HRTIM1_TIMB_INT_IRQn      ,
    INTERRUPT_INDEX_HRTIM1_TIMC     = HRTIM1_TIMC_INT_IRQn      ,
    INTERRUPT_INDEX_HRTIM1_TIMD     = HRTIM1_TIMD_INT_IRQn      ,
    INTERRUPT_INDEX_HRTIM1_TIME     = HRTIM1_TIME_INT_IRQn      ,
    INTERRUPT_INDEX_HRTIM1_TIMF     = HRTIM1_TIMF_INT_IRQn      ,
    INTERRUPT_INDEX_HRTIM2_MASTER   = HRTIM2_MASTER_INT_IRQn    ,
    INTERRUPT_INDEX_HRTIM2_TIMA     = HRTIM2_TIMA_INT_IRQn      ,
    INTERRUPT_INDEX_HRTIM2_TIMB     = HRTIM2_TIMB_INT_IRQn      ,
    INTERRUPT_INDEX_HRTIM2_TIMC     = HRTIM2_TIMC_INT_IRQn      ,
    INTERRUPT_INDEX_HRTIM2_TIMD     = HRTIM2_TIMD_INT_IRQn      ,
    INTERRUPT_INDEX_HRTIM2_TIME     = HRTIM2_TIME_INT_IRQn      ,
    INTERRUPT_INDEX_HRTIM2_TIMF     = HRTIM2_TIMF_INT_IRQn      ,

    INTERRUPT_INDEX_UART1           = UART1_IRQn                ,
    INTERRUPT_INDEX_UART2           = UART2_IRQn                ,
    INTERRUPT_INDEX_UART3           = UART3_IRQn                ,

    INTERRUPT_INDEX_SPI1            = SPI1_IRQn                 ,
    INTERRUPT_INDEX_SPI2            = SPI2_IRQn                 ,
    INTERRUPT_INDEX_SPI3            = SPI3_IRQn                 ,
    INTERRUPT_INDEX_SPI4            = SPI4_IRQn                 ,

    INTERRUPT_INDEX_IIC1            = I2C1_EV_IRQn              ,
    INTERRUPT_INDEX_IIC2            = I2C2_EV_IRQn              ,

    INTERRUPT_INDEX_CAN1            = M_CAN_1_INT0_IRQn         ,
    INTERRUPT_INDEX_CAN2            = M_CAN_2_INT0_IRQn         ,
    INTERRUPT_INDEX_CAN3            = M_CAN_3_INT0_IRQn         ,
    INTERRUPT_INDEX_CAN4            = M_CAN_4_INT0_IRQn         ,

    INTERRUPT_INDEX_ADC1            = ADC1_IRQn                 ,
    INTERRUPT_INDEX_ADC2            = ADC2_IRQn                 ,
    INTERRUPT_INDEX_ADC3            = ADC3_IRQn                 ,
    INTERRUPT_INDEX_ADC4            = ADC4_IRQn                 ,
    INTERRUPT_INDEX_ADC5            = ADC5_IRQn                 ,

    INTERRUPT_INDEX_SD_ADC1         = SD_ADC1_IRQn              ,
    INTERRUPT_INDEX_SD_ADC2         = SD_ADC2_IRQn              ,

    INTERRUPT_INDEX_COMP1_2         = COMP_1_2_IRQn             ,
    INTERRUPT_INDEX_COMP3_4         = COMP_3_4_IRQn             ,
    INTERRUPT_INDEX_COMP5_6         = COMP_5_6_IRQn             ,
    INTERRUPT_INDEX_COMP7_8         = COMP_7_8_IRQn             ,

    INTERRUPT_INDEX_DAC1            = DAC1_IRQn                 ,
    INTERRUPT_INDEX_DAC2            = DAC2_IRQn                 ,
    INTERRUPT_INDEX_DAC3            = DAC3_IRQn                 ,
    INTERRUPT_INDEX_DAC4            = DAC4_IRQn                 ,

    INTERRUPT_INDEX_DMA1_CH0        = DMA1_STREAM0_IRQn         ,
    INTERRUPT_INDEX_DMA1_CH1        = DMA1_STREAM1_IRQn         ,
    INTERRUPT_INDEX_DMA1_CH2        = DMA1_STREAM2_IRQn         ,
    INTERRUPT_INDEX_DMA1_CH3        = DMA1_STREAM3_IRQn         ,
    INTERRUPT_INDEX_DMA1_CH4        = DMA1_STREAM4_IRQn         ,
    INTERRUPT_INDEX_DMA1_CH5        = DMA1_STREAM5_IRQn         ,
    INTERRUPT_INDEX_DMA1_CH6        = DMA1_STREAM6_IRQn         ,
    INTERRUPT_INDEX_DMA1_CH7        = DMA1_STREAM7_IRQn         ,
    INTERRUPT_INDEX_DMA2_CH0        = DMA2_STREAM0_IRQn         ,
    INTERRUPT_INDEX_DMA2_CH1        = DMA2_STREAM1_IRQn         ,
    INTERRUPT_INDEX_DMA2_CH2        = DMA2_STREAM2_IRQn         ,
    INTERRUPT_INDEX_DMA2_CH3        = DMA2_STREAM3_IRQn         ,
    INTERRUPT_INDEX_DMA2_CH4        = DMA2_STREAM4_IRQn         ,
    INTERRUPT_INDEX_DMA2_CH5        = DMA2_STREAM5_IRQn         ,
    INTERRUPT_INDEX_DMA2_CH6        = DMA2_STREAM6_IRQn         ,
    INTERRUPT_INDEX_DMA2_CH7        = DMA2_STREAM7_IRQn         ,

}zf_interrupt_index_enum;

typedef enum                                                                    // 枚举可用中断优先级   此枚举定义不允许用户修改
{
    INTERRUPT_PRIORITY_HIGH     = 0 ,

    INTERRUPT_PRIORITY_0        = 0 ,
    INTERRUPT_PRIORITY_1            ,
    INTERRUPT_PRIORITY_2            ,
    INTERRUPT_PRIORITY_3            ,
    INTERRUPT_PRIORITY_4            ,
    INTERRUPT_PRIORITY_5            ,
    INTERRUPT_PRIORITY_6            ,
    INTERRUPT_PRIORITY_7            ,
#if (0x07 == INTERRUPT_PRIORITY_MASK)
    INTERRUPT_PRIORITY_LOW      = 7 ,
#else
    INTERRUPT_PRIORITY_8            ,
    INTERRUPT_PRIORITY_9            ,
    INTERRUPT_PRIORITY_10           ,
    INTERRUPT_PRIORITY_11           ,
    INTERRUPT_PRIORITY_12           ,
    INTERRUPT_PRIORITY_13           ,
    INTERRUPT_PRIORITY_14           ,
    INTERRUPT_PRIORITY_15           ,

    INTERRUPT_PRIORITY_LOW      = 15,
#endif
}zf_interrupt_priority_enum;

typedef struct                                                                  //  定义一个标准中断回调结构    此枚举定义不允许用户修改
{
    void_callback_uint32_ptr    callback        ;                               // 第一个参数是中断状态
    void                        *parameter_ptr  ;                               // 第二个参数是用户指针参数
}zf_interrupt_callback_struct;
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     全局中断使能
// 参数说明     primask             当前屏蔽嵌套层数
// 返回参数     void
// 使用示例     zf_interrupt_global_enable(primask);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_interrupt_global_enable (uint32 primask);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     全局中断屏蔽
// 参数说明     void
// 返回参数     uint32              当前屏蔽嵌套层数
// 使用示例     primask = zf_interrupt_global_disable();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_interrupt_global_disable (void);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     确认指定中断使能状态
// 参数说明     irqn                指定中断号  (详见 zf_driver_interrupt.h 内 zf_interrupt_index_enum 定义)
// 返回参数     uint8               ZF_TRUE - 已使能 / ZF_FALSE - 未使能
// 使用示例     zf_interrupt_check(irqn);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_interrupt_check (zf_interrupt_index_enum irqn);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     指定中断使能
// 参数说明     irqn                指定中断号  (详见 zf_driver_interrupt.h 内 zf_interrupt_index_enum 定义)
// 返回参数     uint32          
// 使用示例     zf_interrupt_enable(irqn);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_interrupt_enable (zf_interrupt_index_enum irqn);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     指定中断屏蔽
// 参数说明     irqn                指定中断号  (详见 zf_driver_interrupt.h 内 zf_interrupt_index_enum 定义)
// 返回参数     void
// 使用示例     zf_interrupt_disable(irqn);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_interrupt_disable (zf_interrupt_index_enum irqn);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     指定中断获取优先级
// 参数说明     irqn                指定中断号  (详见 zf_driver_interrupt.h 内 zf_interrupt_index_enum 定义)
// 返回参数     uint32              当前优先级
// 使用示例     zf_interrupt_get_priority(irqn);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_interrupt_get_priority (zf_interrupt_index_enum irqn);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     指定中断设置优先级
// 参数说明     irqn                指定中断号  (详见 zf_driver_interrupt.h 内 zf_interrupt_index_enum 定义)
// 参数说明     priority            中断优先级  (详见 zf_driver_interrupt.h 内 zf_interrupt_priority_enum 定义)
// 返回参数     void
// 使用示例     zf_interrupt_set_priority(irqn, priority);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_interrupt_set_priority (zf_interrupt_index_enum irqn, zf_interrupt_priority_enum priority);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     中断组初始化
// 参数说明     void
// 返回参数     void
// 使用示例     zf_interrupt_init();
// 备注信息     会在  clock_init 内部调用
//-------------------------------------------------------------------------------------------------------------------
void zf_interrupt_init (void);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
