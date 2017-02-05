/*
 * ChordExtender.h
 *
 *  Created on: 3 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_CHORDEXTENDER_H_
#define SOURCES_CHORDEXTENDER_H_

#include "ArpProcessor.h"

class CChordExtender : public CArpProcessor {
public:
	int m_octave_shift;
	int m_transpose;
	int m_octave_extend;

	CChordExtender() {
		m_octave_shift = 0;
		m_transpose = 0;
		m_octave_extend = 0;
	}

	///////////////////////////////////////////////////////////////
	// Extend and transpose an incoming chord
	void process(CArpNotes *notes) {
		int delta = m_transpose + 12 * m_octave_shift;
		for(int i=0; i<notes->m_len; ++i) {
			notes->m_notes[i].transpose(delta);
		}
		int pos = notes->m_len;
		delta = 0;
		for(int i=0; i<m_octave_extend && pos < CArpNotes::MAX_LEN; ++i) {
			delta += 12;
			for(int j=0; j<notes->m_len && pos < CArpNotes::MAX_LEN; ++j) {
				CArpNote *clone = notes->m_notes[i].clone();
				clone->transpose(delta);
				notes->m_notes[pos++] = clone;
			}
		}

		// forward the extended note set, the receiver is responsible
		// for it
		forward(notes);
	}
};


#endif /* SOURCES_CHORDEXTENDER_H_ */
