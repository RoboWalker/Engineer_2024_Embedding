/**
 * @file    User_Can.h
 * @brief   CAN外设进一步封装
 *
 * @author  Li-Chenyang
 * @date    2024-1-18 （创建）
 *
 */

#ifndef __HAL_USER_CAN_H
#define __HAL_USER_CAN_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "can.h"

/* 宏定义 -------------------------------------------------------------------------------------------------------------*/
/* 滤波器编号 */
#define CAN_FILTER(x)       ((x) << 3)

/* 接收队列 */
#define CAN_FIFO_0          (0 << 2)
#define CAN_FIFO_1          (1 << 2)

/* 标准帧或扩展帧 */
#define CAN_STDID           (0 << 1)
#define CAN_EXTID           (1 << 1)

/* 数据帧或遥控帧 */
#define CAN_DATA_TYPE       (0 << 0)
#define CAN_REMOTE_TYPE     (1 << 0)

/* 结构体定义 ----------------------------------------------------------------------------------------------------------*/
/**
 * @brief CAN-RX数据结构体
 */
struct Struct_CAN_Rx_Buffer
{
    CAN_RxHeaderTypeDef Header;         /*!< CAN-RX包头 */
    uint8_t Data[8];                    /*!< CAN-RX包数据 */
};

/**
 * @brief CAN处理结构体
 */
struct Struct_CAN_Manage_Object
{
    /* 常量部分 */
    CAN_HandleTypeDef * hcan;           /*!< CAN外设句柄 */

    /* 变量部分 */
    Struct_CAN_Rx_Buffer Rx_Buffer;     /*!< CAN-RX缓冲区*/
};

/* 变量声明 ------------------------------------------------------------------------------------------------------------*/
extern Struct_CAN_Manage_Object CAN1_Manage_Object;
extern Struct_CAN_Manage_Object CAN2_Manage_Object;

/* 函数声明 ------------------------------------------------------------------------------------------------------------*/
void CAN_Init(Struct_CAN_Manage_Object * CAN_Manage_Obj);
uint8_t CAN_Send_Data(Struct_CAN_Manage_Object * CAN_Manage_Obj, uint16_t ID, uint8_t * Data, uint16_t Length);
void CAN_Receive_Data(Struct_CAN_Manage_Object * CAN_Manage_Obj, uint32_t RxFifo);
void CAN_ConfigFilter(CAN_HandleTypeDef * hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID);

#endif /* HAL_User_Can */
