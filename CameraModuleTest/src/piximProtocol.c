#include "piximProtocol.h"
#include "serial.h"

unsigned char calcChecksum(unsigned char * _msg, unsigned int _size) {

	unsigned char sumL = 0;
	unsigned int iL;

	for (iL = 0; iL < _size; iL++)
		sumL += _msg[iL];

	return 0xFF - sumL;
}

unsigned int verifyChecksum(unsigned char * _msg, unsigned int _size) {

	return calcChecksum(_msg, _size-1) == _msg[_size-1];
}

unsigned int isAck(unsigned char * _msg) {

	return *_msg == ACK;
}

unsigned int isAckd(unsigned char * _msg) {

	return *_msg == ACKD;
}

unsigned int isNack(unsigned char * _msg) {

	return *_msg == NACK;
}

unsigned int isAckerr(unsigned char * _msg) {

	return *_msg == ACKERR;
}

void createInquiryMessage(unsigned char * _msg, unsigned int _id) {
	_msg[0] = 0x03;
	_msg[1] = 0x02;
	_msg[2] = 0x00;
	_msg[3] = _id;
	_msg[4] = calcChecksum(_msg, 4);
}

int createCommandMessage (unsigned char * _msg, unsigned int _id, unsigned int _value) {
	_msg[0] = 0x02;
	_msg[1] = getDataSize(_id);
	switch(getDataSize(_id)) {
		case 0:
			return -1;	// invalid data size

		case 1:
			_msg[2] = _value;
			break;

		case 2:
			_msg[2] = _value >> 8;
			_msg[3] = _value;
			break;

		case 4:
			_msg[2] = _value >> 24;
			_msg[3] = _value >> 16;
			_msg[4] = _value >> 8;
			_msg[5] = _value;
			break;
	}
	_msg[getDataSize(_id)+2] = calcChecksum(_msg, getDataSize(_id)+2);

	return 0;
}

unsigned int retrieveProperty(unsigned char * _msg, unsigned int * _error) {

	unsigned int dataSizeL;
	unsigned int resL = 0;
	unsigned int iL;

	if (isAckd(_msg)) {
			dataSizeL = _msg[1];
			for (iL = 0; iL < dataSizeL; iL++)
			 	resL = (resL << 8) | _msg[2+iL];
			*_error = 0;
			return resL;
	}

	*_error = 1;

	return -1;
}

unsigned int receiveMsg(unsigned char * _msg, unsigned int _maxSize) {

	receiveMessageSerial(_msg, 1);

	switch (_msg[0]) {
		case ACK:
			return 1;
			break;
		case ACKD:
			receiveMessageSerial(&_msg[1],1);
			if (_msg[1]+3 <= _maxSize)
				receiveMessageSerial(&_msg[2],_msg[1]+1);
			else
				return -2; // msg > _maxSize
			break;
		case NACK:
			return 1;
		case ACKERR:
			if (_maxSize > 3)
				receiveMessageSerial(&_msg[2],2);
			else
				return -2; // msg > _maxSize
			break;
		default:
			return -3; // invalid message type
	}

	if (verifyChecksum(_msg, _msg[1]+2))
		return _msg[1];

	return -1;
}

unsigned int getDataSize(unsigned char _id) {

	static unsigned int sizesL[] = {
	//	x0    x1    x2    x3    x4    x5    x6    x7    x8    x9    xA    xB    xC    xD    xE    xF
		0x00, 0x00, 0x00, 0x04, 0x04, 0x02, 0x01, 0x04, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, // 0x
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 1x
        0x01, 0x00, 0x01, 0x01, 0x00, 0x02, 0x02, 0x02, 0x02, 0x04, 0x04, 0x01, 0x02, 0x01, 0x01, 0x01, // 2x
        0x01, 0x04, 0x04, 0x01, 0x02, 0x00, 0x00, 0x01, 0x04, 0x04, 0x00, 0x01, 0x04, 0x00, 0x01, 0x01, // 3x
        0x01, 0x01, 0x02, 0x02, 0x02, 0x01, 0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x04, // 4x
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, // 5x
        0x04, 0x00, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 6x
        0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x01, // 7x
        0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, // 8x
        0x04, 0x00, 0x01, 0x01, 0x04, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x04, 0x04, 0x00, 0x00, 0x04, // 9x
        0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, // Ax
        0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, // Bx
        0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Cx
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Dx
        0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Ex
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Fx
	};

	return sizesL[_id];
}

unsigned int getProperty(unsigned char _id, unsigned int * _error) {

	unsigned char msgL[5];
	unsigned char respL[8];

	createInquiryMessage(msgL, _id);

	sendMessageSerial(msgL, 5);

	if (receiveMsg(respL, 8) < 1) {
		*_error = 1; // Receive message error
		return -1;
	}

	return retrieveProperty(respL, _error);
}

int setProperty(unsigned int _property, unsigned int _value) {

	unsigned char msgL[8];
	unsigned char respL[8];

	if (!createCommandMessage(msgL, _property, _value)) {
		sendMessageSerial(msgL, getDataSize(_property)+3);
		if (receiveMsg(respL, 8) < 1) {
			return -1;			// invalid message
		}

		if (isAck(respL))
			return 0;

		return -2;	// Ackerr

	}

	return -1;
}



















