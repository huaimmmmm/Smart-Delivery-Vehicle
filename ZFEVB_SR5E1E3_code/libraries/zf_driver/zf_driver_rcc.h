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
* 文件名称          zf_driver_rcc
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

#ifndef _zf_driver_rcc_h_
#define _zf_driver_rcc_h_

// SDK 底层驱动
#include "platform.h"

// zf_common 层引用
#include "zf_common_typedef.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_rcc_peripheral_reset                                                      // 外设模块复位
// zf_rcc_clock_check                                                           // 外设时钟状态检查
// zf_rcc_clock_disable                                                         // 外设时钟禁止
// zf_rcc_clock_enable                                                          // 外设时钟使能
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件需要的枚举与对象结构等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 本部分驱动是直接操作 RCC 寄存器对应的地址写入对应位
// 因此需要保存对应的 RCC 寄存器地址索引号和具体外设对应的 bit
#define         RCC_BUS_INDEX_OFFSET            ( 0    )                        // 那么低位用来存储寄存器索引号
#define         RCC_BUS_INDEX_MASK              ( 0x0F )                        // 默认用 4bit 保存索引信息 掩码就是 0x0F

#define         RCC_PERIPHERAL_INDEX_OFFSET     ( 4u   )                        // 低 4bit 用于存储索引号了 那么这里偏移值就是 4
#define         RCC_PERIPHERAL_INDEX_MASK       ( 0x1F )                        // 位移数据范围是 [0, 31] 那么掩码就是 0x1F

// 那么以下部分就是定义的具体 RCC 的相关总线索引了 配合寄存器地址列表使用
#define         RCC_INDEX_AHB1L_BUS_INDEX       ( 0u )
#define         RCC_INDEX_AHB1H_BUS_INDEX       ( 1u )
#define         RCC_INDEX_AHB2L_BUS_INDEX       ( 2u )
#define         RCC_INDEX_AHB2H_BUS_INDEX       ( 3u )
#define         RCC_INDEX_APB1L_BUS_INDEX       ( 4u )
#define         RCC_INDEX_APB2L_BUS_INDEX       ( 5u )
#define         RCC_INDEX_APB2H_BUS_INDEX       ( 6u )

typedef enum                                                                    // 枚举 RCC 通道  此枚举定义不允许用户修改
{
    RCC_INDEX_HSEM      = (( 11u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB1L_BUS_INDEX),
    RCC_INDEX_CRC       = (( 12u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB1L_BUS_INDEX),
    RCC_INDEX_CORDIC    = (( 13u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB1L_BUS_INDEX),
    RCC_INDEX_DMA2      = (( 16u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB1L_BUS_INDEX),

    RCC_INDEX_ADC345    = ((  0u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB1H_BUS_INDEX),
    RCC_INDEX_DAC3      = ((  5u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB1H_BUS_INDEX),
    RCC_INDEX_DAC4      = ((  6u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB1H_BUS_INDEX),
    RCC_INDEX_COMP2_DIG = (( 12u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB1H_BUS_INDEX),
    RCC_INDEX_HRTIMER1  = (( 28u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB1H_BUS_INDEX),
    RCC_INDEX_HRTIMER2  = (( 29u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB1H_BUS_INDEX),

    RCC_INDEX_GPIOA     = ((  0u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2L_BUS_INDEX),
    RCC_INDEX_GPIOB     = ((  1u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2L_BUS_INDEX),
    RCC_INDEX_GPIOC     = ((  2u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2L_BUS_INDEX),
    RCC_INDEX_GPIOD     = ((  3u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2L_BUS_INDEX),
    RCC_INDEX_GPIOE     = ((  4u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2L_BUS_INDEX),
    RCC_INDEX_GPIOF     = ((  5u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2L_BUS_INDEX),
    RCC_INDEX_GPIOG     = ((  6u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2L_BUS_INDEX),
    RCC_INDEX_GPIOH     = ((  7u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2L_BUS_INDEX),
    RCC_INDEX_GPIOI     = ((  8u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2L_BUS_INDEX),
    RCC_INDEX_DMA1      = (( 16u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2L_BUS_INDEX),
    RCC_INDEX_DMAMUX1   = (( 17u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2L_BUS_INDEX),

    RCC_INDEX_ADC12     = ((  0u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2H_BUS_INDEX),
    RCC_INDEX_BDAC1     = ((  4u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2H_BUS_INDEX),
    RCC_INDEX_DAC1      = ((  5u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2H_BUS_INDEX),
    RCC_INDEX_DAC2      = ((  6u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2H_BUS_INDEX),
    RCC_INDEX_COMP1_DIG = (( 12u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_AHB2H_BUS_INDEX),

    RCC_INDEX_WWDG1     = ((  0u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_WWDG2     = ((  1u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_IWDG1     = ((  4u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_IWDG2     = ((  5u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_RTC       = (( 10u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_TIM2      = (( 15u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_TIM3      = (( 16u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_TIM6      = (( 17u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_TIM7      = (( 18u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_TIM_TS    = (( 19u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_I2C1      = (( 21u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_I2C2      = (( 22u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_SPI2      = (( 23u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_SPI3      = (( 24u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_UART2     = (( 27u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),
    RCC_INDEX_UART3     = (( 28u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB1L_BUS_INDEX),

    RCC_INDEX_SYSCFG    = ((  0u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2L_BUS_INDEX),
    RCC_INDEX_SMPU      = ((  2u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2L_BUS_INDEX),
    RCC_INDEX_TIM1      = (( 11u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2L_BUS_INDEX),
    RCC_INDEX_TIM8      = (( 12u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2L_BUS_INDEX),
    RCC_INDEX_TIM4      = (( 15u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2L_BUS_INDEX),
    RCC_INDEX_TIM5      = (( 16u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2L_BUS_INDEX),
    RCC_INDEX_TIM15     = (( 17u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2L_BUS_INDEX),
    RCC_INDEX_TIM16     = (( 18u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2L_BUS_INDEX),
    RCC_INDEX_SPI1      = (( 23u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2L_BUS_INDEX),
    RCC_INDEX_SPI4      = (( 24u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2L_BUS_INDEX),
    RCC_INDEX_UART1     = (( 27u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2L_BUS_INDEX),

    RCC_INDEX_SD_ADC1   = ((  0u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2H_BUS_INDEX),
    RCC_INDEX_SD_ADC2   = ((  1u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2H_BUS_INDEX),
    RCC_INDEX_FDCAN1    = ((  8u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2H_BUS_INDEX),
    RCC_INDEX_FDCAN2    = (( 10u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2H_BUS_INDEX),
    RCC_INDEX_FDCAN3    = (( 12u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2H_BUS_INDEX),
    RCC_INDEX_FDCAN4    = (( 14u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2H_BUS_INDEX),
    RCC_INDEX_FDCAN_MRAM= (( 16u << RCC_PERIPHERAL_INDEX_OFFSET) | RCC_INDEX_APB2H_BUS_INDEX),
}zf_rcc_index_enum;

// 这里就是 RCC 的寄存器地址列表 对应上方的总线索引
// 这样本文件中就不再去关心具体 SDK 的接口是什么样的
// 只要它是通过指定 时钟使能寄存器 外设复位寄存器 来控制
// 就可以通过这个模式直接兼容使用 通过下方枚举体直接定位到寄存器和对应位
#define RCC_RESET_REGISTER_LIST     {&RCC->AHB1LRSTR, &RCC->AHB1HRSTR, &RCC->AHB2LRSTR, &RCC->AHB2HRSTR, &RCC->APB1LRSTR, &RCC->APB2LRSTR, &RCC->APB2HRSTR}
#define RCC_ENABLE_REGISTER_LIST    {&RCC->AHB1LENR , &RCC->AHB1HENR , &RCC->AHB2LENR , &RCC->AHB2HENR , &RCC->APB1LENR , &RCC->APB2LENR , &RCC->APB2HENR }
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     外设模块复位
// 参数说明     zf_rcc_index        外设时钟索引    (详见 zf_driver_rcc.h 内 zf_rcc_index_enum 定义)
// 返回参数     void
// 使用示例     zf_rcc_peripheral_reset(zf_rcc_index);
//-------------------------------------------------------------------------------------------------------------------
void zf_rcc_peripheral_reset (zf_rcc_index_enum zf_rcc_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     外设时钟状态检查
// 参数说明     zf_rcc_index        外设时钟索引    (详见 zf_driver_rcc.h 内 zf_rcc_index_enum 定义)
// 返回参数     uint8               外设时钟状态
// 使用示例     zf_rcc_clock_check(zf_rcc_index);
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_rcc_clock_check (zf_rcc_index_enum zf_rcc_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     外设时钟禁止
// 参数说明     zf_rcc_index        外设时钟索引    (详见 zf_driver_rcc.h 内 zf_rcc_index_enum 定义)
// 返回参数     void
// 使用示例     zf_rcc_clock_disable(zf_rcc_index);
//-------------------------------------------------------------------------------------------------------------------
void zf_rcc_clock_disable (zf_rcc_index_enum zf_rcc_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     外设时钟使能
// 参数说明     zf_rcc_index        外设时钟索引    (详见 zf_driver_rcc.h 内 zf_rcc_index_enum 定义)
// 返回参数     void
// 使用示例     zf_rcc_clock_enable(zf_rcc_index);
//-------------------------------------------------------------------------------------------------------------------
void zf_rcc_clock_enable (zf_rcc_index_enum zf_rcc_index);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
