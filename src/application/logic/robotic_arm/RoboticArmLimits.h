/*
 * RoboticArmLimits.h
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMLIMITS_H_
#define SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMLIMITS_H_
/*
 * servo motors are counting from bottom to top
 * starting with index 1
 */

static int SERVO_1_BASE = 1;
static int SERVO_2_BOTTOM_JOINT = 2;
static int SERVO_3_MIDDLE_JOINT = 3;
static int SERVO_4_TOP_JOINT = 4;
static int SERVO_5_CLAW_ROTATE = 5;
static int SERVO_6_CLAW = 6;

/* base rotation */
static int SERVO_1_MIN = 455; // TODO
static int SERVO_1_MIN_ANGLE = 0; // TODO
static int SERVO_1_MAX = 455; // TODO
static int SERVO_1_MAX_ANGLE = 45; // TODO


/* base joint */
static int SERVO_2_MIN = 455; // TODO
static int SERVO_2_MIN_ANGLE = 455; // TODO
static int SERVO_2_MAX = 455; // TODO
static int SERVO_2_MAX_ANGLE = 455; // TODO


/* middle joint */
static int SERVO_3_MIN = 455; // TODO
static int SERVO_3_MIN_ANGLE = 455; // TODO
static int SERVO_3_MAX = 455; // TODO
static int SERVO_3_MAX_ANGLE = 455; // TODO


/* middle joint upper*/
static int SERVO_4_MIN = 455; // TODO
static int SERVO_4_MIN_ANGLE = 455; // TODO
static int SERVO_4_MAX = 455; // TODO
static int SERVO_4_MAX_ANGLE = 455; // TODO


/* claw rotation */
static int SERVO_5_MIN = 455; // TODO
static int SERVO_5_MIN_ANGLE = 455; // TODO
static int SERVO_5_MAX = 455; // TODO
static int SERVO_5_MAX_ANGLE = 455; // TODO


/* claws */
static int SERVO_6_MIN = 455; // TODO
static int SERVO_6_MIN_MM = 455; // TODO
static int SERVO_6_MAX = 1500; // TODO
static int SERVO_6_MAX_MM = 1500; // TODO




#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_ROBOTICARMLIMITS_H_ */
