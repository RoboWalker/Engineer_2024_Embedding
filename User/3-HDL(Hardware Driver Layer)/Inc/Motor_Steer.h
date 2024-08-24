/**
 * @file    Motor_Steer.h
 * @brief   舵机驱动
 *
 * @author  Li-Chenyang
 * @date    2024-4-19 （创建）
 *
 */

#ifndef __MOTOR_STEER_H
#define __MOTOR_STEER_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "tim.h"
#include "User_Math.h"

/* 类定义 -------------------------------------------------------------------------------------------------------------*/
/**
 * @brief 舵机
 */
class Class_Motor_Steer
{
public:
    /* 函数 */
    void Init(TIM_HandleTypeDef htim, uint32_t Channel, uint32_t Counter_Max, float Motor_Angle_max,
              float Angle_Min, float Angle_Max, float Angle_Init);
    void Control();

    inline void Set_Angle(float __Target_Angle);
    inline float Get_Angle();
protected:
    /* 常量 */
    TIM_HandleTypeDef htim;
    uint32_t Channel;
    uint32_t Counter_Max;
    float Motor_Angle_Max;
    float Angle_Max;
    float Angle_Min;

    /* 读写变量 */
    float Target_Angle;
};


/* 接口函数定义 ---------------------------------------------------------------------------------------------------------*/
/**
 * @brief 设置目标角度
 */
void Class_Motor_Steer::Set_Angle(float __Target_Angle)
{
    this->Target_Angle = __Target_Angle;
}

/**
 * @brief 读取目标角度
 */
float Class_Motor_Steer::Get_Angle()
{
    return this->Target_Angle;
}


extern Class_Motor_Steer Motor_Steer_test;

#endif /* HDL_Motor_Steer */
