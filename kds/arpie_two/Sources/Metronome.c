/*

 * Metronome.c
 *
 *  Created on: 19 Feb 2017
 *      Author: Jason
 */
#include "Arpie.h"


typedef struct {
	byte count;
	byte period;
} METRO_DATA;


static METRO_DATA metro[ARP_NUM_CHAINS];

enum
{
  RATE_1    = 96,
  RATE_2D   = 72,
  RATE_2    = 48,
  RATE_4D   = 36,
  RATE_2T   = 32,
  RATE_4    = 24,
  RATE_8D   = 18,
  RATE_4T   = 16,
  RATE_8    = 12,
  RATE_16D  = 9,
  RATE_8T   = 8,
  RATE_16   = 6,
  RATE_16T  = 4,
  RATE_32   = 3
};



void metro_init(byte which) {
	metro[which].count = 0;
	metro[which].period = 0;
}

void metro_on_tick(byte which) {
	METRO_DATA *me = &metro[which];
	switch(g_arp.chain[which].seq_rate) {
	case SEQ_RATE_1: 	me->period = RATE_1; break;
	case SEQ_RATE_2D:	me->period = RATE_2D; break;
	case SEQ_RATE_2:	me->period = RATE_2; break;
	case SEQ_RATE_4D:	me->period = RATE_4D; break;
	case SEQ_RATE_2T:	me->period = RATE_2T; break;
	case SEQ_RATE_4:	me->period = RATE_4; break;
	case SEQ_RATE_8D:	me->period = RATE_8D; break;
	case SEQ_RATE_4T:	me->period = RATE_4T; break;
	case SEQ_RATE_8:	me->period = RATE_8; break;
	case SEQ_RATE_16D:	me->period = RATE_16D; break;
	case SEQ_RATE_8T:	me->period = RATE_8T; break;
	case SEQ_RATE_16:	me->period = RATE_16; break;
	case SEQ_RATE_16T:	me->period = RATE_16T; break;
	case SEQ_RATE_32:	me->period = RATE_32; break;
	}
}
