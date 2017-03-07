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

#ifndef SOURCES_UIPARAMEDIT_H_
#define SOURCES_UIPARAMEDIT_H_


class CUIParamEdit : public IUiComponent {

protected:
	byte m_param;
	const params::INFO *m_info;
	byte m_needs_repaint:1;
	byte m_edit_in_progress:1;
public:
	CUIParamEdit() {
		m_param = 0;
		m_info = NULL;
	}

public:
	void set_param(byte param) {
		m_param = param;
		m_info = params::info(param);
		m_edit_in_progress = 0;
	}
	void ui_init() {
	}
	void ui_done() {

	}
	void ui_render(uint32_t *raster, uint32_t *highlight) {
		int value = params::get(m_param);
		if(m_edit_in_progress || !m_info->name) {
			if(m_info->values) {
				CDisplay::instance().format_text(m_info->values[value], raster);
			}
			else if(m_info->min_value<0){
				CDisplay::instance().format_bipolar2digit(value, raster);
			}
			else {
				CDisplay::instance().format_3digit(value, raster);
			}
			format_scrollbar(value, raster, highlight);
		}
		else {
			CDisplay::instance().format_text(m_info->name, raster);
		}
		format_scrollbar(value, raster, highlight);
	}
	byte ui_needs_repaint() {
		byte r = m_needs_repaint;
		m_needs_repaint = 0;
		return r;
	}
	byte ui_on_key(byte key_event) {
		if(key_event == CKeyboard::KEY_D1) {
			// press shift key to see current value
			m_needs_repaint = 1;
			m_edit_in_progress = 1;
			return 1;
		}
		return 0;
	}
	void ui_on_encoder(char delta, byte modifiers) {
		int value = params::get(m_param);
		int new_value = value;
		if(modifiers & CKeyboard::SHIFT_MODIFIER) {
			new_value += 10 * delta;
		}
		else {
			new_value += delta;
		}
		if(new_value < m_info->min_value) {
			new_value = m_info->min_value;
		}
		else if(new_value > m_info->max_value) {
			new_value = m_info->max_value;
		}
		if(new_value != value) {
			params::set(m_param, new_value);
			m_needs_repaint = 1;
			m_edit_in_progress = 1;
		}
	}

	void format_scrollbar(int value, uint32_t *raster, uint32_t *highlight) {
		byte num_values = 1 + m_info->max_value - m_info->min_value;
		byte thumb_width = 24/num_values;
		if(thumb_width<3) thumb_width = 3;
		byte range = (24 - thumb_width);
		byte pos = (0.5+((value-m_info->min_value) * range)/num_values);
		highlight[7] = 0xFFFFFF;
		uint32_t m = ((uint32_t)1)<<(23-pos);
		uint32_t n = 0;
		while(thumb_width-->0) {
			n |= m;
			m>>=1;
		}
		highlight[7] &= ~n;
		raster[7] |= n;
	}
};
/*
	////////////////////////////////////////////////////////////////
	// Decide how to represent
	static void render_indicator(byte *raster_buf, byte *highlight_buf) {
		byte num_values = c_param[s_edit_param].max_value;
		byte edit_value = s_edit_value;

		// 0         1         2
		// 012345678901234567890123

		// XXXX XXXX
		// XXXX XXXX XXXX XXXX XXXX
		// XXX XXX XXX XXX XXX XXX
		// XX XX XX XX XX XX XX XX
		// X X X X X X X X X X X X
		// XXXXXXXXXXXXXXXXXXXXXXXX

		byte width;
		byte space=0;
		if(num_values <= 5) {
			width = 5;
		}
		else if(num_values <= 6) {
			width = 4;
		}
		else if(num_values <= 8) {
			width = 3;
		}
		else if(num_values <= 12) {
			width = 2;
		}
		else if(num_values <= 24){
			width = 1;
		}
		else {
			return;
		}

		uint32_t raster = 0;
		uint32_t highlight = 0;
		byte this_value = 0;
		byte count = 0;
		for(byte i=0; i<num_values; ++i) {
			for(byte j=0; j<width; ++j) {
				raster<<=1;
				highlight<<=1;
				++count;
				if(this_value == edit_value) {
					raster |= 1;
				}
				else {
					highlight |= 1;
				}
			}
			++this_value;
		}
		while(count++<24) {
			raster<<=1;
			highlight<<=1;
		}
		raster_buf[18] = (byte)(raster >> 16);
		raster_buf[19] = (byte)(raster >> 8);
		raster_buf[20] = (byte)(raster);
		highlight_buf[18] = (byte)(highlight >> 16);
		highlight_buf[19] = (byte)(highlight >> 8);
		highlight_buf[20] = (byte)(highlight);
	}


	////////////////////////////////////////////////////////////////
	// Prepare the UI code to show a new parameter edit screen
	static void ui_new_param() {
		byte raster[24] = {0};
		byte highlight[24] = {0};
		disp_format_text(c_param[s_edit_param].name, raster);
		render_indicator(raster, highlight);
		disp_write(raster, highlight);
	}


	static void ui_new_value() {
		byte raster[24] = {0};
		byte highlight[24] = {0};
		if(c_param[s_edit_param].flags & PFLAG_NUMBER) {
			disp_format_3digit(s_edit_value, raster);
			arpie_set_param(s_edit_param, s_edit_value);
		}
		else if(s_flags & PFLAG_DEFER) {
			disp_format_text(c_param[s_edit_param].value[s_edit_value], highlight);
		}
		else {
			disp_format_text(c_param[s_edit_param].value[s_edit_value], raster);
		}
		render_indicator(raster, highlight);
		disp_write(raster, highlight);
	}


	void ui_begin_edit(byte param) {
		s_edit_param = param;
		s_edit_value = arpie_get_param(s_edit_param);
		ui_new_param();
	}

	void ui_on_key(byte key) {

		if(key & KEY_RELEASE) {
			s_key_status &= ~(((uint32_t)1)<<((key & ~KEY_RELEASE)-1));

			if(s_flags & PFLAG_DEFER) {
				s_flags &= ~PFLAG_DEFER;
				s_flags |= UI_FLAG_VALUE_CHANGE;
			}
		}
		else {
			s_key_status |= (((uint32_t)1)<<(key-1));

			switch(key) {
			case KEY_B1:
				if(s_key_status & KEYBIT_SHIFT)
					ui_begin_edit(PARAM_MIDI_IN_CHAN);
				else
					ui_begin_edit(PARAM_MIDI_OUT_CHAN);
				break;
			case KEY_B2:
				ui_begin_edit(PARAM_GATE_LEN);
				break;
			case KEY_B3:
				ui_begin_edit(PARAM_ARP_TYPE);
				break;
			case KEY_B4:
				ui_begin_edit(PARAM_BPM);
				break;
			case KEY_B5:
				ui_begin_edit(PARAM_SEQ_RATE);
				break;
			}
		}
	}


	void ui_on_encoder(char delta) {
		if(delta < 0 && s_edit_value > c_param[s_edit_param].min_value) {
			--s_edit_value;
			s_flags |= UI_FLAG_VALUE_CHANGE;
			s_flags |= (c_param[s_edit_param].flags & PFLAG_DEFER);
		}
		else if(delta > 0 && s_edit_value < c_param[s_edit_param].max_value) {
			++s_edit_value;
			s_flags |= UI_FLAG_VALUE_CHANGE;
			s_flags |= (c_param[s_edit_param].flags & PFLAG_DEFER);
		}
	}








	void ui_init() {
		//ui_new_param();
	}
	void ui_run() {
		if(s_flags & UI_FLAG_VALUE_CHANGE) {
			ui_new_value();
			s_flags &= ~UI_FLAG_VALUE_CHANGE;
		}
	}





*/


#endif /* SOURCES_UIPARAMEDIT_H_ */
