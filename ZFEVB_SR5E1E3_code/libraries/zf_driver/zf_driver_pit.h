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

#ifndef _zf_driver_pit_h_
#define _zf_driver_pit_h_

// SDK 底层驱动
#include "tim.h"

// zf_common 层引用
#include "zf_common_typedef.h"

// zf_driver 层 类型定义 引用   这里引用其他同层文件是因为本芯片的 PIT 接口是由其他接口组合模拟实现的
#include "zf_driver_timer.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_pit_set_interrupt_callback                                                // PIT 中断设置回调函数
// zf_pit_set_interrupt_priority                                                // PIT 中断设置优先级

// zf_pit_enable                                                                // PIT 中断使能
// zf_pit_disable                                                               // PIT 中断禁止

// zf_pit_deinit                                                                // PIT 注销初始化
// zf_pit_init                                                                  // PIT 初始化 一般调用 pit_ms_init 或 pit_us_init
// zf_pit_ms_init                                                               // PIT 中断初始化 ms 周期
// zf_pit_us_init                                                               // PIT 中断初始化 us 周期
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 PIT 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define PIT_NUM_MAX     ( 10 )                                                  // 最多 PIT_NUM_MAX 个 PIT

typedef enum                                                                    // 枚举 PIT
{
    PIT_TIM1 ,  PIT_TIM8 ,
    PIT_TIM2 ,  PIT_TIM5 ,
    PIT_TIM3 ,  PIT_TIM4 ,
    PIT_TIM15,  PIT_TIM16,
    PIT_TIM6 ,  PIT_TIM7 ,
}zf_pit_index_enum;

typedef enum                                                                    // 枚举 PIT 操作状态    此枚举定义不允许用户修改
{
    PIT_OPERATION_DONE              = ZF_NO_ERROR   ,                           // PIT 操作完成
    PIT_ERROR_UNKNOW                = ZF_ERROR      ,                           // PIT 未知错误 操作无法进行 正常情况下不会出现此错误

    PIT_ERROR_MODULE_OCCUPIED       = 0x20          ,                           // PIT 模块已被占用 操作无法进行
    PIT_ERROR_MODULE_NOT_INIT                       ,                           // PIT 模块未初始化 操作无法进行

    PIT_ERROR_PERIOD_ILLEGAL                        ,                           // PIT 中断周期异常 操作无法进行

    PIT_ERROR_INTERRUPT_CALLBACK_ILLEGAL            ,                           // PIT 中断回调异常 操作无法进行

    PIT_ERROR_DEPENDS_TIMER_OCCUPIED                ,                           // PIT 定时器被占用 操作无法进行
}zf_pit_operation_state_enum;

typedef struct                                                                  // PIT 管理对象模板 用于存储 PIT 的信息
{
    // 此处的结构固定为 配置参数 寄存器或定时器对象指针
    // 根据不同的单片机的实现方式不一样 配置信息可能会有差异

    // 专门有 周期中断 模块的单片机的配置信息写模块必要的参数
    // 使用定时器实现 周期中断 的单片机配置信息指向对应定时器

    // 需要使用指针指向对应模块寄存器
    // 不过此处是使用 TIMER 实现 所以是指向 TIMER 对象

    zf_timer_obj_struct     *timer_obj              ;

    uint32                  period_system_cycles    ;
}zf_pit_obj_struct;

extern zf_pit_obj_struct pit_obj_list[PIT_NUM_MAX];
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define zf_pit_assert(status)       ((status) ? (0) : zf_timer_assert_handler(__FILE__, __LINE__, NULL))
#define zf_pit_log(status, tips)    ((status) ? (0) : zf_timer_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断设置回调函数
// 参数说明     pit_index           PIT 外设模块号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 参数说明     callback            回调函数 为无返回值 uint32 ptr 两个参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     uint8               操作状态 ZF_NO_ERROR / PIT_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pit_set_interrupt_callback(pit_index, callback, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pit_set_interrupt_callback (zf_pit_index_enum pit_index, void_callback_uint32_ptr callback, void *ptr);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断设置优先级
// 参数说明     pit_index           PIT 外设模块号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 参数说明     priority            中断优先级
// 返回参数     void
// 使用示例     zf_pit_set_interrupt_priority(pit_index, priority);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_pit_set_interrupt_priority (zf_pit_index_enum pit_index, uint8 priority);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断使能
// 参数说明     pit_index           PIT 外设模块号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / PIT_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pit_enable(pit_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pit_enable (zf_pit_index_enum pit_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断禁止
// 参数说明     pit_index           PIT 外设模块号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / PIT_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pit_disable(pit_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pit_disable (zf_pit_index_enum pit_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 注销初始化
// 参数说明     pit_index           PIT 外设模块号 (详见 zf_driver_pit.h 内 zf_pit_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / PIT_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_pit_deinit(pit_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_pit_deinit (zf_pit_index_enum pit_index);

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
uint8 zf_pit_init (zf_pit_index_enum pit_index, uint32 period, void_callback_uint32_ptr callback, void *ptr);

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
uint8 zf_pit_ms_init (zf_pit_index_enum pit_index, uint32 period_ms, void_callback_uint32_ptr callback, void *ptr);

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
uint8 zf_pit_us_init (zf_pit_index_enum pit_index, uint32 period_us, void_callback_uint32_ptr callback, void *ptr);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
