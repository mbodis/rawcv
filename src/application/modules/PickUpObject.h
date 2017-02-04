/*
 * PickUpObject.h
 *
 *  Created on: Jan 31, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_MODULES_PICKUPOBJECT_H_
#define SRC_APPLICATION_MODULES_PICKUPOBJECT_H_

const int MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING = 2;
const int MODULE_STATE_PICKUP_OBJECT_PICKED_UP = 3;

#include "iface/CVModule.h"
#include <iostream>
using namespace std;

class PickUpObject : public CVModule{
private:

public:
	PickUpObject(string moduleName):CVModule(moduleName){
		cout << "-- -- -- CONSTRUCTOR: PickUpObject -- -- --" << endl;
		this->numberStates = 3;
	}

	bool initialObjectDetection(Mat *newFrame){
		cout << "initialObjectDetection" << endl;
		return true; // TODO
	}

	void processNextState(Mat *newFrame){
		switch(this->currentState){

		case MODULE_STATE_NONE:
			break;
		case MODULE_STATE_START:
			cout << "start picking object" << endl;
			// TODO start picking the object
			break;
		case MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING:
			// TODO yes / no
		case MODULE_STATE_PICKUP_OBJECT_PICKED_UP:
			// TODO wait - finished
			break;
		}

		checkIfModuleHasEnded();
	}

	void resetContent(){
		// TODO
	}

	~PickUpObject(){
		cout << "-- -- -- DESTRUCTOR: PickUpObject -- -- --" << endl;

	}
};



#endif /* SRC_APPLICATION_MODULES_PICKUPOBJECT_H_ */
