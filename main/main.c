#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "bitwise.h"
#include "pins.h"
#include "shift_registers.h"
#include "tasks.h"

void app_main(void)
{
    init_shift_registers_gpios();

    data_queue = xQueueCreate(5, sizeof(uint8_t));

    if (data_queue != NULL)
    {
        xTaskCreate(sensors_task, "Sensors Read Task", 2048, NULL, 1, NULL);
        xTaskCreate(leds_task, "Data Out Task", 2048, NULL, 1, NULL);
    }
    else
        printf("Queue creation failed!");
}
