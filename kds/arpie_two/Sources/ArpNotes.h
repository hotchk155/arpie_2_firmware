/*
 * ArpSeq.h
 *
 *  Created on: 19 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_INCLUDE_ARPSEQ_H_
#define SOURCES_INCLUDE_ARPSEQ_H_


typedef uint16_t ARP_PITCH;

typedef struct {
	byte index;
	byte size;
} ARP_NOTE;

enum {
	MAX_ARP_PITCHES = 100,
	MAX_ARP_NOTES = 100
};
typedef struct {

	ARP_PITCH pitch[MAX_ARP_PITCHES];
	ARP_NOTE note[MAX_ARP_NOTES];
	byte num_pitches;
	byte num_notes;
} ARP_NOTES;


#define MIDI_2_PITCH(midi) (((ARP_PITCH)(midi))<<8)
#define PITCH_2_MIDI(midi) ((byte)(((midi)&~LAST_PITCH_FLAG)>>8))




#endif /* SOURCES_INCLUDE_ARPSEQ_H_ */
