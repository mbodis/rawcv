/*
 * PickUpObject.h
 *
 *  Created on: Jan 31, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_MODULES_PICKUPOBJECT_H_
#define SRC_APPLICATION_MODULES_PICKUPOBJECT_H_

const int MODULE_STATE_PICKUP_OBJECT_NONE = 0;
const int MODULE_STATE_PICKUP_OBJECT_ARM_PICKING_OBJECT = 1;
const int MODULE_STATE_PICKUP_OBJECT_FINISHED_SUCCESS = 2;
const int MODULE_STATE_PICKUP_OBJECT_FINISHED_FAILED = 3;

#include "iface/CVModule.h"
#include <iostream>
#include "../logic/image/ImagePreprocessItem.h"
using namespace std;

class PickUpObject : public CVModule{
private:
	int triesObjectPickup = 0;
	int MAX_TRIES = 3;

	void increasePickupCounter(){
		triesObjectPickup++;
	}

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

		pickupObject(mImagePreprocessItem, mRoboticArmMove);
		mRoboticArmMove->setObjectIndex(0);
		increasePickupCounter();
		setModulState(MODULE_STATE_PICKUP_OBJECT_ARM_PICKING_OBJECT);

		return true;
	}

	/*
	 * TODO continue
	 */
	void processNextState(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		if (DEBUG_LOCAL) cout << "PickUpObject :: processNextState" << endl;

		testing(mImagePreprocessItem, mRoboticArmMove);

		/*
		switch(this->currentState){

		case MODULE_STATE_NONE:
			if (DEBUG_LOCAL) cout << "PickUpObject :: processNextState - invalid move MODULE_STATE_NONE" << endl;
			break;

		case MODULE_STATE_PICKUP_OBJECT_ARM_PICKING_OBJECT:
			// move arm to default position
			mRoboticArmMove->setDefaultPosition();

			// update state
			setModulState(MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING);
			break;

		case MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING:
			cout << "start picking object" << endl;
			if (triesObjectPickup > MAX_TRIES){
				setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_FAILED);
				setFinished(true);
				if (DEBUG_LOCAL) cout << "picking object FAILED -- END" << endl;
			}

			// object has been picked up
			if (mImagePreprocessItem->detectedObjects.size() == 0){
				setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_SUCCESS);
				setFinished(true);
				pullDownObject(mRoboticArmMove);

			// object still there
			}else{
				pickupObject(mImagePreprocessItem, mRoboticArmMove);
				setModulState(MODULE_STATE_PICKUP_OBJECT_ARM_PICKING_OBJECT);
				increasePickupCounter();
			}
			break;

		case MODULE_STATE_PICKUP_OBJECT_FINISHED_FAILED:
			if (DEBUG_LOCAL) cout << "PickUpObject :: processNextState - invalid move MODULE_STATE_PICKUP_OBJECT_FINISHED_FAILED" << endl;
			break;

		case MODULE_STATE_PICKUP_OBJECT_FINISHED_SUCCESS:
			if (DEBUG_LOCAL) cout << "picking object SUCCESS -- END" << endl;
			break;
		}
		*/
	}

	/*
	 * TODO testing
	 */
	void testing(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){

		if (currentState == MODULE_STATE_PICKUP_OBJECT_ARM_PICKING_OBJECT){
			mRoboticArmMove->setDefaultPosition();
			setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_SUCCESS);
		}

	}

	void pullDownObject(RoboticArmMove *mRoboticArmMove){
		//TODO continue
	}

	void resetContent(){
		triesObjectPickup = 0;
	}

	~PickUpObject(){
		if (DEBUG_LOCAL) cout << "-- -- -- DESTRUCTOR: PickUpObject -- -- --" << endl;

	}
};



#endif /* SRC_APPLICATION_MODULES_PICKUPOBJECT_H_ */
