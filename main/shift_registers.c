#include "shift_registers.h"
#include "bitwise.h"
#include "rom/ets_sys.h"

uint8_t shift_in(void)
{
    uint8_t incoming_data = 0;

    CLEAR_BIT(GPIO.out, PL_PIN);
    ets_delay_us(1);
    SET_BIT(GPIO.out, PL_PIN);
    ets_delay_us(1);

    for (uint8_t i = 0; i < 8; i++)
    {
        uint8_t current_bit = READ_BIT(GPIO.in, Q7_PIN);

        incoming_data |= (current_bit << (7 - i));

        SET_BIT(GPIO.out, CP_PIN);
        ets_delay_us(1);
        CLEAR_BIT(GPIO.out, CP_PIN);
        ets_delay_us(1);
    }

    return incoming_data;
}

void shift_out(uint8_t data_out)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (READ_BIT(data_out, 7 - i) == 1)
            SET_BIT(GPIO.out, SER_PIN);
        else
            CLEAR_BIT(GPIO.out, SER_PIN);

        ets_delay_us(1); // Stability dalay

        SET_BIT(GPIO.out, SRCLK_PIN);
        ets_delay_us(1);
        CLEAR_BIT(GPIO.out, SRCLK_PIN);
        ets_delay_us(1);
    }

    SET_BIT(GPIO.out, RCLK_PIN);
    ets_delay_us(1);
    CLEAR_BIT(GPIO.out, RCLK_PIN);
    ets_delay_us(1);
}