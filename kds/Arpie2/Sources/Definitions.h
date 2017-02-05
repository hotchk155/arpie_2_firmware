/*
 * Definitions.h
 *
 *  Created on: 2 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_DEFINITIONS_H_
#define SOURCES_DEFINITIONS_H_


typedef uint32_t MILLISECONDS_TYPE;

class CArpNote;
class CArpNotes;

/*
 * Listener - gets informed about something
 * Processor -
 */

/*
 * This interface to be implemented by classes that
 * process raw MIDI data
 *
 */
class IMidiMessageListener {
public:
	virtual void on_midi_msg(byte status, byte param1, byte param2, byte num_params) = 0;
	virtual void on_midi_sysex(byte data) = 0;
	virtual void on_midi_realtime(byte data) = 0;
};

class IClockProcessor {
public:
	virtual void on_clock_reset() = 0;
	virtual void on_clock_start() = 0;
	virtual void on_clock_stop() = 0;
	virtual void on_clock_tick() = 0;
};

class ISequencer {
public:
	virtual void step() = 0;
};

class IArpProcessor
{
public:
	virtual void process(CArpNotes *notes) = 0;
};

/*
 * A note consumer receives a single "note" (which can actually represent a chord)
 * and processes that note, passing it on or playing it as needed. A note
 * consumer only supports handling one such note at a time
 */
class IArpNoteProcessor {
public:
	virtual void start_note(CArpNote *note) = 0;
	virtual void stop_note() = 0;
};



#endif /* SOURCES_DEFINITIONS_H_ */
