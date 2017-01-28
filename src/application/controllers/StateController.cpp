/*
 * StateController.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#include "../controllers/StateController.h"

thread t;
bool isStateControllerAlive = false;

StateController::StateController(ConfigExample *mConfigExample, ImageAnalyser *mImageAnalyser){
	this->mConfigExample = mConfigExample;
	this->mImageAnalyser = mImageAnalyser;
}

void StateController::start(){
	isStateControllerAlive = true;
	t = thread(executeLogicLoop);
}

void StateController::end(){
	isStateControllerAlive = false;
}

void StateController::executeLogicLoop(){

	while (isStateControllerAlive){
		// TODO logic
	}

	cout << "-- -- -- STATE CONTROLLER ENDED -- -- --" << endl;
	t.join();
}
