/**
 * @file    Board.h
 * @brief   板载资源相关
 *
 * @author  Li-Chenyang
 * @date    2024-1-17 （创建）
 *
 */

#ifndef __HDL_BOARD_H
#define __HDL_BOARD_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"
#include "tim.h"

/* 类定义 -------------------------------------------------------------------------------------------------------------*/
/**
 * @brief DJI_A开发板类
 */
class Class_Board_DJI_A
{
public:
    void Init();
    void LED_Water_Control(uint8_t number);
    void LED_Water_Stop();
private:

};

/* 变量声明 ------------------------------------------------------------------------------------------------------------*/
extern Class_Board_DJI_A Board;

#endif /* HDL_Board */
