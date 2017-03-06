/*
 * PickUpObject.h
 *
 *  Created on: Jan 31, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_MODULES_PICKUPOBJECT_H_
#define SRC_APPLICATION_MODULES_PICKUPOBJECT_H_

const int MODULE_STATE_PICKUP_OBJECT_NONE = 0;
const int MODULE_STATE_PICKUP_OBJECT_MOVE_CLOSE_TO_PROJECT = 1;
const int MODULE_STATE_PICKUP_OBJECT_ARM_PICKING_OBJECT = 2;
const int MODULE_STATE_PICKUP_OBJECT_FINISHED_SUCCESS = 3;
const int MODULE_STATE_PICKUP_OBJECT_FINISHED_FAILED = 4;

#include "iface/CVModule.h"
#include <iostream>
#include "../logic/image/ImagePreprocessItem.h"
using namespace std;

class PickUpObject : public CVModule{
private:
	bool validateInpuDetectedObject(ImagePreprocessItem *mImagePreprocessItem){

		// is there one object ?
		if (mImagePreprocessItem->detectedObjects.size() != 1){
			if (DEBUG_LOCAL) cout << "PickUpObject :: initialObjectDetection invalid number of objects" << mImagePreprocessItem->detectedObjects.size() << endl;
			return false;
		}

		// TODO check size of object

		return true;
	}

public:
	PickUpObject(string moduleName):CVModule(moduleName){
		if (DEBUG_LOCAL) cout << "-- -- -- CONSTRUCTOR: PickUpObject -- -- --" << endl;
	}

	/*
	 * initialization
	 *
	 * validate input (one object, size fits)
	 * pick up object
	 * update pickup tries counter
	 * update state
	 */
	bool initialObjectDetection(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		if (DEBUG_LOCAL) cout << "PickUpObject :: initialObjectDetection" << endl;

		if (!validateInpuDetectedObject(mImagePreprocessItem)){
			return false;
		}

		mRoboticArmMove->setObjectIndex(0);
		pickupObject(mImagePreprocessItem, mRoboticArmMove);
		// TODO save object
		setModulState(MODULE_STATE_PICKUP_OBJECT_MOVE_CLOSE_TO_PROJECT);

		return true;
	}

	/*
	 * TODO continue
	 */
	void processNextState(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		if (DEBUG_LOCAL) cout << "PickUpObject :: processNextState" << endl;
		testing(mImagePreprocessItem, mRoboticArmMove);


//		switch(this->currentState){
//
//		case MODULE_STATE_NONE:
//			if (DEBUG_LOCAL) cout << "PickUpObject :: processNextState - invalid move MODULE_STATE_NONE" << endl;
//			break;
//
//		// pick up object with claws
//		case MODULE_STATE_PICKUP_OBJECT_ARM_PICKING_OBJECT:
//			mRoboticArmMove->setServo(SERVO_IDX_CLAWS, DIRECTION_FORWARD, 1800);
//
//			// update state
//			setModulState(MODULE_STATE_PICKUP_OBJECT_ARM_PICKING_OBJECT);
//			break;
//
//		//move to default position
//		case MODULE_STATE_PICKUP_OBJECT_ARM_PICKING_OBJECT:
//			// mRoboticArmMove->setDefaultPosition(); // TODO default with
//			setModulState(MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING);
//			break;
//
//
//		case MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING:
//			cout << "start picking object" << endl;
//
//			// object has been picked up
//			if (mImagePreprocessItem->detectedObjects.size() == 0){
//				setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_SUCCESS);
//				setFinished(true);
//				// pullDownObject(mRoboticArmMove); // TODO
//
//			// object still there
//			}else{
//				pickupObject(mImagePreprocessItem, mRoboticArmMove);
//				setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_FAILED);
//				setFinished(true);
//			}
//			break;
//
//		}
	}

	/*
	 * TODO testing
	 */
	void testing(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){

		if (currentState == MODULE_STATE_PICKUP_OBJECT_MOVE_CLOSE_TO_PROJECT){
			mRoboticArmMove->setDefaultPosition();
			setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_SUCCESS);
		}

	}

	void pullDownObject(RoboticArmMove *mRoboticArmMove){
		//TODO continue
	}

	~PickUpObject(){
		if (DEBUG_LOCAL) cout << "-- -- -- DESTRUCTOR: PickUpObject -- -- --" << endl;

	}
};



#endif /* SRC_APPLICATION_MODULES_PICKUPOBJECT_H_ */
