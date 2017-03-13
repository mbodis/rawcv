/*
 * RoboticArmControlling.h
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMCONTROLLER_H_
#define SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMCONTROLLER_H_

#include <queue>
#include <thread>
#include "../../libs/usb/UsbCommunicator.h"
#include "../logic/message/StatusMessageStorage.h"
#include "../logic/message/StatusMessageItem.h"
using namespace std;


static double TIME_DELAY_WAIT_FOR_NEW_COMMAND = 0.5; // seconds
static double TIME_DELAY_WAIT_AFTER_COMMAND = 2.0; // seconds

class RoboticArmController {
private:

public:

	/*
	 * start thread that reads and executes commands
	 */
	void start();

	/*
	 * end thread that executes commands
	 */
	void end();

	static void executeCommandsLoop();
	void executeCommand(string);
	void executeCommandWithCallback(string, string);
	bool wasCommandExecuted(string);
	int getStackSize();


};


#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMCONTROLLER_H_ */
