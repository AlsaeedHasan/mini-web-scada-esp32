#ifndef PINS_H_
#define PINS_H_

#include "soc/gpio_struct.h"

// 74HC595
#define SER_PIN 22
#define SRCLK_PIN 23
#define RCLK_PIN 25

// 74HC165
#define Q7_PIN 5
#define CP_PIN 15
#define PL_PIN 18

void init_shift_registers_gpios(void);

#endif