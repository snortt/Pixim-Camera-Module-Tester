/*
 * serial.h
 *
 * Manipulates the serial port
 *
 * Created on: 20/05/2009
 *
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Usable devices for this program */
#define SERIAL_USB0 "/dev/ttyUSB0"
#define SERIAL_PORT0 "/dev/ttyS0"
#define SERIAL_PORT1 "/dev/ttyS1"

/* Speed */
#define BAUDRATE B38400

/* Tamanho do buffer de leitura */
#define TAMBUFF 128

void openSerial(void);

void configSerial(unsigned int _serial);

void closeSerial(void);

unsigned int sendMessageSerial(unsigned char * _msg, unsigned int _size);

unsigned int receiveMessageSerial(unsigned char * _msg, unsigned int _size);

#endif /* SERIAL_H_ */

