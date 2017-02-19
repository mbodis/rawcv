/*
 * ServoMove.h
 *
 *  Created on: Feb 10, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_SERVO_SERVOMOVE_H_
#define SRC_APPLICATION_LOGIC_SERVO_SERVOMOVE_H_

class ServoMove{
private:

public:
	int type = -1;
	int direction = DIRECTION_UNDEFINED;
	int angle = 0;

	ServoMove(int direction, int angle, int type){
		this->direction = direction;
		this->angle = angle;
		this->type = type;
	}
};



#endif /* SRC_APPLICATION_LOGIC_SERVO_SERVOMOVE_H_ */
