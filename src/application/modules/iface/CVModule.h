/*
 * CVModul.h
 *
 *  Created on: Jan 30, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_MODULES_IFACE_CVMODULE_H_
#define SRC_APPLICATION_MODULES_IFACE_CVMODULE_H_

const int MODULE_STATE_NONE = 0;
const int MODULE_STATE_START = 1;

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "../../config/AppConfig.h"
#include "../../controllers/RoboticArmController.h"
#include "../../logic/image/ImagePreprocessItem.h"
#include "../../logic/InputStorage.h"
#include "../../../system/helper/MathHelper.h"

using namespace std;
using namespace cv;


class CVModule{
private:
	string TAG = "CVModule";

protected:
	RoboticArmController *mRoboticArmController;

	bool finished = false;
	bool enabled = true;
	string moduleName;
	int currentState = MODULE_STATE_NONE;

	long lastMoveMilis = 0;
	int timeTriggerSec = -1;
	ImagePreprocessItem *lastMovePreprocessItem;
	RoboticArmMove *lastMove;

	/*
	 * arm rotation
	 */
	void pickupObjectCalculateArmRotation(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		MyLog::log(LOG_DEBUG, TAG, "plane: " + to_string(mImagePreprocessItem->preprocessFrame.cols) + " x " + to_string(mImagePreprocessItem->preprocessFrame.rows));
		MyLog::log(LOG_DEBUG, TAG, "arm: [" + to_string(mImagePreprocessItem->armCenter.x) + "," + to_string(mImagePreprocessItem->armCenter.y) + "]");
		MyLog::log(LOG_DEBUG, TAG, "object: [" + to_string((int)mImagePreprocessItem->detectedObjects[0].center.x) + "," + to_string((int)mImagePreprocessItem->detectedObjects[0].center.y) + "]");

		int x, y, z, angle, direction;
		direction = (mImagePreprocessItem->detectedObjects[0].center.x > mImagePreprocessItem->armCenter.x) ? DIRECTION_RIGHT : DIRECTION_LEFT;

		// object is in front of arm center
		if (mImagePreprocessItem->armCenter.y > mImagePreprocessItem->detectedObjects[0].center.y){
			x = abs(mImagePreprocessItem->armCenter.x - mImagePreprocessItem->detectedObjects[0].center.x);
			y = abs(mImagePreprocessItem->armCenter.y - mImagePreprocessItem->detectedObjects[0].center.y);
			z = sqrt(x*x + y*y);
			angle = MathHelper::sinInverseDeg((double)x/z);

		// object is behind of arm center
		}else if (mImagePreprocessItem->armCenter.y <= mImagePreprocessItem->detectedObjects[0].center.y){
			x = abs(mImagePreprocessItem->armCenter.y - mImagePreprocessItem->detectedObjects[0].center.y);
			y = abs(mImagePreprocessItem->armCenter.x - mImagePreprocessItem->detectedObjects[0].center.x);
			z = sqrt(x*x + y*y);
			angle = MathHelper::sinInverseDeg((double)x/z);
			angle += 90;
		}

		mRoboticArmMove->setServo(SERVO_IDX_BASE, direction, angle, 0);
		MyLog::log(LOG_DEBUG, TAG, "direction: " + ((string)(direction==DIRECTION_LEFT ? "left " : "right ")) + to_string(angle) + " °");
	}

	/*
	 * arm lean
	 */
	void pickupObjectCalculateArmLean(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		MyLog::log(LOG_DEBUG, TAG, "calculateArmLean--------------------------");

		int ARM_PART_1_PX = ARM_PART_1_MM * mImagePreprocessItem->oneMmInPx;
		int ARM_PART_2_PX = ARM_PART_2_MM * mImagePreprocessItem->oneMmInPx;
		int ARM_PART_3_PX = ARM_PART_3_MM * mImagePreprocessItem->oneMmInPx;

		// base as [0, 0]
		int baseJointX = 0;
		int baseJointY = 0;

		int objectX = cv::norm((Point)mImagePreprocessItem->armCenter - (Point) mImagePreprocessItem->detectedObjects[mImagePreprocessItem->getObjectIndex()].center );
		int objectY = 0;
		MyLog::log(LOG_DEBUG, TAG, "objectX: " + to_string(objectX) + " objectY: " + to_string(objectY));

		double minDistance = 999;
		int minAngle1 = 0, minAngle2 =0, minAngle3 =0, yCord = 999;

		for(int armAngle1=0; armAngle1<90; armAngle1++){
			for(int armAngle2=0; armAngle2<90; armAngle2++){
				for(int armAngle3=0; armAngle3<90; armAngle3++){

					// !important! propagate previous angle rotation
					int angle1 = armAngle1;
					int angle2 = armAngle2;
					int angle3 = armAngle3;

					angle2 += angle1;
					angle3 += angle2;

					//arm part 1
					int joint1X = MathHelper::sinDeg(180+angle1) * ARM_PART_1_PX;
					int joint1Y = MathHelper::sinDeg(90+angle1) * ARM_PART_1_PX;
					Point joint1Center = Point(baseJointX - joint1X, baseJointY - joint1Y);
					if (MyLog::isLogEnabled(LOG_DEBUG)){
						MyLog::log(LOG_DEBUG, TAG, "joint1X: " + to_string(joint1X) + " joint1Y: " + to_string(joint1Y));
						MyLog::log(LOG_DEBUG, TAG, "joint1CenterX: " + to_string(joint1Center.x) + " joint1CenterY: " + to_string(joint1Center.y));
					}

					//arm part 2
					int joint2X = MathHelper::sinDeg(180+angle2) * ARM_PART_2_PX;
					int joint2Y = MathHelper::sinDeg(90+angle2) * ARM_PART_2_PX;
					Point joint2Center = Point(joint1Center.x - joint2X, joint1Center.y - joint2Y);
					if (MyLog::isLogEnabled(LOG_DEBUG)){
						MyLog::log(LOG_DEBUG, TAG, "joint2X: " + to_string(joint2X) + " joint2Y: " + to_string(joint2Y));;
						MyLog::log(LOG_DEBUG, TAG, "joint2CenterX: " + to_string(joint2Center.x) + " joint2CenterY: " + to_string(joint2Center.y));
					}

					//arm part 3
					int joint3X = MathHelper::sinDeg(180+angle3) * ARM_PART_3_PX;
					int joint3Y = MathHelper::sinDeg(90+angle3) * ARM_PART_3_PX;
					Point joint3Center = Point(joint2Center.x - joint3X, joint2Center.y - joint3Y);
					if (MyLog::isLogEnabled(LOG_DEBUG)){
						MyLog::log(LOG_DEBUG, TAG, "joint3X: " + to_string(joint3X) + " joint3Y: " + to_string(joint3Y));
						MyLog::log(LOG_DEBUG, TAG, "joint3CenterX: " + to_string(joint3Center.x) + " joint3CenterY: " + to_string(joint3Center.y));
					}

					// add some limitations (arm should not hit table)
					// 50 mm above table is closest for arm TODO later
					if ( joint3Center.y > -2 * mImagePreprocessItem->oneMmInPx){
						continue;
					}

					double objectArmEndFromObjectPx = cv::norm(joint3Center - Point(objectX, objectY));
					double objectDistanceFromArmMm = objectArmEndFromObjectPx * mImagePreprocessItem->oneMmInPx;
					if (MyLog::isLogEnabled(LOG_DEBUG)){
						MyLog::log(LOG_DEBUG, TAG, "objectArmEndFromObjectPx: " + to_string(objectArmEndFromObjectPx));
						MyLog::log(LOG_DEBUG, TAG, "objectDistanceFromArmMm: " + to_string(objectDistanceFromArmMm));
					}

					if (objectDistanceFromArmMm <= ARM_MAX_DISTANCE_PICKUP_MM){

						if (minDistance > objectDistanceFromArmMm){
							MyLog::log(LOG_DEBUG, TAG, "found better: angle1=" + to_string(armAngle1) + "° angle2="
								+ to_string(armAngle2) + "° angle3=" + to_string(armAngle3) + "° dist=" + to_string(objectDistanceFromArmMm));
							minDistance = objectDistanceFromArmMm;
							minAngle1 = armAngle1;
							minAngle2 = armAngle2;
							minAngle3 = armAngle3;
							yCord = joint3Center.y;
						}
					}
				}
			}
		}

		MyLog::log(LOG_INFO, TAG, "OBJECT DETECTED angle1=" + to_string(minAngle1) + "° angle2=" + to_string(minAngle2)
				+ "° angle3=" + to_string(minAngle3) + "° minDistance=" + to_string(minDistance));

		mRoboticArmMove->setServo(SERVO_IDX_BOTTOM_JOINT, DIRECTION_FORWARD, minAngle1, 0);
		mRoboticArmMove->setServo(SERVO_IDX_MIDDLE_JOINT, DIRECTION_FORWARD, minAngle2, 0);
		mRoboticArmMove->setServo(SERVO_IDX_UPPER_JOINT, DIRECTION_FORWARD, minAngle3, 0);
	}


public:

	CVModule(string moduleName, RoboticArmController *mRoboticArmController){
		this->mRoboticArmController = mRoboticArmController;
		this->moduleName = moduleName;
		this->lastMovePreprocessItem = new ImagePreprocessItem();
		this->lastMove = new RoboticArmMove();
	}

	~CVModule(){

	}

	virtual bool initialObjectDetection(ImagePreprocessItem *mImagePreprocessItem, RoboticArm *mRoboticArm){
		throw std::logic_error(" CVmodule initialObjectDeection - method not implemented");
	}

	virtual void processNextStateFrameTrigger(ImagePreprocessItem *mImagePreprocessItem, RoboticArm *mRoboticArm){
		throw std::logic_error(" CVmodule processNextStateFrameTrigger - method not implemented ");
	}

	virtual void processNextStateTimeTrigger(RoboticArm *mRoboticArm){
		throw std::logic_error(" CVmodule processNextStateTimeTrigger - method not implemented ");
	}

	virtual void resetContent(){
		throw std::logic_error(" CVmodule resetContent - method not implemented");
	}

	void setTimeTrigger(int sec){
		saveLastMoveTime();
		this->timeTriggerSec = sec;
	}
	void unsetTimeTrigger(){
		this->timeTriggerSec = -1;
	}
	bool executeTimeTrigger(){
		if (this->timeTriggerSec == -1){
			return false;
		}

		long now = TimeHelper::getSystemTimeMilis();

		if ((now - this->lastMoveMilis) / 1000 > this->timeTriggerSec){
			unsetTimeTrigger();
			return true;
		}

		return false;
	}

	// save last move time
	void saveLastMoveTime(){
		lastMoveMilis = TimeHelper::getSystemTimeMilis();
	}

	bool isEnabled(){
		return enabled;
	}

	string getName(){
		return this->moduleName;
	}

	bool isModulActive(){
		return (currentState > 0);
	}

	void setFinished(bool finished){
		this->finished = finished;
	}

	bool isFinished(){
		return this->finished;
	}

	void setModulState(int state){
		currentState = state;
	}

	/*
	 * save move Obj
	 * optimize later - clone object
	 */
	void saveMoveObj(ImagePreprocessItem *newImagePreprocessItem){
		lastMovePreprocessItem = new ImagePreprocessItem();
		lastMovePreprocessItem->setObjectIndex(newImagePreprocessItem->getObjectIndex());
		lastMovePreprocessItem->fullInputFrame = newImagePreprocessItem->fullInputFrame;
		lastMovePreprocessItem->preprocessFrame = newImagePreprocessItem->preprocessFrame;
		lastMovePreprocessItem->armCenter = newImagePreprocessItem->armCenter;
		lastMovePreprocessItem->detectedObjects = newImagePreprocessItem->detectedObjects;
		lastMovePreprocessItem->oneMmInPx = newImagePreprocessItem->oneMmInPx;

		InputStorage *mInputStorage = &InputStorage::getInstance();
		mInputStorage->addToDisplayQueue("a1", lastMovePreprocessItem->fullInputFrame);
		mInputStorage->addToDisplayQueue("a1", lastMovePreprocessItem->preprocessFrame);
	}

	/*
	 *
	 */
	void restoreArmMove(RoboticArmMove *mRoboticArmMove){
		mRoboticArmMove->setServo(SERVO_IDX_BASE, lastMove->getDirectionForServo(SERVO_IDX_BASE), lastMove->getAngleForServo(SERVO_IDX_BASE), 0);
		mRoboticArmMove->setServo(SERVO_IDX_BOTTOM_JOINT, lastMove->getDirectionForServo(SERVO_IDX_BOTTOM_JOINT), lastMove->getAngleForServo(SERVO_IDX_BOTTOM_JOINT), 0);
		mRoboticArmMove->setServo(SERVO_IDX_MIDDLE_JOINT, lastMove->getDirectionForServo(SERVO_IDX_MIDDLE_JOINT), lastMove->getAngleForServo(SERVO_IDX_MIDDLE_JOINT), 0);
		mRoboticArmMove->setServo(SERVO_IDX_UPPER_JOINT, lastMove->getDirectionForServo(SERVO_IDX_UPPER_JOINT), lastMove->getAngleForServo(SERVO_IDX_UPPER_JOINT), 0);
		mRoboticArmMove->setServo(SERVO_IDX_CLAW_ROTATE, lastMove->getDirectionForServo(SERVO_IDX_CLAW_ROTATE), lastMove->getAngleForServo(SERVO_IDX_CLAW_ROTATE), 0);
		mRoboticArmMove->setServo(SERVO_IDX_CLAWS, lastMove->getDirectionForServo(SERVO_IDX_CLAWS), 0, lastMove->getMmForServo(SERVO_IDX_CLAWS));
	}

	/*
	 *
	 */
	void saveLastMove(RoboticArmMove *mRoboticArmMove){
		lastMove = new RoboticArmMove();
		lastMove->setServo(SERVO_IDX_BASE, mRoboticArmMove->getDirectionForServo(SERVO_IDX_BASE), mRoboticArmMove->getAngleForServo(SERVO_IDX_BASE), 0);
		lastMove->setServo(SERVO_IDX_BOTTOM_JOINT, mRoboticArmMove->getDirectionForServo(SERVO_IDX_BOTTOM_JOINT), mRoboticArmMove->getAngleForServo(SERVO_IDX_BOTTOM_JOINT), 0);
		lastMove->setServo(SERVO_IDX_MIDDLE_JOINT, mRoboticArmMove->getDirectionForServo(SERVO_IDX_MIDDLE_JOINT), mRoboticArmMove->getAngleForServo(SERVO_IDX_MIDDLE_JOINT), 0);
		lastMove->setServo(SERVO_IDX_UPPER_JOINT, mRoboticArmMove->getDirectionForServo(SERVO_IDX_UPPER_JOINT), mRoboticArmMove->getAngleForServo(SERVO_IDX_UPPER_JOINT), 0);
		lastMove->setServo(SERVO_IDX_CLAW_ROTATE, mRoboticArmMove->getDirectionForServo(SERVO_IDX_CLAW_ROTATE), mRoboticArmMove->getAngleForServo(SERVO_IDX_CLAW_ROTATE), 0);
		lastMove->setServo(SERVO_IDX_CLAWS, mRoboticArmMove->getDirectionForServo(SERVO_IDX_CLAWS), 0, mRoboticArmMove->getMmForServo(SERVO_IDX_CLAWS));
	}

	/*
	 * 1) rotate base to right direction
	 * 2) calculate 3 joints - for object pickup
	 */
	void pickupObject(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		pickupObjectCalculateArmRotation(mImagePreprocessItem, mRoboticArmMove);
		pickupObjectCalculateArmLean(mImagePreprocessItem, mRoboticArmMove);
		mRoboticArmMove->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, CLAWS_OBJECT_MAX_MM);
	}

	/*
	 * rotate base to right direction
	 */
	void pickupObjectRotation(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		pickupObjectCalculateArmRotation(mImagePreprocessItem, mRoboticArmMove);
		mRoboticArmMove->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, CLAWS_OBJECT_MAX_MM);
	}

	/*
	 * rotate base to right direction
	 */
	void pickupObjectLean(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		pickupObjectCalculateArmLean(mImagePreprocessItem, mRoboticArmMove);
		mRoboticArmMove->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, CLAWS_OBJECT_MAX_MM);
	}

};




#endif /* SRC_APPLICATION_MODULES_IFACE_CVMODULE_H_ */
