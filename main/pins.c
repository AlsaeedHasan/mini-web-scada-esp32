#include "pins.h"
#include "bitwise.h"
#include "driver/gpio.h"

void init_shift_registers_gpios(void)
{
    gpio_reset_pin(SER_PIN);
    gpio_set_direction(SER_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(SRCLK_PIN);
    gpio_set_direction(SRCLK_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(RCLK_PIN);
    gpio_set_direction(RCLK_PIN, GPIO_MODE_OUTPUT);

    gpio_reset_pin(CP_PIN);
    gpio_set_direction(CP_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(PL_PIN);
    gpio_set_direction(PL_PIN, GPIO_MODE_OUTPUT);

    gpio_reset_pin(Q7_PIN);
    gpio_set_direction(Q7_PIN, GPIO_MODE_INPUT);

    CLEAR_BIT(GPIO.out, SER_PIN);
    CLEAR_BIT(GPIO.out, SRCLK_PIN);
    CLEAR_BIT(GPIO.out, RCLK_PIN);
    CLEAR_BIT(GPIO.out, CP_PIN);

    SET_BIT(GPIO.out, PL_PIN);
}
