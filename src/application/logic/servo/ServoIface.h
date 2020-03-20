/*
 * ServoIface.h
 *
 *  Created on: Jan 30, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_SERVO_SERVOIFACE_H_
#define SRC_APPLICATION_LOGIC_SERVO_SERVOIFACE_H_

#include "../../../system/config/Constants.h"

using namespace std;

static int SERVO_MAIN_TYPE_ROTATION_LR = 1;
static int SERVO_MAIN_TYPE_ROTATION_FB = 2;
static int SERVO_MAIN_TYPE_RANGE = 3;

static int SERVO_POSITION_DEFAULT_MILIS = 1500;

static int SERVO_SPEED_FASTEST = 100;
static int SERVO_SPEED_NORMAL = 1500;
static int SERVO_SPEED_SLOWEST = 9000;

class ServoIface{
protected:
	int idx;
	string name;
	int mainType;
	int minMilisPosition;
	int maxMilisPosition;

public:
	ServoIface(int idx, string name, int mainType,
			int minMilisPosition, int maxMilisPosition){
		this->idx = idx;
		this->name = name;
		this->mainType = mainType;
		this->minMilisPosition = minMilisPosition;
		this->maxMilisPosition = maxMilisPosition;
	};

	int getIdx(){
		return idx;
	}

	~ServoIface(){

	}

	virtual int convertAngleToMilisForward(int angle){ throw std::logic_error(" ServoIface convertAngleToMilisForward - method not implemented"); };
	virtual int convertAngleToMilisBackward(int angle){ throw std::logic_error(" ServoIface convertAngleToMilisBackward - method not implemented"); };
	virtual int convertAngleToMilisLeft(int angle){ throw std::logic_error(" ServoIface convertAngleToMilisLeft - method not implemented"); };
	virtual int convertAngleToMilisRight(int angle){ throw std::logic_error(" ServoIface convertAngleToMilisRight - method not implemented"); };
	virtual int convertMmToMilis(int mm){ throw std::logic_error(" ServoIface convertMmToMilis - method not implemented"); };

	int convertAngleMmToMilis(int direction, int angle_mm){
		switch(direction){
			case DIRECTION_LEFT:
				return convertAngleToMilisLeft(angle_mm);
			case DIRECTION_RIGHT:
					return convertAngleToMilisRight(angle_mm);
			case DIRECTION_FORWARD:
					return convertAngleToMilisForward(angle_mm);
			case DIRECTION_BACKWARD:
					return convertAngleToMilisBackward(angle_mm);
			case DIRECTION_OPEN:
			case DIRECTION_CLOSE:
				return convertMmToMilis(angle_mm);
		}

		return SERVO_POSITION_DEFAULT_MILIS;
	}

};



#endif /* SRC_APPLICATION_LOGIC_SERVO_SERVOIFACE_H_ */
