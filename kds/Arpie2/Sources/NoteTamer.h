/*
 * NoteTamer.h
 *
 *  Created on: 5 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_NOTETAMER_H_
#define SOURCES_NOTETAMER_H_

///////////////////////////////////////////////////////////////
// This class performs transposition, limiting and force
// to scale before passing a note onwards
//
// This class supports only a single CArpNote at a time
// (this can represent a chord)
class CNoteTamer : public IArpNoteConsumer {

public:
	enum {

	};
	enum {

	};
	INoteConsumer *m_forward_1;
	INoteConsumer *m_forward_2;
	CNoteTamer() {
		m_forward_1 = nullptr;
		m_forward_2 = nullptr;
	}

	void start_note(CArpNote *note) {

		if(m_forward_1) m_forward_1->start_note(note);
		if(m_forward_2) m_forward_2->start_note(note);
	}
	void  stop_note() {
		if(m_forward_1) m_forward_1->stop_note();
		if(m_forward_2) m_forward_2->stop_note();
	}

};



#endif /* SOURCES_NOTETAMER_H_ */
