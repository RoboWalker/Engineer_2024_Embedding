/**
 * @file    Motor_EC.h
 * @brief   因克斯电机驱动
 *
 * @author  Li-Chenyang
 * @date    2024-1-23 （创建）
 *
 */

#ifndef __HDL_MOTOR_EC_H
#define __HDL_MOTOR_EC_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "User_Can.h"
#include "User_Math.h"
#include "math.h"

/* 宏定义 -------------------------------------------------------------------------------------------------------------*/
#define MOTOR_EC_CAN_ID_SETTING         0x7FF

/* 枚举类型定义 --------------------------------------------------------------------------------------------------------*/
/**
 * @brief 因克斯电机状态枚举类型
 */
enum Enum_EC_Motor_Status
{
    EC_Motor_Status_DISABLE = 0,
    EC_Motor_Status_ENABLE,
};

/**
 * @brief 因克斯电机控制模式枚举类型
 */
enum Enum_EC_Motor_ControlModes
{
    EC_Motor_ControlModes_TorquePosition = 0,
    EC_Motor_ControlModes_Position,
    EC_Motor_ControlModes_Speed,
    EC_Motor_ControlModes_Torque,
};

/**
 * @brief 因克斯电机错误枚举类型
 */
enum Enum_EC_Motor_Errors
{
    EC_Motor_Error_NOERROR = 0,
    EC_Motor_Error_OVERTEMPERATURE,
    EC_Motor_Error_OVERCURRENT,
    EC_Motor_Error_UNDERVOLTAGE,
    EC_Motor_Error_ENCODER,
    EC_Motor_Error_OVERVOLTAGE_BRAKE,
    EC_Motor_Error_DRV,
};

/* 结构体定义 ----------------------------------------------------------------------------------------------------------*/
/**
 * @brief 因克斯电机经过处理数据，均为标准单位
 */
struct Struct_EC_Motor_Data
{
    Enum_EC_Motor_Errors Error;
    float Angle;
    float Omega;
    float PhaseCurrent;
    float Temperature_Motor;
    float Temperature_MOS;
};

/* 类定义 -------------------------------------------------------------------------------------------------------------*/
/**
 * @brief 因克斯电机
 */
class Class_Motor_EC
{
public:
    /* 函数 */
    void Init(Struct_CAN_Manage_Object * CAN_Manage_Obj, uint16_t CAN_ID, float Angle_Min, float Angle_Max);
    void Reset_CAN_ID();
    void Set_CAN_ID(uint16_t CAN_ID_Old);
    void Set_Zero();
    Enum_EC_Motor_Status AliveCheck();
    void Control_TorquePosition();
    void Control_Position(uint8_t Ack_Form);
    void Control_Speed(uint8_t Ack_Form);
    void Control_CurrentTorque(uint8_t Control_Mode, uint8_t Ack_Form);
    void Brake(uint8_t Brake_Mode, uint8_t Ack_Form);
    void Set_Acceleration(float Acceleration, uint8_t Ack_Form);
    void DataGet();

    inline Enum_EC_Motor_Errors Get_Error();
    inline float Get_Angle();
    inline float Get_Omega();
    inline float Get_PhaseCurrent();
    inline uint8_t Get_Temperature_Motor();
    inline uint8_t Get_Temperature_MOS();
    inline void Set_Target_Angle(float __Target_Angle);
    inline void Set_Target_Omega(float __Target_Omega);
    inline void Set_Target_Current(float __Target_Current);
    inline void Set_Target_Torque(float __Target_Torque);
    inline void Set_Control_Coefficient(float __Kp, float __Kd);
protected:
    /* 常量 */
    Struct_CAN_Manage_Object * CAN_Manage_Object;
    uint16_t CAN_ID;
    float Angle_Min;
    float Angle_Max;
    uint8_t Can_Tx_Data[8];

    /* 读变量 */
    Struct_EC_Motor_Data Data;

    /* 读写变量 */
    float Target_Angle;
    float Target_Omega;
    float Target_Current;
    float Target_Torque;
    float Kp;
    float Kd;

    /* 内部变量 */
    uint32_t Flag = 0;
    uint32_t Pre_Flag = 0;
    Enum_EC_Motor_Status EC_Motor_Status = EC_Motor_Status_DISABLE;
};

/* 变量声明 ------------------------------------------------------------------------------------------------------------*/
extern Class_Motor_EC Motor_EC_Test;

/* 接口函数定义 ---------------------------------------------------------------------------------------------------------*/
/**
 * @brief 获取电机错误
 *
 * @return Enum_DM_Motor_Errors 电机错误
 */
Enum_EC_Motor_Errors Class_Motor_EC::Get_Error()
{
    return (this->Data.Error);
}

/**
 * @brief 获取电机角度
 *
 * @return float 电机当前角度
 */
float Class_Motor_EC::Get_Angle()
{
    return (this->Data.Angle);
}

/**
 * @brief 获取电机速度
 *
 * @return float 电机当前速度
 */
float Class_Motor_EC::Get_Omega()
{
    return (this->Data.Omega);
}

/**
 * @brief 获取电机扭矩
 *
 * @return float 电机当前扭矩
 */
float Class_Motor_EC::Get_PhaseCurrent()
{
    return (this->Data.PhaseCurrent);
}

/**
 * @brief 获取电机温度
 *
 * @return float 电机当前平均温度
 */
uint8_t Class_Motor_EC::Get_Temperature_Motor()
{
    return (this->Data.Temperature_Motor);
}

/**
 * @brief 获取电机MOS温度
 *
 * @return float 电机当前MOS平均温度
 */
uint8_t Class_Motor_EC::Get_Temperature_MOS()
{
    return (this->Data.Temperature_MOS);
}

/**
 * @brief 设定目标角度, rad
 *
 * @param __Target_Angle 目标角度, rad
 */
void Class_Motor_EC::Set_Target_Angle(float __Target_Angle)
{
    this->Target_Angle = __Target_Angle;
}

/**
 * @brief 设定目标速度, rad/s
 *
 * @param __Target_Omega 目标速度, rad/s
 */
void Class_Motor_EC::Set_Target_Omega(float __Target_Omega)
{
    this->Target_Omega = __Target_Omega;
}

/**
 * @brief 设定目标电流, A
 *
 * @param __Target_Torque 目标电流, A
 */
void Class_Motor_EC::Set_Target_Current(float __Target_Current)
{
    this->Target_Current = __Target_Current;
}

/**
 * @brief 设定目标扭矩, N*M
 *
 * @param __Target_Torque 目标扭矩, N*M
 */
void Class_Motor_EC::Set_Target_Torque(float __Target_Torque)
{
    this->Target_Torque = __Target_Torque;
}

/**
 * @brief 设定电机控制系数
 *
 * @param __Kp 位置比例系数, N/r
 * @param __Kd 位置微分系数, N*s/r
 */
void Class_Motor_EC::Set_Control_Coefficient(float __Kp, float __Kd)
{
    this->Kp = __Kp;
    this->Kd = __Kd;
}

#endif /* HDL_Motor_EC */