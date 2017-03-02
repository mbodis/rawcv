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
#include "../logic/image/ImageDisplayItem.h"
#include "../logic/image/ImagePreprocessItem.h"
#include "../logic/robotic_arm/RoboticArmMove.h"
#include "../../system/helper/RotateBBHelper.h"

class MainLogic{
private:
	bool DEBUG_LOCAL = false;

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
	void continueInActiveModule(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove);
	void detectModuleToStartWith(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove);

	void showArmPositionTopView(ImagePreprocessItem *ImagePreprocessItem, RoboticArmMove *mRoboticArmMove);
	void showArmPositionSideView(ImagePreprocessItem *ImagePreprocessItem, RoboticArmMove *mRoboticArmMove);

	// MODULES
	vector<CVModule*> modules;

};



#endif /* SRC_APPLICATION_CONTROLLERS_MAINLOGIC_H_ */
