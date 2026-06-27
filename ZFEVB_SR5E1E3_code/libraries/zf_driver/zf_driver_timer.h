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
* 文件名称          zf_driver_timer
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

#ifndef _zf_driver_timer_h_
#define _zf_driver_timer_h_

// SDK 底层驱动
#include "tim.h"

// zf_common 层引用
#include "zf_common_typedef.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_timer_funciton_check                                                      // TIMER 外设确认功能状态 库内部调用
// zf_timer_clock_enable                                                        // TIMER 时钟使能 库内部调用

// zf_timer_enable                                                              // TIMER 开始计时
// zf_timer_disable                                                             // TIMER 停止计时

// zf_timer_get                                                                 // TIMER 获取计时器计数
// zf_timer_clear                                                               // TIMER 清除计时器计数

// zf_timer_deinit                                                              // TIMER 注销初始化
// zf_timer_init                                                                // TIMER 计时器初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 TIM 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define     TIM_NUM_MAX             (  10    )                                  // 总共最多 12 个定时器

typedef enum
{
    TIM_1 , TIM_8 ,                                                             // 这两个放一排是因为它们同个类型 16 位高级定时器
    TIM_2 , TIM_5 ,                                                             // 这两个放一排是因为它们同个类型 32 位通用定时器
    TIM_3 , TIM_4 ,                                                             // 这两个放一排是因为它们同个类型 16 位通用定时器
    TIM_15, TIM_16,                                                             // 这两个放一排是因为它们同个类型 16 位高级定时器 只有两通道
    TIM_6 , TIM_7 ,                                                             // 这两个放一排是因为它们同个类型 16 位基本定时器
}zf_timer_index_enum;

typedef enum
{
    TIMER_SYSTEM_CLOCK,                                                         // 系统频率计时 最大 0xFFFF
    TIMER_US,                                                                   // 微秒计时 最大 0xFFFF
    TIMER_MS,                                                                   // 毫秒计时 最大 0xFFFF / 10
}zf_timer_mode_enum;

typedef enum
{
    TIMER_FUNCTION_INIT     = 0 ,                                               // 功能未初始化
    TIMER_FUNCTION_TIMER        ,                                               // 用作 TIMER 计时
    TIMER_FUNCTION_PIT          ,                                               // 用作 PIT
    TIMER_FUNCTION_PWM          ,                                               // 用作 PWM
    TIMER_FUNCTION_ENCODER      ,                                               // 用作 ENCODER
    TIMER_FUNCTION_CAMERA       ,                                               // 用作 CAMERA
}zf_timer_function_enum;

typedef enum
{
    TIM_ETR_RISING      ,
    TIM_ETR_FALLING     ,
}zf_timer_etr_mode_emun;

typedef enum                                                                    // 枚举 TIM 操作状态    此枚举定义不允许用户修改
{
    TIM_OPERATION_DONE                      = ZF_NO_ERROR   ,                   // TIM 操作完成
    TIM_ERROR_UNKNOW                        = ZF_ERROR      ,                   // TIM 未知错误 操作无法进行 正常情况下不会出现此错误

    TIM_ERROR_MODULE_OCCUPIED               = 0x20          ,                   // TIM 模块已被占用 操作无法进行
    TIM_ERROR_MODULE_NOT_INIT                               ,                   // TIM 模块未初始化 操作无法进行

    TIM_ERROR_DATA_BUFFER_NULL                              ,                   // TIM 数据指针异常 操作无法进行
}zf_timer_operation_state_enum;

typedef struct                                                                  // TIMER 管理对象模板 用于存储 TIMER 的信息
{
    TIM_TypeDef         *tim_ptr        ;
    uint32              clock_input     ;

    uint32              div             ;
    uint32              period          ;

    union
    {
        uint32          config_info     ;                                       // 配置整体数据
        struct
        {
            uint32      function    : 4 ;                                       // 功能
            uint32      mode        : 2 ;                                       // 计数模式

            uint32                  : 26;                                       // 保留
        };
    };
}zf_timer_obj_struct;

extern zf_timer_obj_struct timer_obj_list[TIM_NUM_MAX];
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_assert_handler(file, line, tips);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_assert_handler (char *file, int line, char *tips);
#define zf_timer_assert(status)     ((status) ? (0) : zf_timer_assert_handler(__FILE__, __LINE__, NULL))
#define zf_timer_log(status, tips)  ((status) ? (0) : zf_timer_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 外设确认功能状态 库内部调用
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 参数说明     mode                功能模块        (详见 zf_driver_timer.h 内 zf_timer_function_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_funciton_check(index, mode);
// 备注信息     本函数是提供给其他基于 TIM 模块实现的功能模块进行互斥控制的
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_funciton_check (zf_timer_index_enum timer_index, zf_timer_function_enum mode);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 时钟使能 库内部调用
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 返回参数     void
// 使用示例     zf_timer_clock_enable(timer_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_timer_clock_enable (zf_timer_index_enum timer_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 开始计时
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_enable(timer_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_enable (zf_timer_index_enum timer_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 停止计时
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_disable(timer_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_disable (zf_timer_index_enum timer_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 获取计时器计数
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint16              计数值
// 使用示例     zf_timer_get(timer_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_get (zf_timer_index_enum timer_index, uint16 *data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 清除计时器计数
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_clear(timer_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_clear (zf_timer_index_enum timer_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 注销初始化
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_deinit(timer_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_deinit (zf_timer_index_enum timer_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 计时器初始化
// 参数说明     timer_index         TIMER 模块号    (详见 zf_driver_timer.h 内 zf_timer_index_enum 定义)
// 参数说明     mode                计时方式        (详见 zf_driver_timer.h 内 zf_timer_mode_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / TIM_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_timer_init(timer_index, mode);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_timer_init (zf_timer_index_enum timer_index, zf_timer_mode_enum mode);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
