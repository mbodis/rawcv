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
 * reload application constants
 */
static char* EXAMPLE_CONFIG = "src/application/config/json/example1.json";

/*
 * setup output mode
 * [ PRINT_MODE_DEBUG, PRINT_MODE_PRESENTATION ]
 */
static int PRINT_MODE = PRINT_MODE_DEBUG;

/*
 * setup input mode
 * [INPUT_MODE_IMG_FOLDER, INPUT_MODE_VIDEO_RT, INPUT_MODE_VIDEO_FRAME, INPUT_MODE_URL, INPUT_MODE_LOCAL_CAMERA]
 */
static int INPUT_MODE = INPUT_MODE_LOCAL_CAMERA;

/*
 * NOTE: do not modify !
 */
int main2(int argc, char **argv) {

	// init
	ConfigExample mConfigExample(EXAMPLE_CONFIG, INPUT_MODE, PRINT_MODE);
	MyImageAnalyser mImageAnalyser(&mConfigExample, INPUT_MODE, PRINT_MODE);
	StateController *mStateController = new StateController(&mConfigExample, &mImageAnalyser);
	mStateController->start();

    // start arm communication
    RoboticArmController *mRoboticArmController = new RoboticArmController;
    mRoboticArmController->start();

    // start video analyze
	ProcessingFacade mProcessingFacade(&mConfigExample, &mImageAnalyser);
	mProcessingFacade.runAnalyse();

	//---------------------------- until ESC pressed

	mRoboticArmController->end();
	mStateController->end();
	cout << "-- -- -- EXIT -- -- --" << endl;

	return 0;
}

/*
 * TESTING
 */
int main(int argc, char **argv) {

	RoboticArmControllingTest *mRoboticArmControllingTest = new RoboticArmControllingTest;
	mRoboticArmControllingTest->runTest();

	return 0;
}
























