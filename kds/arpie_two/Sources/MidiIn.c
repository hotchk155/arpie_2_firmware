#include "Arpie.h"

#define FIFO_SIZE 32

// Single character buffer into which a character from
// the serial port is stored
static volatile byte s_char;

// cyclic buffer used to store info received from serial
static volatile byte s_fifo[FIFO_SIZE];
static volatile byte s_head;
static volatile byte s_tail;

// state information regarding MIDI data stream
//static byte s_running_status;
static byte s_in_sysex;
static byte s_num_params;
static byte s_param_index;
//static byte s_params[2];

static MIDI_MSG s_msg;


////////////////////////////////////////////////////////////
// call to start the listening process
static void midi_in_listen() {
	LDD_TError e = UART0_ReceiveBlock(UART0_DeviceData, (void*)&s_char, 1);
}

////////////////////////////////////////////////////////////
// invoked by ISR when receive call ends
// TODO error handling
void midi_in_on_rx_complete(byte status) {
	byte next = s_head + 1;
	if(next >= FIFO_SIZE)
		next = 0;
	if(next != s_tail) {
		s_fifo[s_head] = s_char;
		s_head = next;
	}
	midi_in_listen();
}


////////////////////////////////////////////////////////////
// This method is called by the arp core to service the
// MIDI input and pass received information out to the
// listeners
MIDI_MSG *midi_in_msg() {

	// check that the buffer is not empty
	if(s_head != s_tail) {

		// get next received byte
		byte ch = s_fifo[s_tail];
		if(++s_tail >= FIFO_SIZE) {
			s_tail = 0;
		}

		// REALTIME MESSAGE
		if((ch & 0xf0) == 0xf0)  {
			// START OF SYSEX
			if(ch == MIDI_SC_SYSEX_BEGIN) {
				s_in_sysex = 1;
			}
			// END OF SYSEX
			else if(ch == MIDI_SC_SYSEX_END) {
				s_in_sysex = 0;
			}
			else {
				s_msg.status = 0;
				s_msg.param1 = ch;
				s_msg.type = MIDI_MSG_REALTIME;
				return &s_msg;
			}
		}
		// STATUS MESSAGE
		else if(ch & 0x80) 	{

			s_in_sysex = 0;

			// handle a status message
			s_param_index = 0;
			s_msg.status = ch;
			switch(ch & 0xF0)
			{
			//TODO validations
			case MIDI_CH_POLY_PRESS: //  Aftertouch  1  key  touch
			case MIDI_CH_PROG_CHANGE: //  Patch change  1  instrument #
			case MIDI_CH_CHAN_PRESS: //  Channel Pressure  1  pressure
				s_num_params = 1;
				break;
			case MIDI_CH_NOTE_OFF: //  Note-off  2  key  velocity
			case MIDI_CH_NOTE_ON: //  Note-on  2  key  veolcity
			case MIDI_CH_CTRL_CHANGE: //  Continuous controller  2  controller #  controller value
			case MIDI_CH_PITCH_BEND: //  Pitch bend  2  lsb (7 bits)  msb (7 bits)
			default:
				s_num_params = 2;
				break;
			}
		}
		else if(s_in_sysex) {
			s_msg.status = 0;
			s_msg.param1 = ch;
			s_msg.type = MIDI_MSG_SYSEXBYTE;
			return &s_msg;
		}
		else if(s_msg.status)
		{
			// gathering parameters
			if(s_param_index == 0) {
				s_msg.param1 = ch;
			}
			else if(s_param_index == 1) {
				s_msg.param2 = ch;
			}
			if(++s_param_index >= s_num_params) {
				s_param_index = 0;
				return &s_msg;
			}
		}
	}
	return NULL;
}


void midi_in_init() {

	s_head = 0;
	s_tail = 0;
	s_char = 0;
	s_num_params = 0;
	s_param_index = 0;
	s_in_sysex = 0;
	midi_in_listen();
}

