/**
 * @file    Motor_EC.cpp
 * @brief   因克斯电机驱动
 *
 * @author  Li-Chenyang
 * @date    2024-1-23 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Motor_EC.h"

/* 全局变量创建 --------------------------------------------------------------------------------------------------------*/
Class_Motor_EC Motor_EC_Test;

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief 因克斯电机初始化
 *
 * @param CAN_Manage_Obj                CAN处理结构体指针
 * @param CAN_ID                        CAN-ID
 **********************************************************************************************************************/
void Class_Motor_EC::Init(Struct_CAN_Manage_Object *CAN_Manage_Obj, uint16_t CAN_ID, float Angle_Min, float Angle_Max)
{
    this->CAN_Manage_Object = CAN_Manage_Obj;
    this->CAN_ID = CAN_ID;
    this->Angle_Min = Angle_Min;
    this->Angle_Max = Angle_Max;
}

/***********************************************************************************************************************
 * @brief 重置电机CAN_ID为0x01
 **********************************************************************************************************************/
void Class_Motor_EC::Reset_CAN_ID()
{
    this->Can_Tx_Data[0] = 0x7F;
    this->Can_Tx_Data[1] = 0x7F;
    this->Can_Tx_Data[2] = 0x00;
    this->Can_Tx_Data[3] = 0x05;
    this->Can_Tx_Data[4] = 0x7F;
    this->Can_Tx_Data[5] = 0x7F;

    CAN_Send_Data(this->CAN_Manage_Object, MOTOR_EC_CAN_ID_SETTING, this->Can_Tx_Data, 6);
}

/***********************************************************************************************************************
 * @brief 设定电机CAN_ID
 **********************************************************************************************************************/
void Class_Motor_EC::Set_CAN_ID(uint16_t CAN_ID_Old)
{
    this->Can_Tx_Data[0] = (uint8_t)(CAN_ID_Old >> 8);
    this->Can_Tx_Data[1] = (uint8_t)(CAN_ID_Old);
    this->Can_Tx_Data[2] = 0x00;
    this->Can_Tx_Data[3] = 0x04;
    this->Can_Tx_Data[4] = (uint8_t)(this->CAN_ID >> 8);
    this->Can_Tx_Data[5] = (uint8_t)(this->CAN_ID);

    CAN_Send_Data(this->CAN_Manage_Object, MOTOR_EC_CAN_ID_SETTING, this->Can_Tx_Data, 6);
}

/***********************************************************************************************************************
 * @brief 设定电机零点
 **********************************************************************************************************************/
void Class_Motor_EC::Set_Zero()
{
    this->Can_Tx_Data[0] = (uint8_t)(this->CAN_ID >> 8);
    this->Can_Tx_Data[1] = (uint8_t)(this->CAN_ID);
    this->Can_Tx_Data[2] = 0x00;
    this->Can_Tx_Data[3] = 0x03;

    CAN_Send_Data(this->CAN_Manage_Object, MOTOR_EC_CAN_ID_SETTING, this->Can_Tx_Data, 4);
}

/***********************************************************************************************************************
 * @brief 电机存活检测
 **********************************************************************************************************************/
Enum_EC_Motor_Status Class_Motor_EC::AliveCheck()
{
    if (this->Flag == this->Pre_Flag)
    {
        this->EC_Motor_Status = EC_Motor_Status_DISABLE;
    }
    else
    {
        this->EC_Motor_Status = EC_Motor_Status_ENABLE;
        this->Pre_Flag = this->Flag;
    }

    return (this->EC_Motor_Status);
}

/***********************************************************************************************************************
 * @brief 力位混控（未测试）
 **********************************************************************************************************************/
void Class_Motor_EC::Control_TorquePosition()
{
    /* 临时数据存储变量 */
    uint16_t Kp_tmp, Kd_tmp, pos_tmp, spd_tmp, torque_tmp;

    /* 数据限幅 */
    Math_Constrain(&this->Kp, 0.0f, 500.0f);
    Math_Constrain(&this->Kd, 0.0f, 5.0f);
    Math_Constrain(&this->Target_Angle, -12.5f, 12.5f);
    Math_Constrain(&this->Target_Angle, this->Angle_Min, this->Angle_Max);
    Math_Constrain(&this->Target_Omega, -18.0f, 18.0f);
    Math_Constrain(&this->Target_Torque, -90.0f, 90.0f);

    /* 数据格式转化 */
    Kp_tmp = (uint16_t)Math_Float_To_Uint(this->Kp, 0.0f, 500.0f, 12);
    Kd_tmp = (uint16_t)Math_Float_To_Uint(this->Kd, 0.0f, 5.0f, 9);
    pos_tmp = (uint16_t)Math_Float_To_Uint(this->Target_Angle, -12.5f, 12.5f, 16);
    spd_tmp = (uint16_t)Math_Float_To_Uint(this->Target_Omega, -18.0f, 18.0f, 12);
    torque_tmp = (uint16_t)Math_Float_To_Uint(this->Target_Torque, -90.0f, 90.0f, 12);

    /* 将数据填充至CAN发送缓存区 */
    this->Can_Tx_Data[0] = (uint8_t)(Kp_tmp >> 7);
    this->Can_Tx_Data[1] = (uint8_t)((Kp_tmp << 1) | (Kd_tmp >> 8));
    this->Can_Tx_Data[2] = (uint8_t)(Kd_tmp);
    this->Can_Tx_Data[3] = (uint8_t)(pos_tmp >> 8);
    this->Can_Tx_Data[4] = (uint8_t)(pos_tmp);
    this->Can_Tx_Data[5] = (uint8_t)(spd_tmp >> 4);
    this->Can_Tx_Data[6] = (uint8_t)((spd_tmp << 4) | (torque_tmp >> 8));
    this->Can_Tx_Data[7] = (uint8_t)(torque_tmp);

    /* 发送数据至电机 */
    CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 8);
}


/***********************************************************************************************************************
 * @brief 伺服位置控制
 **********************************************************************************************************************/
void Class_Motor_EC::Control_Position(uint8_t Ack_Form)
{
    float angle_degree;                 /* 目标角度（角度制） */
    uint8_t *angle_tmp, ack_tmp;        /* 临时变量 */
    uint16_t omega_tmp, current_tmp;

    /* 数据限幅 */
    Math_Constrain(&this->Target_Angle, -12.5f, 12.5f);
    Math_Constrain(&this->Target_Angle, this->Angle_Min, this->Angle_Max);
    Math_Constrain(&this->Target_Omega, 0.0f, 18.0f);
    Math_Constrain(&this->Target_Current, 0.0f, 50.0f);

    /* 数据格式转化 */
    angle_degree = Math_Rad_To_Degree(this->Target_Angle);
    angle_tmp = (uint8_t *)(&angle_degree);
    omega_tmp = (uint16_t)(this->Target_Omega / 3.1415926f * 60.0f * 10.0f);
    current_tmp = (uint16_t)(this->Target_Current * 10.0f);
    switch (Ack_Form)
    {
        case 0:
            ack_tmp = 0x00;
            break;
        case 1:
            ack_tmp = 0x01;
            break;
        case 2:
            ack_tmp = 0x02;
            break;
        case 3:
            ack_tmp = 0x03;
            break;
        default:
            ack_tmp = 0x00;
            break;
    }

    /* 发送缓冲区填充 */
    this->Can_Tx_Data[0] = (uint8_t)(0x20 | (angle_tmp[3] >> 3));
    this->Can_Tx_Data[1] = (uint8_t)((angle_tmp[3] << 5) | (angle_tmp[2] >> 3));
    this->Can_Tx_Data[2] = (uint8_t)((angle_tmp[2] << 5) | (angle_tmp[1] >> 3));
    this->Can_Tx_Data[3] = (uint8_t)((angle_tmp[1] << 5) | (angle_tmp[0] >> 3));
    this->Can_Tx_Data[4] = (uint8_t)((angle_tmp[0] << 5) | (omega_tmp >> 10));
    this->Can_Tx_Data[5] = (uint8_t)((omega_tmp & 0x3FC) >> 2);
    this->Can_Tx_Data[6] = (uint8_t)(((omega_tmp & 0x03) << 6) | (current_tmp >> 6));
    this->Can_Tx_Data[7] = (uint8_t)((current_tmp << 2) | ack_tmp);

    /* 发送数据至电机 */
    CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 8);
}

/***********************************************************************************************************************
 * @brief 伺服速度控制（未测试）
 **********************************************************************************************************************/
void Class_Motor_EC::Control_Speed(uint8_t Ack_Form)
{
    float omega_rpm;                /* 目标速度 (rpm) */
    uint8_t ack_tmp, *omega_tmp;
    int16_t current_tmp;

    /* 数据限幅 */
    Math_Constrain(&this->Target_Omega, -18.0f, 18.0f);
    Math_Constrain(&this->Target_Current, 0.0f, 50.0f);

    /* 单位转换 */
    omega_rpm = Target_Omega / 3.1415826f * 60.0f;

    /* 格式转换 */
    switch (Ack_Form)
    {
        case 0:
            ack_tmp = 0x00;
            break;
        case 1:
            ack_tmp = 0x01;
            break;
        case 2:
            ack_tmp = 0x02;
            break;
        case 3:
            ack_tmp = 0x03;
            break;
        default:
            ack_tmp = 0x00;
            break;
    }
    omega_tmp = (uint8_t *)(&omega_rpm);
    current_tmp = (int16_t)(Target_Current * 10.0f);

    /* 缓冲区填充 */
    this->Can_Tx_Data[0] = (uint8_t)(0x40 | (ack_tmp & 0x03));
    this->Can_Tx_Data[1] = (uint8_t)(omega_tmp[3]);
    this->Can_Tx_Data[2] = (uint8_t)(omega_tmp[2]);
    this->Can_Tx_Data[3] = (uint8_t)(omega_tmp[1]);
    this->Can_Tx_Data[4] = (uint8_t)(omega_tmp[0]);
    this->Can_Tx_Data[5] = (uint8_t)(current_tmp >> 8);
    this->Can_Tx_Data[6] = (uint8_t)(current_tmp & 0xFF);

    /* 发送数据 */
    CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 7);
}

/***********************************************************************************************************************
 * @brief 电流、力矩控制（电流模式未测试）
 *
 * @param Control_Mode          预留控制状态：   0：正常电流控制    1：力矩控制模式
 * @param Ack_Form              报文返回状态（详见电机手册）
 **********************************************************************************************************************/
void Class_Motor_EC::Control_CurrentTorque(uint8_t Control_Mode, uint8_t Ack_Form)
{
    uint8_t ack_tmp;
    int16_t current_tmp, torque_tmp;

    switch (Ack_Form)
    {
        case 0:
            ack_tmp = 0x00;
            break;
        case 1:
            ack_tmp = 0x01;
            break;
        case 2:
            ack_tmp = 0x02;
            break;
        case 3:
            ack_tmp = 0x03;
            break;
        default:
            ack_tmp = 0x00;
            break;
    }

    switch (Control_Mode)
    {
        case 0:
            Math_Constrain(&this->Target_Current, -50.0f, 50.0f);
            current_tmp = (int16_t)(this->Target_Current * 100.0f);

            this->Can_Tx_Data[0] = (uint8_t)((0x03 << 5) | (0x00 << 2) | (ack_tmp & 0x03));
            this->Can_Tx_Data[1] = (uint8_t)(current_tmp >> 8);
            this->Can_Tx_Data[2] = (uint8_t)(current_tmp & 0xFF);
            break;
        case 1:
            Math_Constrain(&this->Target_Torque, -90.0f, 90.0f);
            torque_tmp = (int16_t)(this->Target_Torque * 100.0f);

            this->Can_Tx_Data[0] = (uint8_t)((0x03 << 5) | (0x01 << 2) | (ack_tmp & 0x03));
            this->Can_Tx_Data[1] = (uint8_t)(torque_tmp >> 8);
            this->Can_Tx_Data[2] = (uint8_t)(torque_tmp & 0xFF);
            break;
        default:
            return;
    }

    CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 3);
}


/***********************************************************************************************************************
 * @brief 设定电机加速度
 *
 * @param Acceleration  加速度, rad/s^2
 **********************************************************************************************************************/
void Class_Motor_EC::Set_Acceleration(float Acceleration, uint8_t Ack_Form)
{
    uint16_t accel_tmp;
    uint8_t ack_tmp;

    accel_tmp = (uint16_t)(Acceleration * 100.0f);
    Math_Constrain(&accel_tmp, (uint16_t)0, (uint16_t)2000);
    switch (Ack_Form)
    {
        case 0:
            ack_tmp = 0x00;
            break;
        case 4:
            ack_tmp = 0x01;
            break;
        default:
            ack_tmp = 0x00;
            break;
    }

    this->Can_Tx_Data[0] = (uint8_t)(0xC0 | ack_tmp);
    this->Can_Tx_Data[1] = (uint8_t)(0x01);
    this->Can_Tx_Data[2] = (uint8_t)(accel_tmp >> 8);
    this->Can_Tx_Data[3] = (uint8_t)(accel_tmp);

    if (Ack_Form == 4)
    {
        do{
            CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 4);
            HAL_Delay(100);
        } while (this->EC_Motor_Status == EC_Motor_Status_DISABLE);
    }
    else
    {
        CAN_Send_Data(this->CAN_Manage_Object, this->CAN_ID, this->Can_Tx_Data, 4);
    }
}

/***********************************************************************************************************************
 * @brief 处理电机回传数据
 **********************************************************************************************************************/
void Class_Motor_EC::DataGet()
{
    uint8_t *buf = this->CAN_Manage_Object->Rx_Buffer.Data;
    uint8_t error_code = (uint8_t)(buf[0] & 0x1F), ack_form = (uint8_t)(buf[0] >> 5);
    uint16_t Angle_tmp, Omega_tmp, Current_tmp;
    float *Angle_degree, *Omega_rpm;

    this->Flag++;

    switch (error_code)
    {
        case 0x00:
            this->Data.Error = EC_Motor_Error_NOERROR;
            break;
        case 0x01:
            this->Data.Error = EC_Motor_Error_OVERTEMPERATURE;
            break;
        case 0x02:
            this->Data.Error = EC_Motor_Error_OVERCURRENT;
            break;
        case 0x03:
            this->Data.Error = EC_Motor_Error_UNDERVOLTAGE;
            break;
        case 0x04:
            this->Data.Error = EC_Motor_Error_ENCODER;
            break;
        case 0x06:
            this->Data.Error = EC_Motor_Error_OVERVOLTAGE_BRAKE;
            break;
        case 0x07:
            this->Data.Error = EC_Motor_Error_DRV;
            break;
    }

    switch (ack_form) {
        case 0x01:
            Angle_tmp = (uint16_t)((buf[1] << 8) | buf[2]);
            Omega_tmp = (uint16_t)((buf[3] << 4) | (buf[4] >> 4));
            Current_tmp = (uint16_t)(((buf[4] & 0x0F) << 8) | buf[5]);

            this->Data.Angle = Math_Uint_To_Float(Angle_tmp, -12.5f, 12.5f, 16);
            this->Data.Omega = Math_Uint_To_Float(Omega_tmp, -18.0f, 18.0f, 12);
            this->Data.PhaseCurrent = Math_Uint_To_Float(Current_tmp, -30.0f, 30.0f, 12);
            this->Data.Temperature_Motor = ((float)buf[6] - 50.0f) / 2.0f;
            this->Data.Temperature_MOS = ((float)buf[7] - 50.0f) / 2.0f;

            break;
        case 0x02:
            Angle_degree = (float *)(buf + 1);
            Current_tmp = (uint16_t)((buf[5] << 8) | buf[6]);

            this->Data.Angle = Math_Degree_To_Rad(*Angle_degree);
            this->Data.PhaseCurrent = Math_Uint_To_Float(Current_tmp, -327.68f, 327.67f, 16);
            this->Data.Temperature_Motor = ((float)buf[7] - 50.0f) / 2.0f;

            break;
        case 0x03:
            Omega_rpm = (float *)(buf + 1);
            Current_tmp = (uint16_t)((buf[5] << 8) | buf[6]);

            this->Data.Omega = *Omega_rpm * 3.1415926f / 60.0f;
            this->Data.PhaseCurrent = Math_Uint_To_Float(Current_tmp, -327.68f, 327.67f, 16);
            this->Data.Temperature_Motor = ((float)buf[7] - 50.0f) / 2.0f;

            break;
        case 0x04:
        {
            if (buf[2] == 0x00)
            {
                this->EC_Motor_Status = EC_Motor_Status_DISABLE;
                return;
            }

            break;
        }
    }

    this->EC_Motor_Status = EC_Motor_Status_ENABLE;
}
