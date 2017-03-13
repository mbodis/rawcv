/*
 * RoboticArmControlling.cpp
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */

#include "RoboticArmController.h"

class ControllerMessage{
	private:
		string message = "";
		string key = "";

	public:
		ControllerMessage(string message){
			this->message = message;
		}
		ControllerMessage(string message, string key){
			this->message = message;
			this->key = key;
		}
		string getKey(){
			return key;
		}
		string getMessage(){
			return message;
		}
};

static thread t;
std::queue<ControllerMessage> armCommandsStack;                    // empty stack
bool readNewCommand = false;
UsbCommunicator *mUsbCommunicator;
StatusMessageStorage *mStatusMessageStorage = &StatusMessageStorage::getInstance();


void RoboticArmController::start(){
	readNewCommand = true;
	mUsbCommunicator = new UsbCommunicator();
	mStatusMessageStorage = &StatusMessageStorage::getInstance();
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
			ControllerMessage newCommand = armCommandsStack.front();
			cout << "executing command: " << newCommand.getMessage() << " " << newCommand.getKey() << endl;
			mUsbCommunicator->sendCommand((char*)newCommand.getMessage().c_str());

			// set message as progress
			if (newCommand.getKey().length() > 0){
				mStatusMessageStorage->setStatusMessageByKey(newCommand.getKey(), MSG_STATUS_IN_PROGRESS, SERVO_SPEED_NORMAL + 3000);
			}

			armCommandsStack.pop();

			sleep(TIME_DELAY_WAIT_AFTER_COMMAND);
		}else{
			sleep(TIME_DELAY_WAIT_FOR_NEW_COMMAND);
		}

		// has any message exit
		mStatusMessageStorage->checkMessages();

	}

	cout << "-- -- -- ROBOTIC ARM CONTROLLING ENDED -- -- --" << endl;
	t.join();
}


void RoboticArmController::executeCommand(string newCommand){
	armCommandsStack.push(ControllerMessage(newCommand));
}

void RoboticArmController::executeCommandWithCallback(string newCommand, string pendingMessageKey){
	armCommandsStack.push(ControllerMessage(newCommand, pendingMessageKey));
	mStatusMessageStorage->addToStatusMessageQueue(StatusMessageItem(MSG_STATUS_INIT, pendingMessageKey));
}


/*
 * check if message exists and status is finished
 */
bool RoboticArmController::wasCommandExecuted(string messageKey){
	// message found - status finished
	if (mStatusMessageStorage->getStatusMessageByKey(messageKey) == MSG_STATUS_FINISHED){
		mStatusMessageStorage->removeStatusMessage(messageKey);
		return true;
	}

	// message not found
	if (mStatusMessageStorage->getStatusMessageByKey(messageKey) == MSG_STATUS_UNKNOWN){
		return true;
	}

	// message found - status not finished yet
	return false;
}

int RoboticArmController::getStackSize(){
	return armCommandsStack.size();
}
























