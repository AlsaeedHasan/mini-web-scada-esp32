#ifndef TASKS_H_
#define TASKS_H_

#include "freertos/queue.h"

extern QueueHandle_t data_queue;

void sensors_task(void *pvParameters);
void leds_task(void *pvParameters);

#endif