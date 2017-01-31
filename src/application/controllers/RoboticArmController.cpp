/*
 * RoboticArmControlling.cpp
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */



#include "RoboticArmController.h"

static thread t;
std::stack<string> armCommandsStack;                    // empty stack
bool readNewCommand = false;
UsbCommunicator *mUsbCommunicator;


void RoboticArmController::start(){
	readNewCommand = true;
	mUsbCommunicator = new UsbCommunicator();
	t = thread(executeCommandsLoop);
}

void RoboticArmController::end(){
	mUsbCommunicator->~UsbCommunicator();
	readNewCommand = false;
	cout << "-- -- -- ROBOTIC ARM CONTROLLING ENDING -- -- --" << endl;
}

void RoboticArmController::executeCommandsLoop(){
	while (readNewCommand){
		if (armCommandsStack.size() >0){
			string newCommand = armCommandsStack.top();
			cout << "executing command: " << newCommand << endl;
			mUsbCommunicator->sendCommand((char*)newCommand.c_str());
			armCommandsStack.pop();

			sleep(TIME_DELAY_WAIT_AFTER_COMMAND);
		}else{
			sleep(TIME_DELAY_WAIT_FOR_NEW_COMMAND);
		}
	}

	cout << "-- -- -- ROBOTIC ARM CONTROLLING ENDED -- -- --" << endl;
	t.join();
}


void RoboticArmController::addToStack(string newCommand){
	armCommandsStack.push(newCommand);
}

int RoboticArmController::getStackSize(){
	return armCommandsStack.size();
}
























