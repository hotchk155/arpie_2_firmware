/*
 * ClockMidi.h
 *
 *  Created on: 4 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_CLOCKMIDI_H_
#define SOURCES_CLOCKMIDI_H_


class ClockMidi : public IMidiClockConsumer {
	byte m_running;
public:
	IMetronome *m_metronome;
	ClockMidi() {
		m_running = 0;
	}
	void on_midi_tick() {
		if(m_running && m_metronome) {
			m_metronome->on_tick();
		}
	}
	void on_midi_start() {
		if(m_metronome) {
			m_metronome->on_tick();
		}
	}
	void on_midi_stop() {
		m_running = 0;
	}
	void on_midi_continue() {
		m_running = 1;
	}
};


#endif /* SOURCES_CLOCKMIDI_H_ */
