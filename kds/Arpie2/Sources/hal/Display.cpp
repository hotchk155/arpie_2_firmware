
#include "Display.h"
const byte g_digits[] = {
2,6,2,2,2,
7,1,7,4,7,
7,1,3,1,7,
5,5,7,1,1,
7,4,7,1,7,
7,4,7,5,7,
7,1,1,1,1,
7,5,7,5,7,
7,5,7,1,1,
7,5,5,5,7
};



/*
#include "arpie2.h"

#define I2C_ADDR 123
#define CMD_CLS 0x81
#define CMD_TEST 0x82




/////////////////////////////////////////////////////////////////////////
//
// LARGE SCOREBOARD DIGITS
//
/////////////////////////////////////////////////////////////////////////

const byte charSet[] = {

1,1,1,1,1,0,1,0, //!
5,5,0,0,0,0,0,0, //"
10,10,31,10,31,10,10,0, //#
2,7,4,7,1,7,2,0, //$
0,0,25,26,4,11,19,0, //%
0,4,10,4,11,10,5,0, //&
1,1,0,0,0,0,0,0, //'
1,2,2,2,2,2,2,1, //(
2,1,1,1,1,1,1,2, //)
0,5,2,7,2,5,0,0, //*
0,2,2,7,2,2,0,0, //+
0,0,0,0,0,1,1,2, //,
0,0,0,7,0,0,0,0, //-
0,0,0,0,0,0,1,0, //.
0,1,1,2,2,4,4,0, ///

6,9,11,13,9,9,6,0, //0
2,6,2,2,2,2,7,0, //1
6,9,1,6,8,8,15,0, //2
6,9,1,6,1,9,6,0, //3
1,3,5,9,15,1,1,0, //4
15,8,8,6,1,9,6,0, //5
6,9,8,14,9,9,6,0, //6
15,1,1,2,4,4,4,0, //7
6,9,9,6,9,9,6,0, //8
6,9,9,7,1,9,6,0, //9

0,0,0,1,0,1,0,0,//:
0,0,0,1,0,1,1,2, //;
0,1,2,4,2,1,0,0, //<
0,0,7,0,7,0,0,0, //=
0,4,2,1,2,4,0,0, //>
2,5,1,2,2,0,2,0, //?
14,17,17,23,21,23,16,15, //@

6,9,9,15,9,9,9,0, //A
14,9,9,14,9,9,14,0, //B
6,9,8,8,8,9,6,0, //C
14,9,9,9,9,9,14,0, //D
15,8,8,14,8,8,15,0, //E
15,8,8,14,8,8,8,0, //F
6,9,8,11,9,9,7,0, //G
9,9,9,15,9,9,9,0, //H
7,2,2,2,2,2,7,0, //I
15,1,1,1,1,9,6,0, //J
9,10,12,8,12,10,9,0, //K
8,8,8,8,8,8,15,0, //L
17,27,21,17,17,17,17,0, //M
9,9,13,11,9,9,9,0, //N
6,9,9,9,9,9,6,0, //O
14,9,9,14,8,8,8,0, //P
6,9,9,9,11,9,7,0, //Q
14,9,9,14,9,9,9,0, //R
6,9,8,6,1,9,6,0, //S
31,4,4,4,4,4,4,0, //T
9,9,9,9,9,9,6,0, //U
9,9,9,9,9,10,12,0, //V
17,17,17,17,21,27,17,0, //W
17,17,10,4,10,17,17,0, //X
17,17,10,4,4,4,4,0, //Y
31,1,2,4,8,16,31,0, //Z

3,2,2,2,2,2,2,3, //[
0,4,4,2,2,1,1,0, // backslash
3,1,1,1,1,1,1,3, //]
2,5,0,0,0,0,0,0, //^
0,0,0,0,0,0,0,7, //_

0b01111110,
0b10000001,
0b10100101,
0b10000001,
0b10100101,
0b10111101,
0b10000001,
0b01111110,

0,0,14,1,7,9,7,0, //a
8,8,14,9,9,9,14,0, //b
0,0,6,9,8,9,6,0, //c
1,1,7,9,9,9,7,0, //d
0,0,6,9,15,8,6,0, //e
3,4,4,6,4,4,4,0, //f
0,0,7,9,9,7,1,14, //g
8,8,14,9,9,9,9,0, //h
1,0,1,1,1,1,1,0, //i
1,0,1,1,1,1,1,2, //j
8,8,9,10,12,10,9,0, //k
2,2,2,2,2,2,1,0, //l
0,0,30,21,21,17,17,0, //m
0,0,14,9,9,9,9,0, //n
0,0,6,9,9,9,6,0, //o
0,0,14,9,9,14,8,8, //p
0,0,7,9,9,7,1,1, //q
0,0,14,9,8,8,8,0, //r
0,0,7,8,6,1,14,0, //s
2,2,3,2,2,2,1,0, //t
0,0,9,9,9,9,7,0, //u
0,0,9,9,9,10,12,0, //v
0,0,17,17,21,21,27,0, //w
0,0,17,10,4,10,17,0, //x
0,0,9,9,9,7,1,14, //y
0,0,31,2,4,8,31,0, //z


1,2,2,4,2,2,2,1, //{
1,1,1,1,1,1,1,1, //|
4,2,2,1,2,2,2,4, //}
5,10,0,0,0,0,0,0 //~

};



/////////////////////////////////////////////////////////////////////////
// Lookup the 8 raster rows for an ASCII character. Unknown characters
// are blanks. The function return the width of the character
int lookupChar(char ch, byte *buf)
{
  if(ch<33 || ch>126) {
    memset(buf, 0, 8);
    return 3;
  }
  else {
    byte mask = 0;
    const byte *p = charSet + ((ch-33) * 8);
    for(int i=0; i<8; ++i) {
      buf[i] = *(p + i);
      mask |= buf[i];
    }
    if(mask & 0x80) {  return 8; }
    if(mask & 0x40) {  return 7; }
    if(mask & 0x20) {  return 6; }
    if(mask & 0x10) {  return 5; }
    if(mask & 0x08) {  return 4; }
    if(mask & 0x04) {  return 3; }
    if(mask & 0x02) {  return 2; }
    if(mask & 0x01) {  return 1; }
    return 3;
  }
}


#define DISP_BUFFER_SIZE 50
static byte disp_buffer[DISP_BUFFER_SIZE];

void disp_zero()
{
	memset(disp_buffer, 0, sizeof(disp_buffer));
}
void disp_init()
{
	disp_zero();
}

void disp_text(char *szText)
{
	uint32_t result[8] = {0};
	int pos = 0;
	while(*szText && pos < 24)
	{

	    // get the definition and width of the next character
	    byte buf[8];
	    int width = lookupChar(*szText, buf);

	    int shift = 31 - width - pos;
	    for(int row=0; row < 8; ++row)
		{
			result[row] |= (((uint32_t)buf[row])<<shift);
		}
	    pos+=(width+1);
	    ++szText;
	}


	disp_zero();
	byte len = 1;
	for(int i=0; i<8; ++i)
	{
		disp_buffer[len++] = result[i]>>24;
		disp_buffer[len++] = result[i]>>16;
		disp_buffer[len++] = result[i]>>8;
	}
	i2c_queue_txn(I2C_ADDR, I2C_WRITE, disp_buffer, len);
}

*/
