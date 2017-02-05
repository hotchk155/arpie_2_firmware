/*
 * Arpeggiator.h
 *
 *  Created on: 2 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_ARPEGGIATOR_H_
#define SOURCES_ARPEGGIATOR_H_

#include "MidiInput.h"
#include "MidiOutput.h"

class CArpeggiator
{
	CMidiInput m_midi_input;
	CMidiOutput m_midi_output;

public:
	CArpeggiator() {
	}

	void init() {
		m_midi_input.listen();
		m_midi_input.m_listener = &m_midi_output;
	}

	void run() {
		m_midi_input.run();
		m_midi_output.run();
	}
	void tick() {

	}
	inline void on_uart0_rx_complete(byte status) {
		m_midi_input.on_rx_complete(status);
	}
};


#endif /* SOURCES_ARPEGGIATOR_H_ */
