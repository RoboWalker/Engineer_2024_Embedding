/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AIR_PUMP_Pin GPIO_PIN_2
#define AIR_PUMP_GPIO_Port GPIOH
#define SOLENOID_VALVE_Pin GPIO_PIN_3
#define SOLENOID_VALVE_GPIO_Port GPIOH
#define LED_8_Pin GPIO_PIN_8
#define LED_8_GPIO_Port GPIOG
#define LED_7_Pin GPIO_PIN_7
#define LED_7_GPIO_Port GPIOG
#define LED_6_Pin GPIO_PIN_6
#define LED_6_GPIO_Port GPIOG
#define LED_5_Pin GPIO_PIN_5
#define LED_5_GPIO_Port GPIOG
#define LED_4_Pin GPIO_PIN_4
#define LED_4_GPIO_Port GPIOG
#define LED_3_Pin GPIO_PIN_3
#define LED_3_GPIO_Port GPIOG
#define LED_2_Pin GPIO_PIN_2
#define LED_2_GPIO_Port GPIOG
#define LED_1_Pin GPIO_PIN_1
#define LED_1_GPIO_Port GPIOG
#define LED_RED_Pin GPIO_PIN_11
#define LED_RED_GPIO_Port GPIOE
#define LED_GREEN_Pin GPIO_PIN_14
#define LED_GREEN_GPIO_Port GPIOF

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
