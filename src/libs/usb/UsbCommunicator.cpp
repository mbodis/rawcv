/*
 * UsbCommunicator.cpp
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */

#include "UsbCommunicator.h"


UsbCommunicator::UsbCommunicator(){
	MyLog::log(LOG_INFO, TAG, "-- -- -- CONSTRUCTOR: UsbCommunicator-- -- --");

	AppConfig &mAppConfig = AppConfig::getInstance();
	USB_PORT = (char*)mAppConfig.USB_PORT.c_str();

	fd = open(USB_PORT, O_RDWR);
	if (fd == -1) {
		 perror(USB_PORT);
		 MyLog::log(LOG_ERROR, TAG, "UsbCommunicator failed to open port: " + mAppConfig.USB_PORT + " please ensure device is plugged in ");
		 //throw std::logic_error("UsbCommunicator failed to open port");
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
	MyLog::log(LOG_INFO, TAG, "-- -- -- DESTRUCTOR: UsbCommunicator-- -- --");

}

void UsbCommunicator::sendCommand(char* msg){
	write(fd, msg, strlen(msg));
}

