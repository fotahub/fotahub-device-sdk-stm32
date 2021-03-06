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
#include "main.h"

#include "Configuration.h"
#include "DemoFOTAUpdateWorkflow.h"
#include "UARTDrv.h"
#include "EthernetDrv.h"
#include "BlinkDrv.h"
#include "DemoProductInfo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SystemClockConfig.h"
#include <stm32f4xx_hal.h>

static void fotaUpdateTasks(void *args);

int main(int argc, char *argv[])
{
  HAL_Init();
  SystemClock_Config();
  uart_init();
  ethernet_Init();
  blink_init(BLINK_PERIOD);
  
  printf("\n--------------------------------------------------------------------------\n");
  printf("Running %s %s firmware\n", DEMO_PRODUCT_NAME, DEMO_PRODUCT_FIRMWARE_VERSION);
  printf("--------------------------------------------------------------------------\n\n");
  
  firmwareUpdateInfoReader_init();
  fotaUpdateWorkflow_init();
  
  fotahub_init(DEMO_PRODUCT_ID, DEMO_PRODUCT_NAME, &STM32_FOTA_UPDATE_CLIENT_CONFIG);
  
  xTaskCreate(&fotaUpdateTasks,"fotaUpdateTasks",FOTA_UPDATE_TASK_STACK_SIZE * 3,NULL,2,NULL);
  
  ethernet_Enable();
  
  vTaskStartScheduler();
  
  return 0;
}

static void fotaUpdateTasks(void *args)
{
  while (true)
  {
    ethernet_run();
    fotahub_run();
    firmwareUpdateInfoReader_run();
  }
}
