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
* 文件名称          zf_driver_spi
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
#include "zf_driver_interrupt.h"
#include "zf_driver_rcc.h"
#include "zf_driver_system.h"

// 自身头文件
#include "zf_driver_spi.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 用于固定值或指向的内容 主要是用少量存储占用来减少栈开销
static const zf_rcc_index_enum spi_rcc_index_list[SPI_NUM_MAX] =
{
    RCC_INDEX_SPI1, RCC_INDEX_SPI2, RCC_INDEX_SPI3, RCC_INDEX_SPI4
};

// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION zf_spi_obj_struct spi_obj_list[SPI_NUM_MAX] =
{
    {.spi_ptr = SPI1, .clock_input = 150000000, .sck_pin = PIN_NULL, .mosi_pin = PIN_NULL, .miso_pin = PIN_NULL, .cs_pin = PIN_NULL, .baudrate = 0, .config_info = 0},
    {.spi_ptr = SPI2, .clock_input =  75000000, .sck_pin = PIN_NULL, .mosi_pin = PIN_NULL, .miso_pin = PIN_NULL, .cs_pin = PIN_NULL, .baudrate = 0, .config_info = 0},
    {.spi_ptr = SPI3, .clock_input =  75000000, .sck_pin = PIN_NULL, .mosi_pin = PIN_NULL, .miso_pin = PIN_NULL, .cs_pin = PIN_NULL, .baudrate = 0, .config_info = 0},
    {.spi_ptr = SPI4, .clock_input = 150000000, .sck_pin = PIN_NULL, .mosi_pin = PIN_NULL, .miso_pin = PIN_NULL, .cs_pin = PIN_NULL, .baudrate = 0, .config_info = 0}
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define SPI_CTRL_CS(x, e)           ;
// #define SPI_CTRL_CS(x, e)           ((e) ? (spi_obj_list[spi_index].spi_ptr->CR1 |= SPI_CR1_SPE) : (spi_obj_list[spi_index].spi_ptr->CR1 &= ~SPI_CR1_SPE))
#define SPI_CTRL_RX(x, e)           ;

#define SPI_DATA_8BIT_RX(x)         (*(__IO uint8_t *)&spi_obj_list[x].spi_ptr->DR)                 // SPI 读数据
#define SPI_DATA_8BIT_TX(x, data)   (*(__IO uint8_t *)&spi_obj_list[x].spi_ptr->DR = (data))        // SPI 写数据

#define SPI_DATA_16BIT_RX(x)        (*(__IO uint16_t *)&spi_obj_list[x].spi_ptr->DR)                // SPI 读数据
#define SPI_DATA_16BIT_TX(x, data)  (*(__IO uint16_t *)&spi_obj_list[x].spi_ptr->DR = (data))       // SPI 写数据

#define SPI_STATE_BUY(x)            (spi_obj_list[x].spi_ptr->SR & SPI_SR_BSY)                      // SPI 忙 正在传输中
#define SPI_STATE_TX_FIFO_FULL(x)   ((spi_obj_list[x].spi_ptr->SR & SPI_SR_FTLVL) == SPI_SR_FTLVL)  // SPI 发送缓冲器满了
#define SPI_STATE_RX_AVAILABLE(x)   (spi_obj_list[x].spi_ptr->SR & SPI_SR_RXNE)                     // SPI 接收缓冲器收到有效数据
#define SPI_STATE_TX_EMPTY(x)       (spi_obj_list[x].spi_ptr->SR & SPI_SR_TXE)                      // SPI 发送缓冲器空了

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 8bit 数据传输 发送与接收数据是同时进行的
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     *write_buffer       发送数据存放缓冲区
// 参数说明     *read_buffer        读取数据存放缓冲区
// 参数说明     len                 发送的字节数
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_transfer_8bit_array(spi_index, write_buffer, read_buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_transfer_8bit_array (zf_spi_index_enum spi_index, const uint8 *write_buffer, uint8 *read_buffer, uint32 len)
{
    zf_spi_operation_state_enum    return_state    =   SPI_ERROR_UNKNOW;

    do
    {
        if(zf_spi_assert(spi_obj_list[spi_index].baudrate))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SPI_ERROR_MODULE_NOT_INIT;                           // SPI 模块未初始化 操作无法进行
            break;
        }
        if(zf_spi_assert((NULL != write_buffer) || (NULL != read_buffer)))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = SPI_ERROR_DATA_BUFFER_NULL;                          // IIC 数据指针异常 操作无法进行
            break;
        }
        if(!spi_obj_list[spi_index].enable)
        {
            return_state = SPI_WARNING_MODULE_DISABLE;                          // SPI 模块失能禁用 操作中断退出
            break;
        }

        SPI_CTRL_RX(spi_index, ZF_ENABLE);
        SPI_CTRL_CS(spi_index, ZF_ENABLE);

        while(len --)                                                           // 判断长度
        {
            SPI_DATA_8BIT_TX(spi_index, (NULL != write_buffer) ? (*write_buffer ++) : 0xFF);
            while(!SPI_STATE_RX_AVAILABLE(spi_index));                          // 接收到有效数据
            if(NULL != read_buffer)                                             // 接收有效
            {
                *read_buffer ++ = SPI_DATA_8BIT_RX(spi_index);                  // 读取数据
            }
            else
            {
                (void)SPI_DATA_8BIT_RX(spi_index);
            }
        }

        while(SPI_STATE_BUY(spi_index));                                        // 等待传输完毕
        SPI_CTRL_CS(spi_index, ZF_DISABLE);
        SPI_CTRL_RX(spi_index, ZF_DISABLE);

        return_state = SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 16bit 数据传输 发送与接收数据是同时进行的
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     *write_buffer       发送数据存放缓冲区
// 参数说明     *read_buffer        读取数据存放缓冲区
// 参数说明     len                 发送的字节数
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_transfer_16bit_array(spi_index, write_buffer, read_buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_transfer_16bit_array (zf_spi_index_enum spi_index, const uint16 *write_buffer, uint16 *read_buffer, uint32 len)
{
    zf_spi_operation_state_enum    return_state    =   SPI_ERROR_UNKNOW;

    do
    {
        if(zf_spi_assert(spi_obj_list[spi_index].baudrate))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SPI_ERROR_MODULE_NOT_INIT;                           // SPI 模块未初始化 操作无法进行
            break;
        }
        if(zf_spi_assert((NULL != write_buffer) || (NULL != read_buffer)))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = SPI_ERROR_DATA_BUFFER_NULL;                          // IIC 数据指针异常 操作无法进行
            break;
        }
        if(!spi_obj_list[spi_index].enable)
        {
            return_state = SPI_WARNING_MODULE_DISABLE;                          // SPI 模块失能禁用 操作中断退出
            break;
        }

        SPI_CTRL_RX(spi_index, ZF_ENABLE);
        SPI_CTRL_CS(spi_index, ZF_ENABLE);

        while(len --)                                                           // 判断长度
        {
            if(spi_obj_list[spi_index].width)
            {
                if(NULL != read_buffer)                                         // 接收有效
                {
                    SPI_DATA_16BIT_TX(spi_index, (NULL != write_buffer) ? (*write_buffer ++) : 0xFF);
                    while(!SPI_STATE_RX_AVAILABLE(spi_index));                  // 接收到有效数据
                    *read_buffer ++ = SPI_DATA_16BIT_RX(spi_index);             // 读取数据
                }
                else
                {
                    SPI_DATA_16BIT_TX(spi_index, *write_buffer ++);             // 发送数据
                    while(!SPI_STATE_RX_AVAILABLE(spi_index));                  // 接收到有效数据
                    (void)SPI_DATA_16BIT_RX(spi_index);                         // 读空数据
                }
            }
            else
            {
                if(NULL != read_buffer)                                         // 接收有效
                {
                    SPI_DATA_8BIT_TX(spi_index, (NULL != write_buffer) ? ((*write_buffer & 0xFF00) >> 8) : 0xFF);
                    while(!SPI_STATE_RX_AVAILABLE(spi_index));                  // 接收到有效数据
                    *read_buffer = (SPI_DATA_8BIT_RX(spi_index) & 0x00FF);      // 读取数据

                    SPI_DATA_8BIT_TX(spi_index, (NULL != write_buffer) ? (*write_buffer & 0x00FF) : 0xFF);
                    while(!SPI_STATE_RX_AVAILABLE(spi_index));                  // 接收到有效数据
                    *read_buffer = (*read_buffer << 8) | (SPI_DATA_8BIT_RX(spi_index)& 0x00FF);  // 读取数据

                    write_buffer ++;
                    read_buffer ++;
                }
                else
                {
                    SPI_DATA_8BIT_TX(spi_index, (*write_buffer & 0xFF00) >> 8); // 发送数据
                    while(!SPI_STATE_RX_AVAILABLE(spi_index));                  // 接收到有效数据
                    (void)SPI_DATA_8BIT_RX(spi_index);                          // 读空数据

                    SPI_DATA_8BIT_TX(spi_index, (*write_buffer & 0x00FF));      // 发送数据
                    while(!SPI_STATE_RX_AVAILABLE(spi_index));                  // 接收到有效数据
                    (void)SPI_DATA_8BIT_RX(spi_index);                          // 读空数据

                    write_buffer ++;
                }
            }
        }

        while(SPI_STATE_BUY(spi_index));                                        // 等待传输完毕
        SPI_CTRL_CS(spi_index, ZF_DISABLE);
        SPI_CTRL_RX(spi_index, ZF_DISABLE);

        return_state = SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口 配置设置
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     item                软件 SPI 参照 zf_driver_spi.h 内 zf_spi_config_item_enum 枚举体定义
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_set_config(spi_index, item);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_set_config (zf_spi_index_enum spi_index, zf_spi_config_item_enum item)
{
    zf_spi_operation_state_enum    return_state    =   SPI_ERROR_UNKNOW;

    do
    {
        if(zf_spi_assert(spi_obj_list[spi_index].baudrate))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SPI_ERROR_MODULE_NOT_INIT;                           // SPI 模块未初始化 操作无法进行
            break;
        }

        while(SPI_STATE_BUY(spi_index));                                        // 等待传输完毕

        uint32 register_temp = 0;
        switch(item & 0xF0)
        {
            case 0x00:
            {
                spi_obj_list[spi_index].width = (item & SPI_CONFIG_VALUE_MASK);
                register_temp = spi_obj_list[spi_index].spi_ptr->CR2;
                register_temp &= ~SPI_CR2_DS;
                switch(item & SPI_CONFIG_VALUE_MASK)
                {
                    case 0x00:  register_temp |= (0x00000007 << SPI_CR2_DS_Pos);    break;
                    case 0x01:  register_temp |= (0x0000000F << SPI_CR2_DS_Pos);    break;
                }
                spi_obj_list[spi_index].spi_ptr->CR2 = register_temp;
            }break;
            case 0x10:
            {
                spi_obj_list[spi_index].format = (item & SPI_CONFIG_VALUE_MASK);
                register_temp = spi_obj_list[spi_index].spi_ptr->CR1;
                register_temp &= ~0x00000080;
                switch(item & SPI_CONFIG_VALUE_MASK)
                {
                    case 0x00:  register_temp |= 0x00000000;    break;
                    case 0x01:  register_temp |= 0x00000080;    break;
                }
                spi_obj_list[spi_index].spi_ptr->CR1 = register_temp;
            }break;
        }

        return_state = SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口 配置速度
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     baudrate            设置 SPI 的波特率 不超过系统时钟的一半 会被适配成相近的速率
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_set_speed(spi_index, baudrate);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_set_speed (zf_spi_index_enum spi_index, uint32 baudrate)
{
    zf_spi_operation_state_enum    return_state    =   SPI_ERROR_UNKNOW;

    do
    {
        if(zf_spi_assert(spi_obj_list[spi_index].baudrate))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SPI_ERROR_MODULE_NOT_INIT;                           // SPI 模块未初始化 操作无法进行
            break;
        }
        if(zf_spi_assert(1000000 <= baudrate && spi_obj_list[spi_index].clock_input / 2 >= baudrate))
        {
            // 此处如果断言报错 那么证明输入的 SPI 的波特率不正确
            return_state = SPI_ERROR_SPEED_UNSUPPORTED;                         // SPI 传输速率错误 操作无法进行
            break;
        }

        while(SPI_STATE_BUY(spi_index));                                        // 等待传输完毕
        spi_obj_list[spi_index].spi_ptr->CR1 &= ~SPI_CR1_SPE;

        {
            uint8   i           = 0;
            uint16  div         = 2;
            uint32  baud_large  = spi_obj_list[spi_index].clock_input / div;
            spi_obj_list[spi_index].baudrate = spi_obj_list[spi_index].clock_input / div;

            for(i = 0; 8 > i; i ++, div *= 2)
            {
                spi_obj_list[spi_index].baudrate = spi_obj_list[spi_index].clock_input / div;
                if(spi_obj_list[spi_index].baudrate < baudrate)
                {
                    break;
                }
                baud_large = spi_obj_list[spi_index].baudrate;
            }
            if((baud_large - baudrate) < (baudrate - spi_obj_list[spi_index].baudrate))
            {
                spi_obj_list[spi_index].baudrate *= 2;
                i -= 1;
            }

            uint32 register_temp = spi_obj_list[spi_index].spi_ptr->CR1;
            register_temp &= ~(0x00000038);
            register_temp |= (i << 3);
            spi_obj_list[spi_index].spi_ptr->CR1 = register_temp;
        }

        return_state = SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口  使能
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_enable(spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_enable (zf_spi_index_enum spi_index)
{
    zf_spi_operation_state_enum    return_state    =   SPI_ERROR_UNKNOW;

    do
    {
        if(zf_spi_assert(spi_obj_list[spi_index].baudrate))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SPI_ERROR_MODULE_NOT_INIT;                           // SPI 模块未初始化 操作无法进行
            break;
        }

        spi_obj_list[spi_index].enable = ZF_ENABLE;

        return_state = SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口  禁止
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_disable(spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_disable (zf_spi_index_enum spi_index)
{
    zf_spi_operation_state_enum    return_state    =   SPI_ERROR_UNKNOW;

    do
    {
        if(zf_spi_assert(spi_obj_list[spi_index].baudrate))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SPI_ERROR_MODULE_NOT_INIT;                           // SPI 模块未初始化 操作无法进行
            break;
        }

        spi_obj_list[spi_index].enable = ZF_DISABLE;

        return_state = SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口 注销初始化
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_deinit(spi_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_deinit (zf_spi_index_enum spi_index)
{
    zf_spi_operation_state_enum    return_state    =   SPI_ERROR_UNKNOW;

    do
    {
        if(zf_spi_assert(spi_obj_list[spi_index].baudrate))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            return_state = SPI_ERROR_MODULE_NOT_INIT;                           // SPI 模块未初始化 操作无法进行
            break;
        }

        zf_gpio_deinit((zf_gpio_pin_enum)spi_obj_list[spi_index].sck_pin);
        zf_gpio_deinit((zf_gpio_pin_enum)spi_obj_list[spi_index].mosi_pin);
        if(spi_obj_list[spi_index].use_miso)
        {
            zf_gpio_deinit((zf_gpio_pin_enum)spi_obj_list[spi_index].miso_pin);
        }
        if(spi_obj_list[spi_index].use_cs)
        {
            zf_gpio_deinit((zf_gpio_pin_enum)spi_obj_list[spi_index].cs_pin);
        }
        spi_obj_list[spi_index].baudrate    = 0;
        spi_obj_list[spi_index].config_info = 0;
        
        spi_obj_list[spi_index].sck_pin     = 0xFF;
        spi_obj_list[spi_index].mosi_pin    = 0xFF;
        spi_obj_list[spi_index].miso_pin    = 0xFF;
        spi_obj_list[spi_index].cs_pin      = 0xFF;

        zf_rcc_peripheral_reset(spi_rcc_index_list[spi_index]);
        zf_rcc_clock_disable(spi_rcc_index_list[spi_index]);

        return_state = SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口初始化
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     mode                SPI 模式        (详见 zf_driver_spi.h 内 zf_spi_mode_enum 定义)
// 参数说明     baudrate            设置 SPI 的波特率 不超过系统时钟的一半 会被适配成相近的速率
// 参数说明     sck_pin             选择 SCK 引脚   (详见 zf_driver_spi.h 内 zf_spi_sck_pin_enum 定义)
// 参数说明     mosi_pin            选择 MOSI 引脚  (详见 zf_driver_spi.h 内 zf_spi_mosi_pin_enum 定义)
// 参数说明     miso_pin            选择 MISO 引脚  (详见 zf_driver_spi.h 内 zf_spi_miso_pin_enum 定义)
// 参数说明     cs_pin              选择 CS 引脚    (详见 zf_driver_spi.h 内 zf_spi_cs_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SPI_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_init(spi_index, mode, baudrate, sck_pin, mosi_pin, miso_pin, cs_pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_spi_init (zf_spi_index_enum spi_index, zf_spi_mode_enum mode, uint32 baudrate, zf_spi_sck_pin_enum sck_pin, zf_spi_mosi_pin_enum mosi_pin, zf_spi_miso_pin_enum miso_pin, zf_spi_cs_pin_enum cs_pin)
{
    zf_spi_operation_state_enum    return_state    =   SPI_ERROR_UNKNOW;

    do
    {
        if(zf_spi_assert(!spi_obj_list[spi_index].baudrate))
        {
            // 此处如果断言报错 那么证明本模块已经被初始化过一次 重复初始化是不允许的
            return_state = SPI_ERROR_MODULE_OCCUPIED;                           // SPI 模块已被占用 操作无法进行
            break;
        }

        if(zf_spi_assert(spi_index == ((sck_pin >> SPI_INDEX_OFFSET) & SPI_INDEX_MASK)))
        {
            // 此处如果断言报错 那么证明输入的 SCK 引脚与 spi_index 不匹配
            return_state = SPI_ERROR_SCK_PIN_ILLEGAL;
            break;
        }
        if(zf_spi_assert(spi_index == ((mosi_pin >> SPI_INDEX_OFFSET) & SPI_INDEX_MASK)))
        {
            // 此处如果断言报错 那么证明输入的 MOSI 引脚与 spi_index 不匹配
            return_state = SPI_ERROR_MOSI_PIN_ILLEGAL;
            break;
        }
        if(zf_spi_assert(
            (spi_index == ((miso_pin >> SPI_INDEX_OFFSET) & SPI_INDEX_MASK))
            || (SPI_MISO_NULL == miso_pin)))
        {
            // 此处如果断言报错 那么证明输入的 MISO 引脚与 spi_index 不匹配
            return_state = SPI_ERROR_MISO_PIN_ILLEGAL;
            break;
        }
        if(zf_spi_assert(
            (spi_index == ((cs_pin >> SPI_INDEX_OFFSET) & SPI_INDEX_MASK))
            || (SPI_CS_NULL == cs_pin)))
        {
            // 此处如果断言报错 那么证明输入的 CS 引脚与 spi_index 不匹配
            return_state = SPI_ERROR_CS_PIN_ILLEGAL;
            break;
        }

        uint8 clock_div = (RCC_INDEX_APB1L_BUS_INDEX == (spi_rcc_index_list[spi_index] & RCC_BUS_INDEX_MASK)) ? (4) : (2);
        spi_obj_list[spi_index].clock_input = zf_system_clock / clock_div;
        if(zf_spi_assert(1000000 <= baudrate && spi_obj_list[spi_index].clock_input / 2 >= baudrate))
        {
            // 此处如果断言报错 那么证明输入的 SPI 的波特率不正确
            return_state = SPI_ERROR_SPEED_UNSUPPORTED;                         // SPI 传输速率错误 操作无法进行
            break;
        }

        spi_obj_list[spi_index].sck_pin     = (zf_gpio_pin_enum)(sck_pin  & SPI_PIN_INDEX_MASK);
        spi_obj_list[spi_index].mosi_pin    = (zf_gpio_pin_enum)(mosi_pin & SPI_PIN_INDEX_MASK);

        spi_obj_list[spi_index].mode        = mode;
        spi_obj_list[spi_index].width       = (SPI_CONFIG_DATA_LENGTH_8BIT & SPI_CONFIG_VALUE_MASK);
        spi_obj_list[spi_index].format      = (SPI_CONFIG_DATA_FORMAT_MSB  & SPI_CONFIG_VALUE_MASK);
        spi_obj_list[spi_index].use_miso    = ZF_DISABLE;
        spi_obj_list[spi_index].use_cs      = ZF_DISABLE;

        zf_afio_init(
            spi_obj_list[spi_index].sck_pin,
            GPO_AF_PUSH_PULL,
            (zf_gpio_af_enum)((sck_pin >> SPI_PIN_AF_OFFSET) & SPI_PIN_AF_MASK));
        zf_afio_init(
            spi_obj_list[spi_index].mosi_pin,
            GPO_AF_PUSH_PULL,
            (zf_gpio_af_enum)((mosi_pin >> SPI_PIN_AF_OFFSET) & SPI_PIN_AF_MASK));
        if(SPI_MISO_NULL != miso_pin)
        {
            spi_obj_list[spi_index].use_miso = ZF_ENABLE;
            spi_obj_list[spi_index].miso_pin = (zf_gpio_pin_enum)(miso_pin &SPI_PIN_INDEX_MASK);
            zf_afio_init(
                spi_obj_list[spi_index].miso_pin,
                GPO_AF_OPEN_DRAIN,
                (zf_gpio_af_enum)((miso_pin >> SPI_PIN_AF_OFFSET) & SPI_PIN_AF_MASK));
        }
        if(SPI_CS_NULL != cs_pin)
        {
            spi_obj_list[spi_index].use_cs = ZF_ENABLE;
            spi_obj_list[spi_index].cs_pin = (zf_gpio_pin_enum)(cs_pin &SPI_PIN_INDEX_MASK);
            zf_afio_init(
                spi_obj_list[spi_index].cs_pin,
                GPO_AF_PUSH_PULL,
                (zf_gpio_af_enum)((cs_pin >> SPI_PIN_AF_OFFSET) & SPI_PIN_AF_MASK));
        }

        zf_rcc_clock_enable(spi_rcc_index_list[spi_index]);

        {
            uint8   i               = 0;
            uint16  div             = 2;
            uint32  register_temp   = 0;
            uint32  baud_large      = spi_obj_list[spi_index].clock_input / div;
            spi_obj_list[spi_index].baudrate = spi_obj_list[spi_index].clock_input / div;

            for(i = 0; 8 > i; i ++, div *= 2)
            {
                spi_obj_list[spi_index].baudrate = spi_obj_list[spi_index].clock_input / div;
                if(spi_obj_list[spi_index].baudrate < baudrate)
                {
                    break;
                }
                baud_large = spi_obj_list[spi_index].baudrate;
            }
            if((baud_large - baudrate) < (baudrate - spi_obj_list[spi_index].baudrate))
            {
                spi_obj_list[spi_index].baudrate *= 2;
                i -= 1;
            }

            register_temp |= (i << 3);

            switch(mode)
            {
                case SPI_MODE0: register_temp |= 0x00000000;    break;
                case SPI_MODE1: register_temp |= 0x00000001;    break;
                case SPI_MODE2: register_temp |= 0x00000002;    break;
                case SPI_MODE3: register_temp |= 0x00000003;    break;
                default:                                        break;
            }

            register_temp |= 0x00000004;

            spi_obj_list[spi_index].spi_ptr->CR1 = register_temp;
        }

        SPI_CTRL_CS(spi_index, ZF_DISABLE);
        spi_obj_list[spi_index].spi_ptr->CR2 |= SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_FRXTH;
        // spi_obj_list[spi_index].spi_ptr->CR1 &= ~SPI_CR1_SPE;
        spi_obj_list[spi_index].spi_ptr->CR1 |= SPI_CR1_SPE;
        spi_obj_list[spi_index].enable = ZF_ENABLE;

        return_state = SPI_OPERATION_DONE;
    }while(0);

    return return_state;
}
