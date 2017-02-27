/*
 * ChordCatcherMidi.h
 *
 *  Created on: 24 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_CHORDCATCHERMIDI_H_
#define SOURCES_CHORDCATCHERMIDI_H_


class CChordCatcherMidi
{
public:
	enum {
		MAX_LEN = 12
	};
	typedef struct {
		byte chan;
	} CONFIG;
protected:

	typedef struct {
		byte note;
		byte velocity:7;
		byte is_held:1;
	} MIDI_NOTE;

	CONFIG m_cfg;
	byte m_len;
	byte m_held_notes;
	byte m_is_changed:1;
	byte m_hold:1;
	MIDI_NOTE m_notes[MAX_LEN];

	void remove_note(int index) {
		for(int j = index; j<m_len-1; ++j) {
			m_notes[j] = m_notes[j+1];
		}
		--m_len;
	}

public:
	///////////////////////////////////////////////////////
	CChordCatcherMidi() {
		m_cfg.chan = 0;
	}

	///////////////////////////////////////////////////////
	void init() {
		m_len = 0;
		m_held_notes = 0;
		m_hold = 0;
	}

	void test() {
		m_len = 3;
		m_notes[0].note = 60;
		m_notes[1].note = 62;
		m_notes[2].note = 64;
		m_is_changed = 1;
	}

	///////////////////////////////////////////////////////
	void set_config(CONFIG *cfg) {
		m_cfg = *cfg;
	}

	///////////////////////////////////////////////////////
	void get_config(CONFIG *cfg) {
		*cfg = m_cfg;
	}

	///////////////////////////////////////////////////////
	inline byte is_changed() {
		return m_is_changed;
	}

	///////////////////////////////////////////////////////
	void get_notes(CArpNotes *notes) {
		notes->init();
		for(byte i=0; i<m_len; ++i) {
			notes->insert_midi_note(m_notes[i].note);
		}
		m_is_changed = 0;
	}

	///////////////////////////////////////////////////////
	void on_midi_msg(midi::MSG *msg) {
		if(msg->type == midi::MSG_2PARAM) {
			switch(msg->status & 0xF0) {
			case midi::CH_NOTE_ON:
				if((m_cfg.chan != midi::OMNI) && (msg->status & 0x0F) != m_cfg.chan) {
					break;
				}
				if(msg->param2) {
					if(!m_held_notes) {
						m_len = 0;
					}
					int i;
					for(i=0; i<m_len; ++i) {
						if(m_notes[i].note == msg->param1) {
							m_notes[i].velocity = msg->param2;
							if(!m_notes[i].is_held) {
								m_notes[i].is_held = 1;
								++m_held_notes;
								break;
							}
						}
					}
					if(i==m_len && m_len < MAX_LEN) {
						m_notes[m_len].note = msg->param1;
						m_notes[m_len].velocity = msg->param2;
						m_notes[m_len].is_held = 1;
						++m_len;
						++m_held_notes;
					}
					m_is_changed = 1;
					break;
				}
				// else fall through (zero note on velocity = note off)
			case midi::CH_NOTE_OFF:
				for(int i=0; i<m_len; ++i) {
					if(m_notes[i].note == msg->param1) {
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
							m_is_changed = 1;
						}
					}
				}
				break;
			}
		}
	}

	///////////////////////////////////////////////////////
	void set_hold(byte hold) {
		// if hold is being turned off we need to cancel any
		// notes that are still in the chord but which are
		// not held in MIDI
		if(m_hold && !hold) {
			for(int i=0; i<m_len;) {
				if(!m_notes[i].is_held) {
					remove_note(i);
					m_is_changed = 1;
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

};



#endif /* SOURCES_CHORDCATCHERMIDI_H_ */
