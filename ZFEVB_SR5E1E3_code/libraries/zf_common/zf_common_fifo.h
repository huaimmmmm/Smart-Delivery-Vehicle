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
* 文件名称          zf_common_fifo
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

#ifndef _zf_common_fifo_h_
#define _zf_common_fifo_h_

// zf_common 层引用
#include "zf_common_typedef.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_fifo_clear                                                                   // FIFO 重置缓冲器 清空当前 FIFO 对象的内存
// zf_fifo_used                                                                    // FIFO 查询当前数据个数

// zf_fifo_write_element                                                           // 向 FIFO 中写入数据
// zf_fifo_write_buffer                                                            // 向 FIFO 中写入数据
// zf_fifo_read_element                                                            // 从 FIFO 读取数据
// zf_fifo_read_buffer                                                             // 从 FIFO 读取数据
// zf_fifo_read_tail_element                                                       // 从 FIFO 尾部读取单个元素
// zf_fifo_read_tail_buffer                                                        // 从 FIFO 尾部读取指定长度 buffer

// zf_fifo_init                                                                    // FIFO 初始化 挂载对应缓冲区
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件需要的枚举与对象结构等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef enum                                                                    // 枚举 FIFO 错误码 此枚举定义不允许用户修改
{
    FIFO_OPERATION_NO_ERROR             = ZF_NO_ERROR   ,                       // 操作正常
    FIFO_OPERATION_ERROR_UNKNOW         = ZF_ERROR      ,                       // 未知错误 操作无法进行 正常情况下不会出现此错误

    FIFO_OPERATION_ERROR_ILLEGAL_PARAMETER              ,                       // 非法参数
    FIFO_OPERATION_ERROR_OBJECT_UNINIT                  ,                       // 对象未初始化

    FIFO_WRITE_ERROR_LOCK                               ,                       // 操作锁定 处于长度计算状态
    FIFO_WRITE_ERROR_NESTED                             ,                       // 写嵌套报错
    FIFO_WRITE_ERROR_NO_SPACE                           ,                       // 没有空闲空间 无法写入
    FIFO_WRITE_ERROR_PARTIAL_DONE                       ,                       // 剩余空间不足 部分写入

    FIFO_READ_ERROR_LOCK                                ,                       // 操作锁定 处于长度计算状态
    FIFO_READ_ERROR_NESTED                              ,                       // 读嵌套报错
    FIFO_READ_ERROR_NO_DATA                             ,                       // 缓冲区没有数据可读 无法读取
    FIFO_READ_ERROR_PARTIAL_DONE                        ,                       // 缓冲区数据不足 部分读取
}zf_fifo_error_code_enum;

typedef enum                                                                    // 枚举 FIFO 读操作类型 此枚举定义不允许用户修改
{
    FIFO_READ_WITHOUT_CLEAN             = 0 ,
    FIFO_READ_WITH_CLEAN                    ,
}zf_fifo_operation_enum;

typedef enum                                                                    // 枚举 FIFO 数据位宽 此枚举定义不允许用户修改
{
    FIFO_DATA_8BIT                      = 1 ,                                   // FIFO 数据位宽 8bit
    FIFO_DATA_16BIT                     = 2 ,                                   // FIFO 数据位宽 16bit
    FIFO_DATA_32BIT                     = 4 ,                                   // FIFO 数据位宽 32bit
}zf_fifo_data_type_enum;

typedef struct                                                                  // FIFO 管理对象模板
{
    uint32                  base_ptr        ;                                   // 缓存基地址
    uint32                  tail_ptr        ;                                   // 缓存尾地址
    
    uint32                  write_ptr       ;                                   // 写指针 指向空缓存
    uint32                  read_ptr        ;                                   // 读指针 指向未读数据
    
    zf_fifo_data_type_enum  step            ;                                   // 数据类型 地址步进值
    union
    {
        uint8               state           ;                                   // 操作状态
        struct
        {
            uint8           init_state  : 1 ;                                   // 对象配置状态
            uint8           write_state : 1 ;                                   // 写入锁定状态
            uint8           read_state  : 1 ;                                   // 读取锁定状态
            uint8           lock_state  : 1 ;                                   // 操作锁定状态
            uint8           clean_state : 1 ;                                   // 缓冲清空状态
            uint8           full_state  : 1 ;                                   // 缓冲区满标志
            uint8                       : 2 ;
        };
    };
}zf_fifo_obj_struct;

// 以下是 FIFO 的操作掩码等
#define     FIFO_STATE_MASK                     ( 0x3E )
#define     FIFO_STATE_MASK_INIT_DONE           ( 0x01 )
#define     FIFO_STATE_MASK_WRITE_LOCK          ( 0x02 )
#define     FIFO_STATE_MASK_READ_LOCK           ( 0x04 )
#define     FIFO_STATE_MASK_OPERATION_LOCK      ( 0x06 )
#define     FIFO_STATE_MASK_PAUSE_LOCK          ( 0x08 )
#define     FIFO_STATE_MASK_CLEAN_LOCK          ( 0x10 )
#define     FIFO_STATE_MASK_BUFFER_FULL         ( 0x20 )
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FIFO 重置缓冲器 清空当前 FIFO 对象的内存
// 参数说明     *fifo               FIFO 对象指针
// 返回参数     void
// 使用示例     zf_fifo_clear(fifo);
// 备注信息     它会在下次调用写入状态时生效
//-------------------------------------------------------------------------------------------------------------------
void zf_fifo_clear (zf_fifo_obj_struct *fifo);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FIFO 查询当前数据个数
// 参数说明     *fifo               FIFO 对象指针
// 返回参数     uint32              已使用长度
// 使用示例     uint32 len = zf_fifo_used(fifo);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_fifo_used (zf_fifo_obj_struct *fifo);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向 FIFO 中写入数据
// 参数说明     *fifo               FIFO 对象指针
// 参数说明     dat                 数据
// 返回参数     uint8               操作状态            (详见 zf_common_fifo.h 中枚举 zf_fifo_error_code_enum 定义)
// 使用示例     zf_fifo_write_element(fifo, dat);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_fifo_write_element (zf_fifo_obj_struct *fifo, uint32 dat);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向 FIFO 中写入数据
// 参数说明     *fifo               FIFO 对象指针
// 参数说明     *dat                数据来源缓冲区指针
// 参数说明     length              需要写入的数据长度
// 返回参数     uint8               操作状态            (详见 zf_common_fifo.h 中枚举 zf_fifo_error_code_enum 定义)
// 使用示例     zf_fifo_write_buffer(fifo, dat, length);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_fifo_write_buffer (zf_fifo_obj_struct *fifo, void *dat, uint32 *length);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从 FIFO 读取数据
// 参数说明     *fifo               FIFO 对象指针
// 参数说明     *dat                目标缓冲区指针
// 参数说明     flag                是否变更 FIFO 状态  (详见 zf_common_fifo.h 中枚举 zf_fifo_operation_enum 定义)
// 返回参数     uint8               操作状态            (详见 zf_common_fifo.h 中枚举 zf_fifo_error_code_enum 定义)
// 使用示例     zf_fifo_read_element(fifo, dat, flag);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_fifo_read_element (zf_fifo_obj_struct *fifo, void *dat, zf_fifo_operation_enum flag);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从 FIFO 读取数据
// 参数说明     *fifo               FIFO 对象指针
// 参数说明     *dat                目标缓冲区指针
// 参数说明     *length             读取的数据长度 如果没有这么多数据这里会被修改
// 参数说明     flag                是否变更 FIFO 状态  (详见 zf_common_fifo.h 中枚举 zf_fifo_operation_enum 定义)
// 返回参数     uint8               操作状态            (详见 zf_common_fifo.h 中枚举 zf_fifo_error_code_enum 定义)
// 使用示例     zf_fifo_read_buffer(fifo, dat, length, flag);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_fifo_read_buffer (zf_fifo_obj_struct *fifo, void *dat, uint32 *length, zf_fifo_operation_enum flag);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从 FIFO 尾部读取单个元素
// 参数说明     *fifo               FIFO 对象指针
// 参数说明     *dat                目标缓冲区指针
// 参数说明     flag                是否变更 FIFO 状态 可选择是否清空读取的数据
// 返回参数     uint8               操作状态            (详见 zf_common_fifo.h 中枚举 zf_fifo_error_code_enum 定义)
// 使用示例     zf_log(zf_fifo_read_tail_element(&fifo, data, FIFO_READ_WITHOUT_CLEAN) == FIFO_SUCCESS, "zf_fifo_read_buffer error");
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_fifo_read_tail_element (zf_fifo_obj_struct *fifo, void *dat, zf_fifo_operation_enum flag);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从 FIFO 尾部读取指定长度 buffer
// 参数说明     *fifo               FIFO 对象指针
// 参数说明     *dat                目标缓冲区指针
// 参数说明     *length             读取的数据长度 如果没有这么多数据这里会被修改
// 参数说明     flag                是否变更 FIFO 状态  (详见 zf_common_fifo.h 中枚举 zf_fifo_operation_enum 定义)
// 返回参数     uint8               操作状态            (详见 zf_common_fifo.h 中枚举 zf_fifo_error_code_enum 定义)
// 使用示例     zf_fifo_read_tail_buffer(fifo, dat, length, flag);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_fifo_read_tail_buffer (zf_fifo_obj_struct *fifo, void *dat, uint32 *length, zf_fifo_operation_enum flag);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FIFO 初始化 挂载对应缓冲区
// 参数说明     *fifo               FIFO 对象指针
// 参数说明     type                FIFO 数据位数       (详见 zf_common_fifo.h 中枚举 zf_fifo_data_type_enum 定义)
// 参数说明     *buffer_addr        要挂载的缓冲区
// 参数说明     size                缓冲区大小 字节单位 建议使用 sizeof 来获得准确大小
// 返回参数     uint8               操作状态            (详见 zf_common_fifo.h 中枚举 zf_fifo_error_code_enum 定义)
// 使用示例     zf_fifo_init(&user_fifo, type, buffer_addr, size);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_fifo_init (zf_fifo_obj_struct *fifo, zf_fifo_data_type_enum type, void *buffer_addr, uint32 size);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
