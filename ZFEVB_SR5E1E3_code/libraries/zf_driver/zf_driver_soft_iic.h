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

#ifndef _zf_driver_soft_iic_h_
#define _zf_driver_soft_iic_h_

// zf_common 层 类型定义 引用
#include "zf_common_typedef.h"

// zf_driver 层 类型定义 引用
#include "zf_driver_gpio.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_soft_iic_write_8bit_splicing_array                                        // SOFT_IIC 接口传输 8bit 数组 写两个数组
// zf_soft_iic_write_16bit_splicing_array                                       // SOFT_IIC 接口传输 16bit 数组 写两个数组

// zf_soft_iic_transfer_8bit_array                                              // SOFT_IIC 接口传输 8bit 数组 先写后读取
// zf_soft_iic_transfer_16bit_array                                             // SOFT_IIC 接口传输 16bit 数组 先写后读取

// zf_soft_iic_sccb_write_register                                              // SOFT_IIC 接口 SCCB 模式向传感器寄存器写 8bit 数据
// zf_soft_iic_sccb_read_register                                               // SOFT_IIC 接口 SCCB 模式从传感器寄存器读取 8bit 数据

// zf_soft_iic_set_speed                                                        // SOFT_IIC 接口配置速度

// zf_soft_iic_enable                                                           // SOFT_IIC 接口 使能
// zf_soft_iic_disable                                                          // SOFT_IIC 接口 禁止

// zf_soft_iic_deinit                                                           // SOFT_IIC 接口注销初始化
// zf_soft_iic_init                                                             // SOFT_IIC 接口初始化 默认 MASTER 模式 不提供 SLAVE 模式
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 SOFT-IIC 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define         SOFT_IIC_MAX            ( 8 )                                   // 支持的最大的 SOFT_IIC 数量

typedef enum                                                                    // 枚举 SOFT_SPI 编号 此枚举定义不允许用户修改
{
    SOFT_IIC_1  = 0 ,
    SOFT_IIC_2      ,
    SOFT_IIC_3      ,
    SOFT_IIC_4      ,
    SOFT_IIC_5      ,
    SOFT_IIC_6      ,
    SOFT_IIC_7      ,
    SOFT_IIC_8      ,
}zf_soft_iic_index_enum;

typedef enum                                                                    // 枚举 SOFT_IIC 操作状态   此枚举定义不允许用户修改
{
    SOFT_IIC_OPERATION_DONE             = ZF_NO_ERROR   ,                       // SOFT_IIC 操作完成
    SOFT_IIC_ERROR_UNKNOW               = ZF_ERROR      ,                       // SOFT_IIC 未知错误 操作无法进行 正常情况下不会出现此错误

    SOFT_IIC_WARNING_MODULE_DISABLE     = 0x10          ,                       // SOFT_IIC 模块失能禁用 操作中断退出

    SOFT_IIC_ERROR_MODULE_OCCUPIED      = 0x20          ,                       // SOFT_IIC 模块已被占用 操作无法进行
    SOFT_IIC_ERROR_MODULE_NOT_INIT                      ,                       // SOFT_IIC 模块未初始化 操作无法进行

    SOFT_IIC_ERROR_SPEED_UNSUPPORTED                    ,                       // SOFT_IIC 传输速率错误 操作无法进行
    SOFT_IIC_ERROR_PIN_ILLEGAL                          ,                       // SOFT_IIC 引脚参数错误 操作无法进行

    SOFT_IIC_ERROR_DATA_BUFFER_NULL                     ,                       // SOFT_IIC 数据指针异常 操作无法进行
}zf_soft_iic_operation_state_enum;

typedef struct
{
    zf_gpio_pin_enum    scl_pin     ;                                           // 用于记录对应的引脚编号
    zf_gpio_pin_enum    sda_pin     ;                                           // 用于记录对应的引脚编号

    uint32              delay       ;
    uint32              enable      ;
}zf_soft_iic_obj_struct;

extern zf_soft_iic_obj_struct soft_iic_obj_list[SOFT_IIC_MAX];
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_assert_handler(file, line, tips);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_assert_handler (char *file, int line, char *tips);
#define zf_soft_iic_assert(status)      ((status) ? (0) : zf_soft_iic_assert_handler(__FILE__, __LINE__, NULL))
#define zf_soft_iic_log(status, tips)   ((status) ? (0) : zf_soft_iic_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口传输 8bit 数组 写两个数组
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     addr                器件地址
// 参数说明     *part1_buffer       发送数据存放缓冲区
// 参数说明     part1_len           发送缓冲区长度
// 参数说明     *part2_buffer       读取数据存放缓冲区
// 参数说明     part2_len           读取缓冲区长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_write_8bit_splicing_array(soft_iic_index, addr, write_buffer, write_len, read_buffer, read_len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_write_8bit_splicing_array (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint8 *part1_buffer, uint32 part1_len, const uint8 *part2_buffer, uint32 part2_len);

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
uint8 zf_soft_iic_write_16bit_splicing_array (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint16 *part1_buffer, uint32 part1_len, const uint16 *part2_buffer, uint32 part2_len);

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
uint8 zf_soft_iic_transfer_8bit_array (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint8 *write_buffer, uint32 write_len, uint8 *read_buffer, uint32 read_len);

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
uint8 zf_soft_iic_transfer_16bit_array (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint16 *write_buffer, uint32 write_len, uint16 *read_buffer, uint32 read_len);

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
uint8 zf_soft_iic_sccb_write_register (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint8 register_name, uint8 data);

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
uint8 zf_soft_iic_sccb_read_register (zf_soft_iic_index_enum soft_iic_index, uint8 addr, const uint8 register_name, uint8 *data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口配置速度
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     delay               软件 SPI 延时 就是时钟高电平时间 越短 SPI 速率越高 最小值为 1
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_set_speed(soft_iic_index, delay);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_set_speed (zf_soft_iic_index_enum soft_iic_index, uint32 delay);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口 使能
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_enable(soft_iic_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_enable (zf_soft_iic_index_enum soft_iic_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口 禁止
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_disable(soft_iic_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_disable (zf_soft_iic_index_enum soft_iic_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SOFT_IIC 接口注销初始化
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / SOFT_IIC_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_soft_iic_deinit(soft_iic_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_soft_iic_deinit (zf_soft_iic_index_enum soft_iic_index);

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
uint8 zf_soft_iic_init (zf_soft_iic_index_enum soft_iic_index, uint32 delay, zf_gpio_pin_enum scl_pin, zf_gpio_pin_enum sda_pin);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif

