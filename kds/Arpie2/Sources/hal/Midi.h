/*
 * Midi.h
 *
 *  Created on: 4 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_MIDI_H_
#define SOURCES_MIDI_H_

namespace midi {
enum {
	CH_NOTE_OFF = 0x80,
	CH_NOTE_ON = 0x90,
	CH_POLY_PRESS = 0xA0,
	CH_CTRL_CHANGE = 0xB0,
	CH_PROG_CHANGE = 0xC0,
	CH_CHAN_PRESS = 0xD0,
	CH_PITCH_BEND = 0xE0,

	SC_SYSEX_BEGIN = 0xF0,
	SC_MTC_QUARTER_FRAME = 0xF1,
	SC_SPP = 0xF2,
	SC_SONG_SELECT = 0xF3,
	SC_TUNE_REQUEST = 0xF6,
	SC_SYSEX_END = 0xF7,

	RT_CLOCK_TICK = 0xF8,
	RT_CLOCK_START = 0xFA,
	RT_CLOCK_CONTINUE = 0xFB,
	RT_CLOCK_STOP = 0xFC,
	RT_ACTIVE_SENSING = 0xFE,
	RT_RESET = 0xFF,

	CC_ALL_SOUND_OFF = 120,
	CC_ALL_RESET = 121,
	CC_LOCAL_CONTROL = 122,
	CC_ALL_NOTES_OFF = 123,
	CC_OMNI_OFF = 124,
	CC_OMNI_ON = 125,
	CC_MONO_ON = 126,
	CC_MONO_OFF = 127,
};

enum {
	CHAN1 = 0x00,
	OMNI = 0xFF
};

}

#endif /* SOURCES_MIDI_H_ */
