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
#include "DemoFOTAUpdateWorkflowNVSFlashPersistence.h"

#include "DemoFOTAUpdateWorkflowPersistence.h"
#include <stm32f7xx_hal.h>
#include <stm32f7xx_hal_flash.h>
#include <stm32f7xx_hal_flash_ex.h>
#include <stm32f7xx_hal_def.h>

void saveState(int16_t state)
{
  uint32_t sectorError;
  HAL_FLASH_Unlock();
  
  
  FLASH_EraseInitTypeDef eraseInitStruct = { 0 };
  eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  eraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  eraseInitStruct.NbSectors = 1;
  eraseInitStruct.Sector = userDataSectorBank1;
  HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
  eraseInitStruct.Sector = userDataSectorBank2;
  HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
  
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, ((uint32_t)((userDataAddressBank1))), *((uint8_t *)((&state))));
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, ((uint32_t)((userDataAddressBank2))), *((uint8_t *)((&state))));
  
  HAL_FLASH_Lock();
}

int16_t loadState(void)
{
  int16_t state = 0;
  /* 
   * read sector
   */
  *(((uint8_t *)(&state))) = *((uint8_t volatile  *)(userDataAddressBank1));
  if (((uint8_t)(state)) == defaultValue) 
  {
    return 0;
  }
  return state;
}
