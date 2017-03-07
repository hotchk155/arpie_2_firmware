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

#ifndef SOURCES_ARP_CHORDSEQUENCER_H_
#define SOURCES_ARP_CHORDSEQUENCER_H_

class CChordSequencer : public IUiComponent, public INoteProvider {

	byte m_ui_repaint:1;
	byte m_play_index;
	byte m_edit_index;
	byte m_ver;
	int m_tick_count;
public:
	enum {
		CHORD_MAJ,
		CHORD_MIN,
		CHORD_DOM7,
		CHORD_MAJ7,
		CHORD_MIN7,
		CHORD_AUG,
		CHORD_DIM
	};
	enum {
		MAX_STEPS = 16
	};
	typedef uint16_t CHORD;

	typedef struct {
		CHORD chord[MAX_STEPS];
		byte num_steps;
		int rate;
		byte edit_mode;
	} CONFIG;
	CONFIG m_cfg;

	void init() {
		m_ui_repaint=0;
		memset(&m_cfg.chord,0,sizeof m_cfg.chord);
		m_cfg.num_steps = MAX_STEPS;
		m_play_index = 0;
		m_edit_index = 0;
		m_ver = 0;
		m_tick_count = 0;
		m_cfg.rate = 24 * 8;
		m_cfg.edit_mode = params::CHORD_MODE_NAME;
	}

	void set_edit_mode(byte value) {
		if(value != m_cfg.edit_mode) {
			memset(&m_cfg.chord,0,sizeof m_cfg.chord);
		}
		m_cfg.edit_mode = value;
	}
	byte get_edit_mode() {
		return m_cfg.edit_mode;
	}
	void set_rate(byte value) {
		m_cfg.rate = 24*value;
	}
	byte get_rate() {
		return m_cfg.rate/24;
	}



	virtual void ui_init() {
	}
	virtual void ui_done() {
	}

	virtual void ui_render(uint32_t *raster, uint32_t *highlight) {

		char i;
		char index = m_edit_index;
		for(i=0; i<MAX_STEPS; ++i) {
			if(m_cfg.chord[index]) {
				break;
			}
			if(--index < 0) {
				index = MAX_STEPS-1;
			}
		}
		CHORD chord = m_cfg.chord[index];

		switch(m_cfg.edit_mode) {
			///////////////////////////////////////////////////////////////////
			case params::CHORD_MODE_DIRECT:
			{
				const byte ofs[13] = {22,21,19,18,16,13,12,10,9,7,6,4,1};
				for(i=0; i<13; ++i) {

					byte row = (i==1 || i==3 || i==6 || i==8 || i==10)? 0: 3;
					if(m_cfg.chord[m_edit_index]) {
						raster[row] |= ((uint32_t)3)<<ofs[i];
						raster[row+1] |= ((uint32_t)3)<<ofs[i];
						if(chord & (((CHORD)1)<<i)) {
							highlight[row] |= ((uint32_t)3)<<ofs[i];
							highlight[row+1] |= ((uint32_t)3)<<ofs[i];
						}
					}
					else {
						if(chord & (((CHORD)1)<<i)) {
							raster[row] |= ((uint32_t)3)<<ofs[i];
							raster[row+1] |= ((uint32_t)3)<<ofs[i];
						}
						else {
							highlight[row] |= ((uint32_t)3)<<ofs[i];
							highlight[row+1] |= ((uint32_t)3)<<ofs[i];
						}
					}
				}
				break;
			}
			case params::CHORD_MODE_NAME:
			{
				char name[7];
				char *pos = name;
				if(chord) {
					switch((chord-1) & 0xFF) {
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

					switch(chord>>8) {
					case CHORD_MAJ: break;
					case CHORD_MIN: *pos++ = '~'; break;
					case CHORD_DOM7: *pos++ = '7'; break;
					case CHORD_MAJ7: *pos++ = 'M'; *pos++ = '7'; break;
					case CHORD_MIN7: *pos++ = '~'; *pos++ = '7'; break;
					case CHORD_AUG: *pos++ = '+'; break;
					case CHORD_DIM: *pos++ = '^'; break;
					}
				}
				*pos = 0;
				uint32_t buf[5] = {0};
				CDisplay::instance().format_text(name,buf);
				if(m_cfg.chord[m_edit_index]) {
					memcpy(raster, buf, sizeof(buf));
				}
				else {
					memcpy(highlight, buf, sizeof(buf));
				}
			}
		}

		uint32_t mask = ((uint32_t)1)<<19;
		for(i=0; i<MAX_STEPS; ++i) {
			if(i<m_cfg.num_steps) {
				if(i==m_play_index) {
					raster[6] |= mask;
				}
				if(m_cfg.chord[i]) {
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
		m_ui_repaint = 0;
	}
	virtual byte ui_needs_repaint() {
		return m_ui_repaint;
	}
	virtual byte ui_on_key(byte key_event) {
		if(key_event==(CKeyboard::SHIFT|CKeyboard::KEY_B1)) {
			m_cfg.chord[m_edit_index] = 0;
		}
		else if(m_cfg.edit_mode == params::CHORD_MODE_DIRECT) {

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
			if(m_cfg.chord[m_edit_index] == 0) {
				char index = m_edit_index;
				for(int i=0; i<MAX_STEPS; ++i) {
					if(m_cfg.chord[index]) {
						break;
					}
					if(--index < 0) {
						index = MAX_STEPS-1;
					}
				}
				m_cfg.chord[m_edit_index] = m_cfg.chord[index];
			}
			m_cfg.chord[m_edit_index] ^= mask;
		}
		else if(m_cfg.edit_mode == params::CHORD_MODE_NAME) {

			CHORD chord = m_cfg.chord[m_edit_index];
			switch(key_event) {
			case CKeyboard::KEY_B1: chord = (chord & 0xFF00)|1; break;
			case CKeyboard::KEY_A1: chord = (chord & 0xFF00)|2; break;
			case CKeyboard::KEY_B2: chord = (chord & 0xFF00)|3; break;
			case CKeyboard::KEY_A2: chord = (chord & 0xFF00)|4; break;
			case CKeyboard::KEY_B3: chord = (chord & 0xFF00)|5; break;
			case CKeyboard::KEY_B4: chord = (chord & 0xFF00)|6; break;
			case CKeyboard::KEY_A3: chord = (chord & 0xFF00)|7; break;
			case CKeyboard::KEY_B5: chord = (chord & 0xFF00)|8; break;
			case CKeyboard::KEY_A4: chord = (chord & 0xFF00)|9; break;
			case CKeyboard::KEY_B6: chord = (chord & 0xFF00)|10; break;
			case CKeyboard::KEY_A5: chord = (chord & 0xFF00)|11; break;
			case CKeyboard::KEY_B7: chord = (chord & 0xFF00)|12; break;
			default:
				return 0;
			}
			if(m_cfg.chord[m_edit_index]) {
				chord &= 0xFF;
				if((byte)m_cfg.chord[m_edit_index] == chord) {
					if((m_cfg.chord[m_edit_index]>>8) < CHORD_DIM) {
						chord |= ((m_cfg.chord[m_edit_index]>>8)+1) << 8;
					}
				}
			}
			m_cfg.chord[m_edit_index] = chord;
		}
		m_ui_repaint = 1;
		return 1;
	}
	virtual void ui_on_encoder(char delta, byte modifiers) {
		if(delta > 0) {
			if(modifiers & CKeyboard::SHIFT_MODIFIER) {
				if(m_cfg.num_steps < MAX_STEPS) {
					++m_cfg.num_steps;
				}
				m_edit_index = m_cfg.num_steps-1;
				m_ui_repaint = 1;

			}
			else if(m_edit_index < m_cfg.num_steps - 1) {
				++m_edit_index;
				m_ui_repaint = 1;
			}
		}
		else if(delta < 0) {
			if(modifiers & CKeyboard::SHIFT_MODIFIER) {
				if(m_cfg.num_steps >  1) {
					--m_cfg.num_steps;
				}
				m_edit_index = m_cfg.num_steps-1;
				m_ui_repaint = 1;
			}
			else if(m_edit_index > 0) {
				--m_edit_index;
				m_ui_repaint = 1;
			}
		}
	}

	byte get_notes_version() {
		return m_ver;
	}

	byte get_notes(CArpNotes *notes) {
		notes->init();

		char index = m_play_index;
		for(int i=0; i<MAX_STEPS; ++i) {
			if(m_cfg.chord[index]) {
				break;
			}
			if(--index < 0) {
				index = MAX_STEPS-1;
			}
		}

		if(m_cfg.edit_mode == params::CHORD_MODE_DIRECT) {
			for(byte i=0; i<13; ++i) {
				if(m_cfg.chord[index] & (((uint32_t)1)<<i)) {
					notes->insert_midi_note(48 + i);
				}
			}
		}
		else if(m_cfg.edit_mode == params::CHORD_MODE_NAME) {
			if(m_cfg.chord[index]) {
				byte root = (m_cfg.chord[index] & 0xFF) + 47;
				byte chord_type =m_cfg.chord[index]>>8;
				notes->insert_midi_note(root);
				if(chord_type == CHORD_MIN || chord_type == CHORD_MIN7 || chord_type == CHORD_DIM) {
					notes->insert_midi_note(root + 3);
				}
				else {
					notes->insert_midi_note(root + 4);
				}

				if(chord_type == CHORD_AUG) {
					notes->insert_midi_note(root + 8);
				}
				else if(chord_type == CHORD_AUG) {
					notes->insert_midi_note(root + 6);
				}
				else {
					notes->insert_midi_note(root + 7);
				}

				if(chord_type == CHORD_DOM7 || chord_type == CHORD_MIN7) {
					notes->insert_midi_note(root + 10);
				}
				else if(chord_type == CHORD_MAJ7) {
					notes->insert_midi_note(root + 11);
				}
			}
		}
		return m_ver;
	}
	///////////////////////////////////////////////////////////////////////////////
	void on_24ppqn() {

		if(!m_tick_count) {
			++m_ver;
			m_ui_repaint = 1;
		}

		// divide 24pqn ticks by step rate
		if(++m_tick_count >= m_cfg.rate) {
			m_tick_count = 0;
			if(++m_play_index >= m_cfg.num_steps) {
				m_play_index=0;
			}
		}
	}
};



#endif /* SOURCES_ARP_CHORDSEQUENCER_H_ */
