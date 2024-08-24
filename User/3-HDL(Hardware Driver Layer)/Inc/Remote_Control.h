/**
 * @file    Remote_Control.h
 * @brief   遥控器驱动
 *
 * @author  Li-Chenyang
 * @date    2024-1-19 （创建）
 *
 */

#ifndef __HDL_REMOTE_CONTROL_H
#define __HDL_REMOTE_CONTROL_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include <limits.h>
#include <string.h>
#include "User_Usart.h"

/* 宏定义 -------------------------------------------------------------------------------------------------------------*/
//拨动开关位置
#define SWITCH_UP               1
#define SWITCH_DOWN             2
#define SWITCH_MIDDLE           3

//按键开关位置
#define KEY_FREE                0
#define KEY_PRESSED             1

//键位宏定义
#define KEY_W                   0
#define KEY_S                   1
#define KEY_A                   2
#define KEY_D                   3
#define KEY_SHIFT               4
#define KEY_CTRL                5
#define KEY_Q                   6
#define KEY_E                   7
#define KEY_R                   8
#define KEY_F                   9
#define KEY_G                   10
#define KEY_Z                   11
#define KEY_X                   12
#define KEY_C                   13
#define KEY_V                   14
#define KEY_B                   15

/* 枚举类型定义 --------------------------------------------------------------------------------------------------------*/
/**
 * @brief 遥控器状态
 *
 */
enum Enum_DR16_Status
{
    DR16_Status_DISABLE = 0,
    DR16_Status_ENABLE,
};

/**
 * @brief 拨动开关状态
 *
 */
enum Enum_DR16_Switch_Status
{
    DR16_Switch_Status_UP = 0,
    DR16_Switch_Status_TRIG_UP_MIDDLE,
    DR16_Switch_Status_TRIG_MIDDLE_UP,
    DR16_Switch_Status_MIDDLE,
    DR16_Switch_Status_TRIG_MIDDLE_DOWN,
    DR16_Switch_Status_TRIG_DOWN_MIDDLE,
    DR16_Switch_Status_DOWN,
};

/**
 * @brief 按键状态
 *
 */
enum Enum_DR16_Key_Status
{
    DR16_Key_Status_FREE = 0,
    DR16_Key_Status_TRIG_FREE_PRESSED,
    DR16_Key_Status_TRIG_PRESSED_FREE,
    DR16_Key_Status_PRESSED,
};

/* 结构体定义 ----------------------------------------------------------------------------------------------------------*/
/**
 * @brief DR16源数据
 *
 */
struct Struct_DR16_UART_Data
{
    uint64_t Channel_0 : 11;
    uint64_t Channel_1 : 11;
    uint64_t Channel_2 : 11;
    uint64_t Channel_3 : 11;
    uint64_t Switch_2 : 2;
    uint64_t Switch_1 : 2;
    int16_t Mouse_X;
    int16_t Mouse_Y;
    int16_t Mouse_Z;
    uint64_t Mouse_Left_Key : 8;
    uint64_t Mouse_Right_Key : 8;
    uint64_t Keyboard_Key : 16;
    uint64_t Channel_Yaw : 11;
} __attribute__((packed));

/**
 * @brief DR16经过处理的的数据, 摇杆信息经过归一化到-1~1, 鼠标信息有待进一步标定
 *
 */
struct Struct_DR16_Data
{
    float Right_X;
    float Right_Y;
    float Left_X;
    float Left_Y;
    Enum_DR16_Switch_Status Left_Switch;
    Enum_DR16_Switch_Status Right_Switch;
    float Mouse_X;
    float Mouse_Y;
    float Mouse_Z;
    Enum_DR16_Key_Status Mouse_Left_Key;
    Enum_DR16_Key_Status Mouse_Right_Key;
    Enum_DR16_Key_Status Keyboard_Key[16];
    float Yaw;
};

/* 类定义 -------------------------------------------------------------------------------------------------------------*/
/**
 * @brief DR16遥控器类
 *
 */
class Class_DR16
{
public:
    /* 函数 */
    void Init(Struct_UART_Manage_Object * UART_Manage_Obj);
    void DataGet(uint8_t * Rx_Data);
    void AliveCheck();

    inline Enum_DR16_Status Get_DR16_Status();
    inline float Get_Right_X();
    inline float Get_Right_Y();
    inline float Get_Left_X();
    inline float Get_Left_Y();
    inline Enum_DR16_Switch_Status Get_Left_Switch();
    inline Enum_DR16_Switch_Status Get_Right_Switch();
    inline float Get_Mouse_X();
    inline float Get_Mouse_Y();
    inline float Get_Mouse_Z();
    inline Enum_DR16_Key_Status Get_Mouse_Left_Key();
    inline Enum_DR16_Key_Status Get_Mouse_Right_Key();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_W();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_S();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_A();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_D();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_Shift();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_Ctrl();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_Q();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_E();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_R();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_F();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_G();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_Z();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_X();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_C();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_V();
    inline Enum_DR16_Key_Status Get_Keyboard_Key_B();
    inline float Get_Yaw();
protected:
    /* 常量 */
    Struct_UART_Manage_Object * UART_Manage_Object;     /*!< 绑定的UART */
    float Rocker_Offset = 1024.0f;                      /*!< 摇杆偏移量 */
    float Rocker_Num = 660.0f;                          /*!< 摇杆总刻度 */

    /* 读变量 */
    Struct_DR16_Data Data;                              /*!< DR16对外接口信息 */

    /* 内部变量 */
    uint32_t Flag = 0;                                  /*!< 当前时刻的遥控器接收flag */
    uint32_t Pre_Flag = 0;                              /*!< 前一时刻的遥控器接收flag */
    Enum_DR16_Status DR16_Status = DR16_Status_DISABLE; /*!< 遥控器状态 */
    Struct_DR16_UART_Data Pre_UART_Rx_Data;             /*!< 前一时刻的遥控器数据 */

    /* 内部函数 */
    void Judge_Switch(Enum_DR16_Switch_Status *Switch, uint8_t Status, uint8_t Pre_Status);
    void Judge_Key(Enum_DR16_Key_Status *Key, uint8_t Status, uint8_t Pre_Status);
};

/* 变量声明 ------------------------------------------------------------------------------------------------------------*/
extern Class_DR16 Remote_Control;

/* 函数声明 ------------------------------------------------------------------------------------------------------------*/

/* 接口函数定义 ---------------------------------------------------------------------------------------------------------*/
/**
 * @brief 获取遥控器在线状态
 *
 * @return Enum_DR16_Status 遥控器在线状态
 */
Enum_DR16_Status Class_DR16::Get_DR16_Status()
{
    return (DR16_Status);
}

/**
 * @brief 获取遥控器右侧x轴摇杆状态
 *
 * @return float 遥控器右侧x轴摇杆状态
 */
float Class_DR16::Get_Right_X()
{
    return (Data.Right_X);
}

/**
 * @brief 获取遥控器右侧y轴摇杆状态
 *
 * @return float 遥控器右侧y轴摇杆状态
 */
float Class_DR16::Get_Right_Y()
{
    return (Data.Right_Y);
}

/**
 * @brief 获取遥控器左侧x轴摇杆状态
 *
 * @return float 遥控器左侧x轴摇杆状态
 */
float Class_DR16::Get_Left_X()
{
    return (Data.Left_X);
}

/**
 * @brief 获取遥控器左侧y轴摇杆状态
 *
 * @return float 遥控器左侧y轴摇杆状态
 */
float Class_DR16::Get_Left_Y()
{
    return (Data.Left_Y);
}

/**
 * @brief 获取遥控器左侧拨动开关状态
 *
 * @return Enum_DR16_Switch_Status 遥控器左侧拨动开关状态
 */
Enum_DR16_Switch_Status Class_DR16::Get_Left_Switch()
{
    return (Data.Left_Switch);
}

/**
 * @brief 获取遥控器右侧拨动开关状态
 *
 * @return Enum_DR16_Switch_Status 遥控器右侧拨动开关状态
 */
Enum_DR16_Switch_Status Class_DR16::Get_Right_Switch()
{
    return (Data.Right_Switch);
}

/**
 * @brief 获取鼠标x轴状态
 *
 * @return float 鼠标x轴状态
 */
float Class_DR16::Get_Mouse_X()
{
    return (Data.Mouse_X);
}

/**
 * @brief 获取鼠标y轴状态
 *
 * @return float 鼠标y轴状态
 */
float Class_DR16::Get_Mouse_Y()
{
    return (Data.Mouse_Y);
}

/**
 * @brief 获取鼠标z轴状态
 *
 * @return float 鼠标z轴状态
 */
float Class_DR16::Get_Mouse_Z()
{
    return (Data.Mouse_Z);
}

/**
 * @brief 获取鼠标左键状态
 *
 * @return Enum_DR16_Key_Status 鼠标左键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Mouse_Left_Key()
{
    return (Data.Mouse_Left_Key);
}

/**
 * @brief 获取鼠标右键状态
 *
 * @return Enum_DR16_Key_Status 鼠标右键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Mouse_Right_Key()
{
    return (Data.Mouse_Right_Key);
}

/**
 * @brief 获取键盘W键状态
 *
 * @return Enum_DR16_Key_Status 键盘W键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_W()
{
    return (Data.Keyboard_Key[KEY_W]);
}

/**
 * @brief 获取键盘S键状态
 *
 * @return Enum_DR16_Key_Status 键盘S键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_S()
{
    return (Data.Keyboard_Key[KEY_S]);
}

/**
 * @brief 获取键盘A键状态
 *
 * @return Enum_DR16_Key_Status 键盘A键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_A()
{
    return (Data.Keyboard_Key[KEY_A]);
}

/**
 * @brief 获取键盘D键状态
 *
 * @return Enum_DR16_Key_Status 键盘D键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_D()
{
    return (Data.Keyboard_Key[KEY_D]);
}

/**
 * @brief 获取键盘Shift键状态
 *
 * @return Enum_DR16_Key_Status 键盘Shift键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_Shift()
{
    return (Data.Keyboard_Key[KEY_SHIFT]);
}

/**
 * @brief 获取键盘Ctrl键状态
 *
 * @return Enum_DR16_Key_Status 键盘Ctrl键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_Ctrl()
{
    return (Data.Keyboard_Key[KEY_CTRL]);
}

/**
 * @brief 获取键盘Q键状态
 *
 * @return Enum_DR16_Key_Status 键盘Q键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_Q()
{
    return (Data.Keyboard_Key[KEY_Q]);
}

/**
 * @brief 获取键盘E键状态
 *
 * @return Enum_DR16_Key_Status 键盘E键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_E()
{
    return (Data.Keyboard_Key[KEY_E]);
}

/**
 * @brief 获取键盘R键状态
 *
 * @return Enum_DR16_Key_Status 键盘R键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_R()
{
    return (Data.Keyboard_Key[KEY_R]);
}

/**
 * @brief 获取键盘F键状态
 *
 * @return Enum_DR16_Key_Status 键盘F键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_F()
{
    return (Data.Keyboard_Key[KEY_F]);
}

/**
 * @brief 获取键盘G键状态
 *
 * @return Enum_DR16_Key_Status 键盘G键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_G()
{
    return (Data.Keyboard_Key[KEY_G]);
}

/**
 * @brief 获取键盘Z键状态
 *
 * @return Enum_DR16_Key_Status 键盘Z键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_Z()
{
    return (Data.Keyboard_Key[KEY_Z]);
}

/**
 * @brief 获取键盘X键状态
 *
 * @return Enum_DR16_Key_Status 键盘X键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_X()
{
    return (Data.Keyboard_Key[KEY_X]);
}

/**
 * @brief 获取键盘C键状态
 *
 * @return Enum_DR16_Key_Status 键盘C键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_C()
{
    return (Data.Keyboard_Key[KEY_C]);
}

/**
 * @brief 获取键盘V键状态
 *
 * @return Enum_DR16_Key_Status 键盘V键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_V()
{
    return (Data.Keyboard_Key[KEY_V]);
}

/**
 * @brief 获取键盘B键状态
 *
 * @return Enum_DR16_Key_Status 键盘B键状态
 */
Enum_DR16_Key_Status Class_DR16::Get_Keyboard_Key_B()
{
    return (Data.Keyboard_Key[KEY_B]);
}

/**
 * @brief 获取遥控器yaw轴状态
 *
 * @return float 遥控器yaw轴状态
 */
float Class_DR16::Get_Yaw()
{
    return (Data.Yaw);
}

#endif /* HDL_Remote_Control */
