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
#include "../modules/PickUpObject.h"
#include "RoboticArmController.h"

class MainLogic{
private:
	RoboticArm *arm;
	RoboticArmController *mRoboticArmController;
public:

	MainLogic();
	~MainLogic();

	void process();
	string getNextFile();
	void processFileThroughModules();

	// MODULES
	PickUpObject *mPickUpObject;
};



#endif /* SRC_APPLICATION_CONTROLLERS_MAINLOGIC_H_ */
