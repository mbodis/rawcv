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
#include "system/helper/MyLog.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*
 * NOTE: do not modify !
 */
int main(int argc, char **argv) {
	cout << cv::getBuildInformation() << endl;
	AppConfig &mAppConfig = AppConfig::getInstance();
	mAppConfig.CAMERA_IDX = 2;
	mAppConfig.USB_PORT = "/dev/ttyACM0";

	// main logic
	StateController *mStateController = new StateController();

    // start video analyze on main thread - until ESC pressed
	(new ProcessingFacade(new MyImageAnalyser()))->runAnalyse();

	delete mStateController;
	MyLog::log(LOG_INFO, "main", "-- -- -- EXIT -- -- --");

	return 0;
}

/*
 * TESTING CONTROLING ROBOTIC ARM
 */
int main3(int argc, char **argv) {

	RoboticArmControllingTest *mRoboticArmControllingTest = new RoboticArmControllingTest;
	mRoboticArmControllingTest->runTest();

	return 0;
}