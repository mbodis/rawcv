/*
 * RoboticArm.h
 *
 *  Created on: Jan 31, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARM_H_
#define SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARM_H_

#include "../servo/ServoIface.h"
#include "../servo/ServoRange.h"
#include "../servo/ServoRotationFB.h"
#include "../servo/ServoRotationLR.h"
#include "RoboticArmMove.h"
#include "../usb_controller/MessageComposer.h"
#include "../../config/AppConfig.h"

const int SERVO_COUNT = 6;


class RoboticArm {
private:
	/*
	 * robotic arm real servo motors
	 */
	ServoIface *servo[SERVO_COUNT];

	/*
	 * next move for arm
	 */
	RoboticArmMove *nextMove;

public:
	RoboticArm(){
		for (int i = 0; i< SERVO_COUNT; i++){
			this->servo[i] = AppConfig::getServoConfiguration(i);
		}

		// empty move
		this->nextMove = new RoboticArmMove();
	}

	~RoboticArm(){

	}

	ServoIface* getServo(int idx){
		return servo[idx];
	}

	void resetNextMove(){
		this->nextMove = new RoboticArmMove();
	}

	void setNextMove(RoboticArmMove newMove){
		this->nextMove = &newMove;
	}

	RoboticArmMove* getNextMove(){
		return this->nextMove;
	}

	/*
	 * make do more generic ?
	 */
	string composeNextMove(){
		return MessageComposer::composeControllerFullMessage(
				this->servo[0], this->nextMove->getDirectionForServo(SERVO_IDX_BASE), this->nextMove->getAngleForServo(SERVO_IDX_BASE),
				this->servo[1], this->nextMove->getDirectionForServo(SERVO_IDX_BOTTOM_JOINT), this->nextMove->getAngleForServo(SERVO_IDX_BOTTOM_JOINT),
				this->servo[2], this->nextMove->getDirectionForServo(SERVO_IDX_MIDDLE_JOINT), this->nextMove->getAngleForServo(SERVO_IDX_MIDDLE_JOINT),
				this->servo[3], this->nextMove->getDirectionForServo(SERVO_IDX_UPPER_JOINT), this->nextMove->getAngleForServo(SERVO_IDX_UPPER_JOINT),
				this->servo[4], this->nextMove->getDirectionForServo(SERVO_IDX_CLAW_ROTATE), this->nextMove->getAngleForServo(SERVO_IDX_CLAW_ROTATE),
				this->servo[5], this->nextMove->getDirectionForServo(SERVO_IDX_CLAWS), this->nextMove->getMmForServo(SERVO_IDX_CLAWS)
				);

	}

};



#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARM_H_ */
