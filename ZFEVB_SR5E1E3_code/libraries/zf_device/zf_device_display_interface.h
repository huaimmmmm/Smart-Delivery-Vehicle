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

#ifndef _zf_device_display_interface_h_
#define _zf_device_display_interface_h_

// 如果移植到其他平台 这里就替换成自己的资源头文件
#include "zf_common_headfile.h"
#include "zf_driver_headfile.h"

// 引用字体文件
#include "zf_device_display_font.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// display_interface_assert                                                     // DISPLAY 断言处理
// display_interface_log                                                        // DISPLAY Log 输出
// display_interface_delay_ms                                                   // DISPLAY 延时

// display_interface_write_8bit_register                                        // DISPLAY 写寄存器 8bit
// display_interface_write_8bit_registers                                       // DISPLAY 写寄存器 8bit
// display_interface_read_8bit_register                                         // DISPLAY 读寄存器 8bit
// display_interface_read_8bit_registers                                        // DISPLAY 读数据 8bit
// display_interface_transfer_8bit_array                                        // DISPLAY 双向传输 8bit 数据

// display_interface_deinit                                                     // DISPLAY 接口注销初始化

// display_interface_spi_init                                                   // DISPLAY SPI 通信 接口初始化
// display_interface_soft_iic_init                                              // DISPLAY SOFT_IIC 通信 接口初始化

// display_interface_spi_default_init                                           // DISPLAY 使用默认引脚进行 SPI 通信 接口初始化
// display_interface_soft_iic_default_init                                      // DISPLAY 使用默认引脚进行 SOFT_IIC 通信 接口初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 DISPLAY 相关的接口资源 这里允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// 在 逐飞科技 开源库中默认支持多种驱动模式
// 自行移植的话看情况是否需要此处的多方式驱动结构
//====================================================硬件 SPI 驱动====================================================
#ifdef _zf_driver_spi_h_

#define DISPLAY_INTERFACE_SPI_INDEX     ( SPI_4             )                   // 硬件 SPI 号
#define DISPLAY_INTERFACE_SPI_SPEED     ( 30 * 1000 * 1000  )                   // 硬件 SPI 速率
#define DISPLAY_INTERFACE_SPI_SCL_PIN   ( SPI4_SCK_G5       )                   // 硬件 SPI SCK  引脚  连接 DISPLAY 的 SPC 引脚
#define DISPLAY_INTERFACE_SPI_SDA_PIN   ( SPI4_MOSI_G7      )                   // 硬件 SPI MOSI 引脚  连接 DISPLAY 的 SDI 引脚

#define DISPLAY_INTERFACE_SPI_RST_PIN   ( G6                )                   // DISPLAY 复位控制引脚
#define DISPLAY_INTERFACE_SPI_DC_PIN    ( H1                )                   // DISPLAY 命令控制引脚
#define DISPLAY_INTERFACE_SPI_CS_PIN    ( H12               )                   // DISPLAY 片选引脚  连接 DISPLAY 的 CS 引脚
#define DISPLAY_INTERFACE_SPI_BLK_PIN   ( H13               )                   // DISPLAY 背光控制引脚

#endif
//====================================================硬件 SPI 驱动====================================================

//====================================================软件 并口 驱动====================================================
// 暂不支持
//====================================================软件 并口 驱动====================================================

#define DISPLAY_DIR_DEFAULT             ( DISPLAY_INTERFACE_DIR_PORTRAIT    )   // 在这设置默认的 显示方向
#define DISPLAY_FONT_SIZE_DEFAULT       ( DISPLAY_INTERFACE_FONT_8X16       )   // 在这设置默认的 字体大小
#define DISPLAY_PENCOLOR_DEFAULT        ( RGB565_WHITE                      )   // 在这设置默认的 显示颜色
#define DISPLAY_BGCOLOR_SIZE_DEFAULT    ( RGB565_BLACK                      )   // 在这设置默认的 背景颜色

// 独立内存管理部分 默认使用 逐飞科技 开源库中的内存分段定义
// 如果移植到其它平台后此处报错 可以将 DISPLAY_INTERFACE_USE_ZF_COMMON_MEMORY 修改为 0
// 当 DISPLAY_INTERFACE_USE_ZF_COMMON_MEMORY 为 0 会禁止内存分段指定 通过编译器随机分配
#define     DISPLAY_INTERFACE_USE_ZF_COMMON_MEMORY  ( 1 )
#if         DISPLAY_INTERFACE_USE_ZF_COMMON_MEMORY
#ifndef _zf_common_memory_h_
#define AT_ZF_LIB_SECTION        
#define AT_ZF_LIB_SECTION_START  
#define AT_ZF_LIB_SECTION_END    
#endif
#else
#define AT_ZF_LIB_SECTION        
#define AT_ZF_LIB_SECTION_START  
#define AT_ZF_LIB_SECTION_END    
#endif

// 调试信息输出部分 默认使用 逐飞科技 开源库中的调试接口
// 如果移植到其它平台后此处报错 可以将 DISPLAY_INTERFACE_USE_ZF_COMMON_DEBUG 修改为 0
// 当 DISPLAY_INTERFACE_USE_ZF_COMMON_DEBUG 为 0 会通过 display_assert 使用 printf 输出报错
#define     DISPLAY_INTERFACE_USE_ZF_COMMON_DEBUG   ( 1 )
#if         DISPLAY_INTERFACE_USE_ZF_COMMON_DEBUG
#ifndef _zf_common_debug_h_
#define zf_assert(pass)         ((pass) ? (0) : (1))
#define zf_log(pass, str)       ((pass) ? (0) : (1))
#endif
#endif

// 延时定向部分 默认使用 逐飞科技 开源库中的延时接口
// 如果移植到其它平台后此处报错 可以将 DISPLAY_INTERFACE_USE_ZF_DIRVER_DELAY 修改为 0
// 当 DISPLAY_INTERFACE_USE_ZF_DIRVER_DELAY 为 0 会通过 display 的软延时实现
#define     DISPLAY_INTERFACE_USE_ZF_DIRVER_DELAY   ( 1 )
#if         DISPLAY_INTERFACE_USE_ZF_DIRVER_DELAY
#ifndef _zf_driver_delay_h_
#define zf_delay_ms(time)   for(vuint32 i = time * 1000000; i --; )
#endif
#endif

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 DISPLAY 相关的控制对象格式 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef enum
{
    DISPLAY_INTERFACE_TYPE_UNKNOW   = 0 ,

    DISPLAY_INTERFACE_TYPE_SPI          ,                                           // SPI 驱动
    DISPLAY_INTERFACE_TYPE_PARALLEL8    ,                                           // 并口驱动
}display_interface_type_enum;

typedef enum
{
    DISPLAY_INTERFACE_DIR_PORTRAIT      = 0,                                    // 竖屏模式
    DISPLAY_INTERFACE_DIR_PORTRAIT_180  = 1,                                    // 竖屏模式  旋转180
    DISPLAY_INTERFACE_DIR_CROSSWISE     = 2,                                    // 横屏模式
    DISPLAY_INTERFACE_DIR_CROSSWISE_180 = 3,                                    // 横屏模式  旋转180
}display_interface_dir_enum;

typedef enum
{
    DISPLAY_INTERFACE_FONT_6X8          = 0,                                    // 6x8      字体
    DISPLAY_INTERFACE_FONT_8X16         = 1,                                    // 8x16     字体
}display_interface_font_enum;

typedef enum
{
    DISPLAY_INTERFACE_OPERATION_DONE                = ZF_NO_ERROR   ,           // 操作完成
    DISPLAY_INTERFACE_ERROR_UNKNOW_ERROR            = ZF_ERROR      ,           // 异常错误 这个错误理论上不会被返回

    DISPLAY_INTERFACE_WARNING_MODULE_DISABLE        = 0x10          ,           // 模块禁止警告 模块未使能
    DISPLAY_INTERFACE_WARNING_OPERATION_TIMEOUT                     ,           // 模块超时警告 通信或者操作超时

    DISPLAY_INTERFACE_ERROR_INTERFACE_OCCUPIED      = 0x20          ,           // 通信接口占用错误 不能重复初始化同一个接口
    DISPLAY_INTERFACE_ERROR_INTERFACE_NOT_INIT                      ,           // 接口未初始化错误 不能未初始化就调用接口
    DISPLAY_INTERFACE_ERROR_INTERFACE_TYPE_UNKNOW                   ,           // 接口错误
 
    DISPLAY_INTERFACE_ERROR_OBJ_BUFFER_NULL                         ,           // 操作对象空指针 不可以传入空指针
    DISPLAY_INTERFACE_ERROR_DATA_BUFFER_NULL                        ,           // 数据缓冲区错误 不可以传入空指针
    DISPLAY_INTERFACE_ERROR_DISPLAY_ZONE_ERROR                      ,
    DISPLAY_INTERFACE_ERROR_PARAMETER_ERROR                         ,
}display_interface_operation_state_enum;

typedef uint8   (*display_func_set_region    )   (void *interface, uint16 x1, uint16 y1, uint16 x2, uint16 y2);

typedef struct                                                                  // DISPLAY 模块化解耦结构
{
    display_interface_type_enum             interface_type          ;           // 必须要的接口类型

    union
    {
        zf_spi_index_enum                   spi_index               ;           // 硬件 SPI 索引
        uint8                               communication_index     ;
    };
    uint8                                   dir_mode                ;
    uint8                                   font_size               ;

    uint16                                  bgcolor                 ;
    uint16                                  pencolor                ;

    zf_gpio_pin_enum                        parallel8_wr_pin        ;           // DISPLAY 输出控制引脚
    zf_gpio_pin_enum                        parallel8_rd_pin        ;           // DISPLAY 读取控制引脚
    zf_gpio_pin_enum                        spi_rst_pin             ;           // DISPLAY 复位控制引脚
    union
    {
        zf_gpio_pin_enum                    spi_dc_pin              ;           // DISPLAY 命令控制引脚
        zf_gpio_pin_enum                    parallel8_rs_pin        ;           // DISPLAY 命令控制引脚
    };
    zf_gpio_pin_enum                        spi_cs_pin              ;           // DISPLAY 片选引脚  连接 DISPLAY 的 CS 引脚
    zf_gpio_pin_enum                        spi_blk_pin             ;           // DISPLAY 背光控制引脚

    uint16                                  display_width_max       ;
    uint16                                  display_high_max        ;

    display_func_set_region                 set_region              ;
}display_interface_obj_struct;

// typedef uint8   (*display_func_get_acc      )   (display_interface_obj_struct *interface, void *result);
// typedef uint8   (*display_func_get_gyro     )   (display_interface_obj_struct *interface, void *result);
// typedef uint8   (*display_func_get_mag      )   (display_interface_obj_struct *interface, void *result);
// typedef uint8   (*display_func_set_config   )   (display_interface_obj_struct *interface, uint8 item);
// typedef uint8   (*display_func_ckeck_id     )   (display_interface_obj_struct *interface);
// typedef uint8   (*display_func_init         )   (display_interface_obj_struct *interface);

// typedef struct
// {
//     display_func_get_acc        get_acc     ;
//     display_func_get_gyro       get_gyro    ;
//     display_func_get_mag        get_mag     ;
//     display_func_set_config     set_config  ;
//     display_func_ckeck_id       ckeck_id    ;
//     display_func_init           init        ;
// }display_module_func_list;

extern display_interface_obj_struct display_interface_default_obj;
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
#define display_interface_assert(pass)      zf_assert(pass)
#define display_interface_log(pass, tips)   zf_log(pass, tips)
#else
uint8 display_interface_assert_handler (char *file, int line, char *tips);
#define display_interface_assert(status)    ((status) ? (0) : display_interface_assert_handler(__FILE__, __LINE__, NULL))
#define display_interface_log(status, tips) ((status) ? (0) : display_interface_assert_handler(__FILE__, __LINE__, (tips)))
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 延时
// 参数说明     ms                  毫秒数
// 返回参数     void
// 使用示例     display_interface_delay_ms(ms);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#if DISPLAY_INTERFACE_USE_ZF_DIRVER_DELAY
#define display_interface_delay_ms(ms)      zf_delay_ms(ms)
#else
void display_interface_delay_ms (uint32 ms);
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 双向传输 8bit 数据
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     cmd                 命令
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_write_8bit_data(interface, cmd);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_write_command (display_interface_obj_struct *interface, const uint8 cmd);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 写 8bit 数据
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     data                数据
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_write_8bit_data(interface, data);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_write_8bit_data (display_interface_obj_struct *interface, const uint8 data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 写 8bit 数据
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     data                数据
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_write_16bit_data(interface, data, len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_write_16bit_data (display_interface_obj_struct *interface, const uint16 data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 写 8bit 数据
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     *data               数据缓冲区
// 参数说明     len                 数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_write_8bit_array(interface, data, len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_write_8bit_array (display_interface_obj_struct *interface, const uint8 *data, uint32 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 写 8bit 数据
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     *data               数据缓冲区
// 参数说明     len                 数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_write_16bit_array(interface, data, len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_write_16bit_array (display_interface_obj_struct *interface, const uint16 *data, uint32 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 清屏函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 返回参数     void
// 使用示例     display_interface_clear(interface);
// 备注信息     将屏幕清空成背景颜色
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_clear (display_interface_obj_struct *interface);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 屏幕填充函数
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     color               RGB565 格式颜色 (详见 zf_device_display_font.h 内 rgb565_color_enum 定义) 或者自行写入
// 返回参数     void
// 使用示例     display_interface_full(interface, color);
// 备注信息     将屏幕清空成背景颜色
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_full (display_interface_obj_struct *interface, const uint16 color);

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
uint8 display_interface_draw_point (display_interface_obj_struct *interface, uint16 x, uint16 y, const uint16 color);

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
uint8 display_interface_draw_line (display_interface_obj_struct *interface, uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color);

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
uint8 display_interface_show_char (display_interface_obj_struct *interface, uint16 x, uint16 y, const char asc_ii_char);

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
uint8 display_interface_show_string (display_interface_obj_struct *interface, uint16 x, uint16 y, const char *asc_ii_string);

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
uint8 display_interface_show_int (display_interface_obj_struct *interface, uint16 x, uint16 y, const int32 dat, uint8 num);

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
uint8 display_interface_show_uint (display_interface_obj_struct *interface, uint16 x, uint16 y, const uint32 dat, uint8 num);

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
uint8 display_interface_show_float (display_interface_obj_struct *interface, uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum);

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
uint8 display_interface_show_wave (display_interface_obj_struct *interface, uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 接口注销初始化
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_deinit(interface);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_deinit (display_interface_obj_struct *interface);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY DEBUG 输出绑定初始化
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_debug_init(interface);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 display_interface_debug_init (display_interface_obj_struct *interface);

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
    zf_gpio_pin_enum blk_pin);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     DISPLAY 使用默认资源进行 SPI 通信 接口初始化
// 参数说明     *interface          接口对象 DISPLAY 系列通用的接口对象
// 参数说明     mode                SPI 模式        (详见 zf_driver_spi.h 内 spi_mode_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / DISPLAY_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     display_interface_spi_default_init(interface, mode);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define display_interface_spi_default_init(interface, mode) \
    display_interface_spi_init(interface, DISPLAY_INTERFACE_SPI_INDEX, mode, \
        DISPLAY_INTERFACE_SPI_SPEED, DISPLAY_INTERFACE_SPI_SCL_PIN, DISPLAY_INTERFACE_SPI_SDA_PIN, \
        DISPLAY_INTERFACE_SPI_RST_PIN, DISPLAY_INTERFACE_SPI_DC_PIN, \
        DISPLAY_INTERFACE_SPI_CS_PIN, DISPLAY_INTERFACE_SPI_BLK_PIN)
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
