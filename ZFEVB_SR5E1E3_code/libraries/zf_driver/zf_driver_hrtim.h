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

#ifndef _zf_driver_hrtim_h_
#define _zf_driver_hrtim_h_

// SDK 底层驱动
#include "hrtim.h"

// zf_common 层引用
#include "zf_common_typedef.h"

// zf_driver 层 类型定义 引用
#include "zf_driver_gpio.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_hrtim_set_duty                                                            // HRTIM PWM 更新占空比
// zf_hrtim_set_freq                                                            // HRTIM PWM 模块设置频率

// zf_hrtim_set_interrupt_callback                                              // HRTIM PIT 中断设置回调函数
// zf_hrtim_set_interrupt_config                                                // HRTIM PIT 中断设置优先级
// zf_hrtim_set_interrupt_priority                                              // HRTIM PIT 中断设置优先级

// zf_hrtim_pin_deinit                                                          // HRTIM PWM 引脚初始化
// zf_hrtim_module_deinit                                                       // HRTIM PWM 模块注销初始化

// zf_hrtim_complementary_init                                                  // HRTIM PWM 互补初始化
// zf_hrtim_channel_init                                                        // HRTIM PWM 通道初始化
// zf_hrtim_module_init                                                         // HRTIM PWM 模块初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 PWM 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define     HRTIM_DUTY_MAX              ( 10000 )                               // 定义 HRTIM 占空比最大值

#define     HRTIM_NUM_MAX               ( 2     )                               // 总共最多 HRTIM_NUM_MAX 个 HRTIM

typedef enum                                                                    // 枚举 HRTIM
{
    HRTIM_1     ,
    HRTIM_2     ,
}zf_hrtim_index_enum;

// 定义引脚要用的宏定义
// bit[11:0 ] 固定为引脚索引
// bit[15:12] 固定为引脚复用
// bit[19:16] 在 ENCODER 模块中为 ENCODER 索引
// bit[23:20] 在 ENCODER 模块中为 CHANNEL 索引

#define     HRTIM_PWM_PIN_INDEX_OFFSET  ( 0      )                              // bit[11:0 ] 存储 GPIO 的索引号
#define     HRTIM_PWM_PIN_INDEX_MASK    ( 0x0FFF )                              // 宽度 12bit 因此掩码为 0x0FFF

#define     HRTIM_PWM_PIN_AF_OFFSET     ( 12     )                              // bit[15:12] 存储 GPIO 的复用功能索引
#define     HRTIM_PWM_PIN_AF_MASK       ( 0x0F   )                              // 宽度 4bit 因此掩码为 0xF

#define     HRTIM_PWM_INDEX_OFFSET      ( 16     )                              // bit[19:16] 存储 PWM 索引
#define     HRTIM_PWM_INDEX_MASK        ( 0x0F   )                              // 宽度 4bit 因此掩码为 0xF

#define     HRTIM_PWM_CHANNEL_OFFSET    ( 20     )                              // bit[23:20] 存储 CHANNEL 索引
#define     HRTIM_PWM_CHANNEL_MASK      ( 0x0F   )                              // 宽度 4bit 因此掩码为 0xF

typedef enum                                                                    // 枚举 PWM 通道索引 此枚举定义不允许用户修改
{
    PWM_CHA1,   PWM_CHB1,   PWM_CHC1,   PWM_CHD1,   PWM_CHE1,   PWM_CHF1,       // 正向四个通道 占空比高电平生效
    PWM_CHA2,   PWM_CHB2,   PWM_CHC2,   PWM_CHD2,   PWM_CHE2,   PWM_CHF2,       // 反向四个通道 占空比低电平生效
}zf_hrtim_pwm_channel_index_enum;

typedef enum                                                                    // 枚举 HRTIM 正向 通道   此枚举定义不允许用户修改
{
    // ----------------------------------------------- HRTIM_1 PWM CHANNEL -----------------------------------------------
    HRTIM1_CHA1_E4      = ((PWM_CHA1 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (E4 )),
    HRTIM1_CHB1_E2      = ((PWM_CHB1 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (E2 )),
    HRTIM1_CHC1_D15     = ((PWM_CHC1 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (D15)),
    HRTIM1_CHD1_D13     = ((PWM_CHD1 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (D13)),
    HRTIM1_CHE1_D11     = ((PWM_CHE1 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (D11)),
    HRTIM1_CHF1_D8      = ((PWM_CHF1 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (D8 )),
    // ----------------------------------------------- HRTIM_1 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- HRTIM_2 PWM CHANNEL -----------------------------------------------
    HRTIM2_CHA1_E9      = ((PWM_CHA1 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_2 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (E9 )),
    HRTIM2_CHE1_F2      = ((PWM_CHE1 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_2 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (F2 )),
    // ----------------------------------------------- HRTIM_2 PWM CHANNEL -----------------------------------------------
}zf_hrtim_pwm_positive_channel_enum;

typedef enum                                                                    // 枚举 HRTIM 反向 通道   此枚举定义不允许用户修改
{
    // ----------------------------------------------- HRTIM_1 PWM CHANNEL -----------------------------------------------
    HRTIM1_CHA2_E3      = ((PWM_CHA2 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (E3 )),
    HRTIM1_CHB2_E1      = ((PWM_CHB2 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (E1 )),
    HRTIM1_CHC2_D14     = ((PWM_CHC2 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (D14)),
    HRTIM1_CHD2_D12     = ((PWM_CHD2 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (D12)),
    HRTIM1_CHE2_D10     = ((PWM_CHE2 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (D10)),
    HRTIM1_CHF2_D7      = ((PWM_CHF2 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_1 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (D7 )),
    // ----------------------------------------------- HRTIM_1 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- HRTIM_2 PWM CHANNEL -----------------------------------------------
    HRTIM2_CHA2_E10     = ((PWM_CHA2 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_2 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (E10)),
    HRTIM2_CHE2_F3      = ((PWM_CHE2 << HRTIM_PWM_CHANNEL_OFFSET) | (HRTIM_2 << HRTIM_PWM_INDEX_OFFSET) | (GPIO_AF3 << HRTIM_PWM_PIN_AF_OFFSET) | (F3 )),
    // ----------------------------------------------- HRTIM_2 PWM CHANNEL -----------------------------------------------
}zf_hrtim_pwm_negative_channel_enum;

#define     HRTIM_INTERRUPT_NUM_MAX     ( 7     )                               // 总共最多 HRTIM_NUM_MAX 个 HRTIM

typedef enum                                                                    // 枚举 HRTIM 中断索引 此枚举定义不允许用户修改
{
    HRTIM_INTERRUPT_INDEX_MAIN_UPDATE   ,                                       // 主定时器更新中断
    HRTIM_INTERRUPT_INDEX_CHA           ,                                       // 通道 A 中断
    HRTIM_INTERRUPT_INDEX_CHB           ,                                       // 通道 B 中断
    HRTIM_INTERRUPT_INDEX_CHC           ,                                       // 通道 C 中断
    HRTIM_INTERRUPT_INDEX_CHD           ,                                       // 通道 D 中断
    HRTIM_INTERRUPT_INDEX_CHE           ,                                       // 通道 E 中断
    HRTIM_INTERRUPT_INDEX_CHF           ,                                       // 通道 F 中断
}zf_hrtim_interrupt_index_enum;

typedef enum                                                                    // 枚举 HRTIM 中断配置 此枚举定义不允许用户修改
{
    HRTIM_INTERRUPT_CONFIG_TX_DISABLE  ,                                        // 禁止 发送中断
    HRTIM_INTERRUPT_CONFIG_RX_DISABLE  ,                                        // 禁止 接收中断
    HRTIM_INTERRUPT_CONFIG_ALL_DISABLE ,                                        // 禁止 所有中断

    HRTIM_INTERRUPT_CONFIG_TX_ENABLE   ,                                        // 使能 发送中断
    HRTIM_INTERRUPT_CONFIG_RX_ENABLE   ,                                        // 使能 接收中断
    HRTIM_INTERRUPT_CONFIG_ALL_ENABLE  ,                                        // 使能 所有中断
}zf_hrtim_interrupt_config_enum;

typedef enum                                                                    // 枚举 HRTIM 操作状态    此枚举定义不允许用户修改
{
    HRTIM_OPERATION_DONE                = ZF_NO_ERROR   ,                       // HRTIM 操作完成
    HRTIM_ERROR_UNKNOW                  = ZF_ERROR      ,                       // HRTIM 未知错误 操作无法进行 正常情况下不会出现此错误

    HRTIM_ERROR_MODULE_OCCUPIED         = 0x20          ,                       // HRTIM 模块已被占用 操作无法进行
    HRTIM_ERROR_MODULE_NOT_INIT                         ,                       // HRTIM 模块未初始化 操作无法进行

    HRTIM_ERROR_CHANNEL_OCCUPIED                        ,                       // HRTIM 通道已被占用 操作无法进行
    HRTIM_ERROR_CHANNEL_NOT_INIT                        ,                       // HRTIM 通道未初始化 操作无法进行
    HRTIM_ERROR_COMPLEMENTARY_NOT_INIT                  ,                       // HRTIM 互补未初始化 操作无法进行

    HRTIM_ERROR_MOUDLE_MISMATCH                         ,                       // HRTIM 引脚模块不对 操作无法进行
    HRTIM_ERROR_COMPLEMENTARY_MISMATCH                  ,                       // HRTIM 互补通道错误 操作无法进行
    HRTIM_ERROR_PIN_MISMATCH                            ,                       // HRTIM 引脚前后不对 操作无法进行

    HRTIM_ERROR_FREQ_ILLEGAL                            ,                       // HRTIM 输出频率异常 操作无法进行
    HRTIM_ERROR_DUTY_ILLEGAL                            ,                       // HRTIM 占空比值异常 操作无法进行

    HRTIM_ERROR_DEPENDS_TIMER_OCCUPIED                  ,                       // HRTIM 定时器被占用 操作无法进行
}zf_hrtim_operation_state_enum;

typedef struct                                                                  // HRTIM 管理对象模板 用于存储 PWM 的信息
{
    // 此处的结构固定为 引脚列表在前 配置信息在后
    // 根据不同的单片机的实现方式不一样 配置信息可能会有差异

    // 专门有 PWM 模块的单片机的配置信息写模块必要的参数
    // 使用定时器实现 PWM 的单片机配置信息指向对应定时器
    
    HRTIM_TypeDef           *hrtimer_obj        ;

    zf_gpio_pin_enum        pin_list[12]        ;

    uint64                  clock_input         ;
    uint32                  freq                ;
    uint32                  period              ;

    union
    {
        uint32              config_info         ;                               // 配置整体数据
        struct
        {
            uint32          channel_state   : 12;
            uint32          interrupt_state : 6 ;
            uint32          enable          : 2 ;
            uint32          self_index      : 6 ;
            uint32                          : 6 ;
        };
    };
}zf_hrtim_obj_struct;

extern zf_hrtim_obj_struct hrtim_obj_list[HRTIM_NUM_MAX];
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
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
uint8 zf_hrtim_assert_handler (char *file, int line, char *tips);
#define zf_hrtim_assert(status)     ((status) ? (0) : zf_hrtim_assert_handler(__FILE__, __LINE__, NULL))
#define zf_hrtim_log(status, tips)  ((status) ? (0) : zf_hrtim_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 更新占空比
// 参数说明     pin                 选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_positive_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_set_duty(pin, duty);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_set_duty (zf_hrtim_pwm_positive_channel_enum pin, const uint32 duty);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 模块设置频率
// 参数说明     hrtim_index         选择 HRTIM 模块     (详见 zf_driver_hrtim.h 内 zf_hrtim_index_enum 定义)
// 参数说明     freq                设置频率 同个模块只有最后一次设置生效
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_set_freq(hrtim_index, freq);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_set_freq (zf_hrtim_index_enum hrtim_index, const uint32 freq);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 注销互补初始化
// 参数说明     positive_pin        选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_positive_channel_enum 定义)
// 参数说明     negative_pin        选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_negative_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_complementary_deinit(positive_pin, negative_pin);
// 备注信息     必须先调用 zf_hrtim_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_complementary_deinit (zf_hrtim_pwm_positive_channel_enum positive_pin, zf_hrtim_pwm_negative_channel_enum negative_pin);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 通道销初始化
// 参数说明     pin                 选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_positive_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_channel_deinit(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_channel_deinit (zf_hrtim_pwm_positive_channel_enum pin);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 模块注销初始化
// 参数说明     hrtim_index         选择 HRTIM 模块     (详见 zf_driver_hrtim.h 内 zf_hrtim_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_module_deinit(hrtim_index);
// 备注信息     模块注销初始化会连带注销当前模块下所有引脚
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_module_deinit (zf_hrtim_index_enum hrtim_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 互补初始化
// 参数说明     positive_pin        选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_positive_channel_enum 定义)
// 参数说明     negative_pin        选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_negative_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_complementary_init(positive_pin, negative_pin, duty);
// 备注信息     必须先调用 zf_hrtim_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_complementary_init (zf_hrtim_pwm_positive_channel_enum positive_pin, zf_hrtim_pwm_negative_channel_enum negative_pin, const uint32 duty);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 通道初始化
// 参数说明     pin                 选择 HRTIM PWM 引脚 (详见 zf_driver_hrtim.h 内 zf_hrtim_pwm_positive_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_channel_init(pin, duty);
// 备注信息     必须先调用 zf_hrtim_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_channel_init (zf_hrtim_pwm_positive_channel_enum pin, const uint32 duty);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 模块初始化
// 参数说明     hrtim_index         选择 HRTIM 模块     (详见 zf_driver_hrtim.h 内 zf_hrtim_index_enum 定义)
// 参数说明     freq                设置频率 同个模块只有最后一次设置生效
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_hrtim_module_init(hrtim_index, freq);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_hrtim_module_init (zf_hrtim_index_enum hrtim_index, const uint32 freq);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
