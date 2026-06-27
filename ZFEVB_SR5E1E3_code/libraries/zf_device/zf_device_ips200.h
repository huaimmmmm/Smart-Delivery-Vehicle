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

#ifndef _zf_device_ips200_h_
#define _zf_device_ips200_h_

// zf_common 层 类型定义 引用
#include "zf_common_typedef.h"

// zf_device 层头文件引用
#include "zf_device_display_interface.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef enum
{
    IPS200_TYPE_SPI         ,                                                   // SPI 驱动
    IPS200_TYPE_PARALLEL8   ,                                                   // 并口驱动
}ips200_type_enum;

typedef enum
{
    IPS200_OPERATION_DONE               = DISPLAY_INTERFACE_OPERATION_DONE              ,
    IPS200_ERROR_UNKNOW_ERROR           = DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR          ,

    IPS200_WARNING_MODULE_DISABLE       = DISPLAY_INTERFACE_WARNING_MODULE_DISABLE      ,
    IPS200_WARNING_OPERATION_TIMEOUT    = DISPLAY_INTERFACE_WARNING_OPERATION_TIMEOUT   ,

    IPS200_ERROR_INTERFACE_OCCUPIED     = DISPLAY_INTERFACE_ERROR_INTERFACE_OCCUPIED    ,
    IPS200_ERROR_INTERFACE_NOT_INIT     = DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT    ,
    IPS200_ERROR_INTERFACE_TYPE_UNKNOW  = DISPLAY_INTERFACE_ERROR_INTERFACE_TYPE_UNKNOW ,

    IPS200_ERROR_OBJ_BUFFER_NULL        = DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL       ,
    IPS200_ERROR_DATA_BUFFER_NULL       = DISPLAY_INTERFACE_ERROR_DATA_BUFFER_NULL      ,
    IPS200_ERROR_DISPLAY_ZONE_ERROR     = DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR    ,
}ips200_operation_state_enum;

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 清屏函数
// 参数说明     void
// 返回参数     void
// 使用示例     ips200_clear();
// 备注信息     将屏幕清空成背景颜色
//-------------------------------------------------------------------------------------------------------------------
#define ips200_clear() \
        display_interface_clear(&display_interface_default_obj)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 屏幕填充函数
// 参数说明     color               RGB565 格式颜色 (详见 zf_device_display_font.h 内 rgb565_color_enum 定义) 或者自行写入
// 返回参数     void
// 使用示例     ips200_full(color);
// 备注信息     将屏幕清空成背景颜色
//-------------------------------------------------------------------------------------------------------------------
#define ips200_full(color) \
        display_interface_full(&display_interface_default_obj, color)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 画点函数
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     color               RGB565 格式颜色 (详见 zf_device_display_font.h 内 rgb565_color_enum 定义) 或者自行写入
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_draw_point(interface, x, y, color);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define ips200_draw_point(x, y, color) \
        display_interface_draw_point(&display_interface_default_obj, x, y, color);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 画线函数
// 参数说明     x_start             坐标 x 方向的起点 [0, interface->display_width_max - 1]
// 参数说明     y_start             坐标 y 方向的起点 [0, interface->display_high_max  - 1]
// 参数说明     x_end               坐标 x 方向的终点 [0, interface->display_width_max - 1]
// 参数说明     y_end               坐标 y 方向的终点 [0, interface->display_high_max  - 1]
// 参数说明     color               RGB565 格式颜色 (详见 zf_device_display_font.h 内 rgb565_color_enum 定义) 或者自行写入
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_draw_line(interface, x_start, y_start, x_end, y_end, color);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define ips200_draw_line(x_start, y_start, x_end, y_end, color) \
        display_interface_draw_line (&display_interface_default_obj, x_start, y_start, x_end, y_end, color)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示字符函数
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     asc_ii_char         需要显示的字符 asi-ii 编码
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_show_char(interface, x, y, asc_ii_char);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define ips200_show_char(x, y, asc_ii_char) \
        display_interface_show_char (&display_interface_default_obj, x, y, asc_ii_char)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示字符串函数
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     asc_ii_string       需要显示的字符串 asi-ii 编码
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_show_string(interface, x, y, asc_ii_string);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define ips200_show_string(x, y, asc_ii_string) \
        display_interface_show_string (&display_interface_default_obj, x, y, asc_ii_string)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示 32bit 有符号整数 函数
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     dat                 需要显示的变量 数据类型 int32
// 参数说明     num                 需要显示的位数 最高10位  不包含正负号
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_show_int(interface, x, y, dat, num);
// 备注信息     去除整数部分无效的 0
//-------------------------------------------------------------------------------------------------------------------
#define ips200_show_int(x, y, dat, num) \
        display_interface_show_int (&display_interface_default_obj, x, y, dat, num)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示 32bit 无符号整数 函数
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     dat                 需要显示的变量 数据类型 uint32
// 参数说明     num                 需要显示的位数 最高10位  不包含正负号
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_show_uint(interface, x, y, dat, num);
// 备注信息     去除整数部分无效的 0
//-------------------------------------------------------------------------------------------------------------------
#define ips200_show_uint(x, y, dat, num) \
        display_interface_show_uint (&display_interface_default_obj, x, y, dat, num)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示浮点数 函数
// 参数说明     x                   坐标 x [0, interface->display_width_max - 1]
// 参数说明     y                   坐标 y [0, interface->display_high_max  - 1]
// 参数说明     dat                 需要显示的变量 数据类型 double
// 参数说明     num                 整数位显示长度   最高8位  
// 参数说明     pointnum            小数位显示长度   最高6位
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_show_float(interface, x, y, dat, num, pointnum);
// 备注信息     去除整数部分无效的 0
//-------------------------------------------------------------------------------------------------------------------
#define ips200_show_float(x, y, dat, num, pointnum) \
        display_interface_show_float (&display_interface_default_obj, x, y, dat, num, pointnum)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示浮点数 函数
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
#define ips200_show_wave(x, y, wave, width, value_max, dis_width, dis_value_max) \
        display_interface_show_wave(&display_interface_default_obj, x, y, wave, width, value_max, dis_width, dis_value_max)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 液晶初始化
// 参数说明     type_select         两寸屏接口类型 (详见 zf_device_ips200.h 内 ips200_type_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     ips200_init(type_select);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 ips200_init (ips200_type_enum type_select);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
