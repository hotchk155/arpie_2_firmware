/*
 * SequenceMutator.h
 *
 *  Created on: 27 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_SEQUENCEMUTATOR_H_
#define SOURCES_SEQUENCEMUTATOR_H_

class CSequenceMutator {
public:
	enum {
		NONE,
		INSERT_LOWEST,
		INSERT_HIGHEST,
		MUTATE_3F1B,
		MUTATE_4F2B
	};
	int m_mode;
	CSequenceMutator() {
		m_mode = MUTATE_3F1B;
	}
	void process(CArpNotes *seq) {
		switch(m_mode) {
		case INSERT_HIGHEST:
			seq->insert_note(seq->highest(), 0);
			break;
		case INSERT_LOWEST:
			seq->insert_note(seq->lowest(), 0);
			break;
		case MUTATE_3F1B:
			seq->mutate(3,1);
			break;
		case MUTATE_4F2B:
			seq->mutate(4,2);
			break;
		}
	}
};



#endif /* SOURCES_SEQUENCEMUTATOR_H_ */
