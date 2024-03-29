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
#include "main.h"

#include "Configuration.h"
#include "DemoFOTAUpdateWorkflow.h"
#include "DemoFOTAUpdateWorkflowNVSFlashPersistence.h"
#include "DemoFOTAUpdateSelfTest.h"
#include "UARTDrv.h"
#include "EthernetDrv.h"
#include "BlinkDrv.h"
#include "DemoProductInfo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SystemClockConfig.h"
#include <stm32f7xx_hal.h>

static void updateTask(void *pvParameters);

int main(int argc, char *argv[])
{
  HAL_Init();
  SystemClock_Config();
  uart_init();
  ethernet_Init();
  blink_init(BLINK_PERIOD);
  
  printf("\r\n--------------------------------------------------------------------------\r\n");
  printf("Running %s %s firmware\r\n", DEMO_PRODUCT_NAME, DEMO_PRODUCT_FIRMWARE_VERSION);
  printf("--------------------------------------------------------------------------\r\n\r\n");
  
  firmwareUpdateInfoReader_init();
  fotaUpdateWorkflow_init();
  
  fotahub_init(DEMO_PRODUCT_ID, DEMO_PRODUCT_NAME, &STM32_FOTA_UPDATE_CLIENT_CONFIG);
  
  xTaskCreate(&updateTask, "updateTask", FOTA_UPDATE_TASK_STACK_SIZE * 3, NULL, 2, NULL);
  
  ethernet_Enable();
  
  vTaskStartScheduler();
  
  return 0;
}

static void updateTask(void *pvParameters)
{
  while (true)
  {
    ethernet_run();
    fotahub_run();
    firmwareUpdateInfoReader_run();
  }
}
