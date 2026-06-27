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

#ifndef _zf_driver_adc_h_
#define _zf_driver_adc_h_

// SDK 底层驱动
#include "saradc.h"

// zf_common 层 类型定义 引用
#include "zf_common_typedef.h"

// zf_driver 层 类型定义 引用
#include "zf_driver_gpio.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_adc_get_convert_data                                                       // ADC 获取对应通道转换数据
// zf_adc_get_convert_average_data                                               // ADC 获取对应通道转换数据的平均值

// zf_adc_enable                                                                 // ADC 通道使能
// zf_adc_disable                                                                // ADC 通道禁止

// zf_adc_set_resolution                                                         // ADC 精度设置

// zf_adc_channel_deinit                                                         // ADC 通道注销初始化
// zf_adc_module_deinit                                                          // ADC 模块注销初始化

// zf_adc_channel_init                                                           // ADC 通道初始化
// zf_adc_module_init                                                            // ADC 模块初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 ADC 相关的结构体数据构成细节 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define     ADC_NUM_MAX             (  5     )                                  // 最多 ADC_NUM_MAX 个 ADC

typedef enum                                                                    // 枚举 ADC 索引 此枚举定义不允许用户修改
{
    ADC_1   ,   ADC_2   ,   ADC_3   ,   ADC_4   ,   ADC_5   ,
}zf_adc_index_enum;

typedef enum                                                                    // 枚举 ADC 通道索引 此枚举定义不允许用户修改
{
    ADC_CH_1    = 1 ,
    ADC_CH_2    ,   ADC_CH_3    ,   ADC_CH_4    ,    ADC_CH_5   ,
    ADC_CH_6    ,   ADC_CH_7    ,   ADC_CH_8    ,    ADC_CH_9   ,
    ADC_CH_10   ,   ADC_CH_11   ,   ADC_CH_12   ,    ADC_CH_13  ,
    ADC_CH_14   ,   ADC_CH_15   ,   ADC_CH_16   ,    ADC_CH_17  ,
    ADC_CH_18   ,
}zf_adc_channel_index_enum;

typedef enum                                                                    // 枚举 ADC 分辨率   此枚举定义不允许用户修改
{
    ADC_RES12BIT,                                                               // 12 位分辨率
    ADC_RES10BIT,                                                               // 10 位分辨率
    ADC_RES8BIT ,                                                               // 8  位分辨率
    ADC_RES6BIT ,                                                               // 6  位分辨率
}zf_adc_resolution_enum;

// 定义引脚要用的宏定义 根据各单片机不同 可以自由定义需要什么信息
// bit[11:0 ] 固定为引脚索引
// bit[15:12] 在 ADC 模块中为 ADC 索引
// bit[19:16] 在 ADC 模块中为 ADC 通道索引

#define     ADC_PIN_INDEX_OFFSET    ( 0      )                                  // bit[11:0 ] 存储 GPIO 的索引号
#define     ADC_PIN_INDEX_MASK      ( 0x0FFF )                                  // 宽度 12bit 因此掩码为 0x0FFF

#define     ADC_INDEX_OFFSET        ( 12     )                                  // bit[19:16] 存储 ADC 通道索引
#define     ADC_INDEX_MASK          ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0x0F

#define     ADC_CHANNEL_OFFSET      ( 16     )                                  // bit[15:12] 存储 ADC 索引
#define     ADC_CHANNEL_MASK        ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0x0F

// CH1-CH5  是 Fast channel
// CH6-CH18 是 Slow channel
typedef enum                                                                    // 枚举 ADC 引脚通道  此枚举定义不允许用户修改
{
    // ADC1 模块通道枚举
    ADC1_CH1_B2                     = ((ADC_CH_1  << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (B2      )),
    ADC1_CH2_B3                     = ((ADC_CH_2  << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (B3      )),
    ADC1_CH3_B4                     = ((ADC_CH_3  << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (B4      )),
    ADC1_CH4_B5                     = ((ADC_CH_4  << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (B5      )),
    ADC1_CH5_B6                     = ((ADC_CH_5  << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (B6      )),
    ADC1_CH6_D3                     = ((ADC_CH_6  << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (D3      )),
    ADC1_CH7_D4                     = ((ADC_CH_7  << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (D4      )),
    ADC1_CH8_D5                     = ((ADC_CH_8  << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (D5      )),
    ADC1_CH9_C1                     = ((ADC_CH_9  << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (C1      )),
    ADC1_CH10_C2                    = ((ADC_CH_10 << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (C2      )),
    ADC1_CH11_C7                    = ((ADC_CH_11 << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (C7      )),
    ADC1_CH12_C8                    = ((ADC_CH_12 << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (C8      )),
    ADC1_CH15_C6                    = ((ADC_CH_15 << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (C6      )),
    ADC1_CH16_A13                   = ((ADC_CH_16 << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (A13     )),
    ADC1_CH17_A14                   = ((ADC_CH_17 << ADC_CHANNEL_OFFSET) | (ADC_1 << ADC_INDEX_OFFSET) | (A14     )),

    // ADC2 模块通道枚举
    ADC2_CH1_B7                     = ((ADC_CH_1  << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (B7      )),
    ADC2_CH2_B8                     = ((ADC_CH_2  << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (B8      )),
    ADC2_CH3_B9                     = ((ADC_CH_3  << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (B9      )),
    ADC2_CH4_B10                    = ((ADC_CH_4  << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (B10     )),
    ADC2_CH5_B11                    = ((ADC_CH_5  << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (B11     )),
    ADC2_CH6_D6                     = ((ADC_CH_6  << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (D6      )),
    ADC2_CH7_D9                     = ((ADC_CH_7  << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (D9      )),
    ADC2_CH8_E0                     = ((ADC_CH_8  << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (E0      )),
    ADC2_CH9_C1                     = ((ADC_CH_9  << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (C1      )),
    ADC2_CH10_C2                    = ((ADC_CH_10 << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (C2      )),
    ADC2_CH11_B15                   = ((ADC_CH_11 << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (B15     )),
    ADC2_CH12_C0                    = ((ADC_CH_12 << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (C0      )),
    ADC2_CH15_B12                   = ((ADC_CH_15 << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (B12     )),
    ADC2_CH16_B13                   = ((ADC_CH_16 << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (B13     )),
    ADC2_CH17_B14                   = ((ADC_CH_17 << ADC_CHANNEL_OFFSET) | (ADC_2 << ADC_INDEX_OFFSET) | (B14     )),

    // ADC3 模块通道枚举
    ADC3_CH1_B12                    = ((ADC_CH_1  << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (B12     )),
    ADC3_CH2_B13                    = ((ADC_CH_2  << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (B13     )),
    ADC3_CH3_B14                    = ((ADC_CH_3  << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (B14     )),
    ADC3_CH4_B15                    = ((ADC_CH_4  << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (B15     )),
    ADC3_CH5_C0                     = ((ADC_CH_5  << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (C0      )),
    ADC3_CH6_C14                    = ((ADC_CH_6  << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (C14     )),
    ADC3_CH7_C15                    = ((ADC_CH_7  << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (C15     )),
    ADC3_CH8_D0                     = ((ADC_CH_8  << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (D0      )),
    ADC3_CH9_C1                     = ((ADC_CH_9  << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (C1      )),
    ADC3_CH10_C2                    = ((ADC_CH_10 << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (C2      )),
    ADC3_CH11_B10                   = ((ADC_CH_11 << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (B10     )),
    ADC3_CH12_B11                   = ((ADC_CH_12 << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (B11     )),
    ADC3_CH15_B9                    = ((ADC_CH_15 << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (B9      )),
    ADC3_CH16_A15                   = ((ADC_CH_16 << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (A15     )),
    ADC3_CH17_B0                    = ((ADC_CH_17 << ADC_CHANNEL_OFFSET) | (ADC_3 << ADC_INDEX_OFFSET) | (B0      )),

    // ADC4 模块通道枚举
    ADC4_CH1_C3                     = ((ADC_CH_1  << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (C3      )),
    ADC4_CH2_C4                     = ((ADC_CH_2  << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (C4      )),
    ADC4_CH3_C6                     = ((ADC_CH_3  << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (C6      )),
    ADC4_CH4_C7                     = ((ADC_CH_4  << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (C7      )),
    ADC4_CH5_C8                     = ((ADC_CH_5  << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (C8      )),
    ADC4_CH6_D1                     = ((ADC_CH_6  << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (D1      )),
    ADC4_CH7_D2                     = ((ADC_CH_7  << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (D2      )),
    ADC4_CH8_E5                     = ((ADC_CH_8  << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (E5      )),
    ADC4_CH9_C1                     = ((ADC_CH_9  << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (C1      )),
    ADC4_CH10_C2                    = ((ADC_CH_10 << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (C2      )),
    ADC4_CH11_B5                    = ((ADC_CH_11 << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (B5      )),
    ADC4_CH12_B6                    = ((ADC_CH_12 << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (B6      )),
    ADC4_CH15_B4                    = ((ADC_CH_15 << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (B4      )),
    ADC4_CH16_B2                    = ((ADC_CH_16 << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (B2      )),
    ADC4_CH17_B3                    = ((ADC_CH_17 << ADC_CHANNEL_OFFSET) | (ADC_4 << ADC_INDEX_OFFSET) | (B3      )),

    // ADC5 模块通道枚举
    ADC5_CH1_C9                     = ((ADC_CH_1  << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (C9      )),
    ADC5_CH2_C10                    = ((ADC_CH_2  << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (C10     )),
    ADC5_CH3_C11                    = ((ADC_CH_3  << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (C11     )),
    ADC5_CH4_C12                    = ((ADC_CH_4  << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (C12     )),
    ADC5_CH5_C13                    = ((ADC_CH_5  << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (C13     )),
    ADC5_CH6_E6                     = ((ADC_CH_6  << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (E6      )),
    ADC5_CH7_E7                     = ((ADC_CH_7  << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (E7      )),
    ADC5_CH8_E8                     = ((ADC_CH_8  << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (E8      )),
    ADC5_CH9_C1                     = ((ADC_CH_9  << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (C1      )),
    ADC5_CH10_C2                    = ((ADC_CH_10 << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (C2      )),
    ADC5_CH16_B7                    = ((ADC_CH_16 << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (B7      )),
    ADC5_CH17_B8                    = ((ADC_CH_17 << ADC_CHANNEL_OFFSET) | (ADC_5 << ADC_INDEX_OFFSET) | (B8      )),
}zf_adc_channel_enum;

typedef enum                                                                    // 枚举 ADC 操作状态  此枚举定义不允许用户修改
{
    ADC_OPERATION_DONE              = ZF_NO_ERROR   ,                           // ADC 操作完成
    ADC_ERROR_UNKNOW                = ZF_ERROR      ,                           // ADC 未知错误 操作无法进行 正常情况下不会出现此错误

    ADC_WARNING_MODULE_DISABLE      = 0x10          ,                           // ADC 模块失能禁用 操作中断退出

    ADC_ERROR_MODULE_OCCUPIED       = 0x20          ,                           // ADC 模块已被占用 操作无法进行
    ADC_ERROR_MODULE_NOT_INIT                       ,                           // ADC 模块未初始化 操作无法进行

    ADC_ERROR_CHANNEL_OCCUPIED                      ,                           // ADC 通道已被占用 操作无法进行
    ADC_ERROR_CHANNEL_NOT_INIT                      ,                           // ADC 通道未初始化 操作无法进行

    ADC_ERROR_DATA_BUFFER_NULL                      ,                           // ADC 数据指针异常 操作无法进行
}zf_adc_operation_state_enum;

typedef struct                                                                  // ADC 管理对象模板 用于存储 ADC 的信息
{
    // 配置信息包括 通道状态 精度选择 使能 初始化信息等
    // 总配置信息所占位宽不限制

    // 需要使用指针指向对应模块寄存器

    ADC_TypeDef         *adc_ptr        ;                                       // 指向模块寄存器

    zf_gpio_pin_enum    pin_list[18]    ;                                       // 引脚列表

    union
    {
        uint32          config_info     ;                                       // 配置整体数据
        struct
        {
            uint32      channel     : 18;                                       // 通道使能

            uint32      init        : 1 ;                                       // 初始化
            uint32      enable      : 1 ;                                       // 使能
            uint32      resolution  : 2 ;                                       // 精度

            uint32      self_index  : 10;                                       // 自身索引
        };
    };
}zf_adc_obj_struct;

extern zf_adc_obj_struct adc_obj_list[ADC_NUM_MAX];
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
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
uint8 zf_adc_assert_handler (char *file, int line, char *tips);
#define zf_adc_assert(status)       ((status) ? (0) : zf_adc_assert_handler(__FILE__, __LINE__, NULL))
#define zf_adc_log(status, tips)    ((status) ? (0) : zf_adc_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 获取对应通道转换数据
// 参数说明     channel             选择 ADC 通道   (详见 zf_driver_adc.h 中枚举 zf_adc_channel_enum 定义)
// 参数说明     *data               ADC 转换存储地址指针 uint16 * 类型
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_get_convert_data(channel, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_get_convert_data (zf_adc_channel_enum channel, uint16 *data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 获取对应通道转换数据的平均值
// 参数说明     channel             选择 ADC 通道   (详见 zf_driver_adc.h 中枚举 zf_adc_channel_enum 定义)
// 参数说明     count               均值滤波次数
// 参数说明     *data               ADC 转换存储地址指针 uint16 * 类型
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_get_convert_average_data(channel, count, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_get_convert_average_data (zf_adc_channel_enum channel, const uint8 count, uint16 *data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 通道使能
// 参数说明     zf_adc_index           选择 ADC 模块   (详见 zf_driver_adc.h 中枚举 zf_adc_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_enable(zf_adc_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_enable (zf_adc_index_enum zf_adc_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 通道禁止
// 参数说明     zf_adc_index           选择 ADC 模块   (详见 zf_driver_adc.h 中枚举 zf_adc_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_disable(zf_adc_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_disable (zf_adc_index_enum zf_adc_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 精度设置
// 参数说明     zf_adc_index           选择 ADC 模块   (详见 zf_driver_adc.h 中枚举 zf_adc_index_enum 定义)
// 参数说明     resolution          选择分辨率      (详见 zf_driver_adc.h 中枚举 zf_adc_resolution_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_set_resolution(zf_adc_index, resolution);
// 备注信息     本函数一定会设置生效 即使没有通道初始化也能设置 会在首个通道初始化时生效
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_set_resolution (zf_adc_index_enum zf_adc_index, zf_adc_resolution_enum resolution);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 通道注销初始化
// 参数说明     channel             选择 ADC 通道   (详见 zf_driver_adc.h 中枚举 zf_adc_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_channel_deinit(channel);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_channel_deinit (zf_adc_channel_enum channel);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 模块注销初始化
// 参数说明     zf_adc_index           选择 ADC 模块   (详见 zf_driver_adc.h 中枚举 zf_adc_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_module_deinit(zf_adc_index);
// 备注信息     模块注销初始化会连带注销当前模块下所有通道
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_module_deinit (zf_adc_index_enum zf_adc_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 通道初始化
// 参数说明     channel             选择 ADC 通道   (详见 zf_driver_adc.h 中枚举 zf_adc_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_channel_init(channel);
// 备注信息     如果通道已经有一个通道初始化了 再初始化同一个通道的通道就会失败
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_channel_init (zf_adc_channel_enum channel);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 模块初始化
// 参数说明     zf_adc_index           选择 ADC 模块   (详见 zf_driver_adc.h 中枚举 zf_adc_index_enum 定义)
// 参数说明     resolution          选择分辨率      (详见 zf_driver_adc.h 中枚举 zf_adc_resolution_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ADC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_adc_module_init(zf_adc_index, resolution);
// 备注信息     本函数一定会设置生效 即使没有通道初始化也能设置 会在首个通道初始化时生效
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_adc_module_init (zf_adc_index_enum zf_adc_index, zf_adc_resolution_enum resolution);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
