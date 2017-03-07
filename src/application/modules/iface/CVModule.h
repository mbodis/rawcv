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
#include "../../logic/image/ImagePreprocessItem.h"
#include "../../../system/helper/MathHelper.h"
#include "../../config/AppConfig.h"

using namespace std;
using namespace cv;


class CVModule{

protected:

	bool DEBUG_LOCAL = false;
	bool finished = true;
	bool enabled = true;
	string moduleName;
	int numberStates = MODULE_STATE_NONE;
	int currentState = MODULE_STATE_NONE;

	long lastMoveMilis = 0;
	int timeTriggerSec = -1;
	ImagePreprocessItem *lastMovePreprocessItem;
	RoboticArmMove *lastMove;

	/*
	 * arm rotation
	 */
	void pickupObjectCalculateArmRotation(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		if (DEBUG_LOCAL) cout << "train rotation" << endl;
		if (DEBUG_LOCAL) cout << "plane: " << mImagePreprocessItem->preprocessFrame.cols << " x " << mImagePreprocessItem->preprocessFrame.rows << endl;
		if (DEBUG_LOCAL) cout << "arm: [" << mImagePreprocessItem->armCenter.x << "," << mImagePreprocessItem->armCenter.y << "]" << endl;
		if (DEBUG_LOCAL) cout << "object: [" << (int)mImagePreprocessItem->detectedObjects[0].center.x << "," << (int)mImagePreprocessItem->detectedObjects[0].center.y << "]" << endl;

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
		if (DEBUG_LOCAL) cout << "direction: " << (direction==DIRECTION_LEFT ? "left" : "right") << " " << angle << " degree" << endl;
	}

	/*
	 * arm lean
	 */
	void pickupObjectCalculateArmLean(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		if (DEBUG_LOCAL) cout << "calculateArmLean--------------------------" << endl;

		int ARM_PART_1_PX = ARM_PART_1_MM * mImagePreprocessItem->oneMmInPx;
		int ARM_PART_2_PX = ARM_PART_2_MM * mImagePreprocessItem->oneMmInPx;
		int ARM_PART_3_PX = ARM_PART_3_MM * mImagePreprocessItem->oneMmInPx;

		// base as [0, 0]
		int baseJointX = 0;
		int baseJointY = 0;

		int objectX = cv::norm((Point)mImagePreprocessItem->armCenter - (Point) mImagePreprocessItem->detectedObjects[mRoboticArmMove->getObjectIndex()].center );
		int objectY = 0;
		if (DEBUG_LOCAL) cout << "objectX: " << objectX << " objectY: " << objectY << endl;

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
					if (DEBUG_LOCAL) cout << "joint1X: " << joint1X << " joint1Y: " << joint1Y << endl;
					if (DEBUG_LOCAL) cout << "joint1CenterX: " << joint1Center.x << " joint1CenterY: " << joint1Center.y << endl;

					//arm part 2
					int joint2X = MathHelper::sinDeg(180+angle2) * ARM_PART_2_PX;
					int joint2Y = MathHelper::sinDeg(90+angle2) * ARM_PART_2_PX;
					Point joint2Center = Point(joint1Center.x - joint2X, joint1Center.y - joint2Y);
					if (DEBUG_LOCAL) cout << "joint2X: " << joint2X << " joint2Y: " << joint2Y << endl;
					if (DEBUG_LOCAL) cout << "joint2CenterX: " << joint2Center.x << " joint2CenterY: " << joint2Center.y << endl;

					//arm part 3
					int joint3X = MathHelper::sinDeg(180+angle3) * ARM_PART_3_PX;
					int joint3Y = MathHelper::sinDeg(90+angle3) * ARM_PART_3_PX;
					Point joint3Center = Point(joint2Center.x - joint3X, joint2Center.y - joint3Y);
					if (DEBUG_LOCAL) cout << "joint3X: " << joint3X << " joint3Y: " << joint3Y << endl;
					if (DEBUG_LOCAL) cout << "joint3CenterX: " << joint3Center.x << " joint3CenterY: " << joint3Center.y << endl;

					// add some limitations (arm should not hit table)
					// 50 mm above table is closest for arm TODO later
					if ( joint3Center.y > -2 * mImagePreprocessItem->oneMmInPx){
						continue;
					}

					double objectArmEndFromObjectPx = cv::norm(joint3Center - Point(objectX, objectY));
					double objectDistanceFromArmMm = objectArmEndFromObjectPx * mImagePreprocessItem->oneMmInPx;
					if (DEBUG_LOCAL) cout << "objectArmEndFromObjectPx: " << objectArmEndFromObjectPx << endl;
					if (DEBUG_LOCAL) cout << "objectDistanceFromArmMm: " << objectDistanceFromArmMm << endl;

					if (objectDistanceFromArmMm <= ARM_MAX_DISTANCE_PICKUP_MM){

						if (minDistance > objectDistanceFromArmMm){
							if (DEBUG_LOCAL) cout << "found better: angle1=" << armAngle1 << "° angle2="
								<< armAngle2 << "° angle3=" << armAngle3 << "° dist=" << objectDistanceFromArmMm << endl;
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

		cout << "OBJECT DETECTED angle1=" << minAngle1 << "° angle2=" << minAngle2 << "° angle3=" << minAngle3 << "° minDistance=" << minDistance <<  " y:" << yCord << endl;

		mRoboticArmMove->setServo(SERVO_IDX_BOTTOM_JOINT, DIRECTION_FORWARD, minAngle1, 0);
		mRoboticArmMove->setServo(SERVO_IDX_MIDDLE_JOINT, DIRECTION_FORWARD, minAngle2, 0);
		mRoboticArmMove->setServo(SERVO_IDX_UPPER_JOINT, DIRECTION_FORWARD, minAngle3, 0);
	}


public:

	CVModule(string moduleName){
		this->moduleName = moduleName;
	}

	~CVModule(){

	}

	void setTimeTrigger(int sec){
		this->timeTriggerSec = sec;
	}
	void unsetTimeTrigger(){
		this->timeTriggerSec = -1;
	}
	bool executeTimeTrigger(){
		if (this->timeTriggerSec == -1){
			return false;
		}

		struct timeval tp;
		gettimeofday(&tp, NULL);
		long now = tp.tv_sec * 1000 + tp.tv_usec / 1000;

		if (now - this->lastMoveMilis / 1000 > this->timeTriggerSec){
			unsetTimeTrigger();
			return true;
		}

		return false;
	}


	// save last move Obj
	void saveLastMoveObj(ImagePreprocessItem *mImagePreprocessItem){
		if (!mImagePreprocessItem->hasContent()) return;

		lastMovePreprocessItem = mImagePreprocessItem;
	}

	void restoreArmMove(RoboticArmMove *mRoboticArmMove){
		mRoboticArmMove->setServo(SERVO_IDX_BASE, lastMove->getDirectionForServo(SERVO_IDX_BASE), lastMove->getAngleForServo(SERVO_IDX_BASE), 0);
		mRoboticArmMove->setServo(SERVO_IDX_BOTTOM_JOINT, lastMove->getDirectionForServo(SERVO_IDX_BOTTOM_JOINT), lastMove->getAngleForServo(SERVO_IDX_BOTTOM_JOINT), 0);
		mRoboticArmMove->setServo(SERVO_IDX_MIDDLE_JOINT, lastMove->getDirectionForServo(SERVO_IDX_MIDDLE_JOINT), lastMove->getAngleForServo(SERVO_IDX_MIDDLE_JOINT), 0);
		mRoboticArmMove->setServo(SERVO_IDX_UPPER_JOINT, lastMove->getDirectionForServo(SERVO_IDX_UPPER_JOINT), lastMove->getAngleForServo(SERVO_IDX_UPPER_JOINT), 0);
		mRoboticArmMove->setServo(SERVO_IDX_CLAW_ROTATE, lastMove->getDirectionForServo(SERVO_IDX_CLAW_ROTATE), lastMove->getAngleForServo(SERVO_IDX_CLAW_ROTATE), 0);
		mRoboticArmMove->setServo(SERVO_IDX_CLAWS, lastMove->getDirectionForServo(SERVO_IDX_CLAWS), 0, lastMove->getMmForServo(SERVO_IDX_CLAWS));
	}

	void saveLastMove(RoboticArmMove *mRoboticArmMove){
		this->lastMove = mRoboticArmMove;
	}

	// save last move time
	void saveLastMoveTime(){
		struct timeval tp;
		gettimeofday(&tp, NULL);
		lastMoveMilis = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	}

	virtual bool initialObjectDetection(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		if (DEBUG_LOCAL) cout << "CVmodule initialObjectDetection" << endl;
		throw std::logic_error(" CVmodule initialObjectDeection - method not implemented");
	}

	virtual void processNextState(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		throw std::logic_error(" CVmodule processNextState - method not implemented ");
	}

	virtual void resetContent(){
		throw std::logic_error(" CVmodule resetContent - method not implemented");
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

	void checkIfModuleHasEnded(){
		if (currentState >= numberStates){
			currentState = 0;
			resetContent();
		}
	}

	/*
	 * 1) rotate base to right direction
	 * 2) calculate 3 joints - for object pickup
	 */
	void pickupObject(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		pickupObjectCalculateArmRotation(mImagePreprocessItem, mRoboticArmMove);
		pickupObjectCalculateArmLean(mImagePreprocessItem, mRoboticArmMove);
		mRoboticArmMove->setServo(SERVO_IDX_CLAWS, DIRECTION_OPEN, 0, 500);
	}

};




#endif /* SRC_APPLICATION_MODULES_IFACE_CVMODULE_H_ */
