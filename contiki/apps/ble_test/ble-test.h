#include <stdint.h>

#ifndef BLE_TEST
#define BLE_TEST

#define DELAY_INT_PORT       GPIO_B_NUM // RF 1.17 on Dev Board
#define DELAY_INT_PIN        5
#define DELAY_INT_VECTOR     NVIC_INT_GPIO_PORT_B
#define DELAY_INT_PORT_BASE  GPIO_PORT_TO_BASE(DELAY_INT_PORT)
#define DELAY_INT_PIN_MASK   GPIO_PIN_MASK(DELAY_INT_PIN)

#define NB_SETUP_MESSAGES 14
#define SETUP_MESSAGES_CONTENT {\
    {0x00,\
        {\
            0x07,0x06,0x00,0x00,0x03,0x02,0x42,0x07,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x06,0x00,0x01,\
            0xc1,0x0a,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x03,0x90,0x00,0xff,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x38,0xff,0xff,0x02,0x58,0x0a,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,\
            0x00,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x05,0x06,0x10,0x54,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x00,0x04,0x04,0x02,0x02,0x00,0x01,0x28,0x00,0x01,0x00,0x18,0x04,0x04,0x05,0x05,0x00,\
            0x02,0x28,0x03,0x01,0x02,0x03,0x00,0x00,0x2a,0x04,0x04,0x14,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x1c,0x05,0x00,0x03,0x2a,0x00,0x01,0x4f,0x70,0x6f,0x56,0x33,0x63,0x73,0x65,0x6d,0x69,\
            0x2e,0x63,0x6f,0x6d,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x38,0x05,0x05,0x00,0x04,0x28,0x03,0x01,0x02,0x05,0x00,0x01,0x2a,0x06,0x04,0x03,0x02,\
            0x00,0x05,0x2a,0x01,0x01,0x47,0x01,0x04,0x04,0x05,0x05,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x54,0x06,0x28,0x03,0x01,0x02,0x07,0x00,0x04,0x2a,0x06,0x04,0x09,0x08,0x00,0x07,0x2a,\
            0x04,0x01,0x06,0x00,0x06,0x00,0x02,0x00,0xff,0xff,0x04,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x70,0x02,0x02,0x00,0x08,0x28,0x00,0x01,0x01,0x18,0x04,0x04,0x02,0x02,0x00,0x09,0x28,\
            0x00,0x01,0x0a,0x18,0x04,0x04,0x05,0x05,0x00,0x0a,0x28,0x03,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x8c,0x01,0x02,0x0b,0x00,0x24,0x2a,0x06,0x04,0x09,0x08,0x00,0x0b,0x2a,0x24,0x01,0x6e,\
            0x52,0x46,0x78,0x78,0x78,0x78,0x78,0x06,0x04,0x08,0x07,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xa8,0x0c,0x29,0x04,0x01,0x19,0x00,0x00,0x00,0x01,0x00,0x00,0x04,0x04,0x05,0x05,0x00,\
            0x0d,0x28,0x03,0x01,0x02,0x0e,0x00,0x25,0x2a,0x06,0x04,0x05,\
        },\
    },\
    {0x00,\
        {\
            0x1e,0x06,0x20,0xc4,0x04,0x00,0x0e,0x2a,0x25,0x01,0x31,0x35,0x38,0x37,0x06,0x04,0x08,0x07,0x00,0x0f,\
            0x29,0x04,0x01,0x19,0x00,0x00,0x00,0x01,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x06,0x06,0xf0,0x00,0x03,0xd2,0x36,\
        },\
    },\
}

#endif