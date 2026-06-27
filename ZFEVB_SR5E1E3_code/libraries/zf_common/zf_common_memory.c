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
* 文件名称          zf_common_memory
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

// zf_common 层引用
#include "zf_common_debug.h"

// 自身头文件
#include "zf_common_memory.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct
{
    union
    {
        uint8           state       ;                                           // 整体操作状态
        struct
        {
            uint8       malloc  : 1 ;                                           // 申请操作
            uint8       free    : 1 ;                                           // 释放操作
            uint8               : 6 ;
        };
    };
}zf_memory_management_state_struct;


AT_ZF_LIB_SECTION_START
// 存储于预开辟的内存池中的管理信息 本部分对于用户来说是不开放的
AT_ZF_LIB_SECTION static memory_management_info_struct      zf_memory_management        = {0, 0, 0, 0, NULL};
AT_ZF_LIB_SECTION static zf_memory_management_state_struct  zf_memory_management_state  = {0};
#if   (!MEMORY_MANAGEMENT_MODE)
uint32 memory_management_buffer[ZF_HEAP_SIZE / 4];
#endif
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GPIO 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_memory_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_memory_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从堆中申请对应大小内存
// 参数说明     *ptr                内存空间地址
// 参数说明     type                内存对齐类型    (详见 zf_common_memory.h 中枚举 memory_align_type_enum 定义)
// 参数说明     size                需要的内存空间大小 固定是字节长度
// 返回参数     void *              返回分配的内存地址
// 使用示例     uint16 *data_buffer = (uint16 * )zf_memory_malloc(ZF_MEMORY_1BYTE, 110);
// 备注信息     不支持多线程操作 不支持中断嵌套调用 不支持递归调用
//              本函数使用弱定义 这样做的目的是在移植操作系统时可以重载此函数
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK void *zf_memory_malloc (memory_align_type_enum type, uint32 size)
{
    void *ptr = NULL;

    do
    {
        if(zf_memory_management_state.state)                                    // 简易互斥判断
        {
            break;
        }
        zf_memory_management_state.malloc = ZF_TRUE;                            // 申请操作占用

        // 由于有时候 IDE 不会检查输入的参数是否符合枚举体限制
        // 所以在这里直接进行断言判断 这是为了保证内存管理不会出错
        if(zf_memory_assert((
                    MEMORY_ALIGN_1BYTE == type
                ||  MEMORY_ALIGN_2BYTE == type
                ||  MEMORY_ALIGN_4BYTE == type)))
        {
            break;
        }

        // 初步判断是否还有足够的空间
        // 因为区块列表信息也是存储在这里 所以还要减去新增区块信息大小
        if(zf_memory_management.size - sizeof(memory_block_info_struct) < size)
        {
            break;
        }

        // 获取信息栈顶信息块的地址
        memory_block_info_struct    *block_top      = zf_memory_management.block_list;
        memory_block_info_struct    *block_bottom   = zf_memory_management.block_list;
        // 新建两个变量存储地址 用于后续的地址对齐计算
        uint32                      addr_temp_top   = 0;
        uint32                      addr_temp_bottom= 0;

        // 这里开始查询动态内存列表 这一步是为了查看是否有小块内存可以分配
        // 例如三块内存中间那块释放了 后续使用时可以申请这个位置
        // 因此必须存在至少两个区块时才会进行查询
        for(uint32 memory_loop = 2; zf_memory_management.block_count >= memory_loop; memory_loop ++)
        {
            // 首先获取前后两个区块的地址信息
            block_top           = zf_memory_management.block_list - (memory_loop - 2);
            block_bottom        = zf_memory_management.block_list - (memory_loop - 1);

            // 前一个区块的结束地址和后一个区块的起始地址进行对齐操作 根据传入的数据长度类型进行对齐
            // 操作逻辑也很简单 就是判断是否有余数 如果有余数就加上对应对齐字节数减去余数的结果 这样就得到下个对齐地址
            addr_temp_top       =   block_top->limit_addr;
            addr_temp_top       +=  ((addr_temp_top % type) ? (type - (addr_temp_top % type)) : (0));
            addr_temp_bottom    =   block_bottom->start_addr;
            addr_temp_bottom    +=  ((addr_temp_bottom % type) ? (type - (addr_temp_bottom % type)) : (0));

            // 判断两个区块间是否能塞进要申请的空间大小
            if(addr_temp_bottom - addr_temp_top >= size)
            {
                // 获取新申请的内存地址
                ptr = (void *)addr_temp_top;
                // 动态内存管理减去对应内存大小
                zf_memory_management.size           -= (size + sizeof(memory_block_info_struct));
                // 新增区块数量
                zf_memory_management.block_count    += 1;

                // 接下来开始遍历更新区块信息列表 将后面的区块后移 然后在对应的位置插入新的区块信息
                // 信息存储栈与系统栈一样 高地址为栈顶 新增的信息块会压入信息栈中 但会进行动态的排序
                // 操作逻辑是 碎片化分配时 是进行遍历查询 因此可以获取到对应的信息块位置
                // 先通过 block_count 将所有高地址的信息块后移 再将新插入的信息块放置到空出来的位置
                // 这样是为了方便后续碎片化操作时 从栈顶遍历一次就能知道是否能够进行碎片化分配
                for(block_top = zf_memory_management.block_list - zf_memory_management.block_count;
                    block_bottom > block_top;
                    block_top ++)
                {
                    // 区块信息转移
                    block_top->start_addr   = (block_top + 1)->start_addr;
                    block_top->limit_addr   = (block_top + 1)->limit_addr;
                }
                // 插入新申请的区块信息
                block_top->start_addr       = addr_temp_top;
                block_top->limit_addr       = block_top->start_addr + size;
                break;
            }
        }

        // 判断是否进行了碎片内存分配 如果没有的话就新增分配
        if(NULL == ptr)
        {
            // 如果是碎片内存分配就会有地址 否则就会进入这一步
            // 首先获取到最后一个区块的信息存储地址
            block_bottom        =   zf_memory_management.block_list - zf_memory_management.block_count;
            block_top           =   block_bottom == zf_memory_management.block_list ? block_bottom : block_bottom + 1;

            // 将最后一个区块的结束地址与区块信息存储的栈底地址进行对齐操作
            addr_temp_top       =   block_top->limit_addr;
            addr_temp_top       +=  ((addr_temp_top % type) ? (type - (addr_temp_top % type)) : (0));

            // 由于是新分配 因此信息栈向下生长
            // 同时肯定是新增块的起始地址跟着上一个块的结束地址
            // 所以进行对齐操作后 对这两个地址间的空间进行判断 是否能够满足本次分配的需求
            if( (       zf_memory_management.limit_addr
                    -   (zf_memory_management.block_count) * sizeof(memory_block_info_struct))
                    -   sizeof(memory_block_info_struct)
                    -   addr_temp_top
                >=  size)
            {
                // 获取新申请的内存地址
                ptr = (void *)addr_temp_top;
                // 动态内存管理减去对应内存大小
                zf_memory_management.size           -= (size + sizeof(memory_block_info_struct));
                // 新增区块数量
                zf_memory_management.block_count    += 1;

                // 插入新申请的区块信息
                block_bottom->start_addr            = (uint32)ptr;
                block_bottom->limit_addr            = block_bottom->start_addr + size;
            }
            else
            {
                break;
            }
        }
    }while(0);
    zf_memory_management_state.malloc = ZF_FALSE;                               // 申请操作占用解除

    return ptr;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     释放指针对应堆内存
// 参数说明     *ptr                内存空间地址
// 返回参数     uint8               ZF_NO_ERROR-成功 / ZF_ERROR-失败
// 使用示例     zf_memory_free(data_buffer);
// 备注信息     必须是申请内存时获取的地址数值 否则遍历内存占用后会直接退出
//              不支持多线程操作 不支持中断嵌套调用 不支持递归调用
//              本函数使用弱定义 这样做的目的是在移植操作系统时可以重载此函数
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK uint8 zf_memory_free (void *ptr)
{
    uint8 return_state = ZF_ERROR;

    do
    {
        if(zf_memory_management_state.state)                                    // 简易互斥判断
        {
            break;
        }
        zf_memory_management_state.free = ZF_TRUE;                              // 释放操作占用

        // 如果没有分配过内存就直接退出去
        // 没有分配过内存释放个锤子
        if(!zf_memory_management.block_count)
        {
            break;
        }

        // 获取信息栈底信息块的地址 也就是最后一个块
        // 这里为什么是 block_count - 1 因为 block_list 就是第一个块地址
        // 所以当 block_count == 1 时就是指向第一个块 因此需要 block_count - 1 才能正确指向对应的区块
        // 数学索引号和程序索引号的关系 数学索引从 1 开始 程序索引从 0 开始 有一个 -1 的关系
        // 这也就是为什么想要判断是否进行过内存分配 否则 0 - 1 就内存溢出了
        memory_block_info_struct    *block_first    = zf_memory_management.block_list - (zf_memory_management.block_count - 1);
        memory_block_info_struct    *block_second   = zf_memory_management.block_list - (zf_memory_management.block_count - 1);

        // 从信息栈底部开始 向上遍历区块信息列表 查询是否有对应的地址
        for(; zf_memory_management.block_list >= block_first; block_first ++)
        {
            // 如果有的话就释放对应的区块
            if((uint32)ptr == block_first->start_addr)
            {
                // 首先将区块数自减 并恢复对应的存储大小
                zf_memory_management.block_count    -= 1;
                zf_memory_management.size           += sizeof(memory_block_info_struct);
                zf_memory_management.size           += (block_first->limit_addr - block_first->start_addr);

                // 然后对信息栈进行调整 这里通过两个块指针判断是否需要进行块信息转移
                for(; block_second != block_first; block_first --)
                {
                    // 如果释放的是中间的地址 那么这里就会依次将底部的信息块上移
                    block_first->start_addr = (block_first - 1)->start_addr;
                    block_first->limit_addr = (block_first - 1)->limit_addr;
                }
                // 最后将底部信息块设置默认值
                block_second->start_addr    = ZF_HEAP_START_ADDR;
                block_second->limit_addr    = ZF_HEAP_START_ADDR;

                return_state = ZF_NO_ERROR;
                break;
            }
        }
    }while(0);
    zf_memory_management_state.free = ZF_FALSE;                                 // 释放操作占用解除

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     内存管理初始化
// 参数说明     void
// 返回参数     void            
// 使用示例     zf_memory_init();
// 备注信息     获取空闲内存地址 通常来说应该在系统时钟初始化自动调用
//              本函数使用弱定义 这样做的目的是在移植操作系统时可以重载此函数
//-------------------------------------------------------------------------------------------------------------------
ZF_WEAK void zf_memory_init (void)
{
    if(ZF_HEAP_START_ADDR != zf_memory_management.start_addr)
    {
        zf_memory_management.size                   = ZF_HEAP_SIZE;
        zf_memory_management.start_addr             = ZF_HEAP_START_ADDR;
        zf_memory_management.limit_addr             = ZF_HEAP_LIMIT_ADDR;
        zf_memory_management.block_count            = 0;

        zf_memory_management_state.state            = 0;

        memset((void *)ZF_HEAP_START_ADDR, 0, zf_memory_management.size);
        zf_memory_management.block_list             = (memory_block_info_struct *)(ZF_HEAP_LIMIT_ADDR - sizeof(memory_block_info_struct));
        zf_memory_management.block_list->start_addr = ZF_HEAP_START_ADDR;
        zf_memory_management.block_list->limit_addr = ZF_HEAP_START_ADDR;
    }
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
