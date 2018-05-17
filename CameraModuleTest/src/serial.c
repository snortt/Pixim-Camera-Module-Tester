/*
 * serial.c
 */

#include "serial.h"

static int serial = 0;

void openSerial(void) {

	int lserial;

	lserial = open(SERIAL_USB0, O_RDWR | O_NDELAY);

	if (lserial == -1) {
		printf("openSerial: error opening the serial port - \n");
	}
	else
		if (fcntl(lserial, F_SETFL, 0) == -1)
			printf("fcntl: error setting the serial por status flags - \n");
		else
			printf("fcntl: serial port status flags OK - \n");

	configSerial(lserial);

	serial = lserial;
}


void configSerial(unsigned int _serial) {

	struct termios opts_serial;

	tcgetattr(_serial, &opts_serial);

	cfsetispeed(&opts_serial, BAUDRATE);

	opts_serial.c_cflag |= (CLOCAL | CREAD);

	tcsetattr(_serial, TCSANOW, &opts_serial);

	opts_serial.c_cflag &= ~PARENB;
	opts_serial.c_cflag &= ~CSTOPB;
	opts_serial.c_cflag &= ~CSIZE;
	opts_serial.c_cflag |= CS8;

	opts_serial.c_iflag &= ~(IXON | IXOFF | IXANY);

	opts_serial.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	opts_serial.c_oflag &= ~(ONLCR | OCRNL | OPOST);
}

void closeSerial(void) {

	close(serial);
}

unsigned int sendMessageSerial(unsigned char * _msg, unsigned int _size) {
        return write(serial, _msg, _size);
}

unsigned int receiveMessageSerial(unsigned char * _msg, unsigned int _size) {
        unsigned int nBytesL = 0;

        do {
                nBytesL += read(serial, &_msg[nBytesL], _size-nBytesL);
        } while (nBytesL < _size);

        return nBytesL;
}

