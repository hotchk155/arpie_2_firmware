/*
 * MetronomeBeat.h
 *
 *  Created on: 6 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_METRONOMEBEAT_H_
#define SOURCES_METRONOMEBEAT_H_

#include "Definitions.h"

class CMetronomeBeat : public IClockListener {
protected:
	enum {
		TICKS_PER_BEAT = 24
	};
	byte m_ticks;
public:
	CMetronomeBeat() {
		m_ticks = 0;
	}
	void on_clock_continue() {
	}
	void on_clock_start() {
		m_ticks = 0;
		g_arp->on_event(IArpEvents::BEAT,0);
	}
	void on_clock_stop() {
	}
	void on_clock_tick() {
		if(++m_ticks >= TICKS_PER_BEAT) {
			g_arp->on_event(IArpEvents::BEAT,0);
			m_ticks = 0;
		}
	}
};




#endif /* SOURCES_METRONOMEBEAT_H_ */
