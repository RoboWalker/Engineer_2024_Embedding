/**
 * @file    Board.cpp
 * @brief   板载资源相关
 *
 * @author  Li-Chenyang
 * @date    2024-1-17 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Board.h"

Class_Board_DJI_A Board;

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief 大疆A板初始化
 **********************************************************************************************************************/
void Class_Board_DJI_A::Init()
{
    return;
}

/***********************************************************************************************************************
 * @brief 大疆A板流水灯控制
 *
 * @param number        控制LED序号
 **********************************************************************************************************************/
void Class_Board_DJI_A::LED_Water_Control(uint8_t number)
{
    switch (number) {
        case 0:
            HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
            break;
        case 1:
            HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
            break;
        case 2:
            HAL_GPIO_TogglePin(LED_3_GPIO_Port, LED_3_Pin);
            break;
        case 3:
            HAL_GPIO_TogglePin(LED_4_GPIO_Port, LED_4_Pin);
            break;
        case 4:
            HAL_GPIO_TogglePin(LED_5_GPIO_Port, LED_5_Pin);
            break;
        case 5:
            HAL_GPIO_TogglePin(LED_6_GPIO_Port, LED_6_Pin);
            break;
        case 6:
            HAL_GPIO_TogglePin(LED_7_GPIO_Port, LED_7_Pin);
            break;
        case 7:
            HAL_GPIO_TogglePin(LED_8_GPIO_Port, LED_8_Pin);
            break;
    }
}

/***********************************************************************************************************************
 * @brief 大疆A板流水灯停止
 **********************************************************************************************************************/
void Class_Board_DJI_A::LED_Water_Stop()
{
    HAL_GPIO_WritePin(LED_1_GPIO_Port,
                      LED_1_Pin | LED_2_Pin | LED_3_Pin | LED_4_Pin | LED_5_Pin | LED_6_Pin | LED_7_Pin | LED_8_Pin,
                      GPIO_PIN_SET);
}

