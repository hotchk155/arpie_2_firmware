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
		ARP_A_PAUSE,
		ARP_B_PAUSE,
		ARP_A_PLAY,
		ARP_B_PLAY,
	};
	byte m_mode:4;
	CUiTitle() {
		m_mode = TITLE1;
	}
	void ui_init() {
	}
	void ui_done() {

	}
	void ui_render(uint32_t *raster, uint32_t *highlight) {
		const uint32_t title[8] = {
			(uint32_t)0b011100011101111001001110,
			(uint32_t)0b100010100001000101010001,
			(uint32_t)0b100010100001000101011111,
			(uint32_t)0b100010100001000101010000,
			(uint32_t)0b011110100001111001001110,
			(uint32_t)0b000000000001000000000000,
			(uint32_t)0b111111111101011111111111,
			(uint32_t)0b000000000001000000000000
		};
		const byte A[8] = {
			0b00000000,
			0b00111000,
			0b01000100,
			0b01111100,
			0b01000100,
			0b01000100,
			0b00000000,
			0b00000000
		};
		const byte B[8] = {
			0b00000000,
			0b01111000,
			0b01000100,
			0b01111000,
			0b01000100,
			0b01111000,
			0b00000000,
			0b00000000
		};
		const byte PLAY[8] = {
			0b00000000,
			0b01100000,
			0b01110000,
			0b01111000,
			0b01110000,
			0b01100000,
			0b00000000,
			0b00000000,
		};
		const byte PAUSE[8] = {
			0b00000000,
			0b01101100,
			0b01101100,
			0b01101100,
			0b01101100,
			0b01101100,
			0b00000000,
			0b00000000,
		};
		const byte BKGND[8] = {
			0b01111100,
			0b11111110,
			0b11111110,
			0b11111110,
			0b11111110,
			0b11111110,
			0b01111100,
			0b00000000
		};


		switch(m_mode) {
			case TITLE1:
				memcpy(highlight,title,sizeof(title));
				memcpy(raster,title,sizeof(title));
				break;
			case TITLE2:
				memcpy(raster,title,sizeof(title));
				break;
			case TITLE3:
				memcpy(highlight,title,sizeof(title));
				break;
			case ARP_A_PLAY:
			case ARP_A_PAUSE:
			case ARP_B_PLAY:
			case ARP_B_PAUSE:
				CDisplay::instance().format_8x8((m_mode == ARP_A_PLAY||m_mode == ARP_A_PAUSE)? A:B, highlight, 0);
				CDisplay::instance().format_8x8(BKGND, raster, 0);
				CDisplay::instance().format_8x8((m_mode == ARP_A_PLAY||m_mode == ARP_B_PLAY)? PLAY:PAUSE, highlight, 1);
				CDisplay::instance().format_8x8(BKGND, raster, 1);
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
