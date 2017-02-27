/*
 * Arpie.h
 *
 *  Created on: 11 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_ARPIE_H_
#define SOURCES_ARPIE_H_

#include <string.h>

extern "C" {
#include "Cpu.h"
#include "Events.h"
#include "I2CBus.h"
#include "SysTick.h"
#include "UART0.h"
#include "GPIOA.h"
#include "GPIO_PDD.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
}

#include "ArpConfig.h"

extern uint32_t g_millis;

#include "Midi.h"
#include "Definitions.h"
#include "I2C.h"
#include "Leds.h"
#include "Encoder.h"
#include "Keyboard.h"
#include "Display.h"
#include "ClockInternal.h"
#include "ArpNotes.h"
#include "Ui.h"
#include "MidiIn.h"
#include "MidiOut.h"
#include "NotePlayer.h"
#include "Sequencer.h"
#include "ChordCatcherMidi.h"
#include "ArpeggioBasic.h"
#include "SequenceMutator.h"

extern CArpNotes scratch;

class CArpie {

	CEncoder m_encoder;
	CClockInternal m_clock_internal;
	CNotePlayer m_note_player_1;
	CSequencer m_sequencer_1;
	CMidiIn m_midi_in;
	CMidiOut m_midi_out;
	CChordCatcherMidi m_chord_catcher_midi;
	CArpeggioBasic m_arpeggio;
	CSequenceMutator m_seq_mutator;

	IUiComponent *m_ui;

	CArpNotes m_seq_1;

	volatile byte m_ticked;
	byte m_key_modifiers;

	byte m_count_24ppqn;
public:
	CArpie() :
		m_clock_internal(),
		m_sequencer_1(&m_seq_1, &m_note_player_1),
		m_note_player_1(&m_midi_out)
	{
		m_ui = &m_sequencer_1;
		m_ticked = 0;
		m_key_modifiers = 0;
		m_count_24ppqn = 0;
	}

	void init() {

		update_display();
		m_midi_in.listen();

		m_chord_catcher_midi.test();

		m_sequencer_1.init();
	}

	void update_display() {
		uint32_t raster[8] = {0};
		uint32_t highlight[8] = {0};
		m_ui->ui_render(raster, highlight);
		CDisplay::instance().write(raster, highlight);

	}
	/////////////////////////////////////////////////////////////////////////
	void run_tasks() {

		if(m_chord_catcher_midi.is_changed()) {
			m_chord_catcher_midi.get_notes(&m_seq_1);
			m_arpeggio.process(&m_seq_1);
			m_seq_mutator.process(&m_seq_1);
			m_sequencer_1.on_sequence_change();
		}


		if(m_clock_internal.is_24ppqn()) {
			m_note_player_1.on_24ppqn();
			m_sequencer_1.on_24ppqn();
			if(++m_count_24ppqn >= 16) {
				m_count_24ppqn = 0;
				CLeds::instance().pulse_clock();
			}
		}

		CLeds::instance().on_tick();

		int delta = m_encoder.read();
		if(delta) {
			m_ui->ui_on_encoder(delta,m_key_modifiers);
			//n+=d;
			//if(n<0) n=0;
			//CDisplay::instance().number(n);
		}

		if(!(g_millis & 0xF)) {
			byte key = CI2C::instance().read(CI2C::DISP_ADDR);
			if(key) {
				if(key & CKeyboard::KEY_RELEASE) {
					if((key & (~CKeyboard::KEY_RELEASE)) == CKeyboard::KEY_D1) {
						m_key_modifiers &= ~CKeyboard::SHIFT_MODIFIER;
					}
				}
				else {
					if(key == CKeyboard::KEY_D1) {
						m_key_modifiers |= CKeyboard::SHIFT_MODIFIER;
					}
				}
				m_ui->ui_on_key(key, m_key_modifiers);
			}
		}

		if(m_ui->ui_needs_repaint()) {
			update_display();
		}


	}

	/////////////////////////////////////////////////////////////////////////
	void run() {
		for(;;) {

			// poll the MIDI input
			midi::MSG *msg = m_midi_in.msg();
			if(msg) {
				// capture any new note in chord
				//m_chord_catcher_midi.on_midi_msg(msg);
			}

			// run the midi output
			m_midi_out.run();

			// run per-tick tasks
			if(m_ticked) {
				run_tasks();
				m_ticked = 0;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////
	// Called from outside when a tick occurs
	void on_tick() {
		m_ticked = 1;
	}

	void on_uart0_rx_complete() {
		m_midi_in.on_rx_complete();
	}
	void on_uart0_rx_error() {
		m_midi_in.on_rx_error();
	}

};

extern CArpie *g_arpie;


/*
typedef struct {
	byte tick_flag:1;
	byte clock_count;
} ARPIE_DATA;

static ARPIE_DATA me;

MS_COUNTER g_millis = 0;
int n = 0;

ARP_CONFIG g_arp;
GLOBAL_CONFIG g_cfg;
GLOBAL_STATE g_state;

///////////////////////////////////////////////////////////////
// ISR for once per millisecond tick count
PE_ISR(ISR_SysTick)
{
	++g_millis;
	me.tick_flag = 1;
}





//void arpie_init_seq(ARP_SEQ *seq) {
//	seq->num_notes = 0;
//	seq->num_pitches = 0;
//}

//void arpie_rebuild_seqs() {
//midichord_get(&s_arp_seq_1);
//	memcpy(&s_arp_seq_2, &s_arp_seq_1, sizeof(ARP_SEQ));
//}

void arpie_default_patch() {
	g_arp.chain[0].seq_rate = SEQ_RATE_16;
	g_arp.chain[1].seq_rate = SEQ_RATE_16;

}

void arpie_apply_config()
{
	clockint_on_set_bpm();
}

///////////////////////////////////////////////////////////////
// task which happen once per millisecond
void arpie_tick()
{
	// poll the keyboard
	if((g_millis%10) == 0) {
		byte key = i2c_read(I2C_ADDR_DISP);
		if(key) {
			ui_on_key(key);
			leds_pulse(LEDS_CLOCK);
		}
	}

	// poll the encoder
	char d = encoder_run(g_millis);
	if(d) {
		ui_on_encoder(d);
	}

	// run the UI
	ui_run();

	leds_tick();


	//if(midichord_is_changed()) {
	//	arpie_rebuild_seqs();
	//}

	if(clockint_is_tick()) {
		if(!me.clock_count) {
			leds_pulse(LEDS_CLOCK);
		}
		if(++me.clock_count >= 24) {
			me.clock_count = 0;
		}
	}

}


void arpie_init_chain(byte which) {
	metro_init(which);
}

void arpie_init()
{

	me.tick_flag = 0;
	me.clock_count = 0;

	g_state.bpm = 120;
	g_state.which_chain = 0;
	g_state.hold = 0;

//	arpie_init_seq(&s_arp_seq_1);
//	arpie_init_seq(&s_arp_seq_2);


	encoder_init();
	i2c_init();
	ui_init();
	//midi_in_init();
	//midichord_init();
	clockint_init();
	arpie_init_chain(0);
	arpie_init_chain(1);

	arpie_default_patch();
	arpie_apply_config();

}


void arpie_run()
{
	MIDI_MSG *msg = midi_in_msg();
	if(msg) {
		midichord_on_msg(msg);
	}

	if(me.tick_flag) {
		arpie_tick();
		me.tick_flag = 0;
	}
}

void arpie_set_param(int param, int value) {
	switch(param) {
	case PARAM_SEQ_RATE:
		g_arp.chain[g_state.which_chain].seq_rate = value;
		break;
	case PARAM_BPM:
		g_state.bpm = value;
		clockint_on_set_bpm();
		break;
	}
}
int arpie_get_param(int param) {
	switch(param) {
	case PARAM_BPM:
		return g_state.bpm;
	case PARAM_SEQ_RATE:
		return g_arp.chain[g_state.which_chain].seq_rate;
	}
	return 0;
}


*/


#endif /* SOURCES_ARPIE_H_ */
