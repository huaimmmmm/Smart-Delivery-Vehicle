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
* 文件名称          zf_device_imu963ra
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

/*********************************************************************************************************************
* 接线定义          查看 zf_device_imu_interface.h 中接口资源定义
********************************************************************************************************************/

// 自身头文件
#include "zf_device_imu963ra.h"

// 此处定义 IMU963RA 驱动所需的内部定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 定义 IMU963RA 内部地址
#define IMU963RA_DEV_ADDR                           ( 0x6B )                    // 7bit 地址格式 SA0接地：0x6A SA0上拉：0x6B 模块默认上拉
#define IMU963RA_SPI_W                              ( 0x00 )
#define IMU963RA_SPI_R                              ( 0x80 )

#define IMU963RA_FUNC_CFG_ACCESS                    ( 0x01 )
#define IMU963RA_PIN_CTRL                           ( 0x02 )
#define IMU963RA_S4S_TPH_L                          ( 0x04 )
#define IMU963RA_S4S_TPH_H                          ( 0x05 )
#define IMU963RA_S4S_RR                             ( 0x06 )
#define IMU963RA_FIFO_CTRL1                         ( 0x07 )
#define IMU963RA_FIFO_CTRL2                         ( 0x08 )
#define IMU963RA_FIFO_CTRL3                         ( 0x09 )
#define IMU963RA_FIFO_CTRL4                         ( 0x0A )
#define IMU963RA_COUNTER_BDR_REG1                   ( 0x0B )
#define IMU963RA_COUNTER_BDR_REG2                   ( 0x0C )
#define IMU963RA_INT1_CTRL                          ( 0x0D )
#define IMU963RA_INT2_CTRL                          ( 0x0E )
#define IMU963RA_WHO_AM_I                           ( 0x0F )
#define IMU963RA_CTRL1_XL                           ( 0x10 )
#define IMU963RA_CTRL2_G                            ( 0x11 )
#define IMU963RA_CTRL3_C                            ( 0x12 )
#define IMU963RA_CTRL4_C                            ( 0x13 )
#define IMU963RA_CTRL5_C                            ( 0x14 )
#define IMU963RA_CTRL6_C                            ( 0x15 )
#define IMU963RA_CTRL7_G                            ( 0x16 )
#define IMU963RA_CTRL8_XL                           ( 0x17 )
#define IMU963RA_CTRL9_XL                           ( 0x18 )
#define IMU963RA_CTRL10_C                           ( 0x19 )
#define IMU963RA_ALL_INT_SRC                        ( 0x1A )
#define IMU963RA_WAKE_UP_SRC                        ( 0x1B )
#define IMU963RA_TAP_SRC                            ( 0x1C )
#define IMU963RA_D6D_SRC                            ( 0x1D )
#define IMU963RA_STATUS_REG                         ( 0x1E )
#define IMU963RA_OUT_TEMP_L                         ( 0x20 )
#define IMU963RA_OUT_TEMP_H                         ( 0x21 )
#define IMU963RA_OUTX_L_G                           ( 0x22 )
#define IMU963RA_OUTX_H_G                           ( 0x23 )
#define IMU963RA_OUTY_L_G                           ( 0x24 )
#define IMU963RA_OUTY_H_G                           ( 0x25 )
#define IMU963RA_OUTZ_L_G                           ( 0x26 )
#define IMU963RA_OUTZ_H_G                           ( 0x27 )
#define IMU963RA_OUTX_L_A                           ( 0x28 )
#define IMU963RA_OUTX_H_A                           ( 0x29 )
#define IMU963RA_OUTY_L_A                           ( 0x2A )
#define IMU963RA_OUTY_H_A                           ( 0x2B )
#define IMU963RA_OUTZ_L_A                           ( 0x2C )
#define IMU963RA_OUTZ_H_A                           ( 0x2D )
#define IMU963RA_EMB_FUNC_STATUS_MAINPAGE           ( 0x35 )
#define IMU963RA_FSM_STATUS_A_MAINPAGE              ( 0x36 )
#define IMU963RA_FSM_STATUS_B_MAINPAGE              ( 0x37 )
#define IMU963RA_STATUS_MASTER_MAINPAGE             ( 0x39 )
#define IMU963RA_FIFO_STATUS1                       ( 0x3A )
#define IMU963RA_FIFO_STATUS2                       ( 0x3B )
#define IMU963RA_TIMESTAMP0                         ( 0x40 )
#define IMU963RA_TIMESTAMP1                         ( 0x41 )
#define IMU963RA_TIMESTAMP2                         ( 0x42 )
#define IMU963RA_TIMESTAMP3                         ( 0x43 )
#define IMU963RA_TAP_CFG0                           ( 0x56 )
#define IMU963RA_TAP_CFG1                           ( 0x57 )
#define IMU963RA_TAP_CFG2                           ( 0x58 )
#define IMU963RA_TAP_THS_6D                         ( 0x59 )
#define IMU963RA_INT_DUR2                           ( 0x5A )
#define IMU963RA_WAKE_UP_THS                        ( 0x5B )
#define IMU963RA_WAKE_UP_DUR                        ( 0x5C )
#define IMU963RA_FREE_FALL                          ( 0x5D )
#define IMU963RA_MD1_CFG                            ( 0x5E )
#define IMU963RA_MD2_CFG                            ( 0x5F )
#define IMU963RA_S4S_ST_CMD_CODE                    ( 0x60 )
#define IMU963RA_S4S_DT_REG                         ( 0x61 )
#define IMU963RA_I3C_BUS_AVB                        ( 0x62 )
#define IMU963RA_INTERNAL_FREQ_FINE                 ( 0x63 )
#define IMU963RA_INT_OIS                            ( 0x6F )
#define IMU963RA_CTRL1_OIS                          ( 0x70 )
#define IMU963RA_CTRL2_OIS                          ( 0x71 )
#define IMU963RA_CTRL3_OIS                          ( 0x72 )
#define IMU963RA_X_OFS_USR                          ( 0x73 )
#define IMU963RA_Y_OFS_USR                          ( 0x74 )
#define IMU963RA_Z_OFS_USR                          ( 0x75 )
#define IMU963RA_FIFO_DATA_OUT_TAG                  ( 0x78 )
#define IMU963RA_FIFO_DATA_OUT_X_L                  ( 0x79 )
#define IMU963RA_FIFO_DATA_OUT_X_H                  ( 0x7A )
#define IMU963RA_FIFO_DATA_OUT_Y_L                  ( 0x7B )
#define IMU963RA_FIFO_DATA_OUT_Y_H                  ( 0x7C )
#define IMU963RA_FIFO_DATA_OUT_Z_L                  ( 0x7D )
#define IMU963RA_FIFO_DATA_OUT_Z_H                  ( 0x7E )

// 集线器功能相关寄存器 需要将FUNC_CFG_ACCESS的SHUB_REG_ACCESS位设置为1才能正确访问
#define IMU963RA_SENSOR_HUB_1                       ( 0x02 ) 
#define IMU963RA_SENSOR_HUB_2                       ( 0x03 ) 
#define IMU963RA_SENSOR_HUB_3                       ( 0x04 ) 
#define IMU963RA_SENSOR_HUB_4                       ( 0x05 ) 
#define IMU963RA_SENSOR_HUB_5                       ( 0x06 ) 
#define IMU963RA_SENSOR_HUB_6                       ( 0x07 ) 
#define IMU963RA_SENSOR_HUB_7                       ( 0x08 ) 
#define IMU963RA_SENSOR_HUB_8                       ( 0x09 ) 
#define IMU963RA_SENSOR_HUB_9                       ( 0x0A ) 
#define IMU963RA_SENSOR_HUB_10                      ( 0x0B ) 
#define IMU963RA_SENSOR_HUB_11                      ( 0x0C ) 
#define IMU963RA_SENSOR_HUB_12                      ( 0x0D ) 
#define IMU963RA_SENSOR_HUB_13                      ( 0x0E ) 
#define IMU963RA_SENSOR_HUB_14                      ( 0x0F ) 
#define IMU963RA_SENSOR_HUB_15                      ( 0x10 ) 
#define IMU963RA_SENSOR_HUB_16                      ( 0x11 ) 
#define IMU963RA_SENSOR_HUB_17                      ( 0x12 ) 
#define IMU963RA_SENSOR_HUB_18                      ( 0x13 ) 
#define IMU963RA_MASTER_CONFIG                      ( 0x14 ) 
#define IMU963RA_SLV0_ADD                           ( 0x15 ) 
#define IMU963RA_SLV0_SUBADD                        ( 0x16 ) 
#define IMU963RA_SLV0_CONFIG                        ( 0x17 ) 
#define IMU963RA_SLV1_ADD                           ( 0x18 ) 
#define IMU963RA_SLV1_SUBADD                        ( 0x19 ) 
#define IMU963RA_SLV1_CONFIG                        ( 0x1A ) 
#define IMU963RA_SLV2_ADD                           ( 0x1B ) 
#define IMU963RA_SLV2_SUBADD                        ( 0x1C ) 
#define IMU963RA_SLV2_CONFIG                        ( 0x1D ) 
#define IMU963RA_SLV3_ADD                           ( 0x1E ) 
#define IMU963RA_SLV3_SUBADD                        ( 0x1F ) 
#define IMU963RA_SLV3_CONFIG                        ( 0x20 ) 
#define IMU963RA_DATAWRITE_SLV0                     ( 0x21 ) 
#define IMU963RA_STATUS_MASTER                      ( 0x22 )

#define IMU963RA_MAG_ADDR                           ( 0x0D )                    // 7位IIC地址
#define IMU963RA_MAG_OUTX_L                         ( 0x00 )
#define IMU963RA_MAG_CONTROL1                       ( 0x09 )
#define IMU963RA_MAG_CONTROL2                       ( 0x0A )
#define IMU963RA_MAG_FBR                            ( 0x0B )
#define IMU963RA_MAG_CHIP_ID                        ( 0x0D )

static uint8 imu963ra_func_get_acc      (imu_interface_obj_struct *interface, imu963ra_measurement_data_struct *result);
static uint8 imu963ra_func_get_gyro     (imu_interface_obj_struct *interface, imu963ra_measurement_data_struct *result);
static uint8 imu963ra_func_get_mag      (imu_interface_obj_struct *interface, imu963ra_measurement_data_struct *result);
static uint8 imu963ra_func_set_config   (imu_interface_obj_struct *interface, imu_config_item_enum item);
static uint8 imu963ra_func_ckeck_id     (imu_interface_obj_struct *interface);
static uint8 imu963ra_func_init         (imu_interface_obj_struct *interface);

const imu_module_func_list imu963ra_module_func_list = 
{
    (imu_func_get_acc   )   imu963ra_func_get_acc   ,
    (imu_func_get_gyro  )   imu963ra_func_get_gyro  ,
    (imu_func_get_mag   )   imu963ra_func_get_mag   ,
    (imu_func_set_config)   imu963ra_func_set_config,
    (imu_func_ckeck_id  )   imu963ra_func_ckeck_id  ,
    (imu_func_init      )   imu963ra_func_init      ,
};
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU963RA 作为 IIC 主机向磁力计写数据
// 参数说明     *interface      IMU963RA 的通信接口对象指针
// 参数说明     addr            目标地址
// 参数说明     reg             目标寄存器
// 参数说明     data            数据
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_write_mag_register(interface, addr, reg, data);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_write_mag_register (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, uint8 data)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    uint8   temp_data       = 0;
    uint16  timeout_count   = 0;

    do
    {
        if(imu_interface_assert(NULL != interface))
        {
            // 此处如果断言报错 那么证明传入数据指针为 NULL 空指针
            // 不可以对空指针进行操作
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }

        addr = addr << 1;
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_CONFIG   , 0x00      );   // 从机0配置清除
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_ADD      , addr | 0  );   // 设置地磁计地址（注意这里需要设置8位的I2C地址） 0x2C
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_SUBADD   , reg       );   // 需要写入的寄存器地址
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_DATAWRITE_SLV0, data      );   // 需要写入的数据
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG , 0x4C      );   // 仅在第一个周期启用通讯 开启上拉 I2C主机使能

        return_state = IMU963RA_OPERATION_DONE;
        // 等待通讯成功
        do
        {
            imu_interface_read_8bit_register(
                interface, IMU963RA_DEV_ADDR,
                (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                    ? (IMU963RA_STATUS_MASTER | IMU963RA_SPI_R)
                    : IMU963RA_STATUS_MASTER,
                &temp_data);
            if(IMU963RA_TIMEOUT_COUNT < timeout_count ++)
            {
                return_state = IMU963RA_ERROR_MAG_WRITE_RIGESTER_ERROR;
                break;
            }
            imu_interface_delay_ms(1);
        }while(!(0x80 & temp_data));
    }while(0);

    return return_state;
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU963RA 作为 IIC 主机向磁力计读数据
// 参数说明     *interface      IMU963RA 的通信接口对象指针
// 参数说明     addr            目标地址
// 参数说明     reg             目标寄存器
// 参数说明     *data           数据指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_read_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CHIP_ID);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_read_mag_register (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, uint8 *data)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    uint8   temp_data       = 0;
    uint16  timeout_count   = 0;

    do
    {
        addr = addr << 1;
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_ADD,        addr | 1  );  // 设置地磁计地址（注意这里需要设置8位的I2C地址） 0x2C
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_SUBADD,     reg       );  // 需要读取的寄存器地址
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_CONFIG,     0x01      );  
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG,   0x4C      );  // 仅在第一个周期启用通讯 开启上拉 I2C主机使能

        return_state = IMU963RA_OPERATION_DONE;
        // 等待通讯成功
        do
        {
            imu_interface_read_8bit_register(
                interface, IMU963RA_DEV_ADDR,
                (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                    ? (IMU963RA_STATUS_MASTER | IMU963RA_SPI_R)
                    : IMU963RA_STATUS_MASTER,
                &temp_data);
            if(IMU963RA_TIMEOUT_COUNT < timeout_count ++)
            {
                return_state = IMU963RA_ERROR_MAG_WRITE_RIGESTER_ERROR;
                break;
            }
            imu_interface_delay_ms(1);
        }while(!(0x01 & temp_data));

        imu_interface_read_8bit_register(
            interface, IMU963RA_DEV_ADDR, 
            (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                ? (IMU963RA_SENSOR_HUB_1 | IMU963RA_SPI_R)
                : IMU963RA_SENSOR_HUB_1,
            data);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 加速度计数据 框架接口
// 参数说明     *interface          IMU963RA 的通信接口对象指针
// 参数说明     *measurement_data   IMU963RA 的测量数据缓冲指针
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_func_get_acc(interface, data_buffer);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_func_get_acc (imu_interface_obj_struct *interface, imu963ra_measurement_data_struct *measurement_data)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != measurement_data))
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }
        uint8 data[6];
        imu_interface_read_8bit_registers(
            interface, IMU963RA_DEV_ADDR, 
            (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                ?   (IMU963RA_OUTX_L_A | IMU963RA_SPI_R)
                :   IMU963RA_OUTX_L_A,
            data, 6);
        measurement_data->x = (int16)(((uint16)data[1] << 8 | data[0]));
        measurement_data->y = (int16)(((uint16)data[3] << 8 | data[2]));
        measurement_data->z = (int16)(((uint16)data[5] << 8 | data[4]));

        return_state = IMU963RA_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 陀螺仪数据 框架接口
// 参数说明     *interface          IMU963RA 的通信接口对象指针
// 参数说明     *measurement_data   IMU963RA 的测量数据缓冲指针
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_func_get_gyro(interface, data_buffer);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_func_get_gyro (imu_interface_obj_struct *interface, imu963ra_measurement_data_struct *measurement_data)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != measurement_data))
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }
        uint8 data[6];
        imu_interface_read_8bit_registers(
            interface, IMU963RA_DEV_ADDR, 
            (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                ?   (IMU963RA_OUTX_L_G | IMU963RA_SPI_R)
                :   IMU963RA_OUTX_L_G,
            data, 6);
        measurement_data->x = (int16)(((uint16)data[1] << 8 | data[0]));
        measurement_data->y = (int16)(((uint16)data[3] << 8 | data[2]));
        measurement_data->z = (int16)(((uint16)data[5] << 8 | data[4]));

        return_state = IMU963RA_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 地磁计数据 框架接口
// 参数说明     *interface          IMU963RA 的通信接口对象指针
// 参数说明     *measurement_data   IMU963RA 的测量数据缓冲指针
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_func_get_mag(interface, data_buffer);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_func_get_mag (imu_interface_obj_struct *interface, imu963ra_measurement_data_struct *measurement_data)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    uint8   temp_data       = 0;

    do
    {
        if(imu_interface_assert(NULL != measurement_data))
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }
        uint8 data[6];
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x40);
        imu_interface_read_8bit_register(
            interface, IMU963RA_DEV_ADDR, 
            (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                ? (IMU963RA_STATUS_MASTER | IMU963RA_SPI_R)
                : IMU963RA_STATUS_MASTER,
            &temp_data);
        if(0x01 & temp_data)
        {
            imu_interface_read_8bit_registers(
                interface, IMU963RA_DEV_ADDR, 
                (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                    ?   (IMU963RA_SENSOR_HUB_1 | IMU963RA_SPI_R)
                    :   IMU963RA_SENSOR_HUB_1,
                data, 6);
            measurement_data->x = (int16)(((uint16)data[1] << 8 | data[0]));
            measurement_data->y = (int16)(((uint16)data[3] << 8 | data[2]));
            measurement_data->z = (int16)(((uint16)data[5] << 8 | data[4]));
        }
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);

        return_state = IMU963RA_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU963RA 配置模块工作参数 框架接口
// 参数说明     *interface      IMU963RA 的通信接口对象指针
// 参数说明     item            IMU963RA 配置参照 zf_device_imu_interface.h 中 imu_config_item_enum 枚举体定义
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_func_set_config(interface, interface);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_func_set_config (imu_interface_obj_struct *interface, imu_config_item_enum item)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    uint8   temp_data       = 0;

    do
    {
        switch((item >> IMU_CONFIG_TYPE_OFFSET) & IMU_CONFIG_TYPE_MASK)
        {
            case 0x00:
            {
                return_state = IMU963RA_OPERATION_DONE;
                if(imu_interface_assert(
                            IMU_ACC_OUTPUT_RATE_4_DIV      <= (item & 0x0F)
                        &&  IMU_ACC_OUTPUT_RATE_128_MUL    >= (item & 0x0F)))
                {
                    return_state = IMU963RA_ERROR_SET_ACC_OUTPUT_RATE_ERROR;
                    break;
                }
                imu_interface_read_8bit_register(
                    interface, IMU963RA_DEV_ADDR, 
                    (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                        ? (IMU963RA_CTRL1_XL | IMU963RA_SPI_R)
                        : IMU963RA_CTRL1_XL,
                    &temp_data);
                temp_data = ((temp_data & ~0xF0) |(((item & 0x0F) - 5) << 4));
                imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL1_XL, temp_data);
            }break;
            case 0x01:
            {
                return_state = IMU963RA_OPERATION_DONE;
                if(imu_interface_assert(
                            IMU_ACC_RANGE_SGN_2G <= (item & 0x0F)
                        &&  IMU_ACC_RANGE_SGN_16G >= (item & 0x0F)))
                {
                    return_state = IMU963RA_ERROR_SET_ACC_RANGE_ERROR;
                    break;
                }
                imu_interface_read_8bit_register(
                    interface, IMU963RA_DEV_ADDR, 
                    (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                        ? (IMU963RA_CTRL1_XL | IMU963RA_SPI_R)
                        : IMU963RA_CTRL1_XL,
                    &temp_data);
                const uint8 acc_range_list[6] = {0x00, 0x00, 0x00, 0x08, 0x0C, 0x04};
                temp_data = ((temp_data & ~0x0C) | acc_range_list[(item & 0x0F)]);
                imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL1_XL, temp_data);
            }break;
            case 0x02:
            {
                return_state = IMU963RA_OPERATION_DONE;
                if(imu_interface_assert(
                            IMU_GYRO_OUTPUT_RATE_4_DIV     <= (item & 0x0F)
                        &&  IMU_GYRO_OUTPUT_RATE_128_MUL   >= (item & 0x0F)))
                {
                    return_state = IMU963RA_ERROR_SET_GYRO_OUTPUT_RATE_ERROR;
                    break;
                }
                imu_interface_read_8bit_register(
                    interface, IMU963RA_DEV_ADDR, 
                    (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                        ? (IMU963RA_CTRL2_G | IMU963RA_SPI_R)
                        : IMU963RA_CTRL2_G,
                    &temp_data);
                temp_data = ((temp_data & ~0xF0) |(((item & 0x0F) - 5) << 4));
                imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL2_G, temp_data);
            }break;
            case 0x03:
            {
                return_state = IMU963RA_OPERATION_DONE;
                if(imu_interface_assert(
                            IMU_GYRO_RANGE_SGN_125DPS  <= (item & 0x0F)
                        &&  IMU_GYRO_RANGE_SGN_4000DPS >= (item & 0x0F)))
                {
                    return_state = IMU963RA_ERROR_SET_GYRO_RANGE_ERROR;
                    break;
                }
                imu_interface_read_8bit_register(
                    interface, IMU963RA_DEV_ADDR, 
                    (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                        ? (IMU963RA_CTRL2_G | IMU963RA_SPI_R)
                        : IMU963RA_CTRL2_G,
                    &temp_data);
                const uint8 gyro_range_list[8] = {0x00, 0x00, 0x02, 0x00, 0x04, 0x08, 0x0C, 0x01};
                temp_data = ((temp_data & ~0x0F) | gyro_range_list[(item & 0x0F)]);
                imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL2_G, temp_data);
            }break;
            case 0x04:
            {
                return_state = IMU963RA_OPERATION_DONE;
                if(imu_interface_assert(
                        (   IMU_MAG_OUTPUT_RATE_4_DIV   <= (item & 0x0F)
                        &&  IMU_MAG_OUTPUT_RATE_4_MUL   >= (item & 0x0F))
                    &&  (   IMU_MAG_OUTPUT_RATE_2_DIV   != (item & 0x0F))))
                {
                    return_state = IMU963RA_ERROR_SET_MAG_OUTPUT_RATE_ERROR;
                    break;
                }
                imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x40);
                do
                {
                    imu963ra_read_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, &temp_data);
                    temp_data = (temp_data & ~0x0C)
                        | (((item & 0x0F) == IMU_MAG_OUTPUT_RATE_4_DIV) ? (0) : (((item & 0x0F) - 7) << 2));
                    imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, temp_data | 0x01);

                    imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_ADD, (IMU963RA_MAG_ADDR << 1) | 1); // 设置地磁计地址（注意这里需要设置8位的I2C地址） 0x2C
                    imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_SUBADD, IMU963RA_MAG_OUTX_L);       // 需要读取的寄存器地址
                    imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_CONFIG, 0x06);    
                    imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG, 0x6C);                    // 仅在第一个周期启用通讯 开启上拉 I2C主机使能
                }while(0);
                imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
            }break;
            case 0x05:
            {
                return_state = IMU963RA_OPERATION_DONE;
                if(imu_interface_assert(
                            IMU_MAG_RANGE_2G    <= (item & 0x0F)
                        &&  IMU_MAG_RANGE_8G    >= (item & 0x0F)))
                {
                    return_state = IMU963RA_ERROR_SET_MAG_RANGE_ERROR;
                    break;
                }
                imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x40);
                do
                {
                    imu963ra_read_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, &temp_data);
                    temp_data = (temp_data & ~0x30) | (((item & 0x0F) - 2) << 4);
                    imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, temp_data | 0x01);

                    imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_ADD, (IMU963RA_MAG_ADDR << 1) | 1); // 设置地磁计地址（注意这里需要设置8位的I2C地址） 0x2C
                    imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_SUBADD, IMU963RA_MAG_OUTX_L);       // 需要读取的寄存器地址
                    imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_CONFIG, 0x06);    
                    imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG, 0x6C);                    // 仅在第一个周期启用通讯 开启上拉 I2C主机使能
                }while(0);
                imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
            }break;
            default:
            {
                if(imu_interface_assert(0))
                {
                    return_state = IMU963RA_ERROR_SET_CONFIG_ERROR;
                    break;
                }
            }break;
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU963RA 六轴 检测 ID 是否匹配 框架接口
// 参数说明     *interface      IMU963RA 的通信接口对象指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_func_ckeck_id(interface);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_func_ckeck_id (imu_interface_obj_struct *interface)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    uint8   temp_data       = 0;
    uint16  timeout_count   = 0;

    do
    {
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL3_C, 0x01);               // 复位设备
        imu_interface_delay_ms(2);                             
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
        do
        {
            if(IMU963RA_TIMEOUT_COUNT < timeout_count ++)
            {
                return_state = IMU963RA_ERROR_ID_CHECK_ERROR;
                break;
            }
            imu_interface_read_8bit_register(
                interface, IMU963RA_DEV_ADDR, 
                (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                    ? (IMU963RA_WHO_AM_I | IMU963RA_SPI_R)
                    : IMU963RA_WHO_AM_I,
                &temp_data);
            imu_interface_delay_ms(10);
        }while(0x6B != temp_data);
        if(IMU963RA_ERROR_ID_CHECK_ERROR == return_state)
        {
            break;
        }

        // 必须要给 加速度计 或 地磁计 任意一个设置输出速率 否则六轴处于 power-down 模式不能跟地磁计通信
        // imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL1_XL, 0x10);
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL2_G,  0x10);

        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x40);
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG, 0x80);         // 复位I2C主机
        imu_interface_delay_ms(2);
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG, 0x00);         // 清除复位标志
        imu_interface_delay_ms(2);
        imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL2, 0x80);// 复位连接的外设
        imu_interface_delay_ms(2);
        imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL2, 0x00);
        imu_interface_delay_ms(2);

        temp_data = 0;
        imu963ra_read_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CHIP_ID, &temp_data);
        if(0xff != temp_data)
        {
            return_state = IMU963RA_ERROR_MAG_ID_CHECK_ERROR;
        }
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);

        return_state = IMU963RA_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 IMU963RA 框架接口
// 参数说明     *interface      IMU963RA 的通信接口对象指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_func_init(interface);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_func_init (imu_interface_obj_struct *interface)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    do
    {
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_INT1_CTRL, 0x03);    // 开启陀螺仪 加速度数据就绪中断

        uint8 register_temp = 0;

        if(imu_interface_assert(
                    IMU_ACC_OUTPUT_RATE_4_DIV   <= IMU_ACC_OUTPUT_RATE_DEFAULT
                &&  IMU_ACC_OUTPUT_RATE_128_MUL >= IMU_ACC_OUTPUT_RATE_DEFAULT))
        {
            return_state = IMU963RA_ERROR_SET_ACC_OUTPUT_RATE_ERROR;
            break;
        }
        register_temp = ((IMU_ACC_OUTPUT_RATE_DEFAULT - 5) << 4);
        if(imu_interface_assert(
                    IMU_ACC_RANGE_SGN_2G    <= IMU_ACC_RANGE_DEFAULT
                &&  IMU_ACC_RANGE_SGN_16G   >= IMU_ACC_RANGE_DEFAULT))
        {
            return_state = IMU963RA_ERROR_SET_ACC_RANGE_ERROR;
            break;
        }
        const uint8 acc_range_list[6] = {0x00, 0x00, 0x00, 0x08, 0x0C, 0x04};
        register_temp |= acc_range_list[IMU_ACC_RANGE_DEFAULT];
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL1_XL, register_temp);

        if(imu_interface_assert(
                    IMU_GYRO_OUTPUT_RATE_4_DIV      <= IMU_GYRO_OUTPUT_RATE_DEFAULT
                &&  IMU_GYRO_OUTPUT_RATE_128_MUL    >= IMU_GYRO_OUTPUT_RATE_DEFAULT))
        {
            return_state = IMU963RA_ERROR_SET_GYRO_OUTPUT_RATE_ERROR;
            break;
        }
        register_temp = ((IMU_GYRO_OUTPUT_RATE_DEFAULT - 5) << 4);
        if(imu_interface_assert(
                    IMU_GYRO_RANGE_SGN_125DPS   <= IMU_GYRO_RANGE_DEFAULT
                &&  IMU_GYRO_RANGE_SGN_4000DPS  >= IMU_GYRO_RANGE_DEFAULT))
        {
            return_state = IMU963RA_ERROR_SET_GYRO_RANGE_ERROR;
            break;
        }
        const uint8 gyro_range_list[8] = {0x00, 0x00, 0x02, 0x00, 0x04, 0x08, 0x0C, 0x01};
        register_temp |= gyro_range_list[IMU_GYRO_RANGE_DEFAULT];
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL2_G, register_temp);

        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL3_C, 0x44);               // 使能陀螺仪数字低通滤波器
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL4_C, 0x02);               // 使能数字低通滤波器
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL5_C, 0x00);               // 加速度计与陀螺仪四舍五入
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL6_C, 0x00);               // 开启加速度计高性能模式 陀螺仪低通滤波 133hz
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL7_G, 0x00);               // 开启陀螺仪高性能模式 关闭高通滤波
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_CTRL9_XL, 0x01);              // 关闭I3C接口

        return_state = IMU963RA_OPERATION_DONE;
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x40);
        do
        {
            if(imu_interface_assert(
                    (   IMU_MAG_OUTPUT_RATE_4_DIV   <= IMU_MAG_OUTPUT_RATE_DEFAULT
                    &&  IMU_MAG_OUTPUT_RATE_4_MUL   >= IMU_MAG_OUTPUT_RATE_DEFAULT)
                &&  (   IMU_MAG_OUTPUT_RATE_2_DIV   != IMU_MAG_OUTPUT_RATE_DEFAULT)))
            {
                return_state = IMU963RA_ERROR_SET_MAG_OUTPUT_RATE_ERROR;
                break;
            }
            register_temp =
                    (IMU_MAG_OUTPUT_RATE_DEFAULT == IMU_MAG_OUTPUT_RATE_4_DIV)
                ?   (0)
                :   (IMU_MAG_OUTPUT_RATE_DEFAULT - 7);
            register_temp = (register_temp << 2);
            if(imu_interface_assert(
                        IMU_MAG_RANGE_2G    <= IMU_MAG_RANGE_DEFAULT
                    &&  IMU_MAG_RANGE_8G    >= IMU_MAG_RANGE_DEFAULT))
            {
                return_state = IMU963RA_ERROR_SET_MAG_RANGE_ERROR;
                break;
            }
            register_temp |= ((IMU_MAG_RANGE_DEFAULT - 2) << 4);
            imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, register_temp | 0x01);
            imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_FBR, 0x01);

            imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_ADD, (IMU963RA_MAG_ADDR << 1) | 1); // 设置地磁计地址（注意这里需要设置8位的I2C地址） 0x2C
            imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_SUBADD, IMU963RA_MAG_OUTX_L);       // 需要读取的寄存器地址
            imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_SLV0_CONFIG, 0x06);    
            imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG, 0x6C);                    // 仅在第一个周期启用通讯 开启上拉 I2C主机使能
        }while(0);
        imu_interface_write_8bit_register(interface, IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 加速度计物理量数据
// 参数说明     *result         IMU963RA 的测量数据缓冲指针
// 返回参数     range           IMU963RA 加速度量程 详见 zf_device_imu_interface.h 中 imu_acc_range_enum 定义
// 参数说明     *physical_data  IMU963RA 的物理量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_get_physical_acc(result, range, physical_data);
// 备注信息     根据量程进行物理量换算
//              加速度计量程 ±2  g   获取到的加速度计数据除以 (1000.0 / 0.061) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//              加速度计量程 ±4  g   获取到的加速度计数据除以 (1000.0 / 0.122) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//              加速度计量程 ±8  g   获取到的加速度计数据除以 (1000.0 / 0.244) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//              加速度计量程 ±16 g   获取到的加速度计数据除以 (1000.0 / 0.488) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//-------------------------------------------------------------------------------------------------------------------
uint8 imu963ra_get_physical_acc (imu963ra_measurement_data_struct *result, imu_acc_range_enum range,imu963ra_physical_data_struct *physical_data)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != result))
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(imu_interface_assert(
                    IMU_ACC_RANGE_SGN_2G   <= range
                &&  IMU_ACC_RANGE_SGN_16G  >= range))
        {
            return_state = IMU963RA_ERROR_SET_ACC_RANGE_ERROR;
            break;
        }
        if(imu_interface_assert(NULL != physical_data))
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }
        const float transition_factor[4] = {1000.0 / 0.061, 1000.0 / 0.122, 1000.0 / 0.244, 1000.0 / 0.488};
        physical_data->x  = (float)result->x / transition_factor[range - IMU_ACC_RANGE_SGN_2G];
        physical_data->y  = (float)result->y / transition_factor[range - IMU_ACC_RANGE_SGN_2G];
        physical_data->z  = (float)result->z / transition_factor[range - IMU_ACC_RANGE_SGN_2G];

        return_state = IMU963RA_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 陀螺仪物理量数据
// 参数说明     *result         IMU963RA 的测量数据缓冲指针
// 返回参数     range           IMU963RA 加速度量程 详见 zf_device_imu_interface.h 中 imu_gyro_range_enum 定义
// 参数说明     *physical_data  IMU963RA 的物理量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_get_physical_gyro(result, range, physical_data);
// 备注信息     根据量程进行物理量换算
//              陀螺仪量程 ±125  dps    获取到的陀螺仪数据除以 (1000.0 / 4.375)  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±250  dps    获取到的陀螺仪数据除以 (1000.0 / 8.75 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±500  dps    获取到的陀螺仪数据除以 (1000.0 / 17.5 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±1000 dps    获取到的陀螺仪数据除以 (1000.0 / 35.0 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±2000 dps    获取到的陀螺仪数据除以 (1000.0 / 70.0 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±4000 dps    获取到的陀螺仪数据除以 (1000.0 / 140.0)  可以转化为带物理单位的数据 单位为 °/s
//-------------------------------------------------------------------------------------------------------------------
uint8 imu963ra_get_physical_gyro (imu963ra_measurement_data_struct *result, imu_gyro_range_enum range,imu963ra_physical_data_struct *physical_data)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != result))
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(imu_interface_assert(
                    IMU_GYRO_RANGE_SGN_125DPS  <= range
                &&  IMU_GYRO_RANGE_SGN_4000DPS >= range))
        {
            return_state = IMU963RA_ERROR_SET_GYRO_RANGE_ERROR;
            break;
        }
        if(imu_interface_assert(NULL != physical_data))
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }
        const float transition_factor[6] = 
        {
            1000.0 / 4.375, 1000.0 / 8.75,  1000.0 / 17.5,
            1000.0 / 35.0,  1000.0 / 70.0,  1000.0 / 140.0,
        };
        physical_data->x  = (float)result->x / transition_factor[range - IMU_GYRO_RANGE_SGN_125DPS];
        physical_data->y  = (float)result->y / transition_factor[range - IMU_GYRO_RANGE_SGN_125DPS];
        physical_data->z  = (float)result->z / transition_factor[range - IMU_GYRO_RANGE_SGN_125DPS];

        return_state = IMU963RA_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 地磁计物理量数据
// 参数说明     *result         IMU963RA 的测量数据缓冲指针
// 返回参数     range           IMU963RA 加速度量程 详见 zf_device_imu_interface.h 中 imu_mag_range_enum 定义
// 参数说明     *physical_data  IMU963RA 的物理量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_get_physical_mag(result, range, physical_data);
// 备注信息     根据量程进行物理量换算
//              地磁计量程 2G   获取到的地磁计数据除以 12000 可以转化为带物理单位的数据 单位为 G(高斯)
//              地磁计量程 8G   获取到的地磁计数据除以  3000 可以转化为带物理单位的数据 单位为 G(高斯)
//-------------------------------------------------------------------------------------------------------------------
uint8 imu963ra_get_physical_mag (imu963ra_measurement_data_struct *result, imu_mag_range_enum range,imu963ra_physical_data_struct *physical_data)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != result))
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(imu_interface_assert(
                    IMU_MAG_RANGE_2G   <= range
                &&  IMU_MAG_RANGE_8G   >= range))
        {
            return_state = IMU963RA_ERROR_SET_MAG_RANGE_ERROR;
            break;
        }
        if(imu_interface_assert(NULL != physical_data))
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }
        const float transition_factor[2] = {12000, 3000};
        physical_data->x  = (float)result->x / transition_factor[range - IMU_MAG_RANGE_2G];
        physical_data->y  = (float)result->y / transition_factor[range - IMU_MAG_RANGE_2G];
        physical_data->z  = (float)result->z / transition_factor[range - IMU_MAG_RANGE_2G];

        return_state = IMU963RA_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 加速度计数据
// 参数说明     *result         IMU963RA 的测量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_get_acc(result);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu963ra_get_acc (imu963ra_measurement_data_struct *result)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != result))                                // 检查数据缓冲是否有效
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }

        return_state = imu963ra_func_get_acc(&imu_interface_default_obj, result);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 陀螺仪数据
// 参数说明     *result         IMU963RA 的测量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_get_gyro(result);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu963ra_get_gyro (imu963ra_measurement_data_struct *result)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != result))                                // 检查数据缓冲是否有效
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }

        return_state = imu963ra_func_get_gyro(&imu_interface_default_obj, result);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 地磁计数据
// 参数说明     *result         IMU963RA 的测量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_get_mag(result);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu963ra_get_mag (imu963ra_measurement_data_struct *result)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != result))                                // 检查数据缓冲是否有效
        {
            return_state = IMU963RA_ERROR_DATA_BUFFER_NULL;
            break;
        }

        return_state = imu963ra_func_get_mag(&imu_interface_default_obj, result);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU963RA 配置模块工作参数
// 参数说明     item            IMU963RA 配置参照 zf_device_imu_interface.h 中 imu_config_item_enum 枚举体定义
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_set_config(item);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 imu963ra_set_config (imu_config_item_enum item)
{
    return imu963ra_func_set_config(&imu_interface_default_obj, item);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 IMU963RA
// 参数说明     void
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU963RA_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu963ra_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 imu963ra_init (void)
{
    imu963ra_operation_state_enum   return_state = IMU963RA_ERROR_UNKNOW_ERROR;

    uint8 init_step = 0x00;

    do
    {
        if(imu_interface_assert(!imu_interface_spi_default_init(&imu_interface_default_obj, SPI_MODE0)))
        {
            return_state = IMU963RA_ERROR_INTERFACE_OCCUPIED;
            break;
        }
        init_step |= 0x01;

        imu_interface_delay_ms(20);                                                 // 等待设备上电成功

        if(imu963ra_func_ckeck_id(&imu_interface_default_obj))
        {
            // 如果程序在输出了断言信息 并且提示出错位置在这里
            // 那么就是 IMU963RA 自检出错并超时退出了
            // 检查一下接线有没有问题 如果没问题可能就是坏了
            return_state = IMU963RA_ERROR_SELF_CHECK_ERROR;
            break;
        }

        return_state = imu963ra_func_init(&imu_interface_default_obj);
    }while(0);

    if(return_state && (init_step & 0x01))
    {
        imu_interface_deinit(&imu_interface_default_obj);                       // 那么就需要将接口注销初始化
    }

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
