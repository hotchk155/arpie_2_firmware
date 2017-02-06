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
#include "ChordMakerMidi.h"
#include "OctaveExtender.h"
#include "NoteOutputMidi.h"
#include "SequencerBasic.h"
#include "ClockInternal.h"
#include "MetronomeBasic.h"
#include "MetronomeBeat.h"
#include "IndicatorLed.h"

class CArpeggiator : public IArpEvents
{
	enum {
		TX_LED_PERIOD = 10,
		RX_LED_PERIOD = 10,
		BEAT_LED_PERIOD = 10
	};
	CMidiInput m_midi_input;
	CMidiOutput m_midi_output;
	CChordMakerMidi m_chord_maker;
	COctaveExtender m_octave_extender;
	CNoteOutputMidi m_note_output_midi;
	CSequencerBasic m_sequencer;
	CClockInternal m_internal_clock;
	CMetronomeBasic m_metronome;
	CMetronomeBeat m_beat_count;
	CIndicatorLed m_tx_led;
	CIndicatorLed m_rx_led;
	CIndicatorLed m_beat_led;

	uint32_t m_ms;
	int m_tx_led_timeout;
	int m_rx_led_timeout;
	int m_beat_led_timeout;
public:
	CArpeggiator() :
		m_tx_led(GPIO1_TX_LED_MASK, TX_LED_PERIOD),
		m_rx_led(GPIO1_RX_LED_MASK, RX_LED_PERIOD),
		m_beat_led(GPIO1_BEAT_LED_MASK, BEAT_LED_PERIOD)
	{
		m_ms = 0;
		m_tx_led_timeout = 0;
		m_rx_led_timeout = 0;
		m_beat_led_timeout = 0;
	}

	void init() {

		GPIO1_SetPortOutputDirection(GPIO1_DeviceData, GPIO1_BEAT_LED_MASK, 0);

		m_midi_input.m_listener = &m_chord_maker;
		m_chord_maker.m_consumer = &m_sequencer;

		m_internal_clock.m_listener_1 = &m_beat_count;
		m_internal_clock.m_listener_2 = &m_metronome;

		m_metronome.m_sequencer = &m_sequencer;
		m_sequencer.m_consumer = &m_note_output_midi;
		m_note_output_midi.m_midi_out = &m_midi_output;

		m_midi_input.listen();
		m_internal_clock.start();

	}

	void run() {
		m_midi_input.run();
		m_midi_output.run();
		m_internal_clock.run(m_ms);
	}
	void on_tick() {
		m_ms++;
		m_note_output_midi.tick();
		m_tx_led.tick();
		m_rx_led.tick();
		m_beat_led.tick();
	}

	void on_event(int type, int param) {
		switch(type) {
		case BEAT:
			m_beat_led.trigger();
			break;
		case TX_DATA:
			m_tx_led.trigger();
			break;
		case RX_DATA:
			m_rx_led.trigger();
			break;
		}
	}

	inline void on_uart0_rx_complete(byte status) {
		m_midi_input.on_rx_complete(status);
	}
};


#endif /* SOURCES_ARPEGGIATOR_H_ */
