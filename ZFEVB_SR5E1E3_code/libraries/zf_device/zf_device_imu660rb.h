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
* 文件名称          zf_device_imu660rb
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

/*********************************************************************************************************************
* 接线定义          查看 zf_device_imu_interface.h 中接口资源定义
********************************************************************************************************************/

#ifndef _zf_device_imu660rb_h_
#define _zf_device_imu660rb_h_

// zf_common 层 类型定义 引用
#include "zf_common_typedef.h"

// zf_device 层头文件引用
#include "zf_device_imu_interface.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// imu660rb_get_physical_acc                                                    // 获取 IMU660RB 加速度计物理量数据
// imu660rb_get_physical_gyro                                                   // 获取 IMU660RB 陀螺仪物理量数据

// imu660rb_get_acc                                                             // 获取 IMU660RB 加速度计数据
// imu660rb_get_gyro                                                            // 获取 IMU660RB 陀螺仪数据

// imu660rb_set_config                                                          // IMU660RB 配置模块工作参数
// imu660rb_init                                                                // 初始化 IMU660RB
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 IMU660RB 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#define IMU660RB_TIMEOUT_COUNT              ( 32 )                              // IMU660RB 超时计数

// 加速度计 相关设置
// IMU660RB_ACC_OUTPUT_RATE_BASE 对应默认的 IMU660RB 的加速度计数据输出频率 实际为 52Hz

// IMU660RB 加速度计 支持的输出速率为
// IMU_ACC_OUTPUT_RATE_4_DIV        ->  12.5    Hz
// IMU_ACC_OUTPUT_RATE_2_DIV        ->  26      Hz
// IMU_ACC_OUTPUT_RATE_BASE         ->  52      Hz
// IMU_ACC_OUTPUT_RATE_2_MUL        ->  104     Hz
// IMU_ACC_OUTPUT_RATE_4_MUL        ->  208     Hz
// IMU_ACC_OUTPUT_RATE_8_MUL        ->  416     Hz
// IMU_ACC_OUTPUT_RATE_16_MUL       ->  833     Hz
// IMU_ACC_OUTPUT_RATE_32_MUL       ->  1666    Hz
// IMU_ACC_OUTPUT_RATE_64_MUL       ->  3332    Hz
// IMU_ACC_OUTPUT_RATE_128_MUL      ->  6667    Hz

// IMU660RB 加速度计 支持的量程为
// IMURA_ACC_RANGE_SGN_2G           ->  [-2 , 2 ]   G 
// IMURA_ACC_RANGE_SGN_4G           ->  [-4 , 4 ]   G 
// IMURA_ACC_RANGE_SGN_8G           ->  [-8 , 8 ]   G 
// IMURA_ACC_RANGE_SGN_16G          ->  [-16, 16]   G

// 陀螺仪 相关设置
// IMU660RB_GYRO_OUTPUT_RATE_BASE 对应默认的 IMU660RB 的陀螺仪数据输出频率 实际为 52Hz

// IMU660RB 陀螺仪 支持的输出速率为
// IMU_GYRO_OUTPUT_RATE_4_DIV       ->  12.5    Hz
// IMU_GYRO_OUTPUT_RATE_2_DIV       ->  26      Hz
// IMU_GYRO_OUTPUT_RATE_BASE        ->  52      Hz
// IMU_GYRO_OUTPUT_RATE_2_MUL       ->  104     Hz
// IMU_GYRO_OUTPUT_RATE_4_MUL       ->  208     Hz
// IMU_GYRO_OUTPUT_RATE_8_MUL       ->  416     Hz
// IMU_GYRO_OUTPUT_RATE_16_MUL      ->  833     Hz
// IMU_GYRO_OUTPUT_RATE_32_MUL      ->  1666    Hz
// IMU_GYRO_OUTPUT_RATE_64_MUL      ->  3332    Hz
// IMU_GYRO_OUTPUT_RATE_128_MUL     ->  6667    Hz

// IMU660RB 陀螺仪 支持的量程为
// IMU_GYRO_RANGE_SGN_125DPS        ->  [-125 , 125 ]   DPS 
// IMU_GYRO_RANGE_SGN_250DPS        ->  [-250 , 250 ]   DPS 
// IMU_GYRO_RANGE_SGN_500DPS        ->  [-500 , 500 ]   DPS 
// IMU_GYRO_RANGE_SGN_1000DPS       ->  [-1000, 1000]   DPS
// IMU_GYRO_RANGE_SGN_2000DPS       ->  [-2000, 2000]   DPS
// IMU_GYRO_RANGE_SGN_4000DPS       ->  [-4000, 4000]   DPS

typedef enum
{
    IMU660RB_OPERATION_DONE                 = IMU_INTERFACE_OPERATION_DONE              ,   // 操作完成
    IMU660RB_ERROR_UNKNOW_ERROR             = IMU_INTERFACE_ERROR_UNKNOW_ERROR          ,   // 异常错误 这个错误理论上不会被返回

    IMU660RB_WARNING_MODULE_DISABLE         = IMU_INTERFACE_WARNING_MODULE_DISABLE      ,   // 模块禁止警告 模块未使能
    IMU660RB_WARNING_OPERATION_TIMEOUT      = IMU_INTERFACE_WARNING_OPERATION_TIMEOUT   ,   // 模块超时警告 通信或者操作超时

    IMU660RB_ERROR_INTERFACE_OCCUPIED       = IMU_INTERFACE_ERROR_INTERFACE_OCCUPIED    ,   // 通信接口占用错误 不能重复初始化同一个接口
    IMU660RB_ERROR_INTERFACE_NOT_INIT       = IMU_INTERFACE_ERROR_INTERFACE_NOT_INIT    ,   // 接口未初始化错误 不能未初始化就调用接口
    IMU660RB_ERROR_INTERFACE_TYPE_UNKNOW    = IMU_INTERFACE_ERROR_INTERFACE_TYPE_UNKNOW ,   // 接口错误

    IMU660RB_ERROR_DATA_BUFFER_NULL         = IMU_INTERFACE_ERROR_DATA_BUFFER_NULL      ,   // 数据缓冲区错误 不可以传入空指针

    IMU660RB_ERROR_ID_CHECK_ERROR                                                       ,   // 模块 ID 匹配异常
    IMU660RB_ERROR_SELF_CHECK_ERROR                                                     ,   // 模块自检错误

    IMU660RB_ERROR_SET_CONFIG_ERROR                                                     ,   // 模块配置错误
    IMU660RB_ERROR_SET_ACC_OUTPUT_RATE_ERROR                                            ,   // 加速度计 输出速率 设置错误
    IMU660RB_ERROR_SET_ACC_RANGE_ERROR                                                  ,   // 加速度计 量程 设置错误
    IMU660RB_ERROR_SET_GYRO_OUTPUT_RATE_ERROR                                           ,   // 陀螺仪 输出速率 设置错误
    IMU660RB_ERROR_SET_GYRO_RANGE_ERROR                                                 ,   // 陀螺仪 量程 设置错误

}imu660rb_operation_state_enum;

typedef struct                                                                  // IMU660RB 数据格式模板 用于存储 IMU660RB 的数据
{
    int16       x   ;                                                           // X 轴的原始数据
    int16       y   ;                                                           // Y 轴的原始数据
    int16       z   ;                                                           // Z 轴的原始数据
}imu660rb_measurement_data_struct;

typedef struct                                                                  // IMU660RB 数据格式模板 用于存储 IMU660RB 的数据
{
    float       x   ;                                                           // X 轴的物理量数据
    float       y   ;                                                           // Y 轴的物理量数据
    float       z   ;                                                           // Z 轴的物理量数据
}imu660rb_physical_data_struct;
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU660RB 加速度计物理量数据
// 参数说明     *result         IMU660RB 的测量数据缓冲指针
// 返回参数     range           IMU660RB 加速度量程 详见 zf_device_imu_interface.h 中 imu_acc_range_enum 定义
// 参数说明     *physical_data  IMU660RB 的物理量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_get_physical_acc(result, range, physical_data);
// 备注信息     根据量程进行物理量换算
//              加速度计量程 ±2  g   获取到的加速度计数据除以 (1000.0 / 0.061) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//              加速度计量程 ±4  g   获取到的加速度计数据除以 (1000.0 / 0.122) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//              加速度计量程 ±8  g   获取到的加速度计数据除以 (1000.0 / 0.244) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//              加速度计量程 ±16 g   获取到的加速度计数据除以 (1000.0 / 0.488) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_get_physical_acc (imu660rb_measurement_data_struct *result, imu_acc_range_enum range,imu660rb_physical_data_struct *physical_data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU660RB 陀螺仪物理量数据
// 参数说明     *result         IMU660RB 的测量数据缓冲指针
// 返回参数     range           IMU660RB 加速度量程 详见 zf_device_imu_interface.h 中 imu_gyro_range_enum 定义
// 参数说明     *physical_data  IMU660RB 的物理量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_get_physical_gyro(result, range, physical_data);
// 备注信息     根据量程进行物理量换算
//              陀螺仪量程 ±125  dps    获取到的陀螺仪数据除以 (1000.0 / 4.375)  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±250  dps    获取到的陀螺仪数据除以 (1000.0 / 8.75 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±500  dps    获取到的陀螺仪数据除以 (1000.0 / 17.5 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±1000 dps    获取到的陀螺仪数据除以 (1000.0 / 35.0 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±2000 dps    获取到的陀螺仪数据除以 (1000.0 / 70.0 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±4000 dps    获取到的陀螺仪数据除以 (1000.0 / 140.0)  可以转化为带物理单位的数据 单位为 °/s
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_get_physical_gyro (imu660rb_measurement_data_struct *result, imu_gyro_range_enum range,imu660rb_physical_data_struct *physical_data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU660RB 加速度计数据
// 参数说明     *result         IMU660RB 的测量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_get_acc(result);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_get_acc (imu660rb_measurement_data_struct *result);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU660RB 陀螺仪数据
// 参数说明     *result         IMU660RB 的测量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_get_gyro(result);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_get_gyro (imu660rb_measurement_data_struct *result);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU660RB 配置模块工作参数
// 参数说明     item            IMU660RB 配置参照 zf_device_imu_interface.h 中 imu_config_item_enum 枚举体定义
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_set_config(item);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_set_config (imu_config_item_enum item);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 IMU660RB
// 参数说明     void
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_init (void);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif

