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

#ifndef _zf_driver_encoder_h_
#define _zf_driver_encoder_h_

// SDK 底层驱动
#include "tim.h"

// zf_common 层引用
#include "zf_common_typedef.h"

// zf_driver 层 类型定义 引用   这里引用其他同层文件是因为本芯片的 ENCODER 接口是由其他接口组合模拟实现的
#include "zf_driver_timer.h"
#include "zf_driver_gpio.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_encoder_get_count                                                         // ENCODER 接口获取编码器计数
// zf_encoder_clear_count                                                       // ENCODER 清空编码器计数

// zf_encoder_deinit                                                            // ENCODER 接口注销初始化
// zf_encoder_init                                                              // ENCODER 接口初始化 带方向编码器使用
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 ENCODER 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define     ENCODER_NUM_MAX             ( 6     )                               // 总共最多 ENCODER_NUM_MAX 个 ENCODER

typedef enum                                                                    // 枚举 ENCODER 此枚举定义不允许用户修改
{
    ENCODER_TIM8 = 1,
    ENCODER_TIM2 ,  ENCODER_TIM5 ,
    ENCODER_TIM3 ,  ENCODER_TIM4 ,
}zf_encoder_index_enum;

typedef enum                                                                    // 枚举 ENCODER 信号模式 此枚举定义不允许用户修改
{
    ENCODER_MODE_QUADRATURE = 1 ,                                               // 正交编码器
    ENCODER_MODE_DIR            ,                                               // 带方向增量编码器 也就是所谓的 <兼容正交编码的编码器>
}zf_encoder_mode_enum;

typedef enum                                                                    // 枚举 ENCODER 通道索引 此枚举定义不允许用户修改
{
    ENCODER_A_PLUS              ,                                               // 用于区分引脚功能 用户不必关心
    ENCODER_B_DIR               ,                                               // 用于区分引脚功能 用户不必关心
}zf_encoder_channel_index_enum;

// 定义引脚要用的宏定义
// bit[11:0 ] 固定为引脚索引
// bit[15:12] 固定为引脚复用
// bit[19:16] 在 ENCODER 模块中为 ENCODER 索引
// bit[23:20] 在 ENCODER 模块中为 CHANNEL 索引

#define     ENCODER_PIN_INDEX_OFFSET    ( 0      )                              // bit[11:0 ] 存储 GPIO 的索引号
#define     ENCODER_PIN_INDEX_MASK      ( 0x0FFF )                              // 宽度 12bit 因此掩码为 0x0FFF

#define     ENCODER_PIN_AF_OFFSET       ( 12     )                              // bit[15:12] 存储 GPIO 的复用功能索引
#define     ENCODER_PIN_AF_MASK         ( 0x0F   )                              // 宽度 4bit 因此掩码为 0xF

#define     ENCODER_INDEX_OFFSET        ( 16     )                              // bit[19:16] 存储 ENCODER 索引
#define     ENCODER_INDEX_MASK          ( 0x0F   )                              // 宽度 4bit 因此掩码为 0xF

#define     ENCODER_CHANNEL_OFFSET      ( 20     )                              // bit[23:20] 存储 CHANNEL 索引
#define     ENCODER_CHANNEL_MASK        ( 0x0F   )                              // 宽度 4bit 因此掩码为 0xF

typedef enum                                                                    // 枚举 ENCOEDER 通道 此枚举定义不允许用户修改
{
    // ENCODER_TIM8  <正交解码 A 相> 或 <增量方向 PLUS> 引脚 具体的复用值通过芯片手册查看确定
    ENCODER_TIM8_A_PLUS_I0  = ((ENCODER_A_PLUS << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM8  << ENCODER_INDEX_OFFSET) | (GPIO_AF4  << ENCODER_PIN_AF_OFFSET) | (I0 )),

    // ENCODER_TIM2  <正交解码 A 相> 或 <增量方向 PLUS> 引脚 具体的复用值通过芯片手册查看确定
    ENCODER_TIM2_A_PLUS_D14 = ((ENCODER_A_PLUS << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM2  << ENCODER_INDEX_OFFSET) | (GPIO_AF1  << ENCODER_PIN_AF_OFFSET) | (D14)),

    // ENCODER_TIM5  <正交解码 A 相> 或 <增量方向 PLUS> 引脚 具体的复用值通过芯片手册查看确定
    ENCODER_TIM5_A_PLUS_G5  = ((ENCODER_A_PLUS << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM5  << ENCODER_INDEX_OFFSET) | (GPIO_AF1  << ENCODER_PIN_AF_OFFSET) | (G5  )),
    ENCODER_TIM5_A_PLUS_I1  = ((ENCODER_A_PLUS << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM5  << ENCODER_INDEX_OFFSET) | (GPIO_AF1  << ENCODER_PIN_AF_OFFSET) | (I1  )),

    // ENCODER_TIM3  <正交解码 A 相> 或 <增量方向 PLUS> 引脚 具体的复用值通过芯片手册查看确定
    ENCODER_TIM3_A_PLUS_E4  = ((ENCODER_A_PLUS << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM3  << ENCODER_INDEX_OFFSET) | (GPIO_AF2  << ENCODER_PIN_AF_OFFSET) | (E4 )),
    ENCODER_TIM3_A_PLUS_G14 = ((ENCODER_A_PLUS << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM3  << ENCODER_INDEX_OFFSET) | (GPIO_AF2  << ENCODER_PIN_AF_OFFSET) | (G14)),

    // ENCODER_TIM4  <正交解码 A 相> 或 <增量方向 PLUS> 引脚 具体的复用值通过芯片手册查看确定
    ENCODER_TIM4_A_PLUS_F2  = ((ENCODER_A_PLUS << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM4  << ENCODER_INDEX_OFFSET) | (GPIO_AF2  << ENCODER_PIN_AF_OFFSET) | (F2 )),
    ENCODER_TIM4_A_PLUS_H12 = ((ENCODER_A_PLUS << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM4  << ENCODER_INDEX_OFFSET) | (GPIO_AF2  << ENCODER_PIN_AF_OFFSET) | (H12)),

}zf_encoder_channel_a_plus_enum;

typedef enum                                                                    // 枚举 ENCOEDER 通道    此枚举定义不允许用户修改
{
    // ENCODER_TIM8  <正交解码 B 相> 或 <增量方向 DIR > 引脚 具体的复用值通过芯片手册查看确定
    ENCODER_TIM8_B_DIR_I2   = ((ENCODER_B_DIR  << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM8  << ENCODER_INDEX_OFFSET) | (GPIO_AF4  << ENCODER_PIN_AF_OFFSET) | (I2 )),

    // ENCODER_TIM2  <正交解码 B 相> 或 <增量方向 DIR > 引脚 具体的复用值通过芯片手册查看确定
    ENCODER_TIM2_B_DIR_D15  = ((ENCODER_B_DIR  << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM2  << ENCODER_INDEX_OFFSET) | (GPIO_AF1  << ENCODER_PIN_AF_OFFSET) | (D15)),

    // ENCODER_TIM5  <正交解码 B 相> 或 <增量方向 DIR > 引脚 具体的复用值通过芯片手册查看确定
    ENCODER_TIM5_B_DIR_G6   = ((ENCODER_B_DIR  << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM5  << ENCODER_INDEX_OFFSET) | (GPIO_AF1  << ENCODER_PIN_AF_OFFSET) | (G6  )),
    ENCODER_TIM5_B_DIR_I3   = ((ENCODER_B_DIR  << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM5  << ENCODER_INDEX_OFFSET) | (GPIO_AF1  << ENCODER_PIN_AF_OFFSET) | (I3  )),

    // ENCODER_TIM3  <正交解码 B 相> 或 <增量方向 DIR > 引脚 具体的复用值通过芯片手册查看确定
    ENCODER_TIM3_B_DIR_E2   = ((ENCODER_B_DIR  << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM3  << ENCODER_INDEX_OFFSET) | (GPIO_AF2  << ENCODER_PIN_AF_OFFSET) | (E2 )),
    ENCODER_TIM3_B_DIR_G15  = ((ENCODER_B_DIR  << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM3  << ENCODER_INDEX_OFFSET) | (GPIO_AF2  << ENCODER_PIN_AF_OFFSET) | (G15)),

    // ENCODER_TIM4  <正交解码 B 相> 或 <增量方向 DIR > 引脚 具体的复用值通过芯片手册查看确定
    ENCODER_TIM4_B_DIR_F3   = ((ENCODER_B_DIR  << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM4  << ENCODER_INDEX_OFFSET) | (GPIO_AF2  << ENCODER_PIN_AF_OFFSET) | (F3 )),
    ENCODER_TIM4_B_DIR_H13  = ((ENCODER_B_DIR  << ENCODER_CHANNEL_OFFSET) | (ENCODER_TIM4  << ENCODER_INDEX_OFFSET) | (GPIO_AF2  << ENCODER_PIN_AF_OFFSET) | (H13)),

}zf_encoder_channel_b_dir_enum;

typedef enum                                                                    // 枚举 ENCODER 操作状态    此枚举定义不允许用户修改
{
    ENCODER_OPERATION_DONE                  = ZF_NO_ERROR   ,                   // ENCODER 操作完成
    ENCODER_ERROR_UNKNOW                    = ZF_ERROR      ,                   // ENCODER 未知错误 操作无法进行 正常情况下不会出现此错误

    ENCODER_ERROR_MODULE_OCCUPIED           = 0x20          ,                   // ENCODER 模块已被占用 操作无法进行
    ENCODER_ERROR_MODULE_NOT_INIT                           ,                   // ENCODER 模块未初始化 操作无法进行

    ENCODER_ERROR_A_PLUS_PIN_MISMATCH                       ,                   // ENCODER 引脚不匹配 操作无法进行
    ENCODER_ERROR_B_DIR_PIN_MISMATCH                        ,                   // ENCODER 引脚不匹配 操作无法进行

    ENCODER_ERROR_DATA_BUFFER_NULL                          ,                   // ENCODER 数据指针异常 操作无法进行

    ENCODER_ERROR_DEPENDS_TIMER_OCCUPIED                    ,                   // ENCODER 定时器被占用 操作无法进行
}zf_encoder_operation_state_enum;

typedef struct                                                                  // ENCODER 管理对象模板 用于存储 ENCODER 的信息
{
    // 此处的结构固定为 引脚列表在前 配置信息在后
    // 根据不同的单片机的实现方式不一样 配置信息可能会有差异

    // 专门有 编码器 模块的单片机的配置信息写模块必要的参数
    // 使用定时器实现 编码器 的单片机配置信息指向对应定时器

    // 需要使用指针指向对应模块寄存器
    // 不过此处是使用 TIMER 实现 所以是指向 TIMER 对象

    zf_timer_obj_struct *timer_obj  ;                                           // 指向定时器模块 因为编码器靠定时器实现

    zf_gpio_pin_enum    a_plus_pin  ;                                           // 存储 <正交解码 A 相> 或 <增量方向 PLUS> 引脚
    zf_gpio_pin_enum    b_dir_pin   ;                                           // 存储 <正交解码 B 相> 或 <增量方向 DIR > 引脚

    uint32              mode        ;                                           // 当前编码器模块的工作模式
}zf_encoder_obj_struct;

extern zf_encoder_obj_struct encoder_obj_list[ENCODER_NUM_MAX];
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define zf_encoder_assert(status)       ((status) ? (0) : zf_timer_assert_handler(__FILE__, __LINE__, NULL))
#define zf_encoder_log(status, tips)    ((status) ? (0) : zf_timer_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ENCODER 接口获取编码器计数
// 参数说明     encoder_index       ENCODER 模块号  (详见 zf_driver_encoder.h 内 zf_encoder_index_enum 定义)
// 参数说明     *data               数据读取地址 int16 * 类型指针
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_encoder_get_count(encoder_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_encoder_get_count (zf_encoder_index_enum encoder_index, int16 *data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ENCODER 清空编码器计数
// 参数说明     encoder_index       ENCODER 模块号  (详见 zf_driver_encoder.h 内 zf_encoder_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_encoder_clear_count(encoder_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_encoder_clear_count (zf_encoder_index_enum encoder_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ENCODER 接口注销初始化
// 参数说明     encoder_index       ENCODER 模块号  (详见 zf_driver_encoder.h 内 zf_encoder_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / ENCODER_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_encoder_deinit(encoder_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_encoder_deinit (zf_encoder_index_enum encoder_index);

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
uint8 zf_encoder_init (zf_encoder_index_enum encoder_index, zf_encoder_mode_enum mode, zf_encoder_channel_a_plus_enum a_plus_pin, zf_encoder_channel_b_dir_enum b_dir_pin);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
