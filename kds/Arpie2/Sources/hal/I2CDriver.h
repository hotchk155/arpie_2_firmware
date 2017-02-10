/*
 * I2CDriver.h
 *
 *  Created on: 8 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_HAL_I2CDRIVER_H_
#define SOURCES_HAL_I2CDRIVER_H_

#include "Definitions.h"
#include <string.h>

class CI2CDriver : public II2CDriver {
public:
	enum {
		MAX_PAYLOAD = 25,
		MAX_TRANSACTIONS = 5
	};
protected:
	typedef struct
	{
		byte addr;							// i2c address of device
		byte payload[MAX_PAYLOAD];			// data sent or received
		byte payload_size;					// and size in bytes
		II2CMessageListener *listener;		// for a READ set to object reading data. For WRITE is null.
	} TRANSACTION;



	TRANSACTION m_txn[MAX_TRANSACTIONS];
	volatile byte m_head;
	volatile byte m_tail;
	volatile byte m_busy;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// i2c_start_next_txn
	// Start off the next transaction
	void init_txn()
	{
		// if the bus is busy or there is no next transaction then
		// we can ignore this call
		if(m_busy || m_head == m_tail) {
			return;
		}
		LDD_TError e = I2CBus_SelectSlaveDevice(I2CBus_DeviceData, LDD_I2C_ADDRTYPE_7BITS, m_txn[m_tail].addr);
		m_busy = 1;
		if(m_txn[m_tail].listener) {
			e = I2CBus_MasterReceiveBlock(I2CBus_DeviceData, (void*)m_txn[m_tail].payload, m_txn[m_tail].payload_size, LDD_I2C_SEND_STOP);
		}
		else {
			e = I2CBus_MasterSendBlock(I2CBus_DeviceData, (void*)m_txn[m_tail].payload, m_txn[m_tail].payload_size, LDD_I2C_SEND_STOP);
		}
	}

public:
	CI2CDriver() {
		m_head = 0;
		m_tail = 0;
		m_busy = 0;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// i2c_on_txn_complete
	// Invoked from interrupt handler when I2C bus transation completes
	void on_txn_complete(byte status)
	{
		m_busy = 0;
		if(m_txn[m_tail].listener) {
			m_txn[m_tail].listener->on_i2c_msg(m_txn[m_tail].addr, m_txn[m_tail].payload, m_txn[m_tail].payload_size);
		}
		if(++m_tail >= MAX_TRANSACTIONS) {
			m_tail = 0;
		}
		// kick off next transaction
		init_txn();
	}

	////////////////////////////////////////////////////////////////////////
	// Called to place a read or write transaction in queue for the i2c bus
	byte send(byte addr, byte *payload, byte payload_size)
	{
		// get the next head position
		byte next = m_head+1;
		if(next >= MAX_TRANSACTIONS) {
			next = 0;
		}

		// check buffer is not full
		if(next == m_tail) {
			return 0;
		}
		m_txn[m_head].addr = addr;
		m_txn[m_head].payload_size = payload_size;
		memcpy((void*)&m_txn[m_head].payload, payload, payload_size);

		m_head = next;

		// kick off next transaction
		init_txn();
	}
	////////////////////////////////////////////////////////////////////////
	// Called to place a read or write transaction in queue for the i2c bus
	byte receive(byte addr, byte payload_size, II2CMessageListener *listener)
	{
		// get the next head position
		byte next = m_head+1;
		if(next >= MAX_TRANSACTIONS) {
			next = 0;
		}

		// check buffer is not full
		if(next == m_tail) {
			return 0;
		}
		m_txn[m_head].addr = addr;
		m_txn[m_head].listener = listener;
		m_txn[m_head].payload_size = payload_size;
		memset((void*)&m_txn[m_head].payload, 0, payload_size);

		m_head = next;

		// kick off next transaction
		init_txn();
	}
};

#endif /* SOURCES_HAL_I2CDRIVER_H_ */
