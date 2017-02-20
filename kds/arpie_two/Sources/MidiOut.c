#include "Arpie.h"


enum {
		TX_SIZE = 8,
		BUF_SIZE = 32
	} ;

// this is the actual send buffer used with the UART peripheral. This can only
// be modified when there is no send in progress.
byte s_tx[TX_SIZE];

// this is the buffer in which incoming bytes are stored ready to be sent
byte s_buf[BUF_SIZE];
byte s_head;
byte s_tail;
byte s_running_status;
byte s_tx_complete;

///////////////////////////////////////////////////////////////
static void midi_out_init_send() {
	int tx_len = 0;
	while(s_tail != s_head && tx_len < TX_SIZE) {
		s_tx[tx_len++] = s_buf[s_tail++];
		if(s_tail >= BUF_SIZE) {
			s_tail = 0;
		}
	}
	if(tx_len > 0) {
		LDD_TError e = UART0_SendBlock(UART0_DeviceData, s_tx, tx_len);
		//TODO error check
	}
}

///////////////////////////////////////////////////////////////
static void midi_out_byte(byte ch) {
	byte next = s_head + 1;
	if(next >= BUF_SIZE) {
		next = 0;
	}
	if(next == s_tail) {
		//TODO report error
	}
	s_buf[s_head] = ch;
	s_head = next;
}

///////////////////////////////////////////////////////////////
void midi_out_msg(MIDI_MSG *msg) {
	switch(msg->type) {
	case MIDI_MSG_REALTIME:
	case MIDI_MSG_0PARAM:
		midi_out_byte(msg->status);
		break;
	case MIDI_MSG_1PARAM:
		if(msg->status != s_running_status) {
			midi_out_byte(msg->status);
			s_running_status = msg->status;
		}
		midi_out_byte(msg->param1);
		break;
	case MIDI_MSG_2PARAM:
		if(msg->status != s_running_status) {
			midi_out_byte(msg->status);
			s_running_status = msg->status;
		}
		midi_out_byte(msg->param1);
		midi_out_byte(msg->param2);
		break;
	default:
	case MIDI_MSG_SYSEXBYTE:
		//TODO
		break;
	}
}

void midi_out_on_tx_complete(byte status) {
	s_tx_complete = 1;
}

///////////////////////////////////////////////////////////////
void midi_out_init() {
	s_head = 0;
	s_tail = 0;
	s_running_status = 0;
	s_tx_complete = 1;
}

///////////////////////////////////////////////////////////////
void midi_out_run() {
	//if(UART0_GetTxCompleteStatus(UART0_DeviceData) && (s_tail != s_head)) {
	if(s_tx_complete && (s_tail != s_head)) {
		midi_out_init_send();
	}
}

