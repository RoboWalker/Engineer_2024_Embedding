/**
 * @file    Pid.h
 * @brief   Pid算法
 *
 * @author  Li-Chenyang
 * @date    2024-1-18 （创建）
 *
 */

#ifndef __MWL_PID_H
#define __MWL_PID_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "User_Math.h"

/* 枚举类型定义 --------------------------------------------------------------------------------------------------------*/
/**
 * @brief 微分先行
 *
 */
enum Enum_PID_D_First
{
    PID_D_First_DISABLE = 0,
    PID_D_First_ENABLE,
};

/* 类定义 -------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Reusable, PID算法
 *
 */
class Class_PID
{
public:
    /* 函数 */
    void Init(float __K_P, float __K_I, float __K_D, float __K_F = 0.0f, float __I_Out_Max = 0.0f,
              float __Out_Max = 0.0f, float __D_T = 0.001f, float __Dead_Zone = 0.0f,
              float __I_Variable_Speed_A = 0.0f, float __I_Variable_Speed_B = 0.0f, float __I_Separate_Threshold = 0.0f,
              Enum_PID_D_First __D_First = PID_D_First_DISABLE);
    void Calculate();

    inline float Get_Integral_Error();
    inline float Get_Out();
    inline void Set_K_P(float __K_P);
    inline void Set_K_I(float __K_I);
    inline void Set_K_D(float __K_D);
    inline void Set_K_F(float __K_F);
    inline void Set_I_Out_Max(float __I_Out_Max);
    inline void Set_Out_Max(float __Out_Max);
    inline void Set_I_Variable_Speed_A(float __Variable_Speed_I_A);
    inline void Set_I_Variable_Speed_B(float __Variable_Speed_I_B);
    inline void Set_I_Separate_Threshold(float __I_Separate_Threshold);
    inline void Set_Target(float __Target);
    inline void Set_Now(float __Now);
    inline void Set_Integral_Error(float __Integral_Error);
protected:
    /* 读变量 */
    float Out = 0.0f;                       /*!< 输出值 */

    /* 写变量 */
    float D_T;                              /*!< PID计时器周期（s） */
    float K_P = 0.0f;                       /*!< P参数 */
    float K_I = 0.0f;                       /*!< I参数 */
    float K_D = 0.0f;                       /*!< D参数 */
    float K_F = 0.0f;                       /*!< 前馈参数 */
    float I_Out_Max = 0;                    /*!< 积分限幅, 0为不限制 */
    float Out_Max = 0;                      /*!< 输出限幅, 0为不限制 */
    float I_Variable_Speed_A = 0.0f;        /*!< 变速积分定速内段阈值, 0为不限制 */
    float I_Variable_Speed_B = 0.0f;        /*!< 变速积分变速区间, 0为不限制 */
    float I_Separate_Threshold = 0.0f;      /*!< 积分分离阈值，需为正数, 0为不限制 */
    float Target = 0.0f;                    /*!< 目标值 */
    float Now = 0.0f;                       /*!< 当前值 */
    float Dead_Zone;                        /*!< 死区, Error在其绝对值内不输出 */
    Enum_PID_D_First D_First;               /*!< 微分先行 */

    /* 读写变量 */
    float Integral_Error = 0.0f;            /*!< 积分值 */

    /* 内部变量 */
    float Pre_Now = 0.0f;                   /*!< 之前的当前值 */
    float Pre_Target = 0.0f;                /*!< 之前的目标值 */
    float Pre_Out = 0.0f;                   /*!< 之前的输出值 */
    float Pre_Error = 0.0f;                 /*!< 前向误差 */
};

/* 函数声明 ------------------------------------------------------------------------------------------------------------*/

/* 接口函数定义 ---------------------------------------------------------------------------------------------------------*/
/**
 * @brief 获取积分值
 *
 * @return float 积分值
 */
float Class_PID::Get_Integral_Error()
{
    return (Integral_Error);
}

/**
 * @brief 获取输出值
 *
 * @return float 输出值
 */
float Class_PID::Get_Out()
{
    return (Out);
}

/**
 * @brief 设定PID的P
 *
 * @param __K_P PID的P
 */
void Class_PID::Set_K_P(float __K_P)
{
    K_P = __K_P;
}

/**
 * @brief 设定PID的I
 *
 * @param __K_I PID的I
 */
void Class_PID::Set_K_I(float __K_I)
{
    K_I = __K_I;
}

/**
 * @brief 设定PID的D
 *
 * @param __K_D PID的D
 */
void Class_PID::Set_K_D(float __K_D)
{
    K_D = __K_D;
}

/**
 * @brief 设定前馈
 *
 * @param __K_D 前馈
 */
void Class_PID::Set_K_F(float __K_F)
{
    K_F = __K_F;
}

/**
 * @brief 设定积分限幅, 0为不限制
 *
 * @param __I_Out_Max 积分限幅, 0为不限制
 */
void Class_PID::Set_I_Out_Max(float __I_Out_Max)
{
    I_Out_Max = __I_Out_Max;
}

/**
 * @brief 设定输出限幅, 0为不限制
 *
 * @param __Out_Max 输出限幅, 0为不限制
 */
void Class_PID::Set_Out_Max(float __Out_Max)
{
    Out_Max = __Out_Max;
}

/**
 * @brief 设定定速内段阈值, 0为不限制
 *
 * @param __I_Variable_Speed_A 定速内段阈值, 0为不限制
 */
void Class_PID::Set_I_Variable_Speed_A(float __I_Variable_Speed_A)
{
    I_Variable_Speed_A = __I_Variable_Speed_A;
}

/**
 * @brief 设定变速区间, 0为不限制
 *
 * @param __I_Variable_Speed_B 变速区间, 0为不限制
 */
void Class_PID::Set_I_Variable_Speed_B(float __I_Variable_Speed_B)
{
    I_Variable_Speed_B = __I_Variable_Speed_B;
}

/**
 * @brief 设定积分分离阈值，需为正数, 0为不限制
 *
 * @param __I_Separate_Threshold 积分分离阈值，需为正数, 0为不限制
 */
void Class_PID::Set_I_Separate_Threshold(float __I_Separate_Threshold)
{
    I_Separate_Threshold = __I_Separate_Threshold;
}

/**
 * @brief 设定目标值
 *
 * @param __Target 目标值
 */
void Class_PID::Set_Target(float __Target)
{
    Target = __Target;
}

/**
 * @brief 设定当前值
 *
 * @param __Now 当前值
 */
void Class_PID::Set_Now(float __Now)
{
    Now = __Now;
}

/**
 * @brief 设定积分, 一般用于积分清零
 *
 * @param __Set_Integral_Error 积分值
 */
void Class_PID::Set_Integral_Error(float __Integral_Error)
{
    Integral_Error = __Integral_Error;
}

#endif /* MWL_Pid */
