/*
 * RoboticArmControlling.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#include "../application/controllers/RoboticArmController.h"
#include "../application/logic/usb_controller/MessageComposer.h"
#include "../system/config/Constants.h"
#include "RoboticArmControllingTest.h"

RoboticArmControllingTest::RoboticArmControllingTest(){
	mRoboticArmController = new RoboticArmController;
	arm = new RoboticArm();
}

void RoboticArmControllingTest::dummyTestCommunicaion(){
	mRoboticArmController->addToStack("command 1");
	sleep(3);
	mRoboticArmController->addToStack("command 2");
	sleep(3);
	mRoboticArmController->addToStack("command 3");
	sleep(3);
}

void RoboticArmControllingTest::testRealValuesStepsMilis(){
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(1, 750, SERVO_SPEED_NORMAL));
	sleep(1);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(2, 1800, SERVO_SPEED_NORMAL));
	sleep(1);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(3, 1900, SERVO_SPEED_NORMAL));
	sleep(1);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(4, 1200, SERVO_SPEED_NORMAL));
	sleep(1);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(5, 2200, SERVO_SPEED_NORMAL));
	sleep(1);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(6, 2520, SERVO_SPEED_NORMAL));
	sleep(1);
}

void RoboticArmControllingTest::resetToDefaultPositionStepsMilis(){
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(1, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(1);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(2, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(1);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(3, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(1);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(4, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(1);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(5, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(1);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(6, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(1);
}

void RoboticArmControllingTest::testRealValuesAllMilis(){
	mRoboticArmController->addToStack(MessageComposer::composeControllerFullMessage(750, 1800, 1900, 1200, 220, 2520, SERVO_SPEED_NORMAL));
	sleep(2);
}

void RoboticArmControllingTest::resetToDefaultPositionAllMilis(){
	mRoboticArmController->addToStack(MessageComposer::composeControllerFullMessage(SERVO_POSITION_DEFAULT_MILIS,
			SERVO_POSITION_DEFAULT_MILIS, SERVO_POSITION_DEFAULT_MILIS, SERVO_POSITION_DEFAULT_MILIS, SERVO_POSITION_DEFAULT_MILIS, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(2);
}

void RoboticArmControllingTest::testRealValuesStepsAngle(){
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_BASE), DIRECTION_LEFT, 30));
	sleep(2);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_BASE), DIRECTION_RIGHT, 30));
	sleep(2);

	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_FORWARD, 30));
	sleep(2);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_BACKWARD, 30));
	sleep(2);

	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_MIDDLE_JOINT), DIRECTION_FORWARD, 30));
	sleep(2);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_UPPER_JOINT), DIRECTION_FORWARD, 30));
	sleep(2);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_CLAW_ROTATE), DIRECTION_LEFT, 30));
	sleep(2);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_CLAWS), DIRECTION_OPEN, 300));
	sleep(2);
}

void RoboticArmControllingTest::resetToDefaultPositionStepsAngle(){
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_BASE), DIRECTION_LEFT, 0));
	sleep(2);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_FORWARD, 0));
	sleep(2);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_MIDDLE_JOINT), DIRECTION_FORWARD, 0));
	sleep(2);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_UPPER_JOINT), DIRECTION_FORWARD, 0));
	sleep(2);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_CLAW_ROTATE), DIRECTION_LEFT, 0));
	sleep(2);
	mRoboticArmController->addToStack(MessageComposer::composeControllerMessage(arm->getServo(SERVO_CLAWS), DIRECTION_OPEN, 0));
	sleep(2);
}

void RoboticArmControllingTest::testRealValuesAllAngle(){

	mRoboticArmController->addToStack(MessageComposer::composeControllerFullMessage(
		arm->getServo(SERVO_BASE), DIRECTION_RIGHT, 0,
		arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_FORWARD, 30,
		arm->getServo(SERVO_MIDDLE_JOINT), DIRECTION_FORWARD, 60,
		arm->getServo(SERVO_UPPER_JOINT), DIRECTION_FORWARD, 70,
		arm->getServo(SERVO_CLAW_ROTATE), DIRECTION_LEFT, 0,
		arm->getServo(SERVO_CLAWS), DIRECTION_OPEN, 420
		));
	sleep(2);

}

void RoboticArmControllingTest::resetToDefaultPositionAllAngle(){
	mRoboticArmController->addToStack(MessageComposer::composeControllerFullMessage(
		arm->getServo(SERVO_BASE), DIRECTION_LEFT, 0,
		arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_FORWARD, 0,
		arm->getServo(SERVO_MIDDLE_JOINT), DIRECTION_FORWARD, 0,
		arm->getServo(SERVO_UPPER_JOINT), DIRECTION_FORWARD, 0,
		arm->getServo(SERVO_CLAW_ROTATE), DIRECTION_LEFT, 0,
		arm->getServo(SERVO_CLAWS), DIRECTION_OPEN, 0
		));
	sleep(2);
}

void RoboticArmControllingTest::stupido(){
	int claw = 30;
//	mRoboticArmController->addToStack(MessageComposer::composeControllerFullMessage(
//	arm->getServo(SERVO_BASE), DIRECTION_RIGHT, 0,
//	arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_FORWARD, 0,
//	arm->getServo(SERVO_MIDDLE_JOINT), DIRECTION_FORWARD, 0,
//	arm->getServo(SERVO_UPPER_JOINT), DIRECTION_FORWARD, 0,
//	arm->getServo(SERVO_CLAW_ROTATE), DIRECTION_LEFT, 0,
//	arm->getServo(SERVO_CLAWS), DIRECTION_OPEN, 420
//	));
//	sleep(2);
//
//	mRoboticArmController->addToStack(MessageComposer::composeControllerFullMessage(
//		arm->getServo(SERVO_BASE), DIRECTION_RIGHT, 0,
//		arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_FORWARD, 0,
//		arm->getServo(SERVO_MIDDLE_JOINT), DIRECTION_FORWARD, 0,
//		arm->getServo(SERVO_UPPER_JOINT), DIRECTION_FORWARD, 0,
//		arm->getServo(SERVO_CLAW_ROTATE), DIRECTION_LEFT, 0,
//		arm->getServo(SERVO_CLAWS), DIRECTION_OPEN, claw
//		));
//		sleep(2);

	mRoboticArmController->addToStack(MessageComposer::composeControllerFullMessage(
	arm->getServo(SERVO_BASE), DIRECTION_RIGHT, 0,
	arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_BACKWARD, 40,
	arm->getServo(SERVO_MIDDLE_JOINT), DIRECTION_FORWARD, 60,
	arm->getServo(SERVO_UPPER_JOINT), DIRECTION_FORWARD, 60,
	arm->getServo(SERVO_CLAW_ROTATE), DIRECTION_LEFT, 0,
	arm->getServo(SERVO_CLAWS), DIRECTION_OPEN, claw
	));
	sleep(2);

	for(int i=0; i< 2;i++){

		//left
		mRoboticArmController->addToStack(MessageComposer::composeControllerFullMessage(
		arm->getServo(SERVO_BASE), DIRECTION_LEFT, 20,
		arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_BACKWARD, 40,
		arm->getServo(SERVO_MIDDLE_JOINT), DIRECTION_FORWARD, 60,
		arm->getServo(SERVO_UPPER_JOINT), DIRECTION_FORWARD, 60,
		arm->getServo(SERVO_CLAW_ROTATE), DIRECTION_RIGHT, 0,
		arm->getServo(SERVO_CLAWS), DIRECTION_OPEN, claw
		));
		sleep(4);

		//top
		mRoboticArmController->addToStack(MessageComposer::composeControllerFullMessage(
		arm->getServo(SERVO_BASE), DIRECTION_LEFT, 0,
		arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_BACKWARD, 40,
		arm->getServo(SERVO_MIDDLE_JOINT), DIRECTION_FORWARD, 55,
		arm->getServo(SERVO_UPPER_JOINT), DIRECTION_FORWARD, 60,
		arm->getServo(SERVO_CLAW_ROTATE), DIRECTION_RIGHT, 0,
		arm->getServo(SERVO_CLAWS), DIRECTION_OPEN, claw
		));
		sleep(4);

		//right
		mRoboticArmController->addToStack(MessageComposer::composeControllerFullMessage(
		arm->getServo(SERVO_BASE), DIRECTION_RIGHT, 20,
		arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_BACKWARD, 40,
		arm->getServo(SERVO_MIDDLE_JOINT), DIRECTION_FORWARD, 60,
		arm->getServo(SERVO_UPPER_JOINT), DIRECTION_FORWARD, 60,
		arm->getServo(SERVO_CLAW_ROTATE), DIRECTION_LEFT, 0,
		arm->getServo(SERVO_CLAWS), DIRECTION_OPEN, claw
		));
		sleep(4);

		//down
		mRoboticArmController->addToStack(MessageComposer::composeControllerFullMessage(
		arm->getServo(SERVO_BASE), DIRECTION_LEFT, 0,
		arm->getServo(SERVO_BOTTOM_JOINT), DIRECTION_BACKWARD, 40,
		arm->getServo(SERVO_MIDDLE_JOINT), DIRECTION_FORWARD, 65,
		arm->getServo(SERVO_UPPER_JOINT), DIRECTION_FORWARD, 65,
		arm->getServo(SERVO_CLAW_ROTATE), DIRECTION_RIGHT, 0,
		arm->getServo(SERVO_CLAWS), DIRECTION_OPEN, claw
		));
		sleep(4);
	}

	resetToDefaultPositionAllAngle();
}

void RoboticArmControllingTest::runTest(){
	mRoboticArmController->start();

//	testRealValuesStepsMilis();
//	resetToDefaultPositionStepsMilis();

//	testRealValuesAllMilis();
//	resetToDefaultPositionAllMilis();

//	testRealValuesStepsAngle();
//	resetToDefaultPositionStepsAngle();

//	testRealValuesAllAngle();
//	resetToDefaultPositionAllAngle();

//	stupido();
	resetToDefaultPositionAllAngle();

	mRoboticArmController->end();

}

