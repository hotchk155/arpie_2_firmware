/*
 * Metronome.h
 *
 *  Created on: 4 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_METRONOMEBASIC_H_
#define SOURCES_METRONOMEBASIC_H_

#include "Sequencer.h"

class CMetronomeBasic : public IMetronome {
	int m_rate;
	int m_ticks;
public:
	ISequencer *m_sequencer;
	enum
	{
	  RATE_1    = 96,
	  RATE_2D   = 72,
	  RATE_2    = 48,
	  RATE_4D   = 36,
	  RATE_2T   = 32,
	  RATE_4    = 24,
	  RATE_8D   = 18,
	  RATE_4T   = 16,
	  RATE_8    = 12,
	  RATE_16D  = 9,
	  RATE_8T   = 8,
	  RATE_16   = 6,
	  RATE_16T  = 4,
	  RATE_32   = 3
	};
	CMetronomeBasic() {
		m_rate = RATE_16;
		m_sequencer = nullptr;
		m_ticks = 0;
	}
	void set_rate(int rate) {
		m_rate = rate;
	}
	int get_rate() {
		return m_rate;
	}
	void on_tick() {
		if(++m_ticks >= m_rate) {
			m_ticks = 0;
			if(m_sequencer) {
				m_sequencer->step();
			}
		}
	}
	void on_reset() {
		m_ticks = 0;
	}
};

#endif /* SOURCES_METRONOMEBASIC_H_ */
