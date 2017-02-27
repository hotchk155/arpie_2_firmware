/*
 * UiSequencer.h
 *
 *  Created on: 21 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_SEQUENCER_H_
#define SOURCES_SEQUENCER_H_

class CSequencer : public IUiComponent {
friend class CSequencerUiPath;

	byte m_ui_repaint:1;
	byte m_edit_step;
	byte m_flags;


	CArpNotes *m_seq;
	CNotePlayer *m_note_player;


	//byte m_seq_index;	// the note within the sequence
	byte m_trig_index;  	// the step within the trigs

	uint32_t m_index;

	byte m_tick_count;
public:
	enum {
		MAX_STEP = 16
	};
	enum {
		TRIG_ENABLED,
		TRIG_ACCENT,
		TRIG_GLIDE,
		TRIG_TRANSPOSE
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
		byte rate;
		byte trigs[MAX_STEP];
		byte num_trigs;
	} CONFIG;
	CONFIG m_cfg;


	CSequencer(CArpNotes *seq, CNotePlayer *note_player) {
		m_seq = seq;
		m_note_player = note_player;
	}

	void init() {
		m_cfg.rate = RATE_16;
		memset(m_cfg.trigs, 0, sizeof(m_cfg.trigs));
		m_cfg.num_trigs = 10;//MAX_STEP;
//		m_step = 0;
//		m_note_pending = 0;
//		m_seq_index = 0;
		m_trig_index = 0;
		m_index = 0;
		m_edit_step = 0;
		//m_flags = 0;
		m_ui_repaint = 0;
		prepare_path();
	}

	void on_sequence_change() {
		prepare_path();
	}
	/*
	inline byte is_note_pending() {
		return m_note_pending;
	}
	void get_note(ARP_NOTE *note) {
		m_seq->get_note (m_step,0,note);
		m_note_pending = 0;
	}
	*/
	void on_24ppqn() {
		if(!m_tick_count && m_seq->len()) {
			ARP_NOTE note;
			m_seq->get_note(m_index % m_seq->len(),0,&note);
			note.m_ticks = m_cfg.rate;
			m_note_player->note_start(&note);
			m_trig_index = m_index % m_cfg.num_trigs;
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

	/*
	void ui_render(uint32_t *raster, uint32_t *highlight) {

		byte which_step = 0;
		for(byte row = 0; row < 2; row ++) {
			 //                    76543210765432107654321076543210
			uint32_t both_bits = 0b00000000110000000000000000000000;
			uint32_t left_bit = 0b10000000000000000000000000000000;
			uint32_t right_bit = 0b01000000000000000000000000000000;

			for(byte col = 0; col < 8; ++col) {

				if(which_step < m_cfg.num_steps) {
					raster[1] |= both_bits;
					raster[2] |= both_bits;

					if(which_step == m_edit_step) {
						highlight[2] |= both_bits;
					}
				}

				both_bits >>= 3;
				left_bit >>= 3;
				right_bit >>= 3;

				++which_step;
			}
			raster+=3;
			highlight+=3;
		}
		m_ui_repaint = 0;
	}
	byte ui_needs_repaint() {
		return m_ui_repaint;
	}
	void ui_on_key(byte key, byte modifiers) {
	}

	void ui_on_encoder(char delta, byte modifiers) {
		if(delta > 0) {
			if(modifiers & CKeyboard::SHIFT_MODIFIER) {
				if(m_cfg.num_steps < SEQ_STEP_MAX) {
					++m_cfg.num_steps;
				}
				m_edit_step = m_cfg.num_steps-1;
				m_ui_repaint = 1;

			}
			else if(m_edit_step < m_cfg.num_steps - 1) {
				++m_edit_step;
				m_ui_repaint = 1;
			}
		}
		else if(delta < 0) {
			if(modifiers & CKeyboard::SHIFT_MODIFIER) {
				if(m_cfg.num_steps >  1) {
					--m_cfg.num_steps;
				}
				m_edit_step = m_cfg.num_steps-1;
				m_ui_repaint = 1;
			}
			else if(m_edit_step > 0) {
				--m_edit_step;
				m_ui_repaint = 1;
			}
		}
	}*/
	enum {
		NUM_COLS = 16
	};

	// the stored path for display - each element in the
	// array is the row number for the path in that column
	char m_path[NUM_COLS];

	void ui_render(uint32_t *raster, uint32_t *highlight) {


		uint32_t raster_mask = 0x080000;

		// vertical shift scrolls the display down if the active
		// step would be off the top
		char yorg = 5*((4+m_path[m_trig_index]-6)/5);
		if(yorg < 0) {
			yorg = 0;
		}

		for(byte x=0; x<NUM_COLS; ++x) {

			char row = 6-m_path[x]+yorg;
			if(row >= 0 && row < 7) {
				raster[row] |= raster_mask;
				if(x == m_trig_index) {
					highlight[row] |= raster_mask;
				}
			}

			if(x < m_cfg.num_trigs) {
				if(m_cfg.trigs[x] & TRIG_ENABLED) {
					raster[7] |= raster_mask;
				}
				else {
					highlight[7] |= raster_mask;
				}
			}
			raster_mask >>= 1;
		}
	}

	void prepare_path() {


		byte i,j,k;

		// The first point in the path is always aligned with the
		// first of the 16 triggers
		// 0         1         2         3         4         5
		// 012345678901234567890123456789012345678901234567890123456789
		// ABCDEFGHIJKLMNOPQRSTUVWXYZ <-sequence
		// XXXXXXXXXXXXXXXX <-trigs
		//
		// ABCDEFGHIJKLMNOP
		// QRSTUVWXYZABCDEF
		// GHIJKLMNOPQRSTUV
		// WXYZABCDEFGHIJKL


		memset(m_path,-1,sizeof(m_path));
		// we need at least one note before we can display anything
		if(m_seq->len() && m_cfg.num_trigs) {

			// get the position in the sequence of the first column
			byte base_index = (m_cfg.num_trigs * (m_index / m_cfg.num_trigs)) % m_seq->len();
			byte seq_index = base_index;

			// build a list of all the unique notes we need to display,
			// sorted into decending order
			byte num_unique_notes = 0;
			byte unique_notes[NUM_COLS];
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
			for(i=0; i<NUM_COLS; ++i ) {

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
	byte ui_needs_repaint() {
		byte r  = m_ui_repaint;
		m_ui_repaint = 0;
		return r;
	}
	void ui_on_key(byte key, byte modifiers) {
	}
	void ui_on_encoder(char delta, byte modifiers) {
	}
};

#endif /* SOURCES_SEQUENCER_H_ */
