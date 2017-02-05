/*
 * NoteOutput.h
 *
 *  Created on: 3 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_NOTEOUTPUT_H_
#define SOURCES_NOTEOUTPUT_H_

class CNoteOutput {
	byte m_midi_chan;
	CArpNote *m_note
	int m_timeout;
public:
	enum {
		TIE_GATE = -1,
		MANUAL_VELOCITY = -1
	};

	CNoteOutput() {
		m_midi_chan = 0;
		m_note = nullptr;
		m_timeout = 0;
	}

	//////////////////////////////////////////////////////////////////////
	// Stop all playing notes
	void stop() {
		if(m_note) {
			for(int i=0; i<m_note->m_count; ++i) {
				midi_stop_note(m_note->m_notes[i]);
			}
			delete m_note;
			m_note = nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////
	// Start a new note playing
	void play(CArpNote *note, int gate_len, int velocity) {
		for(int i=0; i<note->m_count; ++i) {
			midi_start_note(note->m_notes[i], velocity);
		}
		if(m_note) {
			stop();
		}
		duration_count = gate_len;
	}

	//////////////////////////////////////////////////////////////////////
	// Called once per millisecond to time note durations
	void tick() {
		if(duration_count > 0) {
			if(--duration_count == 0) {
				stop();
			}
		}
	}

	//////////////////////////////////////////////////////////////////////
	// Midi channel accessor
	void set_midi_chan(byte chan) {
		stop(); // kill any playing notes before switching channel
		m_midi_chan = chan&0x0F;
	}
	byte get_midi_chan() {
		return m_midi_chan;
	}

};



#endif /* SOURCES_NOTEOUTPUT_H_ */
