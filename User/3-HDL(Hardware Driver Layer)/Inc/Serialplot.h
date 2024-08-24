/**
 * @file dvc_motor.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 串口绘图
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

#ifndef DVC_SERIALPLOT_H
#define DVC_SERIALPLOT_H

/* Includes ------------------------------------------------------------------*/

#include "User_Usart.h"
#include "User_Math.h"

#include <cstdarg>
#include <cstring>

/* Exported macros -----------------------------------------------------------*/

// 串口绘图单条指令最大长度
#define SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH (100)

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 串口绘图传输数据类型
 *
 */
enum Enum_Serialplot_Data_Type
{
    Serialplot_Data_Type_UINT8 = 0,
    Serialplot_Data_Type_UINT16,
    Serialplot_Data_Type_UINT32,
    Serialplot_Data_Type_INT8,
    Serialplot_Data_Type_INT16,
    Serialplot_Data_Type_INT32,
    Serialplot_Data_Type_FLOAT,
    Serialplot_Data_Type_DOUBLE,
};

/**
 * @brief Specialized, 串口绘图工具, 最多支持12个通道
 *
 */
class Class_Serialplot
{
public:
    void Init(UART_HandleTypeDef *huart, uint8_t __Serialplot_Rx_Variable_Assignment_Num = 0, char **__Serialplot_Rx_Variable_Assignment_List = NULL, Enum_Serialplot_Data_Type __Serialplot_Data_Type = Serialplot_Data_Type_FLOAT, uint8_t __Frame_Header = 0xab);

    inline int8_t Get_Variable_Index();

    inline float Get_Variable_Value();

    inline void Set_Data(uint8_t Number, ...);

    void UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length);

    void TIM_1ms_Write_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;
    // 接收指令字典的数量
    uint8_t UART_Rx_Variable_Num;
    // 接收指令字典列表指针
    char **UART_Rx_Variable_List;
    // 串口绘图数据类型
    Enum_Serialplot_Data_Type UART_Tx_Data_Type;
    // 数据包头标
    uint8_t Frame_Header;

    // 常量

    // 内部变量

    // 需要绘图的各个变量数据地址
    const void *Data[12];
    // 当前发送的数据长度, 等价于新数据偏移量
    uint8_t Data_Number = 0;
    // 当前接收的指令在指令字典中的编号
    int8_t Variable_Index = 0;
    // 当前接收的指令在指令字典中的值
    float Variable_Value = 0.0f;

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数

    void Data_Process(uint16_t Length);

    uint8_t _Judge_Variable_Name();

    void _Judge_Variable_Value(int flag);

    void Output();
};

/* Exported variables --------------------------------------------------------*/

extern Class_Serialplot Serialplot;

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取当前接收的指令在指令字典中的编号
 *
 * @return int8_t 当前接收的指令在指令字典中的编号
 */
inline int8_t Class_Serialplot::Get_Variable_Index()
{
    return (Variable_Index);
}

/**
 * @brief 获取当前接收的指令在指令字典中的值
 *
 * @return float 当前接收的指令在指令字典中的值
 */
inline float Class_Serialplot::Get_Variable_Value()
{
    return (Variable_Value);
}

/**
 * @brief 添加被发送的数据
 *
 * @param Number 添加的数据数量
 * @param ... 每个数据的指针, 至多12个
 */
inline void Class_Serialplot::Set_Data(uint8_t Number, ...)
{
    va_list data_ptr;
    va_start(data_ptr, Number);
    for (int i = 0; i < Number; i++)
    {
        Data[i] = (void *) va_arg(data_ptr, int);
    }
    va_end(data_ptr);
    Data_Number = Number;
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
