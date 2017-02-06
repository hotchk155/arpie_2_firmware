/*
 * ChordExtender.h
 *
 *  Created on: 3 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_OCTAVEEXTENDER_H_
#define SOURCES_OCTAVEEXTENDER_H_

#include "Definitions.h"

class COctaveExtender : public IArpSequenceConsumer {
public:
	int m_extend_up;
	int m_extend_down;
	IArpSequenceConsumer *m_consumer;

	COctaveExtender() {
		int m_extend_up = 0;
		int m_extend_down = 0;
		m_consumer = nullptr;
	}

	///////////////////////////////////////////////////////////////
	// Extend and transpose an incoming chord
	void process_sequence(CArpNotes *notes) {
		int pos = notes->count();

		for(int i=-m_extend_down; i<m_extend_up && pos < CArpNotes::MAX_LEN; ++i) {
			int delta = 12 * i;
			for(int j=0; j<notes->count();  ++j) {
				CArpNote *clone = notes->note(i)->clone();
				if(clone->transpose(delta,0)) {
					notes->add(clone);
				}
			}
		}
		if(m_consumer) {
			m_consumer->process_sequence(notes);
		}
	}
};


#endif /* SOURCES_OCTAVEEXTENDER_H_ */
