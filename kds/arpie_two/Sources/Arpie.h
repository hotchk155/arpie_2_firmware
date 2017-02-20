/*
 * Arpie.h
 *
 *  Created on: 11 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_ARPIE_H_
#define SOURCES_ARPIE_H_

#include <string.h>
#include "Cpu.h"
#include "Events.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Config.h"
#include "UART0.h"
#include "GPIO_PDD.h"


#include "Midi.h"
#include "ArpConfig.h"
#include "Parameters.h"

typedef uint32_t MS_COUNTER;
#define I2C_ADDR_DISP 123
extern MS_COUNTER g_millis;
enum {
	LEDS_CLOCK,
	LEDS_RX,
	LEDS_TX,
};


void metro_init(byte which);

void leds_init();
void leds_tick();
void leds_pulse(byte which);

void midichord_init();
void midichord_on_msg(MIDI_MSG *msg);
byte midichord_is_changed();
void midichord_get(ARP_NOTES *result);
void midichord_on_hold_off();


void disp_format_4digit(int n, byte *raster);
void disp_format_3digit(int n, byte *raster);

void disp_text(const byte *sz);
void disp_format_text(const byte *sz, byte *raster);
void disp_write(byte *raster, byte *highlight);

int arpie_get_param(int param);
void arpie_set_param(int param, int value);

void arpie_init();
void arpie_run();


void midi_out_init();
void midi_out_run();
void midi_out_msg(MIDI_MSG *msg);
void midi_out_on_tx_complete(byte status);

void clockint_init();
void clockint_on_set_bpm();
byte clockint_is_tick();

void midi_in_init();
MIDI_MSG *midi_in_msg();
void midi_in_on_rx_complete(byte status);


void ui_init();
void ui_run();
void ui_on_key(byte key);
void ui_on_encoder(char delta);

///////////////////////////////////////////////////////////////////////////
// encoder driver
void encoder_init();
int encoder_run();

///////////////////////////////////////////////////////////////////////////
// I2C Driver
void i2c_init();
byte i2c_init_write(byte addr, byte *payload, byte payload_size);
byte i2c_init_read(byte addr);
byte i2c_is_response_pending();
byte i2c_get_response();
void i2c_on_read_complete();
void i2c_on_write_complete();
void i2c_on_error();

void i2c_write(byte addr, byte *payload, byte payload_size);
byte i2c_read(byte addr);



#endif /* SOURCES_ARPIE_H_ */
