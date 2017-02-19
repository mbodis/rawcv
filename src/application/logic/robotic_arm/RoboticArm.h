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
		this->servo[0] = new ServoRotationLR(SERVO_IDX_BASE, "base", 430, 2520, 1, -1, 10.55);
		this->servo[1] = new ServoRotationFB(SERVO_IDX_BOTTOM_JOINT, "bottom joint", 430, 2520, -1, 1, 10.55);
		this->servo[2] = new ServoRotationFB(SERVO_IDX_MIDDLE_JOINT, "middle joint", 430, 2520, 1, -1, 10.55);
		this->servo[3] = new ServoRotationFB(SERVO_IDX_UPPER_JOINT, "upper joint", 430, 2520, -1, 1, 10.55);
		this->servo[4] = new ServoRotationLR(SERVO_IDX_CLAW_ROTATE, "claw rotation", 430, 2520, 1, -1, 10.55);
		this->servo[5] = new ServoRange(SERVO_IDX_CLAWS, "claws", 1500, 2520, 1, -1, 2.04, 500);

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

	string composeNextMove(){
		return MessageComposer::composeControllerFullMessage(
				this->servo[0], this->nextMove->getAngleForServo(SERVO_IDX_BASE), this->nextMove->getDirectionForServo(SERVO_IDX_BASE),
				this->servo[1], this->nextMove->getAngleForServo(SERVO_IDX_BOTTOM_JOINT), this->nextMove->getDirectionForServo(SERVO_IDX_BOTTOM_JOINT),
				this->servo[2], this->nextMove->getAngleForServo(SERVO_IDX_MIDDLE_JOINT), this->nextMove->getDirectionForServo(SERVO_IDX_MIDDLE_JOINT),
				this->servo[3], this->nextMove->getAngleForServo(SERVO_IDX_UPPER_JOINT), this->nextMove->getDirectionForServo(SERVO_IDX_UPPER_JOINT),
				this->servo[4], this->nextMove->getAngleForServo(SERVO_IDX_CLAW_ROTATE), this->nextMove->getDirectionForServo(SERVO_IDX_CLAW_ROTATE),
				this->servo[5], this->nextMove->getAngleForServo(SERVO_IDX_CLAWS), this->nextMove->getDirectionForServo(SERVO_IDX_CLAWS)
				);

	}

};



#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARM_H_ */
