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
* 文件名称          zf_device_ips200
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

/*********************************************************************************************************************
* 接线定义          查看 zf_device_display_interface.h 中接口资源定义
********************************************************************************************************************/

// 自身头文件
#include "zf_device_ips200.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 设置显示区域
// 参数说明     x1              起始x轴坐标
// 参数说明     y1              起始y轴坐标
// 参数说明     x2              结束x轴坐标
// 参数说明     y2              结束y轴坐标
// 返回参数     void
// 使用示例     ips200_set_region(0, 0, ips200_x_max - 1, ips200_y_max - 1);
// 备注信息     内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static uint8 ips200_set_region (void *obj, uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
	ips200_operation_state_enum return_state = IPS200_ERROR_UNKNOW_ERROR;

    do
    {
        // if(display_interface_assert(NULL != obj))
        // {
        //     return_state = IPS200_ERROR_OBJ_BUFFER_NULL;
        //     break;
        // }
        display_interface_obj_struct *interface = (display_interface_obj_struct *)obj;

        // if(display_interface_assert(x1 < interface->display_width_max))
        // {
        //     return_state = IPS200_ERROR_DISPLAY_ZONE_ERROR;
        //     break;
        // }
        // if(display_interface_assert(x2 < interface->display_width_max))
        // {
        //     return_state = IPS200_ERROR_DISPLAY_ZONE_ERROR;
        //     break;
        // }
        // if(display_interface_assert(y1 < interface->display_high_max))
        // {
        //     return_state = IPS200_ERROR_DISPLAY_ZONE_ERROR;
        //     break;
        // }
        // if(display_interface_assert(y2 < interface->display_high_max))
        // {
        //     return_state = IPS200_ERROR_DISPLAY_ZONE_ERROR;
        //     break;
        // }

        display_interface_write_command(interface, 0x2a);
        display_interface_write_16bit_data(interface, x1);
        display_interface_write_16bit_data(interface, x2);
        
        display_interface_write_command(interface, 0x2b);
        display_interface_write_16bit_data(interface, y1);
        display_interface_write_16bit_data(interface, y2);
        
        display_interface_write_command(interface, 0x2c);

        return_state = IPS200_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 液晶初始化
// 参数说明     type_select         两寸屏接口类型 (详见 zf_device_ips200.h 内 ips200_type_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     ips200_init(type_select);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 ips200_init (ips200_type_enum type_select)
{
    display_interface_operation_state_enum return_state = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(IPS200_TYPE_SPI == type_select)
        {
            if(display_interface_assert(!display_interface_spi_default_init(&display_interface_default_obj, SPI_MODE0)))
            {
                return_state = DISPLAY_INTERFACE_ERROR_INTERFACE_OCCUPIED;
                break;
            }
        }
        else
        {
        }
        display_interface_default_obj.set_region = ips200_set_region;

        zf_gpio_high(display_interface_default_obj.spi_blk_pin);
        zf_gpio_low(display_interface_default_obj.spi_rst_pin);
        display_interface_delay_ms(5);
        zf_gpio_high(display_interface_default_obj.spi_rst_pin);
        display_interface_delay_ms(120);

        if(DISPLAY_INTERFACE_TYPE_SPI == display_interface_default_obj.interface_type)
        {
            zf_gpio_low(display_interface_default_obj.spi_cs_pin);
        }

        display_interface_write_command(&display_interface_default_obj, 0x11);
        display_interface_delay_ms(120);

        display_interface_write_command(&display_interface_default_obj, 0x36);
        switch(display_interface_default_obj.dir_mode)
        {
            case DISPLAY_INTERFACE_DIR_PORTRAIT     :
            {
                display_interface_default_obj.display_width_max  = 240;
                display_interface_default_obj.display_high_max   = 320;
                display_interface_write_8bit_data(&display_interface_default_obj, 0x00);
            }break;
            case DISPLAY_INTERFACE_DIR_PORTRAIT_180 :
            {
                display_interface_default_obj.display_width_max  = 240;
                display_interface_default_obj.display_high_max   = 320;
                display_interface_write_8bit_data(&display_interface_default_obj, 0xC0);
            }break;
            case DISPLAY_INTERFACE_DIR_CROSSWISE    :
            {
                display_interface_default_obj.display_width_max  = 320;
                display_interface_default_obj.display_high_max   = 240;
                display_interface_write_8bit_data(&display_interface_default_obj, 0x70);
            }break;
            case DISPLAY_INTERFACE_DIR_CROSSWISE_180:
            {
                display_interface_default_obj.display_width_max  = 320;
                display_interface_default_obj.display_high_max   = 240;
                display_interface_write_8bit_data(&display_interface_default_obj, 0xA0);
            }break;
        }

        display_interface_write_command(&display_interface_default_obj, 0x3A);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x05);

        display_interface_write_command(&display_interface_default_obj, 0xB2);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x0C);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x0C);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x00);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x33);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x33);

        display_interface_write_command(&display_interface_default_obj, 0xB7);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x35);

        // 32 Vcom=1.35V
        display_interface_write_command(&display_interface_default_obj, 0xBB);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x29);

        display_interface_write_command(&display_interface_default_obj, 0xC2);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x01);

        // GVDD=4.8V 
        display_interface_write_command(&display_interface_default_obj, 0xC3);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x19);

        // VDV, 0x20:0v
        display_interface_write_command(&display_interface_default_obj, 0xC4);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x20);

        // VCOM Offset Set
        display_interface_write_command(&display_interface_default_obj, 0xC5);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x1A);

        // 0x0F:60Hz
        display_interface_write_command(&display_interface_default_obj, 0xC6);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x01F);

        display_interface_write_command(&display_interface_default_obj, 0xD0);
        display_interface_write_8bit_data(&display_interface_default_obj, 0xA4);
        display_interface_write_8bit_data(&display_interface_default_obj, 0xA1);

        display_interface_write_command(&display_interface_default_obj, 0xE0);
        display_interface_write_8bit_data(&display_interface_default_obj, 0xD0);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x08);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x0E);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x09);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x09);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x05);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x31);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x33);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x48);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x17);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x14);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x15);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x31);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x34);

        display_interface_write_command(&display_interface_default_obj, 0xE1);
        display_interface_write_8bit_data(&display_interface_default_obj, 0xD0);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x08);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x0E);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x09);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x09);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x15);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x31);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x33);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x48);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x17);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x14);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x15);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x31);
        display_interface_write_8bit_data(&display_interface_default_obj, 0x34);

        display_interface_write_command(&display_interface_default_obj, 0x21);

        display_interface_write_command(&display_interface_default_obj, 0x29);

        if(DISPLAY_INTERFACE_TYPE_SPI == display_interface_default_obj.interface_type)
        {
            zf_gpio_high(display_interface_default_obj.spi_cs_pin);
        }

        display_interface_clear(&display_interface_default_obj);
        display_interface_debug_init(&display_interface_default_obj);

        return_state = DISPLAY_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
