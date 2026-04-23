/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CBMC_SIZE 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
extern bool nondet_bool(void);
extern unsigned int nondet_uint(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t CBMC_TIMEOUT = nondet_uint();
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//   HAL_Init();

  /* USER CODE BEGIN Init */
  /* init() */
  hi2c1.Instance = I2C1;
  hi2c1.State = HAL_I2C_STATE_READY;
  hi2c1.Lock = HAL_UNLOCKED;
  hi2c1.PreviousState = ((uint32_t)(HAL_I2C_MODE_NONE));
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  /* USER CODE END Init */

  /* Configure the system clock */
//   SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
//   MX_GPIO_Init();
//   MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  uint8_t data[65536];
  for (int i = 0; i < CBMC_SIZE; i++) {
      data[i] = nondet_uint() & 0xFF;
  }

  /*
   * * Producer Vector
   *    每次讀取 index += 非負整數 non-deterministic。當超過 vector 長度時，non-deterministic 選擇 infinitely recurring V_x[recur, *] 的值中其中一個
   *    > e.g., BUSY bit 的 infinitely recurring = {0, 1}
   *    > SysTick 的 infinitely recurring = {Tickstart + 6, Tickstart + 7, ...} (因為 (HAL_GetTick() - Tickstart) > Timeout 在滿足之後因為 SysTick 為單調遞增，條件會變成無限次滿足，所以 infinitely recurring 就是這些滿足條件的值)
   * * 一個迴圈的 Sufficient BMC Bound B(v) = CBMC 需 unwind 此迴圈的數
   *    計算方式為 Code 5.4: 即為 max(所有此迴圈內互相 independent 的迴圈的 B(v)) * 外層迴圈的 B(v)
   *    針對 monotonic variable 會透過 p120 fitness value 的說明計算: B_x 即為 fitness value
   * 
   * * SysTick
   *    為 non-wrap-around monotonic variable，透過 Table 5.2 計算 fitness value
   *    > e.g., (HAL_GetTick() - Tickstart) > Timeout，對應 Table 5.2，predicate 為 x > C、update pattern 為 x += A (C = Tickstart + Timeout、A = 1、B = Tickstart)，若 Timeout 為 5，計算出 fitness value = 6
   *    > producer vector = {Tickstart, Tickstart + 1, ..., Tickstart + <fitness value>}
   *    > unwind 次數即為 producer vector 的長度
   *
   * * CR1 
   *    * POS: 初始值 non-deterministic
   *        因為 HAL_I2C_Master_Transmit() 讀取 POS 前並沒有寫入 POS，所以會將這個讀取值溯源到 V_INIT，故初始值為 non-deterministic
   *    * START: non-deterministic
   *    * STOP: non-deterministic
   * 
   * * DR
   *    transmitter mode: 不用管
   *    receiver mode: produder vector = {0x00, ..., 0xFF}
   *        hardware producer, firmware consumer
   * 
   * * SR1
   *    * SB: non-deterministic
   *    * ADD10: non-deterministic
   *    * AF: non-deterministic
   *    * ADDR: non-deterministic
   *    * TxE: non-deterministic
   *    * BTF: non-deterministic
   *    * ARLO: non-deterministic
   * 
   * * SR2
   *    * BUSY: non-deterministic
   */

  /* FUV */
  HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(&hi2c1, nondet_uint() & 0xFFFF, data, CBMC_SIZE, CBMC_TIMEOUT);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  // }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
uint32_t GetRegister(__IO uint32_t *reg_addr)
{
  uint32_t output = *reg_addr;
  uint32_t offset = (uint32_t)reg_addr & 0x3FF;

  switch (offset) {
    // CR1
    case offsetof(I2C_TypeDef, CR1):  
      // POS
      output = (output & ~I2C_CR1_POS) | (nondet_bool() ? I2C_CR1_POS : 0);

      // START
      output = (output & ~I2C_CR1_START) | (nondet_bool() ? I2C_CR1_START : 0);

      // STOP
      output = (output & ~I2C_CR1_STOP) | (nondet_bool() ? I2C_CR1_STOP : 0);

      __CPROVER_assume((output & I2C_CR1_PE) == I2C_CR1_PE);
      __CPROVER_assume((output & I2C_CR1_SWRST) != I2C_CR1_SWRST);

      break;
    
    case offsetof(I2C_TypeDef, SR1):
      // SB
      output = (output & ~I2C_SR1_SB) | (nondet_bool() ? I2C_SR1_SB : 0);

      // ADD10
      output = (output & ~I2C_SR1_ADD10) | (nondet_bool() ? I2C_SR1_ADD10 : 0);

      // AF
      output = (output & ~I2C_SR1_AF) | (nondet_bool() ? I2C_SR1_AF : 0);

      // ADDR
      output = (output & ~I2C_SR1_ADDR) | (nondet_bool() ? I2C_SR1_ADDR : 0);

      // TxE
      output = (output & ~I2C_SR1_TXE) | (nondet_bool() ? I2C_SR1_TXE : 0);

      // BTF
      output = (output & ~I2C_SR1_BTF) | (nondet_bool() ? I2C_SR1_BTF : 0);

      // ARLO
      output = (output & ~I2C_SR1_ARLO) | (nondet_bool() ? I2C_SR1_ARLO : 0);

      break;

    case offsetof(I2C_TypeDef, SR2):
      // BUSY
      output = (output & ~I2C_SR2_BUSY) | (nondet_bool() ? I2C_SR2_BUSY : 0);

      break;
  }

  return output;
}

void stub_SET_BIT(__IO uint32_t *REG_addr, uint32_t BIT)
{
  *REG_addr = GetRegister(REG_addr) | BIT;
}

void stub_CLEAR_BIT(__IO uint32_t *REG_addr, uint32_t BIT)
{
  *REG_addr = GetRegister(REG_addr) & ~BIT;
}

uint32_t stub_READ_BIT(__IO uint32_t *REG_addr, uint32_t BIT)
{
  return GetRegister(REG_addr) & BIT;
}

FlagStatus stub_HAL_I2C_GET_FLAG(I2C_HandleTypeDef *__HANDLE__, uint32_t __FLAG__)
{
  return (((uint8_t)((__FLAG__) >> 16U)) == 0x01U) ? (((GetRegister(&((__HANDLE__)->Instance->SR1)) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET) : (((GetRegister(&((__HANDLE__)->Instance->SR1)) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET);
}

void stub_HAL_I2C_CLEAR_ADDRFLAG(I2C_HandleTypeDef *__HANDLE__)
{
  do {
    __IO uint32_t tmpreg = 0x00U;
    tmpreg = GetRegister(&((__HANDLE__)->Instance->SR1));
    tmpreg = GetRegister(&((__HANDLE__)->Instance->SR2));
    UNUSED(tmpreg);
  } while(0);
}

uint32_t HAL_GetTick(void)
{
  static unsigned int idx = 0;
  static uint32_t tickstart;
  static bool first_call = true;

  if (first_call) {  // 第一次呼叫 HAL_GetTick() 是取得 tickstart
    first_call = false;
    tickstart = nondet_uint();
    uwTick = tickstart;
    return uwTick;
  }

  /* Fitness Value */
  const int A = 1, B = tickstart, C = tickstart + CBMC_TIMEOUT;
  const int fitness_value = (B - C) > 0 ? 1 : ((C - B) / A) + 1;  // Table 5.2
  
  /* Producer Vector V_x */
  const unsigned int V_x_len = fitness_value + 1;

  /* Infinitely Recurring Values V_x[recur, *] */
  int V_x_recur = tickstart + fitness_value + nondet_uint();

  idx += nondet_uint();

  if (idx < V_x_len) {
    unsigned int V_x = tickstart + idx * A;
    uwTick = V_x;
  }
  else {
    uwTick = V_x_recur;
  }

  return uwTick;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
