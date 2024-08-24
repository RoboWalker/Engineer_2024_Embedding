/**
 * @file    PanTilt.cpp
 * @brief   云台驱动
 *
 * @author  Li-Chenyang
 * @date    2024-4-19 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "PanTilt.h"

/* 全局变量创建 --------------------------------------------------------------------------------------------------------*/
Class_PanTilt Engineer_PanTilt(&Remote_Control);

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief 云台类构造函数
 *
 * @param __DR16        DR16对象指针
 **********************************************************************************************************************/
Class_PanTilt::Class_PanTilt(Class_DR16 *__DR16)
{
    this->DR16 = __DR16;
}

/***********************************************************************************************************************
 * @brief 机械臂初始化函数
 **********************************************************************************************************************/
void Class_PanTilt::Init()
{
    this->PanTilt_Motor_Yaw.Init(&CAN1_Manage_Object,
                                 PANTILT_MOTOR_YAW_DM_CAN_ID,PANTILT_MOTOR_YAW_DM_MASTER_ID,
                                 DM_Motor_ControlModes_PositionSpeed,
                                 0.0f, 3.1415926f);
    this->PanTilt_Motor_Pitch.Init(&CAN1_Manage_Object,
                                   PANTILT_MOTOR_PITCH_DM_CAN_ID,PANTILT_MOTOR_PITCH_DM_MASTER_ID,
                                   DM_Motor_ControlModes_PositionSpeed,
                                   -1.0f, 1.0f);

    this->PanTilt_Motor_Yaw.Set_Target_Omega(5.0f);
    this->PanTilt_Motor_Pitch.Set_Target_Omega(5.0f);

    this->PanTilt_Motor_Yaw.Set_Target_Angle(0.0f);
    this->PanTilt_Motor_Pitch.Set_Target_Angle(0.0f);

    /* 阻塞式初始化电机 */
//    this->PanTilt_Motor_Yaw.Enable();
//    HAL_Delay(1);
//    this->PanTilt_Motor_Yaw.Enable();

    this->Angle_Yaw = 0.0f;
    this->Angle_Pitch = 0.0f;

    this->PanTilt_Status = PanTilt_Status_Die;
}

/***********************************************************************************************************************
 * @brief 获取遥控器数据
 **********************************************************************************************************************/
void Class_PanTilt::DataGet_Remote()
{
    //Ctrl和Shift没有被按下
    if ((this->DR16->Get_Keyboard_Key_Ctrl() == DR16_Key_Status_FREE) &&
        (this->DR16->Get_Keyboard_Key_Shift() == DR16_Key_Status_FREE))
    {
        if (this->DR16->Get_Mouse_Left_Key() == DR16_Key_Status_PRESSED)
        {
            this->Angle_Pitch += 0.01f;
        }
        else if (this->DR16->Get_Mouse_Right_Key() == DR16_Key_Status_PRESSED)
        {
            this->Angle_Pitch -= 0.01f;
        }

        if (this->DR16->Get_Keyboard_Key_F() == DR16_Key_Status_PRESSED)
        {
            this->Angle_Pitch = 0.6f;
        }
    }
    else if ((this->DR16->Get_Keyboard_Key_Ctrl() == DR16_Key_Status_PRESSED) &&
             (this->DR16->Get_Keyboard_Key_Shift() == DR16_Key_Status_FREE))
    {
        //ctrl按下
        //ctrl+F小云台掉头
        if (this->PanTilt_Status == PanTilt_Status_Forward)
        {
            if ((this->DR16->Get_Keyboard_Key_F() == DR16_Key_Status_TRIG_FREE_PRESSED) &&
                (Engineer_Chassis.Lift_Motor[1].Get_Now_Angle() <= -this->Angle_Lift_Limit))
            {
                this->PanTilt_Status = PanTilt_Status_Backward;
                this->Angle_Yaw = 3.1415926f;
                return;
            }
            else
            {
                this->Angle_Yaw = 0.0f;
            }
        }
        else if (this->PanTilt_Status == PanTilt_Status_Backward)
        {
            if ((this->DR16->Get_Keyboard_Key_F() == DR16_Key_Status_TRIG_FREE_PRESSED) &&
                (Engineer_Chassis.Lift_Motor[1].Get_Now_Angle() <= -this->Angle_Lift_Limit))
            {
                this->PanTilt_Status = PanTilt_Status_Forward;
                this->Angle_Yaw = 0.0f;
                return;
            }
            else
            {
                this->Angle_Yaw = 3.1415926f;
            }
        }
    }

    Math_Constrain(&this->Angle_Pitch, this->Angle_Pitch_Min, this->Angle_Pitch_Max);

    this->PanTilt_Motor_Yaw.Set_Target_Angle(this->Angle_Yaw);
    this->PanTilt_Motor_Pitch.Set_Target_Angle(this->Angle_Pitch);
}

/***********************************************************************************************************************
 * @brief 云台存活检测函数
 **********************************************************************************************************************/
void Class_PanTilt::AliveCheck()
{
    if (this->PanTilt_Motor_Yaw.AliveCheck() == DM_Motor_Status_DISABLE)
    {
        this->PanTilt_Status = PanTilt_Status_Die;
        return;
    }

    if (this->PanTilt_Status == PanTilt_Status_Die)
    {
        this->PanTilt_Status = PanTilt_Status_Forward;
//        if (this->PanTilt_Motor_Yaw.Get_Angle() > 1.5f)
//        {
//            this->PanTilt_Status = PanTilt_Status_Backward;
//        }
//        else
//        {
//            this->PanTilt_Status = PanTilt_Status_Forward;
//        }
    }
}

/***********************************************************************************************************************
 * @brief 云台控制函数
 **********************************************************************************************************************/
void Class_PanTilt::Control()
{
    /* 机械臂电机控制计数 */
    this->Control_Counter = (this->Control_Counter + 1) % 2;
    this->Err_Clear_Counter = (this->Err_Clear_Counter + 1) % 400;

    if ((Err_Clear_Counter % 100) != 0) {
        switch (this->PanTilt_Status) {
            case PanTilt_Status_Die:
                this->Control_Die();
                break;
            case PanTilt_Status_Forward:
                this->Control_Forward();
                break;
            case PanTilt_Status_Backward:
                this->Control_Backward();
                break;
            default:
                break;
        }
    }

    //保持使能并清除错误标志位
    switch (Err_Clear_Counter) {
        case 0:
            if (PanTilt_Motor_Yaw.Get_Error() != DM_Motor_Error_NOERROR){
                this->PanTilt_Motor_Yaw.Clear_Error();
            }
            break;
        case 100:
            if (PanTilt_Motor_Pitch.Get_Error() != DM_Motor_Error_NOERROR) {
                this->PanTilt_Motor_Pitch.Clear_Error();
            }
            break;
        case 200:
            this->PanTilt_Motor_Yaw.Enable();
            break;
        case 300:
            this->PanTilt_Motor_Pitch.Enable();
            break;
        default:
            break;
    }
}

/***********************************************************************************************************************
 * @brief 云台死亡状态控制函数
 **********************************************************************************************************************/
void Class_PanTilt::Control_Die()
{
    switch (Control_Counter) {
        case 0:
            this->PanTilt_Motor_Yaw.Enable();
            break;
        case 1:
            this->PanTilt_Motor_Pitch.Enable();
            break;
        default:
            break;
    }
}

/***********************************************************************************************************************
 * @brief 云台正向状态控制函数
 **********************************************************************************************************************/
void Class_PanTilt::Control_Forward()
{
    switch (Control_Counter) {
        case 0:
            this->PanTilt_Motor_Yaw.Control_PositionSpeed();
            break;
        case 1:
            this->PanTilt_Motor_Pitch.Control_PositionSpeed();
            break;
        default:
            break;
    }
}

/***********************************************************************************************************************
 * @brief 云台反向状态控制函数
 **********************************************************************************************************************/
void Class_PanTilt::Control_Backward()
{
    switch (Control_Counter) {
        case 0:
            this->PanTilt_Motor_Yaw.Control_PositionSpeed();
            break;
        case 1:
            this->PanTilt_Motor_Pitch.Control_PositionSpeed();
            break;
        default:
            break;
    }
}




