/*
 * BasicArpeggio.h
 *
 *  Created on: 3 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_SEQUENCESORT_H_
#define SOURCES_SEQUENCESORT_H_

class CSequenceSort : public CArpProcessor
{

public:
	enum {
		SEQ_MANUAL,
		SEQ_UP,
		SEQ_DOWN,
		SEQ_UP_DOWN,
		SEQ_DOWN_UP,
		SEQ_RANDOM
	};
	int m_mode;

	CSequenceSort() {
		m_mode = SEQ_UP;
	}


	///////////////////////////////////////////////////////////////
	// Extend and transpose an incoming chord
	void process(CArpNotes *notes) {

		switch(m_mode) {
			case SEQ_UP:
				notes->sort(1);
				break;
			case SEQ_DOWN:
				notes->sort(0);
				break;
			case SEQ_UP_DOWN:
			case SEQ_DOWN_UP:
				if(notes->m_len > 1) // no need to process single note
				{
					CArpNotes *t = notes->clone(); // copy the notes

					notes->sort(1); // sort original notes UP
					notes->m_len--; // remove highest note to avoid double play

					t->sort(0);	// sort cloned notes DOWN
					t->m_len--; // remove lowest note to avoid double play

					if(m_mode == SEQ_UP_DOWN) {
						notes->append(t); // join descending notes after ascending
						delete t; // and free them
					}
					else {
						t->append(notes); // join ascending after descending
						delete notes; // free them
						notes = t; // and use the new list
					}
				}
				break;
			case SEQ_RANDOM:
				notes->shuffle();
				break;
			default:
			case SEQ_MANUAL:
				break;
		}
		forward(notes);
	}

};



#endif /* SOURCES_SEQUENCESORT_H_ */
