/*
 * StateController.h
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_CONTROLLERS_STATECONTROLLER_H_
#define SRC_APPLICATION_CONTROLLERS_STATECONTROLLER_H_

#include "../../system/controllers/ImageAnalyser.h"
#include "../config/ConfigExample.h"
#include <thread>

class StateController{
private:

	ConfigExample *mConfigExample;
	ImageAnalyser *mImageAnalyser;
	static void executeLogicLoop();
public:
	StateController(ConfigExample *mConfigExample, ImageAnalyser *mImageAnalyser);
	void start();
	void end();
};



#endif /* SRC_APPLICATION_CONTROLLERS_STATECONTROLLER_H_ */
