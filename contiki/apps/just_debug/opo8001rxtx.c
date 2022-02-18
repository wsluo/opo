#include "contiki.h"
#include "cloudcomm-802154.h"
#include "opo.h"
#include "lib/random.h"
#include "vtimer.h"
#include "simplestore.h"
#include "dev/gpio.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cc2538-rf-debug.h"
#include "blink.h"
#include "rtc.h"

PROCESS(opo8001rxtx, "opo8001rxtx");
PROCESS(opo8001rx, "opo8001rx");
PROCESS(opo8001tx, "opo8001tx");
AUTOSTART_PROCESSES(&opo8001rxtx);
static struct vtimer tx_delay;
static cloudcomm_meta_t cc_metadata;
static char murl[] = "http://post.gatd.io/a7ffd6e1-a973-4670-a2d3-4e79c9a4a390";
static opo_data_t mdata;
static bool sending_fail = false;







static unsigned short generate_rand_tx() {
	unsigned short r = random_rand();
	unsigned short timer = 2000 > r ? 2000 % r: r % 2000;
	return timer;
}

void init_callback() {
	process_poll(&opo8001rxtx);
}

void opo_rx_callback(opo_data_t odata) {
	memcpy(&mdata, &odata, sizeof(opo_data_t));
	char debug[100];
	snprintf(debug, 100, "Opo Rx Timing: %lu,time: %lu \n", mdata.range_dt, mdata.m_unixtime);
	send_rf_debug_msg(debug);
	blink_blue();
	process_poll(&opo8001rx);
}

void tx_delay_callback() {
	//send_rf_debug_msg("Opo Tx Delay Callback\n");
	process_poll(&opo8001tx);
}

void opo_tx_callback() {
	
		unsigned short delaytime = generate_rand_tx();
		blink_green();
		schedule_vtimer_ms(&tx_delay, 1000 + delaytime);
		enable_opo_rx();
	
}





PROCESS_THREAD(opo8001rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		send_rf_debug_msg("Opo rx process reenable\n");
		enable_opo_rx();
	}
	PROCESS_END();
}

PROCESS_THREAD(opo8001tx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		send_rf_debug_msg("Perform Opo TX\n");
		if(!perform_opo_tx()) {
			send_rf_debug_msg("Opo Tx failed woo\n");
			unsigned short delaytime = generate_rand_tx();
			//char buffer[100];
			//snprintf(buffer, 100, "Opo Tx Failed: %u\n", delaytime);
			//send_rf_debug_msg(buffer);
			sending_fail = true;
			schedule_vtimer_ms(&tx_delay, 1000 + delaytime);
		}
		else {
			sending_fail = false;
		}
	}
	PROCESS_END();
}

PROCESS_THREAD(opo8001rxtx, ev, data) {
	PROCESS_BEGIN();
	register_opo_rx_callback((opo_rx_callback_t) opo_rx_callback);
	register_opo_tx_callback(opo_tx_callback);
	tx_delay = get_vtimer(tx_delay_callback);
	process_start(&opo8001rx, NULL);
	process_start(&opo8001tx, NULL);

	cc_metadata.dest = &murl[0];
	cc_metadata.dest_len = 56;
	char buffer[50];
	snprintf(buffer, 50, "Opo Data Size:%u \n", sizeof(opo_data_t));
	send_rf_debug_msg(buffer);
	leds_on(LEDS_RED);
	leds_off(LEDS_RED);
	nvic_interrupt_enable(NVIC_INT_GPIO_PORT_A);
	/* Handle case where node is not plugged in on boot */

	unsigned short randtime = generate_rand_tx();
	schedule_vtimer(&tx_delay, 1000 +  randtime);
	enable_opo_rx();

	PROCESS_END();
}
