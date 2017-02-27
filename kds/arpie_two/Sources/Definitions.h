/*
 * Definitions.h
 *
 *  Created on: 24 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_DEFINITIONS_H_
#define SOURCES_DEFINITIONS_H_



/*
 * This interface to be implemented by classes that
 * process raw MIDI data
 *
 */
/*
class IMidiMessageListener {
public:
	virtual void on_midi_msg(midi::MSG& msg) = 0;
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

class IArpSequenceConsumer
{
public:
	virtual void process_sequence(CArpNotes *notes) = 0;
};

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
*/

#endif /* SOURCES_DEFINITIONS_H_ */
