/*
 * main.cpp
 *
 *  Created on: Jan 6, 2017
 *      Author: mbodis
 */

/*
 *
 *
 * setup: set following
 *      EXAMPLE_CONFIG - json file required
 *      PRINT_MODE - debug/presentation
 *      INPUT_MODE - img/folder/video/url
 *
 */

using namespace std;

#include "application/config/AppConfig.h"
#include "system/logic/input/ProcessingFacade.h"
#include "application/controllers/RoboticArmController.h"
#include "application/controllers/StateController.h"
#include "application/controllers/MyImageAnalyser.h"
#include "system/controllers/ImageAnalyser.h"
#include "testing/RoboticArmControllingTest.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*
 * NOTE: do not modify !
 */
int main(int argc, char **argv) {

	cout << cv::getBuildInformation() << endl;
	AppConfig &mAppConfig = AppConfig::getInstance();
	mAppConfig.CAMERA_IDX = 1; // TODO changing param !!

	// main logic
	StateController *mStateController = new StateController();

    // start video analyze on main thread - until ESC pressed
	(new ProcessingFacade(new MyImageAnalyser()))->runAnalyse();

	delete mStateController;
	cout << "-- -- -- EXIT -- -- --" << endl;

	return 0;
}

/*
 * TESTING CONTROLING ROBOTIC ARM
 */
int main2(int argc, char **argv) {

	RoboticArmControllingTest *mRoboticArmControllingTest = new RoboticArmControllingTest;
	mRoboticArmControllingTest->runTest();

	return 0;
}























