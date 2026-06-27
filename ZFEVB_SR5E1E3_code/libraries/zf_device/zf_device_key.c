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

// 自身头文件
#include "zf_device_key.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION key_obj_struct key_default_obj = 
{
    .pin_list       = KEY_LIST,
    .key_state      = KEY_DEFAULT_STATE,
    .key_press_time = KEY_DEFAULT_STATE,
    .scanner_period = 0
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     key_assert_handler(file, line, tips);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#if KEY_INTERFACE_USE_ZF_COMMON_DEBUG
#define key_assert(pass)        zf_assert(pass)
#define key_log(pass, tips)     zf_log(pass, tips)
#else
static uint8 key_assert_handler (char *file, int line, char *tips)
{
    printf("\r\n GNSS OPERATION ASSERT!\r\n");
    printf("\r\n file: %s.\r\n", file);
    printf("\r\n line: %d.\r\n", line);
    if(NULL != tips)
    {
        printf("\r\n tips: %s.\r\n", tips);
    }
    while(1);
    return ZF_ERROR;
}
#define key_assert(pass)        ((pass) ? (0) : key_assert_handler(__FILE__, __LINE__, NULL))
#define key_log(pass, tips)     ((pass) ? (0) : key_assert_handler(__FILE__, __LINE__, (tips)))
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 按键状态扫描
// 参数说明     event               触发中断的事件
// 参数说明     *ptr                回调参数
// 返回参数     void
// 使用示例     key_scanner();
// 备注信息     这个函数跟 PIT 中断绑定即可 或者自己周期调用传入 0 和 NULL
//-------------------------------------------------------------------------------------------------------------------
void key_scanner (uint32 event, void *ptr)
{
    (void)event;
    (void)ptr;

    do
    {
        if(key_assert(0 != key_default_obj.scanner_period))
        {
            break;
        }

        // 按键的检测逻辑为
        // 按键短按释放后触发 长按超过预设时长后触发
        // 短按和长按状态触发后保持状态需要手动清空状态 ( 避免程序不能及时获取导致被刷新 )
        for(uint8 i = 0; KEY_NUM_MAX > i; i ++)
        {
            if(KEY_RELEASE_LEVEL != zf_gpio_get_level(key_default_obj.pin_list[i]))// 按键按下
            {
                key_default_obj.key_press_time[i] ++;                           // 进行按键消抖计数
                if(     KEY_LONG_PRESS_PERIOD / key_default_obj.scanner_period  // 进行长按判断
                    <=  key_default_obj.key_press_time[i])                      // 如果触发次数超过长按的计数
                {
                    key_default_obj.key_state[i] = KEY_LONG_PRESS;              // 那么就会一直更新为长按状态
                }
            }
            else                                                                // 按键释放
            {
                if((    KEY_LONG_PRESS != key_default_obj.key_state[i])         // 如果是按键释放 且不在长按状态
                    &&  (   KEY_MAX_SHOCK_PERIOD / key_default_obj.scanner_period// 判断是否满足短按的消抖次数
                        <=  key_default_obj.key_press_time[i]))                 // 至少要大于等于短按的预设时长
                {
                    key_default_obj.key_state[i] = KEY_SHORT_PRESS;             // 释放后置位短按的标志
                }
                else if(KEY_MAX_SHOCK_PERIOD / key_default_obj.scanner_period   // 如果是单纯按键释放
                    >   key_default_obj.key_press_time[i])                      // 不满足短按的时长
                {
                    key_default_obj.key_state[i] = KEY_RELEASE;                 // 就标记为默认释放状态
                }
            }
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 获取按键状态
// 参数说明     key_index           按键索引
// 返回参数     key_state_enum      按键状态
// 使用示例     key_get_state(key_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
key_state_enum key_get_state (key_index_enum key_index)
{
    key_state_enum return_value = KEY_RELEASE;

    do
    {
        if(key_assert(0 != key_default_obj.scanner_period))
        {
            break;
        }

        return_value = key_default_obj.key_state[key_index];
    }while(0);

    return return_value;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 获取所有按键状态
// 参数说明     *state              按键状态缓冲
// 返回参数     uint8               操作状态 ZF_NO_ERROR / KEY_OPERATION_DONE - 完成 其余值为异常
// 使用示例     key_get_all_state(state);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 key_get_all_state (key_state_enum *state)
{
    key_operation_state_enum return_state = KEY_ERROR_UNKNOW_ERROR;

    do
    {
        if(key_assert(0 != key_default_obj.scanner_period))
        {
            return_state = KEY_ERROR_NOT_INIT;
            break;
        }

        memcpy(state, key_default_obj.key_state, sizeof(key_default_obj.key_state));

        return_state = KEY_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 对应按键处于空闲电平时清除触发状态
// 参数说明     key_index           按键索引
// 返回参数     uint8               操作状态 ZF_NO_ERROR / KEY_OPERATION_DONE - 完成 其余值为异常
// 使用示例     key_clear_state(key_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 key_clear_state (key_index_enum key_index)
{
    key_operation_state_enum return_state = KEY_ERROR_UNKNOW_ERROR;

    do
    {
        if(key_assert(0 != key_default_obj.scanner_period))
        {
            return_state = KEY_ERROR_NOT_INIT;
            break;
        }

        key_default_obj.key_state[key_index] = KEY_RELEASE;
        if(KEY_RELEASE_LEVEL == zf_gpio_get_level(key_default_obj.pin_list[key_index]))
        {
            key_default_obj.key_press_time[key_index] = 0;
        }

        return_state = KEY_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 清除所有处于空闲电平的按键的触发状态
// 参数说明     void                无
// 返回参数     uint8               操作状态 ZF_NO_ERROR / KEY_OPERATION_DONE - 完成 其余值为异常
// 使用示例     key_clear_all_state();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 key_clear_all_state (void)
{
    key_operation_state_enum return_state = KEY_ERROR_UNKNOW_ERROR;

    do
    {
        if(key_assert(0 != key_default_obj.scanner_period))
        {
            return_state = KEY_ERROR_NOT_INIT;
            break;
        }

        for(uint8 i = 0; KEY_NUM_MAX > i; i ++)
        {
            key_default_obj.key_state[i] = KEY_RELEASE;
            if(KEY_RELEASE_LEVEL == zf_gpio_get_level(key_default_obj.pin_list[i]))
            {
                key_default_obj.key_press_time[i] = 0;
            }
        }

        return_state = KEY_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     KEY 按键初始化
// 参数说明     period              按键扫描周期 以毫秒为单位
// 返回参数     uint8               操作状态 ZF_NO_ERROR / KEY_OPERATION_DONE - 完成 其余值为异常
// 使用示例     key_init(period);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 key_init (uint32 period)
{
    key_operation_state_enum return_state = KEY_ERROR_UNKNOW_ERROR;

    do
    {
        if(key_assert(0 < period))
        {
            return_state = KEY_ERROR_PERIOD_ILLEGAL;
            break;
        }

        for(uint8 loop_temp = 0; KEY_NUM_MAX > loop_temp; loop_temp ++)
        {
            zf_gpio_init(key_default_obj.pin_list[loop_temp], GPI_PULL_UP, GPIO_HIGH);
            key_default_obj.key_state[loop_temp] = KEY_RELEASE;
        }
        key_default_obj.scanner_period = period;

        return_state = KEY_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
