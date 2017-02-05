/*
 * Chord.h
 *
 *  Created on: 2 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_CHORDMAKERMIDI_H_
#define SOURCES_CHORDMAKERMIDI_H_


/////////////////////////////////////////////////////////////////////////////
// Class to manage chord information based on notes incoming through
// MIDI input
/////////////////////////////////////////////////////////////////////////////

#include"ChordMaker.h"

#define MAX_CHORD_SIZE	12	// maximum number of notes we can have in a chord



class CChordMakerMIDI :
		public CChordMaker,
		public IMidiMessageProcessor
{

	byte m_hold;
	byte m_held_notes;
	byte m_chan;

	void remove_note(int index) {
		delete m_notes[index];
		for(int j = i; j<m_len-1; ++j) {
			m_notes[j] = m_notes[j+1];
		}
		--m_len;
	}

	typedef struct {
		byte note;
		byte velocity;
		byte is_held;
	} MIDI_NOTE;

	MIDI_NOTE m_notes[MAX_LEN];
	int m_len;

public:
	enum {
		MAX_LEN = 12
	};

	CChordMakerMIDI() {
		m_hold = 0;
		m_len = 0;
		m_held_notes = 0;
	}


	///////////////////////////////////////////////////////
	// IMidiMessageProcessor::on_midi_msg
	void on_midi_msg(byte status, byte param1, byte param2) {
		switch(status & 0xF0) {
		case midi::CH_NOTE_ON:
			if((m_chan != midi::OMNI) && (status & 0x0F) != m_chan) {
				break;
			}
			if(param2) {
				if(!m_held_notes) {
					m_len = 0;
				}
				for(int i=0; i<m_len; ++i) {
					if(m_notes[i].note == note) {
						m_notes[i].vel = velocity;
						if(!m_notes[i].is_held) {
							m_notes[i].is_held = 1;
							++m_held_notes;
							break;
						}
					}
				}
				if(i==m_len && m_len < MAX_LEN) {
					++m_len;
					m_notes[m_len].note = note;
					m_notes[m_len].vel = vel;
					m_notes[m_len].is_held = 1;
					++m_held_notes;
				}
				set_changed();
				break;
			}
			// else fall through (zero note on velocity = note off)
		case midi::CH_NOTE_OFF:
			for(int i=0; i<m_len; ++i) {
				if(m_notes[i].note == note) {
					if(m_hold) {
						if(m_notes[i].is_held) {
							m_notes[i].is_held = 0;
							--m_held_notes;
						}
					}
					else {
						remove_note(i);
						--m_held_notes;
						--i;
						set_changed();
					}
				}
			}
			break;
		}
	}

	///////////////////////////////////////////////////////
	// IMidiMessageProcessor::on_midi_sysex
	void on_midi_sysex(byte data) {
	}

	///////////////////////////////////////////////////////
	// IMidiMessageProcessor::on_midi_realtime
	void on_midi_realtime(byte data) {
	}

	///////////////////////////////////////////////////////
	void set_hold(byte hold) {
		// if hold is being turned off we need to cancel any
		// notes that are still in the chord but which are
		// not held in MIDI
		int do_rebuild = 0;
		if(m_hold && !hold) {
			for(int i=0; i<m_len;) {
				if(!m_notes[i].is_held) {
					remove_note(i);
					set_changed();
				}
				else {
					++i;
				}
			}

		}
		m_hold = hold;
	}

	///////////////////////////////////////////////////////
	inline byte get_hold() {
		return m_hold;
	}

	///////////////////////////////////////////////////////
	CArpNotes *get_chord() {
		CArpNotes *chord = new CArpNotes();
		for(int i=0; i<m_len; ++i) {
			chord->add(new CArpNote(m_notes[i].note, m_notes[i].vel));
		}
		return chord;
	}
};

#endif /* SOURCES_CHORDMAKERMIDI_H_ */
