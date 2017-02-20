/*
 * Encoder.c
 *
 *  Created on: 11 Feb 2017
 *      Author: Jason
 */

#include <stdio.h>

#include "Arpie.h"


#define ENC_PORT GPIOA_BASE_PTR
#define ENC_A GPIO_PDD_PIN_11
#define ENC_B GPIO_PDD_PIN_10

byte s_prev_state[3] = {0};

void encoder_init() {
	GPIO_PDD_SetPortInputDirectionMask(ENC_PORT, (ENC_A|ENC_B));
}

int encoder_run() {

	int delta = 0;
	// get the state of the two inputs into a 2 bit value
	uint32_t input = GPIO_PDD_GetPortDataInput(ENC_PORT);
	byte new_state = 0;
	if(!(input & ENC_A)) {
		new_state |= 0b10;
	}
	if(!(input & ENC_B)) {
		new_state |= 0b01;
	}


	// make sure the state has changed and does not match
	// the previous state (which may indicate a bounce)
	if(new_state != s_prev_state[0] && new_state != s_prev_state[1]) {

		if(new_state == 0b11) {
			if( (s_prev_state[0] == 0b10) &&
				(s_prev_state[1] == 0b00) &&
				(s_prev_state[2] == 0b01)) {
				delta = 1;
			}
			else if( (s_prev_state[0] == 0b01) &&
				(s_prev_state[1] == 0b00) &&
				(s_prev_state[2] == 0b10)) {
				delta = -1;
			}
		}

		s_prev_state[2] = s_prev_state[1];
		s_prev_state[1] = s_prev_state[0];
		s_prev_state[0] = new_state;
	}

	return delta;
}


