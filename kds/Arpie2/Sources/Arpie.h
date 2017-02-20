/*
 * Arpeggiator.h
 *
 *  Created on: 2 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_ARPIE_H_
#define SOURCES_ARPIE_H_

#include "hal/MidiInput.h"
#include "hal/MidiOutput.h"
#include "hal/IndicatorLed.h"
#include "hal/I2CDriver.h"
//#include "hal/Keyboard.h"
#include "arp/ChordMakerMidi.h"
#include "arp/OctaveExtender.h"
#include "arp/NoteOutputMidi.h"
#include "arp/SequencerBasic.h"
#include "arp/ClockInternal.h"
#include "arp/MetronomeBasic.h"
#include "arp/MetronomeBeat.h"

#include "hal/Display.h"

class CArpie : public IArpEvents
{
	enum {
		TX_LED_PERIOD = 10,
		RX_LED_PERIOD = 10,
		BEAT_LED_PERIOD = 10
	};
	CI2CDriver m_i2c;
	CMidiInput m_midi_input;
	CMidiOutput m_midi_output;
/*	CChordMakerMidi m_chord_maker;
	COctaveExtender m_octave_extender;
	CNoteOutputMidi m_note_output_midi;
	CSequencerBasic m_sequencer;
	CClockInternal m_internal_clock;
	CMetronomeBasic m_metronome;
	CMetronomeBeat m_beat_count;*/
	CIndicatorLed m_tx_led;
	CIndicatorLed m_rx_led;
	CIndicatorLed m_beat_led;
	//CKeyboard m_keyboard;
	CDisplay m_disp;

	uint32_t m_ms;
	byte m_tick;
	int m_tx_led_timeout;
	int m_rx_led_timeout;
	int m_beat_led_timeout;
public:
	CArpie() :
		m_tx_led(GPIO1_TX_LED_MASK, TX_LED_PERIOD),
		m_rx_led(GPIO1_RX_LED_MASK, RX_LED_PERIOD),
		m_beat_led(GPIO1_BEAT_LED_MASK, BEAT_LED_PERIOD)
	{
		m_ms = 0;
		m_tick = 0;
		m_tx_led_timeout = 0;
		m_rx_led_timeout = 0;
		m_beat_led_timeout = 0;
	}

	void init() {

		GPIO1_SetPortOutputDirection(GPIO1_DeviceData, GPIO1_BEAT_LED_MASK, 0);

/*		m_midi_input.m_listener = &m_chord_maker;
		m_chord_maker.m_consumer = &m_sequencer;

		m_internal_clock.m_listener_1 = &m_beat_count;
		m_internal_clock.m_listener_2 = &m_metronome;

		m_metronome.m_sequencer = &m_sequencer;
		m_sequencer.m_consumer = &m_note_output_midi;
		m_note_output_midi.m_midi_out = &m_midi_output;
*/
//		m_keyboard.m_i2c = &m_i2c;

		m_midi_input.listen();
		m_i2c.init_read(I2C_ADDR_DISP); // and kick off a new poll

	//	m_internal_clock.start();

	}

	//////////////////////////////////////////////////////////////////////
	// subrouting to manage polling the keyboard
	void handle_key_input() {
		if(m_i2c.is_response_pending()) {
			byte key = m_i2c.get_response();
			m_i2c.init_read(I2C_ADDR_DISP); // and kick off a new poll
			m_disp.show_number(key,&m_i2c);
		}
	}


	void run() {
		m_midi_input.run();
		m_midi_output.run();
		//m_internal_clock.run(m_ms);
		if(m_tick) {
			//m_keyboard.tick();
			//m_note_output_midi.tick();
			m_tx_led.tick();
			m_rx_led.tick();
			m_beat_led.tick();
			m_tick = 0;
		}
		handle_key_input();


	}
	void on_tick() {
		m_ms++;
		m_tick = 1;
		if(!(m_ms & 0x1FF)) {
			static int n=0;
			m_beat_led.trigger();
//			m_disp.show_number(n,&m_i2c);
			n++;
		}
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

	void on_uart0_rx_complete(byte status) {
		m_midi_input.on_rx_complete(status);
	}
	void on_i2c_read_complete(byte is_ok) {
		m_i2c.on_read_complete(is_ok);
	}
	void on_i2c_write_complete(byte is_ok) {
		m_i2c.on_write_complete(is_ok);
	}

};


#endif /* SOURCES_ARPIE_H_ */
