/*
 * Keyboard.c
 *
 *  Created on: 18 Jan 2017
 *      Author: Jason
 */
/*
#include "arpie2.h"

#define I2C_ADDR 123
#define KEYB_BUFFER_SIZE 8
byte s_buffer[KEYB_BUFFER_SIZE];
unsigned int s_next_poll;
volatile byte s_head;
volatile byte s_tail;
void keyb_init()
{
	s_head = 0;
	s_tail = 0;
	s_next_poll = 0;
}
void keyb_handle_input(byte press)
{
	byte s_next = (byte)s_head + 1;
	if(s_next >= KEYB_BUFFER_SIZE)
		s_next = 0;
	if(s_next != s_tail)
	{
		s_buffer[s_head] = press;
		s_head = s_next;
	}
}
byte keyb_event()
{

	byte s_next = (byte)s_tail + 1;
	if(s_next >= KEYB_BUFFER_SIZE)
		s_next = 0;
	if(s_next != s_head)
	{
		byte event = s_buffer[s_tail];
		s_tail = s_next;
		return event;
	}
	return 0;
}
*/
