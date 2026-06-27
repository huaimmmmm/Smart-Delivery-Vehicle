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

#ifndef _zf_common_memory_h_
#define _zf_common_memory_h_

// zf_common 层引用
#include "zf_common_typedef.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// zf_memory_malloc                                                             // 从堆中申请对应大小内存
// zf_memory_free                                                               // 释放指针对应堆内存
// zf_memory_init                                                               // 内存管理初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件需要的枚举与对象结构等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef enum                                                                    // 此枚举定义不允许用户修改
{
    MEMORY_ALIGN_1BYTE          = 0x01      ,                                   // 一字节对齐
    MEMORY_ALIGN_2BYTE          = 0x02      ,                                   // 两字节对齐
    MEMORY_ALIGN_4BYTE          = 0x04      ,                                   // 四字节对齐
}memory_align_type_enum;                                                        // 申请内存的对齐类型

typedef struct                                                                  // 此结构定义不允许用户修改
{
    uint32                      start_addr  ;                                   // 区块起始地址
    uint32                      limit_addr  ;                                   // 区块结束地址
}memory_block_info_struct;                                                      // 动态内存区块信息

typedef struct                                                                  // 此结构定义不允许用户修改
{
    vuint32                     size        ;                                   // 内存池剩余大小
    vuint32                     start_addr  ;                                   // 内存池起始地址
    vuint32                     limit_addr  ;                                   // 内存池结束地址
    vuint32                     block_count ;                                   // 当前内存池已有区块数量
    memory_block_info_struct    *block_list ;                                   // 区块信息列表顶部地址
}memory_management_info_struct;                                                 // 动态内存池信息结构体
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 系统内存的地址等 如果移植到新的 MCU 上需要自行匹配对应的地址等
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define MEMORY_MANAGEMENT_AUTO          ( 1 )                                   // 如果 IDE 支持内存分散加载操作 就使用自动模式
#define MEMORY_MANAGEMENT_MANUAL        ( 0 )                                   // 如果 IDE 不支持内存分散加载操作 就使用手动模式定义一个大数组

#define MEMORY_MANAGEMENT_MODE          ( MEMORY_MANAGEMENT_MANUAL )

#if (MEMORY_MANAGEMENT_MODE)
// -------------------------- IDE 支持分散加载操作 --------------------------
#if (IDE_MDK & IDE_TYPE)
// -------------------------- MDK IDE 适配 --------------------------
// MDK 分散加载文件说明
// 通过分散加载文件进行内存的分配
// 按照顺序是 RW + ZI / HEAP / STACK / ZF_LIB_SECTION / ZF_HEAP
// 用户的全局变量在 RAM 靠前部分 然后是堆和栈
// 接着是逐飞科技库的库管理部分
// 最后是剩余的内存都划分给内存管理

// 定义系统内存地址
#define SYSTEM_RAM_BASE_ADDR    ( 0x20000000 )
#define SYSTEM_RAM_LIMIT_ADDR   ( 0x20020000 )
#define SYSTEM_RAM_SIZE         ( SYSTEM_RAM_LIMIT_ADDR - SYSTEM_RAM_BASE_ADDR )
// 定义空闲内存地址 本文件将接管这部分内存用于动态分配
#define ZF_HEAP_START_ADDR      ( (uint32)&Image$$ZF_LIB_SECTION$$RW$$Length + (uint32)&Image$$ZF_LIB_SECTION$$ZI$$Length + (uint32)&Image$$ZF_LIB_SECTION$$Base[0] )
#define ZF_HEAP_SIZE            ( SYSTEM_RAM_LIMIT_ADDR - ZF_HEAP_START_ADDR )
#define ZF_HEAP_LIMIT_ADDR      ( SYSTEM_RAM_LIMIT_ADDR )

//extern  unsigned int __Vectors[];
//#define VECTORS_START_ADDR      ((unsigned int)__Vectors)
//#define VECTORS_TABLE_SIZE      ( 0x1D8 )                                       // 全局不能定义变长的数组 所以这里是直接手动填
//#define VECTORS_END_ADDR        (VECTORS_START_ADDR + VECTORS_TABLE_SIZE)

// 获取程序内存尾地址 用于定位空闲内存
extern  unsigned int    Image$$ZF_LIB_SECTION$$RW$$Length;
extern  unsigned int    Image$$ZF_LIB_SECTION$$ZI$$Length;
extern  char            Image$$ZF_LIB_SECTION$$Base[];

// 定义指定内存段的宏定义 根据不同 IDE 进行适配
#define AT_ZF_LIB_SECTION       __attribute__((section(".ZF_LIB_SECTION.zf_library_section")))
#define AT_ZF_LIB_SECTION_START 
#define AT_ZF_LIB_SECTION_END   
// -------------------------- MDK IDE 适配 --------------------------
#elif (IDE_IAR & IDE_TYPE)
// -------------------------- IAR IDE 适配 --------------------------
// IAR 分散加载文件说明
// 通过分散加载文件进行内存的分配
// 按照顺序是 HEAP / STACK / RW + ZI / ZF_HEAP / ZF_LIB_SECTION
// 使用 block 划成三个段 第一个段存放堆和栈 地址从 RAM 首地址开始
// 第三个段从 RAM 尾地址往前 存放逐飞科技库的库管理部分
// 中间段从第一段的结尾到第三段的开始 存放用户的全局变量 以及剩余部分全部分配给内存管理

//extern unsigned int __VECTOR_TABLE;
//#define VECTORS_START_ADDR      ((unsigned int)__VECTOR_TABLE)
//#define VECTORS_TABLE_SIZE      ( 0x1D8 )                                       // 全局不能定义变长的数组 所以这里是直接手动填
//#define VECTORS_END_ADDR        (VECTORS_START_ADDR + VECTORS_TABLE_SIZE)

#pragma section = "ZF_HEAP"
#define ZF_HEAP_START_ADDR      ( (uint32)__section_begin("ZF_HEAP") )
#define ZF_HEAP_SIZE            ( (uint32)__section_size("ZF_HEAP") )
#define ZF_HEAP_LIMIT_ADDR      ( (uint32)__section_end("ZF_HEAP") )

#define AT_ZF_LIB_SECTION       _Pragma("location = \".zf_library_section\"")
#define AT_ZF_LIB_SECTION_START //_Pragma("segment = \"zf_library_section\"")
#define AT_ZF_LIB_SECTION_END   //_Pragma("segment = \"zf_library_section\"")
// -------------------------- IAR IDE 适配 --------------------------
#endif
// -------------------------- IDE 支持分散加载操作 --------------------------
#else
// -------------------------- IDE 不支持分散加载操作 --------------------------
// 如果 IDE 不能通过分散加载文件实现动态的内存自适应分配
// 那就仅实现逐飞科技内存动态内存管理即可
// 直接申请一个固定的大数组 然后直接对这个大数组进行操作
// 逐飞科技库的库管理部分就不再隔离 直接由 IDE 自行分配

#define ZF_HEAP_START_ADDR      ( (uint32)&memory_management_buffer )
#define ZF_HEAP_SIZE            ( 64 * 1024 )
#define ZF_HEAP_LIMIT_ADDR      ( ZF_HEAP_START_ADDR + ZF_HEAP_SIZE )

#define AT_ZF_LIB_SECTION       
#define AT_ZF_LIB_SECTION_START 
#define AT_ZF_LIB_SECTION_END   
// -------------------------- IDE 不支持分散加载操作 --------------------------
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_memory_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_memory_assert_handler (char *file, int line, char *tips);
#define zf_memory_assert(status)    ((status) ? (0) : zf_memory_assert_handler(__FILE__, __LINE__, NULL))
#define zf_memory_log(status, tips) ((status) ? (0) : zf_memory_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从堆中申请对应大小内存
// 参数说明     type                内存对齐类型    (详见 zf_common_memory.h 中枚举 memory_align_type_enum 定义)
// 参数说明     size                需要的内存空间大小 字节长度 如果需要 uint32 类型就 *4 如果 uint16 就 *2
// 返回参数     void *              返回分配的内存地址
// 使用示例     uint16 *data_buffer = (uint16 * )zf_memory_malloc(type, sizeof(uint16));
// 备注信息     不支持多线程操作 不支持中断嵌套调用 不支持递归调用
//              本函数使用弱定义 这样做的目的是在移植操作系统时可以重载此函数
//-------------------------------------------------------------------------------------------------------------------
void *zf_memory_malloc (memory_align_type_enum type, uint32 size);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     释放指针对应堆内存
// 参数说明     *ptr                内存空间地址
// 返回参数     uint8               ZF_NO_ERROR-成功 / ZF_ERROR-失败
// 使用示例     zf_memory_free(ptr);
// 备注信息     必须是申请内存时获取的地址数值 否则遍历内存占用后会直接退出
//              不支持多线程操作 不支持中断嵌套调用 不支持递归调用
//              本函数使用弱定义 这样做的目的是在移植操作系统时可以重载此函数
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_memory_free (void *ptr);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     内存管理初始化
// 参数说明     void
// 返回参数     void            
// 使用示例     zf_memory_init();
// 备注信息     获取空闲内存地址
//              本函数使用弱定义 这样做的目的是在移植操作系统时可以重载此函数
//-------------------------------------------------------------------------------------------------------------------
void zf_memory_init (void);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
