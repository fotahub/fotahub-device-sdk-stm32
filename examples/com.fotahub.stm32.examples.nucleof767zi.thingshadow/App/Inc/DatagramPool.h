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
#ifndef DATAGRAMPOOL_H
#define DATAGRAMPOOL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "Datagram.h"

#ifdef __cplusplus
extern "C" {
#endif

struct IDatagramPool__idata {
  void *__instance;
  Datagram_t *((*(newDatagram))(size_t,void *));
  Datagram_t *((*(resizeDatagram))(Datagram_t *,size_t,void *));
  void ((*(deleteDatagram))(Datagram_t *,void *));
};
typedef struct IDatagramPool__idata IDatagramPool__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
