////////////////////////////////////////////////////////
//
//                                           //// ////
//      ////     ////  ////    //  ////      //   //
//         //  //     //  //      //  //    //   //
//     /////  //     //  //  //  //////    //   //
//   //  //  //     //  //  //  //        //   //
//   /////  //     /////   //   //////  //// ////
//                //
//               //     MIDI/CV ARPEGGIATOR
//                        2017/hotchk155
//
////////////////////////////////////////////////////////
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

extern uint32_t g_millis;

#include "Midi.h"
#include "I2C.h"
#include "Leds.h"
#include "Encoder.h"
#include "Keyboard.h"
#include "Display.h"
#include "ClockInternal.h"
#include "ArpNotes.h"
#include "Params.h"
#include "Interfaces.h"
#include "UiParamEdit.h"
#include "MidiIn.h"
#include "MidiOut.h"
#include "NotePlayer.h"
#include "Sequencer.h"
#include "ChordCatcherMidi.h"
#include "ChordSequencer.h"
#include "ArpeggioBasic.h"
#include "SequenceMutator.h"

class CArpie : public IParams {

	enum {
		NUM_CHAINS = 2
	};
	typedef struct {
		INoteProvider *note_provider;
		CArpNotes seq;
		CNotePlayer note_player;
		CSequencer sequencer;
		CArpeggioBasic arpeggio;
		CSequenceMutator mutator;
		short notes_ver;
	} ARPCHAIN;

	ARPCHAIN m_chain[NUM_CHAINS];
	byte m_cur_chain;

	CEncoder m_encoder;
	CClockInternal m_clock_internal;
	CMidiIn m_midi_in;
	CMidiOut m_midi_out;
	CChordCatcherMidi m_chord_catcher_midi;
	CChordSequencer m_chord_sequencer;
	CUIParamEdit m_param_edit;

	IUiComponent *m_ui;


	volatile byte m_ticked;
	byte m_key_modifiers;

	byte m_count_24ppqn;

public:
	CArpie() {
		params::g_params = this;
	}

	static inline CArpie& instance() {
		static CArpie arpie;
		return arpie;
	}

	void init_chain(ARPCHAIN *chain, int chan) {
		chain->seq.init();
		chain->note_player.init(&m_midi_out, chan);
		chain->sequencer.init(&chain->seq, &chain->note_player);

		chain->arpeggio.init();
		chain->mutator.init();
		chain->notes_ver = -1;
		chain->note_provider = &m_chord_sequencer;
	}


	void init() {
		for(int i=0; i<NUM_CHAINS; ++i) {
			init_chain(&m_chain[i], i);
		}
		m_ticked = 0;
		m_key_modifiers = 0;
		m_count_24ppqn = 0;
		m_cur_chain = 0;
		m_midi_in.listen();
		m_chord_sequencer.init();
		m_chord_catcher_midi.init();
		//m_chord_catcher_midi.test();
		CDisplay::instance().init();
		edit_param(params::SEQUENCE);
	}

	void update_display() {
		uint32_t raster[8] = {0};
		uint32_t highlight[8] = {0};
		m_ui->ui_render(raster, highlight);
		CDisplay::instance().write(raster, highlight);

	}

	/////////////////////////////////////////////////////////////////////////
	void on_key(byte key, byte modifiers) {

		if(!m_ui->ui_on_key(key,modifiers)) {
			if(!(key & CKeyboard::KEY_RELEASE)) { // not key release event
				byte key_event = key;
				if(modifiers == CKeyboard::SHIFT_MODIFIER) {
					key_event |= CKeyboard::SHIFT;
				}
				switch(key_event) {
				case CKeyboard::CMD_ARPTYPE: edit_param(params::ARP_TYPE); break;
				case CKeyboard::CMD_OCTSPAN: edit_param(params::OCT_SPAN); break;
				//case CKeyboard::CMD_OCTSHIFT: edit_param(params::OCT_SHIFT); break;
				case CKeyboard::CMD_MUTATE: edit_param(params::SEQ_MUTATE); break;
				case CKeyboard::CMD_BPM: edit_param(params::BPM); break;
				case CKeyboard::CMD_TRIGS: edit_param(params::TRIGS); break;
				case CKeyboard::CMD_RATE: edit_param(params::SEQ_RATE); break;
				case CKeyboard::CMD_AB: edit_param(params::CHAIN); break;
				case CKeyboard::CMD_CHORD: edit_param(params::CHORD_EDIT); break;
				//case CKeyboard::CMD_GATE: edit_param(params::SEQ_GATE); break;
				//case CKeyboard::CMD_VEL: edit_param(params::SEQ_VEL); break;
				//case CKeyboard::CMD_ACCVEL: edit_param(params::SEQ_ACCVEL); break;

				default: return;
				}
				update_display();
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////
	void run_tasks() {

		byte is_24ppqn = m_clock_internal.is_24ppqn();

		if(is_24ppqn) {
			if(++m_count_24ppqn >= 16) {
				m_count_24ppqn = 0;
				CLeds::instance().pulse_clock();
			}
			m_chord_sequencer.on_24ppqn();
		}
		CLeds::instance().on_tick();

		for(int i=0; i<NUM_CHAINS; ++i) {
			ARPCHAIN *chain = &m_chain[i];
			if(chain->notes_ver != chain->note_provider->get_notes_version()) {
				chain->notes_ver = chain->note_provider->get_notes(&chain->seq);
				chain->arpeggio.process(&chain->seq);
				chain->mutator.process(&chain->seq);
				chain->sequencer.on_sequence_change();
			}

			if(is_24ppqn) {
				chain->note_player.on_24ppqn();
				chain->sequencer.on_24ppqn();
			}
		}

		int delta = m_encoder.read();
		if(delta) {
			m_ui->ui_on_encoder(delta,m_key_modifiers);
		}

		if(!(g_millis & 0xF)) {
			byte key = CI2C::instance().read(CDisplay::DISP_ADDR);
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
				on_key(key, m_key_modifiers);
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
				m_chord_catcher_midi.on_midi_msg(msg);
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
	inline void on_tick() {
		m_ticked = 1;
	}
	inline void on_uart0_rx_complete() {
		m_midi_in.on_rx_complete();
	}
	inline void on_uart0_rx_error() {
		m_midi_in.on_rx_error();
	}

	int edit_param(int param) {
		if(m_ui) {
			m_ui->ui_done();
		}
		switch(param) {
		case params::SEQUENCE:
		case params::TRIGS:
			m_ui = &m_chain[m_cur_chain].sequencer;
			break;
		case params::CHORD_EDIT:
			m_ui = &m_chord_sequencer;
			break;

		case params::BPM:
		case params::ARP_TYPE:
		case params::OCT_SPAN:
		case params::SEQ_RATE:
		case params::SEQ_MUTATE:
		case params::CHAIN:
			m_param_edit.set_param(param);
			m_ui = &m_param_edit;
			break;

		}
		m_ui->ui_init();
	}
	int get_param(int param) {
		switch(param) {
		case params::BPM:
			return m_clock_internal.get_bpm();
		case params::ARP_TYPE:
			return m_chain[m_cur_chain].arpeggio.m_cfg.arp_type;
		case params::OCT_SPAN:
			return m_chain[m_cur_chain].arpeggio.m_cfg.octaves;
		case params::SEQ_RATE:
			return m_chain[m_cur_chain].sequencer.m_cfg.rate;
		case params::SEQ_MUTATE:
			return m_chain[m_cur_chain].mutator.m_cfg.m_mode;
		case params::CHAIN:
			return m_cur_chain;
		}
	}
	void set_param(int param, int value) {
		switch(param) {
		case params::BPM:
			m_clock_internal.set_bpm(value);
			break;
		case params::ARP_TYPE:
			m_chain[m_cur_chain].arpeggio.m_cfg.arp_type = value;
			m_chain[m_cur_chain].notes_ver = -1;
			break;
		case params::OCT_SPAN:
			m_chain[m_cur_chain].arpeggio.m_cfg.octaves = value;
			m_chain[m_cur_chain].notes_ver = -1;
			break;
		case params::SEQ_RATE:
			m_chain[m_cur_chain].sequencer.m_cfg.rate = value;
			break;
		case params::SEQ_MUTATE:
			m_chain[m_cur_chain].mutator.m_cfg.m_mode = value;
			m_chain[m_cur_chain].notes_ver = -1;
			break;
		case params::CHAIN:
			m_cur_chain = value;
			break;
		}
	}
};

#endif /* SOURCES_ARPIE_H_ */
