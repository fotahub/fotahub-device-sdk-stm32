/*
 *  Copyright (C) 2022 FotaHub Inc. All rights reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of the FotaHub(R) Device SDK program (https://fotahub.com)
 */
#include "UARTDrv.h"

#include <stm32f4xx_hal.h>
#include <stm32f4xx.h>
#include <sys/unistd.h>

static UART_HandleTypeDef hUART;

void uart_init(void)
{
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_USART3_CLK_ENABLE();
  
  GPIO_InitTypeDef txInit = 
  {
    .Pin = GPIO_PIN_8, 
    .Mode = GPIO_MODE_AF_PP, 
    .Pull = GPIO_NOPULL, 
    .Speed = GPIO_SPEED_FREQ_HIGH, 
    .Alternate = GPIO_AF7_USART3
  };
  
  
  HAL_GPIO_Init(((GPIO_TypeDef *)(GPIOD_BASE)), &txInit);
  
  hUART.Instance = USART3;
  hUART.Init.BaudRate = 115200;
  hUART.Init.WordLength = UART_WORDLENGTH_8B;
  hUART.Init.StopBits = UART_STOPBITS_1;
  hUART.Init.Parity = UART_PARITY_NONE;
  hUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hUART.Init.Mode = UART_MODE_TX;
  hUART.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&hUART);
  
}

/* 
 * Writes characters to specified file
 */
int _write(int file, char const *ptr, int len)
{
  int32_t count = 0;
  for ( int32_t __i = 0 ; __i < len; __i++ )
  {
    while (__HAL_UART_GET_FLAG(&hUART, UART_FLAG_TXE) != SET)
    {
    }
    hUART.Instance->DR = ((uint32_t)((*ptr++)));
  }
  count = len;
  return count;
}
