/*
 * RoboticArmControlling.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#include "RoboticArmControllingTest.h"
#include "../application/logic/robotic_arm/RoboticArmControlling.h"

void RoboticArmControllingTest::dummyTestCommunicaion(){

	RoboticArmControlling *mRoboticArmControlling = new RoboticArmControlling;
	mRoboticArmControlling->start();
	mRoboticArmControlling->addToStack("command 1");
	mRoboticArmControlling->addToStack("command 1");
	sleep(5);
	mRoboticArmControlling->addToStack("command 1");


	sleep(5);
	mRoboticArmControlling->end();
}

void RoboticArmControllingTest::testRealValues(){
	// TODO
}

void RoboticArmControllingTest::resetToDefaultPosition(){
	// TODO
}



void RoboticArmControllingTest::runTest(){
	this->dummyTestCommunicaion();

}

