/*
 * Encoder.h
 *
 *  Created on: 20 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_ENCODER_H_
#define SOURCES_ENCODER_H_

class CEncoder {
	const uint32_t ENC_A = GPIO_PDD_PIN_11;
	const uint32_t ENC_B = GPIO_PDD_PIN_10;
	const GPIO_MemMapPtr ENC_PORT = GPIOA_BASE_PTR;
	byte m_prev_state[3] = {0};
public:
	CEncoder() {
		GPIO_PDD_SetPortInputDirectionMask(ENC_PORT, (ENC_A|ENC_B));
	}
	int read() {

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
		if(new_state != m_prev_state[0] && new_state != m_prev_state[1]) {

			if(new_state == 0b11) {
				if( (m_prev_state[0] == 0b10) &&
					(m_prev_state[1] == 0b00) &&
					(m_prev_state[2] == 0b01)) {
					delta = 1;
				}
				else if( (m_prev_state[0] == 0b01) &&
					(m_prev_state[1] == 0b00) &&
					(m_prev_state[2] == 0b10)) {
					delta = -1;
				}
			}

			m_prev_state[2] = m_prev_state[1];
			m_prev_state[1] = m_prev_state[0];
			m_prev_state[0] = new_state;
		}

		return delta;
	}
};


#endif /* SOURCES_ENCODER_H_ */
