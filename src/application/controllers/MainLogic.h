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
#include "../logic/image/ImageStorage.h"
#include "../logic/image/ImageStoreItem.h"
#include "../logic/robotic_arm/RoboticArmMove.h"

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

	/*
	 * queue for images process/display
	 */
	ImageStorage *mImageStorage;

public:
	MainLogic();
	~MainLogic();

	void process();

	bool isAnyModulActive();
	void continueInActiveModule(Mat *frame, RoboticArmMove *mRoboticArmMove);
	void detectModuleToStartWith(Mat *frame, RoboticArmMove *mRoboticArmMove);

	void showArmPosition(RoboticArmMove *mRoboticArmMove);

	// MODULES
	vector<CVModule*> modules;

	// TODO testing remove later
	int xx = 0;
};



#endif /* SRC_APPLICATION_CONTROLLERS_MAINLOGIC_H_ */
