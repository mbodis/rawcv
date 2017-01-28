/*
 * RoboticArmControlling.h
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMCONTROLLING_H_
#define SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMCONTROLLING_H_

#include <stack>          // std::stack
#include <thread>
#include "../usb/UsbCommunicator.h"
using namespace std;


static double TIME_DELAY_WAIT_FOR_NEW_COMMAND = 0.5; // seconds
static double TIME_DELAY_WAIT_AFTER_COMMAND = 2.0; // seconds

class RoboticArmControlling {
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
	void addToStack(char*);
	int getStackSize();


};


#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMCONTROLLING_H_ */
