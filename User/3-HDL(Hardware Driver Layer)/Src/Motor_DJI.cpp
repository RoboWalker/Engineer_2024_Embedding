/**
 * @file    Motor_DJI_C620.cpp
 * @brief   大疆C620电机调速器驱动
 *
 * @author  Li-Chenyang
 * @date    2024-1-18 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Motor_DJI.h"

/* 全局变量创建 --------------------------------------------------------------------------------------------------------*/

/* 发送缓冲区，便于电机数据打包发送 */
uint8_t DJI_C6x0_H_CAN2_Tx_Data[8];
uint8_t DJI_C6x0_L_CAN2_Tx_Data[8];

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief 大疆CAN发送函数（将缓冲区数据统一发送）（需定时发送）
 **********************************************************************************************************************/
void DJI_CAN_SendData()
{
    /* DJI-CAN2电机 */
    CAN_Send_Data(&CAN2_Manage_Object, DJI_C6x0_H_ID_TX, DJI_C6x0_H_CAN2_Tx_Data, 8);
    CAN_Send_Data(&CAN2_Manage_Object, DJI_C6x0_L_ID_TX, DJI_C6x0_L_CAN2_Tx_Data, 8);
}

/***********************************************************************************************************************
 * @brief 大疆分配CAN发送缓冲区
 *
 * @param CAN_Manage_Obj        CAN处理结构体指针
 * @param __CAN_ID              CAN-ID
 * @return uint8_t*             缓冲区指针
 **********************************************************************************************************************/
uint8_t *allocate_tx_buffer_C6x0(Struct_CAN_Manage_Object * CAN_Manage_Obj, Enum_DJI_Motor_ID __CAN_ID)
{
    CAN_HandleTypeDef * hcan = CAN_Manage_Obj->hcan;
    uint8_t * tmp_tx_data_ptr;

    /* 判断CAN外设 */
    if(hcan->Instance == hcan2.Instance)
    {
        switch(__CAN_ID)
        {
            case (DJI_Motor_ID_0x201):
                tmp_tx_data_ptr = &(DJI_C6x0_H_CAN2_Tx_Data[0]);
                break;
            case (DJI_Motor_ID_0x202):
                tmp_tx_data_ptr = &(DJI_C6x0_H_CAN2_Tx_Data[2]);
                break;
            case (DJI_Motor_ID_0x203):
                tmp_tx_data_ptr = &(DJI_C6x0_H_CAN2_Tx_Data[4]);
                break;
            case (DJI_Motor_ID_0x204):
                tmp_tx_data_ptr = &(DJI_C6x0_H_CAN2_Tx_Data[6]);
                break;
            case (DJI_Motor_ID_0x205):
                tmp_tx_data_ptr = &(DJI_C6x0_L_CAN2_Tx_Data[0]);
                break;
            case (DJI_Motor_ID_0x206):
                tmp_tx_data_ptr = &(DJI_C6x0_L_CAN2_Tx_Data[2]);
                break;
            case (DJI_Motor_ID_0x207):
                tmp_tx_data_ptr = &(DJI_C6x0_L_CAN2_Tx_Data[4]);
                break;
            case (DJI_Motor_ID_0x208):
                tmp_tx_data_ptr = &(DJI_C6x0_L_CAN2_Tx_Data[6]);
                break;
            case (DJI_Motor_ID_UNDEFINED):
                tmp_tx_data_ptr = NULL;
                break;
        }
    }

    return (tmp_tx_data_ptr);
}

/***********************************************************************************************************************
 * @brief C610初始化
 *
 * @param CAN_Manage_Obj                CAN处理结构体指针
 * @param __CAN_ID                      CAN-ID
 * @param __DJI_Motor_Control_Method    电机控制方式, 默认角度
 * @param __Gearbox_Rate                减速箱减速比, 默认为原装减速箱, 如拆去减速箱则该值设为1
 * @param __Torque_Max                  最大扭矩, 需根据不同负载测量后赋值, 也就开环和扭矩环输出用得到, 不过我感觉应该没有奇葩喜欢开环输出这玩意
 **********************************************************************************************************************/
void Class_Motor_DJI_C610::Init(Struct_CAN_Manage_Object * CAN_Manage_Obj, Enum_DJI_Motor_ID __CAN_ID,
                                Enum_DJI_Motor_Control_Method __DJI_Motor_Control_Method,
                                float __Gearbox_Rate, float __Torque_Max)
{
    CAN_Manage_Object = CAN_Manage_Obj;
    CAN_ID = __CAN_ID;
    DJI_Motor_Control_Method = __DJI_Motor_Control_Method;
    Gearbox_Rate = __Gearbox_Rate;
    Torque_Max = __Torque_Max;
    CAN_Tx_Data = allocate_tx_buffer_C6x0(CAN_Manage_Obj, __CAN_ID);
}

/***********************************************************************************************************************
 * @brief C610实际数据接收函数（CAN接收回调函数中）
 **********************************************************************************************************************/
void Class_Motor_DJI_C610::DataGet()
{
    //滑动窗口, 判断电机是否在线
    Flag += 1;

    //数据处理过程
    int16_t delta_encoder;
    uint16_t tmp_encoder;
    int16_t tmp_omega, tmp_torque, tmp_temperature;
    auto tmp_buffer = (Struct_DJI_Motor_CAN_Data *) CAN_Manage_Object->Rx_Buffer.Data;

    //处理大小端
    Math_Endian_Reverse_16((void *) &tmp_buffer->Encoder_Reverse, (void *) &tmp_encoder);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Omega_Reverse, (void *) &tmp_omega);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Torque_Reverse, (void *) &tmp_torque);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Temperature, (void *) &tmp_temperature);

    //计算圈数与总编码器值
    delta_encoder = tmp_encoder - Data.Pre_Encoder;
    if(delta_encoder < -Encoder_Num_Per_Round / 2)
    {
        //正方向转过了一圈
        Data.Total_Round++;
    }
    else if(delta_encoder > Encoder_Num_Per_Round / 2)
    {
        //反方向转过了一圈
        Data.Total_Round--;
    }
    Data.Total_Encoder = Data.Total_Round * Encoder_Num_Per_Round + tmp_encoder;

    //计算电机本身信息
    Data.Now_Angle = (float) Data.Total_Encoder / (float) Encoder_Num_Per_Round * 2.0f * PI / Gearbox_Rate;
    Data.Now_Omega = (float) tmp_omega * RPM_TO_RADPS / Gearbox_Rate;
    Data.Now_Current = tmp_torque / 1000.0f;
    Data.Now_Temperature = (float)tmp_temperature + CELSIUS_TO_KELVIN;

    //存储预备信息
    Data.Pre_Encoder = tmp_encoder;
}

/***********************************************************************************************************************
 * @brief C610存活检测函数
 **********************************************************************************************************************/
Enum_DJI_Motor_Status Class_Motor_DJI_C610::AliveCheck()
{
    //判断该时间段内是否接收过电机数据
    if(Flag == Pre_Flag)
    {
        //电机断开连接
        DJI_Motor_Status = DJI_Motor_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
    }
    else
    {
        //电机保持连接
        DJI_Motor_Status = DJI_Motor_Status_ENABLE;
    }
    Pre_Flag = Flag;

    return (DJI_Motor_Status);
}

/***********************************************************************************************************************
 * @brief C610闭环控制函数（需定时控制）
 **********************************************************************************************************************/
void Class_Motor_DJI_C610::Control()
{
    switch(DJI_Motor_Control_Method)
    {
        case (DJI_Motor_Control_Method_TORQUE):
        {
            //默认闭环扭矩控制
            Out_Current = Target_Torque / (Torque_Ratio * Gearbox_Rate);
            break;
        }
        case (DJI_Motor_Control_Method_OMEGA):
            PID_Omega.Set_Target(Target_Omega);
            PID_Omega.Set_Now(Data.Now_Omega);
            PID_Omega.Calculate();

            Out_Current = PID_Omega.Get_Out();
            break;
        case (DJI_Motor_Control_Method_ANGLE):
            PID_Angle.Set_Target(Target_Angle);
            PID_Angle.Set_Now(Data.Now_Angle);
            PID_Angle.Calculate();

            Target_Omega = PID_Angle.Get_Out();

            PID_Omega.Set_Target(Target_Omega);
            PID_Omega.Set_Now(Data.Now_Omega);
            PID_Omega.Calculate();

            Out_Current = PID_Omega.Get_Out();
            break;
        default:
            Out_Current = 0.0f;
            break;
    }

    int16_t Out = this->Out_Current / this->Current_Conversion;
    if (Out > this->Output_Max)
    {
        Out = this->Output_Max;
    }
    else if (Out < -this->Output_Max)
    {
        Out = -this->Output_Max;
    }

    CAN_Tx_Data[0] = (int16_t) Out >> 8;
    CAN_Tx_Data[1] = (int16_t) Out;
}

/***********************************************************************************************************************
 * @brief C620初始化
 *
 * @param CAN_Manage_Obj                CAN处理结构体指针
 * @param __CAN_ID                      CAN-ID
 * @param __DJI_Motor_Control_Method    电机控制方式, 默认速度
 * @param __Gearbox_Rate                减速箱减速比, 默认为原装减速箱, 如拆去减速箱则该值设为1
 * @param __Torque_Max                  最大扭矩, 需根据不同负载测量后赋值, 也就开环和扭矩环输出用得到, 不过我感觉应该没有奇葩喜欢开环输出这玩意(by T)
 **********************************************************************************************************************/
void Class_Motor_DJI_C620::Init(Struct_CAN_Manage_Object * CAN_Manage_Obj, Enum_DJI_Motor_ID __CAN_ID,
                                Enum_DJI_Motor_Control_Method __DJI_Motor_Control_Method,
                                float __Omega_Max, float __Gearbox_Rate, float __Torque_Max)
{
    CAN_Manage_Object = CAN_Manage_Obj;
    CAN_ID = __CAN_ID;
    DJI_Motor_Control_Method = __DJI_Motor_Control_Method;
    Gearbox_Rate = __Gearbox_Rate;
    Torque_Max = __Torque_Max;
    Omega_Max = __Omega_Max;
    CAN_Tx_Data = allocate_tx_buffer_C6x0(CAN_Manage_Obj, __CAN_ID);
}

/***********************************************************************************************************************
 * @brief C620实际数据接收函数（CAN接收回调函数中）
 **********************************************************************************************************************/
void Class_Motor_DJI_C620::DataGet()
{
    //滑动窗口, 判断电机是否在线
    Flag += 1;

    //数据处理过程
    int16_t delta_encoder;
    uint16_t tmp_encoder;
    int16_t tmp_omega, tmp_torque, tmp_temperature;
    auto tmp_buffer = (Struct_DJI_Motor_CAN_Data *) CAN_Manage_Object->Rx_Buffer.Data;

    //处理大小端
    Math_Endian_Reverse_16((void *) &tmp_buffer->Encoder_Reverse, (void *) &tmp_encoder);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Omega_Reverse, (void *) &tmp_omega);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Torque_Reverse, (void *) &tmp_torque);
    Math_Endian_Reverse_16((void *) &tmp_buffer->Temperature, (void *) &tmp_temperature);

    //计算圈数与总编码器值
    delta_encoder = tmp_encoder - Data.Pre_Encoder;
    if(delta_encoder < -Encoder_Num_Per_Round / 2)
    {
        //正方向转过了一圈
        Data.Total_Round++;
    }
    else if(delta_encoder > Encoder_Num_Per_Round / 2)
    {
        //反方向转过了一圈
        Data.Total_Round--;
    }
    Data.Total_Encoder = Data.Total_Round * Encoder_Num_Per_Round + tmp_encoder;

    //计算电机本身信息
    Data.Now_Angle = (float) Data.Total_Encoder / (float) Encoder_Num_Per_Round * 2.0f * 3.14159f / Gearbox_Rate;
    Data.Now_Omega = (float) tmp_omega * RPM_TO_RADPS / Gearbox_Rate;
    Data.Now_Current = (float) tmp_torque * Current_Conversion;
    Data.Now_Temperature = (float)tmp_temperature + CELSIUS_TO_KELVIN;

    //存储预备信息
    Data.Pre_Encoder = tmp_encoder;
}

/***********************************************************************************************************************
 * @brief C620存活检测函数
 **********************************************************************************************************************/
Enum_DJI_Motor_Status Class_Motor_DJI_C620::AliveCheck()
{
    //判断该时间段内是否接收过电机数据
    if(Flag == Pre_Flag)
    {
        //电机断开连接
        DJI_Motor_Status = DJI_Motor_Status_DISABLE;
        PID_Angle.Set_Integral_Error(0.0f);
        PID_Omega.Set_Integral_Error(0.0f);
    }
    else
    {
        //电机保持连接
        DJI_Motor_Status = DJI_Motor_Status_ENABLE;
    }
    Pre_Flag = Flag;

    return (DJI_Motor_Status);
}

/***********************************************************************************************************************
 * @brief C620闭环控制函数（需定时控制）
 **********************************************************************************************************************/
void Class_Motor_DJI_C620::Control()
{
    switch(DJI_Motor_Control_Method)
    {
        case (DJI_Motor_Control_Method_OPENLOOP):
        {
            //默认开环扭矩控制
            Out_Current = Target_Torque / Torque_Max * (float)Output_Max;
            break;
        }
        case (DJI_Motor_Control_Method_TORQUE):
        {
            //默认闭环扭矩控制
            Out_Current = Target_Torque / Torque_Max * (float)Output_Max;
            break;
        }
        case (DJI_Motor_Control_Method_OMEGA):
        {
            PID_Omega.Set_Target(Target_Omega);
            PID_Omega.Set_Now(Data.Now_Omega);
            PID_Omega.Calculate();

            Out_Current = PID_Omega.Get_Out();
            break;
        }
        case (DJI_Motor_Control_Method_ANGLE):
        {
            PID_Angle.Set_Target(Target_Angle);
            PID_Angle.Set_Now(Data.Now_Angle);
            PID_Angle.Calculate();

            Target_Omega = PID_Angle.Get_Out();
            if (Omega_Max != 0.0f)
            {
                Math_Constrain(&Target_Omega, -Omega_Max, Omega_Max);
            }

            PID_Omega.Set_Target(Target_Omega);
            PID_Omega.Set_Now(Data.Now_Omega);
            PID_Omega.Calculate();

            Out_Current = PID_Omega.Get_Out();
            break;
        }
        default:
        {
            Out_Current = 0.0f;
            break;
        }
    }

    /* 电机功率估算 */
    this->Power_Estimate = this->Power_k0 * this->Out_Current * this->Data.Now_Omega +
                           this->Power_k1 * this->Data.Now_Omega * this->Data.Now_Omega +
                           this->Power_k2 * this->Out_Current * this->Out_Current +
                           this->Power_a;


    int16_t out = Out_Current / Current_Conversion;

    CAN_Tx_Data[0] = (int16_t) out >> 8;
    CAN_Tx_Data[1] = (int16_t) out;
}

void Class_Motor_DJI_C620::Clear_Data()
{
    this->Data.Now_Angle = 0.0f;
    this->Data.Now_Omega = 0.0f;
    this->Data.Now_Current = 0.0f;
    this->Data.Now_Temperature = 0.0f;
    this->Data.Pre_Omega = 0.0f;
    this->Data.Pre_Encoder = 0U;
    this->Data.Total_Encoder = 0U;
    this->Data.Total_Round = 0U;
}


/***********************************************************************************************************************
 * @brief 设定输出功率
 *
 * @param OutPower 输出功率
 **********************************************************************************************************************/
void Class_Motor_DJI_C620::Set_OutPower(float OutPower)
{
    float a, b, c, m, result_1, result_2;

    /* 判断是否需要更新输出力矩电流 */
    if (OutPower == this->Power_Estimate)
    {
        return;
    }

    /* 计算所需力矩电流 */
    a = this->Power_k2;
    b = this->Power_k0 * this->Data.Now_Omega;
    c = this->Power_k1 * this->Data.Now_Omega * this->Data.Now_Omega + this->Power_a - OutPower;
    m = sqrt(b * b - 4.0f * a * c);
    result_1 = (-b + m) / (2.0f * a);
    result_2 = (-b - m) / (2.0f * a);
    if ((result_1 > 0 && result_2 < 0) || (result_1 < 0 && result_2 > 0))
    {
        if ((result_1 > 0 && this->Out_Current > 0) || (result_1 < 0 && this->Out_Current < 0))
        {
            this->Out_Current = result_1;
        }
        else
        {
            this->Out_Current = result_2;
        }
    }
    else
    {
        if (Math_Abs(result_1) < Math_Abs(result_2))
        {
            this->Out_Current = result_1;
        }
        else
        {
            this->Out_Current = result_2;
        }
    }
}

