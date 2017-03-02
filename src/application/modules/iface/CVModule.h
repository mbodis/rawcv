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

using namespace std;
using namespace cv;

static int BASE_HEIGHT_MM = 88;

static int ARM_PART_1_MM = 105;
static int ARM_PART_2_MM = 60;
static int ARM_PART_3_MM = 135;

static int OBJECT_DEPTH_MM = 88;

static int ARM_ANGLE_1_DEFAULT = 90;
static int ARM_ANGLE_2_DEFAULT = 180;
static int ARM_ANGLE_3_DEFAULT = 180;

class CVModule{

protected:

	bool DEBUG_LOCAL = false;
	bool finished = true;
	bool enabled = true;
	string moduleName;
	int numberStates = MODULE_STATE_NONE;
	int currentState = MODULE_STATE_NONE;


	/*
	 * arm rotation
	 */
	void calculateArmRotation(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
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

		mRoboticArmMove->setServo(SERVO_IDX_BASE, direction, angle);
		if (DEBUG_LOCAL) cout << "direction: " << (direction==DIRECTION_LEFT ? "left" : "right") << " " << angle << " degree" << endl;
	}

	/*
	 * arm lean
	 * TODO continue
	 */
	void calculateArmLean(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){


		int armAngle1 = 0;
		int armAngle2 = 0;
		int armAngle3 = 0;
		bool found = false;

		// TODO continue here
		int baseJointX = 0;
		int baseJointY = 0;

		for(int armAngle1=0; armAngle1<90; armAngle1++){
			for(int armAngle2=0; armAngle2<90; armAngle2++){
				for(int armAngle3=0; armAngle3<90; armAngle3++){

					//arm part 1
					int joint1X = MathHelper::sinDeg(180+armAngle1) * ARM_PART_1_MM;
					int joint1Y = MathHelper::sinDeg(90+armAngle1) * ARM_PART_1_MM;
					Point joint1Center = Point(baseJointX - joint1X, baseJointY - joint1Y);


					//arm part 2
					int joint2X = MathHelper::sinDeg(180+armAngle2) * ARM_PART_2_MM;
					int joint2Y = MathHelper::sinDeg(90+armAngle2) * ARM_PART_2_MM;
					Point joint2Center = Point(joint1Center.x - joint2X, joint1Center.y - joint2Y);


					//arm part 3
					int joint3X = MathHelper::sinDeg(180+armAngle3) * ARM_PART_3_MM;
					int joint3Y = MathHelper::sinDeg(90+armAngle3) * ARM_PART_3_MM;
					Point joint3Center = Point(joint2Center.x - joint3X, joint2Center.y - joint3Y);

					// TODO continue here
					if (true){

					}
				}
			}
		}

		if (!found){
			armAngle1 = 30;
			armAngle2 = 60;
			armAngle3 = 35;
		}

		mRoboticArmMove->setServo(SERVO_IDX_BOTTOM_JOINT, DIRECTION_FORWARD, armAngle1);
		mRoboticArmMove->setServo(SERVO_IDX_MIDDLE_JOINT, DIRECTION_FORWARD, armAngle2);
		mRoboticArmMove->setServo(SERVO_IDX_UPPER_JOINT, DIRECTION_FORWARD, armAngle3);

	}

public:

	CVModule(string moduleName){
		this->moduleName = moduleName;
	}

	~CVModule(){

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
		calculateArmRotation(mImagePreprocessItem, mRoboticArmMove);
		calculateArmLean(mImagePreprocessItem, mRoboticArmMove);
	}

};




#endif /* SRC_APPLICATION_MODULES_IFACE_CVMODULE_H_ */
