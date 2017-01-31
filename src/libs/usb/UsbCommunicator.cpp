/*
 * UsbCommunicator.cpp
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */

#include "UsbCommunicator.h"


UsbCommunicator::UsbCommunicator(){
	cout << "-- -- -- UsbCommunicator STARTING -- -- --" << endl;
	fd = open(USB_PORT, O_RDWR);
	if (fd == -1) {
		 perror(USB_PORT);
		 cout << "UsbCommunicator failed to open port: " << USB_PORT << " please ensure device is plugged in "<< endl;
		 throw "UsbCommunicator failed to open port";
	}

	struct termios tios;
	tcgetattr(fd, &tios);
	// disable flow control and all that, and ignore break and parity errors
	tios.c_iflag = IGNBRK | IGNPAR;
	tios.c_oflag = 0;
	tios.c_lflag = 0;
	cfsetspeed(&tios, B9600);
	tcsetattr(fd, TCSAFLUSH, &tios);

	// the serial port has a brief glitch once we turn it on which generates a
	// start bit; sleep for 1ms to let it settle
	usleep(1000);
}

UsbCommunicator::~UsbCommunicator(){

}


void UsbCommunicator::sendCommand(char* msg){
	write(fd, msg, strlen(msg));
}


/*
 * WORKING EXAMPLE
 	 int fd = open("/dev/ttyACM0", O_RDWR); //ttyAMA0
	if (fd == -1) {
	  perror("/dev/ttyACM0");//ttyAMA0
	  return 1;
	}

	struct termios tios;
	tcgetattr(fd, &tios);
	// disable flow control and all that, and ignore break and parity errors
	tios.c_iflag = IGNBRK | IGNPAR;
	tios.c_oflag = 0;
	tios.c_lflag = 0;
	cfsetspeed(&tios, B9600);
	tcsetattr(fd, TCSAFLUSH, &tios);

	// the serial port has a brief glitch once we turn it on which generates a
	// start bit; sleep for 1ms to let it settle
	usleep(1000);

	// channel [1 - 16]
	// servoLocation 100 - 2600
	// speed normal 1500
	//"#"+channel+"P"+servoLocation+"T"+servoSpeed+"\r\n"

	// output to serial port
	char msgA[] = "#1P1500T3000\r\n";//"hi there";
	write(fd, msgA, strlen(msgA));

	char msg2[] = "#2P1500T3000\r\n";//"hi there";
	write(fd, msg2, strlen(msg2));

	char msg3[] = "#3P1500T3000\r\n";//"hi there";
	write(fd, msg3, strlen(msg3));
 */


