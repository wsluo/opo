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
AUTOSTART_PROCESSES(&opo8001rxtx);
static char murl[] = "http://post.gatd.io/a7ffd6e1-a973-4670-a2d3-4e79c9a4a390";
static opo_data_t mdata;
static bool sending_fail = false;
extern uint32_t last_ul_count;   //ul count of last successful ranging evant
extern uint32_t wakeup_count;    //total wakeup
extern uint32_t ul_rf_dt;
extern uint32_t ul_wakeup_time;
extern uint32_t sfd_time;
extern uint32_t wakeup_done_count; //successful wakeup

uint32_t d_ul_wakeup_time=0;
uint32_t d_sfd_time=0;
uint32_t last_ul_wakeup_time=0;
uint32_t last_sfd_time=0;

void init_callback() {
	process_poll(&opo8001rxtx);
}

void opo_rx_callback(opo_data_t odata) {
	memcpy(&mdata, &odata, sizeof(opo_data_t));
	char debug[100];
        d_ul_wakeup_time=ul_wakeup_time-last_ul_wakeup_time;
        d_sfd_time=sfd_time-last_sfd_time;
        last_ul_wakeup_time=ul_wakeup_time;
        last_sfd_time=sfd_time;

        snprintf(debug, 100, "Timing: %d,ul_count: %lu,wk_c: %lu,wk_done_c: %lu, d_wk: %d, d_sfd: %d \n", mdata.range_dt,last_ul_count, wakeup_count, wakeup_done_count,d_ul_wakeup_time, d_sfd_time);
	send_rf_debug_msg(debug);
	blink_blue();
	process_poll(&opo8001rx);
}


PROCESS_THREAD(opo8001rx, ev, data) {
	PROCESS_BEGIN();
	while(1) {
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
		//send_rf_debug_msg("Opo rx process reenable\n");
		enable_opo_rx();
	}
	PROCESS_END();
}


PROCESS_THREAD(opo8001rxtx, ev, data) {
	PROCESS_BEGIN();
	register_opo_rx_callback((opo_rx_callback_t) opo_rx_callback);
	process_start(&opo8001rx, NULL);

	char buffer[50];
	snprintf(buffer, 50, "Opo Data Size:%u \n", sizeof(opo_data_t));
	send_rf_debug_msg(buffer);
	leds_on(LEDS_RED);
	leds_off(LEDS_RED);
	nvic_interrupt_enable(NVIC_INT_GPIO_PORT_A);
	/* Handle case where node is not plugged in on boot */

        enable_opo_rx();
	PROCESS_END();
}
