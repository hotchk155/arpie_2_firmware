/*
 * Definitions.h
 *
 *  Created on: 2 Feb 2017
 *      Author: Jason
 */

//TODO: check volatile declarations on interrupt callable variables

#ifndef SOURCES_DEFINITIONS_H_
#define SOURCES_DEFINITIONS_H_


typedef uint32_t MILLISECONDS_TYPE;

class CArpNote;
class CArpNotes;

enum {
	I2C_ADDR_DISP = 123
};

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

class IClockListener {
public:
	virtual void on_clock_start() = 0;
	virtual void on_clock_stop() = 0;
	virtual void on_clock_continue() = 0;
	virtual void on_clock_tick() = 0;
};

class ISequencer {
public:
	virtual void sequencer_start() = 0;
	virtual void sequencer_stop() = 0;
	virtual void sequencer_continue() = 0;
	virtual void sequencer_step() = 0;
};

/*
 * Sequence consumer takes a sequence of notes and takes ownership of it
 */
class IArpSequenceConsumer
{
public:
	virtual void process_sequence(CArpNotes *notes) = 0;
};

/*
 * A note consumer receives a single "note" (which can actually represent a chord)
 * and processes that note, passing it on or playing it as needed. A note
 * consumer only supports handling one such note at a time
 */
class IArpNoteConsumer {
public:
	virtual void note_start(CArpNote *note) = 0;
	virtual void note_stop() = 0;
};


class IArpEvents {
public:
	enum {
		BEAT,
		TX_DATA,
		RX_DATA
	};
	virtual void on_tick() = 0;
	virtual void on_uart0_rx_complete(byte status) = 0;
	virtual void on_i2c_read_complete(byte is_ok) = 0;
	virtual void on_i2c_write_complete(byte is_ok) = 0;
	virtual void on_event(int event, int param) = 0;
};
extern IArpEvents *g_arp;



#endif /* SOURCES_DEFINITIONS_H_ */
