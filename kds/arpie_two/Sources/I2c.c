#include "Arpie.h"

enum {
	MAX_TX = 50,
	MAX_FIFO = 100,
	READ_BIT = 0x80
};
enum {
	BUSY_FLAG = 0x01,		// when this flag is set it means i2c bus is busy
	READ_TXN_FLAG = 0x02,
	OVERFLOW_FLAG = 0x04,	// set if there is an underflow error on input
	UNDERFLOW_FLAG = 0x08,	// set if there is an underflow error on input
	READ_PENDING_FLAG = 0x10, // there is a keypress to be read by application
	TXN_TOO_LARGE_FLAG = 0x20
};

// This buffer contains outgoing or incoming data and is
// accessed directly by the I2C hardware driver code
static byte m_buf[MAX_TX];
static volatile byte m_rx;

// This buffer contains queued transactions. Each is made up of the following
// [LEN] [<READ FLAG><ADDRESS>] [DATA0] [DATA1] ... [DATAN]
// LEN = total number of bytes including address
static byte m_fifo[MAX_FIFO];
static volatile byte m_head;
static volatile byte m_tail;
static volatile byte m_fifo_count;

// Status flags
static volatile byte m_flags;

static byte wait;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Write to transaction queue
static inline void fifo_write(byte data) {
	if(m_fifo_count >= MAX_FIFO) {
		m_flags |= OVERFLOW_FLAG;
	}
	else {
		++m_fifo_count;
		m_fifo[m_head] = data;
		if(++m_head >= MAX_FIFO) {
			m_head = 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// read from transaction queue
static inline byte fifo_read() {
	if(m_fifo_count == 0) {
		m_flags |= UNDERFLOW_FLAG;
		return 0;
	}
	else {
		--m_fifo_count;
		byte data = m_fifo[m_tail];
		if(++m_tail >= MAX_FIFO) {
			m_tail = 0;
		}
		return data;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// peek next item in transaction queue
static inline byte fifo_peek() {
	if(m_fifo_count == 0) {
		return 0;
	}
	else {
		return m_fifo[m_tail];
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// see if next transaction can be kicked off
static void init_txn()
{
	// no transaction in progress and there is something in the buffer?
	if(!(m_flags & BUSY_FLAG) && m_fifo_count) {

		byte txn_size = fifo_peek();
		if(txn_size <= m_fifo_count) { 			// check we have whole transaction in the buffer
			fifo_read(); 						// skip size
			byte rw_addr = fifo_read(); 		// read the address and read flag
			txn_size--;							// remove from total count
			if(txn_size > MAX_TX) {				// is the transaction too big?
				m_flags |= TXN_TOO_LARGE_FLAG;	// set error flag and clear transaction from buffer
				while(txn_size-- > 0) {
					fifo_read();
				}
			}
			else {

				byte addr = (rw_addr & 0x7f);
				LDD_TError e = I2CBus_SelectSlaveDevice(I2CBus_DeviceData, LDD_I2C_ADDRTYPE_7BITS, addr); // set the i2c slave address
				m_flags |= BUSY_FLAG; // flag busy
				if(rw_addr & READ_BIT) {
					// The first step for a read transaction is to write a single zero
					// byte to the slave and do not send a stop condition
					m_buf[0] = 0;
					m_flags  |= READ_TXN_FLAG;
					m_flags &= ~READ_PENDING_FLAG;
					e = I2CBus_MasterSendBlock(I2CBus_DeviceData, (void*)m_buf, 1, LDD_I2C_NO_SEND_STOP);
				}
				else {
					// move data from next transaction into the transmit buffer
					byte tx_count = 0;
					while(txn_size-- > 0) {
						m_buf[tx_count++] = fifo_read();
					}
					e = I2CBus_MasterSendBlock(I2CBus_DeviceData, (void*)m_buf, tx_count, LDD_I2C_SEND_STOP); // kick off a write
				}

			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PUBLIC INTERFACE
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void i2c_init()
{
	m_rx = 0;
	m_head = 0;
	m_tail = 0;
	m_fifo_count = 0;
	m_flags = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void i2c_on_read_complete()	{
	wait = 0;
	/*
	m_flags &= ~BUSY_FLAG;
	if(m_buf[0]) {
		m_rx = m_buf[0];
		m_flags |= READ_PENDING_FLAG;
	}
	init_txn();
	*/
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void i2c_on_write_complete() {
	wait = 0;
	/*
	if(m_flags & READ_TXN_FLAG) {
		m_flags &= ~READ_TXN_FLAG;
		LDD_TError e = I2CBus_MasterReceiveBlock(I2CBus_DeviceData, (void*)m_buf, 2, LDD_I2C_SEND_STOP); // kick off a read
	}
	else {
		m_flags &= ~BUSY_FLAG;
		init_txn();
	}*/
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void i2c_on_error() {
	wait = 0;
	/*
	m_flags &= ~(BUSY_FLAG|READ_TXN_FLAG);
	m_flags |= READ_PENDING_FLAG;
	m_rx = 0;
	init_txn();
	*/
}

////////////////////////////////////////////////////////////////////////
// Called to place a read or write transaction in queue for the i2c bus
byte i2c_init_write(byte addr, byte *payload, byte payload_size)
{
	byte result = 0;
	// make sure there is space in the buffer
	if(m_fifo_count + payload_size + 2 < MAX_FIFO) {
		fifo_write(payload_size + 1);
		fifo_write(addr);
		while(payload_size-- > 0) {
			fifo_write(*payload);
			payload++;
		}
		result = 1;
	}
	init_txn();
	return result;
}

////////////////////////////////////////////////////////////////////////
// Request async data read, returning 1 if successful or 0 if error.
byte i2c_init_read(byte addr)
{
	if(m_flags & READ_PENDING_FLAG) {
		return 0;
	}

	// make sure there is space in the buffer
	if(m_fifo_count + 2 > MAX_FIFO) {
		return 0;
	}
	else {
		fifo_write(1);
		fifo_write(READ_BIT|addr);
		init_txn();
		return 1;
	}
}

////////////////////////////////////////////////////////////////////////
byte i2c_is_response_pending() {
	return(m_flags & READ_PENDING_FLAG);
}
////////////////////////////////////////////////////////////////////////
byte i2c_get_response() {
	m_flags &= ~READ_PENDING_FLAG;
	return m_rx;
}


////////////////////////////////////////////////////////////////////////
// Called to place a read or write transaction in queue for the i2c bus
void i2c_write(byte addr, byte *payload, byte payload_size)
{
	LDD_TError e = I2CBus_SelectSlaveDevice(I2CBus_DeviceData, LDD_I2C_ADDRTYPE_7BITS, addr); // set the i2c slave address
	wait = 1;
	e = I2CBus_MasterSendBlock(I2CBus_DeviceData, (void*)payload, payload_size, LDD_I2C_SEND_STOP); // kick off a write
	while(wait);
}

////////////////////////////////////////////////////////////////////////
// Called to place a read or write transaction in queue for the i2c bus
byte i2c_read(byte addr)
{
	LDD_TError e = I2CBus_SelectSlaveDevice(I2CBus_DeviceData, LDD_I2C_ADDRTYPE_7BITS, addr); // set the i2c slave address
	wait = 1;
	m_buf[0] = 0;
	e = I2CBus_MasterSendBlock(I2CBus_DeviceData, (void*)m_buf, 1, LDD_I2C_NO_SEND_STOP);
	while(wait);
	wait = 1;
	e = I2CBus_MasterReceiveBlock(I2CBus_DeviceData, (void*)m_buf, 1, LDD_I2C_SEND_STOP);
	while(wait);

	return m_buf[0];
}

