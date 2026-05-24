#ifndef SHIFT_REGISTERS_H_
#define SHIFT_REGISTERS_H_

#include "pins.h"
#include "stdint.h"

uint8_t shift_in(void);
void shift_out(uint8_t data_out);

#endif