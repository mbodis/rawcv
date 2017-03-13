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

static string KEY_VERIFY_STATUS = "PickUpObject-VerifyStatus";

class PickUpObject : public CVModule{
private:
	ImagePreprocessItem *objectDetectedPreprocessItem;
	InputStorage *mInputStorage;

protected:
	bool DEBUG_LOCAL = false;

public:

	PickUpObject(string moduleName, RoboticArmController *mRoboticArmController):CVModule(moduleName, mRoboticArmController){
		if (DEBUG_LOCAL) cout << "-- -- -- CONSTRUCTOR: PickUpObject -- -- --" << endl;
		mInputStorage = &InputStorage::getInstance();
	}

	/*
	 * initialization
	 *
	 * validate input (one object, size fits)
	 * pick up object
	 * update pickup tries counter
	 * update state
	 */
	bool initialObjectDetection(ImagePreprocessItem *mImagePreprocessItem, RoboticArm *mRoboticArm){
		if (DEBUG_LOCAL) cout << "PickUpObject :: initialObjectDetection" << endl;

		// is there one object ?
		if (mImagePreprocessItem->detectedObjects.size() != 1){
			if (DEBUG_LOCAL) cout << "PickUpObject :: initialObjectDetection invalid number of objects" << mImagePreprocessItem->detectedObjects.size() << endl;
			return false;
		}

		// check size of object minimal size 10x10mm, max size 50x50mm
		RotatedRect obj = mImagePreprocessItem->detectedObjects[0];
		double minMm = min(obj.size.width, obj.size.height) * mImagePreprocessItem->oneMmInPx;
		double maxMm = max(obj.size.width, obj.size.height) * mImagePreprocessItem->oneMmInPx;
		if (minMm < CLAWS_OBJECT_MIN_MM){
			cout << "object too small to pick up: " << minMm << " mm" << endl;
			return false;
		}
		if (maxMm > CLAWS_OBJECT_MAX_MM){
			cout << "object too big to pick up: " << maxMm << " mm" << endl;
			return false;
		}

		mImagePreprocessItem->setObjectIndex(0);

		pickupObject(mImagePreprocessItem, mRoboticArm->getNextMove());

		saveMoveObj(mImagePreprocessItem);
		saveDetectedObj(mImagePreprocessItem);
		saveLastMove(mRoboticArm->getNextMove());

		cout << "object selected with index: 0, size:" <<  obj.size.width*mImagePreprocessItem->oneMmInPx
				<< "x" << obj.size.height*mImagePreprocessItem->oneMmInPx << "mm "<< endl;

		setModulState(MODULE_STATE_PICKUP_OBJECT_MOVE_CLOSE_TO_OBJECT);

		mRoboticArmController->executeCommand(mRoboticArm->composeNextMove());
		return true;
	}

	/*
	 * process states of picking object with arm
	 * input from interesting frame (a movement has been detected and stops)
	 */
	void processNextStateFrameTrigger(ImagePreprocessItem *mImagePreprocessItem, RoboticArm *mRoboticArm){
		if (DEBUG_LOCAL) cout << "PickUpObject :: processNextStateFrameTrigger " << this->currentState << endl;
		if (finished) return;

		saveMoveObj(mImagePreprocessItem);

		switch(this->currentState){
			// add states that should be triggered on interesting frame
		}
	}

	/*
	 * process states of picking object with arm
	 * input from by time (some time has passed - check if something has changed)
	 */
	void processNextStateTimeTrigger(RoboticArm *mRoboticArm){
		if (DEBUG_LOCAL) cout << "PickUpObject :: processNextStateTimeTrigger " << this->currentState << endl;
		if (finished) return;

		int objWidthMm, objHeightMm, clawsOpenMm;
		switch(this->currentState){

		// pick up object with claws, stay on place open claws
		case MODULE_STATE_PICKUP_OBJECT_MOVE_CLOSE_TO_OBJECT:
			objWidthMm = objectDetectedPreprocessItem->detectedObjects[objectDetectedPreprocessItem->getObjectIndex()].size.width * objectDetectedPreprocessItem->oneMmInPx;
			objHeightMm = objectDetectedPreprocessItem->detectedObjects[objectDetectedPreprocessItem->getObjectIndex()].size.height * objectDetectedPreprocessItem->oneMmInPx;
			clawsOpenMm = min(objHeightMm, objWidthMm);

			if (DEBUG_LOCAL) cout << "PickUpObject :: pick object that is " << clawsOpenMm << " mm width" << endl;

			restoreArmMove(mRoboticArm->getNextMove());
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, clawsOpenMm);

			// update state
			setModulState(MODULE_STATE_PICKUP_OBJECT_PICK_OBJECT_WITH_CLAWS);
			mRoboticArmController->executeCommand(mRoboticArm->composeNextMove());
			break;

		//move object to predefined position
		case MODULE_STATE_PICKUP_OBJECT_PICK_OBJECT_WITH_CLAWS:
			if (DEBUG_LOCAL) cout << "PickUpObject :: move object to predefined position" << endl;

			objWidthMm = objectDetectedPreprocessItem->detectedObjects[objectDetectedPreprocessItem->getObjectIndex()].size.width * objectDetectedPreprocessItem->oneMmInPx;
			objHeightMm = objectDetectedPreprocessItem->detectedObjects[objectDetectedPreprocessItem->getObjectIndex()].size.height * objectDetectedPreprocessItem->oneMmInPx;
			clawsOpenMm = min(objHeightMm, objWidthMm);

			mRoboticArm->getNextMove()->setServo(SERVO_IDX_BASE, DIRECTION_LEFT, 0, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_BOTTOM_JOINT, DIRECTION_FORWARD, 37, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_MIDDLE_JOINT, DIRECTION_FORWARD, 67, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_UPPER_JOINT, DIRECTION_FORWARD, 16, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_CLAW_ROTATE, DIRECTION_LEFT, 0, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, clawsOpenMm);
			setModulState(MODULE_STATE_PICKUP_OBJECT_MOVE_OBJECT_TO_POSITION);
			mRoboticArmController->executeCommand(mRoboticArm->composeNextMove());
			break;

		//release object
		case MODULE_STATE_PICKUP_OBJECT_MOVE_OBJECT_TO_POSITION:
			if (DEBUG_LOCAL) cout << "PickUpObject :: release object" << endl;

			mRoboticArm->getNextMove()->setServo(SERVO_IDX_BASE, DIRECTION_LEFT, 0, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_BOTTOM_JOINT, DIRECTION_FORWARD, 37, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_MIDDLE_JOINT, DIRECTION_FORWARD, 67, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_UPPER_JOINT, DIRECTION_FORWARD, 16, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_CLAW_ROTATE, DIRECTION_LEFT, 0, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, CLAWS_OBJECT_MAX_MM);
			setModulState(MODULE_STATE_PICKUP_OBJECT_RELEASE_OBJECT);
			mRoboticArmController->executeCommand(mRoboticArm->composeNextMove());
			break;

		// return back to default position
		case MODULE_STATE_PICKUP_OBJECT_RELEASE_OBJECT:
			if (DEBUG_LOCAL) cout << "PickUpObject :: return to default position" << endl;

			mRoboticArm->getNextMove()->setDefaultPosition();
			setModulState(MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING);
			mRoboticArmController->executeCommandWithCallback(mRoboticArm->composeNextMove(), KEY_VERIFY_STATUS);
			break;

		case MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING:

			if (mRoboticArmController->wasCommandExecuted(KEY_VERIFY_STATUS)){
				if (DEBUG_LOCAL) cout << "PickUpObject :: return to default position" << endl;

				// TODO test if object was moved

				// object moved
				if (lastMovePreprocessItem->detectedObjects.size() == 0){
					cout << "PickUpObject :: success object was moved" << endl;
					setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_SUCCESS);
					setFinished(true);

				// object still there
				}else{
					cout << "PickUpObject :: failed object was not moved" << endl;
					setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_FAILED);
					setFinished(true);
				}
			}
			break;

		}
	}

	void resetContent(){
		if (DEBUG_LOCAL) cout << "-- -- -- resetContent -- -- --" << endl;
		setModulState(MODULE_STATE_NONE);
		lastMoveMilis = 0;

		finished = true;
		enabled = true;
		timeTriggerSec = -1;
		lastMovePreprocessItem = new ImagePreprocessItem();
		objectDetectedPreprocessItem = new ImagePreprocessItem();
		lastMove = new RoboticArmMove();
	}

	~PickUpObject(){
		if (DEBUG_LOCAL) cout << "-- -- -- DESTRUCTOR: PickUpObject -- -- --" << endl;

	}

	/*
	 * TODO optimize
	 */
	void saveDetectedObj(ImagePreprocessItem *newImagePreprocessItem){

		objectDetectedPreprocessItem = new ImagePreprocessItem();
		objectDetectedPreprocessItem->setObjectIndex(newImagePreprocessItem->getObjectIndex());
		objectDetectedPreprocessItem->fullInputFrame = newImagePreprocessItem->fullInputFrame;
		objectDetectedPreprocessItem->preprocessFrame = newImagePreprocessItem->preprocessFrame;
		objectDetectedPreprocessItem->armCenter = newImagePreprocessItem->armCenter;
		objectDetectedPreprocessItem->detectedObjects = newImagePreprocessItem->detectedObjects;
		objectDetectedPreprocessItem->oneMmInPx = newImagePreprocessItem->oneMmInPx;
	}
};



#endif /* SRC_APPLICATION_MODULES_PICKUPOBJECT_H_ */
