/**
 * @file    User_Usart.h
 * @brief   USART外设进一步封装
 *
 * @author  Li-Chenyang
 * @date    2024-1-19 （创建）
 *
 */

#ifndef __HAL_USER_USART_H
#define __HAL_USER_USART_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "usart.h"

/* 宏定义 -------------------------------------------------------------------------------------------------------------*/
#define UART_TX_BUFFER_SIZE            256         // 串口TX缓冲区字节长度
#define UART_RX_BUFFER_SIZE            512         // 串口RX缓冲区字节长度

/* 结构体定义 ----------------------------------------------------------------------------------------------------------*/
/**
 * @brief UART处理结构体
 */
struct Struct_UART_Manage_Object
{
    UART_HandleTypeDef * huart;
    uint8_t Tx_Buffer[UART_TX_BUFFER_SIZE];
    uint8_t Rx_Buffer[UART_RX_BUFFER_SIZE];
    uint16_t Rx_Data_Size;
};

/* 变量声明 ------------------------------------------------------------------------------------------------------------*/
extern Struct_UART_Manage_Object UART1_Manage_Object;
extern Struct_UART_Manage_Object UART2_Manage_Object;
extern Struct_UART_Manage_Object UART6_Manage_Object;
extern Struct_UART_Manage_Object UART7_Manage_Object;
extern Struct_UART_Manage_Object UART8_Manage_Object;

/* 函数声明 ------------------------------------------------------------------------------------------------------------*/
void UART_Init(Struct_UART_Manage_Object * UART_Mangae_Obj, uint16_t Rx_Data_Size);
uint8_t UART_Send_Data(Struct_UART_Manage_Object * UART_Mangae_Obj, uint8_t * Data, uint16_t Length);

#endif /* HAL_User_Usart */
