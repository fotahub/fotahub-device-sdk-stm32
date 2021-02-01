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
#include "BlinkDrv.h"

#define BlinkDrv_BLINK_GPIO_PORT (GPIOB)

static TIM_HandleTypeDef hTimer = { 0 };

void blink_init(uint16_t blinkPeriod)
{
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin = BLINK_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  __HAL_RCC_TIM5_CLK_ENABLE();
  hTimer.Instance = TIM5;
  /* 
   * The prescaler is SystemCoreClock / 2 for TIM5
   */
  hTimer.Init.Prescaler = ((uint32_t)((((uint32_t)(Hz_to_MHz_any(((int64_t)((SystemCoreClock / 2)))))) - 1)));
  hTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
  hTimer.Init.Period = (((uint32_t)(ms_to_micros_any(((int64_t) blinkPeriod)))) >> 1) - 1;
  hTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  hTimer.Init.AutoReloadPreload = 0;
  
  if (HAL_TIM_Base_Init(&hTimer) != HAL_OK) 
  {
    while (true)
    {
    }
  }
  
  if (HAL_TIM_Base_Start_IT(&hTimer) != HAL_OK) 
  {
    while (true)
    {
    }
  }
  
  HAL_NVIC_SetPriority(BLINK_TIMER_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(BLINK_TIMER_IRQn);
}

void TIM5_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&hTimer);
  HAL_GPIO_TogglePin(BlinkDrv_BLINK_GPIO_PORT, BLINK_GPIO_PIN);
}
