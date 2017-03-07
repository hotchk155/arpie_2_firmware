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
#include "UiTitle.h"
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
	enum {
		CTX_ARP,
		CTX_TRIG,
		CTX_CHORD
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
	byte m_ui_context;

	CEncoder m_encoder;
	CClockInternal m_clock_internal;
	CMidiIn m_midi_in;
	CMidiOut m_midi_out;
	CChordCatcherMidi m_chord_catcher_midi;
	CChordSequencer m_chord_sequencer;
	CUIParamEdit m_param_edit;
	CUiTitle m_title;
	IUiComponent *m_ui;


	volatile byte m_ticked;
	byte m_key_modifiers;
	byte m_param;
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

	void toggle_chain() {
		if(++m_cur_chain >= NUM_CHAINS) {
			m_cur_chain = 0;
		}
		CLeds::instance().ab_led(m_cur_chain);
		m_title.m_mode = (m_cur_chain == 0) ? CUiTitle::ARP_A : CUiTitle::ARP_B;
		m_ui = &m_title;
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
		m_param = params::SEQUENCE;
		m_ui_context = CTX_ARP;
		m_title.m_mode = CUiTitle::TITLE1;
		m_ui = &m_title;
		update_display();
	}

	void update_display() {
		uint32_t raster[8] = {0};
		uint32_t highlight[8] = {0};
		m_ui->ui_render(raster, highlight);
		CDisplay::instance().write(raster, highlight);

	}

	/////////////////////////////////////////////////////////////////////////
	void on_key(byte key, byte modifiers) {

		if(key & CKeyboard::KEY_RELEASE) {
			switch(key & ~CKeyboard::KEY_RELEASE) {
			case CKeyboard::KEY_D2: edit_param(m_param); break;
			default: return;
			}
			update_display();
		}
		else {
			byte key_event = key;
			if(modifiers == CKeyboard::SHIFT_MODIFIER) {
				key_event |= CKeyboard::SHIFT;
			}

			if(!m_ui->ui_on_key(key_event)) {
				switch(key_event) {

				//////////////////////////////////////////////////////////////
				// ROW A

				//////////////////////////////////////////////////////////////
				// ROW B
				case CKeyboard::KEY_B2:
					if(m_ui_context == CTX_ARP) {
						edit_param(params::ARP_TYPE);
					}
					break;
				case CKeyboard::KEY_B3:
					if(m_ui_context == CTX_ARP) {
						edit_param(params::OCT_SPAN);
					}
					break;
				case CKeyboard::KEY_B4:
					if(m_ui_context == CTX_ARP) {
						edit_param(params::SEQ_MUTATE);
					}
					break;
				case CKeyboard::KEY_B5:
					if(m_ui_context == CTX_ARP) {
						edit_param(params::SEQ_RATE);
					}
					break;


				//////////////////////////////////////////////////////////////
				// SHIFTED ROW B
				case CKeyboard::SHIFT|CKeyboard::KEY_B1:
					if(m_ui_context == CTX_TRIG) {
						edit_param(params::VEL);
					}
					break;
				case CKeyboard::SHIFT|CKeyboard::KEY_B2:
					if(m_ui_context == CTX_TRIG) {
						edit_param(params::ACC_VEL);
					}
					else if(m_ui_context == CTX_CHORD) {
						edit_param(params::CHORDMODE);
					}
					break;
				case CKeyboard::SHIFT|CKeyboard::KEY_B3:
					if(m_ui_context == CTX_TRIG) {
						edit_param(params::GATE);
					}
					break;
				case CKeyboard::SHIFT|CKeyboard::KEY_B4:
					if(m_ui_context == CTX_TRIG) {
						edit_param(params::GATE_LONG);
					}
					else if(m_ui_context == CTX_CHORD) {
						edit_param(params::CHORDRATE);
					}
					break;
				case CKeyboard::SHIFT|CKeyboard::KEY_B5:
					if(m_ui_context == CTX_TRIG) {
						edit_param(params::SHIFT1);
					}
					break;
				case CKeyboard::SHIFT|CKeyboard::KEY_B6:
					if(m_ui_context == CTX_TRIG) {
						edit_param(params::SHIFT2);
					}
					break;

				//////////////////////////////////////////////////////////////
				// ROW C
				case CKeyboard::KEY_C2:
					break;
				case CKeyboard::KEY_C3:
					edit_param(params::BPM);
					break;


				//////////////////////////////////////////////////////////////
				// ROW D
				case CKeyboard::KEY_D2:
					toggle_chain();
					break;
				case CKeyboard::KEY_D3:
					m_ui_context = CTX_CHORD;
					edit_param(params::CHORD_EDIT);
					break;
				case CKeyboard::KEY_D4:
					m_ui_context = CTX_ARP;
					edit_param(params::SEQUENCE);
					break;
				case CKeyboard::KEY_D5:
					m_ui_context = CTX_TRIG;
					edit_param(params::TRIGS);
					break;

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
		m_param = param;
		if(m_ui) {
			m_ui->ui_done();
		}
		switch(param) {
		case params::SEQUENCE:
			m_chain[m_cur_chain].sequencer.m_ui_show_path = 1;
			m_ui = &m_chain[m_cur_chain].sequencer;
			break;
		case params::TRIGS:
			m_chain[m_cur_chain].sequencer.m_ui_show_path = 0;
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
		case params::VEL:
		case params::ACC_VEL:
		case params::GATE:
		case params::GATE_LONG:
		case params::SHIFT1:
		case params::SHIFT2:
		case params::CHORDMODE:
		case params::CHORDRATE:
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
		case params::VEL:
			return m_chain[m_cur_chain].sequencer.m_cfg.normal_velocity;
		case params::ACC_VEL:
			return m_chain[m_cur_chain].sequencer.m_cfg.accent_velocity;
		case params::GATE:
			return m_chain[m_cur_chain].sequencer.m_cfg.normal_gate;
		case params::GATE_LONG:
			return m_chain[m_cur_chain].sequencer.m_cfg.long_gate;
		case params::SHIFT1:
			return m_chain[m_cur_chain].sequencer.m_cfg.shift_interval_1;
		case params::SHIFT2:
			return m_chain[m_cur_chain].sequencer.m_cfg.shift_interval_2;
		case params::CHORDMODE:
			return m_chord_sequencer.get_edit_mode();
		case params::CHORDRATE:
			return m_chord_sequencer.get_rate();

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
		case params::VEL:
			m_chain[m_cur_chain].sequencer.m_cfg.normal_velocity = value;
			break;
		case params::ACC_VEL:
			m_chain[m_cur_chain].sequencer.m_cfg.accent_velocity = value;
			break;
		case params::GATE:
			m_chain[m_cur_chain].sequencer.m_cfg.normal_gate = value;
			break;
		case params::GATE_LONG:
			m_chain[m_cur_chain].sequencer.m_cfg.long_gate = value;
			break;
		case params::SHIFT1:
			m_chain[m_cur_chain].sequencer.m_cfg.shift_interval_1 = value;
			break;
		case params::SHIFT2:
			m_chain[m_cur_chain].sequencer.m_cfg.shift_interval_2 = value;
			break;
		case params::CHORDMODE:
			m_chord_sequencer.set_edit_mode(value);
			break;
		case params::CHORDRATE:
			m_chord_sequencer.set_rate(value);
		}
	}
};

#endif /* SOURCES_ARPIE_H_ */
