/*
 * RoboticArmControllingTest.h
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#ifndef SRC_TESTING_ROBOTICARMCONTROLLINGTEST_H_
#define SRC_TESTING_ROBOTICARMCONTROLLINGTEST_H_

#include "abstract/TestIface.h"
#include "../application/logic/servo/ServoIface.h"
#include "../application/logic/servo/ServoRotationFB.h"
#include "../application/logic/servo/ServoRotationLR.h"
#include "../application/logic/servo/ServoRange.h"
#include "../application/logic/robotic_arm/RoboticArm.h"

#include <iostream>
#include <stdio.h>

using namespace std;

class RoboticArmControllingTest  : TestIface{
private:
	void dummyTestCommunicaion();

	void testRealValuesStepsMilis();
	void resetToDefaultPositionStepsMilis();

	void testRealValuesAllMilis();
	void resetToDefaultPositionAllMilis();

	void testRealValuesStepsAngle();
	void resetToDefaultPositionStepsAngle();

	void testRealValuesAllAngle();
	void resetToDefaultPositionAllAngle();

	void waving();
	void voltageTest();

	RoboticArmController *mRoboticArmController;
	RoboticArm *arm;

public:
	RoboticArmControllingTest();
	void runTest();
};


#endif /* SRC_TESTING_ROBOTICARMCONTROLLINGTEST_H_ */
