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
* 文件名称          zf_device_gnss
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

/*********************************************************************************************************************
* 接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   RX                  查看 zf_device_gnss.h 中 GNSS_RX 宏定义
*                   TX                  查看 zf_device_gnss.h 中 GNSS_TX 宏定义
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/

// 自身头文件
#include "zf_device_gnss.h"

// 此处定义 GNSS 相关的接口资源 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define GNSS_BUFFER_SIZE    ( 128 )
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件用使用的变量与对象等 这里不允许用户修改 这里不允许用户修改 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
AT_ZF_LIB_SECTION_START
AT_ZF_LIB_SECTION uint8                         gnss_flag = 0;                          // 1-采集完成等待处理数据 0-没有采集完成
AT_ZF_LIB_SECTION gnss_info_struct              gnss_info;                              // GNSS 解析之后的数据

AT_ZF_LIB_SECTION static  uint8                 gnss_state = 0;                         // 1-GNSS 初始化完成
AT_ZF_LIB_SECTION static  zf_fifo_obj_struct    gnss_receiver_fifo;                     //
AT_ZF_LIB_SECTION static  uint8                 gnss_receiver_buffer[GNSS_BUFFER_SIZE]; // 数据存放数组

AT_ZF_LIB_SECTION static  gnss_state_enum       gnss_gga_state = GNSS_STATE_RECEIVING;  // gga 语句状态
AT_ZF_LIB_SECTION static  gnss_state_enum       gnss_rmc_state = GNSS_STATE_RECEIVING;  // rmc 语句状态
AT_ZF_LIB_SECTION static  gnss_state_enum       gnss_ths_state = GNSS_STATE_RECEIVING;  // rmc 语句状态

AT_ZF_LIB_SECTION static  uint8                 gnss_gga_buffer[GNSS_BUFFER_SIZE];
AT_ZF_LIB_SECTION static  uint8                 gnss_rmc_buffer[GNSS_BUFFER_SIZE];
AT_ZF_LIB_SECTION static  uint8                 gnss_ths_buffer[GNSS_BUFFER_SIZE];
AT_ZF_LIB_SECTION_END
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取指定 ',' 后面的索引
// 参数说明     num             第几个逗号
// 参数说明     *str            字符串
// 返回参数     uint8           返回索引
// 使用示例     get_parameter_index(num, str);
// 备注信息     内部使用
//-------------------------------------------------------------------------------------------------------------------
static uint8 get_parameter_index (uint8 num, char *str)
{
    uint8   return_value    = 0;

    uint8   search_len      = 0;
    uint8   temp_value1     = 0;
    uint8   temp_value2     = 0;
    char    *str_tail       = strchr(str, '\n');

    if(NULL != str_tail)
    {
        search_len = (uint8)((uint32)str_tail - (uint32)str + 1);
    }

    for(temp_value1 = 0; temp_value1 < search_len; temp_value1 ++)
    {
        if(',' == str[temp_value1])
        {
            temp_value2 ++;
        }
        if(temp_value2 == num)
        {
            return_value =  temp_value1 + 1;
            break;
        }
    }

    return return_value;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     给定字符串第一个 ',' 之前的数据转换为int
// 参数说明     *str            字符串
// 返回参数     float           返回数值
// 使用示例     get_int_number(str);
// 备注信息     内部使用
//-------------------------------------------------------------------------------------------------------------------
static int get_int_number (char *str)
{
    int return_value = 0;

    char buf[10];
    uint8 temp_value1 = 0;

    temp_value1 = get_parameter_index(1, str);
    temp_value1 = temp_value1 - 1;
    strncpy(buf, str, temp_value1);
    buf[temp_value1] = 0;
    return_value = zf_function_str_to_int(buf);

    return return_value;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     给定字符串第一个 ',' 之前的数据转换为float
// 参数说明     *str            字符串
// 返回参数     float           返回数值
// 使用示例     get_float_number(str);
// 备注信息     内部使用
//-------------------------------------------------------------------------------------------------------------------
static float get_float_number (char *str)
{
    float return_value = 0;

    uint8 temp_value1 = 0;
    char buf[15];

    temp_value1 = get_parameter_index(1, str);
    temp_value1 = temp_value1 - 1;
    strncpy(buf, str, temp_value1);
    buf[temp_value1] = 0;
    return_value = (float)zf_function_str_to_double(buf);

    return return_value;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     给定字符串第一个 ',' 之前的数据转换为double
// 参数说明     *str            字符串
// 返回参数     double          返回数值
// 使用示例     get_double_number(str);
// 备注信息     内部使用
//-------------------------------------------------------------------------------------------------------------------
static double get_double_number (char *str)
{
    double return_value = 0;

    uint8 temp_value1 = 0;
    char buf[15];

    temp_value1 = get_parameter_index(1, str);
    temp_value1 = temp_value1 - 1;
    strncpy(buf, str, temp_value1);
    buf[temp_value1] = 0;
    return_value = zf_function_str_to_double(buf);

    return return_value;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     世界时间转换为北京时间
// 参数说明     *time           保存的时间
// 返回参数     void
// 使用示例     utc_to_btc(&gnss->time);
// 备注信息     内部使用
//-------------------------------------------------------------------------------------------------------------------
static void utc_to_btc (gnss_time_struct *time)
{
    uint8 day_num = 0;

    time->hour = time->hour + 8;
    if(23 < time->hour)
    {
        time->hour  -= 24;
        time->day   += 1;

        if(2 == time->month)
        {
            day_num = 28;
            if(     (   0 == time->year % 4
                    &&  0 != time->year % 100)
                ||  0 == time->year % 400)                                      // 判断是否为闰年
            {
                day_num ++;                                                     // 闰月 2月为29天
            }
        }
        else
        {
            day_num = 31;                                                       // 1 3 5 7 8 10 12这些月份为31天
            if(     4  == time->month
                ||  6  == time->month
                ||  9  == time->month
                ||  11 == time->month )
            {
                day_num = 30;
            }
        }

        if(time->day > day_num)
        {
            time->day = 1;
            time->month ++;
            if(12 < time->month)
            {
                time->month -= 12;
                time->year ++;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS RMC 语句解析
// 参数说明     *line           接收到的语句信息
// 参数说明     *gnss           保存解析后的数据
// 返回参数     uint8           ZF_TRUE-解析成功 ZF_FALSE-数据有问题不能解析
// 使用示例     gnss_gnrmc_parse(line, gnss);
// 备注信息     内部使用
//-------------------------------------------------------------------------------------------------------------------
static uint8 gnss_gnrmc_parse (char *line, gnss_info_struct *gnss)
{
    uint8 return_state = ZF_FALSE;

    uint8 state = 0, temp = 0;

    double  latitude = 0;                                                       // 纬度
    double  longitude = 0;                                                      // 经度

    double lati_cent_tmp = 0, lati_second_tmp = 0;
    double long_cent_tmp = 0, long_second_tmp = 0;
    float speed_tmp = 0;
    char *buf = line;

    state = buf[get_parameter_index(2, buf)];

    if(('A' == state) || ('D' == state))                                        // 如果数据有效 则解析数据
    {
        return_state = ZF_TRUE;
        gnss->state = 1;
        gnss -> ns              = buf[get_parameter_index(4, buf)];
        gnss -> ew              = buf[get_parameter_index(6, buf)];

        latitude                = get_double_number(&buf[get_parameter_index(3, buf)]);
        longitude               = get_double_number(&buf[get_parameter_index(5, buf)]);

        gnss->latitude_degree   = (int)latitude / 100;                          // 纬度转换为度分秒
        lati_cent_tmp           = (latitude - gnss->latitude_degree * 100);
        gnss->latitude_cent     = (int)lati_cent_tmp;
        lati_second_tmp         = (lati_cent_tmp - gnss->latitude_cent) * 6000;
        gnss->latitude_second   = (int)lati_second_tmp;

        gnss->longitude_degree  = (int)longitude / 100;                         // 经度转换为度分秒
        long_cent_tmp           = (longitude - gnss->longitude_degree * 100);
        gnss->longitude_cent    = (int)long_cent_tmp;
        long_second_tmp         = (long_cent_tmp - gnss->longitude_cent) * 6000;
        gnss->longitude_second  = (int)long_second_tmp;

        gnss->latitude  = gnss->latitude_degree + lati_cent_tmp / 60;
        gnss->longitude = gnss->longitude_degree + long_cent_tmp / 60;

        speed_tmp       = get_float_number(&buf[get_parameter_index(7, buf)]);  // 速度(海里/小时)
        gnss->speed     = speed_tmp * 1.85f;                                    // 转换为公里/小时
        gnss->direction = get_float_number(&buf[get_parameter_index(8, buf)]);  // 角度
    }
    else
    {
        gnss->state = 0;
    }

    // 在定位没有生效前也是有时间数据的，可以直接解析
    gnss->time.hour    = (buf[7 ] - '0') * 10 + (buf[8 ] - '0');                // 时间
    gnss->time.minute  = (buf[9 ] - '0') * 10 + (buf[10] - '0');
    gnss->time.second  = (buf[11] - '0') * 10 + (buf[12] - '0');
    temp = get_parameter_index(9, buf);
    gnss->time.day     = (buf[temp + 0] - '0') * 10 + (buf[temp + 1] - '0');    // 日期
    gnss->time.month   = (buf[temp + 2] - '0') * 10 + (buf[temp + 3] - '0');
    gnss->time.year    = (buf[temp + 4] - '0') * 10 + (buf[temp + 5] - '0') + 2000;

    utc_to_btc(&gnss->time);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS GGA 语句解析
// 参数说明     *line           接收到的语句信息
// 参数说明     *gnss           保存解析后的数据
// 返回参数     uint8           ZF_TRUE-解析成功 ZF_FALSE-数据有问题不能解析
// 使用示例     gnss_gngga_parse(line, gnss);
// 备注信息     内部使用
//-------------------------------------------------------------------------------------------------------------------
static uint8 gnss_gngga_parse (char *line, gnss_info_struct *gnss)
{
    uint8 return_state = ZF_FALSE;

    uint8 state = 0;
    char *buf = line;

    state = buf[get_parameter_index(2, buf)];

    if(',' != state)
    {
        gnss->satellite_used = (uint8)get_int_number(&buf[get_parameter_index(7 , buf)]);
        // 高度 = 海拔高度 + 地球椭球面相对大地水准面的高度
        gnss->height    =   get_float_number(&buf[get_parameter_index(9 , buf)])
                        +   get_float_number(&buf[get_parameter_index(11, buf)]);
        return_state = ZF_TRUE;
    }

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS THS 语句解析
// 参数说明     *line           接收到的语句信息
// 参数说明     *gnss           保存解析后的数据
// 返回参数     uint8           ZF_TRUE-解析成功 ZF_FALSE-数据有问题不能解析
// 使用示例     gnss_gnths_parse(line, gnss);
// 备注信息     内部使用
//-------------------------------------------------------------------------------------------------------------------
static uint8 gnss_gnths_parse (char *line, gnss_info_struct *gnss)
{
    uint8 return_state = ZF_FALSE;

    uint8 state = 0;
    char *buf = line;

    state = buf[get_parameter_index(2, buf)];

    if('A' == state)
    {
        gnss->antenna_direction_state = 1;
        gnss->antenna_direction = get_float_number(&buf[get_parameter_index(1, buf)]);
        return_state = ZF_TRUE;
    }
    else
    {
        gnss->antenna_direction_state = 0;
    }

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / CAN_OPERATION_DONE - 完成 其余值为异常
// 使用示例     gnss_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
#if GNSS_INTERFACE_USE_ZF_COMMON_DEBUG
#define gnss_assert(pass)       zf_assert(pass)
#define gnss_log(pass, tips)    zf_log(pass, tips)
#else
static uint8 gnss_assert_handler (char *file, int line, char *tips)
{
    printf("\r\n GNSS OPERATION ASSERT!\r\n");
    printf("\r\n file: %s.\r\n", file);
    printf("\r\n line: %d.\r\n", line);
    if(NULL != tips)
    {
        printf("\r\n tips: %s.\r\n", tips);
    }
    while(1);
    return ZF_ERROR;
}
#define gnss_assert(pass)       ((pass) ? (0) : gnss_assert_handler(__FILE__, __LINE__, NULL))
#define gnss_log(pass, tips)    ((pass) ? (0) : gnss_assert_handler(__FILE__, __LINE__, (tips)))
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS 延时
// 参数说明     ms                  毫秒数
// 返回参数     void
// 使用示例     gnss_delay_ms(ms);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
#if GNSS_INTERFACE_USE_ZF_DIRVER_DELAY
#define gnss_delay_ms(ms)        zf_delay_ms(ms)
#else
static void gnss_delay_ms (uint32 ms)
{
    for(uint32 loop_count1 = ms    ; loop_count1 --; )
    for(uint32 loop_count2 = 0xFFFF; loop_count2 --; );
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS 串口回调函数
// 参数说明     void
// 返回参数     void
// 使用示例     gnss_uart_callback();
// 备注信息     此函数需要在串口接收中断内进行调用
//-------------------------------------------------------------------------------------------------------------------
static void gnss_uart_callback (uint32 event, void *ptr)
{
    (void)event;
    (void)ptr;

    uint8 temp_gnss[6];
    uint32 temp_length = 0;

    if(gnss_state)
    {
        uint8 dat;
        while(!zf_uart_query_byte(GNSS_UART_INDEX, &dat))
        {
            temp_length = 1;
            zf_fifo_write_buffer(&gnss_receiver_fifo, &dat, &temp_length);
        }

        if('\n' == dat)
        {
            // 读取前6个数据 用于判断语句类型
            temp_length = 6;
            zf_fifo_read_buffer(&gnss_receiver_fifo, temp_gnss, &temp_length, FIFO_READ_WITHOUT_CLEAN);

            // 根据不同类型将数据拷贝到不同的缓冲区
            if(0 == strncmp((char *)&temp_gnss[3], "RMC", 3))
            {
                // 如果没有在解析数据则更新缓冲区的数据
                if(GNSS_STATE_PARSING != gnss_rmc_state)
                {
                    gnss_rmc_state = GNSS_STATE_RECEIVED;
                    temp_length = zf_fifo_used(&gnss_receiver_fifo);
                    zf_fifo_read_buffer(&gnss_receiver_fifo, gnss_rmc_buffer, &temp_length, FIFO_READ_WITH_CLEAN);
                }
            }
            else if(0 == strncmp((char *)&temp_gnss[3], "GGA", 3))
            {
                // 如果没有在解析数据则更新缓冲区的数据
                if(GNSS_STATE_PARSING != gnss_gga_state)
                {
                    gnss_gga_state = GNSS_STATE_RECEIVED;
                    temp_length = zf_fifo_used(&gnss_receiver_fifo);
                    zf_fifo_read_buffer(&gnss_receiver_fifo, gnss_gga_buffer, &temp_length, FIFO_READ_WITH_CLEAN);
                }
            }
            else if(0 == strncmp((char *)&temp_gnss[3], "THS", 3))
            {
                // 如果没有在解析数据则更新缓冲区的数据
                if(GNSS_STATE_PARSING != gnss_ths_state)
                {
                    gnss_ths_state = GNSS_STATE_RECEIVED;
                    temp_length = zf_fifo_used(&gnss_receiver_fifo);
                    zf_fifo_read_buffer(&gnss_receiver_fifo, gnss_ths_buffer, &temp_length, FIFO_READ_WITH_CLEAN);
                }
            }

            // 统一将FIFO清空
            zf_fifo_clear(&gnss_receiver_fifo);

            gnss_flag = 1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS 计算从第一个点到第二个点的距离
// 参数说明     latitude1       第一个点的纬度
// 参数说明     longitude1      第一个点的经度
// 参数说明     latitude2       第二个点的纬度
// 参数说明     longitude2      第二个点的经度
// 返回参数     double          返回两点距离
// 使用示例     gnss_get_two_points_distance(latitude1_1, longitude1, latitude2, longitude2);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
double gnss_get_two_points_distance (double latitude1, double longitude1, double latitude2, double longitude2)
{
    const double EARTH_RADIUS = 6378137;                                        // 地球半径(单位：m)
    double rad_latitude1 = 0;
    double rad_latitude2 = 0;
    double rad_longitude1 = 0;
    double rad_longitude2 = 0;
    double distance = 0;
    double a = 0;
    double b = 0;

    rad_latitude1   = ANGLE_TO_RAD(latitude1);                                  // 根据角度计算弧度
    rad_latitude2   = ANGLE_TO_RAD(latitude2);
    rad_longitude1  = ANGLE_TO_RAD(longitude1);
    rad_longitude2  = ANGLE_TO_RAD(longitude2);

    a = rad_latitude1 - rad_latitude2;
    b = rad_longitude1 - rad_longitude2;

    distance = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(rad_latitude1) * cos(rad_latitude2) * pow(sin(b / 2), 2)));   // google maps 里面实现的算法
    distance = distance * EARTH_RADIUS;

    return distance;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS 计算从第一个点到第二个点的方位角
// 参数说明     latitude1       第一个点的纬度
// 参数说明     longitude1      第一个点的经度
// 参数说明     latitude2       第二个点的纬度
// 参数说明     longitude2      第二个点的经度
// 返回参数     double          返回方位角（0至360）
// 使用示例     gnss_get_two_points_azimuth(latitude1_1, longitude1, latitude2, longitude2);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
double gnss_get_two_points_azimuth (double latitude1, double longitude1, double latitude2, double longitude2)
{
    latitude1   = ANGLE_TO_RAD(latitude1);
    latitude2   = ANGLE_TO_RAD(latitude2);
    longitude1  = ANGLE_TO_RAD(longitude1);
    longitude2  = ANGLE_TO_RAD(longitude2);

    double x = sin(longitude2 - longitude1) * cos(latitude2);
    double y = cos(latitude1) * sin(latitude2) - sin(latitude1) * cos(latitude2) * cos(longitude2 - longitude1);
    double angle = RAD_TO_ANGLE(atan2(x, y));

    return ((0 < angle) ? angle : (angle + 360));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS 解析数据
// 参数说明     void
// 返回参数     uint8           ZF_TRUE-解析成功 ZF_FALSE-数据有问题不能解析
// 使用示例     gnss_data_parse();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 gnss_data_parse (void)
{
    uint8 return_state = ZF_NO_ERROR;

    uint8 check_buffer[5] = {'0', 'x', 0x00, 0x00, 0x00};
    uint8 bbc_xor_origin = 0;
    uint8 bbc_xor_calculation = 0;
    uint32 data_len = 0;

    do
    {
        if(GNSS_STATE_RECEIVED == gnss_rmc_state)
        {
            gnss_rmc_state = GNSS_STATE_PARSING;
            strncpy((char *)&check_buffer[2], strchr((const char *)gnss_rmc_buffer, '*') + 1, 2);
            bbc_xor_origin = (uint8)zf_function_str_to_hex((char *)check_buffer);
            for(bbc_xor_calculation = gnss_rmc_buffer[1], data_len = 2; '*' != gnss_rmc_buffer[data_len]; data_len ++)
            {
                bbc_xor_calculation ^= gnss_rmc_buffer[data_len];
            }
            if(bbc_xor_calculation != bbc_xor_origin)
            {
                // 数据校验失败
                return_state = ZF_ERROR;
                break;
            }

            gnss_gnrmc_parse((char *)gnss_rmc_buffer, &gnss_info);
        }
        gnss_rmc_state = GNSS_STATE_RECEIVING;

        if(GNSS_STATE_RECEIVED == gnss_gga_state)
        {
            gnss_gga_state = GNSS_STATE_PARSING;
            strncpy((char *)&check_buffer[2], strchr((const char *)gnss_gga_buffer, '*') + 1, 2);
            bbc_xor_origin = (uint8)zf_function_str_to_hex((char *)check_buffer);

            for(bbc_xor_calculation = gnss_gga_buffer[1], data_len = 2; '*' != gnss_gga_buffer[data_len]; data_len ++)
            {
                bbc_xor_calculation ^= gnss_gga_buffer[data_len];
            }
            if(bbc_xor_calculation != bbc_xor_origin)
            {
                // 数据校验失败
                return_state = ZF_ERROR;
                break;
            }

            gnss_gngga_parse((char *)gnss_gga_buffer, &gnss_info);
        }
        gnss_gga_state = GNSS_STATE_RECEIVING;

        if(GNSS_STATE_RECEIVED == gnss_ths_state)
        {
            gnss_ths_state = GNSS_STATE_PARSING;
            strncpy((char *)&check_buffer[2], strchr((const char *)gnss_ths_buffer, '*') + 1, 2);
            bbc_xor_origin = (uint8)zf_function_str_to_hex((char *)check_buffer);

            for(bbc_xor_calculation = gnss_ths_buffer[1], data_len = 2; '*' != gnss_ths_buffer[data_len]; data_len ++)
            {
                bbc_xor_calculation ^= gnss_ths_buffer[data_len];
            }
            if(bbc_xor_calculation != bbc_xor_origin)
            {
                // 数据校验失败
                return_state = ZF_ERROR;
                break;
            }

            gnss_gnths_parse((char *)gnss_ths_buffer, &gnss_info);
        }
        gnss_ths_state = GNSS_STATE_RECEIVING;

    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GNSS 初始化
// 参数说明     void
// 返回参数     uint8           操作状态 ZF_NO_ERROR - 完成 其余值为异常
// 使用示例     gnss_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 gnss_init (gnss_type_enum gnss_type)
{
    uint8 return_state = ZF_ERROR;

    const uint8 set_rate[]      = {0xF1, 0xD9, 0x06, 0x42, 0x14, 0x00, 0x00, 0x0A, 0x05, 0x00, 0x64, 0x00, 0x00, 0x00, 0x60, 0xEA, 0x00, 0x00, 0xD0, 0x07, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x00, 0xB8, 0xED};
    const uint8 open_gga[]      = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x00, 0x01, 0xFB, 0x10};
    const uint8 open_rmc[]      = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x05, 0x01, 0x00, 0x1A};

    const uint8 close_gll[]     = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x01, 0x00, 0xFB, 0x11};
    const uint8 close_gsa[]     = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x02, 0x00, 0xFC, 0x13};
    const uint8 close_grs[]     = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x03, 0x00, 0xFD, 0x15};
    const uint8 close_gsv[]     = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x04, 0x00, 0xFE, 0x17};
    const uint8 close_vtg[]     = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x06, 0x00, 0x00, 0x1B};
    const uint8 close_zda[]     = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x07, 0x00, 0x01, 0x1D};
    const uint8 close_gst[]     = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x08, 0x00, 0x02, 0x1F};
    const uint8 close_txt[]     = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x40, 0x00, 0x3A, 0x8F};
    const uint8 close_txt_ant[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x20, 0x00, 0x1A, 0x4F};

    switch(gnss_type)
    {
        case GNSS_TYPE_TAU1201:
        {
            zf_fifo_init(&gnss_receiver_fifo, FIFO_DATA_8BIT, gnss_receiver_buffer, GNSS_BUFFER_SIZE);
            gnss_delay_ms(500);                                                 // 等待GNSS启动后开始初始化
            zf_uart_init(GNSS_UART_INDEX, 115200, GNSS_RX_PIN, GNSS_TX_PIN);

            // 设置GNSS更新速率为10hz 如果不调用此语句则默认为1hz
            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)set_rate, sizeof(set_rate));
            gnss_delay_ms(200);

            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)open_rmc, sizeof(open_rmc));
            gnss_delay_ms(50);
            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)open_gga, sizeof(open_gga));
            gnss_delay_ms(50);
            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)close_gll, sizeof(close_gll));
            gnss_delay_ms(50);
            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)close_gsa, sizeof(close_gsa));
            gnss_delay_ms(50);
            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)close_grs, sizeof(close_grs));
            gnss_delay_ms(50);
            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)close_gsv, sizeof(close_gsv));
            gnss_delay_ms(50);
            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)close_vtg, sizeof(close_vtg));
            gnss_delay_ms(50);
            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)close_zda, sizeof(close_zda));
            gnss_delay_ms(50);
            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)close_gst, sizeof(close_gst));
            gnss_delay_ms(50);
            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)close_txt, sizeof(close_txt));
            gnss_delay_ms(50);
            zf_uart_write_buffer(GNSS_UART_INDEX, (uint8 *)close_txt_ant, sizeof(close_txt_ant));
            gnss_delay_ms(50);

            gnss_state = 1;
            zf_uart_set_interrupt_callback(GNSS_UART_INDEX, gnss_uart_callback, NULL);
            zf_uart_set_interrupt_config(GNSS_UART_INDEX, UART_INTERRUPT_CONFIG_RX_ENABLE);

            return_state = ZF_NO_ERROR;
        }break;
        case GNSS_TYPE_GN43RFA:
        {
            // GN43RFA RTK模块不需要进行参数设置，如果需要修改参数应该使用专用的上位机修改参数
            zf_fifo_init(&gnss_receiver_fifo, FIFO_DATA_8BIT, gnss_receiver_buffer, GNSS_BUFFER_SIZE);
            zf_uart_init(GNSS_UART_INDEX, 115200, GNSS_RX_PIN, GNSS_TX_PIN);

            gnss_state = 1;
            zf_uart_set_interrupt_callback(GNSS_UART_INDEX, gnss_uart_callback, NULL);
            zf_uart_set_interrupt_config(GNSS_UART_INDEX, UART_INTERRUPT_CONFIG_RX_ENABLE);

            return_state = ZF_NO_ERROR;
        }break;
        default:
        {
        }break;
    }

    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
