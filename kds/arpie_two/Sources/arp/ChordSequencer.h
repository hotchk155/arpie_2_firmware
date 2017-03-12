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

// Class to implement a chord sequencer
class CChordSequencer : public INoteProvider {
friend class CUiChordEditor;

	byte m_ui_repaint:1;
	byte m_play_index;
	byte m_ver;
	int m_tick_count;


public:
	enum {
		CHORD_MAJ = 0b0000010010001,
		CHORD_MIN = 0b0000010001001,
		CHORD_DOM7 = 0b0010010010001,
		CHORD_MIN7 = 0b0010010001001,
		CHORD_MAJ7 = 0b0100010010001,
		CHORD_DIM = 0b0000001001001,
		CHORD_AUG = 0b0000100010001
	};

	typedef struct {
		char shift;
		uint32_t chord;
	} CHORD;

	static const int MAX_STEPS = 16;

	typedef struct {
		CHORD chord[MAX_STEPS];
		byte num_steps;
		int rate;
	} CONFIG;
	CONFIG m_cfg;

	void init() {
		m_ui_repaint=0;
		memset(&m_cfg.chord,0,sizeof m_cfg.chord);
		m_cfg.num_steps = MAX_STEPS;
		m_play_index = 0;
		m_ver = 0;
		m_tick_count = 0;
		m_cfg.rate = 24 * 8;
	}


	void get_chord_at(char index, CHORD *result) {
		for(int i=0; i<MAX_STEPS; ++i) {
			if(m_cfg.chord[index].chord) {
				break;
			}
			if(--index < 0) {
				index = MAX_STEPS-1;
			}
		}
		*result = m_cfg.chord[index];
	}

	void set_rate(byte value) {
		m_cfg.rate = 24*value;
	}
	byte get_rate() {
		return m_cfg.rate/24;
	}



	byte get_notes_version() {
		return m_ver;
	}

	byte get_notes(CArpNotes *notes) {
		notes->init();

		CHORD chord;
		get_chord_at(m_play_index, &chord);
		for(byte i=0; i<32; ++i) {
			if(chord.chord & (((uint32_t)1)<<i)) {
				notes->insert_midi_note(48 + i + chord.shift);
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
