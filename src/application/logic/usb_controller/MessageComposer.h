/*
 * MessageComposer.h
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_ROBOTIC_ARM_MESSAGECOMPOSER_H_
#define SRC_APPLICATION_LOGIC_ROBOTIC_ARM_MESSAGECOMPOSER_H_

#include <iostream>
#include "../servo/ServoIface.h"
#include "../../../system/config/Constants.h"

using namespace std;

/*
 * "#"+channel+"P"+servoLocation+"T"+servoSpeed+"\r\n"
 *
 * ONE SERVO MESSAGE:
 * #1P1500T3000\r\n
 *
 * MULTI SERVO MESSAGE:
 * #1P1500#2P2300#3P2300#4P2300#5P2300#6P2300T100\r\n
 */
class MessageComposer{
private:

public:
	static string composeControllerMessage(int servoIdx, int servoMilis, int timeMilis);
	static string composeControllerMessage(ServoIface *servo, int direcion, int angle);

	static string composeControllerFullMessage(int servo1Milis, int servo2Milis,
			int servo3Milis, int servo4Milis, int servo5Milis, int servo6Milis, int timeMilis);

	static string composeControllerFullMessage(
			ServoIface *servo1, int angle1, int direction1,
			ServoIface *servo2, int angle2, int direction2,
			ServoIface *servo3, int angle3, int direction3,
			ServoIface *servo4, int angle4, int direction4,
			ServoIface *servo5, int angle5, int direction5,
			ServoIface *servo6, int angle6, int direction6
			);


};



#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_MESSAGECOMPOSER_H_ */
