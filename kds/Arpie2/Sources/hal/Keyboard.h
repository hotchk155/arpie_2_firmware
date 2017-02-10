/*
 * Keyboard.h
 *
 *  Created on: 9 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_HAL_KEYBOARD_H_
#define SOURCES_HAL_KEYBOARD_H_

#include "Definitions.h"

class CKeyboard : public II2CMessageListener
{
public:
	enum {
		MAX_SIZE = 5,
		POLL_PERIOD = 10 //ms
	};
	// Define constants for individual keys
	enum {
		NONE = 0,
		   A1,  A2,	      A3,  A4,  A5,
		B1,  B2,  B3,  B4,  B5,  B6,  B7,  B8,
		C1,  C2,  C3,  C4,  C5,
		D1,  D2,  D3,  D4,  D5,

		PRESS 	= 0,
		RELEASE = 0x80
	};
protected:
	volatile byte m_fifo[MAX_SIZE];
	volatile byte m_head;
	volatile byte m_tail;
	int m_next_poll_due;
public:
	II2CDriver *m_i2c;
	CKeyboard() {
		m_head = 0;
		m_tail = 0;
		m_next_poll_due = 0;
	}
	void on_i2c_msg(byte addr, byte *data, byte len) {
		byte next = m_head+1;
		if(next >= MAX_SIZE) {
			next = 0;
		}
		if(next != m_tail) {
			m_fifo[m_head] = *data;
			m_head = next;
		}
	}
	byte get() {
		if(m_head == m_tail) {
			return 0;
		}
		byte result = m_fifo[m_tail];
		if(++m_tail >= MAX_SIZE) {
			m_tail = 0;
		}
		return result;
	}
	void tick() {
		if((--m_next_poll_due <= 0) && m_i2c) {
			m_i2c->receive(I2C_ADDR_DISP, 1, this);
			m_next_poll_due = POLL_PERIOD;
		}
	}
};



#endif /* SOURCES_HAL_KEYBOARD_H_ */
