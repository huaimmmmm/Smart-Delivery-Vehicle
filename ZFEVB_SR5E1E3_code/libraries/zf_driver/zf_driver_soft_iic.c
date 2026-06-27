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
* 文件名称          zf_driver_soft_iic
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

// zf_common 层引用
#include "zf_common_debug.h"
#include "zf_common_memory.h"

// zf_driver 层引用
#include "zf_driver_gpio.h"

// 自身头文件
#include "zf_driver_soft_iic.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
AT_ZF_LIB_SECTION_START
// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION zf_soft_iic_obj_struct soft_iic_obj_list[SOFT_IIC_MAX] =
{
    {.scl_pin = PIN_NULL, .sda_pin = PIN_NULL, .delay = 0, .enable = 0},
    {.scl_pin = PIN_NULL, .sda_pin = PIN_NULL, .delay = 0, .enable = 0},
    {.scl_pin = PIN_NULL, .sda_pin = PIN_NULL, .delay = 0, .enable = 0},
    {.scl_pin = PIN_NULL, .sda_pin = PIN_NULL, .delay = 0, .enable = 0},
    {.scl_pin = PIN_NULL, .sda_pin = PIN_NULL, .delay = 0, .enable = 0},
    {.scl_pin = PIN_NULL, .sda_pin = PIN_NULL, .delay = 0, .enable = 0},
    {.scl_pin = PIN_NULL, .sda_pin = PIN_NULL, .delay = 0, .enable = 0},
    {.scl_pin = PIN_NULL, .sda_pin = PIN_NULL, .delay = 0, .enable = 0}
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define SOFT_IIC_SDA_IO_SWITCH          (0)                                     // 是否需要 SDA 进行 I/O 切换 0-不需要 1-需要

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 设置 SDA 为输入
// 参数说明     io                  引脚
// 返回参数     void
// 使用示例     zf_soft_iic_io_in(soft_iic_obj_list[soft_iic_index].sda_pin);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
#define zf_soft_iic_io_in(io)       (zf_gpio_set_dir((zf_gpio_pin_enum)io, GPI_FLOATING_IN))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 设置 SDA 为输出
// 参数说明     io                  引脚
// 返回参数     void
// 使用示例     zf_soft_iic_io_out(soft_iic_obj_list[soft_iic_index].sda_pin);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
#define zf_soft_iic_io_out(io)      (zf_gpio_set_dir((zf_gpio_pin_enum)io, GPO_OPEN_DTAIN))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 设置引脚电平为高
// 参数说明     io                  引脚
// 返回参数     void
// 使用示例     zf_soft_iic_io_high(soft_iic_obj_list[soft_iic_index].scl_pin);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
#define zf_soft_iic_io_high(io)     (zf_gpio_high((zf_gpio_pin_enum)io))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 设置引脚电平为低
// 参数说明     io                  引脚
// 返回参数     void
// 使用示例     zf_soft_iic_io_low(soft_iic_obj_list[soft_iic_index].scl_pin);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
#define zf_soft_iic_io_low(io)      (zf_gpio_low ((zf_gpio_pin_enum)io))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 延时
// 参数说明     delay               延时次数
// 返回参数     void
// 使用示例     zf_soft_iic_delay(1);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
//static void zf_soft_iic_delay (vuint32 delay)
//{
//    volatile uint32 count = delay;
//    while(count --);
//}
#define zf_soft_iic_delay(x)  for(vuint32 i = x; i --; )

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_assert_handler(file, line, tips);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC START 信号
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 返回参数     void
// 使用示例     zf_soft_iic_start(soft_iic_index);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
ZF_INLINE void zf_soft_iic_start (zf_soft_iic_index_enum soft_iic_index)
{
    do
    {
        if(ZF_DISABLE == soft_iic_obj_list[soft_iic_index].enable)
        {
            break;
        }
        zf_soft_iic_io_high (soft_iic_obj_list[soft_iic_index].scl_pin);        // SCL 高电平
        zf_soft_iic_io_high (soft_iic_obj_list[soft_iic_index].sda_pin);        // SDA 高电平

        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
        zf_soft_iic_io_low  (soft_iic_obj_list[soft_iic_index].sda_pin);        // SDA 先拉低
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
        zf_soft_iic_io_low  (soft_iic_obj_list[soft_iic_index].scl_pin);        // SCL 再拉低
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC STOP 信号
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 返回参数     void
// 使用示例     zf_soft_iic_stop(soft_iic_index);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
ZF_INLINE void zf_soft_iic_stop (zf_soft_iic_index_enum soft_iic_index)
{
    do
    {
        if(ZF_DISABLE == soft_iic_obj_list[soft_iic_index].enable)
        {
            break;
        }
        zf_soft_iic_io_low  (soft_iic_obj_list[soft_iic_index].sda_pin);        // SDA 低电平
        zf_soft_iic_io_low  (soft_iic_obj_list[soft_iic_index].scl_pin);        // SCL 低电平

        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
        zf_soft_iic_io_high (soft_iic_obj_list[soft_iic_index].scl_pin);        // SCL 先拉高
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
        zf_soft_iic_io_high (soft_iic_obj_list[soft_iic_index].sda_pin);        // SDA 再拉高
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 发送 ACK/NAKC 信号 内部调用
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     ack                 ACK 电平 表示的是响应电平 而不是是否发送 ACK 信号
// 返回参数     void
// 使用示例     zf_soft_iic_send_ack(soft_iic_index, 1);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
ZF_INLINE void zf_soft_iic_send_ack (zf_soft_iic_index_enum soft_iic_index, uint8 ack)
{
    do
    {
        if(ack)
        {
            zf_soft_iic_io_high(soft_iic_obj_list[soft_iic_index].sda_pin);     // SDA 拉高
        }
        else
        {
            zf_soft_iic_io_low(soft_iic_obj_list[soft_iic_index].sda_pin);      // SDA 拉低
        }
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
        zf_soft_iic_io_high (soft_iic_obj_list[soft_iic_index].scl_pin);        // SCL 拉高
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
        zf_soft_iic_io_low  (soft_iic_obj_list[soft_iic_index].scl_pin);        // SCL 拉低
        zf_soft_iic_io_high (soft_iic_obj_list[soft_iic_index].sda_pin);        // SDA 拉高
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 获取 ACK/NAKC 信号
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 返回参数     uint8               ACK 状态 表示的是响应电平 而不是是否收到 ACK 信号
// 使用示例     zf_soft_iic_wait_ack(soft_iic_index);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
ZF_INLINE uint8 zf_soft_iic_wait_ack (zf_soft_iic_index_enum soft_iic_index)
{
    uint8 temp = 1;
    do
    {
        zf_soft_iic_io_high (soft_iic_obj_list[soft_iic_index].sda_pin);        // SDA 高电平 释放 SDA
#if SOFT_IIC_SDA_IO_SWITCH
        zf_soft_iic_io_in   (soft_iic_obj_list[soft_iic_index].sda_pin);
#endif
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);

        zf_soft_iic_io_high (soft_iic_obj_list[soft_iic_index].scl_pin);        // SCL 高电平
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
        zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);

        temp = zf_gpio_get_level(soft_iic_obj_list[soft_iic_index].sda_pin);
        zf_soft_iic_io_low  (soft_iic_obj_list[soft_iic_index].scl_pin);        // SCL 低电平
#if SOFT_IIC_SDA_IO_SWITCH
        zf_soft_iic_io_out  (soft_iic_obj_list[soft_iic_index].sda_pin);
#endif
    }while(0);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 发送 8bit 数据
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     data                数据
// 返回参数     uint8               ACK 状态
// 使用示例     zf_soft_iic_send_data(soft_iic_index, data);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
ZF_INLINE uint8 zf_soft_iic_send_data (zf_soft_iic_index_enum soft_iic_index, const uint8 data)
{
    uint8 temp = 1;
    do
    {
        if(ZF_DISABLE == soft_iic_obj_list[soft_iic_index].enable)
        {
            break;
        }
        temp = 0x80;
        while(temp)
        {
            if(data & temp)
            {
                zf_soft_iic_io_high (soft_iic_obj_list[soft_iic_index].sda_pin);// SDA 拉高
            }
            else
            {
                zf_soft_iic_io_low  (soft_iic_obj_list[soft_iic_index].sda_pin);// SDA 拉低
            }
            temp >>= 1;

            zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
            zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
            zf_soft_iic_io_high (soft_iic_obj_list[soft_iic_index].scl_pin);    // SCL 拉高

            zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
            zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
            zf_soft_iic_io_low  (soft_iic_obj_list[soft_iic_index].scl_pin);    // SCL 拉低
        }
        temp = zf_soft_iic_wait_ack(soft_iic_index);
    }while(0);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 读取 8bit 数据
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     ack                 ACK 或 NACK
// 返回参数     uint8               数据
// 使用示例     zf_soft_iic_read_data(soft_iic_index, ack);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
ZF_INLINE uint8 zf_soft_iic_read_data (zf_soft_iic_index_enum soft_iic_index, uint8 ack)
{
    uint8 data = 0x00;
    uint8 temp = 8;
    do
    {
        if(ZF_DISABLE == soft_iic_obj_list[soft_iic_index].enable)
        {
            break;
        }
        zf_soft_iic_io_low      (soft_iic_obj_list[soft_iic_index].scl_pin);    // SCL 低电平
        zf_soft_iic_delay       (soft_iic_obj_list[soft_iic_index].delay);
        zf_soft_iic_io_high     (soft_iic_obj_list[soft_iic_index].sda_pin);    // SDA 高电平 释放 SDA
#if SOFT_IIC_SDA_IO_SWITCH
        zf_soft_iic_io_in       (soft_iic_obj_list[soft_iic_index].sda_pin);
#endif

        while(temp --)
        {
            zf_soft_iic_io_low  (soft_iic_obj_list[soft_iic_index].scl_pin);    // SCL 拉低
            zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
            zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
            zf_soft_iic_io_high (soft_iic_obj_list[soft_iic_index].scl_pin);    // SCL 拉高
            zf_soft_iic_delay   (soft_iic_obj_list[soft_iic_index].delay);
            data = ((data << 1) | zf_gpio_get_level(soft_iic_obj_list[soft_iic_index].sda_pin));
            zf_soft_iic_delay  (soft_iic_obj_list[soft_iic_index].delay);
        }
        zf_soft_iic_io_low      (soft_iic_obj_list[soft_iic_index].scl_pin);    // SCL 低电平
#if SOFT_IIC_SDA_IO_SWITCH
        zf_soft_iic_io_out      (soft_iic_obj_list[soft_iic_index].sda_pin);
#endif
        zf_soft_iic_send_ack    (soft_iic_index,  ack);
    }while(0);
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口传输 8bit 数组 写两个数组
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     addr                器件地址
// 参数说明     *part1_buffer       发送数据存放缓冲区
// 参数说明     part1_len           发送缓冲区长度
// 参数说明     *part2_buffer       读取数据存放缓冲区
// 参数说明     part2_len           读取缓冲区长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_write_8bit_splicing_array(soft_iic_index, addr, part1_buffer, part1_len, part2_buffer, part2_len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_write_8bit_splicing_array (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint8 *part1_buffer, uint32 part1_len, const uint8 *part2_buffer, uint32 part2_len)
{
    zf_soft_iic_operation_state_enum return_state = SOFT_IIC_ERROR_UNKNOW;

    do
    {
        if(zf_soft_iic_assert(soft_iic_obj_list[soft_iic_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_IIC_ERROR_MODULE_NOT_INIT;                      // SOFT_IIC 模块未初始化 操作无法进行
            break;
        }
        if(zf_soft_iic_assert((NULL != part1_buffer) || (NULL != part2_buffer)))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = SOFT_IIC_ERROR_DATA_BUFFER_NULL;                     // IIC 数据指针异常 操作无法进行
            break;
        }
        if(!soft_iic_obj_list[soft_iic_index].enable)
        {
            return_state = SOFT_IIC_WARNING_MODULE_DISABLE;                     // SOFT_IIC 模块失能禁用 操作中断退出
            break;
        }

        if(NULL != part1_buffer && 0 != part1_len)
        {
            zf_soft_iic_start       (soft_iic_index);
            zf_soft_iic_send_data   (soft_iic_index, addr << 1);
            while(part1_len --)
            {
                zf_soft_iic_send_data(soft_iic_index, *part1_buffer ++);
            }
        }

        if(NULL != part2_buffer && 0 != part2_len)
        {
            while(part2_len --)
            {
                zf_soft_iic_send_data(soft_iic_index, *part2_buffer ++);
            }
        }

        zf_soft_iic_stop(soft_iic_index);

        return_state = SOFT_IIC_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口传输 16bit 数组 写两个数组
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     addr                器件地址
// 参数说明     *part1_buffer       发送数据存放缓冲区
// 参数说明     part1_len           发送缓冲区长度
// 参数说明     *part2_buffer       读取数据存放缓冲区
// 参数说明     part2_len           读取缓冲区长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_write_16bit_splicing_array(soft_iic_index, addr, part1_buffer, part1_len, part2_buffer, part2_len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_write_16bit_splicing_array (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint16 *part1_buffer, uint32 part1_len, const uint16 *part2_buffer, uint32 part2_len)
{
    zf_soft_iic_operation_state_enum return_state = SOFT_IIC_ERROR_UNKNOW;

    do
    {
        if(zf_soft_iic_assert(soft_iic_obj_list[soft_iic_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_IIC_ERROR_MODULE_NOT_INIT;                      // SOFT_IIC 模块未初始化 操作无法进行
            break;
        }
        if(zf_soft_iic_assert((NULL != part1_buffer) || (NULL != part2_buffer)))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = SOFT_IIC_ERROR_DATA_BUFFER_NULL;                     // IIC 数据指针异常 操作无法进行
            break;
        }
        if(!soft_iic_obj_list[soft_iic_index].enable)
        {
            return_state = SOFT_IIC_WARNING_MODULE_DISABLE;                     // SOFT_IIC 模块失能禁用 操作中断退出
            break;
        }

        if(NULL != part1_buffer && 0 != part1_len)
        {
            zf_soft_iic_start       (soft_iic_index);
            zf_soft_iic_send_data   (soft_iic_index, addr << 1);
            while(part1_len --)
            {
                zf_soft_iic_send_data   (soft_iic_index, (uint8)((*part1_buffer & 0xFF00) >> 8));
                zf_soft_iic_send_data   (soft_iic_index, (uint8)(*part1_buffer ++ & 0x00FF));
            }
        }

        if(NULL != part2_buffer && 0 != part2_len)
        {
            while(part2_len --)
            {
                zf_soft_iic_send_data   (soft_iic_index, (uint8)((*part2_buffer & 0xFF00) >> 8));
                zf_soft_iic_send_data   (soft_iic_index, (uint8)(*part2_buffer ++ & 0x00FF));
            }
        }

        zf_soft_iic_stop(soft_iic_index);

        return_state = SOFT_IIC_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口传输 8bit 数组 先写后读取
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     addr                器件地址
// 参数说明     *write_buffer       发送数据存放缓冲区
// 参数说明     write_len           发送缓冲区长度
// 参数说明     *read_buffer        读取数据存放缓冲区
// 参数说明     read_len            读取缓冲区长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_transfer_8bit_array(soft_iic_index, addr, write_buffer, write_len, read_buffer, read_len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_transfer_8bit_array (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint8 *write_buffer, uint32 write_len, uint8 *read_buffer, uint32 read_len)
{
    zf_soft_iic_operation_state_enum return_state = SOFT_IIC_ERROR_UNKNOW;

    do
    {
        if(zf_soft_iic_assert(soft_iic_obj_list[soft_iic_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_IIC_ERROR_MODULE_NOT_INIT;                      // SOFT_IIC 模块未初始化 操作无法进行
            break;
        }
        if(zf_soft_iic_assert((NULL != write_buffer) || (NULL != read_buffer)))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = SOFT_IIC_ERROR_DATA_BUFFER_NULL;                     // IIC 数据指针异常 操作无法进行
            break;
        }
        if(!soft_iic_obj_list[soft_iic_index].enable)
        {
            return_state = SOFT_IIC_WARNING_MODULE_DISABLE;                     // SOFT_IIC 模块失能禁用 操作中断退出
            break;
        }

        if(NULL != write_buffer && 0 != write_len)
        {
            zf_soft_iic_start       (soft_iic_index);
            zf_soft_iic_send_data   (soft_iic_index, addr << 1);
            while(write_len --)
            {
                zf_soft_iic_send_data(soft_iic_index, *write_buffer ++);
            }
        }

        if(NULL != read_buffer && 0 != read_len)
        {
            zf_soft_iic_start       (soft_iic_index);
            zf_soft_iic_send_data   (soft_iic_index, addr << 1 | 0x01);
            while(read_len --)
            {
                *read_buffer ++ = zf_soft_iic_read_data(soft_iic_index, 0 == read_len);
            }
        }
        zf_soft_iic_stop(soft_iic_index);

        return_state = SOFT_IIC_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口传输 16bit 数组 先写后读取
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     addr                器件地址
// 参数说明     *write_buffer       发送数据存放缓冲区
// 参数说明     write_len           发送缓冲区长度
// 参数说明     *read_buffer        读取数据存放缓冲区
// 参数说明     read_len            读取缓冲区长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_transfer_16bit_array(soft_iic_index, addr, write_buffer, write_len, read_buffer, read_len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_transfer_16bit_array (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint16 *write_buffer, uint32 write_len, uint16 *read_buffer, uint32 read_len)
{
    zf_soft_iic_operation_state_enum return_state = SOFT_IIC_ERROR_UNKNOW;

    do
    {
        if(zf_soft_iic_assert(soft_iic_obj_list[soft_iic_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_IIC_ERROR_MODULE_NOT_INIT;                      // SOFT_IIC 模块未初始化 操作无法进行
            break;
        }
        if(zf_soft_iic_assert((NULL != write_buffer) || (NULL != read_buffer)))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = SOFT_IIC_ERROR_DATA_BUFFER_NULL;                     // IIC 数据指针异常 操作无法进行
            break;
        }
        if(!soft_iic_obj_list[soft_iic_index].enable)
        {
            return_state = SOFT_IIC_WARNING_MODULE_DISABLE;                     // SOFT_IIC 模块失能禁用 操作中断退出
            break;
        }

        if(NULL != write_buffer && 0 != write_len)
        {
            zf_soft_iic_start       (soft_iic_index);
            zf_soft_iic_send_data   (soft_iic_index, addr << 1);
            while(write_len --)
            {
                zf_soft_iic_send_data   (soft_iic_index, (uint8)((*write_buffer & 0xFF00) >> 8));
                zf_soft_iic_send_data   (soft_iic_index, (uint8)(*write_buffer ++ & 0x00FF));
            }
        }

        if(NULL != read_buffer && 0 != read_len)
        {
            zf_soft_iic_start       (soft_iic_index);
            zf_soft_iic_send_data   (soft_iic_index, addr << 1 | 0x01);
            while(read_len --)
            {
                *read_buffer = zf_soft_iic_read_data(soft_iic_index, 0);
                *read_buffer = ((*read_buffer << 8)| zf_soft_iic_read_data(soft_iic_index, 0 == read_len));
                read_buffer ++;
            }
        }
        zf_soft_iic_stop(soft_iic_index);

        return_state = SOFT_IIC_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口 SCCB 模式向传感器寄存器写 8bit 数据
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     addr                器件地址
// 参数说明     register_name       传感器的寄存器地址
// 参数说明     data                要写入的数据
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_sccb_write_register(soft_iic_index, addr, register_name, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_sccb_write_register (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint8 register_name, uint8 data)
{
    zf_soft_iic_operation_state_enum return_state = SOFT_IIC_ERROR_UNKNOW;

    do
    {
        if(zf_soft_iic_assert(soft_iic_obj_list[soft_iic_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_IIC_ERROR_MODULE_NOT_INIT;                      // SOFT_IIC 模块未初始化 操作无法进行
            break;
        }
        if(!soft_iic_obj_list[soft_iic_index].enable)
        {
            return_state = SOFT_IIC_WARNING_MODULE_DISABLE;                     // SOFT_IIC 模块失能禁用 操作中断退出
            break;
        }

        zf_soft_iic_start       (soft_iic_index);
        zf_soft_iic_send_data   (soft_iic_index, addr << 1);
        zf_soft_iic_send_data   (soft_iic_index, register_name);
        zf_soft_iic_send_data   (soft_iic_index, data);
        zf_soft_iic_stop        (soft_iic_index);

        return_state = SOFT_IIC_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口 SCCB 模式从传感器寄存器读取 8bit 数据
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     addr                器件地址
// 参数说明     register_name       传感器的寄存器地址
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_sccb_read_register(soft_iic_index, addr, register_name, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_sccb_read_register (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint8 register_name, uint8 *data)
{
    zf_soft_iic_operation_state_enum return_state = SOFT_IIC_ERROR_UNKNOW;

    do
    {
        if(zf_soft_iic_assert(soft_iic_obj_list[soft_iic_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_IIC_ERROR_MODULE_NOT_INIT;                      // SOFT_IIC 模块未初始化 操作无法进行
            break;
        }
        if(zf_soft_iic_assert(NULL != data))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = SOFT_IIC_ERROR_DATA_BUFFER_NULL;                     // IIC 数据指针异常 操作无法进行
            break;
        }
        if(!soft_iic_obj_list[soft_iic_index].enable)
        {
            return_state = SOFT_IIC_WARNING_MODULE_DISABLE;                     // SOFT_IIC 模块失能禁用 操作中断退出
            break;
        }

        zf_soft_iic_start       (soft_iic_index);
        zf_soft_iic_send_data   (soft_iic_index, addr << 1);
        zf_soft_iic_send_data   (soft_iic_index, register_name);
        zf_soft_iic_stop        (soft_iic_index);

        zf_soft_iic_start       (soft_iic_index);
        zf_soft_iic_send_data   (soft_iic_index, addr << 1 | 0x01);

        *data = zf_soft_iic_read_data(soft_iic_index, 1);

        zf_soft_iic_stop        (soft_iic_index);

        return_state = SOFT_IIC_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口配置速度
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     delay               软件 SPI 延时 就是时钟高电平时间 越短 SPI 速率越高 最小值为 1
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_set_speed(soft_iic_index, delay);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_set_speed (zf_soft_iic_index_enum soft_iic_index, uint32 delay)
{
    zf_soft_iic_operation_state_enum return_state = SOFT_IIC_ERROR_UNKNOW;

    do
    {
        if(zf_soft_iic_assert(soft_iic_obj_list[soft_iic_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_IIC_ERROR_MODULE_NOT_INIT;                      // SOFT_IIC 模块未初始化 操作无法进行
            break;
        }
        if(zf_soft_iic_assert(0 != delay))
        {
            // 此处如果断言报错 那么证明输入的 SOFT_IIC 的脉宽时间单位为 0
            return_state = SOFT_IIC_ERROR_SPEED_UNSUPPORTED;                    // SOFT_IIC 传输速率错误 操作无法进行
            break;
        }

        soft_iic_obj_list[soft_iic_index].delay = delay;

        return_state = SOFT_IIC_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口 使能
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_enable(soft_iic_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_enable (zf_soft_iic_index_enum soft_iic_index)
{
    zf_soft_iic_operation_state_enum return_state = SOFT_IIC_ERROR_UNKNOW;

    do
    {
        if(zf_soft_iic_assert(soft_iic_obj_list[soft_iic_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_IIC_ERROR_MODULE_NOT_INIT;                      // SOFT_IIC 模块未初始化 操作无法进行
            break;
        }
        soft_iic_obj_list[soft_iic_index].enable = ZF_ENABLE;

        return_state = SOFT_IIC_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口 禁止
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_disable(soft_iic_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_disable (zf_soft_iic_index_enum soft_iic_index)
{
    zf_soft_iic_operation_state_enum return_state = SOFT_IIC_ERROR_UNKNOW;

    do
    {
        if(zf_soft_iic_assert(soft_iic_obj_list[soft_iic_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_IIC_ERROR_MODULE_NOT_INIT;                      // SOFT_IIC 模块未初始化 操作无法进行
            break;
        }
        soft_iic_obj_list[soft_iic_index].enable = ZF_DISABLE;

        return_state = SOFT_IIC_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口注销初始化
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_deinit(soft_iic_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_deinit (zf_soft_iic_index_enum soft_iic_index)
{
    zf_soft_iic_operation_state_enum return_state = SOFT_IIC_ERROR_UNKNOW;

    do
    {
        if(zf_soft_iic_assert(soft_iic_obj_list[soft_iic_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_IIC_ERROR_MODULE_NOT_INIT;                      // SOFT_IIC 模块未初始化 操作无法进行
            break;
        }

        zf_gpio_deinit((zf_gpio_pin_enum)soft_iic_obj_list[soft_iic_index].scl_pin);
        zf_gpio_deinit((zf_gpio_pin_enum)soft_iic_obj_list[soft_iic_index].sda_pin);
        soft_iic_obj_list[soft_iic_index].scl_pin = PIN_NULL;
        soft_iic_obj_list[soft_iic_index].sda_pin = PIN_NULL;
        soft_iic_obj_list[soft_iic_index].delay = 0;
        soft_iic_obj_list[soft_iic_index].enable = ZF_DISABLE;

        return_state = SOFT_IIC_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口初始化 默认 MASTER 模式 不提供 SLAVE 模式
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     delay               SOFT_IIC 延时 就是时钟电平维持时间 越短 IIC 速率越高 最小值为 1
// 参数说明     scl_pin             SCL 引脚 参照   (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     sda_pin             SDA 引脚 参照   (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_init(soft_iic_index, delay, scl_pin, sda_pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_init (zf_soft_iic_index_enum soft_iic_index, uint32 delay, zf_gpio_pin_enum scl_pin, zf_gpio_pin_enum sda_pin)
{
    zf_soft_iic_operation_state_enum return_state = SOFT_IIC_ERROR_UNKNOW;

    do
    {
        if(zf_soft_iic_assert(!soft_iic_obj_list[soft_iic_index].delay))
        {
            // 此处如果断言报错 那么证明本模块已经被初始化过一次 重复初始化是不允许的
            return_state = SOFT_IIC_ERROR_MODULE_OCCUPIED;                      // SOFT_IIC 模块已被占用 操作无法进行
            break;
        }
        if(zf_soft_iic_assert(0 != delay))
        {
            // 此处如果断言报错 那么证明输入的 SOFT_IIC 的脉宽时间单位为 0
            return_state = SOFT_IIC_ERROR_SPEED_UNSUPPORTED;                    // SOFT_IIC 传输速率错误 操作无法进行
            break;
        }
        if(zf_soft_iic_assert(scl_pin != sda_pin && PIN_NULL != scl_pin && PIN_NULL != sda_pin))
        {
            // 此处如果断言报错 那么证明传入的 SCL 和 SDA 引脚重复
            // 或者 SCL 和 SDA 中有引脚是 PIN_NULL
            return_state = SOFT_IIC_ERROR_PIN_ILLEGAL;                          // SOFT_IIC 引脚参数错误 操作无法进行
            break;
        }
        soft_iic_obj_list[soft_iic_index].scl_pin   = scl_pin;
        soft_iic_obj_list[soft_iic_index].sda_pin   = sda_pin;
        soft_iic_obj_list[soft_iic_index].delay     = (delay) ? (delay) : (1);  // 防止关闭调试检查后设置为 0 值
        soft_iic_obj_list[soft_iic_index].enable    = ZF_ENABLE;

        zf_gpio_init(scl_pin, GPO_PUSH_PULL , GPIO_HIGH);                       // 初始化对应 IO
        zf_gpio_init(sda_pin, GPO_OPEN_DRAIN, GPIO_HIGH);                       // 初始化对应 IO

        return_state = SOFT_IIC_OPERATION_DONE;
    }while(0);
    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
