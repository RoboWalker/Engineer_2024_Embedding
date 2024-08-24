/**
 * @file    Pid.cpp
 * @brief   PID算法
 *
 * @author  Li-Chenyang
 * @date    2024-1-18 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Pid.h"

/**
 * @brief PID初始化
 *
 * @param __K_P         P参数
 * @param __K_I         I参数
 * @param __K_D         D参数
 * @param __K_F         前馈参数
 * @param __I_Out_Max   积分限幅
 * @param __Out_Max     输出限幅
 * @param __D_T         控制周期
 */
void Class_PID::Init(float __K_P, float __K_I, float __K_D, float __K_F, float __I_Out_Max, float __Out_Max,
                     float __D_T, float __Dead_Zone, float __I_Variable_Speed_A, float __I_Variable_Speed_B,
                     float __I_Separate_Threshold,Enum_PID_D_First __D_First)
{
    K_P = __K_P;
    K_I = __K_I;
    K_D = __K_D;
    K_F = __K_F;
    I_Out_Max = __I_Out_Max;
    Out_Max = __Out_Max;
    D_T = __D_T;
    Dead_Zone = __Dead_Zone;
    I_Variable_Speed_A = __I_Variable_Speed_A;
    I_Variable_Speed_B = __I_Variable_Speed_B;
    I_Separate_Threshold = __I_Separate_Threshold;
    D_First = __D_First;
}

/**
 * @brief PID计算输出值
 *
 * @return float    输出值
 */
void Class_PID::Calculate()
{
    float p_out = 0.0f;     // P输出
    float i_out = 0.0f;     // I输出
    float d_out = 0.0f;     // D输出
    float f_out = 0.0f;     // F输出（前馈）
    float error;            //误差
    float abs_error;        //绝对值误差
    float speed_ratio;      //线性变速积分

    /* 计算误差 */
    error = Target - Now;
    abs_error = Math_Abs(error);

    /* 判断死区 */
    if(abs_error < Dead_Zone)
    {
        Target = Now;
        error = 0.0f;
        abs_error = 0.0f;
    }

    /* 计算p项 */
    p_out = K_P * error;

    /* 计算i项 */
    if(I_Variable_Speed_A == 0.0f && I_Variable_Speed_B == 0.0f)
    {
        /* 非变速积分 */
        speed_ratio = 1.0f;
    }
    else
    {
        /* 变速积分 */
        if(abs_error <= I_Variable_Speed_A)
        {
            speed_ratio = 1.0f;
        }
        else if(I_Variable_Speed_A < abs_error && abs_error < I_Variable_Speed_B)
        {
            speed_ratio = (I_Variable_Speed_B - abs_error) / (I_Variable_Speed_B - I_Variable_Speed_A);
        }
        if(abs_error >= I_Variable_Speed_B)
        {
            speed_ratio = 0.0f;
        }
    }
    /* 积分限幅 */
    if(I_Out_Max != 0.0f)
    {
        Math_Constrain(&Integral_Error, -I_Out_Max / K_I, I_Out_Max / K_I);
    }
    if(I_Separate_Threshold == 0.0f)
    {
        /* 无积分分离 */
        Integral_Error += speed_ratio * D_T * error;
        i_out = K_I * Integral_Error;
    }
    else
    {
        /* 积分分离 */
        if(abs_error < I_Separate_Threshold)
        {
            Integral_Error += speed_ratio * D_T * error;
            i_out = K_I * Integral_Error;
        }
        else
        {
            Integral_Error = 0.0f;
            i_out = 0.0f;
        }
    }

    /* 计算d项 */
    if(D_First == PID_D_First_ENABLE)
    {
        /* 微分先行 */
        d_out = K_D * (Out - Pre_Out) / D_T;
    }
    else
    {
        /* 无微分先行 */
        d_out = K_D * (error - Pre_Error) / D_T;
    }

    /* 计算前馈 */
    f_out = (Target - Pre_Target) * K_F;

    /* 计算输出 */
    Out = p_out + i_out + d_out + f_out;
    /* 输出限幅 */
    if(Out_Max != 0.0f)
    {
        Math_Constrain(&Out, -Out_Max, Out_Max);
    }

    /* 更新数据 */
    Pre_Now = Now;
    Pre_Target = Target;
    Pre_Out = Out;
    Pre_Error = error;
}
