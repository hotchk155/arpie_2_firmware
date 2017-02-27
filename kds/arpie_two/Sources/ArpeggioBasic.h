/*
 * ArpeggioBasic.h
 *
 *  Created on: 24 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_ARPEGGIOBASIC_H_
#define SOURCES_ARPEGGIOBASIC_H_

class CArpeggioBasic {
public:
	enum {
		SORT_NONE,
		SORT_UP,
		SORT_DOWN,
		SORT_UP_DOWN,
		SORT_DOWN_UP,
		SORT_RANDOM
	};

	typedef struct {
		byte sort;
		byte octaves;
	} CONFIG;
	CONFIG m_cfg;

	CArpeggioBasic() {
		m_cfg.sort = SORT_UP_DOWN;
		m_cfg.octaves = 3;
	}

	///////////////////////////////////////////////////////////////
	void process(CArpNotes *notes) {
		notes->extend(m_cfg.octaves);
		switch(m_cfg.sort) {
			case SORT_UP:
				notes->sort(1);
				break;
			case SORT_DOWN:
				notes->sort(0);
				break;
			case SORT_UP_DOWN:
				notes->sort(1);
				notes->reflect();
				break;
			case SORT_DOWN_UP:
				notes->sort(1);
				notes->reflect();
				break;
			case SORT_RANDOM:
				notes->shuffle();
				break;
			default:
			case SORT_NONE:
				break;
		}
	}


};



#endif /* SOURCES_ARPEGGIOBASIC_H_ */
