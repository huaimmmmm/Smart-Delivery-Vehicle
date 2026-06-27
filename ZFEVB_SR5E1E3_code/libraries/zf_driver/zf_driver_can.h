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
* 文件名称          zf_driver_can
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

#ifndef _zf_driver_can_h_
#define _zf_driver_can_h_

// SDK 底层驱动
#include "can.h"
#include "can_private.h"

// zf_common 层引用
#include "zf_common_typedef.h"

// zf_driver 层 类型定义 引用
#include "zf_driver_gpio.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_can_write_frame                                                           // CAN 发送标准帧
// zf_can_read_frame                                                            // CAN 阻塞接收标准帧
// zf_can_query_frame                                                           // CAN 查询接收标准帧

// zf_can_set_interrupt_callback                                                // CAN 中断设置回调函数
// zf_can_set_interrupt_config                                                  // CAN 设置中断配置
// zf_can_set_interrupt_priority                                                // CAN 设置中断优先级

// zf_can_set_bps                                                               // CAN 设置速率

// zf_can_enable                                                                // CAN 使能
// zf_can_disable                                                               // CAN 禁止

// zf_can_deinit                                                                // CAN 注销初始化
// zf_can_init                                                                  // CAN 初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 CAN 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define     CAN_NUM_MAX             (  4     )                                  // 总共最多 2 个 CAN

typedef enum                                                                    // 枚举 CAN 模块号 此枚举定义不允许用户修改
{
   CAN_1                               ,
   CAN_2                               ,
   CAN_3                               ,
   CAN_4                               ,
}zf_can_index_enum;

typedef enum
{
   CAN_BSP_1MHZ         = 1000000U  ,
   CAN_BSP_2MHZ         = 2000000U  ,
   CAN_BSP_4MHZ         = 4000000U  ,
}zf_can_bsp_enum;

#define     CAN_PIN_INDEX_OFFSET    ( 0      )                                  // 低位开始 先存储 GPIO 的索引号
#define     CAN_PIN_INDEX_MASK      ( 0x0FFF )                                  // 使用低 12bit 存储 GPIO 索引号 因此掩码为 0xFFF

#define     CAN_PIN_AF_OFFSET       ( 12     )                                  // 累计低位的位移后从 12bit 开始
#define     CAN_PIN_AF_MASK         ( 0x0F   )                                  // 同样根据需要添加的引脚服用功能掩码 这里也是 4bit 因此掩码为 0xF

#define     CAN_INDEX_OFFSET        ( 16     )                                  // 累计低位的位移后从 16bit 开始
#define     CAN_INDEX_MASK          ( 0x0F   )                                  // 根据需要定义 CAN 索引掩码 这里是 4bit 因此掩码为 0xF

typedef enum                                                                    // 枚举 CAN_TX 引脚 此枚举定义不允许用户修改
{
   // CAN 1 TX 引脚
   CAN1_TX_A5         = ((CAN_1 << CAN_INDEX_OFFSET) | (GPIO_AF6 << CAN_PIN_AF_OFFSET) | (A5 )),

   // CAN 2 TX 引脚
   CAN2_TX_D7         = ((CAN_2 << CAN_INDEX_OFFSET) | (GPIO_AF6 << CAN_PIN_AF_OFFSET) | (D7 )),

   // CAN 4 TX 引脚
   CAN3_TX_G14        = ((CAN_4 << CAN_INDEX_OFFSET) | (GPIO_AF6 << CAN_PIN_AF_OFFSET) | (G14)),
   CAN3_TX_H12        = ((CAN_4 << CAN_INDEX_OFFSET) | (GPIO_AF6 << CAN_PIN_AF_OFFSET) | (H12)),

}zf_can_tx_pin_enum;

typedef enum                                                                   // 枚举 CAN_RX 引脚 此枚举定义不允许用户修改
{
   // CAN 1 RX 引脚
   CAN1_RX_A4         = ((CAN_1 << CAN_INDEX_OFFSET) | (GPIO_AF6 << CAN_PIN_AF_OFFSET) | (A4 )),

   // CAN 2 RX 引脚
   CAN2_RX_D8         = ((CAN_2 << CAN_INDEX_OFFSET) | (GPIO_AF6 << CAN_PIN_AF_OFFSET) | (D8 )),

   // CAN 4 RX 引脚
   CAN4_RX_G15        = ((CAN_4 << CAN_INDEX_OFFSET) | (GPIO_AF6 << CAN_PIN_AF_OFFSET) | (G15)),
   CAN4_RX_H13        = ((CAN_4 << CAN_INDEX_OFFSET) | (GPIO_AF6 << CAN_PIN_AF_OFFSET) | (H13)),

}zf_can_rx_pin_enum;

typedef enum                                                                    // 枚举 CAN 中断模式 此枚举定义不允许用户修改
{
   CAN_INTERRUPT_CONFIG_DISABLE         ,                                       // 禁止中断
   CAN_INTERRUPT_CONFIG_RX              ,                                       // 开启接收中断
}zf_can_interrupt_config_enum;

typedef enum                                                                    // 枚举 CAN 中断状态 此枚举定义不允许用户修改
{
    CAN_INTERRUPT_STATE_NONE    = 0x00  ,

    CAN_INTERRUPT_STATE_RX      = 0x01  ,
}zf_can_interrupt_state_enum;

#define CAN_DEFAULT_TIMEOUT_COUNT   ( 0x001FFFFF )                              // 超时计数不能太短 否则会直接导致通信异常

typedef enum                                                                    // 枚举 CAN 操作状态   此枚举定义不允许用户修改
{
    CAN_OPERATION_DONE              = ZF_NO_ERROR   ,                           // CAN 操作完成
    CAN_ERROR_UNKNOW                = ZF_ERROR      ,                           // CAN 未知错误 操作无法进行 正常情况下不会出现此错误

    CAN_WARNING_MODULE_DISABLE      = 0x10          ,                           // CAN 模块失能禁用 操作中断退出
    CAN_WARNING_OPERATION_TIMEOUT                   ,                           // CAN 操作超时退出 操作中断退出
    CAN_WARNING_NO_DATA                             ,                           // CAN 查询数据为空 操作无法进行

    CAN_ERROR_MODULE_OCCUPIED       = 0x20          ,                           // CAN 模块已被占用 操作无法进行
    CAN_ERROR_MODULE_NOT_INIT                       ,                           // CAN 模块未初始化 操作无法进行

    CAN_ERROR_TX_PIN_MISMATCH                       ,                           // CAN 引脚不匹配 操作无法进行
    CAN_ERROR_RX_PIN_MISMATCH                       ,                           // CAN 引脚不匹配 操作无法进行

    CAN_ERROR_BSP_UNSUPPORTED                       ,                           // CAN 传输速率错误 操作无法进行
    CAN_ERROR_INTERRUPT_CALLBACK_NOT_SET            ,                           // CAN 中断回调未设 操作无法进行
    CAN_ERROR_INTERRUPT_CALLBACK_ILLEGAL            ,                           // CAN 中断回调异常 操作无法进行

    CAN_ERROR_DATA_BUFFER_NULL                      ,                           // CAN 数据指针异常 操作无法进行
}zf_can_operation_state_enum;

typedef struct                                                                  // 定义 CAN 设备对象 此类型定义不允许用户修改
{
   can_driver_t         *can_ptr        ;                                       // CAN 模块指针
   uint32               bps             ;                                       // CAN 通信速率

   zf_gpio_pin_enum     tx_pin          ;                                       // CAN TX 引脚
   zf_gpio_pin_enum     rx_pin          ;                                       // CAN RX 引脚

    union
    {
        uint32          config_info     ;                                       // 配置整体数据
        struct
        {
            uint32      enable      : 8 ;                                       // 使能
            uint32      tx_interrupt: 8 ;                                       // 中断
            uint32      rx_interrupt: 8 ;                                       // 中断
            uint32      self_index  : 8 ;                                       // 自身索引
        };
    };
}zf_can_obj_struct;

typedef struct                                                                  // 定义 CAN 通信标准帧数据类型 此类型定义不允许用户修改
{
   uint16              id              ;
   uint8               data[8]         ;
}zf_can_message_buffer_struct;

#define CAN_DEFAULT_FILTER_ID               0x780UL                             // 初始过滤 ID
#define CAN_DEFAULT_FILTER_ID_RANGE         127                                 // 允许识别的连续 ID 范围 从初始过滤 ID 到 初始过滤 ID + CAN_DEFAULT_FILTER_ID_RANGE
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_assert_handler(file, line, tips);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_assert_handler (char *file, int line, char *tips);
#define zf_can_assert(status)       ((status) ? (0) : zf_can_assert_handler(__FILE__, __LINE__, NULL))
#define zf_can_log(status, tips)    ((status) ? (0) : zf_can_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 发送标准帧
// 参数说明     can_index           CAN 模块号 (详见 zf_driver_can.h 内 zf_can_index_enum 定义)
// 参数说明     *message_buffer     CAN 缓冲区 (详见 zf_driver_can.h 内 zf_can_message_buffer_struct 定义)
// 参数说明     data_len            数据长度 [1:8]
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_write_frame(can_index, message_buffer, data_len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_write_frame (zf_can_index_enum can_index, zf_can_message_buffer_struct *message_buffer, uint8 data_len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 阻塞接收标准帧
// 参数说明     can_index           CAN 模块号 (详见 zf_driver_can.h 内 zf_can_index_enum 定义)
// 参数说明     *message_buffer     CAN 缓冲区 (详见 zf_driver_can.h 内 zf_can_message_buffer_struct 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_read_frame(can_index, message_buffer, data_len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_read_frame (zf_can_index_enum can_index, zf_can_message_buffer_struct *message_buffer);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 查询接收标准帧
// 参数说明     can_index           CAN 模块号 (详见 zf_driver_can.h 内 zf_can_index_enum 定义)
// 参数说明     *message_buffer     CAN 缓冲区 (详见 zf_driver_can.h 内 zf_can_message_buffer_struct 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_query_frame(can_index, message_buffer, data_len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_query_frame (zf_can_index_enum can_index, zf_can_message_buffer_struct *message_buffer);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 注销初始化
// 参数说明     can_index           CAN 模块号 (详见 zf_driver_can.h 内 zf_can_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_deinit(can_index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_deinit (zf_can_index_enum can_index);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 初始化
// 参数说明     can_index           CAN 模块号 (详见 zf_driver_can.h 内 zf_can_index_enum 定义)
// 参数说明     bps                 CAN 比特率 (详见 zf_driver_can.h 内 zf_can_bsp_enum 定义)
// 参数说明     tx_pin              CAN 发送脚 (详见 zf_driver_can.h 内 zf_can_tx_pin_enum 定义)
// 参数说明     rx_pin              CAN 接收脚 (详见 zf_driver_can.h 内 zf_can_rx_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_init(can_index, bps, tx_pin, rx_pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_init (zf_can_index_enum can_index, zf_can_bsp_enum bps, zf_can_tx_pin_enum tx_pin, zf_can_rx_pin_enum rx_pin);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
