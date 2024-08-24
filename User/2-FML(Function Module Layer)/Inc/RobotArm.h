/**
 * @file    RobotArm.h
 * @brief   机械臂驱动
 *
 * @author  Li-Chenyang
 * @date    2024-1-22 （创建）
 *
 */

#ifndef __FML_ROBOTARM_H
#define __FML_ROBOTARM_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "usbd_cdc_if.h"
#include "Motor_EC.h"
#include "Motor_DM.h"
#include "Remote_Control.h"
#include "gpio.h"
#include "cstring"
#include "math.h"
#include "Chassis.h"
#include "Referee_System.h"

/* 宏定义 -------------------------------------------------------------------------------------------------------------*/
#define ROBOTARM_MOTOR_1_EC_CAN_ID              0x01
#define ROBOTARM_MOTOR_2_EC_CAN_ID              0x02
#define ROBOTARM_MOTOR_3_EC_CAN_ID              0x03
#define ROBOTARM_MOTOR_4_DM_CAN_ID              0x04
#define ROBOTARM_MOTOR_5_DM_CAN_ID              0x05
#define ROBOTARM_MOTOR_6_DM_CAN_ID              0x06

#define ROBOTARM_MOTOR_4_DM_MASTER_ID           0x14
#define ROBOTARM_MOTOR_5_DM_MASTER_ID           0x15
#define ROBOTARM_MOTOR_6_DM_MASTER_ID           0x16

/* 枚举类型定义 --------------------------------------------------------------------------------------------------------*/
/**
 * @brief 机械臂状态机枚举类型
 */
enum Enum_RobotArm_Status
{
    RobotArm_Status_Die = 0,
    RobotArm_Status_Passive,
    RobotArm_Status_Controlled,
    RobotArm_Status_Teach,
    RobotArm_Status_Remote,
    RobotArm_Status_Null,
};

enum Enum_RobotArm_Mode
{
    RobotArm_Mode_Normal = 0,
    RobotArm_Mode_NormalToReverse,
    RobotArm_Mode_Reverse,
    RobotArm_Mode_ReverseToNormal,
};

enum Enum_Pump_Status
{
    Pump_Status_OFF = 0,
    Pump_Status_ON,
};

/* 类定义 -------------------------------------------------------------------------------------------------------------*/
/**
 * @brief 机械臂类
 */
class Class_RobotArm
{
public:
    /* 变量 */
    Class_Motor_EC RobotArm_Motor_EC[3];            /*!< 电机类（根部） */
    Class_Motor_DM_J4310 RobotArm_Motor_DM[3];      /*!< 电机类（末端） */
    Class_DR16 * DR16;                              /*!< DR16遥控器接收机 */
    Class_Referee_System * Referee_System;          /*!< 裁判系统 */

    /* 函数 */
    Class_RobotArm(Class_DR16 * __DR16, Class_Referee_System * __Referee_System);
    void Init();
    void Init_Teach();
    void DataGet_Remote();
    void DataGet_USB(uint8_t *Rx_Buf);
    void Control();
    void Control_Die();
    void Control_Passive();
    void Control_Controlled();
    void Control_Remote();
    void Control_Teach();
    void AliveCheck();
    void Pump_Start();
    void Pump_Stop(uint16_t Valve_Counter);
    void Send_Data_USB();
    void IK_Remote(void);
    inline void Get_Motors_Angle(float *Motors_Angle);
    inline void Set_Motors_Angle(float *Motors_Angle);
    inline void Set_Pump_Status(Enum_Pump_Status __Pump_Status);
    inline void Set_Lift_Position_To_Minipc(float_t __Lift_Position_To_Minipc);
    inline uint8_t Get_FSM_State_Code();
    inline Enum_RobotArm_Status Get_RobotArm_Status();
    inline Enum_Pump_Status Get_Pump_Status();
protected:
    /* 读变量 */
    float Position_Variance = 0.0f;     /*!< 机械臂电机位置方差 */

    /* 读写变量 */
    float Motors_Target_Angle[6];
    float Target_xyzrpy_Remote[6];          /*!< 遥控器控制模式下目标末端位置 */
    uint8_t Lift_Position_To_Minipc = 0;    /*!< 抬升位置反馈给上位机 */
    uint8_t FSM_State_Code = 0xff;          /*!< 机械臂状态机码 */
    bool Back_Placed = false;               /*!< 后方矿仓是否有矿石 */
    bool Front_Placed = false;              /*!< 前方矿仓是否有矿石 */

    /* 内部变量 */
    uint8_t AutoAction_Code = 0x00;     /*!< 自动动作码 */
    uint8_t Control_Counter = 0;        /*!< 电机循环控制计数 */
    uint16_t Err_Clear_Counter = 0;      /*!< 电机错误清除计数 */
    uint32_t Valve_Counter = 0;         /*!< 电磁阀定时断电计数 */
    Enum_RobotArm_Status RobotArm_Status = RobotArm_Status_Null;         /*!< 机械臂状态 */
    Enum_RobotArm_Mode RobotArm_Mode = RobotArm_Mode_Normal;
    Enum_Pump_Status Pump_Status = Pump_Status_OFF;                     /*!< 气泵状态 */
};

/* 变量声明 ------------------------------------------------------------------------------------------------------------*/
extern Class_RobotArm Engineer_RobotArm;

/* 接口函数定义 ---------------------------------------------------------------------------------------------------------*/

void Class_RobotArm::Get_Motors_Angle(float *Motors_Angle)
{
    Motors_Angle[0] = -this->RobotArm_Motor_EC[0].Get_Angle();
    Motors_Angle[1] = this->RobotArm_Motor_EC[1].Get_Angle();
    Motors_Angle[2] = -this->RobotArm_Motor_EC[2].Get_Angle();
    Motors_Angle[3] = this->RobotArm_Motor_DM[0].Get_Angle();
    Motors_Angle[4] = -this->RobotArm_Motor_DM[1].Get_Angle();
    Motors_Angle[5] = this->RobotArm_Motor_DM[2].Get_Angle();
}

void Class_RobotArm::Set_Motors_Angle(float *Motors_Angle)
{
    this->RobotArm_Motor_EC[0].Set_Target_Angle(-Motors_Angle[0]);
    this->RobotArm_Motor_EC[1].Set_Target_Angle(Motors_Angle[1]);
    this->RobotArm_Motor_EC[2].Set_Target_Angle(-Motors_Angle[2]);
    this->RobotArm_Motor_DM[0].Set_Target_Angle(Motors_Angle[3]);
    this->RobotArm_Motor_DM[1].Set_Target_Angle(-Motors_Angle[4]);
    this->RobotArm_Motor_DM[2].Set_Target_Angle(Motors_Angle[5]);
}

void Class_RobotArm::Set_Pump_Status(Enum_Pump_Status __Pump_Status)
{
    this->Pump_Status = __Pump_Status;
}

void Class_RobotArm::Set_Lift_Position_To_Minipc(float_t __Lift_Position_To_Minipc)
{
    this->Lift_Position_To_Minipc = (uint8_t)(__Lift_Position_To_Minipc / 14.5f * 255.0f);
}

uint8_t Class_RobotArm::Get_FSM_State_Code()
{
    return this->FSM_State_Code;
}

Enum_RobotArm_Status Class_RobotArm::Get_RobotArm_Status()
{
    return this->RobotArm_Status;
}

Enum_Pump_Status Class_RobotArm::Get_Pump_Status()
{
    return this->Pump_Status;
}

#endif /* FML_RobotArm */
