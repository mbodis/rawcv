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
	string TAG = "PickUpObject";
	ImagePreprocessItem *objectDetectedPreprocessItem;
	InputStorage *mInputStorage;

public:

	PickUpObject(string moduleName, RoboticArmController *mRoboticArmController):CVModule(moduleName, mRoboticArmController){
		MyLog::log(LOG_DEBUG, TAG, "-- -- -- CONSTRUCTOR: PickUpObject -- -- --");
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
		MyLog::log(LOG_DEBUG, TAG, "PickUpObject :: initialObjectDetection");

		// is there one object ?
		if (mImagePreprocessItem->detectedObjects.size() != 1){
			MyLog::log(LOG_DEBUG, TAG, "PickUpObject :: initialObjectDetection invalid number of objects" + to_string(mImagePreprocessItem->detectedObjects.size()));
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
		if (maxMm > CLAWS_OBJECT_MAX_MM + 30){
			cout << "object too big to pick up: " << maxMm << " mm" << endl;
			return false;
		}

		mImagePreprocessItem->setObjectIndex(0);
		saveMoveObj(mImagePreprocessItem);
		saveDetectedObj(mImagePreprocessItem);

		pickupObjectRotation(objectDetectedPreprocessItem, mRoboticArm->getNextMove());
		mRoboticArmController->executeCommand(mRoboticArm->composeNextMove());

		pickupObjectLean(objectDetectedPreprocessItem, mRoboticArm->getNextMove());
		mRoboticArmController->executeCommand(mRoboticArm->composeNextMove());

		saveLastMove(mRoboticArm->getNextMove());

		MyLog::log(LOG_DEBUG, TAG, "object selected with index: 0, size:" +  to_string(obj.size.width*mImagePreprocessItem->oneMmInPx)
				+ "x" + to_string(obj.size.height*mImagePreprocessItem->oneMmInPx) + "mm ");

		setModulState(MODULE_STATE_PICKUP_OBJECT_MOVE_CLOSE_TO_OBJECT);


		return true;
	}

	/*
	 * process states of picking object with arm
	 * input from interesting frame (a movement has been detected and stops)
	 */
	void processNextStateFrameTrigger(ImagePreprocessItem *mImagePreprocessItem, RoboticArm *mRoboticArm){
		MyLog::log(LOG_DEBUG, TAG, "PickUpObject :: processNextStateFrameTrigger " + to_string(this->currentState));
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
		MyLog::log(LOG_DEBUG, TAG, "PickUpObject :: processNextStateTimeTrigger " + to_string(this->currentState));
		if (finished) return;

		//int objWidthMm = objectDetectedPreprocessItem->detectedObjects[objectDetectedPreprocessItem->getObjectIndex()].size.width * objectDetectedPreprocessItem->oneMmInPx;
		//int objHeightMm = objectDetectedPreprocessItem->detectedObjects[objectDetectedPreprocessItem->getObjectIndex()].size.height * objectDetectedPreprocessItem->oneMmInPx;
		//int clawsOpenMm = min(objHeightMm, objWidthMm);
		int clawsOpenMm = 20; // TODO object size
		switch(this->currentState){

		// pick up object with claws, stay on place open claws
		case MODULE_STATE_PICKUP_OBJECT_MOVE_CLOSE_TO_OBJECT:

			MyLog::log(LOG_DEBUG, TAG, "PickUpObject :: pick object that is " + to_string(clawsOpenMm) + " mm width");

			restoreArmMove(mRoboticArm->getNextMove());
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, clawsOpenMm);

			// update state
			setModulState(MODULE_STATE_PICKUP_OBJECT_PICK_OBJECT_WITH_CLAWS);
			mRoboticArmController->executeCommand(mRoboticArm->composeNextMove());
			break;

		//move object to predefined position
		case MODULE_STATE_PICKUP_OBJECT_PICK_OBJECT_WITH_CLAWS:
			MyLog::log(LOG_DEBUG, TAG, "PickUpObject :: move object to predefined position");

			mRoboticArm->getNextMove()->setServo(SERVO_IDX_BASE, DIRECTION_LEFT, 0, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_BOTTOM_JOINT, DIRECTION_FORWARD, 19, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_MIDDLE_JOINT, DIRECTION_FORWARD, 33, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_UPPER_JOINT, DIRECTION_FORWARD, 8, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_CLAW_ROTATE, DIRECTION_LEFT, 0, 0);
			mRoboticArm->getNextMove()->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, clawsOpenMm);
			mRoboticArmController->executeCommand(mRoboticArm->composeNextMove());

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
			MyLog::log(LOG_DEBUG, TAG, "PickUpObject :: release object");

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
			MyLog::log(LOG_DEBUG, TAG, "PickUpObject :: return to default position");

			mRoboticArm->getNextMove()->setDefaultPosition();
			setModulState(MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING);
			mRoboticArmController->executeCommandWithCallback(mRoboticArm->composeNextMove(), KEY_VERIFY_STATUS);
			break;

		case MODULE_STATE_PICKUP_OBJECT_CONFIRM_OBJECT_IS_MISSING:

			if (mRoboticArmController->wasCommandExecuted(KEY_VERIFY_STATUS)){
				MyLog::log(LOG_DEBUG, TAG, "PickUpObject :: return to default position");


				if (lastMovePreprocessItem->detectedObjects.size() == 1){

					double distPx = cv::norm(objectDetectedPreprocessItem->detectedObjects[0].center
						- lastMovePreprocessItem->detectedObjects[0].center);
					double distMm = distPx * lastMovePreprocessItem->oneMmInPx;

					// if object was moved more than 15mm
					if (distMm > 15){
						cout << "PickUpObject :: SUCCESS object was moved " << distMm << " mm" << endl;
						setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_SUCCESS);
						setFinished(true);

					// object still there
					}else{
						cout << "PickUpObject :: FAILED object was moved " << distMm << " mm" << endl;
						setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_FAILED);
						setFinished(true);
					}

				// object undefined
				}else{
					cout << "PickUpObject :: FAILED object not found #obj:" << lastMovePreprocessItem->detectedObjects.size() << endl;
					setModulState(MODULE_STATE_PICKUP_OBJECT_FINISHED_FAILED);
					setFinished(true);
				}

			}
			break;

		}
	}

	void resetContent(){
		MyLog::log(LOG_DEBUG, TAG, "-- -- -- resetContent -- -- --");
		setModulState(MODULE_STATE_NONE);
		lastMoveMilis = 0;

		finished = false;
		enabled = true;
		timeTriggerSec = -1;
		lastMovePreprocessItem = new ImagePreprocessItem();
		objectDetectedPreprocessItem = new ImagePreprocessItem();
		lastMove = new RoboticArmMove();
	}

	~PickUpObject(){
		MyLog::log(LOG_DEBUG, TAG, "-- -- -- DESTRUCTOR: PickUpObject -- -- --");

	}

	/*
	 * save selected object from initial processing to use in later steps
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
