/*
 * MidiIn.h
 *
 *  Created on: 20 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_MIDIIN_H_
#define SOURCES_MIDIIN_H_

class CMidiIn {

	enum {
		FIFO_SIZE = 32
	};

// Single character buffer into which a character from
// the serial port is stored
	volatile byte m_char;

// cyclic buffer used to store info received from serial
	volatile byte m_fifo[FIFO_SIZE];
	volatile byte m_head;
	volatile byte m_tail;

// state information regarding MIDI data stream
//static byte s_running_status;
	byte m_in_sysex:1;
	byte m_param_index:3;

	midi::MSG m_msg;


public:
	////////////////////////////////////////////////////////////
	CMidiIn() {
		m_head = 0;
		m_tail = 0;
		m_char = 0;
		m_param_index = 0;
		m_in_sysex = 0;
	}

	////////////////////////////////////////////////////////////
	void listen() {
		LDD_TError e = UART0_ReceiveBlock(UART0_DeviceData, (void*)&m_char, 1);
	}

	////////////////////////////////////////////////////////////
	void on_rx_complete() {
		byte next = m_head + 1;
		if(next >= FIFO_SIZE)
			next = 0;
		if(next != m_tail) {
			m_fifo[m_head] = m_char;
			m_head = next;
		}
		listen();
	}

	////////////////////////////////////////////////////////////
	void on_rx_error() {

	}

	////////////////////////////////////////////////////////////
	midi::MSG *msg() {

		// check that the buffer is not empty
		if(m_head != m_tail) {

			CLeds::instance().pulse_rx();

			// get next received byte
			byte ch = m_fifo[m_tail];
			if(++m_tail >= FIFO_SIZE) {
				m_tail = 0;
			}

			// REALTIME MESSAGE
			if((ch & 0xf0) == 0xf0)  {
				// START OF SYSEX
				if(ch == midi::SC_SYSEX_BEGIN) {
					m_in_sysex = 1;
					m_msg.status = 0;
					m_msg.param1 = ch;
					m_msg.type = midi::MSG_SYSEXBEGIN;
					return &m_msg;
				}
				// END OF SYSEX
				else if(ch == midi::SC_SYSEX_END) {
					m_in_sysex = 0;
					m_msg.status = 0;
					m_msg.param1 = ch;
					m_msg.type = midi::MSG_SYSEXEND;
					return &m_msg;
				}
				else {
					m_msg.status = 0;
					m_msg.param1 = ch;
					m_msg.type = midi::MSG_REALTIME;
					return &m_msg;
				}
			}
			// STATUS MESSAGE
			else if(ch & 0x80) 	{

				m_in_sysex = 0;

				// handle a status message
				m_param_index = 0;
				m_msg.status = ch;
				switch(ch & 0xF0)
				{
				//TODO validations
				case midi::CH_POLY_PRESS: //  Aftertouch  1  key  touch
				case midi::CH_PROG_CHANGE: //  Patch change  1  instrument #
				case midi::CH_CHAN_PRESS: //  Channel Pressure  1  pressure
					m_msg.type = midi::MSG_1PARAM;
					break;
				case midi::CH_NOTE_OFF: //  Note-off  2  key  velocity
				case midi::CH_NOTE_ON: //  Note-on  2  key  veolcity
				case midi::CH_CTRL_CHANGE: //  Continuous controller  2  controller #  controller value
				case midi::CH_PITCH_BEND: //  Pitch bend  2  lsb (7 bits)  msb (7 bits)
				default:
					m_msg.type = midi::MSG_2PARAM;
					break;
				}
			}
			else if(m_in_sysex) {
				m_msg.status = 0;
				m_msg.param1 = ch;
				m_msg.type = midi::MSG_SYSEXBYTE;
				return &m_msg;
			}
			else if(m_msg.status)
			{
				// gathering parameters
				if(m_param_index == 0) {
					m_msg.param1 = ch;
				}
				else if(m_param_index == 1) {
					m_msg.param2 = ch;
				}
				++m_param_index;
				if(	(m_msg.type == midi::MSG_1PARAM && m_param_index >= 1) ||
					(m_msg.type == midi::MSG_2PARAM && m_param_index >= 2)) {
					m_param_index = 0;
					return &m_msg;
				}
			}
		}
		return NULL;
	}
};

#endif /* SOURCES_MIDIIN_H_ */
