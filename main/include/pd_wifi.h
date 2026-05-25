#ifndef PD_WIFI_H_
#define PD_WIFI_H_

#include "esp_wifi.h"
#include "esp_event.h"
#include "stdint.h"

extern EventGroupHandle_t wifi_event_group;

#define WIFI_CONNECTED_BIT BIT0

void wifi_init_basic(void);
void start_network_provisioning(void);

#endif