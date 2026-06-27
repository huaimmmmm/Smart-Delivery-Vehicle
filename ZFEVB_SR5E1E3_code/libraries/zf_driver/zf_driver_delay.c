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
* 文件名称          zf_driver_delay
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

// 自身头文件
#include "zf_driver_delay.h"

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DELAY 延时函数 ms 级别
// 参数说明     time                需要延时的时间 ms 单位
// 返回参数     void
// 使用示例     zf_delay_ms(time);
// 备注信息     本函数使用弱定义 这样做的目的是在移植操作系统时可以重载此函数
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK void zf_delay_ms (uint32 time)
{
    vuint32 ms_temp         = 0;

    vuint32 us_temp1        = 0;
    vuint32 us_temp2        = 0;

    vuint32 counter_temp1   = 0;
    vuint32 counter_temp2   = 0;

    while(time)
    {
        ms_temp = (time >= 10) ? (10) : (time);                                 // 延时以 10ms 为切片分段
        time -= ms_temp;                                                        // 不足 10ms 就一次延时完成了 超过的就切片递减

        do                                                                      // 通过两个 volatile 的变量来确保寄存器数值读取正确
        {                                                                       // 一般情况下这两句执行时间会非常短 他们的数值应当是一致的
            counter_temp1 = DRV_TIM_TS.tim_ts->CNT;                             // volatile 变量读取寄存器数值
            counter_temp2 = DRV_TIM_TS.tim_ts->CNT;                             // volatile 变量读取寄存器数值
        }while(counter_temp1 != counter_temp2);                                 // 但不排除遇到计数器重置 或者中断打断操作

        us_temp1 = counter_temp1 + ms_temp * 1000;                              // 计算延时后的时间戳
        us_temp2 = us_temp1 % 50000;                                            // 计算延时后计数范围内的时间戳

        for(;;)
        {
            do                                                                  // 通过两个 volatile 的变量来确保寄存器数值读取正确
            {                                                                   // 一般情况下这两句执行时间会非常短 他们的数值应当是一致的
                counter_temp1 = DRV_TIM_TS.tim_ts->CNT;                         // volatile 变量读取寄存器数值
                counter_temp2 = DRV_TIM_TS.tim_ts->CNT;                         // volatile 变量读取寄存器数值
            }while(counter_temp1 != counter_temp2);                             // 但不排除遇到计数器重置 或者中断打断操作

            if(us_temp1 >= 50000)                                               // 判断是否超出范围
            {                                                                   // 超出计数值周期 就等下一个周期
                if(counter_temp1 < 50000 / 2 && counter_temp1 > us_temp2)       // 等到下个周期前半周再判断是否满足延时退出
                {
                    break;
                }
            }
            else
            {
                if(50000 - us_temp2 <= 100)
                {
                    if(counter_temp1 < 50000 / 2)
                    {
                        break;
                    }
                }
                else
                {
                    if(counter_temp1 > us_temp2)
                    {
                        break;
                    }
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DELAY 延时函数 us 级别
// 参数说明     time                需要延时的时间 us 单位 范围 1-10000
// 返回参数     void
// 使用示例     zf_delay_us(time);
// 备注信息     受限于程序运行跳转 此延时会比输入值高出一些
//              本函数使用弱定义 这样做的目的是在移植操作系统时可以重载此函数
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK void zf_delay_us (uint32 time)
{
    vuint32 us_temp         = 0;

    vuint32 us_temp1        = 0;
    vuint32 us_temp2        = 0;

    vuint32 counter_temp1   = 0;
    vuint32 counter_temp2   = 0;

    us_temp = (10000 <= time) ? (10000) : (time);                               // 限制最长的延时为 10000us 也就是 10ms 超过这个值就该用毫秒延时

    do                                                                          // 通过两个 volatile 的变量来确保寄存器数值读取正确
    {                                                                           // 一般情况下这两句执行时间会非常短 他们的数值应当是一致的
        counter_temp1 = DRV_TIM_TS.tim_ts->CNT;                                 // volatile 变量读取寄存器数值
        counter_temp2 = DRV_TIM_TS.tim_ts->CNT;                                 // volatile 变量读取寄存器数值
    }while(counter_temp1 != counter_temp2);                                     // 但不排除遇到计数器重置 或者中断打断操作

    us_temp1 = counter_temp1 + us_temp;                                         // 计算延时后的时间戳
    us_temp2 = us_temp1 % 50000;                                                // 计算延时后计数范围内的时间戳

    for(;;)
    {
        do                                                                      // 通过两个 volatile 的变量来确保寄存器数值读取正确
        {                                                                       // 一般情况下这两句执行时间会非常短 他们的数值应当是一致的
            counter_temp1 = DRV_TIM_TS.tim_ts->CNT;                             // volatile 变量读取寄存器数值
            counter_temp2 = DRV_TIM_TS.tim_ts->CNT;                             // volatile 变量读取寄存器数值
        }while(counter_temp1 != counter_temp2);                                 // 但不排除遇到计数器重置 或者中断打断操作

        if(us_temp1 >= 50000)                                                   // 判断是否超出范围
        {                                                                       // 超出计数值周期 就等下一个周期
            if(counter_temp1 < 50000 / 2 && counter_temp1 > us_temp2)           // 等到下个周期前半周再判断是否满足延时退出
            {
                break;
            }
        }
        else
        {
            if(50000 - us_temp2 <= 100)
            {
                if(counter_temp1 < 50000 / 2)
                {
                    break;
                }
            }
            else
            {
                if(counter_temp1 > us_temp2)
                {
                    break;
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DELAY 初始化
// 参数说明     void
// 返回参数     void
// 使用示例     zf_delay_init();
// 备注信息     本函数由 system_clock_init 内部调用 用户不需要关心 也不需要调用
//              本函数使用弱定义 这样做的目的是在移植操作系统时可以重载此函数
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK void zf_delay_init (void)
{
    uint16 freq_div     = (150 - 1);                                            // 预分频 1MHz 计数
    uint16 period_temp  = (50000 - 1);                                          // 自动重装载值 50ms

    tim_ts_init(&DRV_TIM_TS);                                                   // 初始化 TS 实例参数

    tim_ts_enable_dithering(&DRV_TIM_TS, false);                                // 禁止抖动功能
    tim_ts_enable_uif_remapping(&DRV_TIM_TS, false);                            // 禁止路由
    tim_ts_enable_autoreload_preload(&DRV_TIM_TS, true);                        // 使能自动重装载
    tim_ts_enable_one_pulse_mode(&DRV_TIM_TS, false);                           // 禁止单次脉冲模式
    tim_ts_set_source_ofuf_only(&DRV_TIM_TS, false);                            // 禁止更新请求事件
    tim_ts_enable_update_event_generation(&DRV_TIM_TS, true);                   // 允许更新事件
    tim_ts_set_master_mode_selection(&DRV_TIM_TS, TIM_TS_MMS_UPDATE);           // 配置模式
    tim_ts_set_prescaler(&DRV_TIM_TS, freq_div);                                // 设置分频
    tim_ts_set_autoreload(&DRV_TIM_TS, period_temp);                            // 设置周期计数值
    tim_ts_start(&DRV_TIM_TS);                                                  // 启动模块
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
