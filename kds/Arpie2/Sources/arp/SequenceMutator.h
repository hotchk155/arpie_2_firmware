/*
 * SequenceMutate.h
 *
 *  Created on: 3 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_SEQUENCEMUTATOR_H_
#define SOURCES_SEQUENCEMUTATOR_H_

#include "ArpProcessor.h"

class CSequenceMutator  : public CArpProcessor {
public:
	enum {
		INSERT_NONE,
		INSERT_HIGHEST,
		INSERT_LOWEST,
		MUTATE_3F1B,
		MUTATE_4F2B
	};
	int m_mode;
	CSequenceMutator() {
		m_mode = INSERT_NONE;
	}
	void process(ArpNotes *notes) {
		switch(m_mode) {
		case INSERT_HIGHEST:
			notes->insert_note(notes->highest(), 0);
			break;
		case INSERT_LOWEST:
			notes->insert_note(notes->highest(), 0);
			break;
		case MUTATE_3F1B:
			notes->mutate(3,1);
			break;
		case MUTATE_4F2B:
			notes->mutate(4,2);
			break;
		}
		forward(notes);
	}
};



#endif /* SOURCES_SEQUENCEMUTATOR_H_ */
