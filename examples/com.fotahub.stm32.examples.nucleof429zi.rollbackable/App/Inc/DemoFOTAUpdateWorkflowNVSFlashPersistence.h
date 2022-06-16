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
#ifndef DEMOFOTAUPDATEWORKFLOWNVSFLASHPERSISTENCE_H
#define DEMOFOTAUPDATEWORKFLOWNVSFLASHPERSISTENCE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define userDataAddressBank1 0x080E0000u

#define userDataAddressBank2 0x081E0000u

#define userDataSectorBank1 11

#define userDataSectorBank2 23

#define defaultValue 0xffu

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
