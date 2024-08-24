/**
 * @file    Remote_Control.cpp
 * @brief   遥控器驱动
 *
 * @author  Li-Chenyang
 * @date    2024-1-19 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Remote_Control.h"

Class_DR16 Remote_Control;

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief DR16遥控器初始化函数
 *
 * @param UART_Manage_Obj               UART处理结构体指针
 **********************************************************************************************************************/
void Class_DR16::Init(Struct_UART_Manage_Object * UART_Manage_Obj)
{
    UART_Manage_Object = UART_Manage_Obj;
    UART_Init(UART_Manage_Object, 18);
}

/***********************************************************************************************************************
 * @brief DR16实际数据接收函数（USART接收回调函数中）
 **********************************************************************************************************************/
void Class_DR16::DataGet(uint8_t * Rx_Data)
{
    auto tmp_buffer = (Struct_DR16_UART_Data *)UART_Manage_Object->Rx_Buffer;

    //滑动窗口, 判断遥控器是否在线
    Flag += 1;

    //摇杆信息
    Data.Right_X = (tmp_buffer->Channel_0 - Rocker_Offset) / Rocker_Num;
    Data.Right_Y = (tmp_buffer->Channel_1 - Rocker_Offset) / Rocker_Num;
    Data.Left_X = (tmp_buffer->Channel_2 - Rocker_Offset) / Rocker_Num;
    Data.Left_Y = (tmp_buffer->Channel_3 - Rocker_Offset) / Rocker_Num;

    //判断拨码触发
    Judge_Switch(&Data.Left_Switch, tmp_buffer->Switch_1, Pre_UART_Rx_Data.Switch_1);
    Judge_Switch(&Data.Right_Switch, tmp_buffer->Switch_2, Pre_UART_Rx_Data.Switch_2);

    //鼠标信息
    Data.Mouse_X = tmp_buffer->Mouse_X / 32768.0f;
    Data.Mouse_Y = tmp_buffer->Mouse_Y / 32768.0f;
    Data.Mouse_Z = tmp_buffer->Mouse_Z / 32768.0f;

    //判断鼠标触发
    Judge_Key(&Data.Mouse_Left_Key, tmp_buffer->Mouse_Left_Key, Pre_UART_Rx_Data.Mouse_Left_Key);
    Judge_Key(&Data.Mouse_Right_Key, tmp_buffer->Mouse_Right_Key, Pre_UART_Rx_Data.Mouse_Right_Key);

    //判断键盘触发
    for (int i = 0; i < 16; i++)
    {
        Judge_Key(&Data.Keyboard_Key[i], ((tmp_buffer->Keyboard_Key) >> i) & 0x1, ((Pre_UART_Rx_Data.Keyboard_Key) >> i) & 0x1);
    }

    //左前轮信息
    Data.Yaw = (tmp_buffer->Channel_Yaw - Rocker_Offset) / Rocker_Num;

    //保留数据
    memcpy(&Pre_UART_Rx_Data, UART_Manage_Object->Rx_Buffer, 18 * sizeof(uint8_t));

    /* 存活状态更新 */
    this->DR16_Status = DR16_Status_ENABLE;
}

/***********************************************************************************************************************
 * @brief DR16存活检测函数（使用此函数才能使能设备）
 **********************************************************************************************************************/
void Class_DR16::AliveCheck()
{
    //判断该时间段内是否接收过遥控器数据
    if (Flag == Pre_Flag)
    {
        //遥控器断开连接
        DR16_Status = DR16_Status_DISABLE;
    }
    else
    {
        //遥控器保持连接
        DR16_Status = DR16_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief 判断拨动开关状态
 *
 */
void Class_DR16::Judge_Switch(Enum_DR16_Switch_Status *Switch, uint8_t Status, uint8_t Pre_Status)
{
    //带触发的判断
    switch (Pre_Status)
    {
        case (SWITCH_UP):
        {
            switch (Status)
            {
                case (SWITCH_UP):
                {
                    *Switch = DR16_Switch_Status_UP;
                }
                    break;
                case (SWITCH_DOWN):
                {
                    *Switch = DR16_Switch_Status_TRIG_MIDDLE_DOWN;
                }
                    break;
                case (SWITCH_MIDDLE):
                {
                    *Switch = DR16_Switch_Status_TRIG_UP_MIDDLE;
                }
                    break;
            }
        }
            break;
        case (SWITCH_DOWN):
        {
            switch (Status)
            {
                case (SWITCH_UP):
                {
                    *Switch = DR16_Switch_Status_TRIG_MIDDLE_UP;
                }
                    break;
                case (SWITCH_DOWN):
                {
                    *Switch = DR16_Switch_Status_DOWN;
                }
                    break;
                case (SWITCH_MIDDLE):
                {
                    *Switch = DR16_Switch_Status_TRIG_DOWN_MIDDLE;
                }
                    break;
            }
        }
            break;
        case (SWITCH_MIDDLE):
        {
            switch (Status)
            {
                case (SWITCH_UP):
                {
                    *Switch = DR16_Switch_Status_TRIG_MIDDLE_UP;
                }
                    break;
                case (SWITCH_DOWN):
                {
                    *Switch = DR16_Switch_Status_TRIG_MIDDLE_DOWN;
                }
                    break;
                case (SWITCH_MIDDLE):
                {
                    *Switch = DR16_Switch_Status_MIDDLE;
                }
                    break;
            }
        }
            break;
    }
}

/**
 * @brief 判断按键状态
 *
 */
void Class_DR16::Judge_Key(Enum_DR16_Key_Status *Key, uint8_t Status, uint8_t Pre_Status)
{
    //带触发的判断
    switch (Pre_Status)
    {
        case KEY_FREE:
            switch (Status)
            {
                case KEY_FREE:
                    *Key = DR16_Key_Status_FREE;
                    break;
                case KEY_PRESSED:
                    *Key = DR16_Key_Status_TRIG_FREE_PRESSED;
                    break;
            }
            break;
        case KEY_PRESSED:
            switch (Status)
            {
                case KEY_FREE:
                    *Key = DR16_Key_Status_TRIG_PRESSED_FREE;
                    break;
                case KEY_PRESSED:
                    *Key = DR16_Key_Status_PRESSED;
                    break;
            }
            break;
    }
}
