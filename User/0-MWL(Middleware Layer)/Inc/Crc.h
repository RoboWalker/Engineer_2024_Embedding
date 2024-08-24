/**
 * @file    Crc.h
 * @brief   Crc校验
 *
 * @author  Tang-yucheng
 * @date    2024-4-6 （创建）
 *
 */

#ifndef __MWL_CRC_H
#define __MWL_CRC_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "User_Math.h"

/* 函数声明 ------------------------------------------------------------------------------------------------------------*/
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);

#endif /* MWL_Crc */
