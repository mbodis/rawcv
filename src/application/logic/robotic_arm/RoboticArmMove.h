/*
 * RoboticArmMove.h
 *
 *  Created on: Feb 9, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMMOVE_H_
#define SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMMOVE_H_

#include "../servo/ServoMove.h"
#include "../../../system/config/Constants.h"

using namespace std;

const int SERVO_IDX_BASE = 0;
const int SERVO_IDX_BOTTOM_JOINT = 1;
const int SERVO_IDX_MIDDLE_JOINT = 2;
const int SERVO_IDX_UPPER_JOINT = 3;
const int SERVO_IDX_CLAW_ROTATE = 4;
const int SERVO_IDX_CLAWS = 5;

class RoboticArmMove{
private:
	vector<ServoMove*> servoMoveSetup;

public:
	RoboticArmMove(){
		// cusom setup
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_BASE));
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_BOTTOM_JOINT));
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_MIDDLE_JOINT));
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_UPPER_JOINT));
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_CLAW_ROTATE));
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_CLAWS));
	}

	bool hasChanged(){
		for(int i=0; i< servoMoveSetup.size(); i++){
			if (servoMoveSetup[i]->direction != DIRECTION_UNDEFINED){
				return true;
			}
		}

		return false;
	}

	int getAngleForServo(int servoIdx){
		for(int i=0; i< servoMoveSetup.size(); i++){
			if (this->servoMoveSetup[i]->type == servoIdx){
				return this->servoMoveSetup[i]->angle;
			}
		}

		return 0;
	}

	int getDirectionForServo(int servoIdx){
		for(int i=0; i< servoMoveSetup.size(); i++){
			if (this->servoMoveSetup[i]->type == servoIdx){
				return this->servoMoveSetup[i]->direction;
			}
		}

		return DIRECTION_UNDEFINED;
	}

};




#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMMOVE_H_ */
