/*
 * Pixim Protocol implementation
 *
 * Message format:
 *
 * 		command byte : byte count : data :  checksum
 *
 * Note:
 * 		data is big endian
 * 		checksum = FF - (command byte + byte count + data + checksum)
 *
 */

#ifndef PIXIMPROTOCOL_H_
#define PIXIMPROTOCOL_H_

/*
 * Return codes:
 */
#define ACK		0x01
#define ACKD	0x02
#define	NACK	0x03
#define ACKERR	0x04

unsigned char calcChecksum(unsigned char * _msg, unsigned int _size);

unsigned int verifyChecksum(unsigned char * _msg, unsigned int _size);

unsigned int isAck(unsigned char * _msg);

unsigned int isAckd(unsigned char * _msg);

unsigned int isNack(unsigned char * _msg);

unsigned int isAckerr(unsigned char * _msg);

void createInquiryMessage(unsigned char * _msg, unsigned int _id);

int createCommandMessage (unsigned char * _msg, unsigned int _id, unsigned int _value);

unsigned int retrieveProperty(unsigned char * _msg, unsigned int * _error);

unsigned int receiveMsg(unsigned char * _msg, unsigned int _maxSize);

unsigned int getDataSize(unsigned char _id);

unsigned int getProperty(unsigned char _id, unsigned int * _error);

int setProperty(unsigned int _property, unsigned int _value);

#endif

