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
* 文件名称          zf_device_key
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

/*********************************************************************************************************************
* 接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   // 一般是主板按键对应的引脚
*                   KEY1/S1             查看 zf_device_key.h 中 KEY_LIST[0]
*                   KEY2/S2             查看 zf_device_key.h 中 KEY_LIST[1]
*                   KEY3/S3             查看 zf_device_key.h 中 KEY_LIST[2]
*                   KEY4/S4             查看 zf_device_key.h 中 KEY_LIST[3]
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_key_h_
#define _zf_device_key_h_

// 如果移植到其他平台 这里就替换成自己的资源头文件
#include "zf_common_headfile.h"
#include "zf_driver_headfile.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// key_scanner                                                                  // KEY 按键状态扫描

// key_get_state                                                                // KEY 获取按键状态
// key_clear_state                                                              // KEY 对应按键处于空闲电平时清除触发状态
// key_clear_all_state                                                          // KEY 清除所有处于空闲电平的按键的触发状态

// key_init                                                                     // KEY 按键初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 KEY 相关的接口资源 这里允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// 定义按键引脚 用户可以新增可以修改 默认定义四个按键
// 如果用户可以新增按键 那么需要同步在下方 KEY_NUM_MAX / KEY_LIST / KEY_DEFAULT_STATE / key_index_enum 进行修改
#define KEY_NUM_MAX                 (  4        )                               // 最多 KEY_NUM_MAX 个 KEY
#define KEY_LIST                    { C7 , C8 , C9 , C10 }
#define KEY_DEFAULT_STATE           { KEY_RELEASE, KEY_RELEASE, KEY_RELEASE, KEY_RELEASE}

#define KEY_RELEASE_LEVEL           ( GPIO_HIGH )                               // 按键的默认状态 也就是按键释放状态的电平
#define KEY_MAX_SHOCK_PERIOD        ( 10        )                               // 按键消抖检测时长 单位毫秒 低于这个时长的信号会被认为是杂波抖动
#define KEY_LONG_PRESS_PERIOD       ( 1000      )                               // 最小长按时长 单位毫秒 高于这个时长的信号会被认为是长按动作

// 独立内存管理部分 默认使用 逐飞科技 开源库中的内存分段定义
// 如果移植到其它平台后此处报错 可以将 KEY_INTERFACE_USE_ZF_COMMON_MEMORY 修改为 0
// 当 KEY_INTERFACE_USE_ZF_COMMON_MEMORY 为 0 会禁止内存分段指定 通过编译器随机分配
#define     KEY_INTERFACE_USE_ZF_COMMON_MEMORY      ( 1 )
#if         KEY_INTERFACE_USE_ZF_COMMON_MEMORY
#ifndef _zf_common_memory_h_
#define AT_ZF_LIB_SECTION        
#define AT_ZF_LIB_SECTION_START  
#define AT_ZF_LIB_SECTION_END    
#endif
#else
#define AT_ZF_LIB_SECTION        
#define AT_ZF_LIB_SECTION_START  
#define AT_ZF_LIB_SECTION_END    
#endif

// 调试信息输出部分 默认使用 逐飞科技 开源库中的调试接口
// 如果移植到其它平台后此处报错 可以将 KEY_INTERFACE_USE_ZF_COMMON_DEBUG 修改为 0
// 当 KEY_INTERFACE_USE_ZF_COMMON_DEBUG 为 0 会通过 dl_assert 使用 printf 输出报错
#define     KEY_INTERFACE_USE_ZF_COMMON_DEBUG       ( 1 )
#if         KEY_INTERFACE_USE_ZF_COMMON_DEBUG
#ifndef _zf_common_debug_h_
#define zf_assert(pass)         ((pass) ? (0) : (1))
#define zf_log(pass, str)       ((pass) ? (0) : (1))
#endif
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 KEY 相关的控制对象格式
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef enum
{
    KEY_1       ,
    KEY_2       ,
    KEY_3       ,
    KEY_4       ,
}key_index_enum;

typedef enum
{
    KEY_RELEASE     ,                                                           // 按键释放状态
    KEY_SHORT_PRESS ,                                                           // 按键短按状态
    KEY_LONG_PRESS  ,                                                           // 按键长按状态
}key_state_enum;

typedef enum
{
    KEY_OPERATION_DONE                  = ZF_NO_ERROR   ,
    KEY_ERROR_UNKNOW_ERROR              = ZF_ERROR      ,

    KEY_ERROR_PERIOD_ILLEGAL            = 0x20          ,
    KEY_ERROR_NOT_INIT                                  ,
}key_operation_state_enum;

typedef struct
{
    zf_gpio_pin_enum    pin_list        [KEY_NUM_MAX]   ;                       // 引脚列表
    key_state_enum      key_state       [KEY_NUM_MAX]   ;
    uint32              key_press_time  [KEY_NUM_MAX]   ;
    uint32              scanner_period                  ;
}key_obj_struct;
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 按键状态扫描
// 参数说明     event               触发中断的事件
// 参数说明     *ptr                回调参数
// 返回参数     void
// 使用示例     key_scanner();
// 备注信息     这个函数跟 PIT 中断绑定即可 或者自己周期调用传入 0 和 NULL
//-------------------------------------------------------------------------------------------------------------------
void key_scanner (uint32 event, void *ptr);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 获取按键状态
// 参数说明     key_index           按键索引
// 返回参数     key_state_enum      按键状态
// 使用示例     key_get_state(key_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
key_state_enum key_get_state (key_index_enum key_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 获取所有按键状态
// 参数说明     *state              按键状态缓冲
// 返回参数     uint8               操作状态 ZF_NO_ERROR / KEY_OPERATION_DONE - 完成 其余值为异常
// 使用示例     key_get_all_state(state);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 key_get_all_state (key_state_enum *state);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 对应按键处于空闲电平时清除触发状态
// 参数说明     key_index           按键索引
// 返回参数     uint8               操作状态 ZF_NO_ERROR / KEY_OPERATION_DONE - 完成 其余值为异常
// 使用示例     key_clear_state(key_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 key_clear_state (key_index_enum key_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 清除所有处于空闲电平的按键的触发状态
// 参数说明     void                无
// 返回参数     uint8               操作状态 ZF_NO_ERROR / KEY_OPERATION_DONE - 完成 其余值为异常
// 使用示例     key_clear_all_state();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 key_clear_all_state (void);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 按键初始化
// 参数说明     period              按键扫描周期 以毫秒为单位
// 返回参数     uint8               操作状态 ZF_NO_ERROR / KEY_OPERATION_DONE - 完成 其余值为异常
// 使用示例     key_init(period);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 key_init (uint32 period);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
