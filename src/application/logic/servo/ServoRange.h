/*
 * ServoRange.h
 *
 *  Created on: Jan 30, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_SERVO_SERVORANGE_H_
#define SRC_APPLICATION_LOGIC_SERVO_SERVORANGE_H_

#include "../../../system/helper/MyLog.h"

class ServoRange : public ServoIface{
private:
	string TAG = "ServoRange";

public:
	int directionOpen = 0; // 1 == adding move to this direction, -1 == subtract to this direction
	int directionClose = 0; // 1 == adding move to this direction, -1 == subtract to this direction
	double mmMilis = 0; // one milimeter in milis
	int maxRangeMm = 0 ; // max range in milimeters

	ServoRange(int idx, string name, int minMilisPosition, int maxMilisPosition, int directionOpen, int directionClose,
			double mmMilis, int maxRangeMm):
			ServoIface(idx, name, SERVO_MAIN_TYPE_RANGE, minMilisPosition, maxMilisPosition){
		this->directionOpen = directionOpen;
		this->directionClose = directionClose;
		this->mmMilis = mmMilis;
		this->maxRangeMm = maxRangeMm;
	}

	int getMaxRangeMm(){
		return maxRangeMm;
	}

	int convertMmToMilis(int mm){
		if (mm <= maxRangeMm){
			if (directionOpen == 1){
				return SERVO_POSITION_DEFAULT_MILIS + (mm * this->mmMilis);

			}else if (directionOpen == -1){
				return SERVO_POSITION_DEFAULT_MILIS - (mm * this->mmMilis);
			}
		}

		MyLog::log(LOG_ERROR, TAG, "convertMmToMilis: invalid mm size, servo idx:" + to_string(idx));
		return SERVO_POSITION_DEFAULT_MILIS;
	}
};



#endif /* SRC_APPLICATION_LOGIC_SERVO_SERVORANGE_H_ */
