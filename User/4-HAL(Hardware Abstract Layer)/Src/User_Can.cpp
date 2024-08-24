/**
 * @file    User_Can.cpp
 * @brief   CAN外设进一步封装
 *
 * @author  Li-chenyang
 * @date    2024-1-18 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "User_Can.h"

/* 全局变量 -----------------------------------------------------------------------------------------------------------*/
Struct_CAN_Manage_Object CAN1_Manage_Object = {&hcan1};
Struct_CAN_Manage_Object CAN2_Manage_Object = {&hcan2};

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * @brief CAN初始化
 *
 * @param CAN_Manage_Obj      CAN处理结构体指针
 **********************************************************************************************************************/
void CAN_Init(Struct_CAN_Manage_Object * CAN_Manage_Obj)
{
    CAN_HandleTypeDef * hcan = CAN_Manage_Obj->hcan;

    HAL_CAN_Start(hcan);
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);

    if(hcan->Instance == CAN1)
    {
        CAN_ConfigFilter(hcan, CAN_FILTER(0) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0, 0);
        CAN_ConfigFilter(hcan, CAN_FILTER(1) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE, 0, 0);
    }
    else if(hcan->Instance == CAN2)
    {
        CAN_ConfigFilter(hcan, CAN_FILTER(14) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0, 0);
        CAN_ConfigFilter(hcan, CAN_FILTER(15) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE, 0, 0);
    }
}

/***********************************************************************************************************************
 * @brief CAN发送数据帧
 *
 * @param CAN_Manage_Obj        CAN处理结构体指针
 * @param ID                    帧ID
 * @param Data                  帧数据
 * @param Length                帧数据长度
 * @return uint8_t              执行结果
 **********************************************************************************************************************/
uint8_t CAN_Send_Data(Struct_CAN_Manage_Object * CAN_Manage_Obj, uint16_t ID, uint8_t * Data, uint16_t Length)
{
    CAN_TxHeaderTypeDef tx_header;
    uint32_t used_mailbox;

    //检测传参是否正确
    assert_param(hcan != NULL);

    tx_header.StdId = ID;
    tx_header.ExtId = 0;
    tx_header.IDE = 0;
    tx_header.RTR = 0;
    tx_header.DLC = Length;

    return (HAL_CAN_AddTxMessage(CAN_Manage_Obj->hcan, &tx_header, Data, &used_mailbox));
}

/***********************************************************************************************************************
 * @brief CAN接收数据帧
 *
 * @param CAN_Manage_Obj        CAN处理结构体指针
 * @param RxFifo                接收FIFO（CAN_FILTER_FIFOx）
 **********************************************************************************************************************/
void CAN_Receive_Data(Struct_CAN_Manage_Object * CAN_Manage_Obj, uint32_t RxFifo)
{
    HAL_CAN_GetRxMessage(CAN_Manage_Obj->hcan, RxFifo, &CAN_Manage_Obj->Rx_Buffer.Header, CAN_Manage_Obj->Rx_Buffer.Data);
}

/***********************************************************************************************************************
 * @brief CAN配置过滤器
 *
 * @param hcan          CAN外设句柄
 * @param Object_Para   编号 | FIFOx | ID类型 | 帧类型
 * @param ID            帧ID
 * @param Mask_ID       屏蔽位(0x3ff, 0x1fffffff)
 **********************************************************************************************************************/
void CAN_ConfigFilter(CAN_HandleTypeDef * hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID)
{
    CAN_FilterTypeDef can_filter_init_structure;

    //检测传参是否正确
    assert_param(hcan != NULL);

    if((Object_Para & 0x02))
    {
        //数据帧
        //掩码后ID的高16bit
        can_filter_init_structure.FilterIdHigh = ID << 3 << 16;
        //掩码后ID的低16bit
        can_filter_init_structure.FilterIdLow = ID << 3 | ((Object_Para & 0x03) << 1);
        // ID掩码值高16bit
        can_filter_init_structure.FilterMaskIdHigh = Mask_ID << 3 << 16;
        // ID掩码值低16bit
        can_filter_init_structure.FilterMaskIdLow = Mask_ID << 3 | ((Object_Para & 0x03) << 1);
    }
    else
    {
        //其他帧
        //掩码后ID的高16bit
        can_filter_init_structure.FilterIdHigh = ID << 5;
        //掩码后ID的低16bit
        can_filter_init_structure.FilterIdLow = ((Object_Para & 0x03) << 1);
        // ID掩码值高16bit
        can_filter_init_structure.FilterMaskIdHigh = Mask_ID << 5;
        // ID掩码值低16bit
        can_filter_init_structure.FilterMaskIdLow = ((Object_Para & 0x03) << 1);
    }
    //滤波器序号, 0-27, 共28个滤波器, 前14个在CAN1, 后14个在CAN2
    can_filter_init_structure.FilterBank = Object_Para >> 3;
    //滤波器绑定FIFO0
    can_filter_init_structure.FilterFIFOAssignment = (Object_Para >> 2) & 0x01;
    //使能滤波器
    can_filter_init_structure.FilterActivation = ENABLE;
    //滤波器模式，设置ID掩码模式
    can_filter_init_structure.FilterMode = CAN_FILTERMODE_IDMASK;
    // 32位滤波
    can_filter_init_structure.FilterScale = CAN_FILTERSCALE_32BIT;
    //从机模式选择开始单元
    can_filter_init_structure.SlaveStartFilterBank = 14;

    HAL_CAN_ConfigFilter(hcan, &can_filter_init_structure);
}
