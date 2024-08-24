/**
 * @file    Callback_Can.cpp
 * @brief   CAN回调函数重写
 *
 * @author  Li-Chenyang
 * @date    2024-1-18 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Callback_Can.h"

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief CAN RX0中断回调函数重写
 *
 * @param hcan  CAN外设句柄
 **********************************************************************************************************************/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef * hcan)
{
    if (hcan->Instance == CAN1)
    {
        /* 接收数据至缓冲区 */
        CAN_Receive_Data(&CAN1_Manage_Object, CAN_FILTER_FIFO0);

        /* 数据处理 */
        switch (CAN1_Manage_Object.Rx_Buffer.Header.StdId)
        {
            case (ROBOTARM_MOTOR_1_EC_CAN_ID):
                Engineer_RobotArm.RobotArm_Motor_EC[0].DataGet();
                break;
            case (ROBOTARM_MOTOR_2_EC_CAN_ID):
                Engineer_RobotArm.RobotArm_Motor_EC[1].DataGet();
                break;
            case (ROBOTARM_MOTOR_3_EC_CAN_ID):
                Engineer_RobotArm.RobotArm_Motor_EC[2].DataGet();
                break;
            case (ROBOTARM_MOTOR_4_DM_MASTER_ID):
                Engineer_RobotArm.RobotArm_Motor_DM[0].DataGet();
                break;
            case (ROBOTARM_MOTOR_5_DM_MASTER_ID):
                Engineer_RobotArm.RobotArm_Motor_DM[1].DataGet();
                break;
            case (ROBOTARM_MOTOR_6_DM_MASTER_ID):
                Engineer_RobotArm.RobotArm_Motor_DM[2].DataGet();
                break;
            case (PANTILT_MOTOR_YAW_DM_MASTER_ID):
                Engineer_PanTilt.PanTilt_Motor_Yaw.DataGet();
                break;
            case (PANTILT_MOTOR_PITCH_DM_MASTER_ID):
                Engineer_PanTilt.PanTilt_Motor_Pitch.DataGet();
                break;
        }
    }
    else if (hcan->Instance == CAN2)
    {
        /* 接收数据至缓冲区 */
        CAN_Receive_Data(&CAN2_Manage_Object, CAN_FILTER_FIFO0);

        /* 数据处理 */
        switch(CAN2_Manage_Object.Rx_Buffer.Header.StdId)
        {
            case (DJI_Motor_ID_0x202):
                Engineer_Chassis.Chassis_Motor[0].DataGet();
                break;
            case (DJI_Motor_ID_0x201):
                Engineer_Chassis.Chassis_Motor[1].DataGet();
                break;
            case (DJI_Motor_ID_0x203):
                Engineer_Chassis.Chassis_Motor[2].DataGet();
                break;
            case (DJI_Motor_ID_0x204):
                Engineer_Chassis.Chassis_Motor[3].DataGet();
                break;
            case (DJI_Motor_ID_0x205):
                Engineer_Chassis.Lift_Motor[0].DataGet();
                break;
            case (DJI_Motor_ID_0x206):
                Engineer_Chassis.Lift_Motor[1].DataGet();
                break;
            case (DJI_Motor_ID_0x207):
                Engineer_Chassis.Store_Motor.DataGet();
                break;
        }
    }
}

/***********************************************************************************************************************
 * @brief CAN RX1中断回调函数重写
 *
 * @param hcan  CAN外设句柄
 **********************************************************************************************************************/
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef * hcan)
{
    if (hcan->Instance == CAN1)
    {
        /* 接收数据至缓冲区 */
        CAN_Receive_Data(&CAN1_Manage_Object, CAN_FILTER_FIFO1);

        /* 数据处理 */
        switch (CAN1_Manage_Object.Rx_Buffer.Header.StdId)
        {
            case (ROBOTARM_MOTOR_1_EC_CAN_ID):
                Engineer_RobotArm.RobotArm_Motor_EC[0].DataGet();
                break;
            case (ROBOTARM_MOTOR_2_EC_CAN_ID):
                Engineer_RobotArm.RobotArm_Motor_EC[1].DataGet();
                break;
            case (ROBOTARM_MOTOR_3_EC_CAN_ID):
                Engineer_RobotArm.RobotArm_Motor_EC[2].DataGet();
                break;
            case (ROBOTARM_MOTOR_4_DM_MASTER_ID):
                Engineer_RobotArm.RobotArm_Motor_DM[0].DataGet();
                break;
            case (ROBOTARM_MOTOR_5_DM_MASTER_ID):
                Engineer_RobotArm.RobotArm_Motor_DM[1].DataGet();
                break;
            case (ROBOTARM_MOTOR_6_DM_MASTER_ID):
                Engineer_RobotArm.RobotArm_Motor_DM[2].DataGet();
                break;
            case (PANTILT_MOTOR_YAW_DM_MASTER_ID):
                Engineer_PanTilt.PanTilt_Motor_Yaw.DataGet();
                break;
            case (PANTILT_MOTOR_PITCH_DM_MASTER_ID):
                Engineer_PanTilt.PanTilt_Motor_Pitch.DataGet();
                break;
        }
    }
    else if (hcan->Instance == CAN2)
    {
        /* 接收数据至缓冲区 */
        CAN_Receive_Data(&CAN2_Manage_Object, CAN_FILTER_FIFO1);

        /* 数据处理 */
        switch (CAN2_Manage_Object.Rx_Buffer.Header.StdId)
        {
            case (DJI_Motor_ID_0x202):
                Engineer_Chassis.Chassis_Motor[0].DataGet();
                break;
            case (DJI_Motor_ID_0x201):
                Engineer_Chassis.Chassis_Motor[1].DataGet();
                break;
            case (DJI_Motor_ID_0x203):
                Engineer_Chassis.Chassis_Motor[2].DataGet();
                break;
            case (DJI_Motor_ID_0x204):
                Engineer_Chassis.Chassis_Motor[3].DataGet();
                break;
            case (DJI_Motor_ID_0x205):
                Engineer_Chassis.Lift_Motor[0].DataGet();
                break;
            case (DJI_Motor_ID_0x206):
                Engineer_Chassis.Lift_Motor[1].DataGet();
                break;
            case (DJI_Motor_ID_0x207):
                Engineer_Chassis.Store_Motor.DataGet();
                break;
        }
    }
}