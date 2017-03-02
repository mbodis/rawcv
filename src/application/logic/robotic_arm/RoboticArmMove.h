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
	int objectIdx = -1;
	vector<ServoMove*> servoMoveSetup;

public:
	RoboticArmMove(){
		// custom setup
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_BASE));
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_BOTTOM_JOINT));
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_MIDDLE_JOINT));
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_UPPER_JOINT));
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_CLAW_ROTATE));
		this->servoMoveSetup.push_back(new ServoMove(DIRECTION_UNDEFINED, 0, SERVO_IDX_CLAWS));
	}

	void setObjectIndex(int objectIdx){
		this->objectIdx = objectIdx;
	}

	int getObjectIndex(){
		return this->objectIdx;
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

	void setServo(int type, int direction, int angle){
		for(int i=0; i< servoMoveSetup.size(); i++){
			if (servoMoveSetup[i]->type == type){
				servoMoveSetup[i]->direction = direction;
				servoMoveSetup[i]->angle = angle;
			}
		}
	}

	bool isMoveDefault(){
		for(int i=0; i< servoMoveSetup.size(); i++){
			if (servoMoveSetup[i]->angle != 0){
				return false;
			}
		}

		return true;
	}

	void setDefaultPosition(){
		this->servoMoveSetup[SERVO_IDX_BASE]->direction = DIRECTION_LEFT;
		this->servoMoveSetup[SERVO_IDX_BASE]->angle = 0;

		this->servoMoveSetup[SERVO_IDX_BOTTOM_JOINT]->direction = DIRECTION_FORWARD;
		this->servoMoveSetup[SERVO_IDX_BOTTOM_JOINT]->angle = 0;

		this->servoMoveSetup[SERVO_IDX_MIDDLE_JOINT]->direction = DIRECTION_FORWARD;
		this->servoMoveSetup[SERVO_IDX_MIDDLE_JOINT]->angle = 0;

		this->servoMoveSetup[SERVO_IDX_UPPER_JOINT]->direction = DIRECTION_FORWARD;
		this->servoMoveSetup[SERVO_IDX_UPPER_JOINT]->angle = 0;

		this->servoMoveSetup[SERVO_IDX_CLAW_ROTATE]->direction = DIRECTION_LEFT;
		this->servoMoveSetup[SERVO_IDX_CLAW_ROTATE]->angle = 0;

		this->servoMoveSetup[SERVO_IDX_CLAWS]->direction = DIRECTION_OPEN;
		this->servoMoveSetup[SERVO_IDX_CLAWS]->angle = 0;
	}

};




#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMMOVE_H_ */
