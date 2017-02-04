/*
 * ServoRotationFB.h
 *
 *  Created on: Jan 30, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_SERVO_SERVOROTATIONFB_H_
#define SRC_APPLICATION_LOGIC_SERVO_SERVOROTATIONFB_H_

class ServoRotationFB : public ServoIface{
private:

	int directionForward = 0; // 1 == adding move to this direction, -1 == subtract to this direction
	int directionBackward = 0; // 1 == adding move to this direction, -1 == subtract to this direction
	double angle = 0; // one degree in milis

public:
	ServoRotationFB(int idx, string name, int minMilisPosition, int maxMilisPosition, int directionForward, int directionBackward,
			double angle):
		ServoIface(idx, name, SERVO_MAIN_TYPE_ROTATION_FB, minMilisPosition, maxMilisPosition){
		this->directionForward = directionForward;
		this->directionBackward = directionBackward;
		this->angle = angle;
	}

	int getMaxForwardAngle(){
		if (directionForward == 1){
			return abs(SERVO_POSITION_DEFAULT_MILIS - this->maxMilisPosition) / this->angle;

		}else if (directionForward == -1){
			return abs(SERVO_POSITION_DEFAULT_MILIS - this->minMilisPosition) / this->angle;
		}

		cout << "getMaxForwardAngle: invalid direction servo idx:" << idx << endl;
		throw std::logic_error("getMaxForwardAngle: invalid direction for ServoRotationFB");
	}

	int getMaxBackwardAngle(){
		if (directionBackward == 1){
			return abs(SERVO_POSITION_DEFAULT_MILIS - this->maxMilisPosition) / this->angle;

		}else if (directionBackward == -1){
			return abs(SERVO_POSITION_DEFAULT_MILIS - this->minMilisPosition) / this->angle;
		}

		cout << "getMaxBackwardAngle: invalid direction servo idx:" << idx << endl;
		throw std::logic_error("getMaxBackwardAngle: invalid direction for ServoRotationFB");
	}

	int convertAngleToMilisForward(int angle){
		if (angle <= getMaxForwardAngle()){
			if (directionForward == 1){
				return SERVO_POSITION_DEFAULT_MILIS + (angle * this->angle);

			}else if (directionForward == -1){
				return SERVO_POSITION_DEFAULT_MILIS - (angle * this->angle);
			}
		}

		return SERVO_POSITION_DEFAULT_MILIS;
	}

	int convertAngleToMilisBackward(int angle){
		if (angle <= getMaxBackwardAngle()){
			if (directionBackward == 1){
				return SERVO_POSITION_DEFAULT_MILIS + (angle * this->angle);

			}else if (directionBackward == -1){
				return SERVO_POSITION_DEFAULT_MILIS - (angle * this->angle);
			}
		}

		return SERVO_POSITION_DEFAULT_MILIS;
	}
};



#endif /* SRC_APPLICATION_LOGIC_SERVO_SERVOROTATIONFB_H_ */
