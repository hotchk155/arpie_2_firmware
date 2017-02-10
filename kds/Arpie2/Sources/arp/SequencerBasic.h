/*
 * BasicSequencer.h
 *
 *  Created on: 3 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_SEQUENCERBASIC_H_
#define SOURCES_SEQUENCERBASIC_H_

#include "Definitions.h"
#include "ArpNote.h"

class CSequencerBasic : public IArpSequenceConsumer, public ISequencer {
public:
protected:
	CArpNotes *m_sequence;
	int m_step;
	void play_step() {
		if(m_sequence && m_step < m_sequence->count() && m_consumer) {
			m_consumer->note_start(m_sequence->note(m_step));
		}
	}
public:
	IArpNoteConsumer *m_consumer;
	CSequencerBasic() {
		m_step = 0;
		m_sequence = nullptr;
		m_consumer = nullptr;
	}
	virtual ~CSequencerBasic() {
		if(m_sequence) {
			delete m_sequence;
		}
	}

	///////////////////////////////////////////////////////
	void process_sequence(CArpNotes *notes) {
		if(m_sequence) {
			delete m_sequence;
		}
		m_sequence = notes;
	}


	void sequencer_start() {
		m_step = 0;
		play_step();
	}
	void sequencer_stop() {
		if(m_consumer) {
			m_consumer->note_stop();
		}
	}
	void sequencer_continue() {
	}
	void sequencer_step() {
		if(m_sequence) {
			if(++m_step >= m_sequence->count()) {
				m_step = 0;
			}
			play_step();
		}
	}

};



#endif /* SOURCES_SEQUENCERBASIC_H_ */
