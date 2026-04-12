/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
uint32_t GetRegister(__IO uint32_t *);
void stub_SET_BIT(__IO uint32_t *, uint32_t);
void stub_CLEAR_BIT(__IO uint32_t *, uint32_t);
uint32_t stub_READ_BIT(__IO uint32_t *, uint32_t);
FlagStatus stub_HAL_I2C_GET_FLAG(I2C_HandleTypeDef *, uint32_t);
void stub_HAL_I2C_CLEAR_ADDRFLAG(I2C_HandleTypeDef *);
uint32_t HAL_GetTick(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
#ifdef SET_BIT
#undef SET_BIT
#define SET_BIT(REG, BIT) stub_SET_BIT(&(REG), BIT)
#endif

#ifdef CLEAR_BIT
#undef CLEAR_BIT
#define CLEAR_BIT(REG, BIT) stub_CLEAR_BIT(&(REG), BIT)
#endif

#ifdef READ_BIT
#undef READ_BIT
#define READ_BIT(REG, BIT) stub_READ_BIT(&(REG), BIT)
#endif

#ifdef __HAL_I2C_GET_FLAG
#undef __HAL_I2C_GET_FLAG
#define __HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__) stub_HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__)
#endif

#ifdef __HAL_I2C_CLEAR_ADDRFLAG
#undef __HAL_I2C_CLEAR_ADDRFLAG
#define __HAL_I2C_CLEAR_ADDRFLAG(__HANDLE__) stub_HAL_I2C_CLEAR_ADDRFLAG(__HANDLE__)
#endif
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
