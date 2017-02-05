/*
 * ChordMaker.h
 *
 *  Created on: 4 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_CHORDMAKER_H_
#define SOURCES_CHORDMAKER_H_

class CChordMaker {
	byte m_changed;
public:
	CChordMaker() {
		m_changed = 0;
	}
	inline void clear_changed() {
		m_changed = 0;
	};
	inline void set_changed() {
		m_changed = 1;
	};
	inline byte is_changed() {
		return m_changed;
	};
	virtual CArpNotes *get_chord() = 0;
};



#endif /* SOURCES_CHORDMAKER_H_ */
