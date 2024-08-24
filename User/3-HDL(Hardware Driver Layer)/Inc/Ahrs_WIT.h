/**
 * @file dvc_witahrs.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief WIT姿态传感器
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2023
 *
 */

#ifndef DVC_WITAHRS_H
#define DVC_WITAHRS_H

/* Includes ------------------------------------------------------------------*/

#include "User_Math.h"
#include "User_Usart.h"
#include <cstring>

// #include <cstring>

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief WIT姿态传感器状态
 *
 */
enum Enum_WIT_AHRS_Status
{
    WIT_AHRS_Status_DISABLE = 0,
    WIT_AHRS_Status_ENABLE,
};

/**
 * @brief WIT姿态传感器接收数据类型
 *
 */
enum Enum_WIT_AHRS_Data_Type : uint8_t
{
    WIT_AHRS_Data_Type_TIME = 0x50,
    WIT_AHRS_Data_Type_ACCELERATE,
    WIT_AHRS_Data_Type_OMEGA,
    WIT_AHRS_Data_Type_ANGLE,
    WIT_AHRS_Data_Type_MAGNETIC,
    WIT_AHRS_Data_Type_PORT,
    WIT_AHRS_Data_Type_PRESSURE_ALTITUDE,
    WIT_AHRS_Data_Type_LONGITUDE_LATITUDE,
    WIT_AHRS_Data_Type_GROUND_SPEED,
    WIT_AHRS_Data_Type_QUATERNION,
    WIT_AHRS_Data_Type_GPS,
    WIT_AHRS_Data_Type_REGISTER,
};

/**
 * @brief WIT姿态传感器源数据
 * 
 */
struct Struct_WIT_AHRS_UART_Data
{
    uint8_t Frame_Header;
    Enum_WIT_AHRS_Data_Type Data_Type;
    uint8_t Data[8];
    uint8_t Checksum;
}__attribute__((packed));

/**
 * @brief WIT姿态传感器经过处理的数据, 时间
 * 
 */
struct Struct_WIT_AHRS_Data_Time
{
    uint8_t Year;
    uint8_t Month;
    uint8_t Date;
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;
    uint16_t Millisecond;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 加速度
 * 
 */
struct Struct_WIT_AHRS_Data_Accelerate
{
    float Accelerate_X;
    float Accelerate_Y;
    float Accelerate_Z;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 角速度
 * 
 */
struct Struct_WIT_AHRS_Data_Omega
{
    float Omega_X;
    float Omega_Y;
    float Omega_Z;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 角度
 * 
 */
struct Struct_WIT_AHRS_Data_Angle
{
    float Angle_Roll;
    float Angle_Pitch;
    float Angle_Yaw;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 磁场
 * 
 */
struct Struct_WIT_AHRS_Data_Magnetic
{
    float Magnetic_X;
    float Magnetic_Y;
    float Magnetic_Z;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 端口输出
 * 
 */
struct Struct_WIT_AHRS_Data_Port
{
    float Port_0;
    float Port_1;
    float Port_2;
    float Port_3;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 海拔经度纬度
 * 
 */
struct Struct_WIT_AHRS_Data_Coordinate
{
    float Altitude;
    float Longitude;
    float Latitude;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 四元数
 * 
 */
struct Struct_WIT_AHRS_Data_Quaternion
{
    float Q_0;
    float Q_1;
    float Q_2;
    float Q_3;
};

/**
 * @brief WIT姿态传感器经过处理的数据, GPS信息
 * 
 */
struct Struct_WIT_AHRS_Data_GPS
{
    uint16_t Satellite_Number;
    float GPS_Altitude;
    float GPS_Yaw;
    float GPS_Velocity;
    float GPS_Locational_Accuracy;
    float GPS_Horizontal_Accuracy;
    float GPS_Vertical_Accuracy;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 指定寄存器信息
 * 
 */
struct Struct_WIT_AHRS_Data_Register
{
    int16_t Register_1;
    int16_t Register_2;
    int16_t Register_3;
    int16_t Register_4;
};

/**
 * @brief WIT姿态传感器经过处理的数据, 其他有关信息
 * 
 */
struct Struct_WIT_AHRS_Data_Other
{
    float Temperature;
    float Pressure;
    float Voltage;
    int16_t Version;
};

/**
 * @brief Generic, WIT姿态传感器
 * 
 */
class Class_WIT_AHRS
{
public:
    void Init(UART_HandleTypeDef *huart, uint16_t __Frame_Header = 0x55);

    inline Enum_WIT_AHRS_Status Get_WIT_Status();

    inline uint8_t Get_Year();

    inline uint8_t Get_Month();

    inline uint8_t Get_Date();

    inline uint8_t Get_Hour();

    inline uint8_t Get_Minute();

    inline uint8_t Get_Second();

    inline uint16_t Get_Millisecond();

    inline float Get_Accelerate_X();

    inline float Get_Accelerate_Y();

    inline float Get_Accelerate_Z();

    inline float Get_Omega_X();

    inline float Get_Omega_Y();

    inline float Get_Omega_Z();

    inline float Get_Angle_Roll();

    inline float Get_Angle_Pitch();

    inline float Get_Angle_Yaw();

    inline float Get_Magnetic_X();

    inline float Get_Magnetic_Y();

    inline float Get_Magnetic_Z();

    inline float Get_Port_0();

    inline float Get_Port_1();

    inline float Get_Port_2();

    inline float Get_Port_3();

    inline float Get_Altitude();

    inline float Get_Longitude();

    inline float Get_Latitude();

    inline float Get_Q_0();

    inline float Get_Q_1();

    inline float Get_Q_2();

    inline float Get_Q_3();

    inline uint16_t Get_Satellite_Number();

    inline float Get_GPS_Altitude();

    inline float Get_GPS_Yaw();

    inline float Get_GPS_Velocity();

    inline float Get_GPS_Locational_Accuracy();

    inline float Get_GPS_Horizontal_Accuracy();

    inline float Get_GPS_Vertical_Accuracy();

    inline int16_t Get_Register_1();

    inline int16_t Get_Register_2();

    inline int16_t Get_Register_3();

    inline int16_t Get_Register_4();

    inline float Get_Temperature();

    inline float Get_Pressure();

    inline float Get_Voltage();

    inline int16_t Get_Version();

    void UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length);

    void TIM_100ms_Alive_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;
    // 数据包头标
    uint8_t Frame_Header;

    // 常量

    // 内部变量

    // 当前时刻的WIT姿态传感器接收flag
    uint32_t Flag = 0;
    // 前一时刻的WIT姿态传感器接收flag
    uint32_t Pre_Flag = 0;

    // 读变量

    // WIT姿态传感器状态
    Enum_WIT_AHRS_Status WIT_AHRS_Status = WIT_AHRS_Status_DISABLE;

    // WIT姿态传感器对外接口信息
    Struct_WIT_AHRS_Data_Time Data_Time;
    Struct_WIT_AHRS_Data_Accelerate Data_Accelerate;
    Struct_WIT_AHRS_Data_Omega Data_Omega;
    Struct_WIT_AHRS_Data_Angle Data_Angle;
    Struct_WIT_AHRS_Data_Magnetic Data_Magnetic;
    Struct_WIT_AHRS_Data_Port Data_Port;
    Struct_WIT_AHRS_Data_Coordinate Data_Coordinate;
    Struct_WIT_AHRS_Data_Quaternion Data_Quaternion;
    Struct_WIT_AHRS_Data_GPS Data_GPS;
    Struct_WIT_AHRS_Data_Register Data_Register;
    Struct_WIT_AHRS_Data_Other Data_Other;

    // 写变量

    // 读写变量

    // 内部函数

    void Data_Process();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取WIT姿态传感器状态
 * 
 * @return Enum_WIT_Status WIT姿态传感器状态
 */
inline Enum_WIT_AHRS_Status Class_WIT_AHRS::Get_WIT_Status()
{
    return (WIT_AHRS_Status);
}

/**
 * @brief 获取年份
 * 
 * @return uint8_t 年份
 */
inline uint8_t Class_WIT_AHRS::Get_Year()
{
    return (Data_Time.Year);
}

/**
 * @brief 获取月份
 * 
 * @return uint8_t 月份
 */
inline uint8_t Class_WIT_AHRS::Get_Month()
{
    return (Data_Time.Month);
}

/**
 * @brief 获取日期
 * 
 * @return uint8_t 日期
 */
inline uint8_t Class_WIT_AHRS::Get_Date()
{
    return (Data_Time.Date);
}

/**
 * @brief 获取小时
 * 
 * @return uint8_t 小时
 */
inline uint8_t Class_WIT_AHRS::Get_Hour()
{
    return (Data_Time.Hour);
}

/**
 * @brief 获取分钟
 * 
 * @return uint8_t 分钟
 */
inline uint8_t Class_WIT_AHRS::Get_Minute()
{
    return (Data_Time.Minute);
}

/**
 * @brief 获取秒
 * 
 * @return uint8_t 秒
 */
inline uint8_t Class_WIT_AHRS::Get_Second()
{
    return (Data_Time.Second);
}

/**
 * @brief 获取毫秒
 * 
 * @return uint16_t 毫秒
 */
inline uint16_t Class_WIT_AHRS::Get_Millisecond()
{
    return (Data_Time.Millisecond);
}

/**
 * @brief 获取加速度x
 * 
 * @return float 加速度x
 */
inline float Class_WIT_AHRS::Get_Accelerate_X()
{
    return (Data_Accelerate.Accelerate_X);
}

/**
 * @brief 获取加速度y
 * 
 * @return float 加速度y
 */
inline float Class_WIT_AHRS::Get_Accelerate_Y()
{
    return (Data_Accelerate.Accelerate_Y);
}

/**
 * @brief 获取加速度z
 * 
 * @return float 加速度z
 */
inline float Class_WIT_AHRS::Get_Accelerate_Z()
{
    return (Data_Accelerate.Accelerate_Z);
}

/**
 * @brief 获取角速度x
 * 
 * @return float 角速度x
 */
inline float Class_WIT_AHRS::Get_Omega_X()
{
    return (Data_Omega.Omega_X);
}

/**
 * @brief 获取角速度y
 * 
 * @return float 角速度y
 */
inline float Class_WIT_AHRS::Get_Omega_Y()
{
    return (Data_Omega.Omega_Y);
}

/**
 * @brief 获取角速度z
 * 
 * @return float 角速度z
 */
inline float Class_WIT_AHRS::Get_Omega_Z()
{
    return (Data_Omega.Omega_Z);
}

/**
 * @brief 获取滚转角
 * 
 * @return float 滚转角
 */
inline float Class_WIT_AHRS::Get_Angle_Roll()
{
    return (Data_Angle.Angle_Roll);
}

/**
 * @brief 获取俯仰角
 * 
 * @return float 俯仰角
 */
inline float Class_WIT_AHRS::Get_Angle_Pitch()
{
    return (Data_Angle.Angle_Pitch);
}

/**
 * @brief 获取航向角
 * 
 * @return float 航向角
 */
inline float Class_WIT_AHRS::Get_Angle_Yaw()
{
    return (Data_Angle.Angle_Yaw);
}

/**
 * @brief 获取磁场强度x
 * 
 * @return float 磁场强度x
 */
inline float Class_WIT_AHRS::Get_Magnetic_X()
{
    return (Data_Magnetic.Magnetic_X);
}

/**
 * @brief 获取磁场强度y
 * 
 * @return float 磁场强度y
 */
inline float Class_WIT_AHRS::Get_Magnetic_Y()
{
    return (Data_Magnetic.Magnetic_Y);
}

/**
 * @brief 获取磁场强度z
 * 
 * @return float 磁场强度z
 */
inline float Class_WIT_AHRS::Get_Magnetic_Z()
{
    return (Data_Magnetic.Magnetic_Z);
}

/**
 * @brief 获取端口0
 * 
 * @return float 端口0
 */
inline float Class_WIT_AHRS::Get_Port_0()
{
    return (Data_Port.Port_0);
}

/**
 * @brief 获取端口1
 * 
 * @return float 端口1
 */
inline float Class_WIT_AHRS::Get_Port_1()
{
    return (Data_Port.Port_1);
}

/**
 * @brief 获取端口2
 * 
 * @return float 端口2
 */
inline float Class_WIT_AHRS::Get_Port_2()
{
    return (Data_Port.Port_2);
}

/**
 * @brief 获取端口3
 * 
 * @return float 端口3
 */
inline float Class_WIT_AHRS::Get_Port_3()
{
    return (Data_Port.Port_3);
}

/**
 * @brief 获取高度
 * 
 * @return float 高度
 */
inline float Class_WIT_AHRS::Get_Altitude()
{
    return (Data_Coordinate.Altitude);
}

/**
 * @brief 获取经度
 * 
 * @return float 经度
 */
inline float Class_WIT_AHRS::Get_Longitude()
{
    return (Data_Coordinate.Longitude);
}

/**
 * @brief 获取纬度
 * 
 * @return float 纬度
 */
inline float Class_WIT_AHRS::Get_Latitude()
{
    return (Data_Coordinate.Latitude);
}

/**
 * @brief 获取四元数0
 * 
 * @return float 四元数0
 */
inline float Class_WIT_AHRS::Get_Q_0()
{
    return (Data_Quaternion.Q_0);
}

/**
 * @brief 获取四元数1
 * 
 * @return float 四元数1
 */
inline float Class_WIT_AHRS::Get_Q_1()
{
    return (Data_Quaternion.Q_1);
}

/**
 * @brief 获取四元数2
 * 
 * @return float 四元数2
 */
inline float Class_WIT_AHRS::Get_Q_2()
{
    return (Data_Quaternion.Q_2);
}

/**
 * @brief 获取四元数3
 * 
 * @return float 四元数3
 */
inline float Class_WIT_AHRS::Get_Q_3()
{
    return (Data_Quaternion.Q_3);
}

/**
 * @brief 获取卫星数
 * 
 * @return uint16_t 卫星数
 */
inline uint16_t Class_WIT_AHRS::Get_Satellite_Number()
{
    return (Data_GPS.Satellite_Number);
}

/**
 * @brief 获取GPS海拔
 * 
 * @return float GPS海拔
 */
inline float Class_WIT_AHRS::Get_GPS_Altitude()
{
    return (Data_GPS.GPS_Altitude);
}

/**
 * @brief 获取GPS航向角
 * 
 * @return float GPS航向角
 */
inline float Class_WIT_AHRS::Get_GPS_Yaw()
{
    return (Data_GPS.GPS_Yaw);
}

/**
 * @brief 获取GPS速度
 * 
 * @return float GPS速度
 */
inline float Class_WIT_AHRS::Get_GPS_Velocity()
{
    return (Data_GPS.GPS_Velocity);
}

/**
 * @brief 获取位置精度
 * 
 * @return float 位置精度
 */
inline float Class_WIT_AHRS::Get_GPS_Locational_Accuracy()
{
    return (Data_GPS.GPS_Locational_Accuracy);
}

/**
 * @brief 获取水平精度
 * 
 * @return float 水平精度
 */
inline float Class_WIT_AHRS::Get_GPS_Horizontal_Accuracy()
{
    return (Data_GPS.GPS_Horizontal_Accuracy);
}

/**
 * @brief 获取垂直精度
 * 
 * @return float 垂直精度
 */
inline float Class_WIT_AHRS::Get_GPS_Vertical_Accuracy()
{
    return (Data_GPS.GPS_Vertical_Accuracy);
}

/**
 * @brief 获取寄存器1
 * 
 * @return int16_t 寄存器1
 */
inline int16_t Class_WIT_AHRS::Get_Register_1()
{
    return (Data_Register.Register_1);
}

/**
 * @brief 获取寄存器2
 * 
 * @return int16_t 寄存器2
 */
inline int16_t Class_WIT_AHRS::Get_Register_2()
{
    return (Data_Register.Register_2);
}

/**
 * @brief 获取寄存器3
 * 
 * @return int16_t 寄存器3
 */
inline int16_t Class_WIT_AHRS::Get_Register_3()
{
    return (Data_Register.Register_3);
}

/**
 * @brief 获取寄存器4
 * 
 * @return int16_t 寄存器4
 */
inline int16_t Class_WIT_AHRS::Get_Register_4()
{
    return (Data_Register.Register_4);
}

/**
 * @brief 获取温度
 * 
 * @return float 温度
 */
inline float Class_WIT_AHRS::Get_Temperature()
{
    return (Data_Other.Temperature);
}

/**
 * @brief 获取压强
 * 
 * @return float 压强
 */
inline float Class_WIT_AHRS::Get_Pressure()
{
    return (Data_Other.Pressure);
}

/**
 * @brief 获取电压
 * 
 * @return float 电压
 */
inline float Class_WIT_AHRS::Get_Voltage()
{
    return (Data_Other.Voltage);
}

/**
 * @brief 获取版本信息
 * 
 * @return int16_t 版本信息
 */
inline int16_t Class_WIT_AHRS::Get_Version()
{
    return (Data_Other.Version);
}

// 变量声明
extern Class_WIT_AHRS AHRS_Chassis;

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
