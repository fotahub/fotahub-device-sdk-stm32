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
#ifndef BLINKDRV_H
#define BLINKDRV_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stm32f4xx_hal.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BLINK_GPIO_PIN (GPIO_PIN_0)

#define BLINK_TIMER_IRQn TIM5_IRQn

#define ms_to_micros_any(val) (val * 1000)

#define Hz_to_MHz_any(val) (val / 1000000)

void blink_init(uint16_t blinkPeriod);

void TIM5_IRQHandler(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
