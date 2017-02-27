/*
 * Display.h
 *
 *  Created on: 20 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_DISPLAY_H_
#define SOURCES_DISPLAY_H_

class CDisplay {
	enum {
		SYM_PCT,
		SYM_MINUS,
		SYM_SLASH,
	};
	static const byte m_digits[];
	static const byte m_letters[];
	static const byte m_symbols[];

	void format(byte *raster, const byte *map, byte pos) {
		byte base = (pos/2)%3;
		for(int i=0; i<5; ++i) {
			byte b = *map++;
			if(!(pos & 1)) b<<=4;
			raster[base]|=b;
			base += 3;
		}
	}

	void format_char(byte *raster, byte ch, byte pos) {
		const byte  *map;
		if(ch >= '0' && ch <= '9') {
			map = &m_digits[5 * (ch - '0')];
		}
		else if(ch >= 'A' && ch <= 'Z') {
			map = &m_letters[5 * (ch - 'A')];
		}
		else if(ch >= 'a' && ch <= 'z') {
			map = &m_letters[5 * (ch - 'a')];
		}
		else {
			switch(ch) {
			case '%': map = &m_symbols[5*SYM_PCT]; break;
			case '-': map = &m_symbols[5*SYM_MINUS]; break;
			case '/': map = &m_symbols[5*SYM_SLASH]; break;
			default:
				return;
			}
		}
		format(raster,map,pos);
	}

	void format_digit(byte *disp, byte n, byte pos) {
		if(n >= 0 && n < 10) {
			format(disp,&m_digits[5 * n],pos);
		}
	}

public:
	static inline CDisplay& instance() {
		static CDisplay disp;
		return disp;
	}

	void write(byte *raster, byte *highlight) {
		byte buf[50];
		byte len = 0;
		if(raster) {
			buf[0] = 0;
			memcpy(&buf[1], raster, 24);
			if(highlight) {
				memcpy(&buf[25], highlight, 24);
				len = 49;
			}
			else {
				len = 25;
			}
		}
		else if(highlight) {
			buf[0] = 24;
			memcpy(&buf[1], highlight, 24);
			len = 25;
		}
		if(len) {
			CI2C::instance().write(CI2C::DISP_ADDR,buf,len);
		}
	}

	void write(uint32_t *raster, uint32_t *highlight) {
		byte buf[50];
		byte len = 0;
		byte i;
		if(raster) {
			buf[len++] = 0;

			for(i=0; i<8; ++i) {
				buf[len++] = (*raster)>> 16;
				buf[len++] = (*raster)>> 8;
				buf[len++] = (*raster);
				++raster;
			}

			if(highlight) {
				for(i=0; i<8; ++i) {
					buf[len++] = (*highlight)>> 16;
					buf[len++] = (*highlight)>> 8;
					buf[len++] = (*highlight);
					++highlight;
				}
			}
		}
		else if(highlight) {
			buf[len++] = 24;
			if(highlight) {
				for(i=0; i<8; ++i) {
					buf[len++] = (*highlight)>> 16;
					buf[len++] = (*highlight)>> 8;
					buf[len++] = (*highlight);
					++highlight;
				}
			}
		}
		if(len) {
			CI2C::instance().write(CI2C::DISP_ADDR,buf,len);
		}
	}

	void format_4digit(int n, byte *raster) {
		format_digit(raster, n/1000, 0);
		n%=1000;
		format_digit(raster, n/100, 1);
		n%=100;
		format_digit(raster, n/10, 2);
		n%=10;
		format_digit(raster, n, 3);
	}

	void format_3digit(int n, byte *raster) {
		format_digit(raster, n/100, 0);
		n%=100;
		format_digit(raster, n/10, 1);
		n%=10;
		format_digit(raster, n, 2);
	}

	void format_text(const byte *sz, byte *raster) {
		int pos = 0;
		while(*sz && pos <6) {
			format_char(raster, *sz++, pos++);
		}
	}

	void text(const byte *sz) {
		byte raster[24] = {0};
		format_text(sz, raster);
		write(raster, NULL);
	}
	void number(int n) {
		byte raster[24] = {0};
		format_4digit(n, raster);
		write(raster, NULL);
	}
};

#endif /* SOURCES_DISPLAY_H_ */
