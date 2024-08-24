/**
 * @file    Callback_Usart.h
 * @brief   USART回调函数重写
 *
 * @author  Li-Chenyang
 * @date    2024-1-19 （创建）
 *
 */

#ifndef __APL_CALLBACK_USART_H
#define __APL_CALLBACK_USART_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "usbd_cdc_if.h"
#include "cstring"

#include "Referee_System.h"
#include "Remote_Control.h"
#include "Ahrs_WIT.h"
#include "Serialplot.h"
#include "Chassis.h"
#include "RobotArm.h"
#include "PanTilt.h"
#include "User_Math.h"

/* 变量声明 ------------------------------------------------------------------------------------------------------------*/
extern "C" {
void UART_USB_RxEventCallback(uint16_t Size);
};

#endif /* APL_Callback_Usart */