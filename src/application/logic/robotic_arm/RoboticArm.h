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

const int SERVO_BASE = 0;
const int SERVO_BOTTOM_JOINT = 1;
const int SERVO_MIDDLE_JOINT = 2;
const int SERVO_UPPER_JOINT = 3;
const int SERVO_CLAW_ROTATE = 4;
const int SERVO_CLAWS = 5;

class RoboticArm {
private:
	ServoIface *servo[6];

public:
	RoboticArm(){
		servo[0] = new ServoRotationLR(1, "base", 430, 2520, 1, -1, 10.55);
		servo[1] = new ServoRotationFB(2, "bottom joint", 430, 2520, -1, 1, 10.55);
		servo[2] = new ServoRotationFB(3, "middle joint", 430, 2520, 1, -1, 10.55);
		servo[3] = new ServoRotationFB(4, "upper joint", 430, 2520, -1, 1, 10.55);
		servo[4] = new ServoRotationLR(5, "claw rotation", 430, 2520, 1, -1, 10.55);
		servo[5] = new ServoRange(6, "claws", 1500, 2520, 1, -1, 2.04, 500);
	}

	ServoIface* getServo(int idx){
		return servo[idx];
	}

	~RoboticArm(){

	}
};



#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARM_H_ */
