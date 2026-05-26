#ifndef TASKS_H_
#define TASKS_H_

#include "freertos/queue.h"
#include "freertos/semphr.h"

extern QueueHandle_t data_queue;
extern uint8_t switches_state;
extern SemaphoreHandle_t state_mutex;

void sensors_task(void *pvParameters);
void leds_task(void *pvParameters);

#endif