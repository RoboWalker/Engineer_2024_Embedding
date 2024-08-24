/**
 * @file    Callback_Tim.cpp
 * @brief   TIM回调函数重写
 *
 * @author  Lichenyang
 * @date    2024-1-17 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Callback_Tim.h"
#include "math.h"

/***********************************************************************************************************************
 * @brief TIM更新中断回调函数重写
 *
 * @param htim  TIM外设句柄
 **********************************************************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim6.Instance)
    {
        /* 系统心跳定时器中断 */
        static uint8_t flag = 0;                /* 系统心跳计数标志 */
        static uint8_t LED_counter = 0;         /* DJI-A板流水灯计数变量 */
        static uint8_t PanTilt_counter = 0;

//        static uint32_t test_counter = 0;
//
//        if (test_counter % 5000 < 2500)
//        {
//            Engineer_Chassis.Store_Motor.Set_Target_Angle(0);
//        }
//        else
//        {
//            Engineer_Chassis.Store_Motor.Set_Target_Angle(0);
//        }
//
//        test_counter++;

        /* 向上位机反馈机械臂数据 */
        if (flag % 20 == 0)
        {
            Engineer_RobotArm.Set_Lift_Position_To_Minipc(Engineer_Chassis.Get_Lift_Front_Angle());
            Engineer_RobotArm.Send_Data_USB();
        }

        /* 底盘控制 */
        if (flag % 2 == 0)
        {
            if (Engineer_PanTilt.Get_Status() == PanTilt_Status_Backward)
            {
                Engineer_Chassis.Set_Forward_Status(false);
            }
            else{
                Engineer_Chassis.Set_Forward_Status(true);
            }

            Engineer_Chassis.Control();
//            Engineer_Chassis.PowerLimit();
//            Engineer_Chassis.OmegaCorrection();
//            Engineer_Chassis.Set_Wheel_Motor_Tx_Out();
            DJI_CAN_SendData();
        }

        PanTilt_counter = (PanTilt_counter + 1) % 7;
        if (PanTilt_counter <= 5)
        {
            /* 机械臂控制 */
            Engineer_RobotArm.Control();
        }
        else
        {
            Engineer_PanTilt.Control();
        }

//        /* 零点设置用 */
//        Motor_EC_Test.Control_Position(1);

        /* 状态检测 */
        flag++;
        if (flag >= 100)
        {
            flag = 0;
            Board.LED_Water_Control(LED_counter);
            LED_counter = (LED_counter + 1) % 8;
            Engineer_Chassis.AliveCheck();
            Engineer_Chassis.DR16->AliveCheck();
            Engineer_RobotArm.AliveCheck();
            Engineer_PanTilt.AliveCheck();
        }

        /* 刷新UI按键 */
        if(Remote_Control.Get_Keyboard_Key_Ctrl() == Enum_DR16_Key_Status::DR16_Key_Status_PRESSED){
            if(Remote_Control.Get_Keyboard_Key_Shift() == Enum_DR16_Key_Status::DR16_Key_Status_PRESSED){
                if(Remote_Control.Get_Keyboard_Key_R() == Enum_DR16_Key_Status::DR16_Key_Status_PRESSED){
                    UI_reset_flag = 1;
                }
                if(Remote_Control.Get_Keyboard_Key_Z() == Enum_DR16_Key_Status::DR16_Key_Status_TRIG_FREE_PRESSED){
                    __set_FAULTMASK(1);
                    HAL_NVIC_SystemReset();
                }
            }
        }

        /* 发送串口绘图数据 */
//        float x1 = Engineer_RobotArm.RobotArm_Mode;
//        float x2 = Engineer_Chassis.Lift_Motor[1].Get_Now_Current();
//        float x3 = Engineer_Chassis.Store_Motor.Get_Now_Omega();
//        float test = Engineer_Chassis.Store_Motor.Get_Now_Current();
//        Serialplot.Set_Data(4, &x1, &x2, &x3, &test);
//        Serialplot.TIM_1ms_Write_PeriodElapsedCallback();
    }

    HAL_IWDG_Refresh(&hiwdg);
}

