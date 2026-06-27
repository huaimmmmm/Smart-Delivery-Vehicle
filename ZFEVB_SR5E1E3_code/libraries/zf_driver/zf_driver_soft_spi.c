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
* 文件名称          zf_driver_soft_spi
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
#include "zf_driver_soft_spi.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
AT_ZF_LIB_SECTION_START
// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION zf_soft_spi_obj_struct soft_spi_obj_list[SOFT_SPI_MAX] =
{
    {.sck_pin = PIN_NULL, .mosi_pin = PIN_NULL, .miso_pin = PIN_NULL, .cs_pin = PIN_NULL, .delay = 0, .config_info = 0},
    {.sck_pin = PIN_NULL, .mosi_pin = PIN_NULL, .miso_pin = PIN_NULL, .cs_pin = PIN_NULL, .delay = 0, .config_info = 0}, 
    {.sck_pin = PIN_NULL, .mosi_pin = PIN_NULL, .miso_pin = PIN_NULL, .cs_pin = PIN_NULL, .delay = 0, .config_info = 0},
    {.sck_pin = PIN_NULL, .mosi_pin = PIN_NULL, .miso_pin = PIN_NULL, .cs_pin = PIN_NULL, .delay = 0, .config_info = 0}, 
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 延时
// 参数说明     void
// 返回参数     void
// 使用示例     zf_soft_spi_delay(1);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
//static void zf_soft_spi_delay (uint32 delay)
//{
//    volatile uint32 count = delay;
//    while(count --);
//}
#define zf_soft_spi_delay(x)  for(vuint32 i = x; i --; )

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_assert_handler(file, line, tips);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 传输开始 拉低 CS 信号 信号线进入工作状态
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 返回参数     void
// 使用示例     zf_soft_spi_transfer_start(soft_spi_index);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void zf_soft_spi_transfer_start (zf_soft_spi_index_enum soft_spi_index)
{
    if(soft_spi_obj_list[soft_spi_index].use_cs)
    {
        zf_gpio_low(soft_spi_obj_list[soft_spi_index].cs_pin);
    }

    if(     0 == soft_spi_obj_list[soft_spi_index].mode
        ||  1 == soft_spi_obj_list[soft_spi_index].mode)                        // CPOL = 0 SCK 空闲低电平
    {
        zf_gpio_low(soft_spi_obj_list[soft_spi_index].sck_pin);
    }
    else                                                                        // CPOL = 1 SCK 空闲高电平
    {
        zf_gpio_high(soft_spi_obj_list[soft_spi_index].sck_pin);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 传输停止 拉高 CS 信号
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 返回参数     void
// 使用示例     zf_soft_spi_transfer_stop(soft_spi_index);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void zf_soft_spi_transfer_stop (zf_soft_spi_index_enum soft_spi_index)
{
    if(soft_spi_obj_list[soft_spi_index].use_cs)
    {
        zf_gpio_high(soft_spi_obj_list[soft_spi_index].cs_pin);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 8bit 数据读写
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     data_in             输入指针
// 参数说明     data_out            输出指针
// 参数说明     len                 数据长度
// 返回参数     void
// 使用示例     zf_soft_spi_8bit_data_handler(soft_spi_index, data_in, data_out, len);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void zf_soft_spi_8bit_data_handler (zf_soft_spi_index_enum soft_spi_index, const uint8 *data_in, uint8 *data_out, uint32 len)
{
    uint8 temp = 0;
    uint8 write_data = 0;
    uint8 read_data = 0;
    uint8 mask_data = 0;

    while(len --)
    {
        if(NULL != data_in)
        {
            write_data = *data_in ++;
        }
        if(0 == soft_spi_obj_list[soft_spi_index].mode % 2)                     // CPHA = 0 第一个边沿采样
        {
            for(temp = 8; 0 < temp; temp --)
            {
                if(soft_spi_obj_list[soft_spi_index].format)
                {
                    mask_data = 0x01 << (8 - temp);
                }
                else
                {
                    mask_data = 0x80 >> (8 - temp);
                }
                if(mask_data & write_data)
                {
                    zf_gpio_high(soft_spi_obj_list[soft_spi_index].mosi_pin);
                }
                else
                {
                    zf_gpio_low(soft_spi_obj_list[soft_spi_index].mosi_pin);
                }
                zf_soft_spi_delay(soft_spi_obj_list[soft_spi_index].delay);
                zf_gpio_toggle_level(soft_spi_obj_list[soft_spi_index].sck_pin);
                if(soft_spi_obj_list[soft_spi_index].use_miso)
                {
                    if(zf_gpio_get_level(soft_spi_obj_list[soft_spi_index].miso_pin))
                    {
                        read_data |= mask_data;
                    }
                }
                zf_soft_spi_delay(soft_spi_obj_list[soft_spi_index].delay);
                zf_gpio_toggle_level(soft_spi_obj_list[soft_spi_index].sck_pin);
            }
        }
        else                                                                    // CPHA = 1 第二个边沿采样
        {
            for(temp = 8; 0 < temp; temp --)
            {
                zf_gpio_toggle_level(soft_spi_obj_list[soft_spi_index].sck_pin);
                if(soft_spi_obj_list[soft_spi_index].format)
                {
                    mask_data = 0x01 << (8 - temp);
                }
                else
                {
                    mask_data = 0x80 >> (8 - temp);
                }
                if(mask_data & write_data)
                {
                    zf_gpio_high(soft_spi_obj_list[soft_spi_index].mosi_pin);
                }
                else
                {
                    zf_gpio_low(soft_spi_obj_list[soft_spi_index].mosi_pin);
                }
                zf_soft_spi_delay(soft_spi_obj_list[soft_spi_index].delay);
                zf_gpio_toggle_level(soft_spi_obj_list[soft_spi_index].sck_pin);
                if(soft_spi_obj_list[soft_spi_index].use_miso)
                {
                    if(zf_gpio_get_level(soft_spi_obj_list[soft_spi_index].miso_pin))
                    {
                        read_data |= mask_data;
                    }
                }
                zf_soft_spi_delay(soft_spi_obj_list[soft_spi_index].delay);
            }
        }
        if(NULL != data_out)
        {
            *data_out ++ = read_data;
            read_data = 0;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 16bit 数据读写
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     data_in             输入指针
// 参数说明     data_out            输出指针
// 参数说明     len                 数据长度
// 返回参数     void
// 使用示例     zf_soft_spi_16bit_data_handler(soft_spi_index, data_in, data_out, len);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void zf_soft_spi_16bit_data_handler (zf_soft_spi_index_enum soft_spi_index, const uint16 *data_in, uint16 *data_out, uint32 len)
{
    uint8 temp = 0;
    uint16 write_data = 0;
    uint16 read_data = 0;
    uint16 mask_data = 0;

    while(len --)
    {
        if(NULL != data_in)
        {
            write_data = *data_in ++;
        }
        if(0 == soft_spi_obj_list[soft_spi_index].mode % 2)                     // CPHA = 0 第一个边沿采样
        {
            for(temp = 16; 0 < temp; temp --)
            {
                if(soft_spi_obj_list[soft_spi_index].format)
                {
                    mask_data = 0x0001 << (16 - temp);
                }
                else
                {
                    mask_data = 0x8000 >> (16 - temp);
                }
                if(mask_data & write_data)
                {
                    zf_gpio_high(soft_spi_obj_list[soft_spi_index].mosi_pin);
                }
                else
                {
                    zf_gpio_low(soft_spi_obj_list[soft_spi_index].mosi_pin);
                }
                zf_soft_spi_delay(soft_spi_obj_list[soft_spi_index].delay);
                zf_gpio_toggle_level(soft_spi_obj_list[soft_spi_index].sck_pin);
                if(soft_spi_obj_list[soft_spi_index].use_miso)
                {
                    if(zf_gpio_get_level(soft_spi_obj_list[soft_spi_index].miso_pin))
                    {
                        read_data |= mask_data;
                    }
                }
                zf_soft_spi_delay(soft_spi_obj_list[soft_spi_index].delay);
                zf_gpio_toggle_level(soft_spi_obj_list[soft_spi_index].sck_pin);
            }
        }
        else                                                                    // CPHA = 1 第二个边沿采样
        {
            for(temp = 16; 0 < temp; temp --)
            {
                zf_gpio_toggle_level(soft_spi_obj_list[soft_spi_index].sck_pin);
                if(soft_spi_obj_list[soft_spi_index].format)
                {
                    mask_data = 0x0001 << (16 - temp);
                }
                else
                {
                    mask_data = 0x8000 >> (16 - temp);
                }
                if(mask_data & write_data)
                {
                    zf_gpio_high(soft_spi_obj_list[soft_spi_index].mosi_pin);
                }
                else
                {
                    zf_gpio_low(soft_spi_obj_list[soft_spi_index].mosi_pin);
                }
                zf_soft_spi_delay(soft_spi_obj_list[soft_spi_index].delay);
                zf_gpio_toggle_level(soft_spi_obj_list[soft_spi_index].sck_pin);
                if(soft_spi_obj_list[soft_spi_index].use_miso)
                {
                    if(zf_gpio_get_level(soft_spi_obj_list[soft_spi_index].miso_pin))
                    {
                        read_data |= mask_data;
                    }
                }
                zf_soft_spi_delay(soft_spi_obj_list[soft_spi_index].delay);
            }
        }
        if(NULL != data_out)
        {
            *data_out ++ = read_data;
            read_data = 0;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 8bit 数据传输 发送与接收数据是同时进行的
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     *write_buffer       发送数据存放缓冲区
// 参数说明     *read_buffer        读取数据存放缓冲区
// 参数说明     len                 发送的字节数
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_transfer_8bit_array(soft_spi_index, write_buffer, read_buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_transfer_8bit_array (zf_soft_spi_index_enum soft_spi_index, const uint8 *write_buffer, uint8 *read_buffer, uint32 len)
{
    zf_soft_spi_operation_state_enum return_state = SOFT_SPI_ERROR_UNKNOW;

    do
    {
        if(zf_soft_spi_assert(soft_spi_obj_list[soft_spi_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_SPI_ERROR_MODULE_NOT_INIT;                      // SOFT_SPI 模块未初始化 操作无法进行
            break;
        }
        if(zf_soft_spi_assert((NULL != write_buffer) || (NULL != read_buffer)))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = SOFT_SPI_ERROR_DATA_BUFFER_NULL;                     // IIC 数据指针异常 操作无法进行
            break;
        }
        if(!soft_spi_obj_list[soft_spi_index].enable)
        {
            return_state = SOFT_SPI_WARNING_MODULE_DISABLE;                     // SOFT_SPI 模块失能禁用 操作中断退出
            break;
        }

        zf_soft_spi_transfer_start(soft_spi_index);
        zf_soft_spi_8bit_data_handler(soft_spi_index, write_buffer, read_buffer, len);
        zf_soft_spi_transfer_stop(soft_spi_index);

        return_state = SOFT_SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 16bit 数据传输 发送与接收数据是同时进行的
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     *write_buffer       发送数据存放缓冲区
// 参数说明     *read_buffer        读取数据存放缓冲区
// 参数说明     len                 发送的字节数
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_transfer_16bit_array(soft_spi_index, write_buffer, read_buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_transfer_16bit_array (zf_soft_spi_index_enum soft_spi_index, const uint16 *write_buffer, uint16 *read_buffer, uint32 len)
{
    zf_soft_spi_operation_state_enum return_state = SOFT_SPI_ERROR_UNKNOW;

    do
    {
        if(zf_soft_spi_assert(soft_spi_obj_list[soft_spi_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_SPI_ERROR_MODULE_NOT_INIT;                      // SOFT_SPI 模块未初始化 操作无法进行
            break;
        }
        if(zf_soft_spi_assert((NULL != write_buffer) || (NULL != read_buffer)))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = SOFT_SPI_ERROR_DATA_BUFFER_NULL;                     // IIC 数据指针异常 操作无法进行
            break;
        }
        if(!soft_spi_obj_list[soft_spi_index].enable)
        {
            return_state = SOFT_SPI_WARNING_MODULE_DISABLE;                     // SOFT_SPI 模块失能禁用 操作中断退出
            break;
        }

        zf_soft_spi_transfer_start(soft_spi_index);
        zf_soft_spi_16bit_data_handler(soft_spi_index, write_buffer, read_buffer, len);
        zf_soft_spi_transfer_stop(soft_spi_index);

        return_state = SOFT_SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口配置设置
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     item                SOFT_SPI 参照   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_config_item_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_set_config(soft_spi_index, item);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_set_config (zf_soft_spi_index_enum soft_spi_index, zf_soft_spi_config_item_enum item)
{
    zf_soft_spi_operation_state_enum return_state = SOFT_SPI_ERROR_UNKNOW;

    do
    {
        if(zf_soft_spi_assert(soft_spi_obj_list[soft_spi_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_SPI_ERROR_MODULE_NOT_INIT;                      // SOFT_SPI 模块未初始化 操作无法进行
            break;
        }

        soft_spi_obj_list[soft_spi_index].format = item;

        return_state = SOFT_SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口配置速度
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     delay               SOFT_SPI 延时 就是时钟高电平时间 越短 SPI 速率越高 最小值为 1
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_set_config(soft_spi_index, item);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_set_speed (zf_soft_spi_index_enum soft_spi_index, uint32 delay)
{
    zf_soft_spi_operation_state_enum return_state = SOFT_SPI_ERROR_UNKNOW;

    do
    {
        if(zf_soft_spi_assert(soft_spi_obj_list[soft_spi_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_SPI_ERROR_MODULE_NOT_INIT;                      // SOFT_SPI 模块未初始化 操作无法进行
            break;
        }
        if(zf_soft_spi_assert(0 != delay))
        {
            // 此处如果断言报错 那么证明输入的 SOFT_SPI 的脉宽时间单位为 0
            return_state = SOFT_SPI_ERROR_SPEED_UNSUPPORTED;                    // SOFT_SPI 传输速率错误 操作无法进行
            break;
        }

        soft_spi_obj_list[soft_spi_index].delay = delay;

        return_state = SOFT_SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口 使能
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_enable(soft_spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_enable (zf_soft_spi_index_enum soft_spi_index)
{
    zf_soft_spi_operation_state_enum return_state = SOFT_SPI_ERROR_UNKNOW;

    do
    {
        if(zf_soft_spi_assert(soft_spi_obj_list[soft_spi_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_SPI_ERROR_MODULE_NOT_INIT;                      // SOFT_SPI 模块未初始化 操作无法进行
            break;
        }
        soft_spi_obj_list[soft_spi_index].enable = ZF_ENABLE;

        return_state = SOFT_SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口 禁止
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_disable(soft_spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_disable (zf_soft_spi_index_enum soft_spi_index)
{
    zf_soft_spi_operation_state_enum return_state = SOFT_SPI_ERROR_UNKNOW;

    do
    {
        if(zf_soft_spi_assert(soft_spi_obj_list[soft_spi_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_SPI_ERROR_MODULE_NOT_INIT;                      // SOFT_SPI 模块未初始化 操作无法进行
            break;
        }
        soft_spi_obj_list[soft_spi_index].enable = ZF_DISABLE;

        return_state = SOFT_SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口注销初始化
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_deinit(soft_spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_deinit (zf_soft_spi_index_enum soft_spi_index)
{
    zf_soft_spi_operation_state_enum return_state = SOFT_SPI_ERROR_UNKNOW;

    do
    {
        if(zf_soft_spi_assert(soft_spi_obj_list[soft_spi_index].delay))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SOFT_SPI_ERROR_MODULE_NOT_INIT;                      // SOFT_SPI 模块未初始化 操作无法进行
            break;
        }

        zf_gpio_deinit((zf_gpio_pin_enum)soft_spi_obj_list[soft_spi_index].sck_pin);
        zf_gpio_deinit((zf_gpio_pin_enum)soft_spi_obj_list[soft_spi_index].mosi_pin);
        if(soft_spi_obj_list[soft_spi_index].use_miso)
        {
            zf_gpio_deinit((zf_gpio_pin_enum)soft_spi_obj_list[soft_spi_index].miso_pin);
        }
        if(soft_spi_obj_list[soft_spi_index].use_cs)
        {
            zf_gpio_deinit((zf_gpio_pin_enum)soft_spi_obj_list[soft_spi_index].cs_pin);
        }
        soft_spi_obj_list[soft_spi_index].sck_pin       = 0xFF  ;
        soft_spi_obj_list[soft_spi_index].mosi_pin      = 0xFF  ;
        soft_spi_obj_list[soft_spi_index].miso_pin      = 0xFF  ;
        soft_spi_obj_list[soft_spi_index].cs_pin        = 0xFF  ;
        soft_spi_obj_list[soft_spi_index].delay         = 0     ;
        soft_spi_obj_list[soft_spi_index].config_info   = 0     ;

        return_state = SOFT_SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_SPI 接口初始化
// 参数说明     soft_spi_index      SOFT_SPI 索引   (详见 zf_driver_soft_spi.h 内 zf_soft_spi_index_enum 定义)
// 参数说明     mode                SPI 模式        (详见 zf_driver_soft_spi.h 内 zf_soft_spi_mode_enum 定义)
// 参数说明     delay               SOFT_SPI 延时 就是时钟高电平时间 越短 SPI 速率越高 最小值为 1
// 参数说明     sck_pin             选择 SCK 引脚   (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     mosi_pin            选择 MOSI 引脚  (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     miso_pin            选择 MISO 引脚  如果不需要这个引脚 就填 PIN_NULL 或 MOSI 一样的引脚 (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     cs_pin              选择 CS 引脚    如果不需要这个引脚 就填 PIN_NULL 或 MISO 一样的引脚 (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_spi_init(soft_spi_index, mode, delay, sck_pin, mosi_pin, miso_pin, cs_pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_spi_init (zf_soft_spi_index_enum soft_spi_index, zf_soft_spi_mode_enum mode, uint32 delay, zf_gpio_pin_enum sck_pin, zf_gpio_pin_enum mosi_pin, zf_gpio_pin_enum miso_pin, zf_gpio_pin_enum cs_pin)
{
    zf_soft_spi_operation_state_enum return_state = SOFT_SPI_ERROR_UNKNOW;

    do
    {
        if(zf_soft_spi_assert(!soft_spi_obj_list[soft_spi_index].delay))
        {
            // 此处如果断言报错 那么证明本模块已经被初始化过一次 重复初始化是不允许的
            return_state = SOFT_SPI_ERROR_MODULE_OCCUPIED;                      // SOFT_SPI 模块已被占用 操作无法进行
            break;
        }
        if(zf_soft_spi_assert(0 != delay))
        {
            // 此处如果断言报错 那么证明输入的 SOFT_SPI 的脉宽时间单位为 0
            return_state = SOFT_SPI_ERROR_SPEED_UNSUPPORTED;                    // SOFT_SPI 传输速率错误 操作无法进行
            break;
        }

        if(zf_soft_spi_assert((PIN_NULL != sck_pin) && (sck_pin != mosi_pin) && (sck_pin != miso_pin) && (sck_pin != cs_pin)))
        {
            // 此处如果断言报错 那么证明输入的 sck_pin 引脚为 PIN_NULL 或与其他引脚冲突
            return_state = SOFT_SPI_ERROR_SCK_PIN_ILLEGAL;                      // SOFT_SPI 传输速率错误 操作无法进行
            break;
        }
        if(zf_soft_spi_assert(PIN_NULL != mosi_pin))
        {
            // 此处如果断言报错 那么证明输入的 mosi_pin 引脚为 PIN_NULL
            return_state = SOFT_SPI_ERROR_MOSI_PIN_ILLEGAL;                     // SOFT_SPI 传输速率错误 操作无法进行
            break;
        }

        soft_spi_obj_list[soft_spi_index].mode              = mode;
        soft_spi_obj_list[soft_spi_index].enable            = ZF_ENABLE;
        soft_spi_obj_list[soft_spi_index].delay             = delay;

        soft_spi_obj_list[soft_spi_index].sck_pin = sck_pin;
        soft_spi_obj_list[soft_spi_index].mosi_pin = mosi_pin;
        if(0 == mode || 1 == mode)
        {
            zf_gpio_init(sck_pin, GPO_PUSH_PULL, GPIO_LOW);
        }
        else
        {
            zf_gpio_init(sck_pin, GPO_PUSH_PULL, GPIO_HIGH);
        }
        zf_gpio_init(mosi_pin, GPO_PUSH_PULL, GPIO_HIGH);

        if(mosi_pin == miso_pin || PIN_NULL == miso_pin)
        {
            soft_spi_obj_list[soft_spi_index].use_miso = 0;
        }
        else
        {
            soft_spi_obj_list[soft_spi_index].use_miso = 1;
            soft_spi_obj_list[soft_spi_index].miso_pin = miso_pin;
            zf_gpio_init(miso_pin, GPI_FLOATING_IN, GPIO_HIGH);
        }
        if(mosi_pin == cs_pin || miso_pin == cs_pin || PIN_NULL == cs_pin)
        {
            soft_spi_obj_list[soft_spi_index].use_cs = 0;
        }
        else
        {
            soft_spi_obj_list[soft_spi_index].use_cs = 1;
            soft_spi_obj_list[soft_spi_index].cs_pin = cs_pin;
            zf_gpio_init(cs_pin, GPO_PUSH_PULL, GPIO_HIGH);
        }

        return_state = SOFT_SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}
