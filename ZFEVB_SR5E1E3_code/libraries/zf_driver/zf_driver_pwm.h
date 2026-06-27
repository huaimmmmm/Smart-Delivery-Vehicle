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
* 文件名称          zf_driver_pwm
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

#ifndef _zf_driver_pwm_h_
#define _zf_driver_pwm_h_

// SDK 底层驱动
#include "tim.h"

// zf_common 层引用
#include "zf_common_typedef.h"

// zf_driver 层 类型定义 引用   这里引用其他同层文件是因为本芯片的 DVP 接口是由其他接口组合模拟实现的
#include "zf_driver_timer.h"
#include "zf_driver_gpio.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_pwm_set_duty                                                              // PWM 更新占空比
// zf_pwm_set_freq                                                              // PWM 模块设置频率

// zf_pwm_pin_deinit                                                            // PWM 引脚初始化
// zf_pwm_module_deinit                                                         // PWM 模块注销初始化

// zf_pwm_complementary_init                                                    // PWM 互补初始化
// zf_pwm_channel_init                                                          // PWM 通道初始化
// zf_pwm_module_init                                                           // PWM 模块初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 PWM 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define     PWM_DUTY_MAX            ( 10000 )                                   // 定义 PWM 占空比最大值

#define     PWM_NUM_MAX             ( 8     )                                   // 总共最多 PWM_NUM_MAX 个 PWM

typedef enum                                                                    // 枚举 PWM
{
    PWM_TIM1    ,   PWM_TIM8    ,
    PWM_TIM2    ,   PWM_TIM5    ,
    PWM_TIM3    ,   PWM_TIM4    ,
    PWM_TIM15   ,   PWM_TIM16   ,
}zf_pwm_index_enum;

typedef enum                                                                    // 枚举 PWM 对齐模式 此枚举定义不允许用户修改
{
    PWM_ALIGNMENT_EDGE          ,
    PWM_ALIGNMENT_CENTER        ,
}zf_pwm_alignment_enum;

// 定义引脚要用的宏定义
// bit[11:0 ] 固定为引脚索引
// bit[15:12] 固定为引脚复用
// bit[19:16] 在 ENCODER 模块中为 ENCODER 索引
// bit[23:20] 在 ENCODER 模块中为 CHANNEL 索引

#define     PWM_PIN_INDEX_OFFSET    ( 0      )                                  // bit[11:0 ] 存储 GPIO 的索引号
#define     PWM_PIN_INDEX_MASK      ( 0x0FFF )                                  // 宽度 12bit 因此掩码为 0x0FFF

#define     PWM_PIN_AF_OFFSET       ( 12     )                                  // bit[15:12] 存储 GPIO 的复用功能索引
#define     PWM_PIN_AF_MASK         ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0xF

#define     PWM_INDEX_OFFSET        ( 16     )                                  // bit[19:16] 存储 PWM 索引
#define     PWM_INDEX_MASK          ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0xF

#define     PWM_CHANNEL_OFFSET      ( 20     )                                  // bit[23:20] 存储 CHANNEL 索引
#define     PWM_CHANNEL_MASK        ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0xF

typedef enum                                                                    // 枚举 PWM 通道索引 此枚举定义不允许用户修改
{
    PWM_CH1 ,   PWM_CH2 ,   PWM_CH3 ,   PWM_CH4 ,                               // 正向四个通道 占空比高电平生效
    PWM_CH1N,   PWM_CH2N,   PWM_CH3N,   PWM_CH4N,                               // 反向四个通道 占空比低电平生效
}zf_pwm_channel_index_enum;

typedef enum                                                                    // 枚举 PWM 正向 通道   此枚举定义不允许用户修改
{
    // ----------------------------------------------- TIM8 PWM CHANNEL -----------------------------------------------
    PWM_TIM8_CH1_I0         = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (I0 )),

    PWM_TIM8_CH2_I2         = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (I2 )),

    PWM_TIM8_CH3_I4         = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (I4 )),
    // ----------------------------------------------- TIM8 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- TIM2 PWM CHANNEL -----------------------------------------------
    PWM_TIM2_CH1_D14        = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM2  << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (D14)),

    PWM_TIM2_CH2_D15        = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM2  << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (D15)),

    PWM_TIM2_CH3_E1         = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM2  << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (E1 )),
    // ----------------------------------------------- TIM2 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- TIM5 PWM CHANNEL -----------------------------------------------
    PWM_TIM5_CH1_G5         = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM5  << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (G5 )),
    PWM_TIM5_CH1_I1         = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM5  << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (I1 )),

    PWM_TIM5_CH2_G6         = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM5  << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (G6 )),
    PWM_TIM5_CH2_I3         = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM5  << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (I3 )),

    PWM_TIM5_CH3_G7         = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM5  << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (G7 )),

    PWM_TIM5_CH4_G8         = ((PWM_CH4  << PWM_CHANNEL_OFFSET) | (PWM_TIM5  << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (G8 )),
    // ----------------------------------------------- TIM5 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- TIM3 PWM CHANNEL -----------------------------------------------
    PWM_TIM3_CH1_E4         = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM3  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (E4 )),
    PWM_TIM3_CH1_G14        = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM3  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (G14)),

    PWM_TIM3_CH2_E2         = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM3  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (E2 )),
    PWM_TIM3_CH2_G15        = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM3  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (G15)),

    PWM_TIM3_CH3_A4         = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM3  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (A4 )),
    PWM_TIM3_CH3_H0         = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM3  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (H0 )),

    PWM_TIM3_CH4_A5         = ((PWM_CH4  << PWM_CHANNEL_OFFSET) | (PWM_TIM3  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (A5 )),
    PWM_TIM3_CH4_E3         = ((PWM_CH4  << PWM_CHANNEL_OFFSET) | (PWM_TIM3  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (E3 )),
    PWM_TIM3_CH4_H1         = ((PWM_CH4  << PWM_CHANNEL_OFFSET) | (PWM_TIM3  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (H1 )),
    // ----------------------------------------------- TIM3 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- TIM4 PWM CHANNEL -----------------------------------------------
    PWM_TIM4_CH1_F2         = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM4  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (F2 )),
    PWM_TIM4_CH1_H12        = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM4  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (H12)),

    PWM_TIM4_CH2_F3         = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM4  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (F3 )),
    PWM_TIM4_CH2_H13        = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM4  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (H13)),
    // ----------------------------------------------- TIM4 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- TIM16 PWM CHANNEL -----------------------------------------------
    PWM_TIM16_CH1_E8        = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM16 << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (E8 )),
    PWM_TIM16_CH1_I5        = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM16 << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (I5 )),
    // ----------------------------------------------- TIM16 PWM CHANNEL -----------------------------------------------
}zf_pwm_positive_channel_enum;

typedef enum                                                                    // 枚举 PWM 反向 通道   此枚举定义不允许用户修改
{
    // ----------------------------------------------- TIM8 PWM CHANNEL -----------------------------------------------
    PWM_TIM8_CH1N_I1        = ((PWM_CH1N << PWM_CHANNEL_OFFSET) | (PWM_TIM8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (I1 )),

    PWM_TIM8_CH2N_I3        = ((PWM_CH2N << PWM_CHANNEL_OFFSET) | (PWM_TIM8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (I3 )),

    PWM_TIM8_CH3N_I5        = ((PWM_CH3N << PWM_CHANNEL_OFFSET) | (PWM_TIM8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (I5 )),
    // ----------------------------------------------- TIM8 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- TIM16 PWM CHANNEL -----------------------------------------------
    PWM_TIM16_CH1N_E9       = ((PWM_CH1N << PWM_CHANNEL_OFFSET) | (PWM_TIM16 << PWM_INDEX_OFFSET) | (GPIO_AF1  << PWM_PIN_AF_OFFSET) | (E9 )),
    // ----------------------------------------------- TIM16 PWM CHANNEL -----------------------------------------------
}zf_pwm_negative_channel_enum;

typedef enum                                                                    // 枚举 PWM 操作状态    此枚举定义不允许用户修改
{
    PWM_OPERATION_DONE              = ZF_NO_ERROR   ,                           // PWM 操作完成
    PWM_ERROR_UNKNOW                = ZF_ERROR      ,                           // PWM 未知错误 操作无法进行 正常情况下不会出现此错误

    PWM_ERROR_MODULE_OCCUPIED       = 0x20          ,                           // PWM 模块已被占用 操作无法进行
    PWM_ERROR_MODULE_NOT_INIT                       ,                           // PWM 模块未初始化 操作无法进行

    PWM_ERROR_CHANNEL_OCCUPIED                      ,                           // PWM 通道已被占用 操作无法进行
    PWM_ERROR_CHANNEL_NOT_INIT                      ,                           // PWM 通道未初始化 操作无法进行
    PWM_ERROR_COMPLEMENTARY_NOT_INIT                ,                           // PWM 互补未初始化 操作无法进行

    PWM_ERROR_MOUDLE_MISMATCH                       ,                           // PWM 引脚模块不对 操作无法进行
    PWM_ERROR_COMPLEMENTARY_MISMATCH                ,                           // PWM 互补通道错误 操作无法进行
    PWM_ERROR_PIN_MISMATCH                          ,                           // PWM 引脚前后不对 操作无法进行

    PWM_ERROR_FREQ_ILLEGAL                          ,                           // PWM 输出频率异常 操作无法进行
    PWM_ERROR_DUTY_ILLEGAL                          ,                           // PWM 占空比值异常 操作无法进行

    PWM_ERROR_DEPENDS_TIMER_OCCUPIED                ,                           // PWM 定时器被占用 操作无法进行
}zf_pwm_operation_state_enum;

typedef struct                                                                  // PWM 管理对象模板 用于存储 PWM 的信息
{
    // 此处的结构固定为 引脚列表在前 配置信息在后
    // 根据不同的单片机的实现方式不一样 配置信息可能会有差异

    // 专门有 PWM 模块的单片机的配置信息写模块必要的参数
    // 使用定时器实现 PWM 的单片机配置信息指向对应定时器

    // 需要使用指针指向对应模块寄存器
    // 不过此处是使用 TIMER 实现 所以是指向 TIMER 对象
    
    zf_timer_obj_struct *timer_obj          ;

    zf_gpio_pin_enum    pin_list[8]         ;

    uint32              freq                ;

    union
    {
        uint32          config_info         ;                                   // 配置整体数据
        struct
        {
            uint32      channel_state   : 8 ;
            uint32      enable          : 1 ;
            uint32      alignment       : 1 ;
            uint32      self_index      : 6 ;
            uint32                      : 16;
        };
    };
}zf_pwm_obj_struct;

extern zf_pwm_obj_struct pwm_obj_list[PWM_NUM_MAX];
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define zf_pwm_assert(status)       ((status) ? (0) : zf_timer_assert_handler(__FILE__, __LINE__, NULL))
#define zf_pwm_log(status, tips)    ((status) ? (0) : zf_timer_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 更新占空比
// 参数说明     pin                 选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_positive_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_set_duty(pin, duty);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_set_duty (zf_pwm_positive_channel_enum pin, const uint32 duty);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 模块设置频率
// 参数说明     pwm_index           选择 PWM 模块   (详见 zf_driver_pwm.h 内 zf_pwm_index_enum 定义)
// 参数说明     freq                设置频率 同个模块只有最后一次设置生效
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_set_freq(pwm_index, freq);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_set_freq (zf_pwm_index_enum pwm_index, const uint32 freq);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 注销互补初始化
// 参数说明     positive_pin        选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_positive_channel_enum 定义)
// 参数说明     negative_pin        选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_negative_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_complementary_deinit(positive_pin, negative_pin);
// 备注信息     必须先调用 zf_pwm_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_complementary_deinit (zf_pwm_positive_channel_enum positive_pin, zf_pwm_negative_channel_enum negative_pin);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 通道销初始化
// 参数说明     pin                 选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_positive_channel_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_channel_deinit(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_channel_deinit (zf_pwm_positive_channel_enum pin);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 模块注销初始化
// 参数说明     pwm_index           选择 PWM 模块   (详见 zf_driver_pwm.h 内 zf_pwm_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_module_deinit(pwm_index);
// 备注信息     模块注销初始化会连带注销当前模块下所有引脚
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_module_deinit (zf_pwm_index_enum pwm_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 互补初始化
// 参数说明     positive_pin        选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_positive_channel_enum 定义)
// 参数说明     negative_pin        选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_negative_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_complementary_init(positive_pin, negative_pin, duty);
// 备注信息     必须先调用 zf_pwm_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_complementary_init (zf_pwm_positive_channel_enum positive_pin, zf_pwm_negative_channel_enum negative_pin, const uint32 duty);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 通道初始化
// 参数说明     pin                 选择 PWM 引脚   (详见 zf_driver_pwm.h 内 zf_pwm_positive_channel_enum 定义)
// 参数说明     duty                设置占空比
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_channel_init(pin, duty);
// 备注信息     必须先调用 zf_pwm_module_init 先初始化模块
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_channel_init (zf_pwm_positive_channel_enum pin, const uint32 duty);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 模块初始化
// 参数说明     pwm_index           选择 PWM 模块   (详见 zf_driver_pwm.h 内 zf_pwm_index_enum 定义)
// 参数说明     mode                PWM 对齐模式    (详见 zf_driver_pwm.h 内 zf_pwm_alignment_enum 定义)
// 参数说明     freq                设置频率 同个模块只有最后一次设置生效
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pwm_module_init(pwm_index, mode, freq);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pwm_module_init (zf_pwm_index_enum pwm_index, zf_pwm_alignment_enum mode, const uint32 freq);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
