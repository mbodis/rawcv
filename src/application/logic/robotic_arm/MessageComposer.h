/*
 * MessageComposer.h
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_ROBOTIC_ARM_MESSAGECOMPOSER_H_
#define SRC_APPLICATION_LOGIC_ROBOTIC_ARM_MESSAGECOMPOSER_H_

class MessageComposer{
private:
	char* composeControllerMessage(int port, int milis, int speed);
	char* composeControllerMessage(int port, int milis);

public:
	char* composeMessage(int servoIdx, int angleDegrese);

};



#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_MESSAGECOMPOSER_H_ */
