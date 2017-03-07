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
#include "../../config/AppConfig.h"

using namespace std;

class RoboticArmMove{
private:
	int objectIdx = -1;
	vector<ServoMove*> servoMoveSetup;

public:

	/*
	 * make do more generic ?
	 */
	RoboticArmMove(){
		// custom setup
		this->servoMoveSetup.push_back(new ServoMove(SERVO_IDX_BASE, DIRECTION_UNDEFINED, 0, 0));
		this->servoMoveSetup.push_back(new ServoMove(SERVO_IDX_BOTTOM_JOINT, DIRECTION_UNDEFINED, 0, 0));
		this->servoMoveSetup.push_back(new ServoMove(SERVO_IDX_MIDDLE_JOINT, DIRECTION_UNDEFINED, 0, 0));
		this->servoMoveSetup.push_back(new ServoMove(SERVO_IDX_UPPER_JOINT, DIRECTION_UNDEFINED, 0, 0));
		this->servoMoveSetup.push_back(new ServoMove(SERVO_IDX_CLAW_ROTATE, DIRECTION_UNDEFINED, 0, 0));
		this->servoMoveSetup.push_back(new ServoMove(SERVO_IDX_CLAWS, DIRECTION_UNDEFINED, 0, 0));
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

	int getMmForServo(int servoIdx){
		for(int i=0; i< servoMoveSetup.size(); i++){
			if (this->servoMoveSetup[i]->type == servoIdx){
				return this->servoMoveSetup[i]->mm;
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

	void setServo(int type, int direction, int angle, int mm){
		for(int i=0; i< servoMoveSetup.size(); i++){
			if (servoMoveSetup[i]->type == type){
				servoMoveSetup[i]->direction = direction;
				servoMoveSetup[i]->angle = angle;
				servoMoveSetup[i]->mm = mm;
			}
		}
	}

	bool isMoveDefault(){
		for(int i=0; i< servoMoveSetup.size(); i++){
			if (servoMoveSetup[i]->angle != 0 || servoMoveSetup[i]->mm != 0){
				return false;
			}
		}

		return true;
	}

	/*
	 * make do more generic ?
	 */
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
		this->servoMoveSetup[SERVO_IDX_CLAWS]->mm = 500;
	}

};




#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMMOVE_H_ */
