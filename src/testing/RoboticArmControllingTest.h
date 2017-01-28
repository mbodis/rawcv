/*
 * RoboticArmControllingTest.h
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#ifndef SRC_TESTING_ROBOTICARMCONTROLLINGTEST_H_
#define SRC_TESTING_ROBOTICARMCONTROLLINGTEST_H_

#include "abstract/TestIface.h"

#include <iostream>
#include <stdio.h>

using namespace std;

class RoboticArmControllingTest  : TestIface{
private:
	void dummyTestCommunicaion();
	void testRealValues();
	void resetToDefaultPosition();

public:
	void runTest();
};




#endif /* SRC_TESTING_ROBOTICARMCONTROLLINGTEST_H_ */
