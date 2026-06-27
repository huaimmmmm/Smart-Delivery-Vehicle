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

// 自身头文件
#include "zf_device_imu660rb.h"

// 此处定义 IMU660RB 驱动所需的内部定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 定义 IMU660RB 内部地址
#define IMU660RB_DEV_ADDR           (0x6B)                                      // SA0接地：0x68 SA0上拉：0x69 模块默认上拉
#define IMU660RB_SPI_W              (0x00)
#define IMU660RB_SPI_R              (0x80)
    
#define IMU660RB_CHIP_ID            (0x0F)

#define IMU660RB_INT1_CTRL          (0x0D)
#define IMU660RB_CTRL1_XL           (0x10)
#define IMU660RB_CTRL2_G            (0x11)
#define IMU660RB_CTRL3_C            (0x12)
#define IMU660RB_CTRL4_C            (0x13)
#define IMU660RB_CTRL5_C            (0x14)
#define IMU660RB_CTRL6_C            (0x15)
#define IMU660RB_CTRL7_G            (0x16)
#define IMU660RB_CTRL9_XL           (0x18)

#define IMU660RB_ACC_ADDRESS        (0x28)
#define IMU660RB_GYRO_ADDRESS       (0x22)

#define IMU660RB_ACC_SAMPLE         (0x3C)                      // 加速度计量程
// 设置为:0x30 加速度量程为:±2G      获取到的加速度计数据 除以16393，可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x38 加速度量程为:±4G      获取到的加速度计数据 除以8197， 可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x3C 加速度量程为:±8G      获取到的加速度计数据 除以4098， 可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x34 加速度量程为:±16G     获取到的加速度计数据 除以2049， 可以转化为带物理单位的数据，单位：g(m/s^2)

#define IMU660RB_GYR_SAMPLE         (0x5C)                      // 陀螺仪量程
// 设置为:0x52 陀螺仪量程为:±125dps  获取到的陀螺仪数据除以228.6，   可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x50 陀螺仪量程为:±250dps  获取到的陀螺仪数据除以114.3，   可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x54 陀螺仪量程为:±500dps  获取到的陀螺仪数据除以57.1，    可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x58 陀螺仪量程为:±1000dps 获取到的陀螺仪数据除以28.6，    可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x5C 陀螺仪量程为:±2000dps 获取到的陀螺仪数据除以14.3，    可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x51 陀螺仪量程为:±4000dps 获取到的陀螺仪数据除以7.1，     可以转化为带物理单位的数据，单位为：°/s

static uint8 imu660rb_func_get_acc      (imu_interface_obj_struct *interface, imu660rb_measurement_data_struct *result);
static uint8 imu660rb_func_get_gyro     (imu_interface_obj_struct *interface, imu660rb_measurement_data_struct *result);
static uint8 imu660rb_func_set_config   (imu_interface_obj_struct *interface, imu_config_item_enum item);
static uint8 imu660rb_func_ckeck_id     (imu_interface_obj_struct *interface);
static uint8 imu660rb_func_init         (imu_interface_obj_struct *interface);

const imu_module_func_list imu660rb_module_func_list = 
{
    (imu_func_get_acc   )   imu660rb_func_get_acc   ,
    (imu_func_get_gyro  )   imu660rb_func_get_gyro  ,
    NULL,
    (imu_func_set_config)   imu660rb_func_set_config,
    (imu_func_ckeck_id  )   imu660rb_func_ckeck_id  ,
    (imu_func_init      )   imu660rb_func_init      ,
};
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU660RB 加速度计数据 框架接口
// 参数说明     *interface          IMU660RB 的通信接口对象指针
// 参数说明     *measurement_data   IMU660RB 的测量数据缓冲指针
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_func_get_acc(interface, data_buffer);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660rb_func_get_acc (imu_interface_obj_struct *interface, imu660rb_measurement_data_struct *measurement_data)
{
    imu660rb_operation_state_enum   return_state = IMU660RB_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != measurement_data))
        {
            return_state = IMU660RB_ERROR_DATA_BUFFER_NULL;
            break;
        }
        uint8 data[6];
        imu_interface_read_8bit_registers(
            interface, IMU660RB_DEV_ADDR, 
            (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                ?   (IMU660RB_ACC_ADDRESS | IMU660RB_SPI_R)
                :   IMU660RB_ACC_ADDRESS,
            data, 6);
        measurement_data->x = (int16)(((uint16)data[1] << 8 | data[0]));
        measurement_data->y = (int16)(((uint16)data[3] << 8 | data[2]));
        measurement_data->z = (int16)(((uint16)data[5] << 8 | data[4]));

        return_state = IMU660RB_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU660RB 陀螺仪数据 框架接口
// 参数说明     *interface          IMU660RB 的通信接口对象指针
// 参数说明     *measurement_data   IMU660RB 的测量数据缓冲指针
// 返回参数     uint8               操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_func_get_gyro(interface, data_buffer);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660rb_func_get_gyro (imu_interface_obj_struct *interface, imu660rb_measurement_data_struct *measurement_data)
{
    imu660rb_operation_state_enum   return_state = IMU660RB_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != measurement_data))
        {
            return_state = IMU660RB_ERROR_DATA_BUFFER_NULL;
            break;
        }
        uint8 data[6];
        imu_interface_read_8bit_registers(
            interface, IMU660RB_DEV_ADDR, 
            (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                ?   (IMU660RB_GYRO_ADDRESS | IMU660RB_SPI_R)
                :   IMU660RB_GYRO_ADDRESS,
            data, 6);
        measurement_data->x = (int16)(((uint16)data[1] << 8 | data[0]));
        measurement_data->y = (int16)(((uint16)data[3] << 8 | data[2]));
        measurement_data->z = (int16)(((uint16)data[5] << 8 | data[4]));

        return_state = IMU660RB_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU660RB 配置模块工作参数 框架接口
// 参数说明     *interface      IMU660RB 的通信接口对象指针
// 参数说明     item            IMU660RB 配置参照 zf_device_imu_interface.h 中 imu_config_item_enum 枚举体定义
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_func_set_config(interface, interface);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660rb_func_set_config (imu_interface_obj_struct *interface, imu_config_item_enum item)
{
    imu660rb_operation_state_enum   return_state = IMU660RB_ERROR_UNKNOW_ERROR;

    uint8   temp_data       = 0;

    do
    {
        switch((item >> IMU_CONFIG_TYPE_OFFSET) & IMU_CONFIG_TYPE_MASK)
        {
            case 0x00:
            {
                return_state = IMU660RB_OPERATION_DONE;
                if(imu_interface_assert(
                            IMU_ACC_OUTPUT_RATE_4_DIV      <= (item & 0x0F)
                        &&  IMU_ACC_OUTPUT_RATE_128_MUL    >= (item & 0x0F)))
                {
                    return_state = IMU660RB_ERROR_SET_ACC_OUTPUT_RATE_ERROR;
                    break;
                }
                imu_interface_read_8bit_register(
                    interface, IMU660RB_DEV_ADDR, 
                    (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                        ? (IMU660RB_CTRL1_XL | IMU660RB_SPI_R)
                        : IMU660RB_CTRL1_XL,
                    &temp_data);
                temp_data = ((temp_data & ~0xF0) |(((item & 0x0F) - 5) << 4));
                imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL1_XL, temp_data);
            }break;
            case 0x01:
            {
                return_state = IMU660RB_OPERATION_DONE;
                if(imu_interface_assert(
                            IMU_ACC_RANGE_SGN_2G <= (item & 0x0F)
                        &&  IMU_ACC_RANGE_SGN_16G >= (item & 0x0F)))
                {
                    return_state = IMU660RB_ERROR_SET_ACC_RANGE_ERROR;
                    break;
                }
                imu_interface_read_8bit_register(
                    interface, IMU660RB_DEV_ADDR, 
                    (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                        ? (IMU660RB_CTRL1_XL | IMU660RB_SPI_R)
                        : IMU660RB_CTRL1_XL,
                    &temp_data);
                const uint8 acc_range_list[6] = {0x00, 0x00, 0x00, 0x08, 0x0C, 0x04};
                temp_data = ((temp_data & ~0x0C) | acc_range_list[(item & 0x0F)]);
                imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL1_XL, temp_data);
            }break;
            case 0x02:
            {
                return_state = IMU660RB_OPERATION_DONE;
                if(imu_interface_assert(
                            IMU_GYRO_OUTPUT_RATE_4_DIV     <= (item & 0x0F)
                        &&  IMU_GYRO_OUTPUT_RATE_128_MUL   >= (item & 0x0F)))
                {
                    return_state = IMU660RB_ERROR_SET_GYRO_OUTPUT_RATE_ERROR;
                    break;
                }
                imu_interface_read_8bit_register(
                    interface, IMU660RB_DEV_ADDR, 
                    (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                        ? (IMU660RB_CTRL2_G | IMU660RB_SPI_R)
                        : IMU660RB_CTRL2_G,
                    &temp_data);
                temp_data = ((temp_data & ~0xF0) |(((item & 0x0F) - 5) << 4));
                imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL2_G, temp_data);
            }break;
            case 0x03:
            {
                return_state = IMU660RB_OPERATION_DONE;
                if(imu_interface_assert(
                            IMU_GYRO_RANGE_SGN_125DPS  <= (item & 0x0F)
                        &&  IMU_GYRO_RANGE_SGN_4000DPS >= (item & 0x0F)))
                {
                    return_state = IMU660RB_ERROR_SET_GYRO_RANGE_ERROR;
                    break;
                }
                imu_interface_read_8bit_register(
                    interface, IMU660RB_DEV_ADDR, 
                    (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                        ? (IMU660RB_CTRL2_G | IMU660RB_SPI_R)
                        : IMU660RB_CTRL2_G,
                    &temp_data);
                const uint8 gyro_range_list[8] = {0x00, 0x00, 0x02, 0x00, 0x04, 0x08, 0x0C, 0x01};
                temp_data = ((temp_data & ~0x0F) | gyro_range_list[(item & 0x0F)]);
                imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL2_G, temp_data);
            }break;
            default:
            {
                if(imu_interface_assert(0))
                {
                    return_state = IMU660RB_ERROR_SET_CONFIG_ERROR;
                    break;
                }
            }break;
        }
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU660RB 六轴 检测 ID 是否匹配 框架接口
// 参数说明     *interface      IMU660RB 的通信接口对象指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_func_ckeck_id(interface);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660rb_func_ckeck_id (imu_interface_obj_struct *interface)
{
    imu660rb_operation_state_enum   return_state = IMU660RB_ERROR_UNKNOW_ERROR;

    uint8   temp_data       = 0;
    uint16  timeout_count   = 0;

    do
    {
        return_state = IMU660RB_OPERATION_DONE;
        do
        {
            if(IMU660RB_TIMEOUT_COUNT < timeout_count ++)
            {
                return_state = IMU660RB_ERROR_ID_CHECK_ERROR;
                break;
            }
            imu_interface_read_8bit_register(
                interface, IMU660RB_DEV_ADDR, 
                (IMU_INTERFACE_TYPE_SPI == interface->interface_type)
                    ? (IMU660RB_CHIP_ID | IMU660RB_SPI_R)
                    : IMU660RB_CHIP_ID,
                &temp_data);
            imu_interface_delay_ms(10);
        }while(0x6B != temp_data);

    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 IMU660RB 框架接口
// 参数说明     *interface      IMU660RB 的通信接口对象指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_func_init(interface);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660rb_func_init (imu_interface_obj_struct *interface)
{
    imu660rb_operation_state_enum   return_state = IMU660RB_ERROR_UNKNOW_ERROR;

    do
    {
        imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_INT1_CTRL, 0x03);    // 开启陀螺仪 加速度数据就绪中断

        uint8 register_temp = 0;

        if(imu_interface_assert(
                    IMU_ACC_OUTPUT_RATE_4_DIV   <= IMU_ACC_OUTPUT_RATE_DEFAULT
                &&  IMU_ACC_OUTPUT_RATE_128_MUL >= IMU_ACC_OUTPUT_RATE_DEFAULT))
        {
            return_state = IMU660RB_ERROR_SET_ACC_OUTPUT_RATE_ERROR;
            break;
        }
        register_temp = ((IMU_ACC_OUTPUT_RATE_DEFAULT - 5) << 4);
        if(imu_interface_assert(
                    IMU_ACC_RANGE_SGN_2G    <= IMU_ACC_RANGE_DEFAULT
                &&  IMU_ACC_RANGE_SGN_16G   >= IMU_ACC_RANGE_DEFAULT))
        {
            return_state = IMU660RB_ERROR_SET_ACC_RANGE_ERROR;
            break;
        }
        const uint8 acc_range_list[6] = {0x00, 0x00, 0x00, 0x08, 0x0C, 0x04};
        register_temp |= acc_range_list[IMU_ACC_RANGE_DEFAULT];
        imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL1_XL, register_temp);

        if(imu_interface_assert(
                    IMU_GYRO_OUTPUT_RATE_4_DIV      <= IMU_GYRO_OUTPUT_RATE_DEFAULT
                &&  IMU_GYRO_OUTPUT_RATE_128_MUL    >= IMU_GYRO_OUTPUT_RATE_DEFAULT))
        {
            return_state = IMU660RB_ERROR_SET_GYRO_OUTPUT_RATE_ERROR;
            break;
        }
        register_temp = ((IMU_GYRO_OUTPUT_RATE_DEFAULT - 5) << 4);
        if(imu_interface_assert(
                    IMU_GYRO_RANGE_SGN_125DPS   <= IMU_GYRO_RANGE_DEFAULT
                &&  IMU_GYRO_RANGE_SGN_4000DPS  >= IMU_GYRO_RANGE_DEFAULT))
        {
            return_state = IMU660RB_ERROR_SET_GYRO_RANGE_ERROR;
            break;
        }
        const uint8 gyro_range_list[8] = {0x00, 0x00, 0x02, 0x00, 0x04, 0x08, 0x0C, 0x01};
        register_temp |= gyro_range_list[IMU_GYRO_RANGE_DEFAULT];
        imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL2_G, register_temp);

        imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL3_C, 0x44);               // 使能陀螺仪数字低通滤波器
        imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL4_C, 0x02);               // 使能数字低通滤波器
        imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL5_C, 0x00);               // 加速度计与陀螺仪四舍五入
        imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL6_C, 0x00);               // 开启加速度计高性能模式 陀螺仪低通滤波 133hz
        imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL7_G, 0x00);               // 开启陀螺仪高性能模式 关闭高通滤波
        imu_interface_write_8bit_register(interface, IMU660RB_DEV_ADDR, IMU660RB_CTRL9_XL, 0x01);              // 关闭I3C接口

        return_state = IMU660RB_OPERATION_DONE;
    }while(0);

    return return_state;
}

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
uint8 imu660rb_get_physical_acc (imu660rb_measurement_data_struct *result, imu_acc_range_enum range,imu660rb_physical_data_struct *physical_data)
{
    imu660rb_operation_state_enum   return_state = IMU660RB_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != result))
        {
            return_state = IMU660RB_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(imu_interface_assert(
                    IMU_ACC_RANGE_SGN_2G   <= range
                &&  IMU_ACC_RANGE_SGN_16G  >= range))
        {
            return_state = IMU660RB_ERROR_SET_ACC_RANGE_ERROR;
            break;
        }
        if(imu_interface_assert(NULL != physical_data))
        {
            return_state = IMU660RB_ERROR_DATA_BUFFER_NULL;
            break;
        }
        const float transition_factor[4] = {1000.0 / 0.061, 1000.0 / 0.122, 1000.0 / 0.244, 1000.0 / 0.488};
        physical_data->x  = (float)result->x / transition_factor[range - IMU_ACC_RANGE_SGN_2G];
        physical_data->y  = (float)result->y / transition_factor[range - IMU_ACC_RANGE_SGN_2G];
        physical_data->z  = (float)result->z / transition_factor[range - IMU_ACC_RANGE_SGN_2G];

        return_state = IMU660RB_OPERATION_DONE;
    }while(0);

    return return_state;
}

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
uint8 imu660rb_get_physical_gyro (imu660rb_measurement_data_struct *result, imu_gyro_range_enum range,imu660rb_physical_data_struct *physical_data)
{
    imu660rb_operation_state_enum   return_state = IMU660RB_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != result))
        {
            return_state = IMU660RB_ERROR_DATA_BUFFER_NULL;
            break;
        }
        if(imu_interface_assert(
                    IMU_GYRO_RANGE_SGN_125DPS  <= range
                &&  IMU_GYRO_RANGE_SGN_4000DPS >= range))
        {
            return_state = IMU660RB_ERROR_SET_GYRO_RANGE_ERROR;
            break;
        }
        if(imu_interface_assert(NULL != physical_data))
        {
            return_state = IMU660RB_ERROR_DATA_BUFFER_NULL;
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

        return_state = IMU660RB_OPERATION_DONE;
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU660RB 加速度计数据
// 参数说明     *result         IMU660RB 的测量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_get_acc(result);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_get_acc (imu660rb_measurement_data_struct *result)
{
    imu660rb_operation_state_enum   return_state = IMU660RB_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != result))                                // 检查数据缓冲是否有效
        {
            return_state = IMU660RB_ERROR_DATA_BUFFER_NULL;
            break;
        }

        return_state = imu660rb_func_get_acc(&imu_interface_default_obj, result);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU660RB 陀螺仪数据
// 参数说明     *result         IMU660RB 的测量数据缓冲指针
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_get_gyro(result);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_get_gyro (imu660rb_measurement_data_struct *result)
{
    imu660rb_operation_state_enum   return_state = IMU660RB_ERROR_UNKNOW_ERROR;

    do
    {
        if(imu_interface_assert(NULL != result))                                // 检查数据缓冲是否有效
        {
            return_state = IMU660RB_ERROR_DATA_BUFFER_NULL;
            break;
        }

        return_state = imu660rb_func_get_gyro(&imu_interface_default_obj, result);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU660RB 配置模块工作参数
// 参数说明     item            IMU660RB 配置参照 zf_device_imu_interface.h 中 imu_config_item_enum 枚举体定义
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_set_config(item);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_set_config (imu_config_item_enum item)
{
    return imu660rb_func_set_config(&imu_interface_default_obj, item);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 IMU660RB
// 参数说明     void
// 返回参数     uint8           操作状态 ZF_NO_ERROR / IMU660RB_OPERATION_DONE - 完成 其余值为异常
// 使用示例     imu660rb_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_init (void)
{
    imu660rb_operation_state_enum   return_state = IMU660RB_ERROR_UNKNOW_ERROR;

    uint8 init_step = 0x00;

    do
    {
        if(imu_interface_assert(!imu_interface_spi_default_init(&imu_interface_default_obj, SPI_MODE0)))
        {
            return_state = IMU660RB_ERROR_INTERFACE_OCCUPIED;
            break;
        }
        init_step |= 0x01;

        imu_interface_delay_ms(20);                                                 // 等待设备上电成功

        if(imu660rb_func_ckeck_id(&imu_interface_default_obj))
        {
            // 如果程序在输出了断言信息 并且提示出错位置在这里
            // 那么就是 IMU660RB 自检出错并超时退出了
            // 检查一下接线有没有问题 如果没问题可能就是坏了
            return_state = IMU660RB_ERROR_SELF_CHECK_ERROR;
            break;
        }

        return_state = imu660rb_func_init(&imu_interface_default_obj);
    }while(0);

    if(return_state && (init_step & 0x01))
    {
        imu_interface_deinit(&imu_interface_default_obj);                       // 那么就需要将接口注销初始化
    }

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
