/*
 * Sequencer.h
 *
 *  Created on: 20 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_SEQUENCER_H_
#define SOURCES_SEQUENCER_H_

/*
 * Sequencer.c
 *
 *  Created on: 19 Feb 2017
 *      Author: Jason
 */

#include "Arpie.h"
/*
typedef struct {
	byte cur_step;
} SEQ_DATA;

static SEQ_DATA seq_data[ARP_NUM_CHAINS];

void seq_init(byte which) {

}
void seq_render(byte which, byte *raster, byte *highlight) {

	uint32_t temp_raster[8] = {0};
	uint32_t temp_highlight[8] = {0};

	ARP_CHAIN_CONFIG *arp = &g_arp.chain[which];
	SEQ_DATA *me =&seq_data[which];
 step_no = 0;
	byte base_row = 0;
	for(byte row = 0; row < 2; ++row) {

		uint32_t mask1 = ((uint32_t)0b100)<<21;
		uint32_t mask2 = ((uint32_t)0b110)<<21;
		for(byte col = 0; col < 8; ++col) {

			if(step_no < arp->seq_num_steps) {

				uint32_t row0 = 0;
				uint32_t row1 = 0;
				uint32_t row2 = 0;

				if(arp->seq_step[step_no] & SEQ_STEP_ACCENT) {
					row0 |= mask1;
				}
				if(arp->seq_step[step_no] & SEQ_STEP_TRANSPOSE) {
					row0 |= (mask1>>1);
				}
				row1 |= mask2;
				if(arp->seq_step[step_no] & SEQ_STEP_FULL_GATE) {
					row1 |= (mask1>>2);
				}
				row2 |= mask2;
				if(arp->seq_step[step_no] & SEQ_STEP_TIED_GATE) {
					row2 |= (mask1>>2);
				}

				if((arp->seq_step[step_no] & SEQ_STEP_PLAY)) {
					temp_raster[base_row+1] |= row0;
					temp_raster[base_row+2] |= row1;
					temp_raster[base_row+3] |= row2;
				}
				if(!(arp->seq_step[step_no] & SEQ_STEP_PLAY) || (step_no == me->cur_step)) {
					temp_highlight[base_row+1] |= row0;
					temp_highlight[base_row+2] |= row1;
					temp_highlight[base_row+3] |= row2;
				}
			}
			mask1 >>= 3;
			mask2 >>= 3;
			++step_no;
		}
		base_row += 4;
	}

	for(int i=0; i<8; ++i) {
		*raster++ = temp_raster[i] >> 16;
		*raster++ = temp_raster[i] >> 8;
		*raster++ = temp_raster[i];

		*highlight++ = temp_highlight[i] >> 16;
		*highlight++ = temp_highlight[i] >> 8;
		*highlight++ = temp_highlight[i];
	}
}


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
*/




#endif /* SOURCES_SEQUENCER_H_ */
