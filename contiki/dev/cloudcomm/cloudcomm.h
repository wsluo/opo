#include "nrf8001.h"
#include "contiki.h"
#include "sys/clock.h"
#include "spi-arch.h"
#include "spi.h"
#include "dev/ssi.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "dev/leds.h"
#include "cpu.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "nrf8001_setup.h"
#include "rf_switch.h"
#include "simplestore.h"
#include "board.h"
#include "rv4162.h"
#include "vtimer.h"
#include "dev/cc2538-rf.h"


#define CLOUDCOMM_REQ_TIME 			0
#define CLOUDCOMM_REQ_LOC			1
#define CLOUDCOMM_REQ_STEPS			2 // Needs a time parameter
#define CC_MAX_DATA_LENGTH	        19
#define CLOUDCOMM_REQ_QUEUE_LENGTH  3
#define CLOUDCOMM_PAGE_SIZE         254 // Flash page size - metadata size

typedef void (* cloudcomm_callback_t)(uint8_t packet[30], uint8_t len);

typedef struct cloudcomm_metainfo {
	char *dest;
	uint8_t dest_len;
} cloudcomm_meta_t;

void cloudcomm_init();

uint8_t cloudcomm_on(void *callback, uint16_t ontime); // ontime in s

void register_cloudcomm_callback(cloudcomm_callback_t callback, uint8_t req);

void cloudcomm_set_metainfo(cloudcomm_meta_t *metainfo);
void cloudcomm_set_packet_length(uint8_t len);
uint8_t cloudcomm_store(void *data);
uint8_t cloudcomm_request_data(uint8_t req);
void cloudcomm_set_rx_callback();
void cloudcomm_clear_data();
