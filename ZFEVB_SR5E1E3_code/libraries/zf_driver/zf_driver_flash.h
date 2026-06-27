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
* 文件名称          zf_driver_flash
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

#ifndef _zf_driver_flash_h_
#define _zf_driver_flash_h_

// SDK 底层驱动
#include <flash.h>

// zf_common 层引用
#include "zf_common_typedef.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// flash_check                                                                  // FLASH 校验当前页是否有数据
// flash_erase_page                                                             // FLASH 擦除页

// flash_read_page                                                              // FLASH 读取页
// flash_write_page                                                             // FLASH 编程页

// flash_read_page_to_buffer                                                    // FLASH  从指定扇区的指定页码读取数据到缓冲区
// flash_write_page_from_buffer                                                 // FLASH 向指定扇区的指定页码写入缓冲区的数据

// flash_buffer_clear                                                           // FLASH 清空数据缓冲区
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 FLASH 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 以下宏定义为芯片固定设置 不允许用户修改
#define USER_FLASH_BASE_ADDR            ( 0x08F00000 )
#define USER_FLASH_MAX_SECTION_INDEX    ( 1 )
#define USER_FLASH_SECTION_SIZE         ( 4 * 16 * 1024 )                       // 64K byte
#define USER_FLASH_MAX_PAGE_INDEX       ( 4 )
#define USER_FLASH_PAGE_SIZE            ( 16 * 1024 )                           // 16K byte
#define USER_FLASH_OPERATION_TIME_OUT   ( 0x0FFF )

// 自动计算每个页能够存下多少个数据
#define USER_FLASH_DATA_BUFFER_SIZE     (USER_FLASH_PAGE_SIZE / sizeof(zf_flash_data_union))

typedef enum                                                                    // 枚举 FLASH 操作状态  此枚举定义不允许用户修改
{
    FLASH_OPERATION_DONE            = ZF_NO_ERROR   ,                           // FLASH 操作完成
    FLASH_ERROR_UNKNOW              = ZF_ERROR      ,                           // FLASH 未知错误 操作无法进行 正常情况下不会出现此错误

    FLASH_STATE_PAGE_OCCUPIED                       ,                           // FLASH 页块存在数据 需要根据情况判断

    FLASH_ERROR_SECTION_ILLEGAL                     ,                           // FLASH 区块索引错误 操作无法进行
    FLASH_ERROR_PAGE_ILLEGAL                        ,                           // FLASH 页码索引错误 操作无法进行

    FLASH_ERROR_DATA_BUFFER_NULL                    ,                           // FLASH 数据指针异常 操作无法进行
    FLASH_ERROR_DATA_SIZE_ILLEGAL                   ,                           // FLASH 数据大小异常 操作无法进行
    
    FLASH_ERROR_OPERATION_TIMEOUT                   ,                           // FLASH 操作响应超时 操作无法进行
    FLASH_ERROR_OPERATION_LOCK_CHECK_ERROR          ,                           // FLASH 锁定检查错误 操作无法进行
}zf_flash_operation_state_enum;

typedef union                                                                   // 固定的数据缓冲单元格式
{
    float           float_type      ;                                           // float  类型
    uint32          uint32_type     ;                                           // uint32 类型
    int32           int32_type      ;                                           // int32  类型
    uint16          uint16_type     ;                                           // uint16 类型
    int16           int16_type      ;                                           // int16  类型
    uint8           uint8_type      ;                                           // uint8  类型
    int8            int8_type       ;                                           // int8   类型
}zf_flash_data_union;                                                              // 所有类型数据共用同一个 32bit 地址

extern zf_flash_data_union flash_union_buffer[USER_FLASH_DATA_BUFFER_SIZE];
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_flash_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_assert_handler (char *file, int line, char *tips);
#define zf_flash_assert(status)     ((status) ? (0) : zf_flash_assert_handler(__FILE__, __LINE__, NULL))
#define zf_flash_log(status, tips)  ((status) ? (0) : zf_flash_assert_handler(__FILE__, __LINE__, (tips)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 校验当前页是否有数据
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (FLASH_MAX_PAGE_INDEX - 1)>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 区块空闲
//                                  FLASH_STATE_PAGE_OCCUPIED - 区块存在数据
//                                  其余值为操作异常报错
// 使用示例     zf_flash_check_page(sector_num, page_num);
// 备注信息     如果需要对有数据的区域写入新的数据则应该对所在扇区进行擦除操作
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_check_page (uint32 sector_num, uint32 page_num);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 擦除页
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (FLASH_MAX_PAGE_INDEX - 1)>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 操作完成
//                                  FLASH_ERROR_OPERATION_TIMEOUT - FLASH 操作超时退出
//                                  其余值为操作异常报错
// 使用示例     zf_flash_erase_page(sector_num, page_num);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_erase_page (uint32 sector_num, uint32 page_num);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 读取页
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (FLASH_MAX_PAGE_INDEX - 1)>
// 参数说明     *buffer             需要读取的数据地址   传入的数组类型必须为 uint32
// 参数说明     len                 需要写入的数据长度   参数范围 <0 - FLASH_DATA_BUFFER_SIZE>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 操作完成
//                                  其余值为操作异常报错
// 使用示例     zf_flash_read_page(sector_num, page_num, buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_read_page (uint32 sector_num, uint32 page_num, uint32 *buffer, uint16 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 编程页
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (FLASH_MAX_PAGE_INDEX - 1)>
// 参数说明     *buffer             需要读取的数据地址   传入的数组类型必须为 uint32
// 参数说明     len                 需要写入的数据长度   参数范围 <0 - 512>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 操作完成
//                                  FLASH_ERROR_OPERATION_TIMEOUT - FLASH 操作超时退出
//                                  其余值为操作异常报错
// 使用示例     zf_flash_write_page(sector_num, page_num, buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_write_page (uint32 sector_num, uint32 page_num, const uint32 *buffer, uint16 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH  从指定扇区的指定页码读取数据到缓冲区
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (FLASH_MAX_PAGE_INDEX - 1)>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 操作完成
//                                  其余值为操作异常报错
// 使用示例     zf_flash_read_page_to_buffer(sector_num, page_num);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_read_page_to_buffer (uint32 sector_num, uint32 page_num);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 向指定扇区的指定页码写入缓冲区的数据
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (FLASH_MAX_PAGE_INDEX - 1)>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 操作完成
//                                  FLASH_ERROR_OPERATION_TIMEOUT - FLASH 操作超时退出
//                                  其余值为操作异常报错
// 使用示例     zf_flash_write_page_from_buffer(sector_num, page_num);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_write_page_from_buffer (uint32 sector_num, uint32 page_num);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 清空数据缓冲区
// 参数说明     void
// 返回参数     void
// 使用示例     zf_flash_buffer_clear();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_flash_buffer_clear (void);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
