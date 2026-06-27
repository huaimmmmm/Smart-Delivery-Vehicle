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
* 文件名称          zf_device_imu_interface
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

/*********************************************************************************************************************
* 本文件的作用是方便 IMU 的任意平台适配
* 内容为将 IMU 相关的接口进行解耦整合
* 在移植到新的平台时 只需要实现了本文件的接口
* 就可以直接适配 SeekFree 所有的 V4 库 IMU 器件
********************************************************************************************************************/

#ifndef _zf_device_imu_interface_h_
#define _zf_device_imu_interface_h_

// 如果移植到其他平台 这里就替换成自己的资源头文件
#include "zf_common_headfile.h"
#include "zf_driver_headfile.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// imu_interface_assert                                                         // IMU 断言处理
// imu_interface_log                                                            // IMU Log 输出
// imu_interface_delay_ms                                                       // IMU 延时

// imu_interface_write_8bit_register                                            // IMU 写寄存器 8bit
// imu_interface_write_8bit_registers                                           // IMU 写寄存器 8bit
// imu_interface_read_8bit_register                                             // IMU 读寄存器 8bit
// imu_interface_read_8bit_registers                                            // IMU 读数据 8bit
// imu_interface_transfer_8bit_array                                            // IMU 双向传输 8bit 数据

// imu_interface_deinit                                                         // IMU 接口注销初始化

// imu_interface_spi_init                                                       // IMU SPI 通信 接口初始化
// imu_interface_soft_iic_init                                                  // IMU SOFT_IIC 通信 接口初始化

// imu_interface_spi_default_init                                               // IMU 使用默认引脚进行 SPI 通信 接口初始化
// imu_interface_soft_iic_default_init                                          // IMU 使用默认引脚进行 SOFT_IIC 通信 接口初始化
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 IMU 相关的接口资源 这里允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// 在 逐飞科技 开源库中默认支持多种驱动模式
// 自行移植的话看情况是否需要此处的多方式驱动结构
//====================================================硬件 SPI 驱动====================================================
#ifdef _zf_driver_spi_h_

#define IMU_INTERFACE_SPI_INDEX                 ( SPI_3             )           // 硬件 SPI 号
#define IMU_INTERFACE_SPI_SPEED                 ( 10 * 1000 * 1000  )           // 硬件 SPI 速率
#define IMU_INTERFACE_SPC_PIN                   ( SPI3_SCK_D10      )           // 硬件 SPI SCK  引脚  连接 IMU 的 SPC 引脚
#define IMU_INTERFACE_SDI_PIN                   ( SPI3_MOSI_D12     )           // 硬件 SPI MOSI 引脚  连接 IMU 的 SDI 引脚
#define IMU_INTERFACE_SDO_PIN                   ( SPI3_MISO_D11     )           // 硬件 SPI MISO 引脚  连接 IMU 的 SDO 引脚

#define IMU_INTERFACE_CS_PIN                    ( D7                )           // CS 片选引脚  连接 IMU 的 CS 引脚

#endif
//====================================================硬件 SPI 驱动====================================================

//====================================================软件 IIC 驱动====================================================
#ifdef _zf_driver_soft_iic_h_

#define IMU_INTERFACE_SOFT_IIC_INDEX            ( SOFT_IIC_7    )               // 软件 IIC 号
#define IMU_INTERFACE_SOFT_IIC_DELAY            ( 10            )               // 软件 IIC 的时钟延时周期 数值越小 IIC 通信速率越快
#define IMU_INTERFACE_SCL_PIN                   ( D9            )               // 软件 IIC SCL 引脚 连接 IMU 的 SCL 引脚
#define IMU_INTERFACE_SDA_PIN                   ( E8            )               // 软件 IIC SDA 引脚 连接 IMU 的 SDA 引脚

#endif
//====================================================软件 IIC 驱动====================================================

// 默认设置请查看下方枚举
// 需要注意的是 部分 IMU 可能不支持一些设置
// 在修改之前最好先去对应 IMU 文件中检查是否支持该设置
#define IMU_ACC_OUTPUT_RATE_DEFAULT             ( IMU_ACC_OUTPUT_RATE_BASE   )  // 在这设置默认的 加速度计  初始化输出频率
#define IMU_ACC_RANGE_DEFAULT                   ( IMU_ACC_RANGE_SGN_8G       )  // 在这设置默认的 加速度计  初始化量程
#define IMU_GYRO_OUTPUT_RATE_DEFAULT            ( IMU_GYRO_OUTPUT_RATE_4_MUL )  // 在这设置默认的 陀螺仪    初始化输出频率
#define IMU_GYRO_RANGE_DEFAULT                  ( IMU_GYRO_RANGE_SGN_2000DPS )  // 在这设置默认的 陀螺仪    初始化量程
#define IMU_MAG_OUTPUT_RATE_DEFAULT             ( IMU_MAG_OUTPUT_RATE_BASE   )  // 在这设置默认的 磁力计    初始化输出频率
#define IMU_MAG_RANGE_DEFAULT                   ( IMU_MAG_RANGE_8G           )  // 在这设置默认的 磁力计    初始化量程

// 独立内存管理部分 默认使用 逐飞科技 开源库中的内存分段定义
// 如果移植到其它平台后此处报错 可以将 IMU_INTERFACE_USE_ZF_COMMON_MEMORY 修改为 0
// 当 IMU_INTERFACE_USE_ZF_COMMON_MEMORY 为 0 会禁止内存分段指定 通过编译器随机分配
#define     IMU_INTERFACE_USE_ZF_COMMON_MEMORY  ( 1 )
#if         IMU_INTERFACE_USE_ZF_COMMON_MEMORY
#ifndef _zf_common_memory_h_
#define AT_ZF_LIB_SECTION        
#define AT_ZF_LIB_SECTION_START  
#define AT_ZF_LIB_SECTION_END    
#endif
#else
#define AT_ZF_LIB_SECTION        
#define AT_ZF_LIB_SECTION_START  
#define AT_ZF_LIB_SECTION_END    
#endif

// 调试信息输出部分 默认使用 逐飞科技 开源库中的调试接口
// 如果移植到其它平台后此处报错 可以将 IMU_INTERFACE_USE_ZF_COMMON_DEBUG 修改为 0
// 当 IMU_INTERFACE_USE_ZF_COMMON_DEBUG 为 0 会通过 imu_assert 使用 printf 输出报错
#define     IMU_INTERFACE_USE_ZF_COMMON_DEBUG   ( 1 )
#if         IMU_INTERFACE_USE_ZF_COMMON_DEBUG
#ifndef _zf_common_debug_h_
#define zf_assert(pass)         ((pass) ? (0) : (1))
#define zf_log(pass, str)       ((pass) ? (0) : (1))
#endif
#endif

// 延时定向部分 默认使用 逐飞科技 开源库中的延时接口
// 如果移植到其它平台后此处报错 可以将 IMU_INTERFACE_USE_ZF_DIRVER_DELAY 修改为 0
// 当 IMU_INTERFACE_USE_ZF_DIRVER_DELAY 为 0 会通过 imu 的软延时实现
#define     IMU_INTERFACE_USE_ZF_DIRVER_DELAY   ( 1 )
#if         IMU_INTERFACE_USE_ZF_DIRVER_DELAY
#ifndef _zf_driver_delay_h_
#define zf_delay_ms(time)   for(vuint32 i = time * 1000000; i --; )
#endif
#endif

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 IMU 相关的控制对象格式 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

typedef enum
{
    IMU_INTERFACE_TYPE_UNKNOW       = 0 ,

    IMU_INTERFACE_TYPE_SPI              ,
    IMU_INTERFACE_TYPE_SOFT_IIC         ,
}imu_interface_type_enum;

// 加速度计 相关设置
// 各 IMU 模块的基准频率不一样 例如 IMU660 基准为 50 Hz IMU963 基准为 52Hz
// 实际会略有偏差 需要去查看对应的芯片手册 或者查看各模块头文件标注
typedef enum
{
    IMU_ACC_OUTPUT_RATE_64_DIV      = 2 ,                                       // 加速度计输出频率 64 分频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_32_DIV          ,                                       // 加速度计输出频率 32 分频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_16_DIV          ,                                       // 加速度计输出频率 16 分频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_8_DIV           ,                                       // 加速度计输出频率 8  分频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_4_DIV           ,                                       // 加速度计输出频率 4  分频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_2_DIV           ,                                       // 加速度计输出频率 2  分频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_BASE            ,                                       // 加速度计输出频率基础频率 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_2_MUL           ,                                       // 加速度计输出频率 2  倍频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_4_MUL           ,                                       // 加速度计输出频率 4  倍频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_8_MUL           ,                                       // 加速度计输出频率 8  倍频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_16_MUL          ,                                       // 加速度计输出频率 16 倍频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_32_MUL          ,                                       // 加速度计输出频率 32 倍频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_64_MUL          ,                                       // 加速度计输出频率 64 倍频 (ACC = Accelerometer 加速度计)
    IMU_ACC_OUTPUT_RATE_128_MUL         ,                                       // 加速度计输出频率 128倍频 (ACC = Accelerometer 加速度计)
}imu_acc_output_rate_enum;

typedef enum
{
    IMU_ACC_RANGE_SGN_2G            = 2 ,                                       // 加速度计量程 ±2 g (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
    IMU_ACC_RANGE_SGN_4G                ,                                       // 加速度计量程 ±4 g (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
    IMU_ACC_RANGE_SGN_8G                ,                                       // 加速度计量程 ±8 g (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
    IMU_ACC_RANGE_SGN_16G               ,                                       // 加速度计量程 ±16g (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
}imu_acc_range_enum;

// 陀螺仪 相关设置
// 各 IMU 模块的基准频率不一样 例如 IMU660 基准为 50 Hz IMU963 基准为 52Hz
// 实际会略有偏差 需要去查看对应的芯片手册 或者查看各模块头文件标注
typedef enum
{
    IMU_GYRO_OUTPUT_RATE_4_DIV      = 6 ,                                       // 陀螺仪输出频率 4  分频 (GYRO = Gyroscope 陀螺仪)
    IMU_GYRO_OUTPUT_RATE_2_DIV          ,                                       // 陀螺仪输出频率 2  分频 (GYRO = Gyroscope 陀螺仪)
    IMU_GYRO_OUTPUT_RATE_BASE           ,                                       // 陀螺仪输出频率基础频率 (GYRO = Gyroscope 陀螺仪)
    IMU_GYRO_OUTPUT_RATE_2_MUL          ,                                       // 陀螺仪输出频率 2  倍频 (GYRO = Gyroscope 陀螺仪)
    IMU_GYRO_OUTPUT_RATE_4_MUL          ,                                       // 陀螺仪输出频率 4  倍频 (GYRO = Gyroscope 陀螺仪)
    IMU_GYRO_OUTPUT_RATE_8_MUL          ,                                       // 陀螺仪输出频率 8  倍频 (GYRO = Gyroscope 陀螺仪)
    IMU_GYRO_OUTPUT_RATE_16_MUL         ,                                       // 陀螺仪输出频率 16 倍频 (GYRO = Gyroscope 陀螺仪)
    IMU_GYRO_OUTPUT_RATE_32_MUL         ,                                       // 陀螺仪输出频率 32 倍频 (GYRO = Gyroscope 陀螺仪)
    IMU_GYRO_OUTPUT_RATE_64_MUL         ,                                       // 陀螺仪输出频率 64 倍频 (GYRO = Gyroscope 陀螺仪)
    IMU_GYRO_OUTPUT_RATE_128_MUL        ,                                       // 陀螺仪输出频率 128倍频 (GYRO = Gyroscope 陀螺仪)
}imu_gyro_output_rate_enum;

typedef enum
{
    IMU_GYRO_RANGE_SGN_125DPS       = 2 ,                                       // 陀螺仪量程 ±125DPS  (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    IMU_GYRO_RANGE_SGN_250DPS           ,                                       // 陀螺仪量程 ±250DPS  (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    IMU_GYRO_RANGE_SGN_500DPS           ,                                       // 陀螺仪量程 ±500DPS  (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    IMU_GYRO_RANGE_SGN_1000DPS          ,                                       // 陀螺仪量程 ±1000DPS (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    IMU_GYRO_RANGE_SGN_2000DPS          ,                                       // 陀螺仪量程 ±2000DPS (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    IMU_GYRO_RANGE_SGN_4000DPS          ,                                       // 陀螺仪量程 ±4000DPS (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
}imu_gyro_range_enum;

// 磁力计 相关设置
// 各 IMU 模块的基准频率不一样 例如 IMU963 基准为 50Hz
// 实际会略有偏差 需要去查看对应的芯片手册 因此对应从低到高可以设置为
typedef enum
{
    IMU_MAG_OUTPUT_RATE_4_DIV       = 6 ,                                       // 磁力计输出频率 4  分频 (MAG = Magnetometer 磁力计)
    IMU_MAG_OUTPUT_RATE_2_DIV           ,                                       // 磁力计输出频率 4  分频 (MAG = Magnetometer 磁力计)
    IMU_MAG_OUTPUT_RATE_BASE            ,                                       // 磁力计输出频率基础频率 (MAG = Magnetometer 磁力计)
    IMU_MAG_OUTPUT_RATE_2_MUL           ,                                       // 磁力计输出频率 2  倍频 (MAG = Magnetometer 磁力计)
    IMU_MAG_OUTPUT_RATE_4_MUL           ,                                       // 磁力计输出频率 4  倍频 (MAG = Magnetometer 磁力计)
}imu_mag_output_rate_enum;

typedef enum
{
    IMU_MAG_RANGE_2G                = 2 ,                                       // 磁力计量程 2G (MAG = Magnetometer 磁力计) (G = Gs 高斯)
    IMU_MAG_RANGE_8G                    ,                                       // 磁力计量程 8G (MAG = Magnetometer 磁力计) (G = Gs 高斯)
}imu_mag_range_enum;

#define IMU_CONFIG_TYPE_OFFSET         ( 4     )                                // 累计低位的位移后从 16bit 开始
#define IMU_CONFIG_TYPE_MASK           ( 0x0F  )                                // 同样根据需要添加的引脚服用功能掩码 这里也是 4bit 因此掩码为 0xF

#define IMU_CONFIG_VALUE_OFFSET        ( 0     )                                // 累计低位的位移后从 16bit 开始
#define IMU_CONFIG_VALUE_MASK          ( 0x0F  )                                // 同样根据需要添加的引脚服用功能掩码 这里也是 4bit 因此掩码为 0xF

typedef enum                                                                    // 枚举 IMU 配置选项 此枚举定义不允许用户修改
{
    IMU_CONFIG_ACC_OUTPUT_RATE_64_DIV       = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_64_DIV    ),
    IMU_CONFIG_ACC_OUTPUT_RATE_32_DIV       = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_32_DIV    ),
    IMU_CONFIG_ACC_OUTPUT_RATE_16_DIV       = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_16_DIV    ),
    IMU_CONFIG_ACC_OUTPUT_RATE_8_DIV        = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_8_DIV     ),
    IMU_CONFIG_ACC_OUTPUT_RATE_4_DIV        = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_4_DIV     ),
    IMU_CONFIG_ACC_OUTPUT_RATE_2_DIV        = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_2_DIV     ),
    IMU_CONFIG_ACC_OUTPUT_RATE_BASE         = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_BASE      ),
    IMU_CONFIG_ACC_OUTPUT_RATE_2_MUL        = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_2_MUL     ),
    IMU_CONFIG_ACC_OUTPUT_RATE_4_MUL        = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_4_MUL     ),
    IMU_CONFIG_ACC_OUTPUT_RATE_8_MUL        = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_8_MUL     ),
    IMU_CONFIG_ACC_OUTPUT_RATE_16_MUL       = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_16_MUL    ),
    IMU_CONFIG_ACC_OUTPUT_RATE_32_MUL       = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_32_MUL    ),
    IMU_CONFIG_ACC_OUTPUT_RATE_64_MUL       = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_64_MUL    ),
    IMU_CONFIG_ACC_OUTPUT_RATE_128_MUL      = ((0x00 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_OUTPUT_RATE_128_MUL   ),

    IMU_CONFIG_ACC_RANGE_SGN_2G             = ((0x01 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_RANGE_SGN_2G          ),
    IMU_CONFIG_ACC_RANGE_SGN_4G             = ((0x01 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_RANGE_SGN_4G          ),
    IMU_CONFIG_ACC_RANGE_SGN_8G             = ((0x01 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_RANGE_SGN_8G          ),
    IMU_CONFIG_ACC_RANGE_SGN_16G            = ((0x01 << IMU_CONFIG_TYPE_OFFSET) | IMU_ACC_RANGE_SGN_16G         ),

    IMU_CONFIG_GYRO_OUTPUT_RATE_4_DIV       = ((0x02 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_OUTPUT_RATE_4_DIV    ),
    IMU_CONFIG_GYRO_OUTPUT_RATE_2_DIV       = ((0x02 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_OUTPUT_RATE_2_DIV    ),
    IMU_CONFIG_GYRO_OUTPUT_RATE_BASE        = ((0x02 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_OUTPUT_RATE_BASE     ),
    IMU_CONFIG_GYRO_OUTPUT_RATE_2_MUL       = ((0x02 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_OUTPUT_RATE_2_MUL    ),
    IMU_CONFIG_GYRO_OUTPUT_RATE_4_MUL       = ((0x02 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_OUTPUT_RATE_4_MUL    ),
    IMU_CONFIG_GYRO_OUTPUT_RATE_8_MUL       = ((0x02 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_OUTPUT_RATE_8_MUL    ),
    IMU_CONFIG_GYRO_OUTPUT_RATE_16_MUL      = ((0x02 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_OUTPUT_RATE_16_MUL   ),
    IMU_CONFIG_GYRO_OUTPUT_RATE_32_MUL      = ((0x02 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_OUTPUT_RATE_32_MUL   ),
    IMU_CONFIG_GYRO_OUTPUT_RATE_64_MUL      = ((0x02 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_OUTPUT_RATE_64_MUL   ),
    IMU_CONFIG_GYRO_OUTPUT_RATE_128_MUL     = ((0x02 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_OUTPUT_RATE_128_MUL  ),

    IMU_CONFIG_GYRO_RANGE_SGN_125DPS        = ((0x03 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_RANGE_SGN_125DPS     ),
    IMU_CONFIG_GYRO_RANGE_SGN_250DPS        = ((0x03 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_RANGE_SGN_250DPS     ),
    IMU_CONFIG_GYRO_RANGE_SGN_500DPS        = ((0x03 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_RANGE_SGN_500DPS     ),
    IMU_CONFIG_GYRO_RANGE_SGN_1000DPS       = ((0x03 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_RANGE_SGN_1000DPS    ),
    IMU_CONFIG_GYRO_RANGE_SGN_2000DPS       = ((0x03 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_RANGE_SGN_2000DPS    ),
    IMU_CONFIG_GYRO_RANGE_SGN_4000DPS       = ((0x03 << IMU_CONFIG_TYPE_OFFSET) | IMU_GYRO_RANGE_SGN_4000DPS    ),

    IMU_CONFIG_MAG_OUTPUT_RATE_4_DIV        = ((0x04 << IMU_CONFIG_TYPE_OFFSET) | IMU_MAG_OUTPUT_RATE_4_DIV     ),
    IMU_CONFIG_MAG_OUTPUT_RATE_BASE         = ((0x04 << IMU_CONFIG_TYPE_OFFSET) | IMU_MAG_OUTPUT_RATE_BASE      ),
    IMU_CONFIG_MAG_OUTPUT_RATE_2_MUL        = ((0x04 << IMU_CONFIG_TYPE_OFFSET) | IMU_MAG_OUTPUT_RATE_2_MUL     ),
    IMU_CONFIG_MAG_OUTPUT_RATE_4_MUL        = ((0x04 << IMU_CONFIG_TYPE_OFFSET) | IMU_MAG_OUTPUT_RATE_4_MUL     ),

    IMU_CONFIG_MAG_RANGE_2G                 = ((0x05 << IMU_CONFIG_TYPE_OFFSET) | IMU_MAG_RANGE_2G              ),
    IMU_CONFIG_MAG_RANGE_8G                 = ((0x05 << IMU_CONFIG_TYPE_OFFSET) | IMU_MAG_RANGE_8G              ),
}imu_config_item_enum;

typedef enum
{
    IMU_INTERFACE_OPERATION_DONE                    = ZF_NO_ERROR   ,           // 操作完成
    IMU_INTERFACE_ERROR_UNKNOW_ERROR                = ZF_ERROR      ,           // 异常错误 这个错误理论上不会被返回

    IMU_INTERFACE_WARNING_MODULE_DISABLE            = 0x10          ,           // 模块禁止警告 模块未使能
    IMU_INTERFACE_WARNING_OPERATION_TIMEOUT                         ,           // 模块超时警告 通信或者操作超时

    IMU_INTERFACE_ERROR_INTERFACE_OCCUPIED          = 0x20          ,           // 通信接口占用错误 不能重复初始化同一个接口
    IMU_INTERFACE_ERROR_INTERFACE_NOT_INIT                          ,           // 接口未初始化错误 不能未初始化就调用接口
    IMU_INTERFACE_ERROR_INTERFACE_TYPE_UNKNOW                       ,           // 接口错误

    IMU_INTERFACE_ERROR_OBJ_BUFFER_NULL                             ,           // 操作对象空指针 不可以传入空指针
    IMU_INTERFACE_ERROR_DATA_BUFFER_NULL                            ,           // 数据缓冲区错误 不可以传入空指针
}imu_interface_operation_state_enum;

typedef struct                                                                  // IMU 模块化解耦结构
{
    imu_interface_type_enum                 interface_type          ;           // 必须要的接口类型
    union
    {
        zf_spi_index_enum                   spi_index               ;           // 硬件 SPI 索引
        zf_soft_iic_index_enum              soft_iic_index          ;           // 软件 IIC 索引
        uint8                               communication_index     ;
    };

    zf_gpio_pin_enum                        spi_cs_pin              ;           // 模块引脚 SPI CS 片选
}imu_interface_obj_struct;

typedef uint8   (*imu_func_get_acc      )   (imu_interface_obj_struct *interface, void *result);
typedef uint8   (*imu_func_get_gyro     )   (imu_interface_obj_struct *interface, void *result);
typedef uint8   (*imu_func_get_mag      )   (imu_interface_obj_struct *interface, void *result);
typedef uint8   (*imu_func_set_config   )   (imu_interface_obj_struct *interface, uint8 item);
typedef uint8   (*imu_func_ckeck_id     )   (imu_interface_obj_struct *interface);
typedef uint8   (*imu_func_init         )   (imu_interface_obj_struct *interface);

typedef struct
{
    imu_func_get_acc        get_acc     ;
    imu_func_get_gyro       get_gyro    ;
    imu_func_get_mag        get_mag     ;
    imu_func_set_config     set_config  ;
    imu_func_ckeck_id       ckeck_id    ;
    imu_func_init           init        ;
}imu_module_func_list;

extern imu_interface_obj_struct imu_interface_default_obj;
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_spi_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
#if IMU_INTERFACE_USE_ZF_COMMON_DEBUG
#define imu_interface_assert(pass)       zf_assert(pass)
#else
uint8 imu_interface_assert_handler (char *file, int line, char *tips);
#define imu_interface_assert(status)     ((status) ? (0) : imu_interface_assert_handler(__FILE__, __LINE__, NULL))
#define imu_interface_log(status, tips)  ((status) ? (0) : imu_interface_assert_handler(__FILE__, __LINE__, (tips)))
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 延时
// 参数说明     ms                  毫秒数
// 返回参数     void
// 使用示例     imu_interface_delay_ms(ms);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#if IMU_INTERFACE_USE_ZF_DIRVER_DELAY
#define imu_interface_delay_ms(ms)       zf_delay_ms(ms)
#else
void imu_interface_delay_ms (uint32 ms);
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 写寄存器 8bit
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 参数说明     addr                IIC 器件地址 SPI 通信时这个参数会被忽略
// 参数说明     reg                 寄存器地址
// 参数说明     data                数据
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_write_8bit_register(interface, addr, reg, data);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_interface_write_8bit_register (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, uint8 data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 写寄存器 8bit
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 参数说明     addr                IIC 器件地址 SPI 通信时这个参数会被忽略
// 参数说明     reg                 寄存器地址
// 参数说明     data                数据缓冲区
// 参数说明     len                 数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_write_8bit_registers(interface, addr, reg, data, len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_interface_write_8bit_registers (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, const uint8 *data, uint32 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 读寄存器 8bit
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 参数说明     addr                IIC 器件地址 SPI 通信时这个参数会被忽略
// 参数说明     reg                 寄存器地址
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_read_8bit_register(interface, addr, reg, data);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_interface_read_8bit_register (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, uint8 *data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 读数据 8bit
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 参数说明     addr                IIC 器件地址 SPI 通信时这个参数会被忽略
// 参数说明     reg                 寄存器地址
// 参数说明     data                数据缓冲区
// 参数说明     len                 数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_read_8bit_registers(interface, addr, reg, data, len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_interface_read_8bit_registers (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, uint8 *data, uint32 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 双向传输 8bit 数据
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 参数说明     addr                IIC 器件地址 SPI 通信时这个参数会被忽略
// 参数说明     *send_data          发送数据缓冲区
// 参数说明     send_len            发送数据长度
// 参数说明     *receive_data       接收数据缓冲区
// 参数说明     receive_len         接收数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_transfer_8bit_array(interface, addr, send_data, send_len, receive_data, receive_len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_interface_transfer_8bit_array (imu_interface_obj_struct *interface, uint8 addr, const uint8 *send_data, uint32 send_len, uint8 *receive_data, uint32 receive_len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 接口注销初始化
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_deinit(interface);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_interface_deinit (imu_interface_obj_struct *interface);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU SPI 通信 接口初始化
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 参数说明     spi_index           SPI 索引        (详见 zf_driver_spi.h 内 zf_spi_index_enum 定义)
// 参数说明     mode                SPI 模式        (详见 zf_driver_spi.h 内 zf_spi_mode_enum 定义)
// 参数说明     baudrate            设置 SPI 的波特率 不超过系统时钟的一半 会被适配成相近的速率
// 参数说明     sck_pin             选择 SCK 引脚   (详见 zf_driver_spi.h 内 zf_spi_sck_pin_enum 定义)
// 参数说明     mosi_pin            选择 MOSI 引脚  (详见 zf_driver_spi.h 内 zf_spi_mosi_pin_enum 定义)
// 参数说明     miso_pin            选择 MISO 引脚  (详见 zf_driver_spi.h 内 zf_spi_miso_pin_enum 定义)
// 参数说明     cs_pin              使用软件 CS 控制 所以此处填写 GPIO 索引
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_spi_init(interface, spi_index, mode, baudrate, sck_pin, mosi_pin, miso_pin, cs_pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_interface_spi_init (imu_interface_obj_struct *interface, zf_spi_index_enum spi_index, zf_spi_mode_enum mode, uint32 baudrate, zf_spi_sck_pin_enum sck_pin, zf_spi_mosi_pin_enum mosi_pin, zf_spi_miso_pin_enum miso_pin, zf_gpio_pin_enum cs_pin);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU SOFT_IIC 通信 接口初始化
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 参数说明     soft_iic_index      SOFT_IIC 索引   (详见 zf_driver_soft_iic.h 内 zf_soft_iic_index_enum 定义)
// 参数说明     delay               SOFT_IIC 延时 就是时钟电平维持时间 越短 IIC 速率越高 最小值为 1
// 参数说明     scl_pin             SCL 引脚 参照   (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 参数说明     sda_pin             SDA 引脚 参照   (详见 zf_driver_gpio.h 内 zf_gpio_pin_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_soft_iic_init(interface, soft_iic_index, delay, scl_pin, sda_pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_interface_soft_iic_init (imu_interface_obj_struct *interface, zf_soft_iic_index_enum soft_iic_index, uint32 delay, zf_gpio_pin_enum scl_pin, zf_gpio_pin_enum sda_pin);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 使用默认资源进行 SPI 通信 接口初始化
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 参数说明     mode                SPI 模式        (详见 zf_driver_spi.h 内 spi_mode_enum 定义)
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_spi_default_init(interface, mode);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define imu_interface_spi_default_init(interface, mode)             imu_interface_spi_init(interface, IMU_INTERFACE_SPI_INDEX, mode, IMU_INTERFACE_SPI_SPEED, IMU_INTERFACE_SPC_PIN, IMU_INTERFACE_SDI_PIN, IMU_INTERFACE_SDO_PIN, IMU_INTERFACE_CS_PIN)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 使用默认资源进行 SOFT_IIC 通信 接口初始化
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_soft_iic_default_init(interface);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define imu_interface_soft_iic_default_init(interface)              imu_interface_soft_iic_init(interface, IMU_INTERFACE_SOFT_IIC_INDEX, IMU_INTERFACE_SOFT_IIC_DELAY, IMU_INTERFACE_SCL_PIN, IMU_INTERFACE_SDA_PIN)
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
