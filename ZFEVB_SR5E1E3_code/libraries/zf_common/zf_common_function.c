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
* 文件名称          zf_common_function
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          StellarStudio 7.0.0
* 适用平台          Stellar-SR5E1E3
* 店铺链接          https://www.seekfree.cn/
********************************************************************************************************************/

// zf_common 层引用
#include "zf_common_debug.h"

// 自身头文件
#include "zf_common_function.h"

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     Fucntion 断言处理
// 参数说明     *file               文件路径
// 参数说明     line                报错行
// 参数说明     *tips               输出信息
// 返回参数     uint8               操作状态 ZF_NO_ERROR / GPIO_OPERATION_DONE - 完成 其余值为异常
// 使用示例     zf_function_assert_handler(file, line, tips);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 zf_function_assert_handler (char *file, int line, char *tips)
{
    while(1)
    {
        printf("Function driver assert:\r\n");
        printf("At : %s,\r\n", file);
        printf("Line : %d.\r\n", line);
        if(NULL != tips)
        {
            printf("Tips : %s.\r\n", tips);
        }
    }
    return ZF_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     使用泰勒级数计算 sin
// 参数说明     x                   弧度
// 返回参数     double              计算结果
// 使用示例     printf("sin is : %f.\r\n", zf_function_sin(ZF_PI_DEFAULT / 6));
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
double zf_function_sin (double x)
{
    // 将 x 限制在[-π, π]范围内
    for(; x < -ZF_PI_DEFAULT * 2; x += ZF_PI_DEFAULT * 2);
    for(; x >  ZF_PI_DEFAULT * 2; x -= ZF_PI_DEFAULT * 2);
    x =     (x > ZF_PI_DEFAULT)
        ?   (x - 2 * ZF_PI_DEFAULT)
        :   (   (x < -ZF_PI_DEFAULT)
            ?   (x + 2 * ZF_PI_DEFAULT)
            :   (x));

    // 使用泰勒级数计算 sin(x)
    double  term    = x;                                                        // 第一项
    double  sum     = term;
    int     n       = 1;
    while (term > 1e-10 || -term > 1e-10)                                       // 计算更多的项 直到它们对结果的影响可以忽略不计
    {
        n       += 2;                                                           // 下一个项的指数
        term    *= -1 * x * x / (n * (n - 1));                                  // 下一项的值
        sum     += term;
    }

    return sum;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     计算 cos
// 参数说明     x                   弧度
// 返回参数     double              计算结果
// 使用示例     printf("cos is : %f.\r\n", zf_function_cos(ZF_PI_DEFAULT / 6));
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
double zf_function_cos (double x)
{
    return zf_function_sin(x + ZF_PI_DEFAULT / 2);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     正弦波转换函数 获取指定采样点
// 参数说明     *data_buffer        数据缓冲区
// 参数说明     sample_max          采样点数
// 参数说明     amplitude_max       最大幅值
// 参数说明     offset_degree       偏移相位角
// 返回参数     void
// 使用示例     zf_function_get_sin_amplitude_table(sound_amplitude, 1024, 512, 270);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void zf_function_get_sin_amplitude_table (uint32 *data_buffer, uint32 sample_max, uint32 amplitude_max, uint32 offset_degree)
{
    uint32 i = 0, j = 0;
    double rad = 0.0;
    double amplitude = 0.0;

    offset_degree = offset_degree % 360;
    offset_degree = (uint32)((float)offset_degree / (float)360.0 * sample_max);

    j = amplitude_max / 2;                                                      // 偏置幅值 默认为一半最大幅值
    rad = ZF_PI_DEFAULT * 2 / sample_max;                                       // 换算每个采样点的弧度差
    for(i = offset_degree; i < sample_max + offset_degree; i ++)
    {
        amplitude = j * zf_function_sin(rad * i) + j;
        *data_buffer = (uint32)amplitude;
        data_buffer ++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取整型数的最大公约数 九章算术之更相减损术
// 参数说明     num1                数字1
// 参数说明     num2                数字2
// 返回参数     uint32              最大公约数
// 使用示例     return zf_function_get_greatest_common_divisor(144, 36);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_function_get_greatest_common_divisor (uint32 num1, uint32 num2)
{
    while(num1 != num2)
    {
        if(num1 > num2)
        {
            num1 = num1 - num2;
        }
        if(num1 < num2)
        {
            num2 = num2 - num1;
        }
    }
    return num1;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件延时
// 参数说明     tick                延时时间
// 返回参数     void
// 使用示例     zf_function_soft_delay(100);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_function_soft_delay (volatile long tick)
{
    while(tick --);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     数据元素定位
// 参数说明     data_size           数据元素类型 (详见 zf_common_typedef.h 的 common_data_size_enum 定义)
// 返回参数     *buffer             数据缓冲区
// 返回参数     buffer_length       数据缓冲区长度
// 返回参数     data                查询的数据值
// 返回参数     id                  查询的数据 ID 比如查询数组中的第一个 data 数据就填 1 此数据范围为 [1 - n]
// 使用示例     uint32 index = zf_function_get_data_index(COMMON_DATA_SIZE_8BIT, (void *)"-100", 4, '-', 1);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_function_get_data_index (common_data_size_enum data_size, void *buffer, uint32 buffer_length, uint32 data, uint32 id)
{
    zf_function_assert(
            COMMON_DATA_SIZE_8BIT  != data_size
        ||  COMMON_DATA_SIZE_16BIT != data_size
        ||  COMMON_DATA_SIZE_32BIT != data_size);
    zf_function_assert(NULL  !=  buffer);
    zf_function_assert(0     <   id);

    uint32 index = 0;
    uint32 data_temp = 0;
    uint32 data_mask = 0;

    switch(data_size)
    {
        default:
        case COMMON_DATA_SIZE_8BIT :    data_mask = 0x000000FF; break;
        case COMMON_DATA_SIZE_16BIT:    data_mask = 0x0000FFFF; break;
        case COMMON_DATA_SIZE_32BIT:    data_mask = 0xFFFFFFFF; break;
    }

    do
    {
        switch(data_size)
        {
            default:
            case COMMON_DATA_SIZE_8BIT :    data_temp = *((uint8  *)buffer + index);    break;
            case COMMON_DATA_SIZE_16BIT:    data_temp = *((uint16 *)buffer + index);    break;
            case COMMON_DATA_SIZE_32BIT:    data_temp = *((uint32 *)buffer + index);    break;
        }
        if((data & data_mask) == data_temp)
        {
            if(!-- id)
            {
                break;
            }
        }
        index ++;
    }while(index < buffer_length);

    return index;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     字符串转整形数字 数据范围是 [-32768,32767]
// 参数说明     *str                传入字符串 可带符号
// 返回参数     int32               转换后的数据
// 使用示例     int32 dat = zf_function_str_to_int("-100");
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
int32 zf_function_str_to_int (char *str)
{
    zf_function_assert(NULL != str);

    uint8 sign = 0;                                                             // 标记符号 0-正数 1-负数
    int32 temp = 0;                                                             // 临时计算变量

    do
    {
        if(NULL == str)
        {
            break;
        }

        if('-' == *str)                                                         // 如果第一个字符是负号
        {
            sign = 1;                                                           // 标记负数
            str ++;
        }
        else if('+' == *str)                                                    // 如果第一个字符是正号
        {
            str ++;
        }

        while(('0' <= *str) && ('9' >= *str))                                   // 确定这是个数字
        {
            temp = temp * 10 + ((uint8)(*str) - 0x30);                          // 计算数值
            str ++;
        }

        if(sign)
        {
            temp = -temp;
        }
    }while(0);

    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     整形数字转字符串 数据范围是 [-32768,32767]
// 参数说明     *str                字符串指针
// 参数说明     number              传入的数据
// 返回参数     void
// 使用示例     zf_function_int_to_str(data_buffer, -300);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_function_int_to_str (char *str, int32 number)
{
    zf_function_assert(NULL != str);

    uint8 data_temp[16];                                                        // 缓冲区
    uint8 bit = 0;                                                              // 数字位数
    int32 number_temp = 0;

    do
    {
        if(NULL == str)
        {
            break;
        }

        if(0 > number)                                                          // 负数
        {
            *str ++ = '-';
            number = -number;
        }
        else if(0 == number)                                                    // 或者这是个 0
        {
            *str = '0';
            break;
        }

        while(0 != number)                                                      // 循环直到数值归零
        {
            number_temp = number % 10;
            data_temp[bit ++] = zf_function_abs(number_temp);                          // 倒序将数值提取出来
            number /= 10;                                                       // 削减被提取的个位数
        }
        while(0 != bit)                                                         // 提取的数字个数递减处理
        {
            *str ++ = (data_temp[bit - 1] + 0x30);                              // 将数字从倒序数组中倒序取出 变成正序放入字符串
            bit --;
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     字符串转无符号整形数字 数据范围是 [0,65535]
// 参数说明     *str                传入字符串 无符号
// 返回参数     uint32              转换后的数据
// 使用示例     uint32 dat = zf_function_str_to_uint("100");
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_function_str_to_uint (char *str)
{
    zf_function_assert(NULL != str);

    uint32 temp = 0;                                                            // 临时计算变量

    do
    {
        if(NULL == str)
        {
            break;
        }

        while(('0' <= *str) && ('9' >= *str))                                  // 确定这是个数字
        {
            temp = temp * 10 + ((uint8)(*str) - 0x30);                         // 计算数值
            str ++;
        }
    }while(0);

    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无符号整形数字转字符串 数据范围是 [0,65535]
// 参数说明     *str                字符串指针
// 参数说明     number              传入的数据
// 返回参数     void
// 使用示例     zf_function_uint_to_str(data_buffer, 300);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_function_uint_to_str (char *str, uint32 number)
{
    zf_function_assert(NULL != str);

    int8 data_temp[16];                                                         // 缓冲区
    uint8 bit = 0;                                                              // 数字位数

    do
    {
        if(NULL == str)
        {
            break;
        }

        if(0 == number)                                                         // 这是个 0
        {
            *str = '0';
            break;
        }

        while(0 != number)                                                      // 循环直到数值归零
        {
            data_temp[bit ++] = (number % 10);                                  // 倒序将数值提取出来
            number /= 10;                                                       // 削减被提取的个位数
        }
        while(0 != bit)                                                         // 提取的数字个数递减处理
        {
            *str ++ = (data_temp[bit - 1] + 0x30);                              // 将数字从倒序数组中倒序取出 变成正序放入字符串
            bit --;
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     字符串转浮点数 有效累计精度为小数点后六位
// 参数说明     *str                传入字符串 可带符号
// 返回参数     float               转换后的数据
// 使用示例     float dat = zf_function_str_to_float("-100.2");
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
float zf_function_str_to_float (char *str)
{
    zf_function_assert(NULL != str);

    uint8 sign = 0;                                                             // 标记符号 0-正数 1-负数
    float temp = 0.0;                                                           // 临时计算变量 整数部分
    float temp_point = 0.0;                                                     // 临时计算变量 小数部分
    float point_bit = 1;                                                        // 小数累计除数

    do
    {
        if(NULL == str)
        {
            break;
        }

        if('-' == *str)                                                         // 负数
        {
            sign = 1;                                                           // 标记负数
            str ++;
        }
        else if('+' == *str)                                                    // 如果第一个字符是正号
        {
            str ++;
        }

        // 提取整数部分
        while(('0' <= *str) && ('9' >= *str))                                   // 确定这是个数字
        {
            temp = temp * 10 + ((uint8)(*str) - 0x30);                          // 将数值提取出来
            str ++;
        }
        if('.' == *str)
        {
            str ++;
            while(  ('0' <= *str)
                &&  ('9' >= *str)
                &&  (float)1000000.0 > point_bit)                               // 确认这是个数字 并且精度控制还没到六位
            {
                temp_point = temp_point * 10 + ((uint8)(*str) - 0x30);          // 提取小数部分数值
                point_bit *= 10;                                                // 计算这部分小数的除数
                str ++;
            }
            temp_point /= point_bit;                                            // 计算小数
        }
        temp += temp_point;                                                     // 将数值拼合

        if(sign)
        {
            temp = -temp;
        }
    }while(0);

    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     浮点数字转字符串
// 参数说明     *str                字符串指针
// 参数说明     number              传入的数据
// 参数说明     point_bit           小数点精度
// 返回参数     void
// 使用示例     zf_function_float_to_str(data_buffer, 3.1415, 2);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_function_float_to_str (char *str, float number, uint8 point_bit)
{
    zf_function_assert(NULL != str);

    int data_int = 0;                                                           // 整数部分
    int data_float = 0.0;                                                       // 小数部分
    int data_temp[8];                                                           // 整数字符缓冲
    int data_temp_point[6];                                                     // 小数字符缓冲
    uint8 bit = point_bit;                                                      // 转换精度位数

    do
    {
        if(NULL == str)
        {
            break;
        }

        // 提取整数部分
        data_int = (int)number;                                                 // 直接强制转换为 int
        if(0 > number)                                                          // 判断源数据是正数还是负数
        {
            *str ++ = '-';
        }
        else if((float)0.0 == number)                                           // 如果是个 0
        {
            *str ++ = '0';
            *str ++ = '.';
            *str = '0';
            break;
        }

        // 提取小数部分
        number = number - data_int;                                             // 减去整数部分即可
        while(bit --)
        {
            number = number * 10;                                               // 将需要的小数位数提取到整数部分
        }
        data_float = (int)number;                                               // 获取这部分数值

        // 整数部分转为字符串
        bit = 0;
        do
        {
            data_temp[bit ++] = data_int % 10;                                  // 将整数部分倒序写入字符缓冲区
            data_int /= 10;
        }while(0 != data_int);
        while(0 != bit)
        {
            *str ++ = (zf_function_abs(data_temp[bit - 1]) + 0x30);                    // 再倒序将倒序的数值写入字符串 得到正序数值
            bit --;
        }

        // 小数部分转为字符串
        if(0 != point_bit)
        {
            bit = 0;
            *str ++ = '.';
            if(0 == data_float)
            {
                *str = '0';
            }
            else
            {
                while(0 != point_bit)                                           // 判断有效位数
                {
                    data_temp_point[bit ++] = data_float % 10;                  // 倒序写入字符缓冲区
                    data_float /= 10;
                    point_bit --;                                                
                }
                while(0 != bit)
                {
                    *str ++ = (zf_function_abs(data_temp_point[bit - 1]) + 0x30);      // 再倒序将倒序的数值写入字符串 得到正序数值
                    bit --;
                }
            }
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     字符串转双精度浮点数 有效累计精度为小数点后九位
// 参数说明     str                 传入字符串 可带符号
// 返回参数     double              转换后的数据
// 使用示例     double dat = zf_function_str_to_double("-100.2");
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
double zf_function_str_to_double (char *str)
{
    zf_function_assert(NULL != str);

    uint8 sign = 0;                                                             // 标记符号 0-正数 1-负数
    double temp = 0.0;                                                          // 临时计算变量 整数部分
    double temp_point = 0.0;                                                    // 临时计算变量 小数部分
    double point_bit = 1;                                                       // 小数累计除数

    do
    {
        if(NULL == str)
        {
            break;
        }

        if('-' == *str)                                                         // 负数
        {
            sign = 1;                                                           // 标记负数
            str ++;
        }
        else if('+' == *str)                                                    // 如果第一个字符是正号
        {
            str ++;
        }

        // 提取整数部分
        while(('0' <= *str) && ('9' >= *str))                                   // 确定这是个数字
        {
            temp = temp * 10 + ((uint8)(*str) - 0x30);                          // 将数值提取出来
            str ++;
        }
        if('.' == *str)
        {
            str ++;
            while(('0' <= *str) && ('9' >= *str) && 1000000000.0 > point_bit)   // 确认这是个数字 并且精度控制还没到九位
            {
                temp_point = temp_point * 10 + ((uint8)(*str) - 0x30);          // 提取小数部分数值
                point_bit *= 10;                                                // 计算这部分小数的除数
                str ++;
            }
            temp_point /= point_bit;                                            // 计算小数
        }
        temp += temp_point;                                                     // 将数值拼合

        if(sign)
        {
            temp = -temp;
        }
    }while(0);

    return temp;

}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     双精度浮点数字转字符串
// 参数说明     *str                字符串指针
// 参数说明     number              传入的数据
// 参数说明     point_bit           小数点精度
// 返回参数     void
// 使用示例     zf_function_double_to_str(data_buffer, 3.1415, 2);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_function_double_to_str (char *str, double number, uint8 point_bit)
{
    zf_function_assert(NULL != str);

    int data_int = 0;                                                           // 整数部分
    int data_float = 0.0;                                                       // 小数部分
    int data_temp[12];                                                          // 整数字符缓冲
    int data_temp_point[9];                                                     // 小数字符缓冲
    uint8 bit = point_bit;                                                      // 转换精度位数

    do
    {
        if(NULL == str)
        {
            break;
        }

        // 提取整数部分
        data_int = (int)number;                                                 // 直接强制转换为 int
        if(0 > number)                                                          // 判断源数据是正数还是负数
        {
            *str ++ = '-';
        }
        else if(0.0 == number)                                                  // 如果是个 0
        {
            *str ++ = '0';
            *str ++ = '.';
            *str = '0';
            break;
        }

        // 提取小数部分
        number = number - data_int;                                             // 减去整数部分即可
        while(bit --)
        {
            number = number * 10;                                               // 将需要的小数位数提取到整数部分
        }
        data_float = (int)number;                                               // 获取这部分数值

        // 整数部分转为字符串
        bit = 0;
        do
        {
            data_temp[bit ++] = data_int % 10;                                  // 将整数部分倒序写入字符缓冲区
            data_int /= 10;
        }while(0 != data_int);
        while(0 != bit)
        {
            *str ++ = (zf_function_abs(data_temp[bit - 1]) + 0x30);                    // 再倒序将倒序的数值写入字符串 得到正序数值
            bit --;
        }

        // 小数部分转为字符串
        if(0 != point_bit)
        {
            bit = 0;
            *str ++ = '.';
            if(0 == data_float)
            {
                *str = '0';
            }
            else
            {
                while(0 != point_bit)                                           // 判断有效位数
                {
                    data_temp_point[bit ++] = data_float % 10;                  // 倒序写入字符缓冲区
                    data_float /= 10;
                    point_bit --;                                                
                }
                while(0 != bit)
                {
                    *str ++ = (zf_function_abs(data_temp_point[bit - 1]) + 0x30);      // 再倒序将倒序的数值写入字符串 得到正序数值
                    bit --;
                }
            }
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     字符串转 Hex
// 参数说明     str                 传入字符串 无符号
// 返回参数     uint32              转换后的数据
// 使用示例     uint32 dat = zf_function_str_to_hex("0x11");
// 备注信息     必须要 0x 开头才行
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_function_str_to_hex (char *str)
{
    zf_function_assert(NULL != str);

    uint32 str_len = strlen(str);                                               // 字符串长
    uint32 result_data = 0;                                                     // 结果缓存
    uint8 temp = 0;                                                             // 计算变量
    uint8 flag = 0;                                                             // 标志位

    do
    {
        if(NULL == str)
        {
            break;
        }

        if(flag)
        {
            if(('a' <= *str) && ('f' >= *str))
            {
                temp = (*str - 87);
            }
            else if(('A' <= *str) && ('F' >= *str))
            {
                temp = (*str - 55);
            }
            else if(('0' <= *str) && ('9' >= *str))
            {
                temp = (*str - 48);
            }
            else
            {
                break;
            }
            result_data = ((result_data << 4) | (temp & 0x0F));
        }
        else
        {
//            if(strncmp("0x", str, 2))
            if(('0' == *str) && ('x' == *(str + 1)))
            {
                str ++;
                flag = 1;
            }
        }
        str ++;
    }while(str_len --);

    return result_data;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     Hex 转字符串
// 参数说明     *str                字符串指针
// 参数说明     number              传入的数据
// 返回参数     void
// 使用示例     zf_function_hex_to_str(data_buffer, 0x11);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void zf_function_hex_to_str (char *str, uint32 number)
{
    zf_function_assert(NULL != str);

    const char hex_index[16] = {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'A', 'B',
        'C', 'D', 'E', 'F'};
    int8 data_temp[12];                                                         // 缓冲区
    uint8 bit = 0;                                                              // 数字位数

    *str ++ = '0';
    *str ++ = 'x';
    do
    {
        if(NULL == str)
        {
            break;
        }

        if(0 == number)                                                         // 这是个 0
        {
            *str = '0';
            break;
        }

        while(0 != number)                                                      // 循环直到数值归零
        {
            data_temp[bit ++] = (number & 0xF);                                 // 倒序将数值提取出来
            number >>= 4;                                                       // 削减被提取的个位数
        }
        while(0 != bit)                                                         // 提取的数字个数递减处理
        {
            *str ++ = hex_index[data_temp[bit - 1]];                            // 将数字从倒序数组中倒序取出 变成正序放入字符串
            bit --;
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     数字转换为 ASCII 值
// 参数说明     dat             传入的数据
// 参数说明     *p              数据缓冲
// 参数说明     neg_type        数据类型
// 参数说明     radix           进制
// 返回参数     uint8           数据
// 使用示例     number_conversion_ascii((uint32)ival, vstr, 1, 10);
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
static uint8 number_conversion_ascii (uint32 dat, int8 *p, uint8 neg_type, uint8 radix)
{
    int32   neg_dat = 0;
    uint32  pos_dat = 0;
    uint8   temp_data = 0;
    uint8   valid_num = 0;

    if(neg_type)
    {
        neg_dat = (int32)dat;
        if(0 > neg_dat)
        {
            neg_dat = -neg_dat;
        }
        while(1)
        {
            *p = neg_dat % radix + '0';
            neg_dat = neg_dat / radix;
            valid_num ++;

            if(!neg_dat)
            {
                break;
            }
            p ++;
        }
    }
    else
    {
        pos_dat = dat;
        while(1)
        {
            temp_data = pos_dat % radix;
            if(10 <= temp_data)
            {
                temp_data += 'A' - 10;
            }
            else
            {
                temp_data += '0';
            }

            *p = temp_data;

            pos_dat = pos_dat / radix;
            valid_num ++;

            if(!pos_dat)
            {
                break;
            }
            p ++;
        }
    }

    return valid_num;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     printf 显示转换
// 参数说明     *d_buff         缓冲区
// 参数说明     len             长度
// 返回参数     void
// 使用示例     printf_reverse_order(vstr, vlen);
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
static void printf_reverse_order (int8 *d_buff, uint32 len)
{
    uint32 i = 0;
    int8  temp_data = 0;

    for(i = 0; len / 2 > i; i ++)
    {
        temp_data = d_buff[len - 1 - i];
        d_buff[len - 1 - i] = d_buff[i];
        d_buff[i] = temp_data; 
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     zf_sprintf 函数实现
// 参数说明     *buff               缓冲区
// 参数说明     *format             源字符串
// 参数说明     ...                 可变参数列表
// 返回参数     uint32              处理后数据长
// 使用示例     zf_sprintf(buff, "Data : %d", 100);
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_sprintf (int8 *buff, const int8 *format, ...)
{
    uint32 buff_len = 0;
    va_list arg;
    va_start(arg, format);

    while (*format)
    {
        int8 ret = *format;
        if ('%' == ret)
        {
            switch (*++ format)
            {
                case 'a':// 十六进制p计数法输出浮点数 暂未实现
                {
                }break;

                case 'c':// 一个字符
                {
                    int8 ch = (int8)va_arg(arg, uint32);
                    *buff = ch;
                    buff ++;
                    buff_len ++;
                }break;

                case 'd':
                case 'i':// 有符号十进制整数
                {
                    int8 vstr[33];
                    int32 ival = (int32)va_arg(arg, int32);
                    uint8 vlen = number_conversion_ascii((uint32)ival, vstr, 1, 10);

                    if(0 > ival)  
                    {
                        vstr[vlen] = '-';
                        vlen ++;
                    }
                    printf_reverse_order(vstr, vlen);
                    memcpy(buff, vstr, vlen);
                    buff += vlen;
                    buff_len += vlen;
                }break;

                case 'f':// 浮点数，输出小数点后六位  不能指定输出精度
                case 'F':// 浮点数，输出小数点后六位  不能指定输出精度
                {
                    int8 vstr[33];
                    double ival = (double)va_arg(arg, double);
                    uint8 vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);

                    if(0 > ival)  
                    {
                        vstr[vlen] = '-';
                        vlen ++;
                    }
                    printf_reverse_order(vstr, vlen);
                    memcpy(buff, vstr, vlen);
                    buff += vlen;
                    buff_len += vlen;

                    ival = ((double)ival - (int32)ival) * 1000000;
                    if(ival)
                    {
                        vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);
                    }
                    else
                    {
                        vstr[0] = vstr[1] = vstr[2] = vstr[3] = vstr[4] = vstr[5] = '0';
                        vlen = 6;
                    }

                    while(6 > vlen)
                    {
                        vstr[vlen] = '0';
                        vlen ++;
                    }

                    vstr[vlen] = '.';
                    vlen ++;

                    printf_reverse_order(vstr, vlen);
                    memcpy(buff, vstr, vlen);
                    buff_len += vlen;
                }break;

                case 'u':// 无符号十进制整数
                {
                    int8 vstr[33];
                    uint32 ival = (uint32)va_arg(arg, uint32);
                    uint8 vlen = number_conversion_ascii(ival, vstr, 0, 10);

                    printf_reverse_order(vstr, vlen);
                    memcpy(buff, vstr, vlen);
                    buff += vlen;
                    buff_len += vlen;
                }break;

                case 'o':// 无符号八进制整数 
                {
                    int8 vstr[33];
                    uint32 ival = (uint32)va_arg(arg, uint32);
                    uint8 vlen = number_conversion_ascii(ival, vstr, 0, 8);

                    printf_reverse_order(vstr, vlen);
                    memcpy(buff, vstr, vlen);
                    buff += vlen;
                    buff_len += vlen;

                }break;

                case 'x':// 无符号十六进制整数
                case 'X':// 无符号十六进制整数
                {
                    int8 vstr[33];
                    uint32 ival = (uint32)va_arg(arg, uint32);
                    uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);

                    printf_reverse_order(vstr, vlen);
                    memcpy(buff, vstr, vlen);
                    buff += vlen;
                    buff_len += vlen;
                }break;

                case 's':// 字符串
                {
                    int8 *pc = va_arg(arg, int8 *);
                    while (*pc)
                    {
                        *buff = *pc;
                        buff ++;
                        buff_len ++;
                        pc ++;
                    }
                }break;

                case 'p':// 以16进制形式输出指针
                {
                    int8 vstr[33];
//                    uint32 ival = (uint32)va_arg(arg, uint32);
//                    uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);

                    printf_reverse_order(vstr, 8);
                    memcpy(buff, vstr, 8);
                    buff += 8;
                    buff_len += 8;
                }break;

                case '%':// 输出字符% 
                {
                    *buff = '%';
                    buff ++;
                    buff_len ++;
                }break;

                default:
                {
                }break;
            }
        }
        else
        {
            *buff = (int8)(*format);
            buff ++;
            buff_len ++;
        }
        format ++;
    }
    va_end(arg);

    return buff_len;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
