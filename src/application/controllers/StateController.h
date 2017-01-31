/*
 * StateController.h
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_CONTROLLERS_STATECONTROLLER_H_
#define SRC_APPLICATION_CONTROLLERS_STATECONTROLLER_H_

#include <thread>

#include "../../system/controllers/ImageAnalyser.h"
#include "../logic/robotic_arm/RoboticArm.h"
#include "../config/ConfigExample.h"
#include "../modules/PickUpObject.h"


class StateController{
private:
	ConfigExample *mConfigExample;
	ImageAnalyser *mImageAnalyser;
	RoboticArm *arm;
	static void executeLogicLoop();
public:

	StateController(ConfigExample *mConfigExample, ImageAnalyser *mImageAnalyser);
	void start();
	void end();

	// MODULES
	PickUpObject *mPickUpObject;
};



#endif /* SRC_APPLICATION_CONTROLLERS_STATECONTROLLER_H_ */
