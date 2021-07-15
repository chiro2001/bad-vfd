/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#ifndef u8
typedef uint8_t u8;
#endif

#ifndef u16
typedef uint16_t u16;
#endif

#ifndef u32
typedef uint32_t u32;
#endif

#define uprint(...) printf(__VA_ARGS__)

#define P_OUT(port, pin, value) HAL_GPIO_WritePin(port, pin, (value ? GPIO_PIN_SET : GPIO_PIN_RESET))
#define P_IN(port, pin) HAL_GPIO_ReadPin(port, pin)
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define E3_Pin GPIO_PIN_3
#define E3_GPIO_Port GPIOE
#define KEY_Pin GPIO_PIN_13
#define KEY_GPIO_Port GPIOC
#define ADF4351_DONE_Pin GPIO_PIN_0
#define ADF4351_DONE_GPIO_Port GPIOC
#define ADF4351_LE_Pin GPIO_PIN_2
#define ADF4351_LE_GPIO_Port GPIOC
#define ADF4351_CE_Pin GPIO_PIN_3
#define ADF4351_CE_GPIO_Port GPIOC
#define TFT_LED_Pin GPIO_PIN_1
#define TFT_LED_GPIO_Port GPIOA
#define TFT_DC_Pin GPIO_PIN_2
#define TFT_DC_GPIO_Port GPIOA
#define TFT_RESET_Pin GPIO_PIN_1
#define TFT_RESET_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_11
#define LCD_CS_GPIO_Port GPIOE
#define LCD_WR_RS_Pin GPIO_PIN_13
#define LCD_WR_RS_GPIO_Port GPIOE
#define SD_DETECT_Pin GPIO_PIN_11
#define SD_DETECT_GPIO_Port GPIOB
#define VFD_HON_Pin GPIO_PIN_8
#define VFD_HON_GPIO_Port GPIOD
#define VFD_DA_Pin GPIO_PIN_9
#define VFD_DA_GPIO_Port GPIOD
#define VFD_CP_Pin GPIO_PIN_10
#define VFD_CP_GPIO_Port GPIOD
#define VFD_CS_Pin GPIO_PIN_11
#define VFD_CS_GPIO_Port GPIOD
#define VFD_RST_Pin GPIO_PIN_12
#define VFD_RST_GPIO_Port GPIOD
#define T_CLK_Pin GPIO_PIN_3
#define T_CLK_GPIO_Port GPIOB
#define T_DIN_Pin GPIO_PIN_4
#define T_DIN_GPIO_Port GPIOB
#define T_DO_Pin GPIO_PIN_5
#define T_DO_GPIO_Port GPIOB
#define T_IRQ_Pin GPIO_PIN_6
#define T_IRQ_GPIO_Port GPIOB
#define T_CS_Pin GPIO_PIN_7
#define T_CS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
