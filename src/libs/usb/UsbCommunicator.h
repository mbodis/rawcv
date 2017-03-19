/*
 * UsbCommunicator.h
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_USB_USBCOMMUNICATOR_H_
#define SRC_APPLICATION_LOGIC_USB_USBCOMMUNICATOR_H_

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include "../../system/helper/MyLog.h"

#include "../../application/config/AppConfig.h"

using namespace std;

static char* USB_PORT;

class UsbCommunicator {
private:
	string TAG = "UsbCommunicator";
	int fd;

public:
	UsbCommunicator();
	~UsbCommunicator();

	void sendCommand(char *);
};



#endif /* SRC_APPLICATION_LOGIC_USB_USBCOMMUNICATOR_H_ */
