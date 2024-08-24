/**
 * @file    Motor_DM_J4310.cpp
 * @brief   达妙J4310电机驱动
 *
 * @author  Li-Chenyang
 * @date    2024-1-21 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Motor_DM.h"

/* 全局变量创建 --------------------------------------------------------------------------------------------------------*/
Class_Motor_DM_J4310 Motor_DM_J4310_Test;

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief DM-J4310电机初始化
 *
 * @param CAN_Manage_Obj                CAN处理结构体指针
 * @param CAN_ID                        CAN-ID
 * @param Master_ID                     Master_ID
 **********************************************************************************************************************/
void Class_Motor_DM_J4310::Init(Struct_CAN_Manage_Object *CAN_Manage_Obj, uint16_t CAN_ID, uint16_t Master_ID,
                                Enum_DM_Motor_ControlModes ControlMode, float Angle_Min, float Angle_Max)
{
    this->CAN_Manage_Object = CAN_Manage_Obj;
    switch (ControlMode) {
        case DM_Motor_ControlModes_MIT:
            this->CAN_ID = CAN_ID;
            break;
        case DM_Motor_ControlModes_PositionSpeed:
            this->CAN_ID = 0x100 + CAN_ID;
            break;
        case DM_Motor_ControlModes_Speed:
            this->CAN_ID = 0x200 + CAN_ID;
            break;
    }
    this->Master_ID = Master_ID;
    this->Angle_Min = Angle_Min;
    this->Angle_Max = Angle_Max;
}

/***********************************************************************************************************************
 * @brief DM-J4310电机清除错误
 **********************************************************************************************************************/
void Class_Motor_DM_J4310::Clear_Error()
{
    uint8_t i;

    for (i = 0; i < 7; i++)
    {
        this->Can_Tx_Data[i] = 0xFF;
    }
    this->Can_Tx_Data[7] = 0xFB;

    CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 8);
}

/***********************************************************************************************************************
 * @brief DM-J4310电机使能
 **********************************************************************************************************************/
void Class_Motor_DM_J4310::Enable()
{
    uint8_t i;

    for (i = 0; i < 7; i++)
    {
        this->Can_Tx_Data[i] = 0xFF;
    }
    this->Can_Tx_Data[7] = 0xFC;

    CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 8);
}

/***********************************************************************************************************************
 * @brief DM-J4310电机使能（阻塞式）
 **********************************************************************************************************************/
void Class_Motor_DM_J4310::Enable_Block()
{
    uint8_t i;

    for (i = 0; i < 7; i++)
    {
        this->Can_Tx_Data[i] = 0xFF;
    }
    this->Can_Tx_Data[7] = 0xFC;

    do {
        CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 8);
        HAL_Delay(100);
    } while (this->DM_Motor_Status == DM_Motor_Status_DISABLE);

}

/***********************************************************************************************************************
 * @brief DM-J4310电机失能
 **********************************************************************************************************************/
void Class_Motor_DM_J4310::Disable()
{
    uint8_t i;

    for (i = 0; i < 7; i++)
    {
        this->Can_Tx_Data[i] = 0xFF;
    }
    this->Can_Tx_Data[7] = 0xFD;

    CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 8);
}

/***********************************************************************************************************************
 * @brief DM-J4310电机存活检测
 **********************************************************************************************************************/
Enum_DM_Motor_Status Class_Motor_DM_J4310::AliveCheck()
{
    if (this->Flag == this->Pre_Flag)
    {
        this->DM_Motor_Status = DM_Motor_Status_DISABLE;
    }
    else
    {
        this->DM_Motor_Status = DM_Motor_Status_ENABLE;
        this->Pre_Flag = this->Flag;
    }

    return (this->DM_Motor_Status);
}

/***********************************************************************************************************************
 * @brief MIT控制模式
 **********************************************************************************************************************/
void Class_Motor_DM_J4310::Control_MIT()
{
    uint16_t pos_tmp, vel_tmp, Kp_tmp, Kd_tmp, torque_tmp;

    Math_Constrain(&this->Target_Angle, -12.5f, 12.5f);
    Math_Constrain(&this->Target_Angle, this->Angle_Min, this->Angle_Max);
    Math_Constrain(&this->Target_Omega, -45.0f, 45.0f);
    Math_Constrain(&this->Kp, 0.0f, 500.0f);
    Math_Constrain(&this->Kd, 0.0f, 5.0f);
    Math_Constrain(&this->Target_Torque, -18.0f, 18.0f);

    pos_tmp = (uint16_t)Math_Float_To_Uint(this->Target_Angle, -12.5f, 12.5f, 16);
    vel_tmp = (uint16_t)Math_Float_To_Uint(this->Target_Omega, -45.0f, 45.0f, 12);
    Kp_tmp = (uint16_t)Math_Float_To_Uint(this->Kp, 0.0f, 500.0f, 12);
    Kd_tmp = (uint16_t)Math_Float_To_Uint(this->Kd, 0.0f, 5.0f, 12);
    torque_tmp = (uint16_t)Math_Float_To_Uint(this->Target_Torque, -18.0f, 18.0f, 12);

    this->Can_Tx_Data[0] = (uint8_t)(pos_tmp >> 8);
    this->Can_Tx_Data[1] = (uint8_t)pos_tmp;
    this->Can_Tx_Data[2] = (uint8_t)(vel_tmp >> 4);
    this->Can_Tx_Data[3] = (uint8_t)(((vel_tmp & 0x0F) << 4) | (Kp_tmp >> 8));
    this->Can_Tx_Data[4] = (uint8_t)Kp_tmp;
    this->Can_Tx_Data[5] = (uint8_t)(Kd_tmp >> 4);
    this->Can_Tx_Data[6] = (uint8_t)(((Kd_tmp & 0x0F) << 4) | (torque_tmp >> 8));
    this->Can_Tx_Data[7] = (uint8_t)torque_tmp;

    CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 8);
}

/***********************************************************************************************************************
 * @brief 位置速度控制模式
 **********************************************************************************************************************/
void Class_Motor_DM_J4310::Control_PositionSpeed()
{
    uint8_t * PBuf, * VBuf;

    Math_Constrain(&this->Target_Angle, this->Angle_Min, this->Angle_Max);
    Math_Constrain(&this->Target_Omega, -45.0f, 45.0f);
    PBuf = (uint8_t *)(&this->Target_Angle);
    VBuf = (uint8_t *)(&this->Target_Omega);

    this->Can_Tx_Data[0] = PBuf[0];
    this->Can_Tx_Data[1] = PBuf[1];
    this->Can_Tx_Data[2] = PBuf[2];
    this->Can_Tx_Data[3] = PBuf[3];
    this->Can_Tx_Data[4] = VBuf[0];
    this->Can_Tx_Data[5] = VBuf[1];
    this->Can_Tx_Data[6] = VBuf[2];
    this->Can_Tx_Data[7] = VBuf[3];

    CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 8);
}

/***********************************************************************************************************************
 * @brief 速度控制模式
 **********************************************************************************************************************/
void Class_Motor_DM_J4310::Control_Speed()
{
    uint8_t * VBuf;

    Math_Constrain(&this->Target_Omega, -45.0f, 45.0f);
    VBuf = (uint8_t *)(&this->Target_Omega);

    this->Can_Tx_Data[0] = VBuf[0];
    this->Can_Tx_Data[1] = VBuf[1];
    this->Can_Tx_Data[2] = VBuf[2];
    this->Can_Tx_Data[3] = VBuf[3];

    CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 4);
}

/***********************************************************************************************************************
 * @brief 处理电机回传数据
 **********************************************************************************************************************/
void Class_Motor_DM_J4310::DataGet()
{
    uint16_t Angle_tmp, Omega_tmp, Torque_tmp;
    uint8_t * buf = this->CAN_Manage_Object->Rx_Buffer.Data;

    this->Flag++;

    Angle_tmp = (buf[1] << 8) | buf[2];
    Omega_tmp = (buf[3] << 4) | (buf[4] >> 4);
    Torque_tmp = ((buf[4] & 0x0F) << 8) | buf[5];

    switch (buf[0] >> 4)
    {
        case (0x08):
            this->Data.Error = DM_Motor_Error_OVERVOLTAGE;
            break;
        case (0x09):
            this->Data.Error = DM_Motor_Error_UNDERVOLTAGE;
            break;
        case (0x0A):
            this->Data.Error = DM_Motor_Error_OVERCURRENT;
            break;
        case (0x0B):
            this->Data.Error = DM_Motor_Error_OVERTEMPERATURE_MOS;
            break;
        case (0x0C):
            this->Data.Error = DM_Motor_Error_OVERTEMPERATURE_Rotor;
            break;
        case (0x0D):
            this->Data.Error = DM_Motor_Error_NOCOMMUNICATION;
            break;
        case (0x0E):
            this->Data.Error = DM_Motor_Error_OVERLOAD;
            break;
        default:
            this->Data.Error = DM_Motor_Error_NOERROR;
            break;
    }
    this->Data.Angle = Math_Uint_To_Float((uint32_t)Angle_tmp, -12.5f, 12.5f, 16);
    this->Data.Omega = Math_Uint_To_Float((uint32_t)Omega_tmp, -45.0f, 45.0f, 12);
    this->Data.Torque = Math_Uint_To_Float((uint32_t)Torque_tmp, -18.0f, 18.0f, 12);
    this->Data.Temperature_MOS = buf[6];
    this->Data.Temperature_Rotor = buf[7];

    this->DM_Motor_Status = DM_Motor_Status_ENABLE;
}
