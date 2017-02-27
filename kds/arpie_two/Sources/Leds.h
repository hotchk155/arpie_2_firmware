/*
 * Leds.h
 *
 *  Created on: 20 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_LEDS_H_
#define SOURCES_LEDS_H_

class CLeds {
public:
	enum {
		RX_TIME = 10,
		TX_TIME = 10,
		CLOCK_TIME = 10,
		TX_LED_PIN = GPIO_PDD_PIN_0,
		RX_LED_PIN = GPIO_PDD_PIN_6,
		CLOCK_LED_PIN = GPIO_PDD_PIN_7
	};
protected:
	byte m_rx_timeout;
	byte m_tx_timeout;
	byte m_clock_timeout;
public:
	CLeds() {
		m_rx_timeout = 0;
		m_tx_timeout = 0;
		m_clock_timeout = 0;
	}
	inline static CLeds& instance() {
		static CLeds leds;
		return leds;
	}
	void pulse_rx() {
		GPIO_PDD_SetPortDataOutputMask(GPIOA_BASE_PTR, RX_LED_PIN);
		m_rx_timeout = RX_TIME;
	}
	void pulse_tx() {
		GPIO_PDD_SetPortDataOutputMask(GPIOA_BASE_PTR, TX_LED_PIN);
		m_tx_timeout = TX_TIME;
	}
	void pulse_clock() {
		GPIO_PDD_SetPortDataOutputMask(GPIOA_BASE_PTR, CLOCK_LED_PIN);
		m_clock_timeout = CLOCK_TIME;
	}
	void on_tick() {
		if(m_rx_timeout) {
			if(--m_rx_timeout == 0) {
				GPIO_PDD_ClearPortDataOutputMask(GPIOA_BASE_PTR, RX_LED_PIN);
			}
		}
		if(m_tx_timeout) {
			if(--m_tx_timeout == 0) {
				GPIO_PDD_ClearPortDataOutputMask(GPIOA_BASE_PTR, TX_LED_PIN);
			}
		}
		if(m_clock_timeout) {
			if(--m_clock_timeout == 0) {
				GPIO_PDD_ClearPortDataOutputMask(GPIOA_BASE_PTR, CLOCK_LED_PIN);
			}
		}
	}
};

#endif /* SOURCES_LEDS_H_ */
