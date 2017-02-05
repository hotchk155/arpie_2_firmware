/*
 * MidiOutput.h
 *
 *  Created on: 4 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_MIDIOUTPUT_H_
#define SOURCES_MIDIOUTPUT_H_

class CMidiOutput :
		//public IArpNoteProcessor,
		public IMidiMessageListener
{
public:
	enum {
		TX_SIZE = 16,
		BUF_SIZE = 128
	} ;
protected:
	byte m_tx[TX_SIZE];

	byte m_buf[BUF_SIZE];
	byte m_head;
	byte m_tail;
	byte m_char;

	byte m_running_status;
	byte m_tx_busy;
	byte m_is_started;

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
			m_tx_busy = 1;
		}

	}

	///////////////////////////////////////////////////////////////
	inline void buffer_byte(byte ch) {
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
	CMidiOutput() {
		m_head = 0;
		m_tail = 0;
		m_char = 0;
		m_running_status = 0;
		m_tx_busy = 0;
		m_is_started = 0;
	}

	///////////////////////////////////////////////////////////////
	void on_midi_msg(byte status, byte param1, byte param2, byte num_params) {
		if(m_running_status != status) {
			buffer_byte(status);
			m_running_status = status;
		}
		if(num_params > 0) {
			buffer_byte(param1);
		}
		if(num_params > 1) {
			buffer_byte(param2);
		}
	}

	///////////////////////////////////////////////////////////////
	void on_midi_sysex(byte data) {
	}

	///////////////////////////////////////////////////////////////
	void on_midi_realtime(byte data) {
		buffer_byte(data);
	}

	///////////////////////////////////////////////////////////////
	void on_clock_reset() {
		m_is_started = 0;
	}

	///////////////////////////////////////////////////////////////
	void on_clock_start() {
		if(m_is_started) {
			buffer_byte(midi::RT_CLOCK_CONTINUE);
		}
		else {
			buffer_byte(midi::RT_CLOCK_START);
			m_is_started = 1;
		}
	}

	///////////////////////////////////////////////////////////////
	void on_clock_stop() {
		buffer_byte(midi::RT_CLOCK_STOP);
	}

	///////////////////////////////////////////////////////////////
	void on_clock_tick() {
		buffer_byte(midi::RT_CLOCK_TICK);
	}

	///////////////////////////////////////////////////////////////
	void run() {
		if(m_tx_busy) {
			if(UART0_GetTxCompleteStatus(UART0_DeviceData)) {
				m_tx_busy = 0;
			}
		}
		else {
			if(m_tail != m_head) {
				init_send();
			}
		}
	}
};



#endif /* SOURCES_MIDIOUTPUT_H_ */
