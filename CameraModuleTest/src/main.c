#include <stdio.h>
#include "serial.h"
#include "piximProtocol.h"

int main() {

	unsigned int errorL;
	unsigned int rL;

	unsigned char message[] = {0x03, 0x02, 0x00, 0x25, 0xD5};

	openSerial();

	rL = getProperty(0x32, &errorL);
	if (! errorL)
		printf("Property = %x\n", rL);
	else
		printf("error = %d\n", errorL);

 	//writeMsgSerial(vet,5);

	closeSerial();

	return 0;
}

