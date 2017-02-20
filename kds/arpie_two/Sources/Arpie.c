#include "Arpie.h"

typedef struct {
	byte tick_flag:1;
	byte clock_count;
} ARPIE_DATA;

static ARPIE_DATA me;

MS_COUNTER g_millis = 0;
int n = 0;

ARP_CONFIG g_arp;
GLOBAL_CONFIG g_cfg;
GLOBAL_STATE g_state;

///////////////////////////////////////////////////////////////
// ISR for once per millisecond tick count
PE_ISR(ISR_SysTick)
{
	++g_millis;
	me.tick_flag = 1;
}





//void arpie_init_seq(ARP_SEQ *seq) {
//	seq->num_notes = 0;
//	seq->num_pitches = 0;
//}

//void arpie_rebuild_seqs() {
//midichord_get(&s_arp_seq_1);
//	memcpy(&s_arp_seq_2, &s_arp_seq_1, sizeof(ARP_SEQ));
//}

void arpie_default_patch() {
	g_arp.chain[0].seq_rate = SEQ_RATE_16;
	g_arp.chain[1].seq_rate = SEQ_RATE_16;

}

void arpie_apply_config()
{
	clockint_on_set_bpm();
}

///////////////////////////////////////////////////////////////
// task which happen once per millisecond
void arpie_tick()
{
	// poll the keyboard
	if((g_millis%10) == 0) {
		byte key = i2c_read(I2C_ADDR_DISP);
		if(key) {
			ui_on_key(key);
			leds_pulse(LEDS_CLOCK);
		}
	}

	// poll the encoder
	char d = encoder_run(g_millis);
	if(d) {
		ui_on_encoder(d);
	}

	// run the UI
	ui_run();

	leds_tick();


	//if(midichord_is_changed()) {
	//	arpie_rebuild_seqs();
	//}

	if(clockint_is_tick()) {
		if(!me.clock_count) {
			leds_pulse(LEDS_CLOCK);
		}
		if(++me.clock_count >= 24) {
			me.clock_count = 0;
		}
	}

}


void arpie_init_chain(byte which) {
	metro_init(which);
}

void arpie_init()
{

	me.tick_flag = 0;
	me.clock_count = 0;

	g_state.bpm = 120;
	g_state.which_chain = 0;
	g_state.hold = 0;

//	arpie_init_seq(&s_arp_seq_1);
//	arpie_init_seq(&s_arp_seq_2);


	encoder_init();
	i2c_init();
	ui_init();
	//midi_in_init();
	//midichord_init();
	clockint_init();
	arpie_init_chain(0);
	arpie_init_chain(1);

	arpie_default_patch();
	arpie_apply_config();

}


void arpie_run()
{
	MIDI_MSG *msg = midi_in_msg();
	if(msg) {
		midichord_on_msg(msg);
	}

	if(me.tick_flag) {
		arpie_tick();
		me.tick_flag = 0;
	}
}

void arpie_set_param(int param, int value) {
	switch(param) {
	case PARAM_SEQ_RATE:
		g_arp.chain[g_state.which_chain].seq_rate = value;
		break;
	case PARAM_BPM:
		g_state.bpm = value;
		clockint_on_set_bpm();
		break;
	}
}
int arpie_get_param(int param) {
	switch(param) {
	case PARAM_BPM:
		return g_state.bpm;
	case PARAM_SEQ_RATE:
		return g_arp.chain[g_state.which_chain].seq_rate;
	}
	return 0;
}
