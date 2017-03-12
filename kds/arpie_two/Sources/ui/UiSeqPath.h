/*
 * UiSeqPath.h
 *
 *  Created on: 7 Mar 2017
 *      Author: Jason
 */

#ifndef SOURCES_ARP_UISEQPATH_H_
#define SOURCES_ARP_UISEQPATH_H_


class CUiSeqPath : public IUiComponent {
	CSequencer *m_p;
	enum {
		PATH_LEN = 16
	};

	// the stored path for display - each element in the
	// array is the row number for the path in that column
	char m_path[PATH_LEN];

	///////////////////////////////////////////////////////////////////////////////
	// method to prepare the "path" for the path mode UI
	void prepare_path() {


		byte i,j,k;
		memset(m_path,-1,sizeof(m_path));
		// we need at least one note before we can display anything
		if(m_p->m_seq->len() && m_p->m_cfg.num_trigs) {

			// get the position in the sequence of the first column
			byte base_index = (m_p->m_cfg.num_trigs * (m_p->m_index / m_p->m_cfg.num_trigs)) % m_p->m_seq->len();
			byte seq_index = base_index;

			// build a list of all the unique notes we need to display,
			// sorted into decending order
			byte num_unique_notes = 0;
			byte unique_notes[PATH_LEN];
			for(i=0; i<m_p->m_cfg.num_trigs; ++i) {
				byte note = m_p->m_seq->midi_note(seq_index);

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
				if(++seq_index >= m_p->m_seq->len()) {
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
				byte note = m_p->m_seq->midi_note(seq_index);
				for(j=0; j<num_unique_notes;++j) {
					if(note == unique_notes[j]) {
						m_path[i] = j;
						break;
					}
				}

				if(++seq_index >= m_p->m_seq->len()) {
					seq_index = 0;
				}

			}
		}
	}

public:
	CUiSeqPath() {
		m_p=NULL;
	}
	void set_sequencer(CSequencer *sequencer) {
		m_p = sequencer;
	}

	void ui_init() {
		m_p->m_seq_changed = 1;
		m_p->m_ui_repaint = 1;
	}
	void ui_done() {

	}
	void ui_render(uint32_t *raster, uint32_t *highlight) {

		if(m_p->m_seq_changed) {
			prepare_path();
			m_p->m_seq_changed = 0;
		}
		uint32_t raster_mask = 0x080000;

		// vertical shift scrolls the display down if the active
		// step would be off the top
		char yorg = 5*((4+m_path[m_p->m_trig_index]-6)/5);
		if(yorg < 0) {
			yorg = 0;
		}

		for(byte x=0; x<PATH_LEN; ++x) {

			char row = 6-m_path[x]+yorg;
			if(row >= 0 && row < 7) {
				if(x == m_p->m_trig_index) {
					highlight[row] |= raster_mask;
					raster[row] |= raster_mask;
				}
				else if(m_p->m_cfg.trigs[x].enabled) {
					raster[row] |= raster_mask;
				}
				else {
					highlight[row] |= raster_mask;

				}
			}

			if(x < m_p->m_cfg.num_trigs) {
				if(m_p->m_cfg.trigs[x].enabled) {
					raster[7] |= raster_mask;
					if(x == m_p->m_edit_trig) {
						highlight[7] |= raster_mask;
					}
				}
				else {
					highlight[7] |= raster_mask;
					if(x == m_p->m_edit_trig) {
						raster[7] |= raster_mask;
					}
				}
			}
			raster_mask >>= 1;
		}
		m_p->m_ui_repaint = 0;
	}
	byte ui_needs_repaint() {
		return m_p->m_ui_repaint;
	}
	byte ui_on_key(byte key_event) {
		switch(key_event) {
		case CKeyboard::SHIFT|CKeyboard::KEY_B1:
			m_p->m_cfg.trigs[m_p->m_edit_trig].enabled = !m_p->m_cfg.trigs[m_p->m_edit_trig].enabled;
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
					prepare_path();
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
					prepare_path();
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


#endif /* SOURCES_ARP_UISEQPATH_H_ */
