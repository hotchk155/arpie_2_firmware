/*
 * BasicSequencer.h
 *
 *  Created on: 3 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_SEQUENCERBASIC_H_
#define SOURCES_SEQUENCERBASIC_H_

#include "ArpProcessor.h"

class CSequencerBasic : public IArpProcessor, public ISequencer {

public:
	CArpNotes *m_seq;

	///////////////////////////////////////////////////////
	void process(CArpNotes *notes) {
		if(m_seq) {
			delete m_seq;
		}
		m_seq = seq;
	}


	void step() {

	}

};



#endif /* SOURCES_SEQUENCERBASIC_H_ */
