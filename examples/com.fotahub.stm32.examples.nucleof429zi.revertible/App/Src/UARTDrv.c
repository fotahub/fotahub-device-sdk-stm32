/*
 *  Copyright (C) 2020-2021 FotaHub Inc. All rights reserved.
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
  
  GPIO_InitTypeDef rxInit = 
  {
    .Pin = GPIO_PIN_9, 
    .Mode = GPIO_MODE_AF_PP, 
    .Pull = GPIO_NOPULL, 
    .Speed = GPIO_SPEED_FREQ_HIGH, 
    .Alternate = GPIO_AF7_USART3
  };
  
  HAL_GPIO_Init(((GPIO_TypeDef *)(GPIOD_BASE)), &txInit);
  HAL_GPIO_Init(((GPIO_TypeDef *)(GPIOD_BASE)), &rxInit);
  
  hUART.Instance = USART3;
  hUART.Init.BaudRate = 115200;
  hUART.Init.WordLength = UART_WORDLENGTH_8B;
  hUART.Init.StopBits = UART_STOPBITS_1;
  hUART.Init.Parity = UART_PARITY_NONE;
  hUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hUART.Init.Mode = UART_MODE_TX_RX;
  hUART.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&hUART);
  
  __HAL_UART_ENABLE_IT(&hUART,UART_IT_RXNE);
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
}

void USART3_IRQHandler(void)
{
  uint32_t flag = __HAL_UART_GET_FLAG(&hUART,UART_FLAG_RXNE);
  uint32_t source = __HAL_UART_GET_IT_SOURCE(&hUART,UART_IT_RXNE);
  if (flag != false && source != false) 
  {
    firmwareUpdateInfoReader_onCharacterReceived(((uint8_t)(((hUART.Instance->DR) & (0x00FFu)))));
  }
}

/* 
 * Writes characters to specified file
 */
int _write(int file, char const *ptr, int len)
{
  int32_t count = 0;
  for ( int32_t __i = 0 ; __i < len; __i++ )
  {
    while (__HAL_UART_GET_FLAG(&hUART,UART_FLAG_TXE) != SET)
    {
    }
    hUART.Instance->DR = ((uint32_t)((*ptr++)));
  }
  count = len;
  return count;
}
