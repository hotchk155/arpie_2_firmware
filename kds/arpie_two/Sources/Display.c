/*
 * Display.c
 *
 *  Created on: 11 Feb 2017
 *      Author: Jason
 */

#include "Arpie.h"


static const byte s_charset[] =
{
	7,5,5,5,7, //0
	2,6,2,2,2, //1
	7,1,7,4,7, //2
	7,1,3,1,7, //3
	5,5,7,1,1, //4
	7,4,7,1,7, //5
	7,4,7,5,7, //6
	7,1,1,1,1, //7
	7,5,7,5,7, //8
	7,5,7,1,1, //9

	7,5,7,5,5, //A
	7,5,6,5,7, //B
	7,4,4,4,7, //C
	6,5,5,5,6, //D
	7,4,6,4,7, //E
	7,4,6,4,4, //F
	7,4,5,5,7, //G
	5,5,7,5,5, //H
	7,2,2,2,7, //I
	7,1,1,1,6, //J
	5,5,6,5,5, //K
	4,4,4,4,7, //L
	5,7,7,5,5, //M
	6,5,5,5,5, //N
	7,5,5,5,7, //O
	7,5,7,4,4, //P
	7,5,5,7,7, //Q
	7,5,6,5,5, //R
	7,4,7,1,7, //S
	7,2,2,2,2, //T
	5,5,5,5,7, //U
	5,5,5,5,6, //V
	5,5,7,7,5, //W
	5,5,2,5,5, //X
	5,5,2,2,2, //Y
	7,1,2,4,7, //Z

	4,1,2,4,1, // %
	0,0,7,0,0, // 0
	1,1,2,4,4  // /
};

static void format(byte *raster, const byte *map, byte pos) {
	byte base = (pos/2)%3;
	for(int i=0; i<5; ++i) {
		byte b = *map++;
		if(!(pos & 1)) b<<=4;
		raster[base]|=b;
		base += 3;
	}
}


static void format_char(byte *raster, byte ch, byte pos) {
	const byte  *map;
	if(ch >= '0' && ch <= '9') {
		map = &s_charset[5 * (ch - '0')];
	}
	else if(ch >= 'A' && ch <= 'Z') {
		map = &s_charset[5 * (10 + ch - 'A')];
	}
	else if(ch >= 'a' && ch <= 'z') {
		map = &s_charset[5 * (10 + ch - 'a')];
	}
	else {
		switch(ch) {
		case '%': map = &s_charset[5*36]; break;
		case '-': map = &s_charset[5*37]; break;
		case '/': map = &s_charset[5*38]; break;
		default:
			return;
		}
	}
	format(raster,map,pos);
}

static void format_digit(byte *disp, byte n, byte pos) {
	if(n >= 0 && n < 10) {
		format(disp,&s_charset[5 * n],pos);
	}
}


//////////////////////////////////////////////////////////////////
void disp_write(byte *raster, byte *highlight) {
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
		i2c_write(I2C_ADDR_DISP,buf,len);
	}
}




void disp_format_4digit(int n, byte *raster) {
	format_digit(raster, n/1000, 0);
	n%=1000;
	format_digit(raster, n/100, 1);
	n%=100;
	format_digit(raster, n/10, 2);
	n%=10;
	format_digit(raster, n, 3);
}

void disp_format_3digit(int n, byte *raster) {
	format_digit(raster, n/100, 0);
	n%=100;
	format_digit(raster, n/10, 1);
	n%=10;
	format_digit(raster, n, 2);
}


void disp_format_text(const byte *sz, byte *raster) {
	int pos = 0;
	while(*sz && pos <6) {
		format_char(raster, *sz++, pos++);
	}
}


void disp_text(const byte *sz) {
	byte raster[24] = {0};
	disp_format_text(sz, raster);
	disp_write(raster, NULL);
}

