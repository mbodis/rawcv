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

#include "application/config/ConfigExample.h"
#include "system/logic/input/ProcessingFacade.h"
#include "application/controllers/RoboticArmController.h"
#include "application/controllers/StateController.h"
#include "application/controllers/MyImageAnalyser.h"
#include "system/controllers/ImageAnalyser.h"
#include "testing/RoboticArmControllingTest.h"


/*
 * NOTE: do not modify !
 */
int main(int argc, char **argv) {

	cout << cv::getBuildInformation() << endl;
	ConfigExample::getInstance();


	// main logic - in thread
	StateController *mStateController = new StateController();

    // start video analyze
	(new ProcessingFacade(new MyImageAnalyser()))->runAnalyse();


	//---------------------------- until ESC pressed
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























