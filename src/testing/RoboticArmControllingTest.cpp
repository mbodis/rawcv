/*
 * RoboticArmControlling.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#include "../application/controllers/RoboticArmController.h"
#include "../system/config/Constants.h"
#include "RoboticArmControllingTest.h"
#include "../application/logic/message/MessageComposer.h"

RoboticArmControllingTest::RoboticArmControllingTest(){
	mRoboticArmController = new RoboticArmController;
	arm = new RoboticArm();
}

void RoboticArmControllingTest::dummyTestCommunicaion(){
	mRoboticArmController->executeCommand("command 1");
	sleep(3);
	mRoboticArmController->executeCommand("command 2");
	sleep(3);
	mRoboticArmController->executeCommand("command 3");
	sleep(3);
}

void RoboticArmControllingTest::testRealValuesStepsMilis(){
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(1, 750, SERVO_SPEED_NORMAL));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(2, 1800, SERVO_SPEED_NORMAL));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(3, 1900, SERVO_SPEED_NORMAL));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(4, 1200, SERVO_SPEED_NORMAL));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(5, 2200, SERVO_SPEED_NORMAL));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(6, 2520, SERVO_SPEED_NORMAL));
	sleep(2);
}

void RoboticArmControllingTest::resetToDefaultPositionStepsMilis(){
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(1, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(2, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(3, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(4, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(5, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(6, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(2);
}

void RoboticArmControllingTest::testRealValuesAllMilis(){
	mRoboticArmController->executeCommand(MessageComposer::composeControllerFullMessage(750, 1800, 1900, 1200, 220, 2520, SERVO_SPEED_NORMAL));
	sleep(2);
}

void RoboticArmControllingTest::resetToDefaultPositionAllMilis(){
	mRoboticArmController->executeCommand(MessageComposer::composeControllerFullMessage(SERVO_POSITION_DEFAULT_MILIS,
			SERVO_POSITION_DEFAULT_MILIS, SERVO_POSITION_DEFAULT_MILIS, SERVO_POSITION_DEFAULT_MILIS, SERVO_POSITION_DEFAULT_MILIS, SERVO_POSITION_DEFAULT_MILIS, SERVO_SPEED_NORMAL));
	sleep(2);
}

void RoboticArmControllingTest::testRealValuesStepsAngle(){
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(1, arm->getServo(SERVO_IDX_BASE), DIRECTION_LEFT, 30));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(1, arm->getServo(SERVO_IDX_BASE), DIRECTION_RIGHT, 30));
	sleep(2);

	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(2, arm->getServo(SERVO_IDX_BOTTOM_JOINT), DIRECTION_FORWARD, 30));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(2, arm->getServo(SERVO_IDX_BOTTOM_JOINT), DIRECTION_BACKWARD, 30));
	sleep(2);

	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(3, arm->getServo(SERVO_IDX_MIDDLE_JOINT), DIRECTION_FORWARD, 30));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(4, arm->getServo(SERVO_IDX_UPPER_JOINT), DIRECTION_FORWARD, 30));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(5, arm->getServo(SERVO_IDX_CLAW_ROTATE), DIRECTION_LEFT, 30));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(6, arm->getServo(SERVO_IDX_CLAWS), DIRECTION_OPEN, 40));
	sleep(2);
}

void RoboticArmControllingTest::resetToDefaultPositionStepsAngle(){
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(1, arm->getServo(SERVO_IDX_BASE), DIRECTION_LEFT, 0));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(2, arm->getServo(SERVO_IDX_BOTTOM_JOINT), DIRECTION_FORWARD, 0));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(3, arm->getServo(SERVO_IDX_MIDDLE_JOINT), DIRECTION_FORWARD, 0));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(4, arm->getServo(SERVO_IDX_UPPER_JOINT), DIRECTION_FORWARD, 0));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(5, arm->getServo(SERVO_IDX_CLAW_ROTATE), DIRECTION_LEFT, 0));
	sleep(2);
	mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(6, arm->getServo(SERVO_IDX_CLAWS), DIRECTION_OPEN, 0));
	sleep(2);
}

void RoboticArmControllingTest::testRealValuesAllAngle(){

	mRoboticArmController->executeCommand(MessageComposer::composeControllerFullMessage(
		arm->getServo(SERVO_IDX_BASE), DIRECTION_RIGHT, 0,
		arm->getServo(SERVO_IDX_BOTTOM_JOINT), DIRECTION_FORWARD, 30,
		arm->getServo(SERVO_IDX_MIDDLE_JOINT), DIRECTION_FORWARD, 60,
		arm->getServo(SERVO_IDX_UPPER_JOINT), DIRECTION_FORWARD, 70,
		arm->getServo(SERVO_IDX_CLAW_ROTATE), DIRECTION_LEFT, 0,
		arm->getServo(SERVO_IDX_CLAWS), DIRECTION_OPEN, 420
		));
	sleep(2);

}

void RoboticArmControllingTest::resetToDefaultPositionAllAngle(){
	mRoboticArmController->executeCommand(MessageComposer::composeControllerFullMessage(
		arm->getServo(SERVO_IDX_BASE), DIRECTION_LEFT, 0,
		arm->getServo(SERVO_IDX_BOTTOM_JOINT), DIRECTION_FORWARD, 0,
		arm->getServo(SERVO_IDX_MIDDLE_JOINT), DIRECTION_FORWARD, 0,
		arm->getServo(SERVO_IDX_UPPER_JOINT), DIRECTION_FORWARD, 0,
		arm->getServo(SERVO_IDX_CLAW_ROTATE), DIRECTION_LEFT, 0,
		arm->getServo(SERVO_IDX_CLAWS), DIRECTION_OPEN, 0
		));
	sleep(2);
}

void RoboticArmControllingTest::waving(){

	for(int i=0; i< 5;i++){
		// forward
		mRoboticArmController->executeCommand(MessageComposer::composeControllerFullMessage(
		arm->getServo(SERVO_IDX_BASE), DIRECTION_LEFT, 0,
		arm->getServo(SERVO_IDX_BOTTOM_JOINT), DIRECTION_BACKWARD, 40,
		arm->getServo(SERVO_IDX_MIDDLE_JOINT), DIRECTION_FORWARD, 60,
		arm->getServo(SERVO_IDX_UPPER_JOINT), DIRECTION_FORWARD, 30,
		arm->getServo(SERVO_IDX_CLAW_ROTATE), DIRECTION_RIGHT, 0,
		arm->getServo(SERVO_IDX_CLAWS), DIRECTION_OPEN, 0
		));
        sleep(2);

		// backward
		mRoboticArmController->executeCommand(MessageComposer::composeControllerFullMessage(
		arm->getServo(SERVO_IDX_BASE), DIRECTION_LEFT, 0,
		arm->getServo(SERVO_IDX_BOTTOM_JOINT), DIRECTION_BACKWARD, 40,
		arm->getServo(SERVO_IDX_MIDDLE_JOINT), DIRECTION_FORWARD, 60,
		arm->getServo(SERVO_IDX_UPPER_JOINT), DIRECTION_FORWARD, 60,
		arm->getServo(SERVO_IDX_CLAW_ROTATE), DIRECTION_RIGHT, 0,
		arm->getServo(SERVO_IDX_CLAWS), DIRECTION_OPEN, 0
		));
        sleep(2);
	}
}

/**
 * note: in case source is set to 5v ar will fall down if bas joint has higher value
 * make sure source si set to 7.5V
 */
void RoboticArmControllingTest::voltageTest(){
    mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(2, arm->getServo(SERVO_IDX_BOTTOM_JOINT), DIRECTION_FORWARD, 50));
    sleep(2);
    mRoboticArmController->executeCommand(MessageComposer::composeControllerMessage(2, arm->getServo(SERVO_IDX_BOTTOM_JOINT), DIRECTION_BACKWARD, 30));
    sleep(2);
}

void RoboticArmControllingTest::runTest(){
	mRoboticArmController->start();

    resetToDefaultPositionAllAngle();

    resetToDefaultPositionAllAngle();
	dummyTestCommunicaion();

	testRealValuesStepsMilis();
	resetToDefaultPositionStepsMilis();

	testRealValuesAllMilis();
	resetToDefaultPositionAllMilis();

	testRealValuesStepsAngle();
	resetToDefaultPositionStepsAngle();

	testRealValuesAllAngle();
	resetToDefaultPositionAllAngle();

	waving();
	resetToDefaultPositionAllAngle();

    voltageTest();
    resetToDefaultPositionAllAngle();

	sleep(4);
	mRoboticArmController->end();
}

