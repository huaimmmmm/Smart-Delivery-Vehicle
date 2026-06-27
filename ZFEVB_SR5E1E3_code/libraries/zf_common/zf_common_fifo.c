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

// 自身头文件
#include "zf_common_fifo.h"

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FIFO 重置缓冲器 清空当前 FIFO 对象的内存
// 参数说明     *fifo               FIFO 对象指针
// 返回参数     void
// 使用示例     zf_fifo_clear(fifo);
// 备注信息     它会在下次调用写入状态时生效
//-------------------------------------------------------------------------------------------------------------------
void zf_fifo_clear (zf_fifo_obj_struct *fifo)
{
    do
    {
        if(NULL == fifo)                                                        // 如果 FIFO 对象为空就不能操作
        {
            break;                                                              // 退出操作
        }
        if(ZF_FALSE == fifo->init_state)                                        // 如果 FIFO 对象未初始化就不能操作
        {
            break;                                                              // 退出操作
        }

        fifo->clean_state = ZF_TRUE;                                            // 清空操作仅置位标志位 操作在写数据时才会生效
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FIFO 查询当前数据个数
// 参数说明     *fifo               FIFO 对象指针
// 返回参数     uint32              已使用长度
// 使用示例     uint32 len = zf_fifo_used(fifo);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_fifo_used (zf_fifo_obj_struct *fifo)
{
    int64 used_lenght = 0;

    do
    {
        if(NULL == fifo)                                                        // 如果 FIFO 对象为空就不能操作
        {
            break;                                                              // 退出操作
        }
        if(ZF_FALSE == fifo->init_state)                                        // 如果 FIFO 对象未初始化就不能操作
        {
            break;                                                              // 退出操作
        }

        if(fifo->clean_state)                                                   // 清空指令置位直接返回没有数据
        {
            break;
        }

        uint32  buffer_lenght   = fifo->tail_ptr - fifo->base_ptr;
        uint8   state_backup    = fifo->lock_state;                             // 缓存锁定状态
        
        fifo->lock_state        = ZF_TRUE;                                      // 锁定当前状态
        if(fifo->full_state)                                                    // 检查 FIFO 缓冲是不是满了
        {
            used_lenght         = buffer_lenght;                                // 如果缓冲满了直接返回缓冲区长度
        }
        else
        {
            used_lenght         = fifo->write_ptr - fifo->read_ptr;             // 计算读写指针差值
            used_lenght         = (used_lenght + buffer_lenght) % buffer_lenght;// 换算得到实际数据量
        }
        fifo->lock_state        = state_backup;                                 // 恢复锁定原始状态
    }while(0);

    return used_lenght;                                                         // 返回实际数据
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向 FIFO 中写入数据
// 参数说明     *fifo               FIFO 对象指针
// 参数说明     dat                 数据
// 返回参数     uint8               操作状态            (详见 zf_common_fifo.h 中枚举 zf_fifo_error_code_enum 定义)
// 使用示例     zf_fifo_write_element(fifo, dat);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_fifo_write_element (zf_fifo_obj_struct *fifo, uint32 dat)
{
    zf_fifo_error_code_enum return_state = FIFO_OPERATION_ERROR_UNKNOW;

    do
    {
        if(NULL == fifo)                                                        // 如果 FIFO 对象为空就不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_ILLEGAL_PARAMETER;       // 非法参数
            break;                                                              // 退出操作
        }
        if(ZF_FALSE == fifo->init_state)                                        // 如果 FIFO 对象未初始化就不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_OBJECT_UNINIT;           // FIFO 对象未初始化
            break;                                                              // 退出操作
        }

        if(fifo->lock_state)                                                    // 判断操作锁定状态
        {
            return_state        = FIFO_WRITE_ERROR_LOCK;                        // 如果已经被锁定就报锁定错误
            break;                                                              // 退出操作
        }
        if(fifo->write_state)                                                   // 判断写锁定状态
        {
            return_state        = FIFO_WRITE_ERROR_NESTED;                      // 如果已经被锁定就报嵌套错误
            break;                                                              // 退出操作
        }

        fifo->write_state       = ZF_TRUE;                                      // 写锁定

        if(fifo->clean_state)                                                   // 判断是否置位清空指令
        {
            fifo->write_ptr     = fifo->base_ptr;                               // 复位写指针
            fifo->read_ptr      = fifo->base_ptr;                               // 复位读指针
            fifo->full_state    = ZF_FALSE;                                     // 复位缓冲区满状态
            fifo->clean_state   = ZF_FALSE;                                     // 复位清空指令
        }
        if(fifo->full_state)                                                    // 判断是否写满了
        {
            return_state        = FIFO_WRITE_ERROR_NO_SPACE;                    // 写满就报空间不足
            fifo->write_state   = ZF_FALSE;                                     // 解除写锁定
            break;                                                              // 退出操作
        }

        uint32 start_ptr        = fifo->write_ptr;                              // 获取写指针状态快照
        uint32 end_ptr          = fifo->read_ptr;                               // 获取读指针状态快照

        memcpy((void *)start_ptr, &dat, fifo->step);                            // 写入数据
        start_ptr               += fifo->step;                                  // 写指针位移计算
        if(start_ptr == fifo->tail_ptr)                                         // 判断是否需要环移
        {
            start_ptr           = fifo->base_ptr;
        }
        fifo->full_state        = (start_ptr == end_ptr);                       // 判断是否写满了 写入后写指针读指针重合就肯定是写满了
        fifo->write_ptr         = start_ptr;                                    // 写指针位移生效

        fifo->write_state       = ZF_FALSE;                                     // 解除写锁定

        return_state            = FIFO_OPERATION_NO_ERROR;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向 FIFO 中写入数据
// 参数说明     *fifo               FIFO 对象指针
// 参数说明     *dat                数据来源缓冲区指针
// 参数说明     length              需要写入的数据长度
// 返回参数     uint8               操作状态            (详见 zf_common_fifo.h 中枚举 zf_fifo_error_code_enum 定义)
// 使用示例     zf_fifo_write_buffer(fifo, dat, length);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_fifo_write_buffer (zf_fifo_obj_struct *fifo, void *dat, uint32 *length)
{
    zf_fifo_error_code_enum return_state = FIFO_OPERATION_ERROR_UNKNOW;

    do
    {
        if(NULL == fifo || NULL == dat || NULL == length || 0 == *length)       // 如果 FIFO 对象为空就不能操作 缓冲 读取长度 异常也不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_ILLEGAL_PARAMETER;       // 非法参数
            break;                                                              // 退出操作
        }

        uint32 length_temp = *length;
        *length = 0;

        if(ZF_FALSE == fifo->init_state)                                        // 如果 FIFO 对象未初始化就不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_OBJECT_UNINIT;           // FIFO 对象未初始化
            break;                                                              // 退出操作
        }

        if(fifo->lock_state)                                                    // 判断操作锁定状态
        {
            return_state        = FIFO_WRITE_ERROR_LOCK;                        // 如果已经被锁定就报锁定错误
            break;                                                              // 退出操作
        }
        if(fifo->write_state)                                                   // 判断写锁定状态
        {
            return_state        = FIFO_WRITE_ERROR_NESTED;                      // 如果已经被锁定就报嵌套错误
            break;                                                              // 退出操作
        }

        fifo->write_state       = ZF_TRUE;                                      // 写锁定
        
        if(fifo->clean_state)                                                   // 判断是否置位清空指令
        {
            fifo->write_ptr     = fifo->base_ptr;                               // 复位写指针
            fifo->read_ptr      = fifo->base_ptr;                               // 复位读指针
            fifo->full_state    = ZF_FALSE;                                     // 复位缓冲区满状态
            fifo->clean_state   = ZF_FALSE;                                     // 复位清空指令
        }
        if(fifo->full_state)                                                    // 判断是否写满了
        {
            return_state        = FIFO_WRITE_ERROR_NO_SPACE;                    // 写满就报空间不足
            fifo->write_state   = ZF_FALSE;                                     // 解除写锁定
            break;                                                              // 退出操作
        }

        uint32 start_ptr        = fifo->write_ptr;                              // 获取写指针状态快照
        uint32 end_ptr          = fifo->read_ptr;                               // 获取读指针状态快照
        uint32 data_size        = 0;

        if(end_ptr > start_ptr)                                                 // 如果写指针在低地址证明可写空间在两个指针中间
        {
            data_size           = end_ptr - start_ptr;                          // 获得可写字节数
            if(length_temp > data_size / fifo->step)                            // 判断写入长度是否超过了可写字节数
            {
                length_temp         = data_size / fifo->step;                   // 修正写入长度
                return_state    = FIFO_WRITE_ERROR_PARTIAL_DONE;                // 写满就报空间不足
            }
            memcpy((void *)start_ptr, dat, length_temp * fifo->step);           // 写入数据
            start_ptr           += length_temp * fifo->step;                    // 写指针位移计算
        }
        else                                                                    // 否则的话就需要计算头尾大小
        {
            data_size           = fifo->tail_ptr - start_ptr;                   // 获取写指针到缓冲区尾部字节数
            if(length_temp <= data_size / fifo->step)                           // 到尾部空间充足 单次写入即可
            {
                memcpy((void *)start_ptr, dat, length_temp * fifo->step);       // 写入数据
                start_ptr       += length_temp * fifo->step;                    // 写指针位移计算
            }
            else
            {
                memcpy((void *)start_ptr, dat, data_size);                      // 写入第一部分数据

                dat             = (void *)((uint32)dat + data_size);            // 缓冲地址自增
                data_size       = length_temp * fifo->step - data_size;         // 获得第二部分写入数据长度

                if(data_size > end_ptr - fifo->base_ptr)                        // 剩余数据超过了可写区域
                {
                    length_temp = length_temp - (data_size - (end_ptr - fifo->base_ptr)) / fifo->step;
                    data_size   = end_ptr - fifo->base_ptr;                     // 更新写入长度
                    return_state= FIFO_WRITE_ERROR_PARTIAL_DONE;                // 写满就报空间不足
                }

                memcpy((void *)fifo->base_ptr, dat, data_size);                 // 写入第二部分数据
                start_ptr       = fifo->base_ptr + data_size;                   // 写指针位移计算
            }
        }

        if(start_ptr == fifo->tail_ptr)                                         // 判断是否需要环移
        {
            start_ptr           = fifo->base_ptr;
        }
        fifo->full_state        = (start_ptr == end_ptr);                       // 判断是否写满了 写入后写指针读指针重合就肯定是写满了
        fifo->write_ptr         = start_ptr;                                    // 写指针位移生效

        fifo->write_state       = ZF_FALSE;                                     // 解除写锁定
        *length                 = length_temp;

        return_state            = (FIFO_OPERATION_ERROR_UNKNOW == return_state) ? FIFO_OPERATION_NO_ERROR : return_state;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从 FIFO 读取数据
// 参数说明     *fifo               FIFO 对象指针
// 参数说明     *dat                目标缓冲区指针
// 参数说明     flag                是否变更 FIFO 状态  (详见 zf_common_fifo.h 中枚举 zf_fifo_operation_enum 定义)
// 返回参数     uint8               操作状态            (详见 zf_common_fifo.h 中枚举 zf_fifo_error_code_enum 定义)
// 使用示例     zf_fifo_read_element(fifo, dat, flag);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_fifo_read_element (zf_fifo_obj_struct *fifo, void *dat, zf_fifo_operation_enum flag)
{
    zf_fifo_error_code_enum return_state = FIFO_OPERATION_ERROR_UNKNOW;

    do
    {
        if(NULL == fifo)                                                        // 如果 FIFO 对象为空就不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_ILLEGAL_PARAMETER;       // 非法参数
            break;                                                              // 退出操作
        }
        if(ZF_FALSE == fifo->init_state)                                        // 如果 FIFO 对象未初始化就不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_OBJECT_UNINIT;           // FIFO 对象未初始化
            break;                                                              // 退出操作
        }

        if(fifo->clean_state)                                                   // 清空指令置位直接返回没有数据
        {
            return_state        = FIFO_READ_ERROR_NO_DATA;                      // 写满标志位没有置位且指针重合肯定是没数据
            break;                                                              // 退出操作
        }
        if(fifo->lock_state)                                                    // 判断操作锁定状态
        {
            return_state        = FIFO_READ_ERROR_LOCK;                         // 如果已经被锁定就报锁定错误
            break;                                                              // 退出操作
        }
        if(fifo->read_state)                                                    // 判断读锁定
        {
            return_state        = FIFO_READ_ERROR_NESTED;                       // 如果已经被锁定就报嵌套错误
            break;                                                              // 退出操作
        }

        fifo->read_state        = ZF_TRUE;                                      // 读锁定

        uint32 start_ptr        = fifo->read_ptr;                               // 获取读指针状态快照
        uint32 end_ptr          = fifo->write_ptr;                              // 获取写指针状态快照

        if(start_ptr == end_ptr && !fifo->full_state)                           // 头尾指针位置重合的话判断是否是没有数据
        {
            return_state        = FIFO_READ_ERROR_NO_DATA;                      // 写满标志位没有置位且指针重合肯定是没数据
            fifo->read_state    = ZF_FALSE;                                     // 解除读锁定
            break;                                                              // 退出操作
        }

        if(NULL != dat)
        {
            memcpy(dat, (void *)start_ptr, fifo->step);                         // 读取数据
        }
        start_ptr               += fifo->step;                                  // 读指针位移计算
        
        if(flag)                                                                // 判断是否需要清空
        {
            if(start_ptr == fifo->tail_ptr)                                     // 判断是否需要环移
            {
                start_ptr       = fifo->base_ptr;
            }
            fifo->full_state    = ZF_FALSE;                                     // 如果数据都读取了 那肯定缓冲区有空位
            fifo->read_ptr      = start_ptr;                                    // 读指针位移生效
        }

        fifo->read_state        = ZF_FALSE;                                     // 解除读锁定

        return_state            = FIFO_OPERATION_NO_ERROR;
    }while(0);

    return return_state;
}

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
uint8 zf_fifo_read_buffer (zf_fifo_obj_struct *fifo, void *dat, uint32 *length, zf_fifo_operation_enum flag)
{
    zf_fifo_error_code_enum return_state = FIFO_OPERATION_ERROR_UNKNOW;

    do
    {
        if(NULL == fifo || NULL == length || 0 == *length)                      // 如果 FIFO 对象为空就不能操作 读取长度 异常也不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_ILLEGAL_PARAMETER;       // 非法参数
            break;                                                              // 退出操作
        }

        uint32 length_temp      = *length;
        *length                 = 0;

        if(ZF_FALSE == fifo->init_state)                                        // 如果 FIFO 对象未初始化就不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_OBJECT_UNINIT;           // FIFO 对象未初始化
            break;                                                              // 退出操作
        }

        if(fifo->clean_state)                                                   // 清空指令置位直接返回没有数据
        {
            return_state        = FIFO_READ_ERROR_NO_DATA;                      // 写满标志位没有置位且指针重合肯定是没数据
            break;                                                              // 退出操作
        }
        if(fifo->lock_state)                                                    // 判断操作锁定状态
        {
            return_state        = FIFO_READ_ERROR_LOCK;                         // 如果已经被锁定就报锁定错误
            break;                                                              // 退出操作
        }
        if(fifo->read_state)                                                    // 判断读锁定
        {
            return_state        = FIFO_READ_ERROR_NESTED;                       // 如果已经被锁定就报嵌套错误
            break;                                                              // 退出操作
        }

        fifo->read_state = ZF_TRUE;                                             // 读锁定

        uint32 start_ptr        = fifo->read_ptr;                               // 获取读指针状态快照
        uint32 end_ptr          = fifo->write_ptr;                              // 获取写指针状态快照
        uint32 data_size        = 0;

        if(start_ptr == end_ptr && !fifo->full_state)                           // 头尾指针位置重合的话判断是否是没有数据
        {
            return_state        = FIFO_READ_ERROR_NO_DATA;                      // 写满标志位没有置位且指针重合肯定是没数据
            fifo->read_state    = ZF_FALSE;                                     // 解除读锁定
            break;                                                              // 退出操作
        }

        if(start_ptr < end_ptr)                                                 // 如果读指针在低地址证明可读空间在两个指针中间
        {
            data_size           = end_ptr - start_ptr;
            if(length_temp > data_size / fifo->step)
            {
                length_temp     = data_size / fifo->step;                       // 修正读取长度
                return_state    = FIFO_READ_ERROR_PARTIAL_DONE;                 // 读不满就报数据不够
            }
            if(NULL != dat)
            {
                memcpy(dat, (void *)start_ptr, length_temp * fifo->step);       // 读取数据
            }
            start_ptr           += length_temp * fifo->step;                    // 读指针位移计算
        }
        else
        {
            data_size           = fifo->tail_ptr - start_ptr;                   // 获取读指针到缓冲区尾部字节数
            if(length_temp <= data_size / fifo->step)                           // 到尾部空间充足 单次读取即可
            {
                if(NULL != dat)
                {
                    memcpy(dat, (void *)start_ptr, length_temp * fifo->step);   // 读取数据
                }
                start_ptr       += length_temp * fifo->step;                    // 读指针位移计算
            }
            else
            {
                if(NULL != dat)
                {
                    memcpy(dat, (void *)start_ptr, data_size);                  // 写入第一部分数据
                }

                dat             = (void *)((uint32)dat + data_size);            // 缓冲地址自增
                data_size       = length_temp * fifo->step - data_size;         // 获得第二部分写入数据长度

                if(data_size > end_ptr - fifo->base_ptr)                        // 剩余数据超过了可读区域
                {
                    length_temp = length_temp - (data_size - (end_ptr - fifo->base_ptr)) / fifo->step;
                    data_size   = end_ptr - fifo->base_ptr;                     // 更新读取长度
                    return_state= FIFO_READ_ERROR_PARTIAL_DONE;                 // 读不满就报数据不够
                }
                if(NULL != dat)
                {
                    memcpy(dat, (void *)fifo->base_ptr, data_size);             // 读取第二部分数据
                }
                start_ptr       = fifo->base_ptr + data_size;                   // 读指针位移计算
            }
        }

        if(flag)                                                                // 判断是否需要清空
        {
            if(start_ptr == fifo->tail_ptr)                                     // 判断是否需要环移
            {
                start_ptr       = fifo->base_ptr;
            }
            fifo->full_state    = ZF_FALSE;                                     // 如果数据都读取了 那肯定缓冲区有空位
            fifo->read_ptr      = start_ptr;                                    // 读指针位移生效
        }

        fifo->read_state        = ZF_FALSE;                                     // 解除读锁定
        *length                 = length_temp;

        return_state            = (FIFO_OPERATION_ERROR_UNKNOW == return_state) ? FIFO_OPERATION_NO_ERROR : return_state;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从 FIFO 尾部读取单个元素
// 参数说明     *fifo               FIFO 对象指针
// 参数说明     *dat                目标缓冲区指针 允许读取缓冲为空 如果读取缓冲为空就会直接跳过取数据的步骤
// 参数说明     flag                是否变更 FIFO 状态 可选择是否清空读取的数据
// 返回参数     zf_fifo_state_enum     操作状态
// 使用示例     zf_log(zf_fifo_read_tail_element(&fifo, data, FIFO_READ_WITHOUT_CLEAN) == FIFO_SUCCESS, "zf_fifo_read_buffer error");
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_fifo_read_tail_element (zf_fifo_obj_struct *fifo, void *dat, zf_fifo_operation_enum flag)
{
    zf_fifo_error_code_enum return_state = FIFO_OPERATION_ERROR_UNKNOW;

    do
    {
        if(NULL == fifo)                                                        // 如果 FIFO 对象为空就不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_ILLEGAL_PARAMETER;       // 非法参数
            break;                                                              // 退出操作
        }
        if(ZF_FALSE == fifo->init_state)                                        // 如果 FIFO 对象未初始化就不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_OBJECT_UNINIT;           // FIFO 对象未初始化
            break;                                                              // 退出操作
        }

        if(fifo->clean_state)                                                   // 清空指令置位直接返回没有数据
        {
            return_state        = FIFO_READ_ERROR_NO_DATA;                      // 写满标志位没有置位且指针重合肯定是没数据
            break;                                                              // 退出操作
        }
        if(fifo->lock_state)                                                    // 判断操作锁定状态
        {
            return_state        = FIFO_READ_ERROR_LOCK;                         // 如果已经被锁定就报锁定错误
            break;                                                              // 退出操作
        }
        if(fifo->state & FIFO_STATE_MASK_OPERATION_LOCK)                        // 判断读写锁定 因为尾部读取实际是操作写指针 所以要判断写锁定
        {
            return_state        = FIFO_READ_ERROR_NESTED;                       // 如果已经被锁定就报嵌套错误
            break;                                                              // 退出操作
        }

        fifo->state             |= FIFO_STATE_MASK_OPERATION_LOCK;              // 读写锁定

        uint32 start_ptr        = fifo->write_ptr;                              // 获取写指针状态快照
        uint32 end_ptr          = fifo->read_ptr;                               // 获取读指针状态快照

        if(start_ptr == end_ptr && !fifo->full_state)                           // 头尾指针位置重合的话判断是否是没有数据
        {
            return_state        = FIFO_READ_ERROR_NO_DATA;                      // 写满标志位没有置位且指针重合肯定是没数据
            fifo->state         &= ~FIFO_STATE_MASK_OPERATION_LOCK;             // 读写取消锁定
            break;                                                              // 退出操作
        }

        start_ptr               -= fifo->step;                                  // 写指针位移计算
        if(start_ptr == fifo->base_ptr - 1)                                     // 判断是否需要环移
        {
            start_ptr           = fifo->tail_ptr - 1;
        }
        if(NULL != dat)
        {
            memcpy(dat, (void *)start_ptr, fifo->step);                         // 读取数据
        }

        if(flag)                                                                // 判断是否需要清空
        {
            fifo->full_state    = ZF_FALSE;                                     // 如果数据都读取了 那肯定缓冲区有空位
            fifo->write_ptr     = start_ptr;                                    // 写指针位移生效
        }

        fifo->state             &= ~FIFO_STATE_MASK_OPERATION_LOCK;             // 读写取消锁定

        return_state            = FIFO_OPERATION_NO_ERROR;
    }while(0);

    return return_state;
}

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
uint8 zf_fifo_read_tail_buffer (zf_fifo_obj_struct *fifo, void *dat, uint32 *length, zf_fifo_operation_enum flag)
{
    zf_fifo_error_code_enum return_state = FIFO_OPERATION_ERROR_UNKNOW;

    do
    {
        if(NULL == fifo || NULL == length || 0 == *length)                      // 如果 FIFO 对象为空就不能操作 读取长度 异常也不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_ILLEGAL_PARAMETER;       // 非法参数
            break;                                                              // 退出操作
        }

        uint32 length_temp      = *length;
        *length                 = 0;

        if(ZF_FALSE == fifo->init_state)                                        // 如果 FIFO 对象未初始化就不能操作
        {
            return_state        = FIFO_OPERATION_ERROR_OBJECT_UNINIT;           // FIFO 对象未初始化
            break;                                                              // 退出操作
        }

        if(fifo->clean_state)                                                   // 清空指令置位直接返回没有数据
        {
            return_state        = FIFO_READ_ERROR_NO_DATA;                      // 写满标志位没有置位且指针重合肯定是没数据
            break;                                                              // 退出操作
        }
        if(fifo->lock_state)                                                    // 判断操作锁定状态
        {
            return_state        = FIFO_READ_ERROR_LOCK;                         // 如果已经被锁定就报锁定错误
            break;                                                              // 退出操作
        }
        if(fifo->state & FIFO_STATE_MASK_OPERATION_LOCK)                        // 判断读写锁定 因为尾部读取实际是操作写指针 所以要判断写锁定
        {
            return_state        = FIFO_READ_ERROR_NESTED;                       // 如果已经被锁定就报嵌套错误
            break;                                                              // 退出操作
        }

        fifo->state             |= FIFO_STATE_MASK_OPERATION_LOCK;              // 读写锁定

        uint32 start_ptr        = fifo->write_ptr;                              // 获取写指针状态快照
        uint32 end_ptr          = fifo->read_ptr;                               // 获取读指针状态快照
        uint32 data_size        = 0;

        if(start_ptr > end_ptr)                                                 // 如果写指针在高地址证明可读空间在中间
        {
            data_size           = start_ptr - end_ptr;                          // 获得可读字节数
            if(length_temp > data_size / fifo->step)                            // 判断写入长度是否超过了可读字节数
            {
                length_temp     = data_size / fifo->step;                       // 修正读取长度
                return_state    = FIFO_READ_ERROR_PARTIAL_DONE;                 // 读不满就报数据不够
            }
            start_ptr           -= length_temp * fifo->step;                    // 写指针位移计算
            if(NULL != dat)
            {
                memcpy(dat, (void *)start_ptr, length_temp * fifo->step);       // 读取数据
            }
        }
        else                                                                    // 否则的话就需要计算头尾大小
        {
            data_size           = start_ptr - fifo->base_ptr;                   // 获取写指针到缓冲区头部字节数
            if(length_temp <= data_size / fifo->step)                           // 到尾部空间充足 单次读取即可
            {
                start_ptr       -= length_temp * fifo->step;                    // 写指针位移计算
                if(NULL != dat)
                {
                    memcpy(dat, (void *)start_ptr, length_temp * fifo->step);   // 读取数据
                }
            }
            else
            {
                start_ptr       = fifo->base_ptr;                               // 写指针位移计算
                if(NULL != dat)
                {
                    memcpy((void *)((uint32)dat + data_size), (void *)start_ptr, data_size);    // 读取第一部分数据
                }

                data_size       = length_temp * fifo->step - data_size;         // 获得第二部分读取数据长度

                if(data_size > fifo->tail_ptr - end_ptr)                        // 剩余数据超过了可读区域
                {
                    length_temp = length_temp - (data_size - (fifo->tail_ptr - end_ptr)) / fifo->step;
                    data_size   = fifo->tail_ptr - end_ptr;                     // 更新读取长度
                    return_state= FIFO_READ_ERROR_PARTIAL_DONE;                 // 读不满就报数据不够
                }
                start_ptr       = fifo->tail_ptr - data_size;
                if(NULL != dat)
                {
                    memcpy(dat, (void *)start_ptr, data_size);                  // 读取第二部分数据
                }
            }
        }

        if(flag)                                                                // 判断是否需要清空
        {
            if(start_ptr == fifo->base_ptr - 1)                                 // 判断是否需要环移
            {
                start_ptr       = fifo->tail_ptr - 1;
            }
            fifo->full_state    = ZF_FALSE;                                     // 如果数据都读取了 那肯定缓冲区有空位
            fifo->write_ptr     = start_ptr;                                    // 写指针位移生效
        }

        fifo->state             &= ~FIFO_STATE_MASK_OPERATION_LOCK;             // 读写取消锁定

        return_state            = (FIFO_OPERATION_ERROR_UNKNOW == return_state) ? FIFO_OPERATION_NO_ERROR : return_state;
    }while(0);

    return return_state;
}

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
uint8 zf_fifo_init (zf_fifo_obj_struct *fifo, zf_fifo_data_type_enum type, void *buffer_addr, uint32 size)
{
    zf_fifo_error_code_enum return_state = FIFO_OPERATION_ERROR_UNKNOW;

    do
    {
        if(NULL == fifo || NULL == buffer_addr || 0 == size)                    // 如果 FIFO 对象为空就不能操作 缓冲区或者缓冲区大小为空也不能操作
        {
            return_state    = FIFO_OPERATION_ERROR_ILLEGAL_PARAMETER;           // 非法参数
            break;                                                              // 退出操作
        }

        fifo->state         = FIFO_STATE_MASK;                                  // 首先是将对象锁定 避免错误操作顺序导致的异常

        fifo->base_ptr      = (uint32)buffer_addr;                              // 缓存基地址
        fifo->tail_ptr      = fifo->base_ptr + size;                            // 缓存尾地址

        fifo->write_ptr     = fifo->base_ptr;                                   // 写指针 指向空缓存
        fifo->read_ptr      = fifo->base_ptr;                                   // 读指针 指向未读数据

        fifo->step          = type;                                             // 数据类型 地址步进值

        fifo->state         = FIFO_STATE_MASK_INIT_DONE;                        // 完成配置后将状态标记为完成初始化

        return_state        = FIFO_OPERATION_NO_ERROR;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
