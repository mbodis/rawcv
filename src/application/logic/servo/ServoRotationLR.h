/*
 * ServoRotationLR.h
 *
 *  Created on: Jan 30, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_SERVO_SERVOROTATIONLR_H_
#define SRC_APPLICATION_LOGIC_SERVO_SERVOROTATIONLR_H_

#include <iostream>
#include <algorithm>

using namespace std;

class ServoRotationLR : public ServoIface{
private:

	int directionLeft = 0; // 1 == adding move to this direction, -1 == subtract to this direction
	int directionRight = 0; // 1 == adding move to this direction, -1 == subtract to this direction
	double angle = 0; // one degree in milis

public:
	ServoRotationLR(int idx, string name, int minMilisPosition, int maxMilisPosition, int directionLeft, int directionRight,
			double angle):
		ServoIface(idx, name, SERVO_MAIN_TYPE_ROTATION_LR, minMilisPosition, maxMilisPosition){
		this->directionLeft = directionLeft;
		this->directionRight = directionRight;
		this->angle = angle;
	}

	int getMaxLeftAngle(){
		if (directionLeft == 1){
			return abs(SERVO_POSITION_DEFAULT_MILIS - this->maxMilisPosition) / this->angle;

		}else if (directionLeft == -1){
			return abs(SERVO_POSITION_DEFAULT_MILIS - this->minMilisPosition) / this->angle;
		}

		cout << "getMaxLeftAngle: invalid direction servo idx:" << idx << endl;
		throw std::logic_error("getMaxLeftAngle: invalid direction for ServoRotationLR");
	}

	int getMaxRightAngle(){
		if (directionRight == 1){
			return abs(SERVO_POSITION_DEFAULT_MILIS - this->maxMilisPosition) / this->angle;

		}else if (directionRight == -1){
			return abs(SERVO_POSITION_DEFAULT_MILIS - this->minMilisPosition) / this->angle;
		}

		cout << "getMaxRightAngle: invalid direction servo idx:" << idx << endl;
		throw std::logic_error("getMaxRightAngle: invalid direction for ServoRotationLR");
	}

	int convertAngleToMilisLeft(int angle){
		if (angle <= getMaxLeftAngle()){
			if (directionLeft == 1){
				return SERVO_POSITION_DEFAULT_MILIS + (angle * this->angle);

			}else if (directionLeft == -1){
				return SERVO_POSITION_DEFAULT_MILIS - (angle * this->angle);
			}
		}

		cout << "convertAngleToMilisLeft: invalid angle, servo idx:" << idx << endl;
		return SERVO_POSITION_DEFAULT_MILIS;
	}

	int convertAngleToMilisRight(int angle){
		if (angle <= getMaxRightAngle()){
			if (directionRight == 1){
				return SERVO_POSITION_DEFAULT_MILIS + (angle * this->angle);

			}else if (directionRight == -1){
				return SERVO_POSITION_DEFAULT_MILIS - (angle * this->angle);
			}
		}

		cout << "convertAngleToMilisRight: invalid angle, servo idx:" << idx << endl;
		return SERVO_POSITION_DEFAULT_MILIS;
	}
};



#endif /* SRC_APPLICATION_LOGIC_SERVO_SERVOROTATIONLR_H_ */
