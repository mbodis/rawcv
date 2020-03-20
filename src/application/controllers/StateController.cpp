/*
 * StateController.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#include "StateController.h"

thread t;
bool isStateControllerAlive = false;

StateController::StateController(){


	isStateControllerAlive = true;
	t = thread(executeLogicLoop);
}

StateController::~StateController(){
	MyLog::log(LOG_INFO, "StateController", "-- -- -- DESTRUCTOR: StateController -- -- --");
	isStateControllerAlive = false;
}

void StateController::executeLogicLoop(){

	MainLogic *mMainLogic = new MainLogic();

	while (isStateControllerAlive){
		mMainLogic->process();
		usleep(500);
	}

	delete mMainLogic;

	MyLog::log(LOG_INFO, "StateController", "-- -- -- STATE CONTROLLER ENDED -- -- --");
	t.join();
}

