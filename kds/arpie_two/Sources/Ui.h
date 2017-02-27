/*
 * Ui.h
 *
 *  Created on: 20 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_UI_H_
#define SOURCES_UI_H_



/*
 * UI.c
 *
 *  Created on: 17 Feb 2017
 *      Author: Jason
 */
#include "Arpie.h"

class IUiComponent {
public:
	virtual void ui_render(uint32_t *raster, uint32_t *highlight) = 0;
	virtual byte ui_needs_repaint() = 0;
	virtual void ui_on_key(byte key, byte modifiers) = 0;
	virtual void ui_on_encoder(char delta, byte modifiers) = 0;
};

class CUi {
public:
	CUi() {
	}
};

/*
//////////////////////////////////////////////////////////////////
// UI PAGE FOR SELECTING A PARAMETER FROM A SET OF DISCRETE VALUES

#define UI_FLAG_REFRESH  0x01
#define UI_FLAG_VALUE_CHANGE  0x02

static byte s_flags = 0;

static byte s_edit_param = PARAM_GATE_LEN;
static byte s_edit_value = 4;


static uint32_t s_key_status = 0;


enum {
	KEY_ZERO = 0,

        KEY_A1,  KEY_A2,           KEY_A3,  KEY_A4,  KEY_A5,
	KEY_B1,  KEY_B2,  KEY_B3,  KEY_B4,  KEY_B5,  KEY_B6,  KEY_B7,  KEY_B8,
	KEY_C1,  KEY_C2,  KEY_C3,  KEY_C4,  KEY_C5,
	KEY_D1,  KEY_D2,  KEY_D3,  KEY_D4,  KEY_D5,


	KEY_RELEASE = 0x80,

};

enum {
	KEYBIT_A1 = ((uint32_t)1<<0),
	KEYBIT_A2 = ((uint32_t)1<<1),
	KEYBIT_A3 = ((uint32_t)1<<2),
	KEYBIT_A4 = ((uint32_t)1<<3),
	KEYBIT_A5 = ((uint32_t)1<<4),
	KEYBIT_B1 = ((uint32_t)1<<5),
	KEYBIT_B2 = ((uint32_t)1<<6),
	KEYBIT_B3 = ((uint32_t)1<<7),
	KEYBIT_B4 = ((uint32_t)1<<8),
	KEYBIT_B5 = ((uint32_t)1<<9),
	KEYBIT_B6 = ((uint32_t)1<<10),
	KEYBIT_B7 = ((uint32_t)1<<11),
	KEYBIT_B8 = ((uint32_t)1<<12),
	KEYBIT_C1 = ((uint32_t)1<<13),
	KEYBIT_C2 = ((uint32_t)1<<14),
	KEYBIT_C3 = ((uint32_t)1<<15),
	KEYBIT_C4 = ((uint32_t)1<<16),
	KEYBIT_C5 = ((uint32_t)1<<17),
	KEYBIT_D1 = ((uint32_t)1<<18),
	KEYBIT_D2 = ((uint32_t)1<<19),
	KEYBIT_D3 = ((uint32_t)1<<20),
	KEYBIT_D4 = ((uint32_t)1<<21),
	KEYBIT_D5 = ((uint32_t)1<<22),

	KEYBIT_SHIFT = KEYBIT_D1,
};


typedef struct {
	const byte *params;
	const byte param;	// if params=NULL, this is the param to edit otherwise it is the count of items in list params points to

} MENU_BUTTON;


enum {
	PFLAG_DEFER = 0x01,
	PFLAG_NUMBER = 0x02
};
typedef struct {
	const char *name;
	const char **value;
	const byte max_value;
	const byte flags;
	const byte min_value;
} PARAM_INFO;

static const byte s_shift_bit = 0;



static const char* c_arp_type[ARPTYPE_ENUM_MAX] = {"UP", "DOWN", "UP-DN", "DN-UP", "RANDOM"};

static const char* c_gate_len[GATE_ENUM_MAX] = {"OFF", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "FULL", "TIE"};
static const char* c_midi_chan[CHAN_ENUM_MAX] = {"CH1", "CH2", "CH3", "CH4", "CH5", "CH6", "CH7", "CH8", "CH9", "CH10", "CH11", "CH12", "CH13", "CH14", "CH15", "CH16", "OMNI"};
static const char* c_seq_rate[SEQ_RATE_ENUM_MAX] = {"WHOLE", "1/2D", "1/2", "1/4D", "1/2T", "1/4", "1/8D", "1/4T", "1/8", "1/16D", "1/8T", "1/16", "1/16T", "1/32"};

static const PARAM_INFO c_param[PARAM_ENUM_MAX] = {
		{ "RATE", c_seq_rate, SEQ_RATE_ENUM_MAX-1 },
		{ "GATE", c_gate_len, GATE_ENUM_MAX-1 },
		{ "IN CH", c_midi_chan, CHAN_ENUM_MAX-1 },
		{ "OUT CH", c_midi_chan, CHAN_ENUM_MAX-2 },
		{ "TYPE", c_arp_type, ARPTYPE_ENUM_MAX-1, PFLAG_DEFER },
		{ "BPM", NULL, 250, PFLAG_NUMBER, 30 }
};


////////////////////////////////////////////////////////////////
// Decide how to represent
static void render_indicator(byte *raster_buf, byte *highlight_buf) {
	byte num_values = c_param[s_edit_param].max_value;
	byte edit_value = s_edit_value;

	// 0         1         2
	// 012345678901234567890123

	// XXXX XXXX
	// XXXX XXXX XXXX XXXX XXXX
	// XXX XXX XXX XXX XXX XXX
	// XX XX XX XX XX XX XX XX
	// X X X X X X X X X X X X
	// XXXXXXXXXXXXXXXXXXXXXXXX

	byte width;
	byte space=0;
	if(num_values <= 5) {
		width = 5;
	}
	else if(num_values <= 6) {
		width = 4;
	}
	else if(num_values <= 8) {
		width = 3;
	}
	else if(num_values <= 12) {
		width = 2;
	}
	else if(num_values <= 24){
		width = 1;
	}
	else {
		return;
	}

	uint32_t raster = 0;
	uint32_t highlight = 0;
	byte this_value = 0;
	byte count = 0;
	for(byte i=0; i<num_values; ++i) {
		for(byte j=0; j<width; ++j) {
			raster<<=1;
			highlight<<=1;
			++count;
			if(this_value == edit_value) {
				raster |= 1;
			}
			else {
				highlight |= 1;
			}
		}
		++this_value;
	}
	while(count++<24) {
		raster<<=1;
		highlight<<=1;
	}
	raster_buf[18] = (byte)(raster >> 16);
	raster_buf[19] = (byte)(raster >> 8);
	raster_buf[20] = (byte)(raster);
	highlight_buf[18] = (byte)(highlight >> 16);
	highlight_buf[19] = (byte)(highlight >> 8);
	highlight_buf[20] = (byte)(highlight);
}


////////////////////////////////////////////////////////////////
// Prepare the UI code to show a new parameter edit screen
static void ui_new_param() {
	byte raster[24] = {0};
	byte highlight[24] = {0};
	disp_format_text(c_param[s_edit_param].name, raster);
	render_indicator(raster, highlight);
	disp_write(raster, highlight);
}


static void ui_new_value() {
	byte raster[24] = {0};
	byte highlight[24] = {0};
	if(c_param[s_edit_param].flags & PFLAG_NUMBER) {
		disp_format_3digit(s_edit_value, raster);
		arpie_set_param(s_edit_param, s_edit_value);
	}
	else if(s_flags & PFLAG_DEFER) {
		disp_format_text(c_param[s_edit_param].value[s_edit_value], highlight);
	}
	else {
		disp_format_text(c_param[s_edit_param].value[s_edit_value], raster);
	}
	render_indicator(raster, highlight);
	disp_write(raster, highlight);
}


void ui_begin_edit(byte param) {
	s_edit_param = param;
	s_edit_value = arpie_get_param(s_edit_param);
	ui_new_param();
}

void ui_on_key(byte key) {

	if(key & KEY_RELEASE) {
		s_key_status &= ~(((uint32_t)1)<<((key & ~KEY_RELEASE)-1));

		if(s_flags & PFLAG_DEFER) {
			s_flags &= ~PFLAG_DEFER;
			s_flags |= UI_FLAG_VALUE_CHANGE;
		}
	}
	else {
		s_key_status |= (((uint32_t)1)<<(key-1));

		switch(key) {
		case KEY_B1:
			if(s_key_status & KEYBIT_SHIFT)
				ui_begin_edit(PARAM_MIDI_IN_CHAN);
			else
				ui_begin_edit(PARAM_MIDI_OUT_CHAN);
			break;
		case KEY_B2:
			ui_begin_edit(PARAM_GATE_LEN);
			break;
		case KEY_B3:
			ui_begin_edit(PARAM_ARP_TYPE);
			break;
		case KEY_B4:
			ui_begin_edit(PARAM_BPM);
			break;
		case KEY_B5:
			ui_begin_edit(PARAM_SEQ_RATE);
			break;
		}
	}
}


void ui_on_encoder(char delta) {
	if(delta < 0 && s_edit_value > c_param[s_edit_param].min_value) {
		--s_edit_value;
		s_flags |= UI_FLAG_VALUE_CHANGE;
		s_flags |= (c_param[s_edit_param].flags & PFLAG_DEFER);
	}
	else if(delta > 0 && s_edit_value < c_param[s_edit_param].max_value) {
		++s_edit_value;
		s_flags |= UI_FLAG_VALUE_CHANGE;
		s_flags |= (c_param[s_edit_param].flags & PFLAG_DEFER);
	}
}








void ui_init() {
	//ui_new_param();
}
void ui_run() {
	if(s_flags & UI_FLAG_VALUE_CHANGE) {
		ui_new_value();
		s_flags &= ~UI_FLAG_VALUE_CHANGE;
	}
}





*/
#endif /* SOURCES_UI_H_ */
