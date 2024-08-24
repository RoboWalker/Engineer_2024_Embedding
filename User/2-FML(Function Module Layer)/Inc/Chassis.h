/**
 * @file    Chassis.h
 * @brief   麦克纳姆轮底盘（包含升降机构）驱动
 *
 * @author  Li-Chenyang
 * @date    2024-1-18 （创建）
 *
 */

#ifndef __FML_CHASSIS_H
#define __FML_CHASSIS_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Motor_DJI.h"
#include "Remote_Control.h"
#include "Ahrs_WIT.h"

/* 结构体定义 ----------------------------------------------------------------------------------------------------------*/

/* 枚举类型定义 --------------------------------------------------------------------------------------------------------*/
enum Enum_Chassis_Status
{
    Chassis_Status_DISABLE = 0,
    Chassis_Status_ENABLE,
};

enum Enum_Lift_Status
{
    Lift_Status_Bottom = 0,
    Lift_Status_Middle,
    Lift_Status_Top,
};

enum Enum_Store_Status
{
    Store_Status_NotClear = 0,
    Store_Status_Close,
    Store_Status_Open,
};

/* 类定义 -------------------------------------------------------------------------------------------------------------*/
/**
 * @brief 底盘类
 */
class Class_Chassis
{
public:
    /* 变量 */
    Class_Motor_DJI_C620 Chassis_Motor[4];          /*!< 底盘电机 DJI-3508 */
    Class_Motor_DJI_C620 Lift_Motor[2];             /*!< 抬升电机 DJI-3508 */
    Class_Motor_DJI_C610 Store_Motor;               /*!< 矿仓电机 DJI-2006 */
    Class_DR16 * DR16;                              /*!< DR16遥控器接收机 */
    Class_WIT_AHRS * AHRS;                           /*!< 陀螺仪 */
    Class_PID PID_Chassis_Omega;                    /*!< 底盘角速度PID */
    Class_PID PID_Omega_Revise;                     /*!< 底盘角度修正PID（前置） */

    /* 函数 */
    Class_Chassis(Class_DR16 * __DR16, Class_WIT_AHRS * __AHRS);
    void Init();
    void AliveCheck();
    void DataGet_Remote();
    void Control();

    inline Enum_Chassis_Status Get_Chassis_Status();
    inline Enum_Lift_Status Get_Lift_Front_Status();
    inline Enum_Lift_Status Get_Lift_Back_Status();
    inline float Get_Lift_Front_Angle();
    inline float Get_Lift_Back_Angle();
    inline void Set_Velocity(float Velocity_X, float Velocity_Y, float Omega);
    inline void Set_Lift_Omega(float Omega_Front, float Omega_Back);
    inline void Set_Lift_Front_Angle(float Angle);
    inline void Set_Lift_Back_Angle(float Angle);
    inline void Adjust_Lift_Front_Angle(float __Angle);
    inline void Adjust_Lift_Back_Angle(float __Angle);
    inline void Set_Forward_Status(bool Status);
    inline void Set_Wheel_Motor_Tx_Out();
    inline Enum_Store_Status Get_Store_Status();
    inline void Set_Store_Status(Enum_Store_Status __Store_Status);
    void PowerLimit();
    void OmegaCorrection();

    /* 常量 */
    const float Radius_Wheel = 0.076f;            /*!< 麦克纳姆轮半径 */
    const float WheelTrack_X = 0.36f;             /*!< 轮距-前后 */
    const float WheelTrack_Y = 0.345f;            /*!< 轮距-左右 */

    const float Angle_Lift_Front_Min = 0.2f;
    const float Angle_Lift_Back_Min = 7.0f;
    const float Angle_Lift_Front_Max = 14.5f;  /*!< 抬升电机最大活动角度 (rad) */
    const float Angle_Lift_Back_Max = 26.5f;   /*!< 抬升电机最大活动角度 (rad) */

 protected:
    /* 读写变量 */
    float Target_Velocity_X;           /*!< 底盘前后平移速度，向前为正 (m/s) */
    float Target_Velocity_Y;           /*!< 底盘左右平移速度，向左为正 (m/s) */
    float Target_Omega;                /*!< 底盘旋转速度，逆时针为正 (rad/s) */
    float Velocity_X;           /*!< 底盘前后平移速度，向前为正 (m/s) */
    float Velocity_Y;           /*!< 底盘左右平移速度，向左为正 (m/s) */
    float Omega;                /*!< 底盘旋转速度，逆时针为正 (rad/s) */
    float Omega_Lift_Front;     /*!< 机械臂（前）升降电机速度 (rad/s) */
    float Omega_Lift_Back;      /*!< 云台（后）升降电机速度 (rad/s) */
    float Angle_Lift_Front;     /*!< 机械臂（前）升降电机角度 (rad) */
    float Angle_Lift_Back;      /*!< 云台（后）升降电机角度 (rad) */
    bool Forward_Status = true;
    Enum_Store_Status Store_Status = Store_Status_NotClear;

    /* 内部变量 */
    uint32_t Reset_Flag = 0;
    Enum_Chassis_Status Chassis_Status = Chassis_Status_DISABLE;
    Enum_Lift_Status Lift_Front_Status = Lift_Status_Bottom;
    Enum_Lift_Status Lift_Back_Status = Lift_Status_Bottom;
    float Power_Limit_Max = 140;
    uint16_t Motor_Store_Limit_Counter = 0;
};

/* 变量声明 ------------------------------------------------------------------------------------------------------------*/
extern Class_Chassis Engineer_Chassis;

/* 接口函数定义 ---------------------------------------------------------------------------------------------------------*/
/**
 * @brief 设置底盘速度
 *
 * @param Velocity_X        底盘目标平移速度-X（向前为正） (m/s)
 * @param Velocity_Y        底盘目标平移速度-Y（向左为正） (m/s)
 * @param Omega             底盘目标旋转速度 (rad/s)
 */
Enum_Chassis_Status Class_Chassis::Get_Chassis_Status()
{
    return (Chassis_Status);
}

Enum_Lift_Status Class_Chassis::Get_Lift_Front_Status()
{
    return (Lift_Front_Status);
}

Enum_Lift_Status Class_Chassis::Get_Lift_Back_Status()
{
    return (Lift_Back_Status);
}

float Class_Chassis::Get_Lift_Front_Angle()
{
    return this->Lift_Motor[0].Get_Now_Angle();
}

float Class_Chassis::Get_Lift_Back_Angle()
{
    return this->Lift_Motor[1].Get_Now_Angle();
}

void Class_Chassis::Set_Velocity(float Velocity_X, float Velocity_Y, float Omega)
{
    this->Velocity_X = Velocity_X;
    this->Velocity_Y = Velocity_Y;
    this->Omega = Omega;
}

void Class_Chassis::Set_Lift_Omega(float Omega_Front, float Omega_Back)
{
    this->Omega_Lift_Front = Omega_Front;
    this->Omega_Lift_Back = Omega_Back;
}

void Class_Chassis::Set_Lift_Front_Angle(float Angle)
{
    this->Angle_Lift_Front = Angle;
}

void Class_Chassis::Set_Lift_Back_Angle(float Angle)
{
    this->Angle_Lift_Back = Angle;
}

/* 接口函数定义 ---------------------------------------------------------------------------------------------------------*/
/**
 * @brief 调节升降电机角度
 *
 * @param __Angle        电机角度增量
 */
void Class_Chassis::Adjust_Lift_Front_Angle(float __Angle)
{
    this->Angle_Lift_Front += __Angle;
}

void Class_Chassis::Adjust_Lift_Back_Angle(float __Angle)
{
    this->Angle_Lift_Back += __Angle;
}

void Class_Chassis::Set_Forward_Status(bool Status)
{
    this->Forward_Status = Status;
}

void Class_Chassis::Set_Wheel_Motor_Tx_Out()
{
    for (int i = 0; i < 4; i++)
    {
        this->Chassis_Motor[i].Set_Tx_out();
    }
}

Enum_Store_Status Class_Chassis::Get_Store_Status()
{
    return (this->Store_Status);
}

void Class_Chassis::Set_Store_Status(Enum_Store_Status __Store_Status)
{
    this->Store_Status = __Store_Status;
}

#endif /* FML_Chassis */
