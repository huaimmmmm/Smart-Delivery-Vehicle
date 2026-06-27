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

// zf_common 层引用
#include "zf_common_memory.h"

// 自身头文件
#include "zf_driver_interrupt.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION static uint32 interrupt_nest_count = 0;
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的外部重载函数 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     debug 保护处理 主要是防止断言后出现依旧进行操作的现象
// 参数说明     void
// 返回参数     void
// 使用示例     debug_interrupt_protective_handler();
// 备注信息     本函数在文件内部调用 用户不用关注
//              本函数由底层在 zf_driver_interrupt 中进行重载
//-------------------------------------------------------------------------------------------------------------------
void debug_interrupt_protective_handler (void)
{
    // 触发断言后需要理解关断所有中断 所以这里直接调用内核指令
    __ASM volatile("cpsid i");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     全局中断使能
// 参数说明     primask             当前屏蔽嵌套层数
// 返回参数     void
// 使用示例     zf_interrupt_global_enable(primask);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_interrupt_global_enable (uint32 primask)
{
    if(primask)
    {
        interrupt_nest_count = (interrupt_nest_count > 0) ? (interrupt_nest_count - 1) : (0);
    }
    if(!interrupt_nest_count)
    {
        __ASM volatile("cpsie i");
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     全局中断屏蔽
// 参数说明     void
// 返回参数     uint32              当前屏蔽嵌套层数
// 使用示例     primask = zf_interrupt_global_disable();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_interrupt_global_disable (void)
{
    if(!interrupt_nest_count)
    {
        __ASM volatile("cpsid i");
    }
    interrupt_nest_count ++;
    return interrupt_nest_count;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     确认指定中断使能状态
// 参数说明     irqn                指定中断号  (详见 zf_driver_interrupt.h 内 zf_interrupt_index_enum 定义)
// 返回参数     uint8               ZF_TRUE - 已使能 / ZF_FALSE - 未使能
// 使用示例     zf_interrupt_check(irqn);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_interrupt_check (zf_interrupt_index_enum irqn)
{
    return NVIC_GetEnableIRQ((IRQn_Type)irqn) ? ZF_TRUE : ZF_FALSE;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     指定中断使能
// 参数说明     irqn                指定中断号  (详见 zf_driver_interrupt.h 内 zf_interrupt_index_enum 定义)
// 返回参数     uint32          
// 使用示例     zf_interrupt_enable(irqn);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_interrupt_enable (zf_interrupt_index_enum irqn)
{
    NVIC_EnableIRQ((IRQn_Type)irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     指定中断屏蔽
// 参数说明     irqn                指定中断号  (详见 zf_driver_interrupt.h 内 zf_interrupt_index_enum 定义)
// 返回参数     void
// 使用示例     zf_interrupt_disable(irqn);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_interrupt_disable (zf_interrupt_index_enum irqn)
{
    NVIC_DisableIRQ((IRQn_Type)irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     指定中断获取优先级
// 参数说明     irqn                指定中断号  (详见 zf_driver_interrupt.h 内 zf_interrupt_index_enum 定义)
// 返回参数     uint32              当前优先级
// 使用示例     zf_interrupt_get_priority(irqn);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_interrupt_get_priority (zf_interrupt_index_enum irqn)
{
    return NVIC_GetPriority((IRQn_Type)irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     指定中断设置优先级
// 参数说明     irqn                指定中断号  (详见 zf_driver_interrupt.h 内 zf_interrupt_index_enum 定义)
// 参数说明     priority            中断优先级  (详见 zf_driver_interrupt.h 内 zf_interrupt_priority_enum 定义)
// 返回参数     void
// 使用示例     zf_interrupt_set_priority(irqn, priority);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_interrupt_set_priority (zf_interrupt_index_enum irqn, zf_interrupt_priority_enum priority)
{
    NVIC_SetPriority((IRQn_Type)irqn, priority);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     中断组初始化
// 参数说明     void
// 返回参数     void
// 使用示例     zf_interrupt_init();
// 备注信息     会在  clock_init 内部调用
//-------------------------------------------------------------------------------------------------------------------
void zf_interrupt_init (void)
{
    // M0/3/4   优先级 0-7      可分配 那么就是组 4 设置 3bit 抢占
    // M7       优先级 0-15     可分配 那么就是组 3 设置 4bit 抢占
    NVIC_SetPriorityGrouping(5 - (INTERRUPT_PRIORITY_LOW + 1) / 8);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
