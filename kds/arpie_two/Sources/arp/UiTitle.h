/*
 * UiTitle.h
 *
 *  Created on: 6 Mar 2017
 *      Author: Jason
 */

#ifndef SOURCES_ARP_UITITLE_H_
#define SOURCES_ARP_UITITLE_H_


class CUiTitle : public IUiComponent {
public:
	enum {
		TITLE1,
		TITLE2,
		TITLE3,
		ARP_A,
		ARP_B,
	};
	byte m_mode:4;
	CUiTitle() {
		m_mode = ARP_A;
	}
	void ui_init() {
	}
	void ui_done() {

	}
	void ui_render(uint32_t *raster, uint32_t *highlight) {
		const uint32_t t[8] = {
			(uint32_t)0b011100011101111001001110,
			(uint32_t)0b100010100001000101010001,
			(uint32_t)0b100010100001000101011111,
			(uint32_t)0b100010100001000101010000,
			(uint32_t)0b011110100001111001001110,
			(uint32_t)0b000000000001000000000000,
			(uint32_t)0b111111111101011111111111,
			(uint32_t)0b000000000001000000000000
		};

		switch(m_mode) {
			case TITLE1:
				memcpy(highlight,t,sizeof(t));
				memcpy(raster,t,sizeof(t));
				break;
			case TITLE2:
				memcpy(raster,t,sizeof(t));
				break;
			case TITLE3:
				memcpy(highlight,t,sizeof(t));
				break;
			case ARP_A:
				CDisplay::instance().format_text("ARP A", raster);
				break;
			case ARP_B:
				CDisplay::instance().format_text("ARP B", raster);
				break;
		}

	}
	byte ui_needs_repaint() {
		return 0;
	}
	byte ui_on_key(byte key_event) {
		return 0;
	}
	void ui_on_encoder(char delta, byte modifiers) {

	}

};



#endif /* SOURCES_ARP_UITITLE_H_ */
