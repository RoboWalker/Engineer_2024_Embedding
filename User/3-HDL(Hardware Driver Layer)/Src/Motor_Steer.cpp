/**
 * @file    Motor_Steer.cpp
 * @brief   舵机驱动
 *
 * @author  Li-Chenyang
 * @date    2024-4-19 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Motor_Steer.h"

/* 全局变量创建 --------------------------------------------------------------------------------------------------------*/
Class_Motor_Steer Motor_Steer_test;

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief 舵机初始化
 *
 * @param htim                          定时器结构体
 * @param Channel                       PWM输出通道
 * @param Counter_Max                   定时器重装载值
 * @param Angle_max                     舵机最大角度
 **********************************************************************************************************************/
void Class_Motor_Steer::Init(TIM_HandleTypeDef htim, uint32_t Channel, uint32_t Counter_Max, float Motor_Angle_Max,
                             float Angle_Min, float Angle_Max, float Angle_Init)
{
    this->htim = htim;
    this->Channel = Channel;
    this->Counter_Max = Counter_Max;
    this->Motor_Angle_Max = Motor_Angle_Max;
    this->Angle_Min = Angle_Min;
    this->Angle_Max = Angle_Max;
    this->Target_Angle = Angle_Init;

    Math_Constrain(&this->Target_Angle, this->Angle_Min, this->Angle_Max);

    __HAL_TIM_SET_COMPARE(&this->htim, this->Channel, (this->Target_Angle / this->Motor_Angle_Max * 2.0f + 0.5f) * this->Counter_Max / 20.0f);
    HAL_TIM_PWM_Start(&htim, Channel);
}

void Class_Motor_Steer::Control()
{
    Math_Constrain(&this->Target_Angle, this->Angle_Min, this->Angle_Max);
    __HAL_TIM_SET_COMPARE(&this->htim, this->Channel, (this->Target_Angle / this->Motor_Angle_Max * 2.0f + 0.5f) * this->Counter_Max / 20.0f);
}

