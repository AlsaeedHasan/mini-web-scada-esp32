#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "bitwise.h"
#include "pins.h"
#include "shift_registers.h"
#include "tasks.h"
#include "pd_wifi.h"

void app_main(void)
{
    wifi_event_group = xEventGroupCreate();

    data_queue = xQueueCreate(5, sizeof(uint8_t));

    if (data_queue != NULL && wifi_event_group != NULL)
    {
        wifi_init_basic();
        start_network_provisioning();

        init_shift_registers_gpios();

        xTaskCreate(sensors_task, "Sensors Read Task", 2048, NULL, 1, NULL);
        xTaskCreate(leds_task, "Data Out Task", 2048, NULL, 1, NULL);

        printf("System Initialized...\n");
    }
    else
        printf("Failed to create RTOS primitives!\n");
}
