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

// 自身头文件
#include "zf_driver_rcc.h"

// 此处定义 本文件用使用的模块管理数据 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
__IO uint32* const rcc_reset_register [] = RCC_RESET_REGISTER_LIST ;
__IO uint32* const rcc_enable_register[] = RCC_ENABLE_REGISTER_LIST;
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     外设模块复位
// 参数说明     rcc_index           外设时钟索引    (详见 zf_driver_rcc.h 内 zf_rcc_index_enum 定义)
// 返回参数     void
// 使用示例     zf_rcc_peripheral_reset(rcc_index);
//-------------------------------------------------------------------------------------------------------------------
void zf_rcc_peripheral_reset (zf_rcc_index_enum rcc_index)
{
    *rcc_reset_register[rcc_index & RCC_BUS_INDEX_MASK] |= (0x00000001 << ((rcc_index >> RCC_PERIPHERAL_INDEX_OFFSET) & RCC_PERIPHERAL_INDEX_MASK));
    *rcc_reset_register[rcc_index & RCC_BUS_INDEX_MASK] &= ~(0x00000001 << ((rcc_index >> RCC_PERIPHERAL_INDEX_OFFSET) & RCC_PERIPHERAL_INDEX_MASK));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     外设时钟状态检查
// 参数说明     rcc_index           外设时钟索引    (详见 zf_driver_rcc.h 内 zf_rcc_index_enum 定义)
// 返回参数     uint8               外设时钟状态
// 使用示例     zf_rcc_clock_check(rcc_index);
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_rcc_clock_check (zf_rcc_index_enum rcc_index)
{
    return (*rcc_enable_register[rcc_index & RCC_BUS_INDEX_MASK] & (0x00000001 << ((rcc_index >> RCC_PERIPHERAL_INDEX_OFFSET) & RCC_PERIPHERAL_INDEX_MASK))) ? (1) : (0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     外设时钟禁止
// 参数说明     rcc_index           外设时钟索引    (详见 zf_driver_rcc.h 内 zf_rcc_index_enum 定义)
// 返回参数     void
// 使用示例     zf_rcc_clock_disable(rcc_index);
//-------------------------------------------------------------------------------------------------------------------
void zf_rcc_clock_disable (zf_rcc_index_enum rcc_index)
{
    *rcc_enable_register[rcc_index & RCC_BUS_INDEX_MASK] &= ~(0x00000001 << ((rcc_index >> RCC_PERIPHERAL_INDEX_OFFSET) & RCC_PERIPHERAL_INDEX_MASK));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     外设时钟使能
// 参数说明     rcc_index           外设时钟索引    (详见 zf_driver_rcc.h 内 zf_rcc_index_enum 定义)
// 返回参数     void
// 使用示例     zf_rcc_clock_enable(rcc_index);
//-------------------------------------------------------------------------------------------------------------------
void zf_rcc_clock_enable (zf_rcc_index_enum rcc_index)
{
    *rcc_enable_register[rcc_index & RCC_BUS_INDEX_MASK] |= (0x00000001 << ((rcc_index >> RCC_PERIPHERAL_INDEX_OFFSET) & RCC_PERIPHERAL_INDEX_MASK));
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
