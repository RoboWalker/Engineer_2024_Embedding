/**
 * @file    User_Usart.cpp
 * @brief   USART外设进一步封装
 *
 * @author  Li-Chenyang
 * @date    2024-1-19 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "User_Usart.h"

/* 全局变量 -----------------------------------------------------------------------------------------------------------*/
Struct_UART_Manage_Object UART1_Manage_Object = {&huart1};
Struct_UART_Manage_Object UART2_Manage_Object = {&huart2};
Struct_UART_Manage_Object UART6_Manage_Object = {&huart6};
Struct_UART_Manage_Object UART7_Manage_Object = {&huart7};
Struct_UART_Manage_Object UART8_Manage_Object = {&huart8};

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief UART初始化
 *
 * @param UART_Mangae_Obj      UART处理结构体指针
 **********************************************************************************************************************/
void UART_Init(Struct_UART_Manage_Object * UART_Mangae_Obj, uint16_t Rx_Data_Size)
{
    /* RX接收数据长度赋值 */
    UART_Mangae_Obj->Rx_Data_Size = Rx_Data_Size;
    /* 开启串口接收（DMA-IDLE） */
    HAL_UARTEx_ReceiveToIdle_DMA(UART_Mangae_Obj->huart, UART_Mangae_Obj->Rx_Buffer, UART_Mangae_Obj->Rx_Data_Size);
}

/***********************************************************************************************************************
 * @brief UART发送数据
 *
 * @param UART_Manage_Obj       UART处理结构体指针
 * @param Data                  数据
 * @param Length                数据长度
 * @return uint8_t              执行结果
 **********************************************************************************************************************/
uint8_t UART_Send_Data(Struct_UART_Manage_Object * UART_Mangae_Obj, uint8_t * Data, uint16_t Length)
{
    return (HAL_UART_Transmit_DMA(UART_Mangae_Obj->huart, Data, Length));
}
