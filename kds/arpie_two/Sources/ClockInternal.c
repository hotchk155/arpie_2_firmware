/*
 * ClockInternal.c
 *
 *  Created on: 19 Feb 2017
 *      Author: Jason
 */

#include "Arpie.h"
#define MS_PER_MIN (60 * 1000)
#define TICKS_PQN 24

typedef struct {
	double tick_period;
	double next_tick;
} CLOCKINT_DATA;

static CLOCKINT_DATA me;

void clockint_init() {
	me.tick_period = 500;
	me.next_tick = 0;
}
void clockint_on_set_bpm() {
	me.tick_period = (double)MS_PER_MIN/(g_state.bpm * TICKS_PQN);
}
byte clockint_is_tick() {
	if(g_millis >= me.next_tick) {
		me.next_tick += me.tick_period;
		if(g_millis >= me.next_tick) {
			me.next_tick = g_millis + me.tick_period;
		}
		return 1;
	}
	return 0;
}
