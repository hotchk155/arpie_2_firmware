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

#ifndef SOURCES_ARP_UICHORDEDITOR_H_
#define SOURCES_ARP_UICHORDEDITOR_H_


class CUiChordEditor : public IUiComponent {
protected:
	CChordSequencer& m_seq;
	byte m_edit_index:4;
	byte m_chord_name:1;
	byte m_edit_octave:2;
public:
	//////////////////////////////////////////////////////////////////////////////////
	CUiChordEditor(CChordSequencer& seq) : m_seq(seq) {
		m_edit_index = 0;
		m_edit_octave = 0;
	}

	//////////////////////////////////////////////////////////////////////////////////
	virtual void ui_init() {
	}

	//////////////////////////////////////////////////////////////////////////////////
	virtual void ui_done() {
	}

	//////////////////////////////////////////////////////////////////////////////////
	virtual void ui_render(uint32_t *raster, uint32_t *highlight) {

		// get the chord which applies at the current edit step
		CChordSequencer::CHORD chord;
		m_seq.get_chord_at(m_edit_index, &chord);

		// if we are in "chord name" mode we will try to identify the chord type
		const char *chord_name = NULL;
		if(m_chord_name) {
			switch(chord.chord) {
			case CChordSequencer::CHORD_MAJ: chord_name = ""; break;
			case CChordSequencer::CHORD_MIN: chord_name = "~"; break;
			case CChordSequencer::CHORD_DOM7: chord_name = "7"; break;
			case CChordSequencer::CHORD_MAJ7: chord_name = "M7"; break;
			case CChordSequencer::CHORD_MIN7: chord_name = "~7"; break;
			case CChordSequencer::CHORD_AUG: chord_name = "+"; break;
			case CChordSequencer::CHORD_DIM: chord_name = "^"; break;
			case 0:
				chord_name = "CHORD";
			}
		}
		if(chord_name && chord.shift >= 0 && chord.shift < 12) {
			// build and display the full chord name
			char text[6];
			char *pos = text;
			if(chord.chord) {
				switch(chord.shift) {
				case 0: *pos++ = 'C'; break;
				case 1: *pos++ = 'C'; *pos++ = '#'; break;
				case 2: *pos++ = 'D'; break;
				case 3: *pos++ = 'D';  *pos++ = '#'; break;
				case 4: *pos++ = 'E'; break;
				case 5: *pos++ = 'F'; break;
				case 6: *pos++ = 'F'; *pos++ = '#'; break;
				case 7: *pos++ = 'G'; break;
				case 8: *pos++ = 'G'; *pos++ = '#'; break;
				case 9: *pos++ = 'A'; break;
				case 10: *pos++ = 'A'; *pos++ = '#'; break;
				case 11: *pos++ = 'B'; break;
				}
			}
			while(*chord_name) {
				*pos++ = *chord_name++;
			}
			*pos = 0;
			uint32_t buf[5] = {0};
			CDisplay::instance().format_text(text,buf);
			if(m_seq.m_cfg.chord[m_edit_index].chord) {
				memcpy(raster, buf, sizeof(buf));
			}
			else {
				memcpy(highlight, buf, sizeof(buf));
			}
		}
		else {
			// display as notes
			const byte ofs[13] = {22,21,19,18,16,13,12,10,9,7,6,4,1};
			char shift = (int)m_edit_octave * 12-chord.shift;
			byte max_index = (m_edit_octave == 2)? 8:13;
			for(byte i=0; i<max_index; ++i) {
				uint32_t mask = (((uint32_t)1)<<(i+shift));
				byte row = (i==1 || i==3 || i==6 || i==8 || i==10)? 0: 3;
				if(m_seq.m_cfg.chord[m_edit_index].chord) {
					raster[row] |= ((uint32_t)3)<<ofs[i];
					raster[row+1] |= ((uint32_t)3)<<ofs[i];
					if(chord.chord & mask) {
						highlight[row] |= ((uint32_t)3)<<ofs[i];
						highlight[row+1] |= ((uint32_t)3)<<ofs[i];
					}
				}
				else {
					if(chord.chord & mask) {
						raster[row] |= ((uint32_t)3)<<ofs[i];
						raster[row+1] |= ((uint32_t)3)<<ofs[i];
					}
					else {
						highlight[row] |= ((uint32_t)3)<<ofs[i];
						highlight[row+1] |= ((uint32_t)3)<<ofs[i];
					}
				}
			}
		}

		// show the sequencer bar at bottom of display
		uint32_t mask = ((uint32_t)1)<<19;
		for(int i=0; i<CChordSequencer::MAX_STEPS; ++i) {
			if(i<m_seq.m_cfg.num_steps) {
				if(i==m_seq.m_play_index) {
					raster[6] |= mask;
				}
				if(m_seq.m_cfg.chord[i].chord) {
					raster[7] |= mask;
					if(i==m_edit_index) {
						highlight[7] |= mask;
					}
				}
				else {
					highlight[7] |= mask;
					if(i==m_edit_index) {
						raster[7] |= mask;
					}
				}
			}
			mask>>=1;
		}

		// octave indicator
		raster[7] |= 0b111;
		highlight[7] |= 0b100>>m_edit_octave;
		m_seq.m_ui_repaint = 0;
	}

	//////////////////////////////////////////////////////////////////////////////////
	virtual byte ui_needs_repaint() {
		return m_seq.m_ui_repaint;
	}

	//////////////////////////////////////////////////////////////////////////////////
	virtual byte ui_on_key(byte key_event) {
		if(key_event==(CKeyboard::SHIFT|CKeyboard::KEY_B1)) {
			m_seq.m_cfg.chord[m_edit_index].chord = 0;
			m_seq.m_cfg.chord[m_edit_index].shift = 0;
			m_seq.m_ui_repaint = 1;
		}
		else if(key_event==(CKeyboard::SHIFT|CKeyboard::KEY_B2)) {
			m_chord_name = !m_chord_name;
			m_seq.m_ui_repaint = 1;
		}
		else if(key_event==(CKeyboard::SHIFT|CKeyboard::KEY_B6)) {
			m_edit_octave = 0;
			m_seq.m_ui_repaint = 1;
		}
		else if(key_event==(CKeyboard::SHIFT|CKeyboard::KEY_B7)) {
			m_edit_octave = 1;
			m_seq.m_ui_repaint = 1;
		}
		else if(key_event==(CKeyboard::SHIFT|CKeyboard::KEY_B8)) {
			m_edit_octave = 2;
			m_seq.m_ui_repaint = 1;
		}
		else if(m_chord_name) {
			CChordSequencer::CHORD chord = m_seq.m_cfg.chord[m_edit_index];
			switch(key_event) {
			case CKeyboard::KEY_B1: chord.shift = 0; break;
			case CKeyboard::KEY_A1: chord.shift = 1; break;
			case CKeyboard::KEY_B2: chord.shift = 2; break;
			case CKeyboard::KEY_A2: chord.shift = 3; break;
			case CKeyboard::KEY_B3: chord.shift = 4; break;
			case CKeyboard::KEY_B4: chord.shift = 5; break;
			case CKeyboard::KEY_A3: chord.shift = 6; break;
			case CKeyboard::KEY_B5: chord.shift = 7; break;
			case CKeyboard::KEY_A4: chord.shift = 8; break;
			case CKeyboard::KEY_B6: chord.shift = 9; break;
			case CKeyboard::KEY_A5: chord.shift = 10; break;
			case CKeyboard::KEY_B7: chord.shift = 11; break;
			default:
				return 0;
			}
			switch(chord.chord) {
			case CChordSequencer::CHORD_MAJ: chord.chord = CChordSequencer::CHORD_MIN; break;
			case CChordSequencer::CHORD_MIN: chord.chord = CChordSequencer::CHORD_DOM7; break;
			case CChordSequencer::CHORD_DOM7: chord.chord = CChordSequencer::CHORD_MAJ7; break;
			case CChordSequencer::CHORD_MAJ7: chord.chord = CChordSequencer::CHORD_MIN7; break;
			case CChordSequencer::CHORD_MIN7: chord.chord = CChordSequencer::CHORD_AUG; break;
			case CChordSequencer::CHORD_AUG: chord.chord = CChordSequencer::CHORD_DIM; break;
			case CChordSequencer::CHORD_DIM:
			default:
				chord.chord = CChordSequencer::CHORD_MAJ;
				break;
			}
			m_seq.m_cfg.chord[m_edit_index] = chord;
			m_seq.m_ui_repaint = 1;
		}
		else {

			uint32_t mask;
			switch(key_event) {
			case CKeyboard::KEY_B1: mask=(((uint32_t)1)<<0); break;
			case CKeyboard::KEY_A1: mask=(((uint32_t)1)<<1); break;
			case CKeyboard::KEY_B2: mask=(((uint32_t)1)<<2); break;
			case CKeyboard::KEY_A2: mask=(((uint32_t)1)<<3); break;
			case CKeyboard::KEY_B3: mask=(((uint32_t)1)<<4); break;
			case CKeyboard::KEY_B4: mask=(((uint32_t)1)<<5); break;
			case CKeyboard::KEY_A3: mask=(((uint32_t)1)<<6); break;
			case CKeyboard::KEY_B5: mask=(((uint32_t)1)<<7); break;
			case CKeyboard::KEY_A4: mask=(((uint32_t)1)<<8); break;
			case CKeyboard::KEY_B6: mask=(((uint32_t)1)<<9); break;
			case CKeyboard::KEY_A5: mask=(((uint32_t)1)<<10); break;
			case CKeyboard::KEY_B7: mask=(((uint32_t)1)<<11); break;
			case CKeyboard::KEY_B8: mask=(((uint32_t)1)<<12); break;
			default:
				return 0;
			}

			// if there is no chord trig at this step, we will insert one
			// based chord playing at the step
			if(m_seq.m_cfg.chord[m_edit_index].chord == 0) {
				m_seq.get_chord_at(m_edit_index, &m_seq.m_cfg.chord[m_edit_index]);
			}

			// if the chord has been entered using the chord name input, it
			// may be a shifted pattern - we need to convert it so it can
			// can be directly edited
			if(m_seq.m_cfg.chord[m_edit_index].shift) {
				m_seq.m_cfg.chord[m_edit_index].chord <<= m_seq.m_cfg.chord[m_edit_index].shift;
				m_seq.m_cfg.chord[m_edit_index].shift = 0;
			}
			mask <<= (12*m_edit_octave);
			m_seq.m_cfg.chord[m_edit_index].chord ^= mask;
			m_seq.m_ui_repaint = 1;
		}
		return 1;
	}

	//////////////////////////////////////////////////////////////////////////////////
	virtual void ui_on_encoder(char delta, byte modifiers) {
		if(delta > 0) {
			if(modifiers & CKeyboard::SHIFT_MODIFIER) {
				if(m_seq.m_cfg.num_steps < CChordSequencer::MAX_STEPS) {
					++m_seq.m_cfg.num_steps;
				}
				m_edit_index = m_seq.m_cfg.num_steps-1;
				m_seq.m_ui_repaint = 1;

			}
			else if(m_edit_index < m_seq.m_cfg.num_steps - 1) {
				++m_edit_index;
				m_seq.m_ui_repaint = 1;
			}
		}
		else if(delta < 0) {
			if(modifiers & CKeyboard::SHIFT_MODIFIER) {
				if(m_seq.m_cfg.num_steps >  1) {
					--m_seq.m_cfg.num_steps;
				}
				m_edit_index = m_seq.m_cfg.num_steps-1;
				m_seq.m_ui_repaint = 1;
			}
			else if(m_edit_index > 0) {
				--m_edit_index;
				m_seq.m_ui_repaint = 1;
			}
		}
	}
};

#endif /* SOURCES_ARP_UICHORDEDITOR_H_ */
