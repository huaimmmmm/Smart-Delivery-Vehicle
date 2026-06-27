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

#ifndef _zf_driver_exti_h_
#define _zf_driver_exti_h_

// SDK 底层驱动
#include "exti.h"

// zf_common 层 类型定义 引用
#include "zf_common_typedef.h"

// zf_driver 层 类型定义 引用
#include "zf_driver_gpio.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_exti_enable                                                               // EXTI 中断使能
// zf_exti_disable                                                              // EXTI 中断禁止

// zf_exti_set_callback                                                         // EXTI 中断设置回调函数
// zf_exti_set_interrupt_priority                                               // EXTI 中断设置优先级

// zf_exti_deinit                                                               // EXTI 中断注销初始化
// zf_exti_init                                                                 // EXTI 中断初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 UART 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define EXTI_MODULE_NUM_MAX     ( 1  )                                          // 最多 EXTI_MODULE_NUM_MAX 个 EXTI
#define EXTI_CHANNEL_NUM_MAX    ( 16 )                                          // 最多 EXTI_CHANNEL_NUM_MAX 个 CHANNEL

typedef enum                                                                    // 枚举 EXTI 索引 此枚举定义不允许用户修改
{
    EXTI_1,
}zf_exti_index_enum;

typedef enum                                                                    // 枚举 ADC 通道索引 此枚举定义不允许用户修改
{
    EXTI_CH_0   ,   EXTI_CH_1   ,   EXTI_CH_2   ,   EXTI_CH_3   ,
    EXTI_CH_4   ,   EXTI_CH_5   ,   EXTI_CH_6   ,   EXTI_CH_7   ,
    EXTI_CH_8   ,   EXTI_CH_9   ,   EXTI_CH_10  ,   EXTI_CH_11  ,
    EXTI_CH_12  ,   EXTI_CH_13  ,   EXTI_CH_14  ,   EXTI_CH_15  ,
}zf_exti_channel_index_enum;

// 定义引脚要用的宏定义 根据各单片机不同 可以自由定义需要什么信息
// bit[11:0 ] 固定为引脚索引
// bit[15:12] 在 EXTI 模块中为 EXTI 通道索引
// bit[19:16] 在 EXTI 模块中为 EXTI 通道索引

#define     EXTI_PIN_INDEX_OFFSET   ( 0      )                                  // bit[11:0 ] 存储 GPIO 的索引号
#define     EXTI_PIN_INDEX_MASK     ( 0x0FFF )                                  // 宽度 12bit 因此掩码为 0x0FFF

#define     EXTI_INDEX_OFFSET       ( 12     )                                  // bit[19:16] 存储 EXTI 通道索引
#define     EXTI_INDEX_MASK         ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0x0F

#define     EXTI_CHANNEL_OFFSET     ( 16     )                                  // bit[15:12] 存储 EXTI 索引
#define     EXTI_CHANNEL_MASK       ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0x0F

// 不同单片机通过各自模块属性匹配到这个结构
typedef enum                                                                    // 枚举 EXTI 通道索引 此枚举定义不允许用户修改
{
    EXTI1_CH0_A0    = ((EXTI_CH_0  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A0 )),
    EXTI1_CH0_B0    = ((EXTI_CH_0  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B0 )),
    EXTI1_CH0_C0    = ((EXTI_CH_0  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C0 )),
    EXTI1_CH0_D0    = ((EXTI_CH_0  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D0 )),
    EXTI1_CH0_E0    = ((EXTI_CH_0  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E0 )),
    EXTI1_CH0_F0    = ((EXTI_CH_0  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F0 )),
    EXTI1_CH0_G0    = ((EXTI_CH_0  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G0 )),
    EXTI1_CH0_H0    = ((EXTI_CH_0  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H0 )),
    EXTI1_CH0_I0    = ((EXTI_CH_0  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I0 )),

    EXTI1_CH1_A1    = ((EXTI_CH_1  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A1 )),
    EXTI1_CH1_B1    = ((EXTI_CH_1  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B1 )),
    EXTI1_CH1_C1    = ((EXTI_CH_1  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C1 )),
    EXTI1_CH1_D1    = ((EXTI_CH_1  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D1 )),
    EXTI1_CH1_E1    = ((EXTI_CH_1  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E1 )),
    EXTI1_CH1_F1    = ((EXTI_CH_1  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F1 )),
    EXTI1_CH1_G1    = ((EXTI_CH_1  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G1 )),
    EXTI1_CH1_H1    = ((EXTI_CH_1  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H1 )),
    EXTI1_CH1_I1    = ((EXTI_CH_1  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I1 )),

    EXTI1_CH2_A2    = ((EXTI_CH_2  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A2 )),
    EXTI1_CH2_B2    = ((EXTI_CH_2  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B2 )),
    EXTI1_CH2_C2    = ((EXTI_CH_2  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C2 )),
    EXTI1_CH2_D2    = ((EXTI_CH_2  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D2 )),
    EXTI1_CH2_E2    = ((EXTI_CH_2  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E2 )),
    EXTI1_CH2_F2    = ((EXTI_CH_2  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F2 )),
    EXTI1_CH2_G2    = ((EXTI_CH_2  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G2 )),
    EXTI1_CH2_H2    = ((EXTI_CH_2  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H2 )),
    EXTI1_CH2_I2    = ((EXTI_CH_2  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I2 )),

    EXTI1_CH3_A3    = ((EXTI_CH_3  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A3 )),
    EXTI1_CH3_B3    = ((EXTI_CH_3  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B3 )),
    EXTI1_CH3_C3    = ((EXTI_CH_3  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C3 )),
    EXTI1_CH3_D3    = ((EXTI_CH_3  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D3 )),
    EXTI1_CH3_E3    = ((EXTI_CH_3  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E3 )),
    EXTI1_CH3_F3    = ((EXTI_CH_3  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F3 )),
    EXTI1_CH3_G3    = ((EXTI_CH_3  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G3 )),
    EXTI1_CH3_H3    = ((EXTI_CH_3  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H3 )),
    EXTI1_CH3_I3    = ((EXTI_CH_3  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I3 )),

    EXTI1_CH4_A4    = ((EXTI_CH_4  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A4 )),
    EXTI1_CH4_B4    = ((EXTI_CH_4  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B4 )),
    EXTI1_CH4_C4    = ((EXTI_CH_4  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C4 )),
    EXTI1_CH4_D4    = ((EXTI_CH_4  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D4 )),
    EXTI1_CH4_E4    = ((EXTI_CH_4  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E4 )),
    EXTI1_CH4_F4    = ((EXTI_CH_4  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F4 )),
    EXTI1_CH4_G4    = ((EXTI_CH_4  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G4 )),
    EXTI1_CH4_H4    = ((EXTI_CH_4  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H4 )),
    EXTI1_CH4_I4    = ((EXTI_CH_4  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I4 )),

    EXTI1_CH5_A5    = ((EXTI_CH_5  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A5 )),
    EXTI1_CH5_B5    = ((EXTI_CH_5  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B5 )),
    EXTI1_CH5_C5    = ((EXTI_CH_5  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C5 )),
    EXTI1_CH5_D5    = ((EXTI_CH_5  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D5 )),
    EXTI1_CH5_E5    = ((EXTI_CH_5  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E5 )),
    EXTI1_CH5_F5    = ((EXTI_CH_5  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F5 )),
    EXTI1_CH5_G5    = ((EXTI_CH_5  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G5 )),
    EXTI1_CH5_H5    = ((EXTI_CH_5  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H5 )),
    EXTI1_CH5_I5    = ((EXTI_CH_5  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I5 )),

    EXTI1_CH6_A6    = ((EXTI_CH_6  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A6 )),
    EXTI1_CH6_B6    = ((EXTI_CH_6  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B6 )),
    EXTI1_CH6_C6    = ((EXTI_CH_6  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C6 )),
    EXTI1_CH6_D6    = ((EXTI_CH_6  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D6 )),
    EXTI1_CH6_E6    = ((EXTI_CH_6  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E6 )),
    EXTI1_CH6_F6    = ((EXTI_CH_6  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F6 )),
    EXTI1_CH6_G6    = ((EXTI_CH_6  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G6 )),
    EXTI1_CH6_H6    = ((EXTI_CH_6  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H6 )),
    EXTI1_CH6_I6    = ((EXTI_CH_6  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I6 )),

    EXTI1_CH7_A7    = ((EXTI_CH_7  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A7 )),
    EXTI1_CH7_B7    = ((EXTI_CH_7  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B7 )),
    EXTI1_CH7_C7    = ((EXTI_CH_7  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C7 )),
    EXTI1_CH7_D7    = ((EXTI_CH_7  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D7 )),
    EXTI1_CH7_E7    = ((EXTI_CH_7  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E7 )),
    EXTI1_CH7_F7    = ((EXTI_CH_7  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F7 )),
    EXTI1_CH7_G7    = ((EXTI_CH_7  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G7 )),
    EXTI1_CH7_H7    = ((EXTI_CH_7  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H7 )),
    EXTI1_CH7_I7    = ((EXTI_CH_7  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I7 )),

    EXTI1_CH8_A8    = ((EXTI_CH_8  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A8 )),
    EXTI1_CH8_B8    = ((EXTI_CH_8  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B8 )),
    EXTI1_CH8_C8    = ((EXTI_CH_8  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C8 )),
    EXTI1_CH8_D8    = ((EXTI_CH_8  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D8 )),
    EXTI1_CH8_E8    = ((EXTI_CH_8  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E8 )),
    EXTI1_CH8_F8    = ((EXTI_CH_8  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F8 )),
    EXTI1_CH8_G8    = ((EXTI_CH_8  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G8 )),
    EXTI1_CH8_H8    = ((EXTI_CH_8  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H8 )),
    EXTI1_CH8_I8    = ((EXTI_CH_8  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I8 )),

    EXTI1_CH9_A9    = ((EXTI_CH_9  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A9 )),
    EXTI1_CH9_B9    = ((EXTI_CH_9  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B9 )),
    EXTI1_CH9_C9    = ((EXTI_CH_9  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C9 )),
    EXTI1_CH9_D9    = ((EXTI_CH_9  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D9 )),
    EXTI1_CH9_E9    = ((EXTI_CH_9  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E9 )),
    EXTI1_CH9_F9    = ((EXTI_CH_9  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F9 )),
    EXTI1_CH9_G9    = ((EXTI_CH_9  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G9 )),
    EXTI1_CH9_H9    = ((EXTI_CH_9  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H9 )),
    EXTI1_CH9_I9    = ((EXTI_CH_9  << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I9 )),

    EXTI1_CH10_A10  = ((EXTI_CH_10 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A10)),
    EXTI1_CH10_B10  = ((EXTI_CH_10 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B10)),
    EXTI1_CH10_C10  = ((EXTI_CH_10 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C10)),
    EXTI1_CH10_D10  = ((EXTI_CH_10 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D10)),
    EXTI1_CH10_E10  = ((EXTI_CH_10 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E10)),
    EXTI1_CH10_F10  = ((EXTI_CH_10 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F10)),
    EXTI1_CH10_G10  = ((EXTI_CH_10 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G10)),
    EXTI1_CH10_H10  = ((EXTI_CH_10 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H10)),
    EXTI1_CH10_I10  = ((EXTI_CH_10 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I10)),

    EXTI1_CH11_A11  = ((EXTI_CH_11 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A11)),
    EXTI1_CH11_B11  = ((EXTI_CH_11 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B11)),
    EXTI1_CH11_C11  = ((EXTI_CH_11 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C11)),
    EXTI1_CH11_D11  = ((EXTI_CH_11 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D11)),
    EXTI1_CH11_E11  = ((EXTI_CH_11 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E11)),
    EXTI1_CH11_F11  = ((EXTI_CH_11 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F11)),
    EXTI1_CH11_G11  = ((EXTI_CH_11 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G11)),
    EXTI1_CH11_H11  = ((EXTI_CH_11 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H11)),
    EXTI1_CH11_I11  = ((EXTI_CH_11 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I11)),

    EXTI1_CH12_A12  = ((EXTI_CH_12 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A12)),
    EXTI1_CH12_B12  = ((EXTI_CH_12 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B12)),
    EXTI1_CH12_C12  = ((EXTI_CH_12 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C12)),
    EXTI1_CH12_D12  = ((EXTI_CH_12 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D12)),
    EXTI1_CH12_E12  = ((EXTI_CH_12 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E12)),
    EXTI1_CH12_F12  = ((EXTI_CH_12 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F12)),
    EXTI1_CH12_G12  = ((EXTI_CH_12 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G12)),
    EXTI1_CH12_H12  = ((EXTI_CH_12 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H12)),
    EXTI1_CH12_I12  = ((EXTI_CH_12 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I12)),

    EXTI1_CH13_A13  = ((EXTI_CH_13 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A13)),
    EXTI1_CH13_B13  = ((EXTI_CH_13 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B13)),
    EXTI1_CH13_C13  = ((EXTI_CH_13 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C13)),
    EXTI1_CH13_D13  = ((EXTI_CH_13 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D13)),
    EXTI1_CH13_E13  = ((EXTI_CH_13 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E13)),
    EXTI1_CH13_F13  = ((EXTI_CH_13 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F13)),
    EXTI1_CH13_G13  = ((EXTI_CH_13 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G13)),
    EXTI1_CH13_H13  = ((EXTI_CH_13 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H13)),
    EXTI1_CH13_I13  = ((EXTI_CH_13 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I13)),

    EXTI1_CH14_A14  = ((EXTI_CH_14 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A14)),
    EXTI1_CH14_B14  = ((EXTI_CH_14 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B14)),
    EXTI1_CH14_C14  = ((EXTI_CH_14 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C14)),
    EXTI1_CH14_D14  = ((EXTI_CH_14 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D14)),
    EXTI1_CH14_E14  = ((EXTI_CH_14 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E14)),
    EXTI1_CH14_F14  = ((EXTI_CH_14 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F14)),
    EXTI1_CH14_G14  = ((EXTI_CH_14 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G14)),
    EXTI1_CH14_H14  = ((EXTI_CH_14 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H14)),
    EXTI1_CH14_I14  = ((EXTI_CH_14 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I14)),

    EXTI1_CH15_A15  = ((EXTI_CH_15 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (A15)),
    EXTI1_CH15_B15  = ((EXTI_CH_15 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (B15)),
    EXTI1_CH15_C15  = ((EXTI_CH_15 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (C15)),
    EXTI1_CH15_D15  = ((EXTI_CH_15 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (D15)),
    EXTI1_CH15_E15  = ((EXTI_CH_15 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (E15)),
    EXTI1_CH15_F15  = ((EXTI_CH_15 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (F15)),
    EXTI1_CH15_G15  = ((EXTI_CH_15 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (G15)),
    EXTI1_CH15_H15  = ((EXTI_CH_15 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (H15)),
    EXTI1_CH15_I15  = ((EXTI_CH_15 << EXTI_CHANNEL_OFFSET) | (EXTI_1 << EXTI_INDEX_OFFSET) | (I15)),
}zf_exti_channel_pin_enum;

typedef enum{                                                                   // 枚举 EXTI 触发信号  此枚举定义不允许用户修改
    EXTI_TRIGGER_TYPE_RISING                =   1           ,                   // 上升沿触发模式
    EXTI_TRIGGER_TYPE_FALLING               =   2           ,                   // 下降沿触发模式
    EXTI_TRIGGER_TYPE_BOTH                  =   3           ,                   // 双边沿触发模式
}zf_exti_trigger_enum;

typedef enum                                                                    // 枚举 EXTI 中断状态 此枚举定义不允许用户修改
{
    EXTI_INTERRUPT_STATE_NONE               =   0x00        ,

    EXTI_INTERRUPT_STATE_RISING             =   0x01        ,
    EXTI_INTERRUPT_STATE_FALLING            =   0x02        ,
    EXTI_INTERRUPT_STATE_BOTH               =   0x03        ,
}zf_exti_interrupt_state_enum;

typedef enum                                                                    // 枚举 EXTI 操作状态   此枚举定义不允许用户修改
{
    EXTI_OPERATION_DONE                     = ZF_NO_ERROR   ,                   // EXTI 操作完成
    EXTI_ERROR_UNKNOW                       = ZF_ERROR      ,                   // EXTI 未知错误 操作无法进行 正常情况下不会出现此错误

    EXTI_ERROR_MODULE_OCCUPIED              = 0x20          ,                   // EXTI 模块已被占用 操作无法进行
    EXTI_ERROR_MODULE_NOT_INIT                              ,                   // EXTI 模块未初始化 操作无法进行

    EXTI_ERROR_PIN_MISMATCH                                 ,                   // EXTI 引脚不匹配 操作无法进行
    EXTI_ERROR_INTERRUPT_CALLBACK_ILLEGAL                   ,                   // EXTI 中断回调异常 操作无法进行
}zf_exti_operation_state_enum;

typedef struct                                                                  // EXTI 管理对象模板 用于存储 EXTI 的信息
{
    // 此处的结构固定为 触发源状态信息

    // 通常来说不少单片机是将 GPIO 按照组或者线划分到 EXTI
    // 按照实际的每个可配置中断来进行分组管理
    // 以 Line 划分的就单个引脚单 Line 记录 一般 Line 只支持单个输入源
    // 以 Group 划分的就每个 Group 进行划分 并使用 pin_list 记录引脚

    EXTI_TypeDef        *exti_ptr                           ;

    zf_gpio_pin_enum    pin_list[16]                        ;

    union
    {
        uint32          config_info                         ;
        struct
        {
            uint32      trigger_type0       : 2             ;
            uint32      trigger_type1       : 2             ;
            uint32      trigger_type2       : 2             ;
            uint32      trigger_type3       : 2             ;
            uint32      trigger_type4       : 2             ;
            uint32      trigger_type5       : 2             ;
            uint32      trigger_type6       : 2             ;
            uint32      trigger_type7       : 2             ;
            uint32      trigger_type8       : 2             ;
            uint32      trigger_type9       : 2             ;
            uint32      trigger_type10      : 2             ;
            uint32      trigger_type11      : 2             ;
            uint32      trigger_type12      : 2             ;
            uint32      trigger_type13      : 2             ;
            uint32      trigger_type14      : 2             ;
            uint32      trigger_type15      : 2             ;
        };
    };
}zf_exti_obj_struct;
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
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
uint8 zf_exti_assert_handler (char *file, int line, char *tips);
#define zf_exti_assert(status)      ((status) ? (0) : zf_exti_assert_handler(__FILE__, __LINE__, NULL))
#define zf_exti_log(status, tips)   ((status) ? (0) : zf_exti_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断使能
// 参数说明     channel             选择 EXTI 引脚  (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / EXTI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_enable(channel);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_enable (zf_exti_channel_pin_enum channel);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断失能
// 参数说明     channel             选择 EXTI 引脚  (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / EXTI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_disable(channel);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_disable (zf_exti_channel_pin_enum channel);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断设置回调函数
// 参数说明     channel             选择 EXTI 引脚  (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 参数说明     callback            回调函数 为无返回值 uint32 void* 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / EXTI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_set_callback(channel, callback, NULL);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_set_callback (zf_exti_channel_pin_enum channel, void_callback_uint32_ptr callback, void *ptr);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断设置优先级
// 参数说明     channel             选择 EXTI 引脚  (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 参数说明     priority            中断优先级
// 返回参数     uint8               操作状态 ZF_NO_ERROR / EXTI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_set_interrupt_priority(channel, priority);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_set_interrupt_priority (zf_exti_channel_pin_enum channel, uint8 priority);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断注销初始化
// 参数说明     channel             选择 EXTI 引脚  (详见 zf_driver_exti.h 内 zf_exti_channel_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / EXTI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_exti_deinit(channel);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_exti_channel_deinit (zf_exti_channel_pin_enum channel);

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
uint8 zf_exti_channel_init (zf_exti_channel_pin_enum channel, zf_exti_trigger_enum trigger, void_callback_uint32_ptr callback, void *ptr);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
