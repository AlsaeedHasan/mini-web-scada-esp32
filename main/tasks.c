#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "shift_registers.h"
#include "tasks.h"
#include "freertos/semphr.h"
#include "esp_http_server.h"
#include "server.h"
// #include "pd_wifi.h"

QueueHandle_t data_queue;
uint8_t switches_state = 0;
SemaphoreHandle_t state_mutex = NULL;
extern void send_ws_message_worker(void *arg);

void sensors_task(void *pvParameters)
{
    uint8_t old_state = 0;

    while (1)
    {
        // xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdPASS, portMAX_DELAY);

        uint8_t temp_switches_state = shift_in();

        if (old_state != temp_switches_state)
        {
            old_state = temp_switches_state;

            if (xSemaphoreTake(state_mutex, pdMS_TO_TICKS(10)) == pdTRUE)
            {
                switches_state = temp_switches_state;
                xSemaphoreGive(state_mutex);
            }

            if (server != NULL)
                httpd_queue_work(server, send_ws_message_worker, (void *)((uint32_t)temp_switches_state));
        }

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