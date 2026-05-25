#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "shift_registers.h"
#include "tasks.h"
// #include "pd_wifi.h"

QueueHandle_t data_queue;

void sensors_task(void *pvParameters)
{
    uint8_t switches_state = 0;

    while (1)
    {
        // xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdPASS, portMAX_DELAY);

        switches_state = shift_in();

        xQueueSend(data_queue, &switches_state, 0);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void leds_task(void *pvParameters)
{
    uint8_t received_data = 0;

    while (1)
    {
        if (xQueueReceive(data_queue, &received_data, portMAX_DELAY) == pdPASS)
            shift_out(received_data);
    }
}