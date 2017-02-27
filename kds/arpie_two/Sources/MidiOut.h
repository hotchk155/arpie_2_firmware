/*
 * MidiOut.h
 *
 *  Created on: 20 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_MIDIOUT_H_
#define SOURCES_MIDIOUT_H_


class CMidiOut {

	enum {
			TX_SIZE = 8,
			BUF_SIZE = 32,
			RUNNING_STATUS_TTL = 20
		} ;

	// this is the actual send buffer used with the UART peripheral. This can only
	// be modified when there is no send in progress.
	byte m_tx[TX_SIZE];

	// this is the buffer in which incoming bytes are stored ready to be sent
	byte m_buf[BUF_SIZE];
	byte m_head;
	byte m_tail;
	byte m_running_status;
	byte m_tx_complete:1;
	byte m_running_status_ttl;

	///////////////////////////////////////////////////////////////
	void init_send() {
		int tx_len = 0;
		while(m_tail != m_head && tx_len < TX_SIZE) {
			m_tx[tx_len++] = m_buf[m_tail++];
			if(m_tail >= BUF_SIZE) {
				m_tail = 0;
			}
		}
		if(tx_len > 0) {
			LDD_TError e = UART0_SendBlock(UART0_DeviceData, m_tx, tx_len);
			//TODO error check
		}
	}

	///////////////////////////////////////////////////////////////
	void queue_byte(byte ch) {
		byte next = m_head + 1;
		if(next >= BUF_SIZE) {
			next = 0;
		}
		if(next == m_tail) {
			//TODO report error
		}
		m_buf[m_head] = ch;
		m_head = next;
	}

public:

	///////////////////////////////////////////////////////////////
	CMidiOut() {
		m_head = 0;
		m_tail = 0;
		m_running_status = 0;
		m_tx_complete = 1;
	}

	///////////////////////////////////////////////////////////////
	void on_tx_complete() {
		m_tx_complete = 1;
	}

	///////////////////////////////////////////////////////////////
	void on_tx_error() {
	}

	///////////////////////////////////////////////////////////////
	void msg(midi::MSG *msg) {
		CLeds::instance().pulse_tx();

		switch(msg->type) {
		case midi::MSG_REALTIME:
			queue_byte(msg->status);
			break;
		case midi::MSG_1PARAM:
			if(m_running_status_ttl) --m_running_status_ttl;
			if(msg->status != m_running_status || !m_running_status_ttl) {
				queue_byte(msg->status);
				m_running_status = msg->status;
				m_running_status_ttl = RUNNING_STATUS_TTL;
			}
			queue_byte(msg->param1);
			break;
		case midi::MSG_2PARAM:
			if(m_running_status_ttl) --m_running_status_ttl;
			if(msg->status != m_running_status || !m_running_status_ttl) {
				queue_byte(msg->status);
				m_running_status = msg->status;
				m_running_status_ttl = RUNNING_STATUS_TTL;
			}
			queue_byte(msg->param1);
			queue_byte(msg->param2);
			break;
		default:
		case midi::MSG_SYSEXBEGIN:
		case midi::MSG_SYSEXEND:
		case midi::MSG_SYSEXBYTE:
			//TODO
			break;
		}
	}

	///////////////////////////////////////////////////////////////
	void run() {
		//if(UART0_GetTxCompleteStatus(UART0_DeviceData) && (s_tail != s_head)) {
		if(m_tx_complete && (m_tail != m_head)) {
			init_send();
		}
	}
};

#endif /* SOURCES_MIDIOUT_H_ */
