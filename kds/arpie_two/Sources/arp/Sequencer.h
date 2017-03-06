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

#ifndef SOURCES_SEQUENCER_H_
#define SOURCES_SEQUENCER_H_

class CSequencer : public IUiComponent {
friend class CSequencerUiPath;

	byte m_ui_show_path:1;
	byte m_ui_repaint:1;
	byte m_edit_trig;
//	byte m_flags;


	CArpNotes *m_seq;
	CNotePlayer *m_note_player;


	//byte m_seq_index;	// the note within the sequence
	byte m_trig_index;  	// the step within the trigs

	uint32_t m_index;

	byte m_tick_count;

	enum {
		PATH_LEN = 16
	};

	// the stored path for display - each element in the
	// array is the row number for the path in that column
	char m_path[PATH_LEN];

public:
	enum {
		UI_TRIGS,
		UI_PATH
	};
	enum {
		MAX_TRIG = 16
	};
	enum
	{
	  RATE_1    = 96,
	  RATE_2D   = 72,
	  RATE_2    = 48,
	  RATE_4D   = 36,
	  RATE_2T   = 32,
	  RATE_4    = 24,
	  RATE_8D   = 18,
	  RATE_4T   = 16,
	  RATE_8    = 12,
	  RATE_16D  = 9,
	  RATE_8T   = 8,
	  RATE_16   = 6,
	  RATE_16T  = 4,
	  RATE_32   = 3
	};

	typedef struct {
		byte enabled:1;
		byte accent:1;
		byte glide:1;
		byte octave:1;
		byte chord:1;
	} TRIG;
	typedef struct {
		TRIG trigs[MAX_TRIG];
		byte num_trigs;
		byte rate;
	} CONFIG;
	CONFIG m_cfg;


	CSequencer() {
		m_seq = NULL;
		m_note_player = NULL;
	}

	void init(CArpNotes *seq, CNotePlayer *note_player) {
		m_seq = seq;
		m_note_player = note_player;
		m_cfg.rate = RATE_16;
		memset(m_cfg.trigs, 0, sizeof(m_cfg.trigs));
		for(int i=0; i<MAX_TRIG;++i) {
			m_cfg.trigs[i].enabled = 1;
		}
		m_cfg.num_trigs = MAX_TRIG;
		m_trig_index = 0;
		m_index = 0;
		m_edit_trig = 0;
		m_ui_repaint = 0;
		m_ui_show_path = 0;
		prepare_path();
	}

	///////////////////////////////////////////////////////////////////////////////
	void on_sequence_change() {
		prepare_path();
	}

	///////////////////////////////////////////////////////////////////////////////
	void on_24ppqn() {
		if(!m_tick_count && m_seq->len()) {
			m_trig_index = m_index % m_cfg.num_trigs;
			if(m_cfg.trigs[m_trig_index].enabled) {
				ARP_NOTE note;
				m_seq->get_note(m_index % m_seq->len(),0,&note);
				note.m_accent = m_cfg.trigs[m_trig_index].accent;
				note.m_ticks = m_cfg.rate;
				note.m_tie = m_cfg.trigs[m_trig_index].glide;
				m_note_player->note_start(&note);
			}
			if(m_trig_index == 0) {
				prepare_path();
			}
			m_ui_repaint = 1;
			++m_index;
		}

		// divide 24pqn ticks by step rate
		if(++m_tick_count >= m_cfg.rate) {
			m_tick_count = 0;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	void ui_init() {
		prepare_path();
	}
	///////////////////////////////////////////////////////////////////////////////
	void ui_done() {
	}

	///////////////////////////////////////////////////////////////////////////////
	// method to prepare the "path" for the path mode UI
	void ui_render(uint32_t *raster, uint32_t *highlight) {

		///////////////////////////////////////////////////////////////////////////////
		// PATH MODE
		if(m_ui_show_path) {

			uint32_t raster_mask = 0x080000;

			// vertical shift scrolls the display down if the active
			// step would be off the top
			char yorg = 5*((4+m_path[m_trig_index]-6)/5);
			if(yorg < 0) {
				yorg = 0;
			}

			for(byte x=0; x<PATH_LEN; ++x) {

				char row = 6-m_path[x]+yorg;
				if(row >= 0 && row < 7) {
					if(x == m_trig_index) {
						highlight[row] |= raster_mask;
						raster[row] |= raster_mask;
					}
					else if(m_cfg.trigs[x].enabled) {
						raster[row] |= raster_mask;
					}
					else {
						highlight[row] |= raster_mask;

					}
				}

				if(x < m_cfg.num_trigs) {
					if(m_cfg.trigs[x].enabled) {
						raster[7] |= raster_mask;
						if(x == m_edit_trig) {
							highlight[7] |= raster_mask;
						}
					}
					else {
						highlight[7] |= raster_mask;
						if(x == m_edit_trig) {
							raster[7] |= raster_mask;
						}
					}
				}
				raster_mask >>= 1;
			}
		}
		///////////////////////////////////////////////////////////////////////////////
		// TRIGS MODE
		else {
			byte which_trig = 0;

			// two rows...
			for(byte row = 0; row < 2; row ++) {
				 //                    76543210765432107654321076543210
				uint32_t both_bits = 0b00000000110000000000000000000000;
				uint32_t left_bit =  0b00000000100000000000000000000000;
				uint32_t right_bit = 0b00000000010000000000000000000000;

				//... of 8 columns
				for(byte col = 0; col < 8; ++col) {

					if(which_trig < m_cfg.num_trigs) {

						if(m_cfg.trigs[which_trig].enabled) { // enabled trigger

							// normal colour
							raster[1] |= both_bits;
							raster[2] |= both_bits;

							if(m_cfg.trigs[which_trig].accent) {
								raster[0] |= left_bit;
							}
							if(m_cfg.trigs[which_trig].octave||m_cfg.trigs[which_trig].chord) {
								raster[0] |= right_bit;
							}
							if(m_cfg.trigs[which_trig].glide) {
								raster[2] |= right_bit>>1;
							}

							// highlight the current play trigger
							if(which_trig == m_trig_index) {
								highlight[1] |= left_bit;
							}
							// highlight the current edit trigger
							if(which_trig == m_edit_trig) {
								highlight[2] |= both_bits;
							}
						}
						else { // disabled trigger

							// "bright black" shows its disabled
							highlight[1] |= both_bits;
							highlight[2] |= both_bits;

							// highlight the current play trigger
							if(which_trig == m_trig_index) {
								raster[1] |= left_bit;
							}
							// highlight the current edit trigger
							if(which_trig == m_edit_trig) {
								raster[2] |= both_bits;
							}
						}
					}

					both_bits >>= 3;
					left_bit >>= 3;
					right_bit >>= 3;

					++which_trig;
				}
				raster+=3;
				highlight+=3;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	byte ui_needs_repaint() {
		byte r  = m_ui_repaint;
		m_ui_repaint = 0;
		return r;
	}

	///////////////////////////////////////////////////////////////////////////////
	byte ui_on_key(byte key, byte modifiers) {
		switch(key) {
		case CKeyboard::CMD_TRIG_ENABLE:
			m_cfg.trigs[m_edit_trig].enabled = !m_cfg.trigs[m_edit_trig].enabled;
			m_ui_repaint = 1;
			return 1;
		case CKeyboard::CMD_TRIG_ACCENT:
			m_cfg.trigs[m_edit_trig].accent = !m_cfg.trigs[m_edit_trig].accent;
			m_ui_repaint = 1;
			return 1;
		case CKeyboard::CMD_TRIG_GLIDE:
			m_cfg.trigs[m_edit_trig].glide = !m_cfg.trigs[m_edit_trig].glide;
			m_ui_repaint = 1;
			return 1;
		case CKeyboard::CMD_TRIG_OCTAVE:
			m_cfg.trigs[m_edit_trig].octave = !m_cfg.trigs[m_edit_trig].octave;
			m_ui_repaint = 1;
			return 1;
		case CKeyboard::CMD_TRIG_CHORD:
			m_cfg.trigs[m_edit_trig].chord= !m_cfg.trigs[m_edit_trig].chord;
			m_ui_repaint = 1;
			return 1;
		case  CKeyboard::CMD_TRIGS:
			m_ui_show_path = !m_ui_show_path;
			m_ui_repaint = 1;
			return 1;
		}
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////
	void ui_on_encoder(char delta, byte modifiers) {
		if(delta > 0) {
			if(modifiers & CKeyboard::SHIFT_MODIFIER) {
				if(m_cfg.num_trigs < MAX_TRIG) {
					++m_cfg.num_trigs;
					prepare_path();
				}
				m_edit_trig = m_cfg.num_trigs-1;
				m_ui_repaint = 1;

			}
			else if(m_edit_trig < m_cfg.num_trigs - 1) {
				++m_edit_trig;
				m_ui_repaint = 1;
			}
		}
		else if(delta < 0) {
			if(modifiers & CKeyboard::SHIFT_MODIFIER) {
				if(m_cfg.num_trigs >  1) {
					--m_cfg.num_trigs;
					prepare_path();
				}
				m_edit_trig = m_cfg.num_trigs-1;
				m_ui_repaint = 1;
			}
			else if(m_edit_trig > 0) {
				--m_edit_trig;
				m_ui_repaint = 1;
			}
		}
	}

protected:
	///////////////////////////////////////////////////////////////////////////////
	// method to prepare the "path" for the path mode UI
	void prepare_path() {


		byte i,j,k;
		memset(m_path,-1,sizeof(m_path));
		// we need at least one note before we can display anything
		if(m_seq->len() && m_cfg.num_trigs) {

			// get the position in the sequence of the first column
			byte base_index = (m_cfg.num_trigs * (m_index / m_cfg.num_trigs)) % m_seq->len();
			byte seq_index = base_index;

			// build a list of all the unique notes we need to display,
			// sorted into decending order
			byte num_unique_notes = 0;
			byte unique_notes[PATH_LEN];
			for(i=0; i<m_cfg.num_trigs; ++i) {
				byte note = m_seq->midi_note(seq_index);

				for(j=0; j<num_unique_notes; ++j) {
					if(note <= unique_notes[j]) {
						break;
					}
				}
				if(j==num_unique_notes) {
					// highest note value - add to end of the list
					unique_notes[num_unique_notes++] = note;
				}
				else if(note < unique_notes[j]) {
					// need to insert the note, shifting others up
					++num_unique_notes;
					for(k=num_unique_notes; k>j; --k) {
						unique_notes[k] = unique_notes[k-1];
					}
					unique_notes[j] = note;
				}
				if(++seq_index >= m_seq->len()) {
					seq_index = 0;
				}
				if(seq_index == base_index) {
					// we've wrapped back to start of sequence so no point
					// processing the same notes again
					break;
				}
			}

			// map notes into the column
			seq_index = base_index;
			for(i=0; i<PATH_LEN; ++i ) {

				// map midi note to row
				byte note = m_seq->midi_note(seq_index);
				for(j=0; j<num_unique_notes;++j) {
					if(note == unique_notes[j]) {
						m_path[i] = j;
						break;
					}
				}

				if(++seq_index >= m_seq->len()) {
					seq_index = 0;
				}

			}
		}
	}

};

#endif /* SOURCES_SEQUENCER_H_ */
