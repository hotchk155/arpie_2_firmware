/*
 * i2c.c
 *
 *  Created on: 4 Jan 2017
 *      Author: Jason
 */
/*
#include "arpie2.h"


#define MAX_PAYLOAD_SIZE 100
typedef struct
{
	byte addr;
	byte mode;
	byte payload[MAX_PAYLOAD_SIZE];
	byte payload_size;
} I2C_TRANSACTION;

#define MAX_TRANSACTION_QUEUE 10

static volatile I2C_TRANSACTION s_queue[MAX_TRANSACTION_QUEUE];
static volatile I2C_TRANSACTION *s_head;
static volatile I2C_TRANSACTION *s_tail;
static volatile I2C_TRANSACTION *s_end_of_queue;
static volatile byte s_busy;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i2c_start_next_txn
// Start off the next transaction
static byte i2c_start_next_txn()
{
	if(s_busy)
		return 0;
	// is there anything in the buffer?
	if(s_head != s_tail)
	{
		LDD_TError e = I2CBus_SelectSlaveDevice(I2CBus_DeviceData, LDD_I2C_ADDRTYPE_7BITS, s_tail->addr);
		s_busy = 1;
		if(s_tail->mode == I2C_READ) {
			e = I2CBus_MasterReceiveBlock(I2CBus_DeviceData, (void*)s_tail->payload, s_tail->payload_size, LDD_I2C_SEND_STOP);
		}
		else {
			e = I2CBus_MasterSendBlock(I2CBus_DeviceData, (void*)s_tail->payload, s_tail->payload_size, LDD_I2C_SEND_STOP);
		}
		return 1;
	}
	return 0;
}

void i2c_handle_response(I2C_TRANSACTION *rx)
{
	for(int i=0; i<rx->payload_size; ++i)
	{
		if(rx->payload[i])
			keyb_handle_input(rx->payload[i]);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i2c_init
// Initialise I2C stuff
void i2c_init()
{
	s_head = s_queue;
	s_tail = s_queue;
	s_end_of_queue = &s_queue[MAX_TRANSACTION_QUEUE];
	s_busy = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i2c_on_txn_complete
// Invoked from event handler when I2C bus transation completes
void i2c_on_txn_complete(byte error)
{
	s_busy = 0;
	if(s_tail->mode == I2C_READ)
		i2c_handle_response((I2C_TRANSACTION *)s_tail);
	if(++s_tail == s_end_of_queue)
		s_tail = s_queue;
	i2c_start_next_txn();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i2c_queue_txn
//
int i2c_queue_txn(byte addr, byte mode, byte *payload, byte payload_size)
{
	// get the next head position and make sure the
	// buffer is not full
	I2C_TRANSACTION *s_next = (I2C_TRANSACTION *)s_head + 1;
	if(s_next == s_end_of_queue)
		s_next = (I2C_TRANSACTION *)s_queue;
	if(s_next == s_tail)
		return 0;

	// copy supplied data into the buffer
	s_head->addr = addr;
	s_head->mode = mode;
	s_head->payload_size = payload_size;
	if(mode == I2C_READ) {
		memset((void*)s_head->payload, 0, payload_size);
	}
	else {
		memcpy((void*)s_head->payload, payload, payload_size);
	}

	// advance the head
	s_head = s_next;

	// if
	if(!s_busy)
		i2c_start_next_txn();
}
*/
