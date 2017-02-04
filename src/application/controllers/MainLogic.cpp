/*
 * MainLogic.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: mbodis
 */

#include "MainLogic.h"


MainLogic::MainLogic(){
	cout << "-- -- -- CONSTRUCTOR: MainLogic -- -- --" << endl;

	// start arm communication
	this->mRoboticArmController = new RoboticArmController;
	this->mRoboticArmController->start();

	// setup ARM
	this->arm = new RoboticArm();

	// modules
	this->mPickUpObject = new PickUpObject;
}


MainLogic::~MainLogic(){
	cout << "-- -- -- DESTRUCTOR: MainLogic -- -- --" << endl;
	this->mRoboticArmController->end();
}

void MainLogic::process(){
	this->mRoboticArmController->addToStack("test message");
//	this->nextFile = getNextFile();
//	if ()
}


string MainLogic::getNextFile(){
	return "";
}

void MainLogic::processFileThroughModules(){

}
