
#ifndef SOURCES_INCLUDE_ARPNOTES_H_
#define SOURCES_INCLUDE_ARPNOTES_H_

#include <Stdlib.h>

typedef uint16_t PITCH;

#define MIDI_2_PITCH(midi) (((uint16_t)(midi))<<8)
#define PITCH_2_MIDI(pitch) ((byte)((pitch)>>8))

typedef struct {
	enum {
		MAX_PITCHES = 10
	};
	PITCH m_pitch[MAX_PITCHES];
	byte m_num_pitches;
	byte m_accent:1;
	byte m_tie:1;
	byte m_ticks;	// 24PPQN ticks duration
} ARP_NOTE;

class CArpNotes {
public:
	enum {
		MAX_PITCHES = 100,
		MAX_NOTES = 100,
		OCTAVE_PITCH = (((PITCH)12)<<8),
		MIN_PITCH = (((PITCH)0)<<8),
		MAX_PITCH = (((PITCH)127)<<8)
	};
	typedef struct {
		byte index;
		byte count;
	} NOTE;
	typedef struct {
		PITCH pitch[MAX_PITCHES];
		NOTE note[MAX_NOTES];
		byte num_pitches;
		byte num_notes;
	} CONFIG;
	CONFIG m_cfg;

	///////////////////////////////////////////////////////////////
	CArpNotes() {
		init();
	}

	///////////////////////////////////////////////////////////////
	void init() {
		m_cfg.num_pitches = 0;
		m_cfg.num_notes = 0;
	}

	///////////////////////////////////////////////////////////////
	inline int len() {
		return m_cfg.num_notes;
	}

	///////////////////////////////////////////////////////////////
	inline PITCH pitch(int i) {
		return m_cfg.pitch[m_cfg.note[i].index];
	}

	///////////////////////////////////////////////////////////////
	inline byte midi_note(int i) {
		return PITCH_2_MIDI(m_cfg.pitch[m_cfg.note[i].index]);
	}

	///////////////////////////////////////////////////////////////
	inline void insert_pitch(PITCH p) {
		if(m_cfg.num_pitches < MAX_PITCHES - 1 && m_cfg.num_notes < MAX_NOTES - 1) {
			m_cfg.note[m_cfg.num_notes].index = m_cfg.num_pitches;
			m_cfg.note[m_cfg.num_notes].count = 1;
			m_cfg.pitch[m_cfg.num_pitches] = p;
			++m_cfg.num_pitches;
			++m_cfg.num_notes;
		}
	}

	///////////////////////////////////////////////////////////////
	inline void insert_midi_note(byte n) {
		insert_pitch(MIDI_2_PITCH(n));
	}

	///////////////////////////////////////////////////////////////
	void get_note(int i, int transpose, ARP_NOTE *result) {
		memset(result, 0, sizeof(ARP_NOTE));
		if(i>=0 && i < m_cfg.num_notes) {
			int index = m_cfg.note[i].index;
			int count = m_cfg.note[i].count;
			int j;
			for(j = 0; j<count && result->m_num_pitches < ARP_NOTE::MAX_PITCHES - 1; ++j) {
				PITCH pitch = m_cfg.pitch[index++] + transpose;
				if(pitch >= MIN_PITCH && pitch <= MAX_PITCH) {
					result->m_pitch[result->m_num_pitches++] = pitch;
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////
	// sorts notes
	void sort(int sort_up = 1) {
		int sorted = 0;
		while(!sorted) {
			sorted = 1;
			for(int i=0; i<m_cfg.num_notes-1; ++i) {
				if(
					(sort_up && m_cfg.pitch[m_cfg.note[i].index] > m_cfg.pitch[m_cfg.note[i+1].index]) ||
					(!sort_up && m_cfg.pitch[m_cfg.note[i].index] < m_cfg.pitch[m_cfg.note[i+1].index])
				) {
					sorted = 0;
					NOTE t = m_cfg.note[i];
					m_cfg.note[i] = m_cfg.note[i+1];
					m_cfg.note[i+1] = t;
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////
	// shuffles the notes into random order
	void shuffle() {
		for(int i=m_cfg.num_notes-1; i<0; --i) {
			int j = rand()%(i+1);
			NOTE t = m_cfg.note[i];
			m_cfg.note[i] = m_cfg.note[j];
			m_cfg.note[j] = t;
		}
	}

	///////////////////////////////////////////////////////////////
	// extends a sequence over multiple octaves
	void extend(byte octaves) {

		int num_notes = m_cfg.num_notes; // original number of notes in chord
		int transpose = OCTAVE_PITCH;

		for(int o=1; o<octaves; ++o) { // loop over octaves
			for(int i=0; i<num_notes; ++i) { // loop over notes in original chord
				if(m_cfg.num_notes >= MAX_NOTES - 1 || m_cfg.num_pitches >= MAX_PITCHES - 1) {
					return; // no more space!
				}

				int index = m_cfg.note[i].index;
				int count = m_cfg.note[i].count; // the source note and its number of pitches

				m_cfg.note[m_cfg.num_notes].index = m_cfg.num_pitches; // first pitch location
				int j;
				for(j = 0; j<count && m_cfg.num_pitches < MAX_PITCHES - 1; ++j) { // copy over pitches until no space

					PITCH pitch = m_cfg.pitch[index++] + transpose;
					if(pitch <= MAX_PITCH) {
						m_cfg.pitch[m_cfg.num_pitches++] = pitch;
					}
				}
				if(j > 0) {
					m_cfg.note[m_cfg.num_notes++].count = j;
				}
			}
			transpose += OCTAVE_PITCH;
		}
	}

	///////////////////////////////////////////////////////////////
	// join a reversed version of a sequence on to the end of itself
	// excluding the first and last notes (to avoid repetition)
	void reflect() {
		int num_notes = m_cfg.num_notes;
		for(int i=num_notes-2; i>0 && m_cfg.num_notes < MAX_NOTES-1; --i) {
			m_cfg.note[m_cfg.num_notes++] = m_cfg.note[i];
		}
	}

	///////////////////////////////////////////////////////////////
	// Inserts a note between every note in source sequence. The
	// inserted note must already exist in the pitch list for the
	// sequence!
	void insert_note(NOTE note_insert, int allow_dup = 1) {
		NOTE t[MAX_NOTES];
		int pos = 0;
		for(int i=0; i<m_cfg.num_notes; ++i) {
			if(pos >= MAX_NOTES - 1) {
				break;
			}
			t[pos++] = m_cfg.note[i];
			if((note_insert.index != m_cfg.note[i].index) || allow_dup) {
				if(pos >= MAX_NOTES - 1) {
					break;
				}
				t[pos++] = note_insert;
			}
		}
		memcpy(m_cfg.note, t, sizeof m_cfg.note);
		m_cfg.num_notes = pos;
	}

	///////////////////////////////////////////////////////////////
	// Gets the highest note in the list
	NOTE highest() {
		NOTE t = {0};
		for(byte i=0; i<m_cfg.num_notes; ++i) {
			if(!t.count || ( m_cfg.pitch[m_cfg.note[i].index] > m_cfg.pitch[t.index] ) ) {
				t = m_cfg.note[i];
			}
		}
		return t;
	}

	///////////////////////////////////////////////////////////////
	// Gets the lowest note in the list
	NOTE lowest() {
		NOTE t = {0};
		for(byte i=0; i<m_cfg.num_notes; ++i) {
			if(!t.count || ( m_cfg.pitch[m_cfg.note[i].index] < m_cfg.pitch[t.index] ) ) {
				t = m_cfg.note[i];
			}
		}
		return t;
	}

	///////////////////////////////////////////////////////////////
	// Make a copy of a sequence extended in an X fwd, Y back
	// step pattern
	void mutate(int fwd, int back)	{
		NOTE t[MAX_NOTES];
		int src_pos = 0;
		int dest_pos = 0;
		int count = fwd;
		int dir = 1;
		for(int i=0; i<m_cfg.num_notes && dest_pos < MAX_NOTES; ++i) {
			t[dest_pos++] = m_cfg.note[src_pos];
			if(count <= 0) {
				dir = !dir;
				count = dir? fwd : back;
			}
			if(dir) {
				if(src_pos < m_cfg.num_notes - 1)
					++src_pos;
			}
			else {
				if(src_pos > 0)
					--src_pos;
			}
			--count;
		}
		memcpy(m_cfg.note, t, sizeof m_cfg.note);
		m_cfg.num_notes = dest_pos;
	}


};





#endif /* SOURCES_INCLUDE_ARPNOTES_H_ */
