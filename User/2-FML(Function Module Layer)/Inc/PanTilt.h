/**
 * @file    PanTilt.h
 * @brief   云台驱动
 *
 * @author  Li-Chenyang
 * @date    2024-2-28 （创建）
 *
 */

#ifndef __FML_PANTILT_H
#define __FML_PANTILT_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Motor_DM.h"
#include "Motor_Steer.h"
#include "Remote_Control.h"
#include "Chassis.h"
#include "cstring"

/* 宏定义 -------------------------------------------------------------------------------------------------------------*/
#define PANTILT_MOTOR_YAW_DM_CAN_ID             0x07
#define PANTILT_MOTOR_PITCH_DM_CAN_ID           0x08

#define PANTILT_MOTOR_YAW_DM_MASTER_ID          0x17
#define PANTILT_MOTOR_PITCH_DM_MASTER_ID        0x18

/* 枚举类型定义 --------------------------------------------------------------------------------------------------------*/
/**
 * @brief 云台状态枚举类型
 */
enum Enum_PanTilt_Status
{
    PanTilt_Status_Die = 0,
    PanTilt_Status_Forward,
    PanTilt_Status_Backward,
};

/* 类定义 -------------------------------------------------------------------------------------------------------------*/
/**
 * @brief 云台类
 */
class Class_PanTilt
{
public:
    /* 变量 */
    Class_Motor_DM_J4310 PanTilt_Motor_Yaw;
    Class_Motor_DM_J4310 PanTilt_Motor_Pitch;
    Class_DR16 * DR16;

    /* 函数 */
    Class_PanTilt(Class_DR16 * __DR16);
    void Init();
    void DataGet_Remote();
    void AliveCheck();
    void Control();
    void Control_Die();
    void Control_Forward();
    void Control_Backward();
    inline Enum_PanTilt_Status Get_Status();

protected:
    /* 常量 */
    float Angle_Lift_Limit = 6.0f;
    float Angle_Pitch_Min = -0.8f;
    float Angle_Pitch_Max = 1.15f;

    /* 读写变量 */
    float Angle_Yaw;
    float Angle_Pitch;

    /* 内部变量 */
    uint8_t Control_Counter = 0;        /*!< 电机循环控制计数 */
    uint16_t Err_Clear_Counter = 0;      /*!< 电机错误清除计数 */
    Enum_PanTilt_Status PanTilt_Status = PanTilt_Status_Die;
};

Enum_PanTilt_Status Class_PanTilt::Get_Status()
{
    return this->PanTilt_Status;
}

/* 变量声明 ------------------------------------------------------------------------------------------------------------*/
extern Class_PanTilt Engineer_PanTilt;


#endif /* FML_PanTilt */
