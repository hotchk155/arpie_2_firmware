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

#include "Arpie.h"


using namespace params;


static const char* c_arp_type[ARPTYPE_ENUM_MAX] = {"UP", "DOWN", "UP-DN", "DN-UP", "RANDOM", "CHORD"};
static const char* c_oct_span[ARPTYPE_ENUM_MAX] = {"NONE", "2XOCT", "3XOCT", "4XOCT"};
static const char* c_seq_rate[SEQ_RATE_ENUM_MAX] = {"WHOLE", "1/2D", "1/2", "1/4D", "1/2T", "1/4", "1/8D", "1/4T", "1/8", "1/16D", "1/8T", "1/16", "1/16T", "1/32"};
static const char* c_seq_mutate[SEQ_MUTATE_ENUM_MAX] = {"NONE", "INS HI", "INS LO", "3F1B", "4F2B"};
static const char* c_chain[CHAIN_ENUM_MAX] = {"ARP A", "ARP B"};

/*
static const char* c_gate_len[GATE_ENUM_MAX] = {"OFF", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "FULL", "TIE"};
static const char* c_midi_chan[CHAN_ENUM_MAX] = {"CH1", "CH2", "CH3", "CH4", "CH5", "CH6", "CH7", "CH8", "CH9", "CH10", "CH11", "CH12", "CH13", "CH14", "CH15", "CH16", "OMNI"};
static const char* c_seq_rate[SEQ_RATE_ENUM_MAX] = {"WHOLE", "1/2D", "1/2", "1/4D", "1/2T", "1/4", "1/8D", "1/4T", "1/8", "1/16D", "1/8T", "1/16", "1/16T", "1/32"};

*/

IParams *params::g_params;
static const INFO dummy = { "OOPS",0,0};
static const INFO seq_rate = { "RATE", 0, SEQ_RATE_ENUM_MAX - 1, c_seq_rate};
static const INFO arp_type = { "TYPE", 0, params::ARPTYPE_ENUM_MAX - 1, c_arp_type};
static const INFO oct_span = { "SPAN", 0, 4, c_oct_span};
static const INFO seq_mutate = { "MUTATE", 0, params::SEQ_MUTATE_ENUM_MAX-1, c_seq_mutate};
static const INFO bpm = { "BPM", 30, 255};
static const INFO chain = { NULL, 0, CHAIN_ENUM_MAX-1, c_chain};


const INFO *params::info(int param) {
	switch(param) {
	case params::BPM: return &bpm;
	case params::ARP_TYPE: return &arp_type;
	case params::OCT_SPAN: return &oct_span;
	case params::SEQ_RATE: return &seq_rate;
	case params::SEQ_MUTATE: return &seq_mutate;
	case params::CHAIN: return &chain;
	}
	return &dummy;
}
/*
static const PARAM_INFO c_param[PARAM_ENUM_MAX] = {
		{ "RATE", c_seq_rate, SEQ_RATE_ENUM_MAX-1 },
		{ "GATE", c_gate_len, GATE_ENUM_MAX-1 },
		{ "IN CH", c_midi_chan, CHAN_ENUM_MAX-1 },
		{ "OUT CH", c_midi_chan, CHAN_ENUM_MAX-2 },
		{ "TYPE", c_arp_type, ARPTYPE_ENUM_MAX-1, PFLAG_DEFER },
		{ "BPM", NULL, 250, PFLAG_NUMBER, 30 }
};
*/
