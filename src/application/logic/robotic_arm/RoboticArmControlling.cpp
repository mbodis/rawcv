/*
 * RoboticArmControlling.cpp
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */



#include "RoboticArmControlling.h";

static thread t;
std::stack<char*> armCommandsStack;                    // empty stack
bool readNewCommand = false;
UsbCommunicator *mUsbCommunicator;


void RoboticArmControlling::start(){
	readNewCommand = true;
	mUsbCommunicator = new UsbCommunicator();
	t = thread(executeCommandsLoop);
}

void RoboticArmControlling::end(){
	mUsbCommunicator->~UsbCommunicator();
	readNewCommand = false;
	cout << "-- -- -- ROBOTIC ARM CONTROLLING ENDING -- -- --" << endl;
}

void RoboticArmControlling::executeCommandsLoop(){
	while (readNewCommand){
		if (armCommandsStack.size() >0){
			char* newCommand = armCommandsStack.top();
			cout << "executing command: " << newCommand << endl;
			mUsbCommunicator->sendCommand(newCommand);
			armCommandsStack.pop();

			sleep(TIME_DELAY_WAIT_AFTER_COMMAND);
		}else{
			sleep(TIME_DELAY_WAIT_FOR_NEW_COMMAND);
		}
	}

	cout << "-- -- -- ROBOTIC ARM CONTROLLING ENDED -- -- --" << endl;
	t.join();
}


void RoboticArmControlling::addToStack(char* newCommand){
	armCommandsStack.push(newCommand);
}

int RoboticArmControlling::getStackSize(){
	return armCommandsStack.size();
}
























