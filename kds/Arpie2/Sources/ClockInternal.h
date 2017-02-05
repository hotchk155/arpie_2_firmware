/*
 * Clock.h
 *
 *  Created on: 4 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_CLOCKINTERNAL_H_
#define SOURCES_CLOCKINTERNAL_H_


class CClockInternal {
	double m_bpm;
	double m_tick_period;
	double m_next_tick;
public:
	CMetronome *m_metronome_1;
	CMetronome *m_metronome_2;
	enum {
		DEFAULT_BPM = (120<<8),
		MS_PER_MIN = (60 * 1000),
		TICKS_PQN = 24
	};
	CClockInternal() {
		m_bpm = 0;
		m_tick_period = 0;
		m_next_tick = 0;
		m_metronome_1 = nullptr;
		m_metronome_2 = nullptr;
	}
	void set_bpm(double  bpm) {
		m_bpm = bpm;
		m_tick_period = (double)MS_PER_MIN/(bpm * TICKS_PQN);
	}
	double  get_bpm() {
		return m_bpm;
	}

	void init() {
		set_bpm(DEFAULT_BPM);
	}
	/////////////////////////////////////////////////
	// run the clock - if a 24ppqn tick has occurred
	// then a nonzero value is returned
	byte run(uint32_t ms) {
		if(ms >= m_next_tick) {
			m_next_tick += m_tick_period;
			if(m_metronome_1) {
				m_metronome_1->tick();
			}
			if(m_metronome_2) {
				m_metronome_2->tick();
			}
		}
	}
};


#endif /* SOURCES_CLOCKINTERNAL_H_ */
