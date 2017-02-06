/*
 * MIDI INPUT CLASS
 */
#ifndef SOURCES_MIDIINPUT_H_
#define SOURCES_MIDIINPUT_H_

#include "Midi.h"
#include "Definitions.h"

class CMidiInput {
	enum {
		FIFO_SIZE = 32,
	};

	// Single character buffer into which a character from
	// the serial port is stored
	byte m_char;

	// cyclic buffer used to store info received from serial
	byte m_fifo[FIFO_SIZE];
	byte m_head;
	byte m_tail;

	// state information regarding MIDI data stream
	byte m_running_status;
	byte m_in_sysex;
	byte m_num_params;
	byte m_param_index;
	byte m_params[2];


	////////////////////////////////////////////////////////////////////
	inline void fire_on_realtime(byte ch) {
		if(m_listener)
			m_listener->on_midi_realtime(ch);
	}

	////////////////////////////////////////////////////////////////////
	inline void fire_on_sysex(byte ch) {
		if(m_listener)
			m_listener->on_midi_sysex(ch);
	}

	////////////////////////////////////////////////////////////////////
	inline void fire_on_msg() {
		switch(m_num_params) {
		case 0:
			if(m_listener)
				m_listener->on_midi_msg(m_running_status,0,0,0);
			break;
		case 1:
			if(m_listener)
				m_listener->on_midi_msg(m_running_status,m_params[0],0,1);
			break;
		case 2:
			if(m_listener)
				m_listener->on_midi_msg(m_running_status,m_params[0],m_params[1],2);
			break;
		}
	}


public:

	////////////////////////////////////////////////////////////
	IMidiMessageListener *m_listener;

	CMidiInput() {
		m_head = 0;
		m_tail = 0;
		m_char = 0;
		m_running_status = 0;
		m_num_params = 0;
		m_param_index = 0;
		m_in_sysex = 0;
		m_listener = nullptr;

	}
	////////////////////////////////////////////////////////////
	// call to start the listening process
	void listen() {
		LDD_TError e = UART0_ReceiveBlock(UART0_DeviceData, &m_char, 1);
	}

	////////////////////////////////////////////////////////////
	// invoked by ISR when receive call ends
	// TODO error handling
	void on_rx_complete(byte status) {
		byte next = m_head + 1;
		if(next >= FIFO_SIZE)
			next = 0;
		if(next != m_tail) {
			m_fifo[m_head] = m_char;
			m_head = next;
		}
		g_arp->on_event(IArpEvents::RX_DATA,0);
		listen();
	}

	////////////////////////////////////////////////////////////
	// This method is called by the arp core to service the
	// MIDI input and pass received information out to the
	// listeners
	void run() {

		// check that the buffer is not empty
		if(m_head != m_tail) {

			// get next received byte
			byte ch = m_fifo[m_tail];
			if(++m_tail >= FIFO_SIZE) {
				m_tail = 0;
			}

			// REALTIME MESSAGE
			if((ch & 0xf0) == 0xf0)  {
				// START OF SYSEX
				if(ch == midi::SC_SYSEX_BEGIN) {
					m_running_status = 0;
					m_in_sysex = 1;
					fire_on_sysex(ch);
				}
				// END OF SYSEX
				else if(ch == midi::SC_SYSEX_END) {
					fire_on_sysex(ch);
					m_in_sysex = 0;
				}
				else {
					fire_on_realtime(ch);
				}
			}
			// STATUS MESSAGE
			else if(ch & 0x80) 	{

				// sysex ending prematurely
				if(m_in_sysex) {
					fire_on_sysex(midi::SC_SYSEX_END);
					m_in_sysex = 0;
				}

				// handle a status message
				m_param_index = 0;
				m_running_status = ch;
				switch(ch & 0xF0)
				{
				//TODO validations
				case midi::CH_POLY_PRESS: //  Aftertouch  1  key  touch
				case midi::CH_PROG_CHANGE: //  Patch change  1  instrument #
				case midi::CH_CHAN_PRESS: //  Channel Pressure  1  pressure
					m_num_params = 1;
					break;
				case midi::CH_NOTE_OFF: //  Note-off  2  key  velocity
				case midi::CH_NOTE_ON: //  Note-on  2  key  veolcity
				case midi::CH_CTRL_CHANGE: //  Continuous controller  2  controller #  controller value
				case midi::CH_PITCH_BEND: //  Pitch bend  2  lsb (7 bits)  msb (7 bits)
				default:
					m_num_params = 2;
					break;
				}
			}
			else if(m_in_sysex) {
				fire_on_sysex(ch);
			}
			else if(m_running_status)
			{
				// gathering parameters
				m_params[m_param_index++] = ch;
				if(m_param_index >= m_num_params) {
					fire_on_msg();
					m_param_index = 0;
				}
			}
		}
	}
};


#endif /* SOURCES_MIDIINPUT_H_ */
