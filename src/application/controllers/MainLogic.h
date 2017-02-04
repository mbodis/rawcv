/*
 * MainLogic.h
 *
 *  Created on: Feb 4, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_CONTROLLERS_MAINLOGIC_H_
#define SRC_APPLICATION_CONTROLLERS_MAINLOGIC_H_

#include "../../system/controllers/ImageAnalyser.h"
#include "../logic/robotic_arm/RoboticArm.h"
#include "../config/ConfigExample.h"
#include "../modules/iface/CVModule.h"
#include "../modules/PickUpObject.h"
#include "RoboticArmController.h"
#include "../../system/helper/FileSystemHelper.h"

class MainLogic{
private:
	bool DEBUG_LOCAL = true;
	Mat activeFrame;
	string lastFramePath = "";

	/*
	 * servo setup for robotic arm
	 */
	RoboticArm *arm;

	/*
	 * stack with separate thread to send messages via USB
	 */
	RoboticArmController *mRoboticArmController;
public:

	MainLogic();
	~MainLogic();

	void process();
	string getNextFile();

	bool isAnyModulActive();
	void continueInActiveModule(Mat *frame);
	void detectModuleToStartWith(Mat *frame);

	void showArmPosition();

	// MODULES
	CVModule *modules[1];
};



#endif /* SRC_APPLICATION_CONTROLLERS_MAINLOGIC_H_ */
