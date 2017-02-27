/*
 * Keyboard.h
 *
 *  Created on: 22 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_KEYBOARD_H_
#define SOURCES_KEYBOARD_H_

class CKeyboard {
public:
	enum {
		SHIFT_MODIFIER = 0x01
	};
	enum {
		KEY_ZERO = 0,

			KEY_A1,  KEY_A2,           KEY_A3,  KEY_A4,  KEY_A5,
		KEY_B1,  KEY_B2,  KEY_B3,  KEY_B4,  KEY_B5,  KEY_B6,  KEY_B7,  KEY_B8,
		KEY_C1,  KEY_C2,  KEY_C3,  KEY_C4,  KEY_C5,
		KEY_D1,  KEY_D2,  KEY_D3,  KEY_D4,  KEY_D5,


		KEY_RELEASE = 0x80,

	};

	enum {
		KEYBIT_A1 = ((uint32_t)1<<0),
		KEYBIT_A2 = ((uint32_t)1<<1),
		KEYBIT_A3 = ((uint32_t)1<<2),
		KEYBIT_A4 = ((uint32_t)1<<3),
		KEYBIT_A5 = ((uint32_t)1<<4),
		KEYBIT_B1 = ((uint32_t)1<<5),
		KEYBIT_B2 = ((uint32_t)1<<6),
		KEYBIT_B3 = ((uint32_t)1<<7),
		KEYBIT_B4 = ((uint32_t)1<<8),
		KEYBIT_B5 = ((uint32_t)1<<9),
		KEYBIT_B6 = ((uint32_t)1<<10),
		KEYBIT_B7 = ((uint32_t)1<<11),
		KEYBIT_B8 = ((uint32_t)1<<12),
		KEYBIT_C1 = ((uint32_t)1<<13),
		KEYBIT_C2 = ((uint32_t)1<<14),
		KEYBIT_C3 = ((uint32_t)1<<15),
		KEYBIT_C4 = ((uint32_t)1<<16),
		KEYBIT_C5 = ((uint32_t)1<<17),
		KEYBIT_D1 = ((uint32_t)1<<18),
		KEYBIT_D2 = ((uint32_t)1<<19),
		KEYBIT_D3 = ((uint32_t)1<<20),
		KEYBIT_D4 = ((uint32_t)1<<21),
		KEYBIT_D5 = ((uint32_t)1<<22),

		KEYBIT_SHIFT = KEYBIT_D1,
	};
};
#endif /* SOURCES_KEYBOARD_H_ */
