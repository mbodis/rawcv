/*
 * StateController.h
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_CONTROLLERS_STATECONTROLLER_H_
#define SRC_APPLICATION_CONTROLLERS_STATECONTROLLER_H_

#include <thread>
#include <unistd.h>

#include "../../system/controllers/ImageAnalyser.h"
#include "../logic/robotic_arm/RoboticArm.h"
#include "../config/AppConfig.h"
#include "../modules/PickUpObject.h"
#include "RoboticArmController.h"
#include "MainLogic.h"


class StateController{
private:
	static void executeLogicLoop();
public:
	StateController();
	~StateController();
};



#endif /* SRC_APPLICATION_CONTROLLERS_STATECONTROLLER_H_ */
