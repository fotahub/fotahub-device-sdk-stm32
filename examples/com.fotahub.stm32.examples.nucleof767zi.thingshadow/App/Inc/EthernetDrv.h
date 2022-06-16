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
#ifndef ETHERNETDRV_H
#define ETHERNETDRV_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "ethernetif.h"
#include <lwip/netif.h>
#include <lwip/dhcp.h>
#include <lwip/tcp.h>
#include <lwip/tcpip.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DHCP_TIMEOUT 10000

#define DHCP_RETRY_COUNT 3

enum EthernetStatus {
  ETHERNET_STATUS_IDLE = 0,
  ETHERNET_STATUS_STARTED = 1,
  ETHERNET_STATUS_CONNECTED = 2,
  ETHERNET_STATUS_GOT_IP = 3,
  ETHERNET_STATUS_DISCONNECTED = 4,
  ETHERNET_STATUS_STOPPED = 5
};
typedef enum EthernetStatus EthernetStatus_t;

struct EthernetControlData {
  struct netif networkInterface;
  uint32_t dhcpTimeout;
  uint8_t dhcpRetryCount;
  EthernetStatus_t currentStatus;
};
typedef struct EthernetControlData EthernetControlData_t;

void ethernet_Init(void);

void ethernet_Enable(void);

void ethernet_run(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
