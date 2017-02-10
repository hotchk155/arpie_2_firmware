/*
 * IndicatorLed.h
 *
 *  Created on: 6 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_INDICATORLED_H_
#define SOURCES_INDICATORLED_H_

class CIndicatorLed {
	uint32_t m_mask;
	int m_timeout;
	int m_duration;
public:
	CIndicatorLed(uint32_t mask, int duration) {
		m_mask = mask;
		m_timeout = 0;
		m_duration = duration;
	}
	inline void trigger() {
		m_timeout = m_duration;
		GPIO1_SetPortBits(GPIO1_DeviceData, m_mask);
	}
	inline void tick() {
		if(m_timeout) {
			if(!--m_timeout) {
				GPIO1_ClearPortBits(GPIO1_DeviceData, m_mask);
			}
		}
	}
};



#endif /* SOURCES_INDICATORLED_H_ */
