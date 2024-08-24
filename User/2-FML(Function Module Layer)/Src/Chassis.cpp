/**
 * @file    Chassis.cpp
 * @brief   麦克纳姆轮底盘（包含升降机构）驱动
 *
 * @author  Li-Chenyang
 * @date    2024-1-18 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Chassis.h"

/* 全局变量创建 --------------------------------------------------------------------------------------------------------*/
Class_Chassis Engineer_Chassis(&Remote_Control, &AHRS_Chassis);

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief 底盘类构造函数
 *
 * @param __DR16        DR16对象指针
 **********************************************************************************************************************/
Class_Chassis::Class_Chassis(Class_DR16 * __DR16, Class_WIT_AHRS * __AHRS)
{
    this->DR16 = __DR16;
    this->AHRS = __AHRS;
}

/***********************************************************************************************************************
 * @brief 底盘初始化函数
 **********************************************************************************************************************/
void Class_Chassis::Init()
{
    uint8_t i;

    /* 设置底盘运动电机PID参数 */
    for (i = 0; i < 4; i++)
    {
        this->Chassis_Motor[i].PID_Omega.Init(3.0f, 15.0f, 0.0f, 0.2f, 20.0f, 20.0f);
    }

    /* 底盘运动电机初始化 */
    this->Chassis_Motor[0].Init(&CAN2_Manage_Object, DJI_Motor_ID_0x202, DJI_Motor_Control_Method_OMEGA, 0.0f, 14.0f, 16384.0f);
    this->Chassis_Motor[1].Init(&CAN2_Manage_Object, DJI_Motor_ID_0x201, DJI_Motor_Control_Method_OMEGA, 0.0f, 14.0f, 16384.0f);
    this->Chassis_Motor[2].Init(&CAN2_Manage_Object, DJI_Motor_ID_0x203, DJI_Motor_Control_Method_OMEGA, 0.0f, 14.0f, 16384.0f);
    this->Chassis_Motor[3].Init(&CAN2_Manage_Object, DJI_Motor_ID_0x204, DJI_Motor_Control_Method_OMEGA, 0.0f, 14.0f, 16384.0f);

    /* 设置升降电机PID参数 */
    this->Lift_Motor[0].PID_Angle.Init(10.0f, 0.5f, 0.0f, 0.0f, 10.0f, 10.0f);
    this->Lift_Motor[1].PID_Angle.Init(10.0f, 0.5f, 0.0f, 0.0f, 10.0f, 10.0f);
    this->Lift_Motor[0].PID_Omega.Init(5.0f, 5.0f, 0.0f, 0.2f, 10.0f, 20.0f);
    this->Lift_Motor[1].PID_Omega.Init(3.0f, 3.0f, 0.0f, 0.2f, 10.0f, 20.0f);

    /* 升降电机初始化 */
    this->Lift_Motor[0].Init(&CAN2_Manage_Object, DJI_Motor_ID_0x205, DJI_Motor_Control_Method_TORQUE, 5.0f);
    this->Lift_Motor[1].Init(&CAN2_Manage_Object, DJI_Motor_ID_0x206, DJI_Motor_Control_Method_TORQUE, 8.0f);

    /* 底盘角速度PID初始化 */
    this->PID_Chassis_Omega.Init(7.0f, 35.0f, 0.0f, 0.0f, 15.0f, 20.0f);
    this->PID_Omega_Revise.Init(2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f);

    /* 设置升降电机PID参数 */
    this->Store_Motor.PID_Angle.Init(10.0f, 10.0f, 0.0f, 0.0f, 1.0f, 10.0f);
    this->Store_Motor.PID_Omega.Init(2.5f, 15.0f, 0.0f, 0.0f, 5.0f, 6.0f);

    /* 升降电机初始化 */
    this->Store_Motor.Init(&CAN2_Manage_Object, DJI_Motor_ID_0x207, DJI_Motor_Control_Method_ANGLE);
}

/***********************************************************************************************************************
 * @brief 底盘存活检测函数（当且仅当全部电机存活时，底盘存活）
 **********************************************************************************************************************/
void Class_Chassis::AliveCheck()
{
    uint8_t i;

    this->Store_Motor.AliveCheck();

    /* 检测底盘运动电机 */
    for (i = 0; i < 4; i++)
    {
        if (this->Chassis_Motor[i].AliveCheck() == DJI_Motor_Status_DISABLE)
        {
            this->Chassis_Status = Chassis_Status_DISABLE;
            return;
        }
    }

    /* 检测升降电机 */
    for (i = 0; i < 2; i++)
    {
        if (this->Lift_Motor[i].AliveCheck() == DJI_Motor_Status_DISABLE)
        {
            this->Chassis_Status = Chassis_Status_DISABLE;
            return;
        }
    }

    this->Chassis_Status = Chassis_Status_ENABLE;
}

/***********************************************************************************************************************
 * @brief 获取遥控器数据
 **********************************************************************************************************************/
void Class_Chassis::DataGet_Remote()
{
    if( (this->DR16->Get_Keyboard_Key_Ctrl() == DR16_Key_Status_FREE) &&
        (this->DR16->Get_Keyboard_Key_Shift() == DR16_Key_Status_FREE)) {
        /* 设置底盘速度 */
        this->Power_Limit_Max = 100.0f;
        if (this->Forward_Status) {
            this->Target_Velocity_X = 0.5f * this->DR16->Get_Keyboard_Key_W() - 0.5f * this->DR16->Get_Keyboard_Key_S();
            this->Target_Velocity_Y = 0.5f * this->DR16->Get_Keyboard_Key_A() - 0.5f * this->DR16->Get_Keyboard_Key_D();
            this->Target_Omega =  -200.0f * this->DR16->Get_Mouse_X() + 0.2f * this->DR16->Get_Keyboard_Key_Q() - 0.2f * this->DR16->Get_Keyboard_Key_E();
            //左边拨码在最下面的时候，才允许使用遥控器动底盘
            if(this->DR16->Get_Left_Switch() == DR16_Switch_Status_DOWN){
                this->Target_Velocity_X += 1.5f * this->DR16->Get_Left_Y();
                this->Target_Velocity_Y += -1.5f * this->DR16->Get_Left_X();
                this->Target_Omega += -1.5f * this->DR16->Get_Right_X();
                this->Adjust_Lift_Front_Angle(this->DR16->Get_Right_Y() * 0.06f);
            }
        }
        else{
            this->Target_Velocity_X = -0.35f * this->DR16->Get_Keyboard_Key_W() +
                               0.35f * this->DR16->Get_Keyboard_Key_S();
            this->Target_Velocity_Y = -0.35f * this->DR16->Get_Keyboard_Key_A() +
                               0.35f * this->DR16->Get_Keyboard_Key_D();
            this->Target_Omega = -200.0f * this->DR16->Get_Mouse_X() + 0.2f * this->DR16->Get_Keyboard_Key_Q() - 0.2f * this->DR16->Get_Keyboard_Key_E();
            //左边拨码在最下面的时候，才允许使用遥控器动底盘
            if(this->DR16->Get_Left_Switch() == DR16_Switch_Status_DOWN){
                this->Target_Velocity_X -= 1.5f * this->DR16->Get_Left_Y();
                this->Target_Velocity_Y -= -1.5f * this->DR16->Get_Left_X();
                this->Target_Omega += -1.5f * this->DR16->Get_Right_X();
                this->Adjust_Lift_Front_Angle(this->DR16->Get_Right_Y() * 0.06f);
            }
        }

        /* 设置升降机构运动状态（限位） */
        if (this->DR16->Get_Left_Switch() == DR16_Switch_Status_UP) {
            /* 机械臂升降机构控制 */
            if ((this->DR16->Get_Yaw() >= 0.3) && (this->Lift_Front_Status != Lift_Status_Top)) {
                /* 未触发底部限位，允许上升 */
                this->Angle_Lift_Front += 0.03f;
            } else if ((this->DR16->Get_Yaw() <= -0.3) && (this->Lift_Front_Status != Lift_Status_Bottom)) {
                /* 未触发顶部限位，允许下降 */
                this->Angle_Lift_Front -= 0.03f;
            }
        } else if (this->DR16->Get_Left_Switch() == DR16_Switch_Status_DOWN) {
            /* 云台升降机构控制 */
            if ((this->DR16->Get_Yaw() >= 0.3) && (this->Lift_Back_Status != Lift_Status_Top)) {
                this->Angle_Lift_Back -= 0.04f;
            } else if ((this->DR16->Get_Yaw() <= -0.3) && (this->Lift_Back_Status != Lift_Status_Bottom)) {
                this->Angle_Lift_Back += 0.04f;
            }
        }
        /* 云台升降机构键盘控制 */
        if ((this->DR16->Get_Keyboard_Key_R() == DR16_Key_Status_PRESSED) &&
            (this->Lift_Back_Status != Lift_Status_Top)) {
            this->Angle_Lift_Back -= 0.05f;
        }
        if (this->DR16->Get_Keyboard_Key_F() == DR16_Key_Status_PRESSED)
        {
            this->Lift_Motor[1].Set_Data_Angle(0.0f);
            this->Angle_Lift_Back = 0.0f;
        }
    }
    if ((this->DR16->Get_Keyboard_Key_Ctrl() == DR16_Key_Status_PRESSED) &&
        (this->DR16->Get_Keyboard_Key_Shift() == DR16_Key_Status_FREE) &&
        (this->DR16->Get_Keyboard_Key_R() == DR16_Key_Status_PRESSED))
    {
        this->Angle_Lift_Back += 0.05f;
    }
    //按住Shift加速
    if( (this->DR16->Get_Keyboard_Key_Ctrl() == DR16_Key_Status_FREE) &&
        (this->DR16->Get_Keyboard_Key_Shift() == DR16_Key_Status_PRESSED)){
        this->Power_Limit_Max = 140.0f;
        if (this->Forward_Status) {
//            this->Target_Velocity_X = 0.9f * this->DR16->Get_Keyboard_Key_W() -
//                    0.9f * this->DR16->Get_Keyboard_Key_S();
//            this->Target_Velocity_Y = 0.9f * this->DR16->Get_Keyboard_Key_A() -
//                    0.9f * this->DR16->Get_Keyboard_Key_D();
//            this->Omega = - 200.0f * this->DR16->Get_Mouse_X() + 0.2f * this->DR16->Get_Keyboard_Key_Q() - 0.2f * this->DR16->Get_Keyboard_Key_E();

            this->Target_Velocity_X = 0.75f * this->DR16->Get_Keyboard_Key_W() - 0.75f * this->DR16->Get_Keyboard_Key_S();
            this->Target_Velocity_Y = 0.75f * this->DR16->Get_Keyboard_Key_A() - 0.75f * this->DR16->Get_Keyboard_Key_D();
            this->Target_Omega =  -100.0f * this->DR16->Get_Mouse_X() + 0.2f * this->DR16->Get_Keyboard_Key_Q() - 0.2f * this->DR16->Get_Keyboard_Key_E();

            if(this->DR16->Get_Left_Switch() == DR16_Switch_Status_DOWN){
                this->Target_Velocity_X += 1.5f * this->DR16->Get_Left_Y();
                this->Target_Velocity_Y += -1.5f * this->DR16->Get_Left_X();
                this->Target_Omega += -1.5f * this->DR16->Get_Right_X();
                this->Adjust_Lift_Front_Angle(this->DR16->Get_Right_Y() * 0.06f);
            }
        }
        else{
            this->Target_Velocity_X = - 0.9f * this->DR16->Get_Keyboard_Key_W() +
                    0.9f * this->DR16->Get_Keyboard_Key_S();
            this->Target_Velocity_Y = - 0.9f * this->DR16->Get_Keyboard_Key_A() +
                    0.9f * this->DR16->Get_Keyboard_Key_D();
            this->Target_Omega = - 200.0f * this->DR16->Get_Mouse_X() + 0.2f * this->DR16->Get_Keyboard_Key_Q() - 0.2f * this->DR16->Get_Keyboard_Key_E();
            if(this->DR16->Get_Left_Switch() == DR16_Switch_Status_DOWN){
                this->Target_Velocity_X -= 1.5f * this->DR16->Get_Left_Y();
                this->Target_Velocity_Y -= -1.5f * this->DR16->Get_Left_X();
                this->Target_Omega += -1.5f * this->DR16->Get_Right_X();
                this->Adjust_Lift_Front_Angle(this->DR16->Get_Right_Y() * 0.06f);
            }
        }
    }
}

/***********************************************************************************************************************
 * @brief 底盘控制函数
 **********************************************************************************************************************/
void Class_Chassis::Control()
{
    static uint32_t high_torque_counter_front = 0;
    float Wheel_Omega[4];       /* 地盘运动电机转速 */
    float Omega_Revise;
    uint8_t i;

    /* 底盘运动斜坡函数 */
    this->Velocity_X = Math_Ramp(this->Target_Velocity_X, this->Velocity_X, 0.006f);
    this->Velocity_Y = Math_Ramp(this->Target_Velocity_Y, this->Velocity_Y, 0.003f);
    this->Omega = Math_Ramp(this->Target_Omega, this->Omega, 0.01f);

    /* 底盘角速度修正 */
    this->PID_Omega_Revise.Set_Target(this->Omega);
    this->PID_Omega_Revise.Set_Now(this->AHRS->Get_Omega_Z());
    this->PID_Omega_Revise.Calculate();
    Omega_Revise = this->PID_Omega_Revise.Get_Out();

    /* 计算底盘运动电机转速 */
    Wheel_Omega[0] = (this->Velocity_X - this->Velocity_Y -
                        (this->Omega + Omega_Revise) * (this->WheelTrack_X + this->WheelTrack_Y) / 2) / this->Radius_Wheel;
    Wheel_Omega[1] = -(this->Velocity_X + this->Velocity_Y +
                        (this->Omega + Omega_Revise) * (this->WheelTrack_X + this->WheelTrack_Y) / 2) / this->Radius_Wheel;
    Wheel_Omega[2] = (this->Velocity_X + this->Velocity_Y -
                        (this->Omega + Omega_Revise) * (this->WheelTrack_X + this->WheelTrack_Y) / 2) / this->Radius_Wheel;
    Wheel_Omega[3] = -(this->Velocity_X - this->Velocity_Y +
                        (this->Omega + Omega_Revise) * (this->WheelTrack_X + this->WheelTrack_Y) / 2) / this->Radius_Wheel;

    /* 底盘运动电机控制 */
    for (i = 0; i < 4; i++)
    {
        this->Chassis_Motor[i].Set_Target_Omega(Wheel_Omega[i]);
        this->Chassis_Motor[i].Control();
    }

    /* 检测机械臂（前）升降机构限位 */
    if (this->Lift_Motor[0].Get_Now_Current() >= 15.0f)
    {
        high_torque_counter_front += 1;
        if (high_torque_counter_front > 250){
            this->Lift_Motor[0].Set_Data_Angle(this->Angle_Lift_Front_Max + 0.2f);
            this->Angle_Lift_Front = this->Angle_Lift_Front_Max + 0.2f;
        }

    }
    else if (this->Lift_Motor[0].Get_Now_Current() <= -10.0f)
    {
        high_torque_counter_front += 1;
        if (high_torque_counter_front > 250) {
            this->Lift_Motor[0].Set_Data_Angle(0.0f);
            this->Angle_Lift_Front = 0.0f;
        }
    }
    else{
        high_torque_counter_front = 0;
    }

    if (this->Angle_Lift_Front <= this->Angle_Lift_Front_Min)
    {
        this->Lift_Front_Status = Lift_Status_Bottom;
        /* 触发底部限位，禁止下降 */
        this->Angle_Lift_Front = Angle_Lift_Front_Min;
    }
    else if (this->Angle_Lift_Front >= this->Angle_Lift_Front_Max)
    {
        this->Lift_Front_Status = Lift_Status_Top;
        /* 触发顶部限位，禁止上升 */
        this->Angle_Lift_Front = Angle_Lift_Front_Max;
    }
    else
    {
        this->Lift_Front_Status = Lift_Status_Middle;
    }

    /* 检测云台（后）升降机构限位 */
    if (this->Lift_Motor[1].Get_Now_Current() <= -8.0f)
    {
        this->Lift_Motor[1].Set_Data_Angle(-(Angle_Lift_Back_Max) - 0.7f);
        this->Angle_Lift_Back = -(Angle_Lift_Back_Max) - 0.7f;
    }
    else if (this->Lift_Motor[1].Get_Now_Current() >= 8.0f)
    {
        this->Lift_Motor[1].Set_Data_Angle(0.0f);
        this->Angle_Lift_Back = 0.0f;
    }

    if (this->Angle_Lift_Back >= -this->Angle_Lift_Back_Min)
    {
        this->Lift_Back_Status = Lift_Status_Bottom;
        this->Angle_Lift_Back -= 0.03f;
    }
    else if (this->Angle_Lift_Back <= -this->Angle_Lift_Back_Max)
    {
        this->Lift_Back_Status = Lift_Status_Top;
        this->Angle_Lift_Back = -this->Angle_Lift_Back_Max;
    }
    else
    {
        this->Lift_Back_Status = Lift_Status_Middle;
    }

    /* 遥控器断开后，升降机构自动复位 */
    if (this->DR16->Get_DR16_Status() == DR16_Status_DISABLE)
    {
        this->Reset_Flag++;
        this->Lift_Motor[0].Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_TORQUE);
        this->Lift_Motor[1].Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_TORQUE);

        /* 力矩控制，使升降机构缓慢下落 */
        if (this->Reset_Flag < 50)
        {
            /* 开始下落 */
            this->Lift_Motor[1].Set_Target_Torque(1.22f);
            this->Lift_Motor[0].Set_Target_Torque(0.61f);
        }
        else
        {
            /* 持续下落 */
            this->Lift_Motor[1].Set_Target_Torque(0.06f);
            this->Lift_Motor[0].Set_Target_Torque(2.08f);
        }

        this->Angle_Lift_Front = 0.0f;
        this->Angle_Lift_Back = 0.0f;
    }
    else
    {
        this->Reset_Flag = 0;
        this->Lift_Motor[0].Set_Target_Angle(this->Angle_Lift_Front);
        this->Lift_Motor[1].Set_Target_Angle(this->Angle_Lift_Back);
        this->Lift_Motor[0].Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_ANGLE);
        this->Lift_Motor[1].Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_ANGLE);
    }

    /* 升降机构电机控制 */
    this->Lift_Motor[0].Control();
    this->Lift_Motor[1].Control();

    if (this->Store_Motor.Get_DJI_Motor_Status() == DJI_Motor_Status_DISABLE)
    {
        this->Store_Status = Store_Status_NotClear;
    }

    if (this->Store_Status == Store_Status_NotClear)
    {
        this->Store_Motor.Set_Target_Angle(this->Store_Motor.Get_Target_Angle() - 0.005f);
    }

    if (abs(this->Store_Motor.Get_Now_Omega()) < 1.0f)
    {
        if (this->Store_Motor.Get_Now_Current() < -5.0f)
        {
            if (++this->Motor_Store_Limit_Counter > 20)
            {
                this->Store_Motor.Set_Data_Angle(-0.5f);
                this->Store_Status = Store_Status_Open;
                this->Motor_Store_Limit_Counter = 0;
            }
        }
        else if (this->Store_Motor.Get_Now_Current() > 5.0f)
        {
            if (++this->Motor_Store_Limit_Counter > 20)
            {
                this->Store_Motor.Set_Data_Angle(3.4f);
                this->Store_Status = Store_Status_Close;
                this->Motor_Store_Limit_Counter = 0;
            }
        }
    }

    if (this->Store_Status == Store_Status_Open)
    {
        this->Store_Motor.Set_Target_Angle(0.0f);
    }
    else if (this->Store_Status == Store_Status_Close)
    {
        this->Store_Motor.Set_Target_Angle(3.1415926f);
    }

    this->Store_Motor.Control();
}

/***********************************************************************************************************************
 * @brief 舵轮底盘功率控制函数（需置于电机输出数据计算之后，数据发送之前）
 **********************************************************************************************************************/
void Class_Chassis::PowerLimit()
{
    float Power_MAX = this->Power_Limit_Max;

    float Available_Power_3508;
    float Consume_Power_3508 = 0.0f;
    float Single_Power_3508[4];
    float Power_Factor_3508 = 1.0f;
    uint8_t i;

    for (i = 0; i < 4; i++)
    {
        /* 获取轮向电机估算功率 */
        Single_Power_3508[i] = this->Chassis_Motor[i].Get_PowerEstimate();

        Consume_Power_3508 += Single_Power_3508[i];
    }

    Available_Power_3508 = Power_MAX;

    if (Consume_Power_3508 >= Available_Power_3508)
    {

        // 功率分配因子计算
        Power_Factor_3508 = Available_Power_3508 / Consume_Power_3508;
    }

    for (i = 0; i < 4; i++)
    {
        /* 对功率大于零轮向电机进行限制 */
        if (Single_Power_3508[i] > 0)
        {
            this->Chassis_Motor[i].Set_OutPower(Single_Power_3508[i] * Power_Factor_3508);
        }
    }
}

/***********************************************************************************************************************
 * @brief 底盘角速度修正
 **********************************************************************************************************************/
 void Class_Chassis::OmegaCorrection()
 {
     float Current_Correction = 0.0f;

     /* 底盘角速度PID控制 */
     this->PID_Chassis_Omega.Set_Target(this->Omega);
     this->PID_Chassis_Omega.Set_Now(this->AHRS->Get_Omega_Z());
     this->PID_Chassis_Omega.Calculate();
     Current_Correction = this->PID_Chassis_Omega.Get_Out();

     this->Chassis_Motor[0].Set_Current_Correction(-Current_Correction);
     this->Chassis_Motor[1].Set_Current_Correction(-Current_Correction);
     this->Chassis_Motor[2].Set_Current_Correction(-Current_Correction);
     this->Chassis_Motor[3].Set_Current_Correction(-Current_Correction);
 }
