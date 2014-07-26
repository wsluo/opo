#include <stdint.h>

#ifndef BLE_TEST
#define BLE_TEST

#define DELAY_INT_PORT       GPIO_B_NUM // RF 1.17 on Dev Board
#define DELAY_INT_PIN        5
#define DELAY_INT_VECTOR     NVIC_INT_GPIO_PORT_B
#define DELAY_INT_PORT_BASE  GPIO_PORT_TO_BASE(DELAY_INT_PORT)
#define DELAY_INT_PIN_MASK   GPIO_PIN_MASK(DELAY_INT_PIN)



#endif