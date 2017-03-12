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

#ifndef SOURCES_NOTEPLAYER_H_
#define SOURCES_NOTEPLAYER_H_

class CNotePlayer {

	enum {
	  MICROTONAL =(uint16_t)0b1111111111111111,  //no scale
	  CHROMATIC = (uint16_t)0b0000111111111111,  //no scale
	  IONIAN    = (uint16_t)0b0000101011010101,  //diatonic modes
	  DORIAN    = (uint16_t)0b0000101101010110,  //:
	  PHRYGIAN  = (uint16_t)0b0000110101011010,  //:
	  LYDIAN    = (uint16_t)0b0000101010110101,  //:
	  MIXOLYDIAN= (uint16_t)0b0000101011010110,  //:
	  AEOLIAN   = (uint16_t)0b0000101101011010,  //:
	  LOCRIAN   = (uint16_t)0b0000110101101010,  //:
	};
	ARP_NOTE m_note;
	int m_duration;
	CMidiOut *m_midi_out;
public:
	typedef struct {
		byte chan;
		char transpose;
		char oct_shift;
		byte scale_mode;
		uint16_t scale_mask;
		byte scale_root;

	} CONFIG;
	CONFIG m_cfg;

	//////////////////////////////////////////////////////////////////////
	void init(CMidiOut *midi_out, byte chan) {
		m_note.num_pitches = 0;
		m_midi_out = midi_out;
		m_cfg.chan = chan;
		m_cfg.transpose = 0;
		m_cfg.oct_shift = 0;
		set_scale_mode(params::SCALE_CHROMATIC);
		set_scale_root(0);
	}

	//////////////////////////////////////////////////////////////////////
	void set_scale_mode(byte scale_mode) {
		switch(scale_mode) {
		case params::SCALE_IONIAN:
			m_cfg.scale_mask = IONIAN;
			break;
		case params::SCALE_DORIAN:
			m_cfg.scale_mask = DORIAN;
			break;
		case params::SCALE_PHRYGIAN:
			m_cfg.scale_mask = PHRYGIAN;
			break;
		case params::SCALE_LYDIAN:
			m_cfg.scale_mask = LYDIAN;
			break;
		case params::SCALE_MIXOLYDIAN:
			m_cfg.scale_mask = MIXOLYDIAN;
			break;
		case params::SCALE_AEOLIAN:
			m_cfg.scale_mask = AEOLIAN;
			break;
		case params::SCALE_LOCRIAN:
			m_cfg.scale_mask = LOCRIAN;
			break;
		case params::SCALE_MICROTONAL:
			m_cfg.scale_mask = MICROTONAL;
			break;
		case params::SCALE_CHROMATIC:
		default:
			m_cfg.scale_mask = CHROMATIC;
			break;
		}
		m_cfg.scale_mode = scale_mode;
	}
	//////////////////////////////////////////////////////////////////////
	byte get_scale_mode() {
		return m_cfg.scale_mode;
	}
	//////////////////////////////////////////////////////////////////////
	void set_scale_root(byte scale_root) {
		m_cfg.scale_root = scale_root%12;
	}
	//////////////////////////////////////////////////////////////////////
	byte get_scale_root(byte scale_root) {
		return m_cfg.scale_root;
	}
	//////////////////////////////////////////////////////////////////////
	// Stop all playing notes
	void note_stop(ARP_NOTE *other) {
		if(m_note.num_pitches) {
			midi::MSG msg;
			msg.type = midi::MSG_2PARAM;
			msg.status = midi::CH_NOTE_ON | m_cfg.chan;
			msg.param2 = 0;

			// stop the note playing.. we use convention of NOTE ON with zero
			// velocity to turn a note off (allow efficient use of running status)
			for(int i=0; i<m_note.num_pitches; ++i) {

				// if another note is provided, we make sure that any notes we
				// silence should not be playing
				byte allow_pitch = 0;
				if(other) {
					for(int j=0; j<other->num_pitches; ++j) {
						if(other->pitch[j] == m_note.pitch[i]) {
							allow_pitch = 1;
							break;
						}
					}
				}
				if(!allow_pitch) {
					msg.param1 = PITCH_2_MIDI(m_note.pitch[i]);
					m_midi_out->msg(&msg);
				}
			}
			m_note.num_pitches = 0;
		}
	}

	//////////////////////////////////////////////////////////////////////
	// Start a new note playing
	void note_start(ARP_NOTE *note) {

		int i;
		/*
		ARP_NOTE n = *note;
		n.m_num_pitches = 0;
		for(i=0; i<note->m_num_pitches; ++i) {
			if(m_cfg.scale_mask != MICROTONAL) {
				int midi_note = PITCH_2_MIDI(note->m_pitch[i]) - m_cfg.root;
		        while(midi_note<0)
		        	midi_note+=12;
		        if(!(m_cfg.scale_mask & ((int)0x0800>>(midi_note % 12)))) {
		        	switch(m_cfg.force_to_scale_mode) {
		        		case ADJUST_MUTE:
		        			continue;
		        		case ADJUST_FLAT:
		        			--midi_note;
		        			break;
		        		case ADJUST_SHARP:
		        			++midi_note;
		        			break;
		        	}
		        }
				n.m_pitch[n.m_num_pitches++] = MIDI_2_PITCH(midi_note);
			}
			else {
				n.m_pitch[n.m_num_pitches++] = note->m_pitch[i];
			}
		}*/

		int transpose = 256 * (12 * (int)m_cfg.oct_shift + m_cfg.transpose);

		// transpose the incoming note
		ARP_NOTE new_note;
		new_note.duration = note->duration;
		new_note.tie = note->tie;
		new_note.velocity = note->velocity;
		new_note.num_pitches = 0;
		for(int i=0; i<note->num_pitches; ++i) {
			int pitch = note->pitch[i] + transpose;
			if(pitch >= CArpNotes::MIN_PITCH && pitch <= CArpNotes::MAX_PITCH) {
				new_note.pitch[new_note.num_pitches++] = pitch;
			}
		}

		// if the OLD note is not tied then we need to make sure
		// that it has stopped before we start the new note
		if(!m_note.tie) {
			note_stop(NULL);
		}

		// Now send out the NEW note
		midi::MSG msg;
		msg.type = midi::MSG_2PARAM;
		msg.status = midi::CH_NOTE_ON | m_cfg.chan;
		msg.param2 = note->velocity;
		for(int i=0; i<new_note.num_pitches; ++i) {
			// play new notes
			msg.param1 = PITCH_2_MIDI(new_note.pitch[i]);
			m_midi_out->msg(&msg);
		}

		// If the OLD note is tied then we need to stop
		// pitches that are hanging over from that note
		// but which are not in the new note
		if(m_note.tie) {
			note_stop(&new_note);
		}

		// store the NEW note
		m_note = new_note;
		if(new_note.tie) {
			// tied note
			m_duration = 0;
		}
		else {
			// non-tied
			m_duration = new_note.duration;

			// zero duration
			if(!m_duration) {
				note_stop(NULL);
			}
		}

	}

	//////////////////////////////////////////////////////////////////////
	// Called once per millisecond to time note durations
	void on_24ppqn() {
		if(m_duration > 0) {
			if(--m_duration == 0) {
				note_stop(NULL);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////
	// Midi channel accessor
	void set_chan(byte chan) {
		note_stop(NULL); // kill any playing notes before switching channel
		m_cfg.chan = chan&0x0F;
	}
	byte get_chan() {
		return m_cfg.chan;
	}

};



#endif /* SOURCES_NOTEPLAYER_H_ */
