/**
 * @file    RobotArm.cpp
 * @brief   机械臂驱动
 *
 * @author  Li-Chenyang
 * @date    2024-1-22 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "RobotArm.h"

/* 全局变量创建 --------------------------------------------------------------------------------------------------------*/
Class_RobotArm Engineer_RobotArm(&Remote_Control, &Referee_System);

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief 底盘类构造函数
 *
 * @param __DR16        DR16对象指针
 **********************************************************************************************************************/
Class_RobotArm::Class_RobotArm(Class_DR16 *__DR16, Class_Referee_System *__Referee_System)
{
    this->DR16 = __DR16;
    this->Referee_System = __Referee_System;
}

/***********************************************************************************************************************
 * @brief 机械臂初始化函数
 **********************************************************************************************************************/
void Class_RobotArm::Init()
{
    static float Motors_Angle_Init[6] = {-0.8f, -0.6f, 2.1f, -1.3f, 0.2f, 0.0f};     /* 机械臂初始位置 */
//    static float Motors_Angle_Init[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};     /* 机械臂初始位置 */

    Target_xyzrpy_Remote[0] = 0.35f;
    Target_xyzrpy_Remote[1] = 0.0f;
    Target_xyzrpy_Remote[2] = 0.0f;
    Target_xyzrpy_Remote[3] = 0.0f;
    Target_xyzrpy_Remote[4] = 0.0f;
    Target_xyzrpy_Remote[5] = 0.0f;

    //气泵复位
    this->Pump_Stop(5000);

    /* 电机基本参数初始化 */
    this->RobotArm_Motor_EC[0].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_1_EC_CAN_ID, -0.7f, 1.5);
    this->RobotArm_Motor_EC[1].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_2_EC_CAN_ID, -3.0f, 0.05f);
    this->RobotArm_Motor_EC[2].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_3_EC_CAN_ID, -3.2f, 3.2f);

    this->RobotArm_Motor_DM[0].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_4_DM_CAN_ID, ROBOTARM_MOTOR_4_DM_MASTER_ID,
                                    DM_Motor_ControlModes_PositionSpeed, -4.0f, 4.0f);
    this->RobotArm_Motor_DM[1].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_5_DM_CAN_ID, ROBOTARM_MOTOR_5_DM_MASTER_ID,
                                    DM_Motor_ControlModes_PositionSpeed, -5.0f, 4.0f);
    this->RobotArm_Motor_DM[2].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_6_DM_CAN_ID, ROBOTARM_MOTOR_6_DM_MASTER_ID,
                                    DM_Motor_ControlModes_PositionSpeed, -5.0f, 5.0f);

    /* 电机控制参数初始化 */
    this->RobotArm_Motor_EC[0].Set_Target_Omega(0.5f);
    this->RobotArm_Motor_EC[1].Set_Target_Omega(0.5f);
    this->RobotArm_Motor_EC[2].Set_Target_Omega(1.0f);

    this->RobotArm_Motor_EC[0].Set_Target_Current(30.0f);
    this->RobotArm_Motor_EC[1].Set_Target_Current(30.0f);
    this->RobotArm_Motor_EC[2].Set_Target_Current(15.0f);

    this->RobotArm_Motor_DM[0].Set_Target_Omega(20.0f);
    this->RobotArm_Motor_DM[1].Set_Target_Omega(20.0f);
    this->RobotArm_Motor_DM[2].Set_Target_Omega(20.0f);

    /* 等待电机开机 */
//    this->RobotArm_Motor_DM[0].Enable_Block();
//    this->RobotArm_Motor_DM[1].Enable_Block();
//    this->RobotArm_Motor_DM[2].Enable_Block();
//
//    this->RobotArm_Motor_EC[0].Set_Acceleration(10.0f, 4);
//    this->RobotArm_Motor_EC[1].Set_Acceleration(10.0f, 4);
//    this->RobotArm_Motor_EC[2].Set_Acceleration(10.0f, 4);

    /* 设置机械臂初始位置 */
    this->Set_Motors_Angle(Motors_Angle_Init);

    this->RobotArm_Status = RobotArm_Status_Die;
}

/***********************************************************************************************************************
 * @brief 机械臂示教初始化函数
 **********************************************************************************************************************/
void Class_RobotArm::Init_Teach()
{
    //气泵复位
    this->Pump_Stop(5000);

    /* 电机基本参数初始化 */
    this->RobotArm_Motor_EC[0].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_1_EC_CAN_ID, -0.75f, 1.5);
    this->RobotArm_Motor_EC[1].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_2_EC_CAN_ID, -2.8f, 0.05f);
    this->RobotArm_Motor_EC[2].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_3_EC_CAN_ID, -2.5f, 2.5f);

    this->RobotArm_Motor_DM[0].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_4_DM_CAN_ID, ROBOTARM_MOTOR_4_DM_MASTER_ID,
                                    DM_Motor_ControlModes_MIT, -3.0f, 3.0f);
    this->RobotArm_Motor_DM[1].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_5_DM_CAN_ID, ROBOTARM_MOTOR_5_DM_MASTER_ID,
                                    DM_Motor_ControlModes_MIT, -3.0f, 2.0f);
    this->RobotArm_Motor_DM[2].Init(&CAN1_Manage_Object, ROBOTARM_MOTOR_6_DM_CAN_ID, ROBOTARM_MOTOR_6_DM_MASTER_ID,
                                    DM_Motor_ControlModes_MIT, -3.2f, 3.2f);

    /* 电机控制参数初始化 */
    this->RobotArm_Motor_EC[0].Set_Target_Torque(0.0f);
    this->RobotArm_Motor_EC[1].Set_Target_Torque(0.0f);
    this->RobotArm_Motor_EC[2].Set_Target_Torque(0.0f);

    this->RobotArm_Motor_EC[0].Set_Target_Current(50.0f);
    this->RobotArm_Motor_EC[1].Set_Target_Current(50.0f);
    this->RobotArm_Motor_EC[2].Set_Target_Current(20.0f);

    this->RobotArm_Motor_DM[0].Set_Target_Torque(0.0f);
    this->RobotArm_Motor_DM[1].Set_Target_Torque(0.0f);
    this->RobotArm_Motor_DM[2].Set_Target_Torque(0.0f);

    this->RobotArm_Motor_DM[0].Set_Control_Coefficient(0.0f, 0.1f);
    this->RobotArm_Motor_DM[1].Set_Control_Coefficient(0.0f, 0.1f);
    this->RobotArm_Motor_DM[2].Set_Control_Coefficient(0.0f, 0.0f);

    /* 等待电机开机 */
    this->RobotArm_Motor_DM[0].Enable_Block();
    this->RobotArm_Motor_DM[1].Enable_Block();
    this->RobotArm_Motor_DM[2].Enable_Block();

    this->RobotArm_Motor_EC[0].Set_Acceleration(2.0f, 4);
    this->RobotArm_Motor_EC[1].Set_Acceleration(2.0f, 4);
    this->RobotArm_Motor_EC[2].Set_Acceleration(2.0f, 4);

    this->RobotArm_Status = RobotArm_Status_Teach;
}

//bool fequal(float a, float b)
//{
//    return fabs(a - b) < 0.0001;
//}

/***********************************************************************************************************************
 * @brief 获取遥控器数据
 **********************************************************************************************************************/
void Class_RobotArm::DataGet_Remote()
{
    //先执行状态切换
    //拨到最下面的时候，从Remote切换到Passive，如果收到上位机数据会切换到Controlled
    if (this->DR16->Get_Left_Switch() == DR16_Switch_Status_DOWN && this->RobotArm_Status == RobotArm_Status_Remote)
    {
        this->RobotArm_Status = RobotArm_Status_Passive;
    }
    //拨到上面或者中间的时候，只要机械臂不死，就切换为Remote
    if (this->DR16->Get_Left_Switch() != DR16_Switch_Status_DOWN && (this->RobotArm_Status == RobotArm_Status_Passive || this->RobotArm_Status == RobotArm_Status_Controlled))
    {
        this->RobotArm_Status = RobotArm_Status_Remote;
        Target_xyzrpy_Remote[0] = 0.35f;
        Target_xyzrpy_Remote[1] = 0.0f;
        Target_xyzrpy_Remote[2] = 0.0f;
        Target_xyzrpy_Remote[3] = 0.0f;
        Target_xyzrpy_Remote[4] = 0.0f;
        Target_xyzrpy_Remote[5] = 0.0f;
    }

    //上位机接管时的操作指令
    if (this->RobotArm_Status == RobotArm_Status_Controlled)
    {
        //Ctrl和Shift没有按下
        if( (this->DR16->Get_Keyboard_Key_Ctrl() == DR16_Key_Status_FREE) &&
            (this->DR16->Get_Keyboard_Key_Shift() == DR16_Key_Status_FREE))
        {
            if (this->DR16->Get_Keyboard_Key_Z() == DR16_Key_Status_PRESSED && (!this->Back_Placed) && (!this->Front_Placed)){
                this->AutoAction_Code = 0x01;
            } else if (this->DR16->Get_Keyboard_Key_X() == DR16_Key_Status_PRESSED && (!this->Front_Placed)) {
                this->AutoAction_Code = 0x02;
            } else if (this->DR16->Get_Keyboard_Key_C() == DR16_Key_Status_PRESSED && (!this->Front_Placed)) {
                this->AutoAction_Code = 0x03;
            } else if (this->DR16->Get_Keyboard_Key_V() == DR16_Key_Status_PRESSED) {
                this->AutoAction_Code = 0x04;
            } else if (this->DR16->Get_Keyboard_Key_G() == DR16_Key_Status_PRESSED) {

                this->AutoAction_Code = 0x07;
            }else if (this->DR16->Get_Keyboard_Key_B() == DR16_Key_Status_PRESSED) {

                this->AutoAction_Code = 0x08; //保护矿石
            } else {
                this->AutoAction_Code = 0x00;
            }
        }
        //Shift没有按下, Ctrl按下
        else if( (this->DR16->Get_Keyboard_Key_Ctrl() == DR16_Key_Status_PRESSED) &&
            (this->DR16->Get_Keyboard_Key_Shift() == DR16_Key_Status_FREE))
        {
            if (this->DR16->Get_Keyboard_Key_Z() == DR16_Key_Status_PRESSED){
                this->AutoAction_Code = 0x01;
            } else if (this->DR16->Get_Keyboard_Key_X() == DR16_Key_Status_PRESSED) {
                this->AutoAction_Code = 0x02;
            } else if (this->DR16->Get_Keyboard_Key_C() == DR16_Key_Status_PRESSED) {
                this->AutoAction_Code = 0x03;
            } else if (this->DR16->Get_Keyboard_Key_V() == DR16_Key_Status_PRESSED) {
                this->AutoAction_Code = 0x04;
            } else if (this->DR16->Get_Keyboard_Key_B() == DR16_Key_Status_PRESSED && (!this->Back_Placed) && (!this->Front_Placed)) {
                this->AutoAction_Code = 0x05;
            }
            else {
                this->AutoAction_Code = 0x00;
            }
        }
        //Ctrl和Shift都按下
        else if( (this->DR16->Get_Keyboard_Key_Ctrl() == DR16_Key_Status_PRESSED) &&
            (this->DR16->Get_Keyboard_Key_Shift() == DR16_Key_Status_PRESSED))
        {
            if (this->DR16->Get_Keyboard_Key_B() == DR16_Key_Status_PRESSED){
                this->AutoAction_Code = 0x05;
            }
            if ((this->DR16->Get_Keyboard_Key_S() == DR16_Key_Status_TRIG_FREE_PRESSED) && (this->FSM_State_Code == 0x00))
            {
                if ((this->RobotArm_Mode == RobotArm_Mode_Normal) || (this->RobotArm_Mode == RobotArm_Mode_ReverseToNormal))
                {
                    this->RobotArm_Mode = RobotArm_Mode_NormalToReverse;
                }
                else if ((this->RobotArm_Mode == RobotArm_Mode_Reverse) || (this->RobotArm_Mode == RobotArm_Mode_NormalToReverse))
                {
                    this->RobotArm_Mode = RobotArm_Mode_ReverseToNormal;
                }
            }
        }
    }
    //遥控器模式下的操作指令
    else if (this->RobotArm_Status == RobotArm_Status_Remote)
    {
        if(DR16->Get_Right_Switch() == DR16_Switch_Status_UP)
        {
            this->Set_Pump_Status(Pump_Status_ON);
        }
        else
        {
            this->Set_Pump_Status(Pump_Status_OFF);
        }
        if (DR16->Get_Left_Switch() == DR16_Switch_Status_MIDDLE)
        {
            Target_xyzrpy_Remote[0] += DR16->Get_Right_Y() * 0.002f;
            Target_xyzrpy_Remote[1] -= DR16->Get_Left_X() * 0.002f;
            Target_xyzrpy_Remote[2] += DR16->Get_Left_Y() * 0.002f;
            Target_xyzrpy_Remote[3] += 4.0f * DR16->Get_Right_X() * 0.002f;
            Target_xyzrpy_Remote[4] += 4.0f * DR16->Get_Yaw() * 0.002f;
        }
        if (DR16->Get_Left_Switch() == DR16_Switch_Status_UP)
        {
            Target_xyzrpy_Remote[5] += 4.0f * DR16->Get_Right_X() * 0.002f;
        }
    }
}

/***********************************************************************************************************************
 * @brief 获取上位机数据
 **********************************************************************************************************************/
void Class_RobotArm::DataGet_USB(uint8_t *Rx_Buf)
{
    static uint32_t flag = 0, flag_1 = 0;

    /* 机械臂电机角度，共24字节, float, Byte[1]~Byte[24] */
    memcpy((void *)this->Motors_Target_Angle, (const void *)(Rx_Buf + 1), 24);

    if ((this->RobotArm_Mode == RobotArm_Mode_NormalToReverse) || (this->RobotArm_Mode == RobotArm_Mode_Reverse))
    {
        this->Motors_Target_Angle[4] += 3.1415926f;
        this->Motors_Target_Angle[5] = -this->Motors_Target_Angle[5] + 3.1415926f;
    }
    this->Set_Motors_Angle(this->Motors_Target_Angle);

    if (this->RobotArm_Mode == RobotArm_Mode_NormalToReverse)
    {
        if ((fabs(this->Motors_Target_Angle[4] + this->RobotArm_Motor_DM[1].Get_Angle()) < 0.1f) &&
            (fabs(this->Motors_Target_Angle[5] - this->RobotArm_Motor_DM[2].Get_Angle()) < 0.1f))
        {
            this->RobotArm_Mode = RobotArm_Mode_Reverse;
        }
    }
    else if (this->RobotArm_Mode == RobotArm_Mode_ReverseToNormal)
    {
        if ((fabs(this->Motors_Target_Angle[4] + this->RobotArm_Motor_DM[1].Get_Angle()) < 0.1f) &&
            (fabs(this->Motors_Target_Angle[5] - this->RobotArm_Motor_DM[2].Get_Angle()) < 0.1f))
        {
            this->RobotArm_Mode = RobotArm_Mode_Normal;
        }
    }

    /* 气泵状态, Byte[25].bit0 */
    if (this->RobotArm_Status == RobotArm_Status_Controlled){
        if (Rx_Buf[25] & 0x01)
        {
            this->Pump_Status = Pump_Status_ON;
        }
        else
        {
            this->Pump_Status = Pump_Status_OFF;
        }
    }

    /* 机械臂电机速度, Byte[25].bit4 */
    if ((Rx_Buf[25] & (0x01 << 4)) && ((this->RobotArm_Mode == RobotArm_Mode_Normal) || (this->RobotArm_Mode == RobotArm_Mode_Reverse)))
    {
        this->RobotArm_Motor_EC[0].Set_Target_Omega(5.0f);
        this->RobotArm_Motor_EC[1].Set_Target_Omega(5.0f);
        this->RobotArm_Motor_EC[2].Set_Target_Omega(5.0f);
        this->RobotArm_Motor_DM[0].Set_Target_Omega(5.0f);
        this->RobotArm_Motor_DM[1].Set_Target_Omega(5.0f);
        this->RobotArm_Motor_DM[2].Set_Target_Omega(5.0f);
    }
    else
    {
        this->RobotArm_Motor_EC[0].Set_Target_Omega(0.45f);
        this->RobotArm_Motor_EC[1].Set_Target_Omega(0.45f);
        this->RobotArm_Motor_EC[2].Set_Target_Omega(0.8f);
        this->RobotArm_Motor_DM[0].Set_Target_Omega(2.0f);
        this->RobotArm_Motor_DM[1].Set_Target_Omega(3.0f);
        this->RobotArm_Motor_DM[2].Set_Target_Omega(3.0f);
    }

    /* 在被动状态下接收到上位机数据，则进入受控状态 */
    if (this->RobotArm_Status == RobotArm_Status_Passive)
    {
        float motors_angle[6];
        this->Get_Motors_Angle(motors_angle);
        if ((motors_angle[0] >= -1.5f) && (motors_angle[1] >= -1.5f) && (motors_angle[2] >= 0.0f))
        {
            this->RobotArm_Status = RobotArm_Status_Controlled;
        }
        else if (Engineer_Chassis.Get_Lift_Front_Angle() >= 14.0f)
        {
            this->RobotArm_Status = RobotArm_Status_Controlled;
        }
    }
    this->FSM_State_Code = Rx_Buf[26];
    if (this->FSM_State_Code == 0x01){
        Back_Placed = true;
    }
    if (this->FSM_State_Code == 0x02){
        Front_Placed = true;
    }
    if (this->FSM_State_Code == 0x03){
        Back_Placed = false;
    }
    if (this->FSM_State_Code == 0x04){
        Front_Placed = false;
    }
    if (this->FSM_State_Code == 0x05){
        Back_Placed = true;
        Front_Placed = true;
    }

    if ((this->FSM_State_Code != 0x00) && (this->RobotArm_Mode != RobotArm_Mode_Normal))
    {
        this->RobotArm_Mode = RobotArm_Mode_ReverseToNormal;
    }
}

/***********************************************************************************************************************
 * @brief 机械臂控制函数
 **********************************************************************************************************************/
void Class_RobotArm::Control()
{
    /* 机械臂电机控制计数 */
    this->Control_Counter = (this->Control_Counter + 1) % 6;
    this->Err_Clear_Counter = (this->Err_Clear_Counter + 1) % 600;

    /* 根据控制状态选择控制函数 */
    if ((Err_Clear_Counter % 100) != 0){
        switch (this->RobotArm_Status)
        {
            case RobotArm_Status_Die:
                this->Control_Die();
                break;
            case RobotArm_Status_Passive:
                this->Control_Passive();
                break;
            case RobotArm_Status_Controlled:
                this->Control_Controlled();
                break;
            case RobotArm_Status_Teach:
                this->Control_Teach();
                break;
            case RobotArm_Status_Remote:
                this->Control_Remote();
                break;
            default:
                break;
        }
    }

    //保持使能并清除错误标志位
    switch (Err_Clear_Counter) {
        case 0:
            if (RobotArm_Motor_DM[0].Get_Error() != DM_Motor_Error_NOERROR){
                this->RobotArm_Motor_DM[0].Clear_Error();
            }
            break;
        case 100:
            if (RobotArm_Motor_DM[1].Get_Error() != DM_Motor_Error_NOERROR) {
                this->RobotArm_Motor_DM[1].Clear_Error();
            }
            break;
        case 200:
            if (RobotArm_Motor_DM[2].Get_Error() != DM_Motor_Error_NOERROR) {
                this->RobotArm_Motor_DM[2].Clear_Error();
            }
            break;
        case 300:
            this->RobotArm_Motor_DM[0].Enable();
            break;
        case 400:
            this->RobotArm_Motor_DM[1].Enable();
            break;
        case 500:
            this->RobotArm_Motor_DM[2].Enable();
            break;
        default:
            break;
    }

//    if (this->RobotArm_Mode == RobotArm_Mode_NormalToReverse)
//    {
//        if ((fabs(this->Motors_Target_Angle[4] - this->RobotArm_Motor_DM[1].Get_Angle()) < 0.1f) &&
//            (fabs(this->Motors_Target_Angle[5] - this->RobotArm_Motor_DM[2].Get_Angle()) < 0.1f))
//        {
//            this->RobotArm_Mode = RobotArm_Mode_Reverse;
//        }
//    }
//    else if (this->RobotArm_Mode == RobotArm_Mode_ReverseToNormal)
//    {
//        if ((fabs(this->Motors_Target_Angle[4] - this->RobotArm_Motor_DM[1].Get_Angle()) < 0.1f) &&
//            (fabs(this->Motors_Target_Angle[5] - this->RobotArm_Motor_DM[2].Get_Angle()) < 0.1f))
//        {
//            this->RobotArm_Mode = RobotArm_Mode_Normal;
//        }
//    }

    /* 气泵+电磁阀控制 */
    if (this->RobotArm_Status == RobotArm_Status_Controlled || this->RobotArm_Status == RobotArm_Status_Remote) {
        if (this->Pump_Status == Pump_Status_ON) {
            this->Valve_Counter = 0;
            this->Pump_Start();
        } else {
            this->Valve_Counter++;
            this->Pump_Stop(Valve_Counter);
        }
    }
    else{
        Pump_Stop(5000);    //设置大于3000的值，气泵和电磁阀一起关闭
    }
}

/***********************************************************************************************************************
 * @brief 机械臂控制函数——死亡状态（使能电机）
 **********************************************************************************************************************/
void Class_RobotArm::Control_Die()
{
    //尝试重新初始化电机
    switch (this->Control_Counter)
    {
        case 0:
            this->RobotArm_Motor_EC[0].Set_Target_Torque(0.0f);
            this->RobotArm_Motor_EC[0].Control_CurrentTorque(1, 1);
            break;
        case 1:
            this->RobotArm_Motor_EC[1].Set_Target_Torque(0.0f);
            this->RobotArm_Motor_EC[1].Control_CurrentTorque(1, 1);
            break;
        case 2:
            this->RobotArm_Motor_EC[2].Set_Target_Torque(0.0f);
            this->RobotArm_Motor_EC[2].Control_CurrentTorque(1, 1);
            break;
        case 3:
            this->RobotArm_Motor_DM[0].Enable();
            break;
        case 4:
            this->RobotArm_Motor_DM[1].Enable();
            break;
        case 5:
            this->RobotArm_Motor_DM[2].Enable();
            break;
        default:
            break;
    }
}

/***********************************************************************************************************************
 * @brief 机械臂控制函数——被动状态（此状态仅在机械臂死亡复活后进入）
 **********************************************************************************************************************/
void Class_RobotArm::Control_Passive()
{
    switch (this->Control_Counter)
    {
        case 0:
            this->RobotArm_Motor_EC[0].Set_Target_Torque(-7.5f * cos(-this->RobotArm_Motor_EC[0].Get_Angle())
                                                         * cos(this->RobotArm_Motor_EC[1].Get_Angle())
                                                         - 3.5f * cos(this->RobotArm_Motor_EC[1].Get_Angle() - this->RobotArm_Motor_EC[2].Get_Angle())
                                                           * cos(-this->RobotArm_Motor_EC[0].Get_Angle()));
            this->RobotArm_Motor_EC[0].Control_CurrentTorque(1, 1);
            break;
        case 1:
            this->RobotArm_Motor_EC[1].Set_Target_Torque((6.5f * sin(this->RobotArm_Motor_EC[1].Get_Angle())
                                                          + 3.7f * sin(this->RobotArm_Motor_EC[1].Get_Angle() - this->RobotArm_Motor_EC[2].Get_Angle()))
                                                         * sin(this->RobotArm_Motor_EC[0].Get_Angle()));
            this->RobotArm_Motor_EC[1].Control_CurrentTorque(1, 1);
            break;
        case 2:
            this->RobotArm_Motor_EC[2].Set_Target_Torque(2.4f * sin(this->RobotArm_Motor_EC[1].Get_Angle() - this->RobotArm_Motor_EC[2].Get_Angle())
                                                         * sin(-this->RobotArm_Motor_EC[0].Get_Angle()));
            this->RobotArm_Motor_EC[2].Control_CurrentTorque(1, 1);
            break;
        case 3:
            this->RobotArm_Motor_DM[0].Control_PositionSpeed();
            break;
        case 4:
            this->RobotArm_Motor_DM[1].Control_PositionSpeed();
            break;
        case 5:
            this->RobotArm_Motor_DM[2].Control_PositionSpeed();
            break;
    }
}

/***********************************************************************************************************************
 * @brief 机械臂控制函数——自定义控制器控制状态
 **********************************************************************************************************************/
void Class_RobotArm::Control_Controlled()
{
    /* 直接使用来自上位机的角度数据 */
    switch (this->Control_Counter)
    {
        case 0:
            this->RobotArm_Motor_EC[0].Control_Position(1);
            break;
        case 1:
            this->RobotArm_Motor_EC[1].Control_Position(1);
            break;
        case 2:
            this->RobotArm_Motor_EC[2].Control_Position(1);
            break;
        case 3:
            this->RobotArm_Motor_DM[0].Control_PositionSpeed();
            break;
        case 4:
            this->RobotArm_Motor_DM[1].Control_PositionSpeed();
            break;
        case 5:
            this->RobotArm_Motor_DM[2].Control_PositionSpeed();
            break;
    }
}

/***********************************************************************************************************************
 * @brief 机械臂控制函数——示教（重力补偿）状态
 **********************************************************************************************************************/
void Class_RobotArm::Control_Teach()
{
    switch (this->Control_Counter)
    {
        case 0:
            this->RobotArm_Motor_EC[0].Set_Target_Torque(-7.5f * cos(-this->RobotArm_Motor_EC[0].Get_Angle())
                                                         * cos(this->RobotArm_Motor_EC[1].Get_Angle())
                                                         - 3.5f * cos(this->RobotArm_Motor_EC[1].Get_Angle() - this->RobotArm_Motor_EC[2].Get_Angle())
                                                           * cos(-this->RobotArm_Motor_EC[0].Get_Angle()));
            this->RobotArm_Motor_EC[0].Control_CurrentTorque(1, 1);
            break;
        case 1:
            this->RobotArm_Motor_EC[1].Set_Target_Torque((7.0f * sin(this->RobotArm_Motor_EC[1].Get_Angle())
                                                          + 3.0f * sin(this->RobotArm_Motor_EC[1].Get_Angle() - this->RobotArm_Motor_EC[2].Get_Angle()))
                                                         * sin(this->RobotArm_Motor_EC[0].Get_Angle()));
            this->RobotArm_Motor_EC[1].Control_CurrentTorque(1, 1);
            break;
        case 2:
            this->RobotArm_Motor_EC[2].Set_Target_Torque(2.4f * sin(this->RobotArm_Motor_EC[1].Get_Angle() - this->RobotArm_Motor_EC[2].Get_Angle())
                                                         * sin(-this->RobotArm_Motor_EC[0].Get_Angle()));
            this->RobotArm_Motor_EC[2].Control_CurrentTorque(1, 1);
            break;
        case 3:
            this->RobotArm_Motor_DM[0].Control_MIT();
            break;
        case 4:
            this->RobotArm_Motor_DM[1].Control_MIT();
            break;
        case 5:
            this->RobotArm_Motor_DM[2].Control_MIT();
            break;
    }
}

/***********************************************************************************************************************
 * @brief 机械臂存活检测函数
 **********************************************************************************************************************/
void Class_RobotArm::AliveCheck()
{
    uint8_t i;

    if ((this->RobotArm_Status == RobotArm_Status_Null) || (this->RobotArm_Status == RobotArm_Status_Teach))
    {
        return;
    }

    uint8_t alive_count = 0;

    /* 若存在电机处于失能状态，判定机械臂死亡 */
    for (i = 0; i < 3; i++)
    {
        if (this->RobotArm_Motor_EC[i].AliveCheck() == EC_Motor_Status_DISABLE)
        {
            alive_count ++;
        }

        if (this->RobotArm_Motor_DM[i].AliveCheck() == DM_Motor_Status_DISABLE)
        {
            alive_count ++;
        }
    }

    if (alive_count == 6){
        this->RobotArm_Status = RobotArm_Status_Die;
        return;
    }

    //裁判系统云台接口断电，视为死亡
    //经测试裁判系统延迟太大，故放弃
//    if (this->Referee_System->robot_status.power_management_gimbal_output == 0){
//        this->RobotArm_Status = RobotArm_Status_Die;
//        return;
//    }

    /* 机械臂复活后进入 passive 状态 */
    if (this->RobotArm_Status == RobotArm_Status_Die)
    {
        this->RobotArm_Status = RobotArm_Status_Passive;
    }
}

/***********************************************************************************************************************
 * @brief 打开气泵，关闭阀门
 **********************************************************************************************************************/
void Class_RobotArm::Pump_Start()
{
    HAL_GPIO_WritePin(AIR_PUMP_GPIO_Port, AIR_PUMP_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SOLENOID_VALVE_GPIO_Port, SOLENOID_VALVE_Pin, GPIO_PIN_RESET);
}

/***********************************************************************************************************************
 * @brief 关闭气泵，打开阀门
 **********************************************************************************************************************/
void Class_RobotArm::Pump_Stop(uint16_t Valve_Counter)
{
    HAL_GPIO_WritePin(AIR_PUMP_GPIO_Port, AIR_PUMP_Pin, GPIO_PIN_RESET);

    if (Valve_Counter <= 3000)
    {
        HAL_GPIO_WritePin(SOLENOID_VALVE_GPIO_Port, SOLENOID_VALVE_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(SOLENOID_VALVE_GPIO_Port, SOLENOID_VALVE_Pin, GPIO_PIN_RESET);
    }
}

/***********************************************************************************************************************
 * @brief 发送数据到上位机
 **********************************************************************************************************************/
void Class_RobotArm::Send_Data_USB()
{
    static uint8_t Tx_Buffer[33] = {};
    static float Motor_Angle[6];
    uint8_t i;

    if (this->RobotArm_Mode == RobotArm_Mode_Reverse)
    {
        Get_Motors_Angle(Motor_Angle);
        Motor_Angle[4] -= 3.1415926f;
        Motor_Angle[5] = -Motor_Angle[5] + 3.1415926f;
    }
    else if (this->RobotArm_Mode == RobotArm_Mode_Normal)
    {
        Get_Motors_Angle(Motor_Angle);
    }

    Tx_Buffer[0] = 0xAC;
    for (i = 0; i < 6; i++)
    {
        memcpy((void *)(Tx_Buffer + i * 4 + 1), (const void *)(Motor_Angle + i), 4);
    }

    if (this->RobotArm_Status != RobotArm_Status_Die)
    {
        Tx_Buffer[25] |= 0x01;
    }
    else
    {
        Tx_Buffer[25] &= ~0x01;
    }

    Tx_Buffer[26] = this->AutoAction_Code;
    Tx_Buffer[27] = Math_Float_To_Uint(Engineer_Chassis.Get_Lift_Front_Angle(),
                                       Engineer_Chassis.Angle_Lift_Front_Min,
                                       Engineer_Chassis.Angle_Lift_Front_Max, 8);

    CDC_Transmit_FS(Tx_Buffer, 33);
}

/***********************************************************************************************************************
 * @brief 遥控器模式下的解算函数
 **********************************************************************************************************************/
void Class_RobotArm::IK_Remote()
{
    float Motors_Target_Angle_Temp[6];
    static float Target_xyzrpy_Remote_Last[6] = {0.35f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    //目标位置限幅
    Math_Constrain(&this->Target_xyzrpy_Remote[0], 0.20f, 0.55f);
    if (this->Lift_Position_To_Minipc > 64){
        Math_Constrain(&this->Target_xyzrpy_Remote[1], -0.35f, 0.35f);
    }
    else{
        Math_Constrain(&this->Target_xyzrpy_Remote[1], -0.35f, 0.00f);
    }
    Math_Constrain(&this->Target_xyzrpy_Remote[1], -0.15f, 0.20f);
    Math_Constrain(&this->Target_xyzrpy_Remote[2], -0.15f, 0.30f);

    //大小臂长度
    float_t lf = 0.325;
    float_t lt = 0.240;
    //计算虚拟臂长度
    float_t x = this->Target_xyzrpy_Remote[0];
    float_t y = this->Target_xyzrpy_Remote[1];
    float_t z = this->Target_xyzrpy_Remote[2];
    float_t roll = this->Target_xyzrpy_Remote[3];
    float_t pitch = this->Target_xyzrpy_Remote[4];
    float_t yaw = this->Target_xyzrpy_Remote[5];
    float_t lVirtual = sqrt(x*x + y*y + z*z);
    float sigma, alpha, beta, theta;
    if (lVirtual > lf + lt - 0.002)
    {
        //将Target_xyzrpy_Remote重新设置为上一次的值
        for (int i = 0; i < 6; ++i) {
            Target_xyzrpy_Remote[i] = Target_xyzrpy_Remote_Last[i];
        }
        return; //无解
    }
    if (x < 0.15)
    {
        //将Target_xyzrpy_Remote重新设置为上一次的值
        for (int i = 0; i < 6; ++i) {
            Target_xyzrpy_Remote[i] = Target_xyzrpy_Remote_Last[i];
        }
        return;
    }

    sigma = asin(y/lVirtual);
    alpha = acos((lVirtual * lVirtual + lf * lf - lt * lt) / (2 * lf * lVirtual));
    beta = acos((lf*lf +lt*lt - lVirtual * lVirtual) / (2 * lt * lf));
    theta = asin(z/x);

    if (isnan(sigma) || isnan(alpha) || isnan(beta) || isnan(theta))
    {
        //将Target_xyzrpy_Remote重新设置为上一次的值
        for (int i = 0; i < 6; ++i) {
            Target_xyzrpy_Remote[i] = Target_xyzrpy_Remote_Last[i];
        }
        return;
    }

    Motors_Target_Angle_Temp[0] = theta;
    Motors_Target_Angle_Temp[1] = -alpha - sigma;
    Motors_Target_Angle_Temp[2] = -beta + (float)M_PI;
    Motors_Target_Angle_Temp[3] = -Motors_Target_Angle_Temp[1] - Motors_Target_Angle_Temp[2] + this->Target_xyzrpy_Remote[5];
    Motors_Target_Angle_Temp[4] = theta + this->Target_xyzrpy_Remote[4];
    Motors_Target_Angle_Temp[5] = this->Target_xyzrpy_Remote[3];

    //超出角度范围，返回
    if (Motors_Target_Angle_Temp[0] > 0.7f || Motors_Target_Angle_Temp[0] < -1.5f)
    {
        //将Target_xyzrpy_Remote重新设置为上一次的值
        for (int i = 0; i < 6; ++i) {
            Target_xyzrpy_Remote[i] = Target_xyzrpy_Remote_Last[i];
        }
        return;
    }
    if (Motors_Target_Angle_Temp[1] < -2.8f || Motors_Target_Angle_Temp[1] > 0.0f)
    {
        //将Target_xyzrpy_Remote重新设置为上一次的值
        for (int i = 0; i < 6; ++i) {
            Target_xyzrpy_Remote[i] = Target_xyzrpy_Remote_Last[i];
        }
        return;
    }
    if (Motors_Target_Angle_Temp[2] < -2.5f || Motors_Target_Angle_Temp[2] > 2.5f)
    {
        //将Target_xyzrpy_Remote重新设置为上一次的值
        for (int i = 0; i < 6; ++i) {
            Target_xyzrpy_Remote[i] = Target_xyzrpy_Remote_Last[i];
        }
        return;
    }
    if (Motors_Target_Angle_Temp[3] < -3.0f || Motors_Target_Angle_Temp[3] > 3.0f)
    {
        //将Target_xyzrpy_Remote重新设置为上一次的值
        for (int i = 0; i < 6; ++i) {
            Target_xyzrpy_Remote[i] = Target_xyzrpy_Remote_Last[i];
        }
        return;
    }
    if (Motors_Target_Angle_Temp[4] < -3.0f || Motors_Target_Angle_Temp[4] > 3.0f)
    {
        //将Target_xyzrpy_Remote重新设置为上一次的值
        for (int i = 0; i < 6; ++i) {
            Target_xyzrpy_Remote[i] = Target_xyzrpy_Remote_Last[i];
        }
        return;
    }
    if (Motors_Target_Angle_Temp[5] < -4.0f || Motors_Target_Angle_Temp[5] > 4.0f)
    {
        //将Target_xyzrpy_Remote重新设置为上一次的值
        for (int i = 0; i < 6; ++i) {
            Target_xyzrpy_Remote[i] = Target_xyzrpy_Remote_Last[i];
        }
        return;
    }

    //可解，更新上一次的值
    for (int i = 0; i < 6; ++i) {
        Target_xyzrpy_Remote_Last[i] = Target_xyzrpy_Remote[i];
    }

    for (int i = 0; i < 6; ++i) {
        this->Motors_Target_Angle[i] = Motors_Target_Angle_Temp[i];
    }
}

void Class_RobotArm::Control_Remote()
{
    IK_Remote();
    this->Set_Motors_Angle(this->Motors_Target_Angle);

    this->RobotArm_Motor_EC[0].Set_Target_Omega(0.5f);
    this->RobotArm_Motor_EC[1].Set_Target_Omega(0.5f);
    this->RobotArm_Motor_EC[2].Set_Target_Omega(0.8f);
    this->RobotArm_Motor_DM[0].Set_Target_Omega(1.0f);
    this->RobotArm_Motor_DM[1].Set_Target_Omega(1.0f);
    this->RobotArm_Motor_DM[2].Set_Target_Omega(2.0f);
    switch (this->Control_Counter)
    {
        case 0:
            this->RobotArm_Motor_EC[0].Control_Position(1);
            break;
        case 1:
            this->RobotArm_Motor_EC[1].Control_Position(1);
            break;
        case 2:
            this->RobotArm_Motor_EC[2].Control_Position(1);
            break;
        case 3:
            this->RobotArm_Motor_DM[0].Control_PositionSpeed();
            break;
        case 4:
            this->RobotArm_Motor_DM[1].Control_PositionSpeed();
            break;
        case 5:
            this->RobotArm_Motor_DM[2].Control_PositionSpeed();
            break;
    }
}

