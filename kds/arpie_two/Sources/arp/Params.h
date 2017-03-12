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

#ifndef SOURCES_INCLUDE_PARAMETERS_H_
#define SOURCES_INCLUDE_PARAMETERS_H_

class IParams
{
public:
	virtual int get_param(int param) = 0;
	virtual void set_param(int param, int value) = 0;
};


namespace params
{
enum {
	DEFER_UPDATE = 0x01,
};
typedef struct {
	const char *name;
	const short min_value;
	const short max_value;
	const char **values;
} INFO;

extern IParams *g_params;
inline int get(int param) {
	return g_params->get_param(param);
}
inline void set(int param, int value) {
	g_params->set_param(param, value);
}
const INFO *info(int param);


enum {
	TRIGS_UI,
	PATH_UI,
	CHORD_UI,

	SEQ_RATE,
	MIDI_IN_CHAN,
	MIDI_OUT_CHAN,
	ARP_TYPE,
	BPM,
	OCT_SPAN,
	SEQ_MUTATE,

	VEL,
	ACC_VEL,
	GATE,
	GATE_LONG,
	SHIFT1,
	SHIFT2,
	CHORDRATE,

	OCT_SHIFT,
	TRANSPOSE,

	ENUM_MAX
};


enum {
	ARPTYPE_UP = 0,
	ARPTYPE_DN,
	ARPTYPE_UPDN,
	ARPTYPE_DNUP,
	ARPTYPE_RND,
	ARPTYPE_CHORD,
	ARPTYPE_ENUM_MAX,
};


enum {
	GATE_0 = 0,
	GATE_10,
	GATE_20,
	GATE_30,
	GATE_40,
	GATE_50,
	GATE_60,
	GATE_70,
	GATE_80,
	GATE_90,
	GATE_FULL,
	GATE_TIE,
	GATE_ENUM_MAX
};

enum {
	CHAN_1 = 0,
	CHAN_2,
	CHAN_3,
	CHAN_4,
	CHAN_5,
	CHAN_6,
	CHAN_7,
	CHAN_8,
	CHAN_9,
	CHAN_10,
	CHAN_11,
	CHAN_12,
	CHAN_13,
	CHAN_14,
	CHAN_15,
	CHAN_16,
	CHAN_OMNI,
	CHAN_ENUM_MAX
};


enum
{
  SEQ_RATE_1,
  SEQ_RATE_2D,
  SEQ_RATE_2,
  SEQ_RATE_4D,
  SEQ_RATE_2T,
  SEQ_RATE_4,
  SEQ_RATE_8D,
  SEQ_RATE_4T,
  SEQ_RATE_8,
  SEQ_RATE_16D,
  SEQ_RATE_8T,
  SEQ_RATE_16,
  SEQ_RATE_16T,
  SEQ_RATE_32,
  SEQ_RATE_ENUM_MAX
};

enum {
	SEQ_STEP_PLAY = 0x01,
	SEQ_STEP_ACCENT = 0x02,
	SEQ_STEP_FULL_GATE = 0x04,
	SEQ_STEP_TIED_GATE = 0x08,
	SEQ_STEP_OCT_UP = 0x10,
	SEQ_STEP_CHORD = 0x20
};

enum {
	SEQ_MUTATE_NONE,
	SEQ_MUTATE_INSERT_LOWEST,
	SEQ_MUTATE_INSERT_HIGHEST,
	SEQ_MUTATE_3F1B,
	SEQ_MUTATE_4F2B,
	SEQ_MUTATE_ENUM_MAX
};

enum {
	SCALE_CHROMATIC,
	SCALE_IONIAN,
	SCALE_DORIAN,
	SCALE_PHRYGIAN,
	SCALE_LYDIAN,
	SCALE_MIXOLYDIAN,
	SCALE_AEOLIAN,
	SCALE_LOCRIAN,
	SCALE_MICROTONAL,
	SCALE_ENUM_MAX
};

enum {
	SCALE_ADJUST_MUTE,
	SCALE_ADJUST_FLAT,
	SCALE_ADJUST_SHARP,
	SCALE_ADJUST_ENUM_MAX
};


}; // namespace params
#endif /* SOURCES_INCLUDE_PARAMETERS_H_ */
