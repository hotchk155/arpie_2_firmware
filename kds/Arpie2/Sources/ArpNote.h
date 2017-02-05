/*
 * Notes.h
 *
 *  Created on: 3 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_ARPNOTE_H_
#define SOURCES_ARPNOTE_H_

#include <stdlib.h>

///////////////////////////////////////////////////////////////
// Class to define a set of one or more notes that will
// be played at the same time by the sequencer
class CArpNote
{
	byte m_vel;
	NOTE_TYPE *m_notes;
	byte m_count;
public:
	enum {
		MIN_NOTE_VAL = 0,
		MAX_NOTE_VAL = 127*256,
		OCTAVE_NOTE_VAL = 12*256
	};
	typedef uint16_t NOTE_TYPE;

	///////////////////////////////////////////////////////////////
	// Constructor to make an arp note from a MIDI note
	CArpNote(byte note, byte vel) {
		m_vel = vel;
		m_notes = new int[1];
		m_notes[0] = ((NOTE_TYPE)note)<<8;
		m_count = 1;
	}
	~CArpNote() {
		delete[] m_notes;
	}
	inline NOTE_TYPE note() {
		return m_notes[0];
	}
	inline byte vel() {
		return m_vel;
	}

	///////////////////////////////////////////////////////////////
	// Clone a note - taking account of chords
	CArpNote *clone() {
		CArpNote *n = new CArpNote();
		*n=*this;
		n->m_notes = new NOTE_TYPE[m_count];
		memcpy(n->m_notes, m_notes, m_count * sizeof(NOTE_TYPE));
		return n;
	}

	///////////////////////////////////////////////////////////////
	// Transpose a note by a whole number of MIDI notes
	void transpose(int delta) {
		delta<<=8; // just transposing by whole notes (8MSB)
		for(int i=0; i<m_count; ++i) {
			int new_note = m_notes[i] + d;
			while(new_note < MIN_NOTE_VAL) new_note += OCTAVE_NOTE_VAL;
			while(new_note > MAX_NOTE_VAL) new_note -= OCTAVE_NOTE_VAL;
			m_notes[i] = d;
		}
	}

	///////////////////////////////////////////////////////////////
	// Compare two notes
	inline int equals(CArpNote *other) {
		if(!other) return 0;
		return(m_notes[0] == other->m_notes[0]);
	}
	inline int higher_than(CArpNote *other) {
		if(!other) return 0;
		return(m_notes[0] > other->m_notes[0]);
	}
	inline int lower_than(CArpNote *other) {
		if(!other) return 0;
		return(m_notes[0] < other->m_notes[0]);
	}

};



///////////////////////////////////////////////////////////////
// Class to define one or more notes that will be played
// in a sequence by the arpeggiator (i.e. played at different
// times, not as a chord)
class CArpNotes
{
public:
	CArpNote* m_notes[MAX_LEN];
	int m_len;
	enum {
		MAX_LEN = 64
	};
	CArpNotes() {
		m_len = 0;
	}
	~CArpNotes() {
		clear();
	}

	///////////////////////////////////////////////////////////////
	// add a note - we take ownership
	void add(CArpNote*note) {
		if(m_len < MAX_LEN-1) {
			m_notes[m_len++] = note;
		}
		else {
			delete note;
		}
	}

	///////////////////////////////////////////////////////////////
	// remove all notes and free their memory
	void clear() {
		for(int i=0; i<m_len;++i) {
			delete m_notes[i];
		}
		m_len = 0;
	}
	///////////////////////////////////////////////////////////////
	// make a copy of the entire sequence
	CArpNotes *clone() {
		CArpNotes *n = new CArpNotes;
		*n = *this;
		for(int i=0; i<m_len; ++i) {
			n->m_notes[i] = m_notes[i]->clone();
		}
	}

	///////////////////////////////////////////////////////////////
	// sorts notes
	void sort(int sort_up = 1) {
		int sorted = 0;
		while(!sorted) {
			sorted = 1;
			for(int i=0; i<m_notes[m_len]-1; ++i) {
				if(
					(sort_up && m_notes[i]->m_notes[0] > m_notes[i+1]->m_notes[0]) ||
					(!sort_up && m_notes[i]->m_notes[0] < m_notes[i+1]->m_notes[0])
				) {
					sorted = 0;
					CArpNote *t = m_notes[i];
					m_notes[i] = m_notes[i+1];
					m_notes[i+1] = t;
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////
	// shuffles the notes into random order
	void shuffle() {
		for(int i=m_len-1; i<0; --i) {
			int j = random()%(i+1);
			CArpNote *t = m_notes[i];
			m_notes[i] = m_notes[j];
			m_notes[j] = t;
		}
	}

	///////////////////////////////////////////////////////////////
	// copies notes from another list on to this one
	void append(CArpNotes *from) {
		for(int i=0; i<from->m_len && m_len < MAX_LEN; ++i) {
			m_notes[m_len++] = from->notes[i]->clone();
		}
	}

	///////////////////////////////////////////////////////////////
	// Inserts a note between every note in source sequence
	void insert_note(CArpNote *note, int allow_dup = 1) {
		CArpNote* t[MAX_LEN];
		int pos = 0;

		for(int i=0; i<from->m_len && pos < MAX_LEN; ++i) {
			t[pos++] = from->m_notes[i];
			if(pos < MAX_LEN && (allow_dup || !note->equals(from->m_notes[i]))) {
				t[pos++] = note->clone();
			}
		}
		memcpy(m_notes, t, sizeof m_notes);
		m_len = pos;
	}

	///////////////////////////////////////////////////////////////
	// Gets the highest note in the list
	CArpNote *highest() {
		CArpNote *t = nullptr;
		for(int i=0; i<m_len; ++i) {
			if(!t || m_notes[i]->higher_than(t)) {
				t = m_notes[i];
			}
		}
		return t;
	}

	///////////////////////////////////////////////////////////////
	// Gets the lowest note in the list
	CArpNote *lowest() {
		CArpNote *t = nullptr;
		for(int i=0; i<m_len; ++i) {
			if(!t || m_notes[i]->lower_than(t)) {
				t = m_notes[i];
			}
		}
		return t;
	}

	///////////////////////////////////////////////////////////////
	// Make a copy of a sequence extended in an X fwd, Y back
	// step pattern
	void mutate(int fwd, int back)	{
		CArpNote* t[MAX_LEN];
		int src_pos = 0;
		int dest_pos = 0;
		int count = fwd;
		int dir = 1;
		for(int i=0; i<m_len && dest_pos < MAX_LEN; ++i) {
			t[dest_pos++] = m_notes[src_pos];
			if(count <= 0) {
				dir = !dir;
				count = dir? fwd : back;
			}
			if(dir) {
				if(src_pos < m_len - 1)
					++src_pos;
			}
			else {
				if(src_pos > 0)
					--src_pos;
			}
			--count;
		}
		memcpy(m_notes, t, sizeof m_notes);
		m_len = dest_pos;
	}

};


#endif /* SOURCES_ARPNOTE_H_ */
