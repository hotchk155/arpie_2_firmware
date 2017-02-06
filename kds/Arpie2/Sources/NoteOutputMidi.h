/*
 * NoteOutput.h
 *
 *  Created on: 3 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_NOTEOUTPUTMIDI_H_
#define SOURCES_NOTEOUTPUTMIDI_H_

#include "Definitions.h"

class CNoteOutputMidi : public IArpNoteConsumer {
public:
	enum {
		ORIGINAL_VELOCITY = -1
	};
protected:
	CArpNote *m_note;
	byte m_chan;
	int m_duration;
	int m_velocity;
public:
	IMidiMessageListener *m_midi_out;


	CNoteOutputMidi() {
		m_chan = 0;
		m_note = nullptr;
		m_duration = 0;
		m_midi_out = nullptr;
		m_velocity = ORIGINAL_VELOCITY;
	}

	//////////////////////////////////////////////////////////////////////
	// Stop all playing notes
	void note_stop() {
		if(!m_midi_out)
			return;
		if(m_note) {

			// stop the note playing.. we use convention of NOTE ON with zero
			// velocity to turn a note off (allow efficient use of running status)
			for(int i=0; i<m_note->count(); ++i) {
				m_midi_out->on_midi_msg(midi::CH_NOTE_ON | m_chan, m_note->midi_note(i), 0, 2);
			}
			delete m_note;
			m_note = nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////
	// Start a new note playing
	void note_start(CArpNote *note) {
		if(!m_midi_out)
			return;
		if(m_note && m_note->dur() != CArpNote::DUR_TIED) {
			note_stop();
		}
		for(int i=0; i<note->count(); ++i) {
			int vel = (m_velocity == ORIGINAL_VELOCITY) ? note->vel() : m_velocity;
			m_midi_out->on_midi_msg(midi::CH_NOTE_ON | m_chan, note->midi_note(i), vel, 2);
		}
		if(m_note) {
			note_stop();
		}
		m_note = note->clone();
		m_duration = m_note->dur();
	}

	//////////////////////////////////////////////////////////////////////
	// Called once per millisecond to time note durations
	void tick() {
		if(m_duration > 0) {
			if(--m_duration == 0) {
				note_stop();
			}
		}
	}

	//////////////////////////////////////////////////////////////////////
	// Midi channel accessor
	void set_chan(byte chan) {
		note_stop(); // kill any playing notes before switching channel
		m_chan = chan&0x0F;
	}
	byte get_chan() {
		return m_chan;
	}

};



#endif /* SOURCES_NOTEOUTPUTMIDI_H_ */
