/**
 * @file    Motor_DM_J4310.h
 * @brief   达妙J4310电机驱动
 *
 * @author  Li-Chenyang
 * @date    2024-1-21 （创建）
 *
 */

#ifndef __HDL_MOTOR_DM_J4310_H
#define __HDL_MOTOR_DM_J4310_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "User_Can.h"
#include "User_Math.h"

/* 宏定义 -------------------------------------------------------------------------------------------------------------*/

/* 枚举类型定义 --------------------------------------------------------------------------------------------------------*/
/**
 * @brief 达妙电机状态枚举类型
 */
enum Enum_DM_Motor_Status
{
    DM_Motor_Status_DISABLE = 0,
    DM_Motor_Status_ENABLE,
};

/**
 * @brief 达妙电机控制模式枚举类型
 */
enum Enum_DM_Motor_ControlModes
{
    DM_Motor_ControlModes_MIT = 0,
    DM_Motor_ControlModes_PositionSpeed,
    DM_Motor_ControlModes_Speed,
};

/**
 * @brief 达妙电机错误枚举类型
 */
enum Enum_DM_Motor_Errors
{
    DM_Motor_Error_NOERROR = 0,
    DM_Motor_Error_OVERVOLTAGE,
    DM_Motor_Error_UNDERVOLTAGE,
    DM_Motor_Error_OVERCURRENT,
    DM_Motor_Error_OVERTEMPERATURE_MOS,
    DM_Motor_Error_OVERTEMPERATURE_Rotor,
    DM_Motor_Error_NOCOMMUNICATION,
    DM_Motor_Error_OVERLOAD,
};

/* 结构体定义 ----------------------------------------------------------------------------------------------------------*/
/**
 * @brief 达妙电机经过处理数据，均为标准单位
 */
struct Struct_DM_Motor_Data
{
    Enum_DM_Motor_Errors Error;
    float Angle;
    float Omega;
    float Torque;
    uint8_t Temperature_MOS;
    uint8_t Temperature_Rotor;
};

/* 类定义 -------------------------------------------------------------------------------------------------------------*/
/**
 * @brief DM_J4310电机
 */
class Class_Motor_DM_J4310
{
public:
    /* 函数 */
    void Init(Struct_CAN_Manage_Object * CAN_Manage_Obj, uint16_t CAN_ID, uint16_t Master_ID,
              Enum_DM_Motor_ControlModes ControlMode, float Angle_Min, float Angle_Max);
    void Clear_Error();
    void Enable();
    void Enable_Block();
    void Disable();
    Enum_DM_Motor_Status AliveCheck();
    void Control_MIT();
    void Control_PositionSpeed();
    void Control_Speed();
    void DataGet();

    inline Enum_DM_Motor_Errors Get_Error();
    inline float Get_Angle();
    inline float Get_Omega();
    inline float Get_Torque();
    inline uint8_t Get_Temperature_MOS();
    inline uint8_t Get_Temperature_Rotor();
    inline void Set_Target_Angle(float __Target_Angle);
    inline void Set_Target_Omega(float __Target_Omega);
    inline void Set_Target_Torque(float __Target_Torque);
    inline void Set_Control_Coefficient(float __Kp, float __Kd);
protected:
    /* 常量 */
    Struct_CAN_Manage_Object * CAN_Manage_Object;   /*!< 绑定CAN */
    uint16_t CAN_ID;                                /*!< 控制帧ID */
    uint16_t Master_ID;                             /*!< 反馈帧ID */
    float Control_Period = 6.0f;                    /*!< 控制周期, ms */
    float Angle_Min;                                /*!< 最小角度, rad */
    float Angle_Max;                                /*!< 最大角度, rad */
    uint8_t Can_Tx_Data[8];                         /*!< 数据发送缓存区 */

    /* 读变量 */
    Struct_DM_Motor_Data Data;                      /*!< 电机数据 */

    /* 读写变量 */
    float Target_Angle;                             /*!< 目标角度, rad */
    float Target_Omega;                             /*!< 目标速度, rad/s */
    float Target_Torque;                            /*!< 目标扭矩, N*M */
    float Pre_Angle;
    float Pre_Omega;
    float Kp;                                       /*!< 位置比例系数 */
    float Kd;                                       /*!< 位置微分系数 */

    /* 内部变量 */
    uint32_t Flag = 0;
    uint32_t Pre_Flag = 0;
    Enum_DM_Motor_Status DM_Motor_Status =          /*!< 电机状态 */
            DM_Motor_Status_DISABLE;
};

/* 变量声明 ------------------------------------------------------------------------------------------------------------*/
extern Class_Motor_DM_J4310 Motor_DM_J4310_Test;

/* 接口函数定义 ---------------------------------------------------------------------------------------------------------*/
/**
 * @brief 获取电机错误
 *
 * @return Enum_DM_Motor_Errors 电机错误
 */
Enum_DM_Motor_Errors Class_Motor_DM_J4310::Get_Error()
{
    return (this->Data.Error);
}

/**
 * @brief 获取电机角度
 *
 * @return float 电机当前角度
 */
float Class_Motor_DM_J4310::Get_Angle()
{
    return (this->Data.Angle);
}

/**
 * @brief 获取电机速度
 *
 * @return float 电机当前速度
 */
float Class_Motor_DM_J4310::Get_Omega()
{
    return (this->Data.Omega);
}

/**
 * @brief 获取电机扭矩
 *
 * @return float 电机当前扭矩
 */
float Class_Motor_DM_J4310::Get_Torque()
{
    return (this->Data.Torque);
}

/**
 * @brief 获取电机MOS温度
 *
 * @return float 电机当前MOS平均温度
 */
uint8_t Class_Motor_DM_J4310::Get_Temperature_MOS()
{
    return (this->Data.Temperature_MOS);
}

/**
 * @brief 获取电机线圈温度
 *
 * @return float 电机当前线圈平均温度
 */
uint8_t Class_Motor_DM_J4310::Get_Temperature_Rotor()
{
    return (this->Data.Temperature_Rotor);
}

/**
 * @brief 设定目标角度, rad
 *
 * @param __Target_Angle 目标角度, rad
 */
void Class_Motor_DM_J4310::Set_Target_Angle(float __Target_Angle)
{
//    if (__Target_Angle - this->Target_Angle <= -3.1415926f)
//    {
//        this->Target_Angle = __Target_Angle + 3.1415926f * 2.0f;
//    }
//    else if (__Target_Angle - this->Target_Angle >= 3.1415926f)
//    {
//        this->Target_Angle = __Target_Angle - 3.1415926f * 2.0f;
//    }
//    else
//    {
//        this->Target_Angle = __Target_Angle;
//    }
    this->Target_Angle = __Target_Angle;
}

/**
 * @brief 设定目标速度, rad/s
 *
 * @param __Target_Omega 目标速度, rad/s, MIT模式下角度控制时，此项为0
 */
void Class_Motor_DM_J4310::Set_Target_Omega(float __Target_Omega)
{
    this->Target_Omega = __Target_Omega;
}

/**
 * @brief 设定目标扭矩, N*M
 *
 * @param __Target_Torque 目标扭矩, N*M, MIT模式下角度控制时，此项为0
 */
void Class_Motor_DM_J4310::Set_Target_Torque(float __Target_Torque)
{
    this->Target_Torque = __Target_Torque;
}

/**
 * @brief 设定电机控制系数
 *
 * @param __Kp 位置比例系数, N/r, MIT模式下角度控制时，转速、扭矩与此项正相关，较大时电机产生低频振动
 * @param __Kd 位置微分系数, N*s/r, MIT模式下角度控制时，用于抑制电机低频振动，过大时电机产生高频振动
 */
void Class_Motor_DM_J4310::Set_Control_Coefficient(float __Kp, float __Kd)
{
    this->Kp = __Kp;
    this->Kd = __Kd;
}

#endif /* HDL_Motor_DM_J4310 */
