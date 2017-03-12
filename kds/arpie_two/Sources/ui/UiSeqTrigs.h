/*
 * UiSeqTrigs.h
 *
 *  Created on: 7 Mar 2017
 *      Author: Jason
 */

#ifndef SOURCES_ARP_UISEQTRIGS_H_
#define SOURCES_ARP_UISEQTRIGS_H_

class CUiSeqTrigs : public IUiComponent {
	CSequencer *m_p;
public:
	CUiSeqTrigs() {
		m_p = NULL;
	}
	void set_sequencer(CSequencer *sequencer) {
		m_p = sequencer;
	}

	void ui_init() {

	}
	void ui_done() {

	}
	void ui_render(uint32_t *raster, uint32_t *highlight) {
		byte which_trig = 0;

		// two rows...
		for(byte row = 0; row < 2; row ++) {
			 //                    76543210765432107654321076543210
			uint32_t both_bits = 0b00000000110000000000000000000000;
			uint32_t left_bit =  0b00000000100000000000000000000000;
			uint32_t right_bit = 0b00000000010000000000000000000000;

			//... of 8 columns
			for(byte col = 0; col < 8; ++col) {

				if(which_trig < m_p->m_cfg.num_trigs) {

					if(m_p->m_cfg.trigs[which_trig].enabled) { // enabled trigger

						// normal colour
						raster[1] |= both_bits;
						raster[2] |= both_bits;

						if(m_p->m_cfg.trigs[which_trig].accent) {
							raster[0] |= left_bit;
						}
						if(m_p->m_cfg.trigs[which_trig].shift) {
							raster[0] |= right_bit;
						}
						if(m_p->m_cfg.trigs[which_trig].glide) {
							raster[2] |= right_bit>>1;
						}

						// highlight the current play trigger
						if(which_trig == m_p->m_trig_index) {
							highlight[1] |= left_bit;
						}
						// highlight the current edit trigger
						if(which_trig == m_p->m_edit_trig) {
							highlight[2] |= both_bits;
						}
					}
					else { // disabled trigger

						// "bright black" shows its disabled
						highlight[1] |= both_bits;
						highlight[2] |= both_bits;

						// highlight the current play trigger
						if(which_trig == m_p->m_trig_index) {
							raster[1] |= left_bit;
						}
						// highlight the current edit trigger
						if(which_trig == m_p->m_edit_trig) {
							raster[2] |= both_bits;
						}
					}
				}

				both_bits >>= 3;
				left_bit >>= 3;
				right_bit >>= 3;

				++which_trig;
			}
			raster+=4;
			highlight+=4;
		}
		m_p->m_ui_repaint = 0;
	}
	byte ui_needs_repaint() {
		return m_p->m_ui_repaint;
	}
	byte ui_on_key(byte key_event) {
		switch(key_event) {
		case CKeyboard::KEY_B1:
			m_p->m_cfg.trigs[m_p->m_edit_trig].enabled = !m_p->m_cfg.trigs[m_p->m_edit_trig].enabled;
			m_p->m_ui_repaint = 1;
			return 1;
		case CKeyboard::KEY_B2:
			m_p->m_cfg.trigs[m_p->m_edit_trig].accent = !m_p->m_cfg.trigs[m_p->m_edit_trig].accent;
			m_p->m_ui_repaint = 1;
			return 1;

		case CKeyboard::KEY_B3:
			if(m_p->m_cfg.trigs[m_p->m_edit_trig].glide == CSequencer::GLIDE_TIED) {
				m_p->m_cfg.trigs[m_p->m_edit_trig].glide = CSequencer::GLIDE_NONE;
			}
			else {
				m_p->m_cfg.trigs[m_p->m_edit_trig].glide = CSequencer::GLIDE_TIED;
			}
			m_p->m_ui_repaint = 1;
			return 1;

		case CKeyboard::KEY_B4:
			if(m_p->m_cfg.trigs[m_p->m_edit_trig].glide == CSequencer::GLIDE_LONG) {
				m_p->m_cfg.trigs[m_p->m_edit_trig].glide = CSequencer::GLIDE_NONE;
			}
			else {
				m_p->m_cfg.trigs[m_p->m_edit_trig].glide = CSequencer::GLIDE_LONG;
			}
			m_p->m_ui_repaint = 1;
			return 1;

		case CKeyboard::KEY_B5:
			if(m_p->m_cfg.trigs[m_p->m_edit_trig].shift == CSequencer::SHIFT_1) {
				m_p->m_cfg.trigs[m_p->m_edit_trig].shift = CSequencer::SHIFT_NONE;
			}
			else {
				m_p->m_cfg.trigs[m_p->m_edit_trig].shift = CSequencer::SHIFT_1;
			}
			m_p->m_ui_repaint = 1;
			return 1;

		case CKeyboard::KEY_B6:
			if(m_p->m_cfg.trigs[m_p->m_edit_trig].shift == CSequencer::SHIFT_2) {
				m_p->m_cfg.trigs[m_p->m_edit_trig].shift = CSequencer::SHIFT_NONE;
			}
			else {
				m_p->m_cfg.trigs[m_p->m_edit_trig].shift = CSequencer::SHIFT_2;
			}
			m_p->m_ui_repaint = 1;
			return 1;

		case CKeyboard::KEY_B7:
			if(m_p->m_cfg.trigs[m_p->m_edit_trig].shift == CSequencer::SHIFT_CHORD) {
				m_p->m_cfg.trigs[m_p->m_edit_trig].shift = CSequencer::SHIFT_NONE;
			}
			else {
				m_p->m_cfg.trigs[m_p->m_edit_trig].shift = CSequencer::SHIFT_CHORD;
			}
			m_p->m_ui_repaint = 1;
			return 1;
		}
		return 0;
	}

	void ui_on_encoder(char delta, byte modifiers) {
		if(delta > 0) {
			if(modifiers & CKeyboard::SHIFT_MODIFIER) {
				if(m_p->m_cfg.num_trigs < CSequencer::MAX_TRIG) {
					++m_p->m_cfg.num_trigs;
				}
				m_p->m_edit_trig = m_p->m_cfg.num_trigs-1;
				m_p->m_ui_repaint = 1;

			}
			else if(m_p->m_edit_trig < m_p->m_cfg.num_trigs - 1) {
				++m_p->m_edit_trig;
				m_p->m_ui_repaint = 1;
			}
		}
		else if(delta < 0) {
			if(modifiers & CKeyboard::SHIFT_MODIFIER) {
				if(m_p->m_cfg.num_trigs >  1) {
					--m_p->m_cfg.num_trigs;
				}
				m_p->m_edit_trig = m_p->m_cfg.num_trigs-1;
				m_p->m_ui_repaint = 1;
			}
			else if(m_p->m_edit_trig > 0) {
				--m_p->m_edit_trig;
				m_p->m_ui_repaint = 1;
			}
		}
	}
};




#endif /* SOURCES_ARP_UISEQTRIGS_H_ */
