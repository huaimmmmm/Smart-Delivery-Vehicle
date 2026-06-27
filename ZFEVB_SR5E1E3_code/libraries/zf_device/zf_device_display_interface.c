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
* 文件名称          zf_device_display_interface
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

/*********************************************************************************************************************
* 本文件的作用是方便 IPS/TFT 的任意平台适配
* 内容为将 IPS/TFT 相关的接口进行解耦整合
* 在移植到新的平台时 只需要实现了本文件的接口
* 就可以直接适配 SeekFree 所有的 V4 库 IPS/TFT 器件
********************************************************************************************************************/

// 自身头文件
#include "zf_device_display_interface.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION display_interface_obj_struct display_interface_default_obj = 
{
    .interface_type         = DISPLAY_INTERFACE_TYPE_UNKNOW,
    .communication_index    = 0,

    .dir_mode               = DISPLAY_DIR_DEFAULT,
    .font_size              = DISPLAY_FONT_SIZE_DEFAULT,

    .bgcolor                = DISPLAY_BGCOLOR_SIZE_DEFAULT,
    .pencolor               = DISPLAY_PENCOLOR_DEFAULT,

    .parallel8_wr_pin       = PIN_NULL,
    .parallel8_rd_pin       = PIN_NULL,
    .spi_rst_pin            = PIN_NULL,
    .spi_dc_pin             = PIN_NULL,
    .spi_cs_pin             = PIN_NULL,
    .spi_blk_pin            = PIN_NULL,

    .display_width_max      = 0,
    .display_high_max       = 0,

    .set_region             = NULL
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
#if DISPLAY_INTERFACE_USE_ZF_COMMON_DEBUG
#else
uint8 display_interface_assert_handler (char *file, int line, char *tips)
{
    printf("\r\n IMU OPERATION ASSERT!\r\n");
    printf("\r\n file: %s.\r\n", file);
    printf("\r\n line: %d.\r\n", line);
    if(NULL != tips)
    {
        printf("\r\n tips: %s.\r\n", tips);
    }
    while(1);
    return ZF_ERROR;
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 延时
// 参数说明     ms                  毫秒数
// 返回参数     void
// 使用示例     display_interface_delay_ms(ms);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#if DISPLAY_INTERFACE_USE_ZF_DIRVER_DELAY
#else
void display_interface_delay_ms (uint32 ms)
{
    for(uint32 loop_count1 = ms    ; loop_count1 --; )
    for(uint32 loop_count2 = 0xFFFF; loop_count2 --; );
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 写 8bit 数据
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     cmd                 命令
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_write_8bit_data(interface, cmd);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_write_command (display_interface_obj_struct *interface, const uint8 cmd)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_OPERATION_DONE;

    do
    {
        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_dc_pin);
            zf_spi_transfer_8bit_array(interface->spi_index, &cmd, NULL, 1);
            zf_gpio_high(interface->spi_dc_pin);
        }
        else
        {
            // zf_gpio_low(interface->parallel8_cs_pin);
            // zf_gpio_high(interface->parallel8_rd_pin);
            // zf_gpio_low(interface->parallel8_wr_pin);
            // *(volatile uint8 *)IPS200_DATA_ADD = (uint8)cmd;
            // zf_gpio_high(interface->parallel8_wr_pin);
            // zf_gpio_high(interface->parallel8_cs_pin);
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 写 8bit 数据
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     data                数据
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_write_8bit_data(interface, data);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_write_8bit_data (display_interface_obj_struct *interface, const uint8 data)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_OPERATION_DONE;

    do
    {
        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_spi_transfer_8bit_array(interface->spi_index, &data, NULL, 1);
        }
        else
        {
            // zf_gpio_low(interface->parallel8_cs_pin);
            // zf_gpio_high(interface->parallel8_rd_pin);
            // zf_gpio_low(interface->parallel8_wr_pin);
            // *(volatile uint8 *)IPS200_DATA_ADD = data;
            // zf_gpio_high(interface->parallel8_wr_pin);
            // zf_gpio_high(interface->parallel8_cs_pin);
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 写 8bit 数据
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     data                数据
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_write_16bit_data(interface, data, len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_write_16bit_data (display_interface_obj_struct *interface, const uint16 data)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_OPERATION_DONE;

    do
    {
        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_spi_transfer_16bit_array(interface->spi_index, &data, NULL, 1);
        }
        else
        {
            // zf_gpio_low(interface->parallel8_cs_pin);
            // zf_gpio_high(interface->parallel8_rd_pin);
            // zf_gpio_low(interface->parallel8_wr_pin);
            // *(volatile uint8 *)IPS200_DATA_ADD = (uint8)(data >> 8);
            // zf_gpio_high(interface->parallel8_wr_pin);
            // zf_gpio_low(interface->parallel8_wr_pin);
            // *(volatile uint8 *)IPS200_DATA_ADD = (uint8)(data & 0x00FF);
            // zf_gpio_high(interface->parallel8_wr_pin);
            // zf_gpio_high(interface->parallel8_cs_pin);
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 写 8bit 数据
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     *data               数据缓冲区
// 参数说明     len                 数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_write_8bit_array(interface, data, len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_write_8bit_array (display_interface_obj_struct *interface, const uint8 *data, uint32 len)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_OPERATION_DONE;

    do
    {
        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_spi_transfer_8bit_array(interface->spi_index, data, NULL, len);
        }
        else
        {
            // zf_gpio_low(interface->parallel8_cs_pin);
            // zf_gpio_high(interface->parallel8_rd_pin);
            // while(len --)
            // {
            //     zf_gpio_low(interface->parallel8_wr_pin);
            //     *(volatile uint8 *)IPS200_DATA_ADD = (uint8)*data;
            //     zf_gpio_high(interface->parallel8_wr_pin);
            //     data ++;
            // }
            // zf_gpio_high(interface->parallel8_cs_pin);
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 双向传输 8bit 数据
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     *data               数据缓冲区
// 参数说明     len                 数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_write_16bit_array(interface, data, len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_write_16bit_array (display_interface_obj_struct *interface, const uint16 *data, uint32 len)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_OPERATION_DONE;

    do
    {
        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_spi_set_config(interface->spi_index, SPI_CONFIG_DATA_LENGTH_16BIT);
            zf_spi_transfer_16bit_array(interface->spi_index, data, NULL, len);
            zf_spi_set_config(interface->spi_index, SPI_CONFIG_DATA_LENGTH_8BIT);
        }
        else
        {
            // zf_gpio_low(interface->parallel8_cs_pin);
            // zf_gpio_high(interface->parallel8_rd_pin);
            // while(len --)
            // {
            //     zf_gpio_low(interface->parallel8_wr_pin);
            //     *(volatile uint8 *)IPS200_DATA_ADD = (uint8)((*data) >> 8);
            //     zf_gpio_high(interface->parallel8_wr_pin);
            //     zf_gpio_low(interface->parallel8_wr_pin);
            //     *(volatile uint8 *)IPS200_DATA_ADD = (uint8)((*data) & 0x00FF);
            //     zf_gpio_high(interface->parallel8_wr_pin);
            //     data ++;
            // }
            // zf_gpio_high(interface->parallel8_cs_pin);
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 清屏函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_clear(interface);
// 备注信息     将屏幕清空成背景颜色
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_clear (display_interface_obj_struct *interface)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
        }

        {
            interface->set_region(interface, 0, 0, interface->display_width_max - 1, interface->display_high_max - 1);

            uint16 data_buffer[512];
            for(uint32 loop_temp = 0; loop_temp < interface->display_width_max; loop_temp ++)
            {
                data_buffer[loop_temp] = interface->bgcolor;
            }
            for(uint32 loop_temp = 0; loop_temp < interface->display_high_max; loop_temp ++)
            {
                display_interface_write_16bit_array(interface, data_buffer, interface->display_width_max);
            }
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_high(interface->spi_cs_pin);
        }

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 屏幕填充函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     color               RGB565 格式颜色 (详见 zf_device_display_font.h 内 rgb565_color_enum 定义) 或者自行写入
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_full(interface, color);
// 备注信息     将屏幕清空成背景颜色
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_full (display_interface_obj_struct *interface, const uint16 color)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
        }

        {
            interface->set_region(interface, 0, 0, interface->display_width_max - 1, interface->display_high_max - 1);

            uint16 data_buffer[512];
            for(uint32 loop_temp = 0; loop_temp < interface->display_width_max; loop_temp ++)
            {
                data_buffer[loop_temp] = color;
            }
            for(uint32 loop_temp = 0; loop_temp < interface->display_high_max; loop_temp ++)
            {
                display_interface_write_16bit_array(interface, data_buffer, interface->display_width_max);
            }
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_high(interface->spi_cs_pin);
        }

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 画点函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     color               RGB565 格式颜色 (详见 zf_device_display_font.h 内 rgb565_color_enum 定义) 或者自行写入
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_draw_point(interface, x, y, color);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_draw_point (display_interface_obj_struct *interface, uint16 x, uint16 y, const uint16 color)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(display_interface_assert(x < interface->display_width_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(y < interface->display_high_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
        }

        {
            interface->set_region(interface, x, y, x, y);
            display_interface_write_16bit_data(interface, color);
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_high(interface->spi_cs_pin);
        }

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 画线函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     x_start             坐标 x 方向的起点 [0, interface->display_width_max - 1]
// 参数说明     y_start             坐标 y 方向的起点 [0, interface->display_high_max  - 1]
// 参数说明     x_end               坐标 x 方向的终点 [0, interface->display_width_max - 1]
// 参数说明     y_end               坐标 y 方向的终点 [0, interface->display_high_max  - 1]
// 参数说明     color               RGB565 格式颜色 (详见 zf_device_display_font.h 内 rgb565_color_enum 定义) 或者自行写入
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_draw_line(interface, y_start, x_end, y_end, color);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_draw_line (display_interface_obj_struct *interface, uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(display_interface_assert(x_start < interface->display_width_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(y_start < interface->display_high_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(x_end < interface->display_width_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(y_end < interface->display_high_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
        }

        {
            int16 x_dir = (x_start < x_end ? 1 : -1);
            int16 y_dir = (y_start < y_end ? 1 : -1);
            float temp_rate = 0;
            float temp_b = 0;
            do
            {
                if(x_start != x_end)
                {
                    temp_rate = (float)(y_start - y_end) / (float)(x_start - x_end);
                    temp_b = (float)y_start - (float)x_start * temp_rate;
                }
                else
                {
                    while(y_start != y_end)
                    {
                        interface->set_region(interface, x_start, y_start, x_start, y_start);
                        display_interface_write_16bit_data(interface, color);
                        y_start += y_dir;
                    }
                    break;
                }
                if(zf_function_abs(y_start - y_end) > zf_function_abs(x_start - x_end))
                {
                    while(y_start != y_end)
                    {
                        interface->set_region(interface, x_start, y_start, x_start, y_start);
                        display_interface_write_16bit_data(interface, color);
                        y_start += y_dir;
                        x_start = (int16)(((float)y_start - temp_b) / temp_rate);
                    }
                }
                else
                {
                    while(x_start != x_end)
                    {
                        interface->set_region(interface, x_start, y_start, x_start, y_start);
                        display_interface_write_16bit_data(interface, color);
                        x_start += x_dir;
                        y_start = (int16)((float)x_start * temp_rate + temp_b);
                    }
                }
            }while(0);
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_high(interface->spi_cs_pin);
        }

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 显示字符函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     asc_ii_char         需要显示的字符 asi-ii 编码
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_show_char(interface, x, y, asc_ii_char);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_show_char (display_interface_obj_struct *interface, uint16 x, uint16 y, const char asc_ii_char)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(display_interface_assert(x < interface->display_width_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(y < interface->display_high_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
        }

        {
            uint8 i = 0, j = 0;
            switch(interface->font_size)
            {
                case DISPLAY_INTERFACE_FONT_6X8:
                {
                    for(i = 0; 6 > i; i ++)
                    {
                        interface->set_region(interface, x + i, y, x + i, y + 8);
                        // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
                        uint8 temp_top = display_font_ascii_6x8[asc_ii_char - 32][i];
                        for(j = 0; 8 > j; j ++)
                        {
                            if(temp_top & 0x01)
                            {
                                display_interface_write_16bit_data(interface, interface->pencolor);
                            }
                            else
                            {
                                display_interface_write_16bit_data(interface, interface->bgcolor);
                            }
                            temp_top >>= 1;
                        }
                    }
                }break;
                case DISPLAY_INTERFACE_FONT_8X16:
                {
                    for(i = 0; 8 > i; i ++)
                    {
                        interface->set_region(interface, x + i, y, x + i, y + 15);
                        // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
                        uint8 temp_top = display_font_ascii_8x16[asc_ii_char - 32][i];
                        uint8 temp_bottom = display_font_ascii_8x16[asc_ii_char - 32][i + 8];
                        for(j = 0; 8 > j; j ++)
                        {
                            if(temp_top & 0x01)
                            {
                                display_interface_write_16bit_data(interface, interface->pencolor);
                            }
                            else
                            {
                                display_interface_write_16bit_data(interface, interface->bgcolor);
                            }
                            temp_top >>= 1;
                        }
                        for(j = 0; 8 > j; j ++)
                        {
                            if(temp_bottom & 0x01)
                            {
                                display_interface_write_16bit_data(interface, interface->pencolor);
                            }
                            else
                            {
                                display_interface_write_16bit_data(interface, interface->bgcolor);
                            }
                            temp_bottom >>= 1;
                        }
                    }
                }break;
            }
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_high(interface->spi_cs_pin);
        }

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 显示字符串函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     asc_ii_string       需要显示的字符串 asi-ii 编码
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_show_string(interface, x, y, asc_ii_string);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_show_string (display_interface_obj_struct *interface, uint16 x, uint16 y, const char *asc_ii_string)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(display_interface_assert(x < interface->display_width_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(y < interface->display_high_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
        }

        {
            uint8 i = 0, j = 0, str_len = 0;
            while('\0' != asc_ii_string[str_len])
            {
                switch(interface->font_size)
                {
                    case DISPLAY_INTERFACE_FONT_6X8:
                    {
                        for(i = 0; 6 > i; i ++)
                        {
                            interface->set_region(interface, x + i, y, x + i, y + 8);
                            // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
                            uint8 temp_top = display_font_ascii_6x8[asc_ii_string[str_len] - 32][i];
                            for(j = 0; 8 > j; j ++)
                            {
                                if(temp_top & 0x01)
                                {
                                    display_interface_write_16bit_data(interface, interface->pencolor);
                                }
                                else
                                {
                                    display_interface_write_16bit_data(interface, interface->bgcolor);
                                }
                                temp_top >>= 1;
                            }
                        }
                        x += 6;
                    }break;
                    case DISPLAY_INTERFACE_FONT_8X16:
                    {
                        for(i = 0; 8 > i; i ++)
                        {
                            interface->set_region(interface, x + i, y, x + i, y + 15);
                            // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
                            uint8 temp_top = display_font_ascii_8x16[asc_ii_string[str_len] - 32][i];
                            uint8 temp_bottom = display_font_ascii_8x16[asc_ii_string[str_len] - 32][i + 8];
                            for(j = 0; 8 > j; j ++)
                            {
                                if(temp_top & 0x01)
                                {
                                    display_interface_write_16bit_data(interface, interface->pencolor);
                                }
                                else
                                {
                                    display_interface_write_16bit_data(interface, interface->bgcolor);
                                }
                                temp_top >>= 1;
                            }
                            for(j = 0; 8 > j; j ++)
                            {
                                if(temp_bottom & 0x01)
                                {
                                    display_interface_write_16bit_data(interface, interface->pencolor);
                                }
                                else
                                {
                                    display_interface_write_16bit_data(interface, interface->bgcolor);
                                }
                                temp_bottom >>= 1;
                            }
                        }
                        x += 8;
                    }break;
                }
                str_len ++;
            }
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_high(interface->spi_cs_pin);
        }

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 显示 32bit 有符号整数 函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     dat                 需要显示的变量 数据类型 int32
// 参数说明     num                 需要显示的位数 最高10位  不包含正负号
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_show_int(interface, x, y, dat, num);
// 备注信息     去除整数部分无效的 0
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_show_int (display_interface_obj_struct *interface, uint16 x, uint16 y, const int32 dat, uint8 num)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(display_interface_assert(x < interface->display_width_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(y < interface->display_high_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(0 < num && 10 >= num))
        {
            return_state = DISPLAY_INTERFACE_ERROR_PARAMETER_ERROR;
            break;
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
        }

        {
            int32 dat_temp = dat;
            int32 offset = 1;
            char data_buffer[12];

            memset(data_buffer, 0, 12);
            memset(data_buffer, ' ', num+1);

            // 用来计算余数显示 123 显示 2 位则应该显示 23
            if(10 > num)
            {
                for(; 0 < num; num --)
                {
                    offset *= 10;
                }
                dat_temp %= offset;
            }
            zf_function_int_to_str(data_buffer, dat_temp);
            display_interface_show_string(interface, x, y, (const char *)&data_buffer);
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_high(interface->spi_cs_pin);
        }

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 显示 32bit 无符号整数 函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     dat                 需要显示的变量 数据类型 uint32
// 参数说明     num                 需要显示的位数 最高10位  不包含正负号
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_show_uint(interface, x, y, dat, num);
// 备注信息     去除整数部分无效的 0
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_show_uint (display_interface_obj_struct *interface, uint16 x, uint16 y, const uint32 dat, uint8 num)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(display_interface_assert(x < interface->display_width_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(y < interface->display_high_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(0 < num && 10 >= num))
        {
            return_state = DISPLAY_INTERFACE_ERROR_PARAMETER_ERROR;
            break;
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
        }

        {
            uint32 dat_temp = dat;
            int32 offset = 1;
            char data_buffer[12];

            memset(data_buffer, 0, 12);
            memset(data_buffer, ' ', num);

            // 用来计算余数显示 123 显示 2 位则应该显示 23
            if(10 > num)
            {
                for(; 0 < num; num --)
                {
                    offset *= 10;
                }
                dat_temp %= offset;
            }
            zf_function_uint_to_str(data_buffer, dat_temp);
            display_interface_show_string(interface, x, y, (const char *)&data_buffer);
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_high(interface->spi_cs_pin);
        }

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 显示浮点数 函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     dat                 需要显示的变量 数据类型 double
// 参数说明     num                 整数位显示长度   最高8位  
// 参数说明     pointnum            小数位显示长度   最高6位
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_show_float(interface, x, y, dat, num, pointnum);
// 备注信息     去除整数部分无效的 0
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_show_float (display_interface_obj_struct *interface, uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(display_interface_assert(x < interface->display_width_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(y < interface->display_high_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(0 < num && 10 >= num))
        {
            return_state = DISPLAY_INTERFACE_ERROR_PARAMETER_ERROR;
            break;
        }
        if(display_interface_assert(0 < pointnum && 6 >= pointnum))
        {
            return_state = DISPLAY_INTERFACE_ERROR_PARAMETER_ERROR;
            break;
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
        }

        {
            double dat_temp = dat;
            double offset = 1.0;
            char data_buffer[17];

            memset(data_buffer, 0, 17);
            memset(data_buffer, ' ', num+pointnum + 2);

            // 用来计算余数显示 123 显示 2 位则应该显示 23
            for(; 0 < num; num --)
            {
                offset *= 10;
            }
            dat_temp = dat_temp - ((int)dat_temp / (int)offset) * offset;

            zf_function_double_to_str(data_buffer, dat_temp, pointnum);
            display_interface_show_string(interface, x, y, (const char *)&data_buffer);
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_high(interface->spi_cs_pin);
        }

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 显示浮点数 函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     *wave               波形数组指针
// 参数说明     width               波形实际宽度
// 参数说明     value_max           波形实际最大值
// 参数说明     dis_width           波形显示宽度 参数范围 [0, interface->display_width_max]
// 参数说明     dis_value_max       波形显示最大值 参数范围 [0, interface->display_high_max]
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_show_wave(interface, x, y, wave, width, value_max, dis_width, dis_value_max);
// 备注信息     去除整数部分无效的 0
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_show_wave (display_interface_obj_struct *interface, uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(display_interface_assert(x < interface->display_width_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(y < interface->display_high_max))
        {
            return_state = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR;
            break;
        }
        if(display_interface_assert(NULL != wave))
        {
            return_state = DISPLAY_INTERFACE_ERROR_PARAMETER_ERROR;
            break;
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
        }

        {
            uint32 i = 0, j = 0;
            uint32 width_index = 0, value_max_index = 0;
            uint16 data_buffer[512];
            interface->set_region(interface, x, y, x + dis_width - 1, y + dis_value_max - 1);
            for(i = 0; i < dis_value_max; i ++)
            {
                data_buffer[i] = interface->bgcolor;
            }
            for(j = 0; j < dis_width; j ++)
            {
                display_interface_write_16bit_array(interface, data_buffer, dis_value_max);
            }

            for(i = 0; i < dis_width; i ++)
            {
                width_index = i * width / dis_width;
                value_max_index = *(wave + width_index) * (dis_value_max - 1) / value_max;
                interface->set_region(interface, i + x, (dis_value_max - 1) - value_max_index + y, i + x, (dis_value_max - 1) - value_max_index + y);
                display_interface_write_16bit_data(interface, interface->pencolor);
            }
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_high(interface->spi_cs_pin);
        }

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 接口注销初始化
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_deinit(interface);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_deinit (display_interface_obj_struct *interface)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(DISPLAY_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_spi_deinit (interface->spi_index);
            zf_gpio_deinit(interface->spi_rst_pin);
            zf_gpio_deinit(interface->spi_dc_pin );
            zf_gpio_deinit(interface->spi_cs_pin );
            zf_gpio_deinit(interface->spi_blk_pin);

            interface->spi_rst_pin  = PIN_NULL;
            interface->spi_dc_pin   = PIN_NULL;
            interface->spi_cs_pin   = PIN_NULL;
            interface->spi_blk_pin  = PIN_NULL;
        }
        else
        {
        }

        interface->interface_type       = DISPLAY_INTERFACE_TYPE_UNKNOW ;
        interface->communication_index  = 0                         ;

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY DEBUG 输出绑定初始化
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_debug_init(interface);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_debug_init (display_interface_obj_struct *interface)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        uint8 font_x_size = 0;
        uint8 font_y_size = 0;
        switch(interface->font_size)
        {
            case DISPLAY_INTERFACE_FONT_6X8:
            {
                font_x_size = 6;
                font_y_size = 8;
            }break;
            case DISPLAY_INTERFACE_FONT_8X16:
            {
                font_x_size = 8;
                font_y_size = 16;
            }break;
            default:
            {
            }break;
        }

        debug_screen_init(
            interface,
            interface->display_width_max,
            interface->display_high_max,
            font_x_size,
            font_y_size,
            NULL,// display_interface_show_string,
			NULL);// display_interface_clear);

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY SPI 通信 接口初始化
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     mode                SPI 模式        (详见 zf_driver_spi.h 内 zf_spi_mode_enum 定义)
// 参数说明     baudrate            设置 SPI 的波特率 不超过系统时钟的一半 会被适配成相近的速率
// 参数说明     sck_pin             选择 SCK 引脚   (详见 zf_driver_spi.h 内 zf_spi_sck_pin_enum 定义)
// 参数说明     mosi_pin            选择 MOSI 引脚  (详见 zf_driver_spi.h 内 zf_spi_mosi_pin_enum 定义)
// 参数说明     rst_pin             DISPLAY 复位控制引脚 所以此处填写 GPIO 索引
// 参数说明     dc_pin              DISPLAY 命令控制引脚 所以此处填写 GPIO 索引
// 参数说明     cs_pin              DISPLAY 软件片选引脚 所以此处填写 GPIO 索引
// 参数说明     blk_pin             DISPLAY 背光控制引脚 所以此处填写 GPIO 索引
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_spi_init(interface, spi_index, mode, baudrate, sck_pin, mosi_pin, rst_pin, dc_pin, cs_pin, blk_pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_spi_init (
    display_interface_obj_struct *interface,
    zf_spi_index_enum spi_index,
    zf_spi_mode_enum mode,
    uint32 baudrate,
    zf_spi_sck_pin_enum sck_pin,
    zf_spi_mosi_pin_enum mosi_pin,
    zf_gpio_pin_enum rst_pin,
    zf_gpio_pin_enum dc_pin ,
    zf_gpio_pin_enum cs_pin ,
    zf_gpio_pin_enum blk_pin)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(DISPLAY_INTERFACE_TYPE_UNKNOW != interface->interface_type)
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_OCCUPIED;
            break;
        }

        if(zf_spi_init(spi_index, mode, baudrate, sck_pin, mosi_pin, SPI_MISO_NULL, SPI_CS_NULL))
        {
            return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_OCCUPIED;
            break;
        }
        zf_gpio_init(rst_pin, GPO_PUSH_PULL, GPIO_LOW );
        zf_gpio_init(dc_pin , GPO_PUSH_PULL, GPIO_LOW );
        zf_gpio_init(cs_pin , GPO_PUSH_PULL, GPIO_HIGH);
        zf_gpio_init(blk_pin, GPO_PUSH_PULL, GPIO_HIGH);

        interface->interface_type   = DISPLAY_INTERFACE_TYPE_SPI;
        interface->spi_index        = spi_index                 ;
        interface->spi_rst_pin      = rst_pin                   ;
        interface->spi_dc_pin       = dc_pin                    ;
        interface->spi_cs_pin       = cs_pin                    ;
        interface->spi_blk_pin      = blk_pin                   ;

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
