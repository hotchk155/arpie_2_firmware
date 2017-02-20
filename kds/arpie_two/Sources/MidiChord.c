/*
 * ChordCatcherMidi.c
 *
 *  Created on: 19 Feb 2017
 *      Author: Jason
 */
#include "Arpie.h"


enum {
	MIDICHORD_MAX_NOTES = 12
};

// Runtime state information that is not saved with a patch
typedef struct {
	byte note;
	byte vel:7;
	byte is_held:1;
} MIDICHORD_NOTE;

typedef struct {
	MIDICHORD_NOTE notes[MIDICHORD_MAX_NOTES];
	byte len;
	byte held_notes;
	byte is_changed:1;
} MIDICHORD_DATA;


static MIDICHORD_DATA me;

void midichord_remove_note(int index) {
	for(int j = index; j<me.len-1; ++j) {
		me.notes[j] = me.notes[j+1];
	}
	--me.len;
}


void midichord_init()
{
	me.len = 0;
	me.held_notes = 0;
	me.is_changed = 0;
}

void midichord_on_msg(MIDI_MSG *msg) {
	if(msg->type != MIDI_MSG_2PARAM) {
		return;
	}
	if((g_cfg.midi_in_chan != CHAN_OMNI) && (msg->status & 0x0F) != g_cfg.midi_in_chan) {
		return;
	}

	switch(msg->status & 0xF0) {
	case MIDI_CH_NOTE_ON:
		if(msg->param2) {
			if(!me.held_notes) {
				me.len = 0;
			}
			int i;
			for(i=0; i<me.len; ++i) {
				if(me.notes[i].note == msg->param1) {
					me.notes[i].vel = msg->param2;
					if(!me.notes[i].is_held) {
						me.notes[i].is_held = 1;
						++me.held_notes;
						break;
					}
				}
			}
			if(i==me.len && me.len < MIDICHORD_MAX_NOTES) {
				me.notes[me.len].note = msg->param1;
				me.notes[me.len].vel = msg->param2;
				me.notes[me.len].is_held = 1;
				++me.len;
				++me.held_notes;
			}
			me.is_changed = 1;
			break;
		}
		// else fall through (zero note on velocity = note off)
	case MIDI_CH_NOTE_OFF:
		for(int i=0; i<me.len; ++i) {
			if(me.notes[i].note == msg->param1) {
				if(g_state.hold) {
					if(me.notes[i].is_held) {
						me.notes[i].is_held = 0;
						--me.held_notes;
					}
				}
				else {
					midichord_remove_note(i);
					--me.held_notes;
					--i;
					me.is_changed = 1;
				}
			}
		}
		break;
	}
}


byte midichord_is_changed() {
	return me.is_changed;
}

//TODO - when patch loaded
void midichord_set(ARP_NOTES *result) {
}

void midichord_get(ARP_NOTES *result) {
	result->num_pitches = 0;
	result->num_notes = 0;
	for(int i=0; i<me.len; ++i) {
		result->pitch[result->num_pitches] = MIDI_2_PITCH(me.notes[i].note);
		result->note[result->num_notes].index = result->num_pitches;
		result->note[result->num_notes].size = 1;
		result->num_pitches++;
		result->num_notes++;
	}
	me.is_changed = 0;
}

void midichord_on_hold_off() {
	for(int i=0; i<me.len;) {
		if(!me.notes[i].is_held) {
			midichord_remove_note(i);
			me.is_changed = 1;
		}
		else {
			++i;
		}
	}
}
