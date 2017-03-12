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

class CSequencer  {
friend class CUiSeqPath;
friend class CUiSeqTrigs;

	byte m_ui_repaint:1;
	byte m_seq_changed:1;
	byte m_edit_trig;
	CArpNotes *m_seq;
	CNotePlayer *m_note_player;


	//byte m_seq_index;	// the note within the sequence
	byte m_trig_index;  	// the step within the trigs

	uint32_t m_index;

	byte m_tick_count;


	enum {
		SHIFT_NONE,
		SHIFT_1,
		SHIFT_2,
		SHIFT_CHORD
	};

	enum {
		GLIDE_NONE,
		GLIDE_LONG,
		GLIDE_TIED
	};

public:
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
		byte glide:2;
		byte shift:2;
	} TRIG;
	typedef struct {
		TRIG trigs[MAX_TRIG];
		byte num_trigs;
		byte rate;
		byte normal_velocity;
		byte accent_velocity;
		byte normal_gate;
		byte long_gate;
		char shift_interval_1;
		char shift_interval_2;
	} CONFIG;
	CONFIG m_cfg;

	CSequencer() {
		m_seq = NULL;
		m_note_player = NULL;
		m_edit_trig = 0;
	}

	void init(CArpNotes *seq, CNotePlayer *note_player) {
		m_seq = seq;
		m_note_player = note_player;
		m_cfg.rate = RATE_16;
		m_cfg.normal_velocity = 100;
		m_cfg.accent_velocity = 127;
		m_cfg.normal_gate = 100;
		m_cfg.long_gate = 127;
		m_cfg.shift_interval_1 = 12;
		m_cfg.shift_interval_2 = 24;
		for(int i=0; i<MAX_TRIG;++i) {
			m_cfg.trigs[i].enabled = 1;
		}
		m_cfg.num_trigs = MAX_TRIG;

		m_trig_index = 0;
		m_index = 0;
		m_ui_repaint = 0;

	}

	///////////////////////////////////////////////////////////////////////////////
	void on_sequence_change() {
		m_seq_changed = 1;
	}

	///////////////////////////////////////////////////////////////////////////////
	void on_24ppqn() {
		if(!m_tick_count && m_seq->len()) {
			m_trig_index = m_index % m_cfg.num_trigs;
			int seq_index = m_index % m_seq->len();
			if(m_cfg.trigs[m_trig_index].enabled) {
				ARP_NOTE note;
				switch(m_cfg.trigs[m_trig_index].shift ) {
				case SHIFT_1:
					m_seq->get_note(seq_index,m_cfg.shift_interval_1,&note);
					break;
				case SHIFT_2:
					m_seq->get_note(seq_index,m_cfg.shift_interval_2,&note);
					break;
				case SHIFT_CHORD:
					m_seq->get_chord(&note);
					break;
				case SHIFT_NONE:
					m_seq->get_note(seq_index,0,&note);
					break;
				}


				if(m_cfg.trigs[m_trig_index].accent) {
					note.velocity = m_cfg.accent_velocity;
				}
				else {
					note.velocity = m_cfg.normal_velocity;
				}

				switch(m_cfg.trigs[m_trig_index].glide) {
					case GLIDE_LONG:
						note.duration = (m_cfg.long_gate * m_cfg.rate)/127;
						break;
					case GLIDE_TIED:
						note.tie = 1;
						break;
					case GLIDE_NONE:
					default:
						note.duration = (m_cfg.normal_gate * m_cfg.rate)/127;
						break;
				};

				m_note_player->note_start(&note);
			}
			if(m_trig_index == 0) {
				m_seq_changed = 1;
			}
			m_ui_repaint = 1;
			++m_index;
		}

		// divide 24pqn ticks by step rate
		if(++m_tick_count >= m_cfg.rate) {
			m_tick_count = 0;
		}
	}


};

#endif /* SOURCES_SEQUENCER_H_ */
