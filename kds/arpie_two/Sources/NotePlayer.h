/*
 * NotePlayer.h
 *
 *  Created on: 24 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_NOTEPLAYER_H_
#define SOURCES_NOTEPLAYER_H_

class CNotePlayer {

	ARP_NOTE m_note;
	int m_duration;
	CMidiOut *m_midi_out;
public:
	typedef struct {
		byte chan;
		byte normal_velocity;
		byte accent_velocity;
	} CONFIG;
	CONFIG m_cfg;

	CNotePlayer(CMidiOut *midi_out) {
		m_note.m_num_pitches = 0;
		m_midi_out = midi_out;
		m_cfg.chan = midi::CHAN_1;
		m_cfg.normal_velocity = 100;
		m_cfg.accent_velocity = 127;
	}

	//////////////////////////////////////////////////////////////////////
	// Stop all playing notes
	void note_stop() {
		if(m_note.m_num_pitches) {
			midi::MSG msg;
			msg.type = midi::MSG_2PARAM;
			msg.status = midi::CH_NOTE_ON | m_cfg.chan;
			msg.param2 = 0;

			// stop the note playing.. we use convention of NOTE ON with zero
			// velocity to turn a note off (allow efficient use of running status)
			for(int i=0; i<m_note.m_num_pitches; ++i) {
				msg.param1 = PITCH_2_MIDI(m_note.m_pitch[i]);
				m_midi_out->msg(&msg);
			}
			m_note.m_num_pitches = 0;
		}
	}

	//////////////////////////////////////////////////////////////////////
	// Start a new note playing
	void note_start(ARP_NOTE *note) {
		midi::MSG msg;
		msg.type = midi::MSG_2PARAM;
		msg.status = midi::CH_NOTE_ON | m_cfg.chan;
		msg.param2 = note->m_accent ? m_cfg.accent_velocity : m_cfg.normal_velocity;
		if(m_note.m_num_pitches && !m_note.m_tie) {
			note_stop();
		}
		for(int i=0; i<note->m_num_pitches; ++i) {
			msg.param1 = PITCH_2_MIDI(note->m_pitch[i]);
			m_midi_out->msg(&msg);
		}
		if(m_note.m_num_pitches) {
			note_stop();
		}
		m_note = *note;
		m_duration = m_note.m_ticks;
	}

	//////////////////////////////////////////////////////////////////////
	// Called once per millisecond to time note durations
	void on_24ppqn() {
		if(m_duration > 0) {
			if(--m_duration == 0) {
				note_stop();
			}
		}
	}

	//////////////////////////////////////////////////////////////////////
	// Midi channel accessor
	void set_chan(byte chan) {
		note_stop(); // kill any playing notes before switching channel
		m_cfg.chan = chan&0x0F;
	}
	byte get_chan() {
		return m_cfg.chan;
	}

};



#endif /* SOURCES_NOTEPLAYER_H_ */
