/*
 * MessageComposer.cpp
 *
 *  Created on: Jan 27, 2017
 *      Author: mbodis
 */

#include "MessageComposer.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
using namespace std;


/*
 * #1P1500T3000\r\n
 * #servo-indexPtime-in-milis-as-rotationTtime-for-execution\r\n
 */
string MessageComposer::composeControllerMessage(int servoIdx, int servoMilis, int timeMilis){

	stringstream result;
	result << "#";
	result << servoIdx;
	result << "P";
	result << servoMilis;
	result << "T";
	result << timeMilis;
	result << "\r\n";
	return result.str();
}

/*
 * #1P1500T3000\r\n
 * #servo-indexPtime-in-milis-as-rotationTtime-for-execution\r\n
 */
string MessageComposer::composeControllerMessage(ServoIface *servo, int direction, int angle_mm){
	return composeControllerMessage(servo->getIdx(), servo->convertAngleMmToMilis(direction, angle_mm), SERVO_SPEED_NORMAL);
}

/*
 * #1P1500#2P2300#3P2300#4P2300#5P2300#6P2300T100\r\n
 * #servo-indexPtime-in-milis-as-rotation#servo-indexPtime-in-milis-as-rotation...Ttime-for-execution\r\n
 */
string MessageComposer::composeControllerFullMessage(int servo1Milis, int servo2Milis,
			int servo3Milis, int servo4Milis, int servo5Milis, int servo6Milis, int timeMilis){

	stringstream result;
		result << "#";
		result << "1";
		result << "P";
		result << servo1Milis;

		result << "#";
		result << "2";
		result << "P";
		result << servo2Milis;

		result << "#";
		result << "3";
		result << "P";
		result << servo3Milis;

		result << "#";
		result << "4";
		result << "P";
		result << servo4Milis;

		result << "#";
		result << "5";
		result << "P";
		result << servo5Milis;

		result << "#";
		result << "6";
		result << "P";
		result << servo6Milis;


		result << "T";
		result << timeMilis;
		result << "\r\n";

		return result.str();
}

/*
 * #1P1500#2P2300#3P2300#4P2300#5P2300#6P2300T100\r\n
 * #servo-indexPtime-in-milis-as-rotation#servo-indexPtime-in-milis-as-rotation...Ttime-for-execution\r\n
 */
string MessageComposer::composeControllerFullMessage(
		ServoIface *servo1, int direction1, int angle1,
		ServoIface *servo2, int direction2, int angle2,
		ServoIface *servo3, int direction3, int angle3,
		ServoIface *servo4, int direction4, int angle4,
		ServoIface *servo5, int direction5, int angle5,
		ServoIface *servo6, int direction6, int angle6
		){

	int servo1Milis = servo1->convertAngleMmToMilis(direction1, angle1);
	int servo2Milis = servo2->convertAngleMmToMilis(direction2, angle2);
	int servo3Milis = servo3->convertAngleMmToMilis(direction3, angle3);
	int servo4Milis = servo4->convertAngleMmToMilis(direction4, angle4);
	int servo5Milis = servo5->convertAngleMmToMilis(direction5, angle5);
	int servo6Milis = servo6->convertAngleMmToMilis(direction6, angle6);

	return composeControllerFullMessage(servo1Milis, servo2Milis,
			servo3Milis, servo4Milis, servo5Milis, servo6Milis, SERVO_SPEED_NORMAL);
}








