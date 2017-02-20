/*
 * Display.h
 *
 *  Created on: 10 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_HAL_DISPLAY_H_
#define SOURCES_HAL_DISPLAY_H_


class CDisplay {

public:


	void show_number(int n, CI2CDriver *i2c) {
		byte disp[25] = {0};
		show_digit(disp+1, n/1000, 0);
		n%=1000;
		show_digit(disp+1, n/100, 1);
		n%=100;
		show_digit(disp+1, n/10, 2);
		n%=10;
		show_digit(disp+1, n, 3);
		i2c->init_write(I2C_ADDR_DISP,disp,25);
	}

	void show_digit(byte *disp, byte digit, byte pos) {
		static const byte raster[] = {
		7,5,5,5,7,
		2,6,2,2,2,
		7,1,7,4,7,
		7,1,3,1,7,
		5,5,7,1,1,
		7,4,7,1,7,
		7,4,7,5,7,
		7,1,1,1,1,
		7,5,7,5,7,
		7,5,7,1,1
		};
		byte b[5];
		digit *= 5;
		byte i;
		byte base = (pos/2)%3;
		for(i=0; i<5; ++i) {
			byte b = raster[digit++];
			if(!(pos & 1)) b<<=4;
			disp[base]|=b;
			base += 3;
		}

	}

};


#endif /* SOURCES_HAL_DISPLAY_H_ */
