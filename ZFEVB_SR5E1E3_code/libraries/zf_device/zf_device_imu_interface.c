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

// 自身头文件
#include "zf_device_imu_interface.h"

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION imu_interface_obj_struct imu_interface_default_obj = 
{
    .interface_type         = IMU_INTERFACE_TYPE_UNKNOW,
    .communication_index    = 0,
    .spi_cs_pin             = IMU_INTERFACE_CS_PIN
};
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 IMU 相关的接口实现 这里允许用户修改
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
#else
uint8 imu_interface_assert_handler (char *file, int line, char *tips)
{
    printf("\r\n IMU OPERATION ASSERT!\r\n");
    printf("\r\n file: %s.\r\n", file);
    printf("\r\n line: %d.\r\n", line);
    if(NULL != tips)
    {
        printf("\r\n tips: %s.\r\n", tips);
    }
    while(1);
    return ZF_ERROR;
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 延时
// 参数说明     ms              毫秒数
// 返回参数     void
// 使用示例     imu_interface_delay_ms(ms);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#if IMU_INTERFACE_USE_ZF_DIRVER_DELAY
#else
void imu_interface_delay_ms (uint32 ms)
{
    for(uint32 loop_count1 = ms    ; loop_count1 --; )
    for(uint32 loop_count2 = 0xFFFF; loop_count2 --; );
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 写寄存器 8bit
// 参数说明     *interface          接口对象 IMU 系列通用的接口对象
// 参数说明     addr                IIC 器件地址 非 IIC 器件就填 0
// 参数说明     reg                 寄存器地址
// 参数说明     data                数据
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_write_8bit_register(interface, addr, reg, data);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_interface_write_8bit_register (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, uint8 data)
{
    imu_interface_operation_state_enum  return_state    =   IMU_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = IMU_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(IMU_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = IMU_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        return_state = IMU_INTERFACE_OPERATION_DONE;
        if(IMU_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
            zf_spi_transfer_8bit_array(interface->spi_index, &reg , NULL, 1);
            zf_spi_transfer_8bit_array(interface->spi_index, &data, NULL, 1);
            zf_gpio_high(interface->spi_cs_pin);
        }
        else
        {
            zf_soft_iic_write_8bit_splicing_array(interface->soft_iic_index, addr, &reg, 1, &data, 1);
        }

    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 写寄存器 8bit
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 参数说明     addr                IIC 器件地址 SPI 通信时这个参数会被忽略
// 参数说明     reg                 寄存器地址
// 参数说明     *data               数据缓冲区
// 参数说明     len                 数据长度
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_write_8bit_registers(interface, addr, reg, data, len);
// 备注信息     自行实现此处接口时 按照标准传输格式实现即可
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_interface_write_8bit_registers (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, const uint8 *data, uint32 len)
{
    imu_interface_operation_state_enum  return_state    =   IMU_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = IMU_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(NULL == data)
        {
            return_state = IMU_INTERFACE_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(IMU_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = IMU_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        return_state = IMU_INTERFACE_OPERATION_DONE;
        if(IMU_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
            zf_spi_transfer_8bit_array(interface->spi_index, &reg , NULL, 1);
            zf_spi_transfer_8bit_array(interface->spi_index, data, NULL, len);
            zf_gpio_high(interface->spi_cs_pin);
        }
        else
        {
            zf_soft_iic_write_8bit_splicing_array(interface->soft_iic_index, addr, &reg, 1, data, len);
        }

    }while(0);

    return return_state;
}

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
uint8 imu_interface_read_8bit_register (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, uint8 *data)
{
    imu_interface_operation_state_enum  return_state    =   IMU_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = IMU_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(NULL == data)
        {
            return_state = IMU_INTERFACE_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(IMU_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = IMU_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        return_state = IMU_INTERFACE_OPERATION_DONE;
        if(IMU_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
            zf_spi_transfer_8bit_array(interface->spi_index, &reg , NULL , 1);
            zf_spi_transfer_8bit_array(interface->spi_index, NULL, data, 1);
            zf_gpio_high(interface->spi_cs_pin);
        }
        else
        {
            zf_soft_iic_transfer_8bit_array(interface->soft_iic_index, addr, &reg, 1, data, 1);
        }

    }while(0);

    return return_state;
}

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
uint8 imu_interface_read_8bit_registers (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, uint8 *data, uint32 len)
{
    imu_interface_operation_state_enum  return_state    =   IMU_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = IMU_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(NULL == data)
        {
            return_state = IMU_INTERFACE_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(IMU_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = IMU_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        return_state = IMU_INTERFACE_OPERATION_DONE;
        if(IMU_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
            zf_spi_transfer_8bit_array(interface->spi_index, &reg , NULL , 1);
            zf_spi_transfer_8bit_array(interface->spi_index, NULL, data, len);
            zf_gpio_high(interface->spi_cs_pin);
        }
        else
        {
            zf_soft_iic_transfer_8bit_array(interface->soft_iic_index, addr, &reg, 1, data, len);
        }

    }while(0);

    return return_state;
}

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
uint8 imu_interface_transfer_8bit_array (imu_interface_obj_struct *interface, uint8 addr, const uint8 *send_data, uint32 send_len, uint8 *receive_data, uint32 receive_len)
{
    imu_interface_operation_state_enum  return_state    =   IMU_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = IMU_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(IMU_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = IMU_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        return_state = IMU_INTERFACE_OPERATION_DONE;
        if(IMU_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_gpio_low(interface->spi_cs_pin);
            zf_spi_transfer_8bit_array(interface->spi_index, send_data, NULL, send_len);
            zf_spi_transfer_8bit_array(interface->spi_index, NULL, receive_data, receive_len);
            zf_gpio_high(interface->spi_cs_pin);
        }
        else
        {
            zf_soft_iic_transfer_8bit_array(interface->soft_iic_index, addr, send_data, send_len, receive_data, receive_len);
        }

    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU 接口注销初始化
// 参数说明     *interface          接口对象 IMUxx 系列通用的接口对象
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU_INTERFACE_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu_interface_deinit(interface);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_interface_deinit (imu_interface_obj_struct *interface)
{
    imu_interface_operation_state_enum  return_state    =   IMU_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = IMU_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(IMU_INTERFACE_TYPE_UNKNOW == interface->interface_type)
        {
            return_state = IMU_INTERFACE_ERROR_INTERFACE_NOT_INIT;
            break;
        }

        if(IMU_INTERFACE_TYPE_SPI == interface->interface_type)
        {
            zf_spi_deinit(interface->spi_index);
            zf_gpio_deinit(interface->spi_cs_pin);
        }
        else
        {
            zf_soft_iic_deinit(interface->soft_iic_index);
        }

        interface->interface_type   = IMU_INTERFACE_TYPE_UNKNOW ;
        interface->spi_index        = 0                         ;
        interface->spi_cs_pin       = IMU_INTERFACE_CS_PIN      ;

        return_state = IMU_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

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
uint8 imu_interface_spi_init (imu_interface_obj_struct *interface, zf_spi_index_enum spi_index, zf_spi_mode_enum mode, uint32 baudrate, zf_spi_sck_pin_enum sck_pin, zf_spi_mosi_pin_enum mosi_pin, zf_spi_miso_pin_enum miso_pin, zf_gpio_pin_enum cs_pin)
{
    imu_interface_operation_state_enum  return_state    =   IMU_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = IMU_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(IMU_INTERFACE_TYPE_UNKNOW != interface->interface_type)
        {
            return_state = IMU_INTERFACE_ERROR_INTERFACE_OCCUPIED;
            break;
        }

        if(zf_spi_init(spi_index, mode, baudrate, sck_pin, mosi_pin, miso_pin, SPI_CS_NULL))
        {
            return_state = IMU_INTERFACE_ERROR_INTERFACE_OCCUPIED;
            break;
        }
        zf_gpio_init(cs_pin, GPO_PUSH_PULL, GPIO_HIGH);

        interface->interface_type   = IMU_INTERFACE_TYPE_SPI;
        interface->spi_index        = spi_index             ;
        interface->spi_cs_pin       = cs_pin                ;

        return_state = IMU_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}

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
uint8 imu_interface_soft_iic_init (imu_interface_obj_struct *interface, zf_soft_iic_index_enum soft_iic_index, uint32 delay, zf_gpio_pin_enum scl_pin, zf_gpio_pin_enum sda_pin)
{
    imu_interface_operation_state_enum  return_state    =   IMU_INTERFACE_ERROR_UNKNOW_ERROR;

    do
    {
        if(NULL == interface)
        {
            return_state = IMU_INTERFACE_ERROR_OBJ_BUFFER_NULL;
            break;
        }
        if(IMU_INTERFACE_TYPE_UNKNOW != interface->interface_type)
        {
            return_state = IMU_INTERFACE_ERROR_INTERFACE_OCCUPIED;
            break;
        }

        if(zf_soft_iic_init(soft_iic_index, delay, scl_pin, sda_pin))
        {
            return_state = IMU_INTERFACE_ERROR_INTERFACE_OCCUPIED;
            break;
        }

        interface->interface_type   = IMU_INTERFACE_TYPE_SOFT_IIC   ;
        interface->soft_iic_index   = soft_iic_index                ;

        return_state = IMU_INTERFACE_OPERATION_DONE;
    }while(0);

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
