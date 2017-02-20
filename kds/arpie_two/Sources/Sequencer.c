/*
 * Sequencer.c
 *
 *  Created on: 19 Feb 2017
 *      Author: Jason
 */

#include "Arpie.h"

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
	/*
	 * X     X
	 * XX XX XX XX XX XX XX XX
	 * XX XX XX XX XX XX XX XX
	 *
	 * XX XX XX XX XX XX XX XX
	 * XX XX XX XX XX XX XX XX
	 *
	 * GLIDE
	 * XXXXX      XX XX
	 * XX XX      XXXXX
	 *
	 *
	 * XX
	 * X
	 */
	byte step_no = 0;
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
