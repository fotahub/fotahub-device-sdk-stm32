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
#include "EthernetDrv.h"

#include "FotaHub.h"
#include <stm32f7xx_hal.h>

enum EthernetEngine__inevents {
  EthernetEngine_enable__event,
  EthernetEngine_disable__event,
  EthernetEngine_linkUp__event,
  EthernetEngine_linkDown__event,
  EthernetEngine_dhcpSuccess__event,
  EthernetEngine_dhcpFailed__event
};
typedef enum EthernetEngine__inevents EthernetEngine__inevents_t;

enum EthernetEngine__states {
  EthernetEngine_stopped__state = 0,
  EthernetEngine_started__state,
  EthernetEngine_gettingIP__state,
  EthernetEngine_connected__state,
  EthernetEngine_disconnected__state
};
typedef enum EthernetEngine__states EthernetEngine__states_t;

struct EthernetEngine__data {
  EthernetEngine__states_t __currentState;
};
typedef struct EthernetEngine__data EthernetEngine__data_t;

static void EthernetEngine__init(EthernetEngine__data_t *instance);

static bool EthernetEngine__execute(EthernetEngine__data_t *instance, EthernetEngine__inevents_t event, void **arguments);

static inline void EthernetEngine_gettingIP_EntryAction(EthernetEngine__data_t *instance);

static EthernetControlData_t controlData = { 0 };

static EthernetEngine__data_t engine;

void ethernet_Init(void)
{
  EthernetEngine__init(&engine);
  controlData.dhcpRetryCount = 0;
  controlData.dhcpTimeout = 0;
  controlData.currentStatus = ETHERNET_STATUS_IDLE;
}

void ethernet_Enable(void)
{
  EthernetEngine__execute(&engine, EthernetEngine_enable__event, NULL);
}

static void EthernetEngine__init(EthernetEngine__data_t *instance)
{
  instance->__currentState = EthernetEngine_stopped__state;
}

static bool EthernetEngine__execute(EthernetEngine__data_t *instance, EthernetEngine__inevents_t event, void **arguments)
{
  switch (instance->__currentState)
  {
    case EthernetEngine_stopped__state:
    {
      switch (event)
      {
        case EthernetEngine_enable__event:
        {
          /* 
           * transition actions
           */
          tcpip_init(NULL, NULL);
          ip_addr_t ip = 
          {
            .addr = 0
          };
          ip_addr_t mask = 
          {
            .addr = 0
          };
          ip_addr_t g = 
          {
            .addr = 0
          };
          netif_add(&controlData.networkInterface, &ip, &mask, &g, NULL, &ethernetif_init, &tcpip_input);
          netif_set_default(&controlData.networkInterface);
          controlData.currentStatus = ETHERNET_STATUS_STARTED;
          
          if (instance->__currentState == EthernetEngine_stopped__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = EthernetEngine_started__state;
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case EthernetEngine_started__state:
    {
      switch (event)
      {
        case EthernetEngine_linkUp__event:
        {
          /* 
           * transition actions
           */
          netif_set_up(&controlData.networkInterface);
          controlData.dhcpRetryCount = 0;
          controlData.currentStatus = ETHERNET_STATUS_CONNECTED;
          
          if (instance->__currentState == EthernetEngine_started__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = EthernetEngine_gettingIP__state;
            EthernetEngine_gettingIP_EntryAction(instance);
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case EthernetEngine_gettingIP__state:
    {
      switch (event)
      {
        case EthernetEngine_dhcpFailed__event:
        {
          if (controlData.dhcpRetryCount == DHCP_RETRY_COUNT) 
          {
            /* 
             * transition actions
             */
            netif_remove(&controlData.networkInterface);
            controlData.dhcpRetryCount = 0;
            controlData.currentStatus = ETHERNET_STATUS_DISCONNECTED;
            
            if (instance->__currentState == EthernetEngine_gettingIP__state) 
            {
              /* 
               * enter target state
               */
              instance->__currentState = EthernetEngine_stopped__state;
            }
            break;
          }
          if (controlData.dhcpRetryCount < DHCP_RETRY_COUNT) 
          {
            /* 
             * transition actions
             */
            controlData.dhcpRetryCount++;
            
            /* 
             * enter target state
             */
            EthernetEngine_gettingIP_EntryAction(instance);
          }
          break;
        }
        case EthernetEngine_dhcpSuccess__event:
        {
          /* 
           * transition actions
           */
          controlData.dhcpRetryCount = 0;
          controlData.currentStatus = ETHERNET_STATUS_GOT_IP;
          printf("Got IP address %lu.%lu.%lu.%lu\r\n", (controlData.networkInterface.ip_addr.addr & 0xffu), (controlData.networkInterface.ip_addr.addr >> 8) & 0xffu, (controlData.networkInterface.ip_addr.addr >> 16) & 0xffu, (controlData.networkInterface.ip_addr.addr >> 24) & 0xffu);
          
          /* 
           * enter target state
           */
          instance->__currentState = EthernetEngine_connected__state;
          break;
        }
        case EthernetEngine_disable__event:
        {
          /* 
           * transition actions
           */
          netif_remove(&controlData.networkInterface);
          
          if (instance->__currentState == EthernetEngine_gettingIP__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = EthernetEngine_stopped__state;
          }
          break;
        }
        case EthernetEngine_linkDown__event:
        {
          /* 
           * transition actions
           */
          netif_set_down(&controlData.networkInterface);
          controlData.currentStatus = ETHERNET_STATUS_DISCONNECTED;
          
          if (instance->__currentState == EthernetEngine_gettingIP__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = EthernetEngine_disconnected__state;
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case EthernetEngine_connected__state:
    {
      switch (event)
      {
        case EthernetEngine_disable__event:
        {
          /* 
           * transition actions
           */
          netif_remove(&controlData.networkInterface);
          controlData.currentStatus = ETHERNET_STATUS_STOPPED;
          
          if (instance->__currentState == EthernetEngine_connected__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = EthernetEngine_stopped__state;
          }
          break;
        }
        case EthernetEngine_linkDown__event:
        {
          /* 
           * transition actions
           */
          netif_set_down(&controlData.networkInterface);
          controlData.currentStatus = ETHERNET_STATUS_DISCONNECTED;
          
          if (instance->__currentState == EthernetEngine_connected__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = EthernetEngine_disconnected__state;
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case EthernetEngine_disconnected__state:
    {
      switch (event)
      {
        case EthernetEngine_disable__event:
        {
          /* 
           * transition actions
           */
          netif_remove(&controlData.networkInterface);
          controlData.currentStatus = ETHERNET_STATUS_STOPPED;
          
          if (instance->__currentState == EthernetEngine_disconnected__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = EthernetEngine_stopped__state;
          }
          break;
        }
        case EthernetEngine_linkUp__event:
        {
          /* 
           * transition actions
           */
          netif_set_up(&controlData.networkInterface);
          controlData.currentStatus = ETHERNET_STATUS_CONNECTED;
          
          if (instance->__currentState == EthernetEngine_disconnected__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = EthernetEngine_gettingIP__state;
            EthernetEngine_gettingIP_EntryAction(instance);
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    default: {
      break;
    }
  }
  return true;
}

void ethernet_run(void)
{
  ethernetif_set_link(&controlData.networkInterface);
  if (netif_is_link_up(&controlData.networkInterface)) 
  {
    EthernetEngine__execute(&engine, EthernetEngine_linkUp__event, NULL);
  }
  else
  {
    EthernetEngine__execute(&engine, EthernetEngine_linkDown__event, NULL);
  }
  
  if ((engine.__currentState == EthernetEngine_gettingIP__state)) 
  {
    if (controlData.networkInterface.ip_addr.addr != 0) 
    {
      EthernetEngine__execute(&engine, EthernetEngine_dhcpSuccess__event, NULL);
    }
    else if (HAL_GetTick() - controlData.dhcpTimeout > (DHCP_TIMEOUT)) {
      EthernetEngine__execute(&engine, EthernetEngine_dhcpFailed__event, NULL);
    }
  }
  
  if (controlData.currentStatus != ETHERNET_STATUS_IDLE) 
  {
    if (controlData.currentStatus == ETHERNET_STATUS_GOT_IP) 
    {
      fotahub_notifyNetworkStatusChange(NETWORK_ADAPTER_STATUS_CONNECTED);
    }
    else if (controlData.currentStatus > ETHERNET_STATUS_GOT_IP) {
      fotahub_notifyNetworkStatusChange(NETWORK_ADAPTER_STATUS_DISCONNECTED);
    }
    controlData.currentStatus = ETHERNET_STATUS_IDLE;
  }
}

static inline void EthernetEngine_gettingIP_EntryAction(EthernetEngine__data_t *instance)
{
  dhcp_start(&controlData.networkInterface);
  controlData.dhcpTimeout = HAL_GetTick();
}
