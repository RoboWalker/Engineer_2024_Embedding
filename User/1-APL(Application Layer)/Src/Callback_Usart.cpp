/**
 * @file    Callback_Usart.cpp
 * @brief   USART回调函数重写
 *
 * @author  Li-Chenyang
 * @date    2024-1-19 （创建）
 *
 */

#pragma GCC push_options
#pragma GCC optimize("O0")

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Callback_Usart.h"

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief UART-RX事件中断回调函数重写
 *
 * @param huart  UART外设句柄
 * @param Size   当前接收长度
 **********************************************************************************************************************/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef * huart, uint16_t Size)
{
    if (huart->Instance == huart1.Instance)
    {
//        float X, Y, Omega;
        /* 获取遥控器数据 */
        Remote_Control.DataGet(UART1_Manage_Object.Rx_Buffer);

        /* 根据遥控器设置底盘运动数据 */
        Engineer_Chassis.DataGet_Remote();

        Engineer_RobotArm.DataGet_Remote();

        Engineer_PanTilt.DataGet_Remote();

        /* 开启新一次串口接收（DMA-IDLE） */
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer, UART1_Manage_Object.Rx_Data_Size);
    }
    else if (huart->Instance == huart2.Instance)
    {
        /* 处理串口绘图数据 */
        Serialplot.UART_RxCpltCallback(UART2_Manage_Object.Rx_Buffer, Size);

        /* 开启新一次串口接收（DMA-IDLE） */
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer, UART2_Manage_Object.Rx_Data_Size);
    }
    else if (huart->Instance == huart6.Instance)
    {
        /* 处理裁判系统数据 */
        Referee_System.Referee_data_processing(UART6_Manage_Object.Rx_Buffer, Size);

        /* 开启新一次串口接收（DMA-IDLE） */
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer, UART6_Manage_Object.Rx_Data_Size);
    }
    else if (huart->Instance == huart8.Instance)
    {
        /* 处理陀螺仪数据 */
        AHRS_Chassis.UART_RxCpltCallback(UART8_Manage_Object.Rx_Buffer, Size);

        /* 开启新一次串口接收（DMA-IDLE） */
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART8_Manage_Object.Rx_Buffer, UART8_Manage_Object.Rx_Data_Size);
    }
}

/***********************************************************************************************************************
 * @brief UART-USB回调函数重写
 *
 * @param Size   当前接收长度
 **********************************************************************************************************************/
void UART_USB_RxEventCallback(uint16_t Size)
{
    static float Motors_Angle[6];
    uint16_t lift_angle_tmp_uint;
    float lift_angle_tmp;

    /* 从上位机获取数据 */
    if (UART_USB_RX_Buffer[0] == 0xAB)
    {
//        /* 机械臂电机角度，共24字节，float */
//        memcpy((void *)Motors_Angle, (const void *)(UART_USB_RX_Buffer + 1), 24);
//        Engineer_RobotArm.Set_Motors_Angle(Motors_Angle);
//
//        /* 气泵状态，1字节 */
//        if (UART_USB_RX_Buffer[25] & 0x01)
//        {
//            Engineer_RobotArm.Set_Pump_Status(Pump_Status_ON);
//        }
//        else
//        {
//            Engineer_RobotArm.Set_Pump_Status(Pump_Status_OFF);
//        }

        Engineer_RobotArm.DataGet_USB(UART_USB_RX_Buffer);

        if (UART_USB_RX_Buffer[25] & (0x01 << 1))
        {
            Engineer_Chassis.Adjust_Lift_Front_Angle(0.09f);
        }

        if (UART_USB_RX_Buffer[25] & (0x01 << 2))
        {
            Engineer_Chassis.Adjust_Lift_Front_Angle(-0.09f);
        }

        if (Engineer_Chassis.Get_Store_Status() != Store_Status_NotClear)
        {
            if (UART_USB_RX_Buffer[25] & (0x01 << 3))
            {
                Engineer_Chassis.Set_Store_Status(Store_Status_Close);
            }
            else
            {
                Engineer_Chassis.Set_Store_Status(Store_Status_Open);
            }
        }

        if (!UART_USB_RX_Buffer[26] && !UART_USB_RX_Buffer[27])
        {

        }

        /* 机械臂升降电机角度 */
        lift_angle_tmp_uint = (uint16_t)((UART_USB_RX_Buffer[27] << 8) | UART_USB_RX_Buffer[28]);
        lift_angle_tmp = Math_Uint_To_Float((uint32_t)lift_angle_tmp_uint, 0.0f, 15.0f, 16);
    }
}

/**
 * @brief HAL库UART错误中断
 *
 * @param huart UART编号
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer, UART1_Manage_Object.Rx_Data_Size);
    }
    else if (huart->Instance == USART2)
    {
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer, UART2_Manage_Object.Rx_Data_Size);
    }
    else if (huart->Instance == USART6)
    {
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer, UART6_Manage_Object.Rx_Data_Size);
    }
    else if (huart->Instance == UART8)
    {
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART8_Manage_Object.Rx_Buffer, UART8_Manage_Object.Rx_Data_Size);
    }
}

#pragma GCC pop_options
