/*
 * ConfigExample.h
 *
 *  Created on: Jul 11, 2016
 *      Author: mbodis
 */

#ifndef SRC_LOGIC_CONFIGEXAMPLE_H_
#define SRC_LOGIC_CONFIGEXAMPLE_H_

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "../../system/config/Constants.h"
#include "../logic/servo/ServoIface.h"
#include "../logic/servo/ServoMove.h"
#include "../logic/servo/ServoRange.h"
#include "../logic/servo/ServoRotationFB.h"
#include "../logic/servo/ServoRotationLR.h"

using namespace std;

/*
 * reload application constants
 */
static char* EXAMPLE_CONFIG = "src/application/config/json/config1.json";

/*
 * setup output mode
 * [ PRINT_MODE_DEBUG, PRINT_MODE_PRESENTATION ]
 */
static int PRINT_MODE = PRINT_MODE_DEBUG;

/*
 * setup input mode
 * [INPUT_MODE_IMG_FOLDER, INPUT_MODE_VIDEO_RT, INPUT_MODE_VIDEO_FRAME, INPUT_MODE_URL, INPUT_MODE_LOCAL_CAMERA]
 */
static int INPUT_MODE = INPUT_MODE_LOCAL_CAMERA;

static int BASE_HEIGHT_MM = 88;

static int ARM_PART_1_MM = 105;
static int ARM_PART_2_MM = 60;
static int ARM_PART_3_MM = 135;

static int OBJECT_DEPTH_MM = 88;

static int ARM_ANGLE_1_DEFAULT = 90;
static int ARM_ANGLE_2_DEFAULT = 180;
static int ARM_ANGLE_3_DEFAULT = 180;

static int ARM_MAX_DISTANCE_PICKUP_MM = 25;

const int SERVO_IDX_BASE = 0;
const int SERVO_IDX_BOTTOM_JOINT = 1;
const int SERVO_IDX_MIDDLE_JOINT = 2;
const int SERVO_IDX_UPPER_JOINT = 3;
const int SERVO_IDX_CLAW_ROTATE = 4;
const int SERVO_IDX_CLAWS = 5;

/*
 * TODO desc
 */
class AppConfig{
private:
    bool isFontSizeSet = false;

    AppConfig();	// Private so that it can  not be called
    AppConfig(AppConfig const&){};	// copy constructor is private
    AppConfig& operator=(AppConfig const&){};  // assignment operator is private
	static AppConfig* m_pInstance;

public:

	static AppConfig* Instance();
	static AppConfig& getInstance(){
		static AppConfig instance;
		return instance;
	}

	void debugPrintOut();
    void initFontSize(Mat*);
    void setFontSize(int, int);

	string VIDEO_NAME;
	string FOLDER;
	char* FOLDER_NAME = (char*)FOLDER.c_str();
	int inputMode;
	int printMode;
    string URL;
    int CAMERA_IDX;
    
    int IMG_WIDTH;
	int IMG_HEIGHT;
    
    double FONT_SIZE = 0.5;
	double FONT_SIZE_SM = 0.4;
	int ROW = 10; // row height

	static ServoIface* getServoConfiguration(int idx){
		switch(idx){
		case 0:
			return new ServoRotationLR(SERVO_IDX_BASE, "base", 430, 2520, 1, -1, 10.55);
		case 1:
			return new ServoRotationFB(SERVO_IDX_BOTTOM_JOINT, "bottom joint", 430, 2520, -1, 1, 10.55);
		case 2:
			return new ServoRotationFB(SERVO_IDX_MIDDLE_JOINT, "middle joint", 430, 2520, 1, -1, 10.55);
		case 3:
			return new ServoRotationFB(SERVO_IDX_UPPER_JOINT, "upper joint", 430, 2520, -1, 1, 10.55);
		case 4:
			return new ServoRotationLR(SERVO_IDX_CLAW_ROTATE, "claw rotation", 430, 2520, 1, -1, 10.55);
		case 5:
			return new ServoRange(SERVO_IDX_CLAWS, "claws", 1500, 2520, 1, -1, 2.04, 500);
		}

		return NULL;
	}

};

#endif /* SRC_LOGIC_CONFIGEXAMPLE_H_ */
