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

// zf_common 层引用
#include "zf_common_debug.h"

// zf_driver 层引用
#include "zf_driver_interrupt.h"

// 自身头文件
#include "zf_driver_flash.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
zf_flash_data_union flash_union_buffer[USER_FLASH_DATA_BUFFER_SIZE];            // FLASH 操作的数据缓冲区
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
uint8 zf_flash_assert_handler (char *file, int line, char *tips)
{
    debug_message_handler(DEBUG_OUTPUT_LEVEL_ASSERT, ZF_FALSE, tips, file, line);
    while(1);
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 校验当前页是否有数据
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (USER_FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (USER_FLASH_MAX_PAGE_INDEX - 1)>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 区块空闲
//                                  FLASH_STATE_PAGE_OCCUPIED - 区块存在数据
//                                  其余值为操作异常报错
// 使用示例     zf_flash_check_page(sector_num, page_num);
// 备注信息     如果需要对有数据的区域写入新的数据则应该对所在扇区进行擦除操作
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_check_page (uint32 sector_num, uint32 page_num)
{
    zf_flash_operation_state_enum return_state = FLASH_ERROR_UNKNOW;

    do
    {
        if(zf_flash_assert(USER_FLASH_MAX_SECTION_INDEX > sector_num))          // 检查 扇区编号
        {
            // 此处如果断言报错 那么证明输入的区块号非法 超过了定义的 USER_FLASH_MAX_SECTION_INDEX 范围
            // 请按照函数注释限定操作的 FLASH 扇区编号
            return_state = FLASH_ERROR_SECTION_ILLEGAL;
            break;
        }
        if(zf_flash_assert(USER_FLASH_MAX_PAGE_INDEX > page_num))               // 检查 页码编号
        {
            // 此处如果断言报错 那么证明输入的页码号非法 超过了定义的 USER_FLASH_MAX_PAGE_INDEX 范围
            // 请按照函数注释限定操作的 FLASH 页码编号
            return_state = FLASH_ERROR_PAGE_ILLEGAL;
            break;
        }

        {
            uint32 blk_lock_state = 0x00000000UL;


            if (C55_OK != flash_getLock(&FLASHD0, C55_BLOCK_HIGH, (unsigned long *)(&blk_lock_state)))
            {
                return_state = FLASH_ERROR_OPERATION_LOCK_CHECK_ERROR;
                break;
            }
            if (C55_OK != flash_setLock(&FLASHD0, C55_BLOCK_HIGH, 0x00000000))
            {
                return_state = FLASH_ERROR_OPERATION_LOCK_CHECK_ERROR;
                break;
            }
            if (C55_OK != flash_getLock(&FLASHD0, C55_BLOCK_HIGH, (unsigned long *)(&blk_lock_state)))
            {
                return_state = FLASH_ERROR_OPERATION_LOCK_CHECK_ERROR;
                break;
            }

            if (blk_lock_state & 0x0000000F)
            {
                return_state = FLASH_ERROR_OPERATION_LOCK_CHECK_ERROR;
                break;
            }

            uint32 fail_address;
            uint32 fail_data;

            uint32  flash_addr  =   USER_FLASH_BASE_ADDR
                                +   USER_FLASH_SECTION_SIZE * sector_num
                                +   USER_FLASH_PAGE_SIZE * page_num;

            if(C55_OK != flash_blankCheck(&FLASHD0, flash_addr, USER_FLASH_PAGE_SIZE, &fail_address, &fail_data))
            {
                return_state = FLASH_STATE_PAGE_OCCUPIED;
                break;
            }
        }

        return_state = FLASH_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 擦除页
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (USER_FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (USER_FLASH_MAX_PAGE_INDEX - 1)>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 操作完成
//                                  FLASH_ERROR_OPERATION_TIMEOUT - FLASH 操作超时退出
//                                  其余值为操作异常报错
// 使用示例     zf_flash_erase_page(sector_num, page_num);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_erase_page (uint32 sector_num, uint32 page_num)
{
    zf_flash_operation_state_enum return_state = FLASH_ERROR_UNKNOW;

    do
    {
        if(zf_flash_assert(USER_FLASH_MAX_SECTION_INDEX > sector_num))          // 检查 扇区编号
        {
            // 此处如果断言报错 那么证明输入的区块号非法 超过了定义的 USER_FLASH_MAX_SECTION_INDEX 范围
            // 请按照函数注释限定操作的 FLASH 扇区编号
            return_state = FLASH_ERROR_SECTION_ILLEGAL;
            break;
        }
        if(zf_flash_assert(USER_FLASH_MAX_PAGE_INDEX > page_num))               // 检查 页码编号
        {
            // 此处如果断言报错 那么证明输入的页码号非法 超过了定义的 USER_FLASH_MAX_PAGE_INDEX 范围
            // 请按照函数注释限定操作的 FLASH 页码编号
            return_state = FLASH_ERROR_PAGE_ILLEGAL;
            break;
        }

        {
            return_state = FLASH_OPERATION_DONE;
            if(!zf_flash_check_page(sector_num, page_num))
            {
                break;
            }

            uint32          primask         = zf_interrupt_global_disable();

            uint32          low_block_select    = 0UL;
            uint32          mid_block_select    = 0UL;
            uint32          high_block_select   = 0x00000001UL << page_num;
            N256K_BLOCK_SEL n256k_block_select  = {.first256KBlockSelect = 0UL, .second256KBlockSelect = 0UL};

            flash_flashErase(
                &FLASHD0, C55_ERASE_MAIN,
                low_block_select, mid_block_select,
                high_block_select, n256k_block_select);

            zf_interrupt_global_enable(primask);
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 读取页
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (USER_FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (USER_FLASH_MAX_PAGE_INDEX - 1)>
// 参数说明     *buffer             需要读取的数据地址   传入的数组类型必须为 uint32
// 参数说明     len                 需要写入的数据长度   参数范围 <0 - USER_FLASH_PAGE_SIZE>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 操作完成
//                                  其余值为操作异常报错
// 使用示例     zf_flash_read_page(sector_num, page_num, buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_read_page (uint32 sector_num, uint32 page_num, uint32 *buffer, uint16 len)
{
    zf_flash_operation_state_enum return_state = FLASH_ERROR_UNKNOW;

    do
    {
        if(zf_flash_assert(USER_FLASH_MAX_SECTION_INDEX > sector_num))          // 检查 扇区编号
        {
            // 此处如果断言报错 那么证明输入的区块号非法 超过了定义的 USER_FLASH_MAX_SECTION_INDEX 范围
            // 请按照函数注释限定操作的 FLASH 扇区编号
            return_state = FLASH_ERROR_SECTION_ILLEGAL;
            break;
        }
        if(zf_flash_assert(USER_FLASH_MAX_PAGE_INDEX > page_num))               // 检查 页码编号
        {
            // 此处如果断言报错 那么证明输入的页码号非法 超过了定义的 USER_FLASH_MAX_PAGE_INDEX 范围
            // 请按照函数注释限定操作的 FLASH 页码编号
            return_state = FLASH_ERROR_PAGE_ILLEGAL;
            break;
        }

        if(zf_flash_assert(USER_FLASH_PAGE_SIZE >= len))                        // 检查 数据操作大小
        {
            // 此处如果断言报错 那么证明数据量超过定义的 USER_FLASH_DATA_BUFFER_SIZE 限制
            // 请按照函数注释限定操作的数据量
            return_state = FLASH_ERROR_DATA_SIZE_ILLEGAL;
            break;
        }
        if(zf_flash_assert(NULL != buffer))                                     // 检查 数据缓冲
        {
            // 此处如果断言报错 那么证明数据缓冲区为 NULL 空指针
            return_state = FLASH_ERROR_DATA_BUFFER_NULL;
            break;
        }

        uint16  temp_loop   = 0;
        uint32  flash_addr  =   USER_FLASH_BASE_ADDR
                            +   USER_FLASH_SECTION_SIZE * sector_num
                            +   USER_FLASH_PAGE_SIZE * page_num;                // 提取当前 Flash 地址

        for(temp_loop = 0; temp_loop < len; temp_loop += 4)                     // 根据指定长度读取
        {
            *buffer ++ = *(__IO uint32*)(flash_addr + temp_loop);               // 循环读取 Flash 的值
        }

        return_state = FLASH_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 编程页
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (USER_FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (USER_FLASH_MAX_PAGE_INDEX - 1)>
// 参数说明     *buffer             需要读取的数据地址   传入的数组类型必须为 uint32
// 参数说明     len                 需要写入的数据长度   参数范围 <0 - USER_FLASH_PAGE_SIZE>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 操作完成
//                                  FLASH_ERROR_OPERATION_TIMEOUT - FLASH 操作超时退出
//                                  其余值为操作异常报错
// 使用示例     zf_flash_write_page(sector_num, page_num, buffer, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_write_page (uint32 sector_num, uint32 page_num, const uint32 *buffer, uint16 len)
{
    zf_flash_operation_state_enum return_state = FLASH_ERROR_UNKNOW;

    do
    {
        if(zf_flash_assert(USER_FLASH_MAX_SECTION_INDEX > sector_num))          // 检查 扇区编号
        {
            // 此处如果断言报错 那么证明输入的区块号非法 超过了定义的 USER_FLASH_MAX_SECTION_INDEX 范围
            // 请按照函数注释限定操作的 FLASH 扇区编号
            return_state = FLASH_ERROR_SECTION_ILLEGAL;
            break;
        }
        if(zf_flash_assert(USER_FLASH_MAX_PAGE_INDEX > page_num))               // 检查 页码编号
        {
            // 此处如果断言报错 那么证明输入的页码号非法 超过了定义的 USER_FLASH_MAX_PAGE_INDEX 范围
            // 请按照函数注释限定操作的 FLASH 页码编号
            return_state = FLASH_ERROR_PAGE_ILLEGAL;
            break;
        }

        if(zf_flash_assert(USER_FLASH_PAGE_SIZE >= len))                        // 检查 数据操作大小
        {
            // 此处如果断言报错 那么证明数据量超过定义的 USER_FLASH_DATA_BUFFER_SIZE 限制
            // 请按照函数注释限定操作的数据量
            return_state = FLASH_ERROR_DATA_SIZE_ILLEGAL;
            break;
        }
        if(zf_flash_assert(NULL != buffer))                                     // 检查 数据缓冲
        {
            // 此处如果断言报错 那么证明数据缓冲区为 NULL 空指针
            return_state = FLASH_ERROR_DATA_BUFFER_NULL;
            break;
        }

        uint32          primask         = zf_interrupt_global_disable();


        {
            zf_flash_erase_page(sector_num, page_num);


            uint32  flash_addr  =   USER_FLASH_BASE_ADDR
                                +   USER_FLASH_SECTION_SIZE * sector_num
                                +   USER_FLASH_PAGE_SIZE * page_num;

            flash_flashProgram(&FLASHD0, FALSE, flash_addr, len, (uint32)buffer);
        }

        zf_interrupt_global_enable(primask);
        return_state = FLASH_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH  从指定扇区的指定页码读取数据到缓冲区
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (USER_FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (USER_FLASH_MAX_PAGE_INDEX - 1)>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 操作完成
//                                  其余值为操作异常报错
// 使用示例     zf_flash_read_page_to_buffer(sector_num, page_num);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_read_page_to_buffer (uint32 sector_num, uint32 page_num)
{
    return zf_flash_read_page(sector_num, page_num, (uint32 *)flash_union_buffer, sizeof(flash_union_buffer));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 向指定扇区的指定页码写入缓冲区的数据
// 参数说明     sector_num          需要写入的扇区编号 参数范围 <0 - (USER_FLASH_MAX_SECTION_INDEX - 1)>
// 参数说明     page_num            当前扇区页的编号   参数范围 <0 - (USER_FLASH_MAX_PAGE_INDEX - 1)>
// 返回参数     uint8               操作状态 ZF_NO_ERROR / FLASH_OPERATION_DONE - 操作完成
//                                  FLASH_ERROR_OPERATION_TIMEOUT - FLASH 操作超时退出
//                                  其余值为操作异常报错
// 使用示例     zf_flash_write_page_from_buffer(sector_num, page_num);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_flash_write_page_from_buffer (uint32 sector_num, uint32 page_num)
{
    return zf_flash_write_page(sector_num, page_num, (const uint32 *)flash_union_buffer, sizeof(flash_union_buffer));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 清空数据缓冲区
// 参数说明     void
// 返回参数     void
// 使用示例     zf_flash_buffer_clear();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_flash_buffer_clear (void)
{
    memset(flash_union_buffer, 0xFF, sizeof(flash_union_buffer));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     FLASH 初始化
// 参数说明     void
// 返回参数     void
// 使用示例     zf_flash_init();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_flash_init (void)
{
    flash_init();
    flash_start(&FLASHD0);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
