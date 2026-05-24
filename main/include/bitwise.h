#ifndef BITWISE_H_
#define BITWISE_H_

#define READ_BIT(REG, BIT) (((REG) >> (BIT)) & 1)
#define SET_BIT(REG, BIT) ((REG) |= (1 << (BIT)))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(1 << (BIT)))
#define TOGGLE_BIT(REG, BIT) ((REG) ^= (1 << (BIT)))

#endif