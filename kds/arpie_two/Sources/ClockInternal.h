/*
 * ClockInternal.h
 *
 *  Created on: 20 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_CLOCKINTERNAL_H_
#define SOURCES_CLOCKINTERNAL_H_

class CClockInternal {
	enum {
		MS_PER_MIN = (60 * 1000),
		TICKS_PQN = 24
	};
	double m_next_tick;
public:
	typedef struct {
		double tick_period;
		byte bpm;
	} CONFIG;
	CONFIG m_cfg;

	CClockInternal()  {
		m_next_tick = 0;
		set_bpm(120);
	}

	void set_bpm(byte bpm) {
		m_cfg.bpm = bpm;
		m_cfg.tick_period = (double)MS_PER_MIN/((double)m_cfg.bpm * TICKS_PQN);

	}
	byte get_bpm() {
		return m_cfg.bpm;
	}

	byte is_24ppqn() {
		if(g_millis >= m_next_tick) {
			m_next_tick += m_cfg.tick_period;
			if(g_millis >= m_next_tick) {
				m_next_tick = g_millis + m_cfg.tick_period;
			}
			return 1;
		}
		return 0;
	}
};



#endif /* SOURCES_CLOCKINTERNAL_H_ */
