/*
 * PickUpObject.h
 *
 *  Created on: Jan 31, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_MODULES_PICKUPOBJECT_H_
#define SRC_APPLICATION_MODULES_PICKUPOBJECT_H_

const int MODULE_STATE_PICKUP_OBJECT_NONE = 0;
const int MODULE_STATE_PICKUP_OBJECT_MOVE_CLOSE_TO_OBJECT = 1;
const int MODULE_STATE_PICKUP_OBJECT_PICK_OBJECT_WITH_CLAWS = 2;
const int MODULE_STATE_PICKUP_OBJECT_MOVE_OBJECT_TO_POSITION = 3;
const int MODULE_STATE_PICKUP_OBJECT_RELEASE_OBJECT = 4;
const int MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING = 5;
const int MODULE_STATE_PICKUP_OBJECT_FINISHED_SUCCESS = 6;
const int MODULE_STATE_PICKUP_OBJECT_FINISHED_FAILED = 7;

#include "iface/CVModule.h"
#include <iostream>
#include "../logic/image/ImagePreprocessItem.h"
using namespace std;

class PickUpObject : public CVModule{
private:

protected:
	bool DEBUG_LOCAL = false;

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

		// is there one object ?
		if (mImagePreprocessItem->detectedObjects.size() != 1){
			if (DEBUG_LOCAL) cout << "PickUpObject :: initialObjectDetection invalid number of objects" << mImagePreprocessItem->detectedObjects.size() << endl;
			return false;
		}

		// TODO check size of object

		mRoboticArmMove->setObjectIndex(0);
		pickupObject(mImagePreprocessItem, mRoboticArmMove);
		setModulState(MODULE_STATE_PICKUP_OBJECT_MOVE_CLOSE_TO_OBJECT);

		saveLastMoveObj(mImagePreprocessItem);
		saveLastMove(mRoboticArmMove);
		saveLastMoveTime();
		setTimeTrigger(3);

		return true;
	}

	/*
	 * process states of picking object with arm
	 */
	void processNextState(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		if (DEBUG_LOCAL) cout << "PickUpObject :: processNextState " << this->currentState << endl;

		switch(this->currentState){

		// nothing
		case MODULE_STATE_NONE:
			if (DEBUG_LOCAL) cout << "PickUpObject :: processNextState - invalid move MODULE_STATE_NONE" << endl;
			break;

		// pick up object with claws, stay on place open claws
		case MODULE_STATE_PICKUP_OBJECT_MOVE_CLOSE_TO_OBJECT:
			if (executeTimeTrigger()){
				cout << "PickUpObject :: close claws" << endl;

				// TODO calculate value
				int clawsOpenMm = 300;
				restoreArmMove(mRoboticArmMove);
				mRoboticArmMove->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, clawsOpenMm);

				// update state
				setModulState(MODULE_STATE_PICKUP_OBJECT_PICK_OBJECT_WITH_CLAWS);
			}
			break;

		//move object to predefined position
		case MODULE_STATE_PICKUP_OBJECT_PICK_OBJECT_WITH_CLAWS:
			cout << "PickUpObject :: move object to predefined position" << endl;
			mRoboticArmMove->setServo(SERVO_IDX_BASE, DIRECTION_LEFT, 90, 0);
			mRoboticArmMove->setServo(SERVO_IDX_BOTTOM_JOINT, DIRECTION_FORWARD, 37, 0);
			mRoboticArmMove->setServo(SERVO_IDX_MIDDLE_JOINT, DIRECTION_FORWARD, 67, 0);
			mRoboticArmMove->setServo(SERVO_IDX_UPPER_JOINT, DIRECTION_FORWARD, 16, 0);
			mRoboticArmMove->setServo(SERVO_IDX_CLAW_ROTATE, DIRECTION_LEFT, 0, 0);
			mRoboticArmMove->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, 300); // TODO value
			setModulState(MODULE_STATE_PICKUP_OBJECT_MOVE_OBJECT_TO_POSITION);
			break;

		//release object
		case MODULE_STATE_PICKUP_OBJECT_MOVE_OBJECT_TO_POSITION:
			cout << "PickUpObject :: release object" << endl;
			mRoboticArmMove->setServo(SERVO_IDX_BASE, DIRECTION_LEFT, 90, 0);
			mRoboticArmMove->setServo(SERVO_IDX_BOTTOM_JOINT, DIRECTION_FORWARD, 37, 0);
			mRoboticArmMove->setServo(SERVO_IDX_MIDDLE_JOINT, DIRECTION_FORWARD, 67, 0);
			mRoboticArmMove->setServo(SERVO_IDX_UPPER_JOINT, DIRECTION_FORWARD, 16, 0);
			mRoboticArmMove->setServo(SERVO_IDX_CLAW_ROTATE, DIRECTION_LEFT, 0, 0);
			mRoboticArmMove->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, 500); // TODO
			setModulState(MODULE_STATE_PICKUP_OBJECT_RELEASE_OBJECT);
			break;

		// return back to default position
		case MODULE_STATE_PICKUP_OBJECT_RELEASE_OBJECT:
			cout << "PickUpObject :: return to default position" << endl;
			mRoboticArmMove->setDefaultPosition();
			setModulState(MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING);
			break;

		case MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING:
			// TODO compare detected object with object that was picked up
			// object has been picked up
			if (mImagePreprocessItem->detectedObjects.size() == 0){
				cout << "PickUpObject :: processNextState - success object was moved" << endl;
				setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_SUCCESS);
				setFinished(true);

			// object still there
			}else{
				cout << "PickUpObject :: processNextState - failed object was not moved" << endl;
				setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_FAILED);
				setFinished(true);
			}
			break;

		}
	}

	~PickUpObject(){
		if (DEBUG_LOCAL) cout << "-- -- -- DESTRUCTOR: PickUpObject -- -- --" << endl;

	}
};



#endif /* SRC_APPLICATION_MODULES_PICKUPOBJECT_H_ */
