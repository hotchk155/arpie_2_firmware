/*
 * Leds.c
 *
 *  Created on: 19 Feb 2017
 *      Author: Jason
 */

#include "Arpie.h"

#define TX_LED_PIN	GPIO_PDD_PIN_0
#define RX_LED_PIN	GPIO_PDD_PIN_6
#define CLOCK_LED_PIN	GPIO_PDD_PIN_7
#define LED_TIMEOUT	20

static byte rx_led_timeout;
static byte tx_led_timeout;
static byte clock_led_timeout;

void leds_init() {
	rx_led_timeout = 0;
	tx_led_timeout = 0;
	clock_led_timeout = 0;
}

void leds_tick() {
	if(rx_led_timeout) {
		if(--rx_led_timeout == 0) {
			GPIO_PDD_ClearPortDataOutputMask(GPIOA_BASE_PTR, RX_LED_PIN);
		}
	}
	if(tx_led_timeout) {
		if(--tx_led_timeout == 0) {
			GPIO_PDD_ClearPortDataOutputMask(GPIOA_BASE_PTR, TX_LED_PIN);
		}
	}
	if(clock_led_timeout) {
		if(--clock_led_timeout == 0) {
			GPIO_PDD_ClearPortDataOutputMask(GPIOA_BASE_PTR, CLOCK_LED_PIN);
		}
	}
}

void leds_pulse(byte which) {
	switch(which) {
	case LEDS_RX:
		rx_led_timeout = LED_TIMEOUT;
		GPIO_PDD_SetPortDataOutputMask(GPIOA_BASE_PTR, RX_LED_PIN);
		break;
	case LEDS_TX:
		tx_led_timeout = LED_TIMEOUT;
		GPIO_PDD_SetPortDataOutputMask(GPIOA_BASE_PTR, TX_LED_PIN);
		break;
	case LEDS_CLOCK:
		clock_led_timeout = LED_TIMEOUT;
		GPIO_PDD_SetPortDataOutputMask(GPIOA_BASE_PTR, CLOCK_LED_PIN);
		break;
	}
}
