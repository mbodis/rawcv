/*
 * StateController.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#include "../controllers/StateController.h"
#include "../logic/robotic_arm/RoboticArm.h"

#include <unistd.h>

thread t;
bool isStateControllerAlive = false;

StateController::StateController(){


	isStateControllerAlive = true;
	t = thread(executeLogicLoop);
}

StateController::~StateController(){
	cout << "-- -- -- DESTRUCTOR: StateController -- -- --" << endl;
	isStateControllerAlive = false;
}

void StateController::executeLogicLoop(){

	MainLogic *mMainLogic = new MainLogic();

	while (isStateControllerAlive){
		mMainLogic->process();
		usleep(500);
	}

	delete mMainLogic;

	cout << "-- -- -- STATE CONTROLLER ENDED -- -- --" << endl;
	t.join();
}

