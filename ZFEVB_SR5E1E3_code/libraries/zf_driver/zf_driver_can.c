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

// zf_common 层引用
#include "zf_common_debug.h"
#include "zf_common_fifo.h"
#include "zf_common_memory.h"

// zf_driver 层引用
#include "zf_driver_interrupt.h"
#include "zf_driver_rcc.h"

// 自身头文件
#include "zf_driver_can.h"

// 此处定义 本文件用使用的模块管理数据 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION zf_can_obj_struct can_obj_list[CAN_NUM_MAX] = 
{
    {.can_ptr = &DRV_CAN1, .bps = 0, .tx_pin = PIN_NULL, .rx_pin = PIN_NULL, .config_info = 0},
    {.can_ptr = &DRV_CAN2, .bps = 0, .tx_pin = PIN_NULL, .rx_pin = PIN_NULL, .config_info = 0},
    {.can_ptr = &DRV_CAN3, .bps = 0, .tx_pin = PIN_NULL, .rx_pin = PIN_NULL, .config_info = 0},
    {.can_ptr = &DRV_CAN4, .bps = 0, .tx_pin = PIN_NULL, .rx_pin = PIN_NULL, .config_info = 0},
};

AT_ZF_LIB_SECTION zf_can_message_buffer_struct can_message_buffer[CAN_NUM_MAX] = 
{
    {.id = 0xFFFF, .data = {0, 0, 0, 0, 0, 0, 0, 0}},
    {.id = 0xFFFF, .data = {0, 0, 0, 0, 0, 0, 0, 0}},
    {.id = 0xFFFF, .data = {0, 0, 0, 0, 0, 0, 0, 0}},
    {.id = 0xFFFF, .data = {0, 0, 0, 0, 0, 0, 0, 0}}
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件用使用的静态或常亮数据 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 用于固定值或指向的内容 主要是用少量存储占用来减少栈开销
static const zf_interrupt_index_enum can_irq_index_list[CAN_NUM_MAX] =
{
    INTERRUPT_INDEX_CAN1, INTERRUPT_INDEX_CAN2, INTERRUPT_INDEX_CAN3, INTERRUPT_INDEX_CAN4
};

static const zf_rcc_index_enum can_rcc_index_list[CAN_NUM_MAX] = 
{
    RCC_INDEX_FDCAN1, RCC_INDEX_FDCAN2, RCC_INDEX_FDCAN3, RCC_INDEX_FDCAN4
};
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 接收中断回调
// 参数说明     *cdp            CAN 实例
// 参数说明     frame           CAN 帧 实例
// 参数说明     event           触发事件
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void can_receive_cb (can_driver_t *cdp, can_frame_t frame, can_evt_t event)
{
    (void)frame;
    (void)event;

    if ((CAN_CB_RX == event) &&
        (CAN_ID_STD == frame.TYPE))
    {
        for(uint8 i = 0; i < CAN_NUM_MAX; i ++)
        {
            if(cdp == can_obj_list[i].can_ptr)
            {
                can_message_buffer[i].id = frame.ID;
                can_message_buffer[i].data[0] = frame.data8[0];
                can_message_buffer[i].data[1] = frame.data8[1];
                can_message_buffer[i].data[2] = frame.data8[2];
                can_message_buffer[i].data[3] = frame.data8[3];
                can_message_buffer[i].data[4] = frame.data8[4];
                can_message_buffer[i].data[5] = frame.data8[5];
                can_message_buffer[i].data[6] = frame.data8[6];
                can_message_buffer[i].data[7] = frame.data8[7];
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_assert_handler(file, line, tips);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 发送标准帧
// 参数说明     can_index           CAN 模块号 (详见 zf_driver_can.h 内 zf_can_index_enum 定义)
// 参数说明     *message_buffer     CAN 缓冲区 (详见 zf_driver_can.h 内 zf_can_message_buffer_struct 定义)
// 参数说明     data_len            数据长度 [1:8]
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_write_frame(can_index, message_buffer, data_len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_write_frame (zf_can_index_enum can_index, zf_can_message_buffer_struct *message_buffer, uint8 data_len)
{
    zf_can_operation_state_enum return_state = CAN_ERROR_UNKNOW;

    do
    {
        if(zf_can_assert(can_obj_list[can_index].bps))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = CAN_ERROR_MODULE_NOT_INIT;                           // CAN 模块未初始化 操作无法进行
            break;
        }
        if(zf_can_assert(NULL != message_buffer))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = CAN_ERROR_DATA_BUFFER_NULL;                          // CAN 数据指针异常 操作无法进行
            break;
        }

        can_frame_t frame;
        frame.TYPE = CAN_ID_STD;                                                    // 标准 ID
        frame.DLC = data_len;                                                       // 默认数据长
        frame.OPERATION = CAN_OPERATION_NORMAL;                                     // 正常模式
        frame.ID = message_buffer->id;
        frame.data8[0] = message_buffer->data[0];
        frame.data8[1] = message_buffer->data[1];
        frame.data8[2] = message_buffer->data[2];
        frame.data8[3] = message_buffer->data[3];
        frame.data8[4] = message_buffer->data[4];
        frame.data8[5] = message_buffer->data[5];
        frame.data8[6] = message_buffer->data[6];
        frame.data8[7] = message_buffer->data[7];
        can_transmit(can_obj_list[can_index].can_ptr, CAN_TX_ANY_BUFFER, &frame);

        return_state = CAN_OPERATION_DONE;
    }while(0);

return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 阻塞接收标准帧
// 参数说明     can_index           CAN 模块号 (详见 zf_driver_can.h 内 zf_can_index_enum 定义)
// 参数说明     *message_buffer     CAN 缓冲区 (详见 zf_driver_can.h 内 zf_can_message_buffer_struct 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_read_frame(can_index, message_buffer, data_len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_read_frame (zf_can_index_enum can_index, zf_can_message_buffer_struct *message_buffer)
{
    zf_can_operation_state_enum return_state = CAN_ERROR_UNKNOW;

    do
    {
        if(zf_can_assert(can_obj_list[can_index].bps))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = CAN_ERROR_MODULE_NOT_INIT;                           // CAN 模块未初始化 操作无法进行
            break;
        }
        if(zf_can_assert(NULL != message_buffer))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = CAN_ERROR_DATA_BUFFER_NULL;                          // CAN 数据指针异常 操作无法进行
            break;
        }

        vuint32 timeout_count   = 0;
        while(timeout_count ++ <= CAN_DEFAULT_TIMEOUT_COUNT && 0xFFFF == can_message_buffer[can_index].id);
        if(timeout_count >= CAN_DEFAULT_TIMEOUT_COUNT)
        {
            return_state = CAN_WARNING_OPERATION_TIMEOUT;                       // UART 操作超时退出 操作中断退出
            break;
        }

        memcpy(message_buffer, &can_message_buffer[can_index], sizeof(zf_can_message_buffer_struct));
        can_message_buffer[can_index].id = 0xFFFF;

        return_state = CAN_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 查询接收标准帧
// 参数说明     can_index           CAN 模块号 (详见 zf_driver_can.h 内 zf_can_index_enum 定义)
// 参数说明     *message_buffer     CAN 缓冲区 (详见 zf_driver_can.h 内 zf_can_message_buffer_struct 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_query_frame(can_index, message_buffer, data_len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_query_frame (zf_can_index_enum can_index, zf_can_message_buffer_struct *message_buffer)
{
    zf_can_operation_state_enum return_state = CAN_ERROR_UNKNOW;

    do
    {
        if(zf_can_assert(can_obj_list[can_index].bps))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = CAN_ERROR_MODULE_NOT_INIT;                           // CAN 模块未初始化 操作无法进行
            break;
        }
        if(zf_can_assert(NULL != message_buffer))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = CAN_ERROR_DATA_BUFFER_NULL;                          // CAN 数据指针异常 操作无法进行
            break;
        }

        return_state = CAN_WARNING_NO_DATA;
        if(0xFFFF != can_message_buffer[can_index].id)
        {
            memcpy(message_buffer, &can_message_buffer[can_index], sizeof(zf_can_message_buffer_struct));
            can_message_buffer[can_index].id = 0xFFFF;
            return_state = CAN_OPERATION_DONE;
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 注销初始化
// 参数说明     can_index           CAN 模块号 (详见 zf_driver_can.h 内 zf_can_index_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_deinit(CAN_1);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_deinit (zf_can_index_enum can_index)
{
    zf_can_operation_state_enum return_state = CAN_ERROR_UNKNOW;

    do
    {
        if(zf_can_assert(can_obj_list[can_index].bps))
        {
            // 此处如果断言报错 那么证明本模块没有初始化过 是不允许直接操作的
            // 未初始化模块很可能没有开启模块时钟或者没有使能模块
            // 对于部分单片机没有使能模块直接操作会导致总线异常或者硬件错误
            return_state = CAN_ERROR_MODULE_NOT_INIT;
            break;
        }

        zf_gpio_deinit(can_obj_list[can_index].tx_pin);
        zf_gpio_deinit(can_obj_list[can_index].rx_pin);

        zf_rcc_peripheral_reset(can_rcc_index_list[can_index]);
        zf_rcc_clock_disable(can_rcc_index_list[can_index]);

        zf_interrupt_disable(can_irq_index_list[can_index]);

        can_obj_list[can_index].bps = 0;
        can_obj_list[can_index].tx_pin = PIN_NULL;
        can_obj_list[can_index].rx_pin = PIN_NULL;
        can_obj_list[can_index].config_info = 0;

        return_state = CAN_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 初始化
// 参数说明     can_index           CAN 模块号 (详见 zf_driver_can.h 内 zf_can_index_enum 定义)
// 参数说明     bps                 CAN 比特率 (详见 zf_driver_can.h 内 zf_can_bsp_enum 定义)
// 参数说明     tx_pin              CAN 发送脚 (详见 zf_driver_can.h 内 zf_can_tx_pin_enum 定义)
// 参数说明     rx_pin              CAN 接收脚 (详见 zf_driver_can.h 内 zf_can_rx_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_can_init(CAN_1, 250 * 1000, CAN1_TX_D2, CAN1_RX_D1);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_can_init (zf_can_index_enum can_index, zf_can_bsp_enum bps, zf_can_tx_pin_enum tx_pin, zf_can_rx_pin_enum rx_pin)
{
    zf_can_operation_state_enum return_state = CAN_ERROR_UNKNOW;

    do
    {
        if(zf_can_assert(!can_obj_list[can_index].bps))
        {
            // 此处如果断言报错 那么证明本模块已经被初始化过一次 重复初始化是不允许的
            // 否则应当先注销初始化之后再重新初始化
            return_state = CAN_ERROR_MODULE_OCCUPIED;                           // CAN 模块未初始化 操作无法进行
            break;
        }
        if(zf_can_assert(
            can_index == ((tx_pin >> CAN_INDEX_OFFSET) & CAN_INDEX_MASK)))                                                     // 输出错误码 具体问题查看函数上方注释
        {
            // 此处如果断言报错 那么证明 TX 引脚跟 UART 索引不匹配
            return_state = CAN_ERROR_TX_PIN_MISMATCH;
            break;
        }
        if(zf_can_assert(
            can_index == ((rx_pin >> CAN_INDEX_OFFSET) & CAN_INDEX_MASK)))                                                     // 输出错误码 具体问题查看函数上方注释
        {
            // 此处如果断言报错 那么证明 RX 引脚跟 UART 索引不匹配
            return_state = CAN_ERROR_RX_PIN_MISMATCH;
            break;
        }
        zf_rcc_clock_enable(can_rcc_index_list[can_index]);

        can_obj_list[can_index].bps = bps;
        can_obj_list[can_index].tx_pin = (zf_gpio_pin_enum)(tx_pin & CAN_PIN_INDEX_MASK);
        can_obj_list[can_index].rx_pin = (zf_gpio_pin_enum)(rx_pin & CAN_PIN_INDEX_MASK);
        can_obj_list[can_index].enable = ZF_ENABLE;

        zf_afio_init(can_obj_list[can_index].tx_pin,
            GPO_AF_PUSH_PULL,
            (zf_gpio_af_enum)((tx_pin >> CAN_PIN_AF_OFFSET) & CAN_PIN_AF_MASK));
        zf_afio_init(can_obj_list[can_index].rx_pin,
            GPO_AF_PUSH_PULL,
//            GPO_AF_OPEN_DRAIN,
            (zf_gpio_af_enum)((rx_pin >> CAN_PIN_AF_OFFSET) & CAN_PIN_AF_MASK));

        {
            can_init(can_obj_list[can_index].can_ptr);                                              // 初始化实例配置

            can_set_irq_line    (can_obj_list[can_index].can_ptr, CAN_IRQ_LINE0);                   // 设置中断线
            can_set_irq_mask    (can_obj_list[can_index].can_ptr, CAN_IRQ_ALL_ENABLED);             // 启用所有中断触发
            can_set_loopback    (can_obj_list[can_index].can_ptr, CAN_NO_LOOPBACK);                 // 禁止回环

            can_set_fd_mode     (can_obj_list[can_index].can_ptr, false);                           // FD 模式
            can_set_datasize    (can_obj_list[can_index].can_ptr, CAN_DATASIZE_8BYTE);              // 数据包长
            can_set_baudrate    (can_obj_list[can_index].can_ptr, (can_baudrate_t)bps);                             // 波特率设置
            can_set_dmu_mode    (can_obj_list[can_index].can_ptr, false);                           // DMU 模式

            for( uint32_t id = 0; id <  CAN_DEFAULT_FILTER_ID_RANGE && id < 128; id ++)
            {
                can_set_filter  (can_obj_list[can_index].can_ptr, id + CAN_DEFAULT_FILTER_ID, 0,    // 设置过滤
                                     CAN_STD_FILTER_TYPE,
                                     CAN_FILTER_CLASSIC,
                                     CAN_FEC_RX_BUF);
            }

            can_set_buffers         (can_obj_list[can_index].can_ptr, 8, 8, 1, 1, 1);                   // BUFF 参数设置

            can_set_rx_polling_mode (can_obj_list[can_index].can_ptr, false);                       // 接收轮询模式
            can_set_tx_mode         (can_obj_list[can_index].can_ptr, CAN_TX_MODE_DEDICATED_BUFF);  // 发送模式

            can_set_prio            (can_obj_list[can_index].can_ptr, IRQ_PRIORITY_4);
            can_set_cb              (can_obj_list[can_index].can_ptr, can_receive_cb);              // 设置中断回调

            can_start(can_obj_list[can_index].can_ptr);                                            // 启用参数配置 启用模块
        }

        return_state = CAN_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
