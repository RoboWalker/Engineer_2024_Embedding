/**
 * @file    User_Main.cpp
 * @brief   初始化，主函数实现
 *
 * @author  Li-Chenyang
 * @date    2024-1-18 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "User_Main.h"
#include "tim.h"
#include "Motor_DM.h"
#include "Motor_EC.h"
#include "Motor_Steer.h"
#include "Referee_System.h"
#include "Remote_Control.h"
#include "AHRS_WIT.h"
#include "Serialplot.h"
#include "Chassis.h"
#include "RobotArm.h"
#include "PanTilt.h"
#include "Referee_UI_Function.h"

static char Serialplot_Variable_Assignment_List[][SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH] = {
        // 电机调PID
        "pa",
        "ia",
        "da",
        "fa",
        "po",
        "io",
        "do",
        "fo",};

void User_setup(void)
{
    CAN_Init(&CAN1_Manage_Object);
    CAN_Init(&CAN2_Manage_Object);

    Remote_Control.Init(&UART1_Manage_Object);
    Referee_System.Init(&UART6_Manage_Object);
    AHRS_Chassis.Init(UART8_Manage_Object.huart);
    Serialplot.Init(UART2_Manage_Object.huart, 9, (char **) Serialplot_Variable_Assignment_List);

    Engineer_Chassis.Init();
    Engineer_Chassis.Lift_Motor[0].Set_Target_Torque(0.0f);
    Engineer_Chassis.Lift_Motor[1].Set_Target_Torque(0.0f);
    Engineer_Chassis.Lift_Motor[0].Control();
    Engineer_Chassis.Lift_Motor[1].Control();
    DJI_CAN_SendData();

//    HAL_Delay(1000);



//    Motor_DM_J4310_Test.Init(&CAN1_Manage_Object, 0x07, 0x17, DM_Motor_ControlModes_PositionSpeed, -12.0f, 12.0f);
//    Motor_DM_J4310_Test.Set_Target_Omega(20.0f);
//    Motor_DM_J4310_Test.Enable();

    Engineer_RobotArm.Init();
//    Engineer_RobotArm.Init_Teach();

    Engineer_PanTilt.Init();


//    Motor_EC_Test.Init(&CAN1_Manage_Object, 0x03, -0.5f, 0.5f);
//    Motor_EC_Test.Reset_CAN_ID();
//    HAL_Delay(1000);
//    Motor_EC_Test.Set_CAN_ID(0x01);
//    HAL_Delay(1000);
//    Motor_EC_Test.Set_Zero();
//    HAL_Delay(1000);
//    Motor_EC_Test.Set_Target_Omega(20.0f);
//    Motor_EC_Test.Set_Target_Current(50.0f);
//    Motor_EC_Test.Set_Target_Angle(0.0f);



    Engineer_Chassis.Lift_Motor[0].Clear_Data();
    Engineer_Chassis.Lift_Motor[1].Clear_Data();

//    Motor_DM_J4310_Test.Enable();

    /* 使能系统心跳定时器 */

    HAL_TIM_Base_Start_IT(&htim6);
}

void User_loop(void)
{
        // UI_reset_flag=0,发送动态UI
    if (UI_reset_flag == 0)
    {
        UI_draw_arm_state();
        HAL_Delay(110);
        UI_draw_sucker_state();
        HAL_Delay(110);
        UI_draw_layer_5();
        HAL_Delay(110);
    }
    else{
        // UI_reset_flag=1,刷新全部UI
        UI_draw_layer_9();
        HAL_Delay(110);
        UI_draw_layer_8();
        HAL_Delay(110);
        UI_draw_arm_state();
        HAL_Delay(110);
        UI_draw_sucker_state();
        HAL_Delay(110);
        UI_draw_layer_5();
        HAL_Delay(110);
        UI_reset_flag = 0;
    }
}
