/*
 * ConfigExample.cpp
 *
 *  Created on: Jul 11, 2016
 *      Author: mbodis
 */

#include "AppConfig.h"

#include "../../system/helper/FileSystemHelper.h"

using namespace std;

#include "iostream"
#include "../../libs/json/JSON.h"

#include <stddef.h>  // defines NULL


// Global static pointer used to ensure a single instance of the class.
AppConfig* AppConfig::m_pInstance = NULL;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/

AppConfig* AppConfig::Instance()
{
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new AppConfig;

   return m_pInstance;
}

/*
 * parse string value by key from JSON structure
 */
string getStrByKey(wstring wkey, JSONObject root){

	string key(wkey.begin(), wkey.end());

	if (root.find(wkey) == root.end() || !root[wkey]->IsString()){
		MyLog::log(LOG_INFO, "AppConfig", "json file required " + key + " attr");
		return "";
	}
	wstring ws = root[wkey]->AsString();
	return string(ws.begin(), ws.end());
}

/*
 * parse number by key from JSON structure
 */
double getNumberByKey(wstring wkey, JSONObject root){

	string key(wkey.begin(), wkey.end());

	if (root.find(wkey) == root.end() || !root[wkey]->IsNumber()){
		MyLog::log(LOG_INFO, "AppConfig", "json file required " + key + " attr");
		return -1;
	}

	return  root[wkey]->AsNumber();
}

/*
 * parse scalar - color scalar
 * 	(json object {"r":123,"g":123,"b":123})
 */
Scalar getScalarByKey(wstring wkey, JSONObject root){

	string key(wkey.begin(), wkey.end());

	if (root.find(wkey) == root.end() || !root[wkey]->IsObject()){
		MyLog::log(LOG_INFO, "AppConfig", "json file required " + key + " attr");
		return Scalar(0,0,0);
	}

	JSONObject obj = root[wkey]->AsObject();
	int b = obj[L"b"]->AsNumber();
	int g = obj[L"g"]->AsNumber();
	int r = obj[L"r"]->AsNumber();

	return  Scalar(b,g,r);
}

/*
 * parse Point from JSON structure
 * 	(json object {"x":45,"y":456}
 */
Point getPointByKey(wstring wkey, JSONObject root){

	string key(wkey.begin(), wkey.end());

	if (root.find(wkey) == root.end() || !root[wkey]->IsObject()){
		MyLog::log(LOG_INFO, "AppConfig", "json file required " + key + " attr");
		return Point(-1, -1);
	}

	JSONObject obj = root[wkey]->AsObject();
	int x = obj[L"x"]->AsNumber();
	int y = obj[L"y"]->AsNumber();

	return  Point(x, y);
}

/*
 * parsing constant form JSON file
 */
AppConfig::AppConfig(){

	this->inputMode = INPUT_MODE;
	this->printMode = PRINT_MODE;
	FileSystemHelper mFileSystemHelper;
	string s = mFileSystemHelper.getFileContent(EXAMPLE_CONFIG);
	MyLog::log(LOG_INFO, TAG, s);

	char *json_string = (char*) s.c_str();

	JSONObject root, localObj;
	JSONValue *value = JSON::Parse(json_string);

	if (value == NULL) {
		MyLog::log(LOG_INFO, TAG, "Json failed to parse: " + s);
	} else {
		root = value->AsObject();

		VIDEO_NAME = string(FOLDER_ASSETS).append(getStrByKey(L"VIDEO_NAME", root));
		FOLDER = string(FOLDER_ASSETS).append(getStrByKey(L"FOLDER", root));
        URL = string().append(getStrByKey(L"URL", root));
        CAMERA_IDX  = getNumberByKey(L"CAMERA_IDX", root);

        //
        // add your constants here
        //

        setFontSize(-1, -1);
		debugPrintOut();
	}
}

void AppConfig::initFontSize(Mat *frame){
    if (isFontSizeSet) 
        return;
    
    IMG_WIDTH = frame->cols;
    IMG_HEIGHT = frame->rows;
    setFontSize(IMG_WIDTH, IMG_HEIGHT);
    
    isFontSizeSet = true;
}

void AppConfig::setFontSize(int width, int height){
    if ( (width == -1 && height == -1) 
        || (width == 1280 && height == 720) ){
        // fonts and line height - 1280:720
        FONT_SIZE = 0.5;
        FONT_SIZE_SM = 0.4;
        ROW = 10; // row height

    }else{
        // fonts and line height - 1920:1080
        FONT_SIZE = 0.8;
        FONT_SIZE_SM = 0.6;
        ROW = 20; // row height
    }
}

void AppConfig::debugPrintOut(){

	MyLog::log(LOG_INFO, TAG, "VIDEO_NAME:" + VIDEO_NAME);
	MyLog::log(LOG_INFO, TAG, "FOLDER:" + FOLDER);
	MyLog::log(LOG_INFO, TAG, "FOLDER_NAME:" + ((string)FOLDER_NAME) );
	MyLog::log(LOG_INFO, TAG, "URL:" + URL);
	MyLog::log(LOG_INFO, TAG, "CAMERA_IDX:" + to_string(CAMERA_IDX));

	MyLog::log(LOG_INFO, TAG, "FONT_SIZE:" + to_string(FONT_SIZE));
	MyLog::log(LOG_INFO, TAG, "FONT_SIZE_SM:" + to_string(FONT_SIZE_SM));
	MyLog::log(LOG_INFO, TAG, "ROW:" + to_string(ROW));

}


/*
 *FIRST SETUP:
 *
	case SERVO_IDX_BASE:
		return new ServoRotationLR(SERVO_IDX_BASE, "base", 430, 2520, 1, -1, 10.55);
	case SERVO_IDX_BOTTOM_JOINT:
		return new ServoRotationFB(SERVO_IDX_BOTTOM_JOINT, "bottom joint", 430, 2520, -1, 1, 10.55);
	case SERVO_IDX_MIDDLE_JOINT:
		return new ServoRotationFB(SERVO_IDX_MIDDLE_JOINT, "middle joint", 430, 2520, 1, -1, 10.55);
	case SERVO_IDX_UPPER_JOINT:
		return new ServoRotationFB(SERVO_IDX_UPPER_JOINT, "upper joint", 430, 2520, -1, 1, 10.55);
	case SERVO_IDX_CLAW_ROTATE:
		return new ServoRotationLR(SERVO_IDX_CLAW_ROTATE, "claw rotation", 430, 2520, 1, -1, 10.55);
	case SERVO_IDX_CLAWS:
		return new ServoRange(SERVO_IDX_CLAWS, "claws", 1500, 2520, 1, -1, 2.04, 500);
	}
*/
ServoIface* AppConfig::getServoConfiguration(int idx){
		switch(idx){

		case SERVO_IDX_BASE:
			return new ServoRotationLR(SERVO_IDX_BASE, "base", 430, 2520, 1, -1, 10.55);
		case SERVO_IDX_BOTTOM_JOINT:
			return new ServoRotationFB(SERVO_IDX_BOTTOM_JOINT, "bottom joint", 430, 2520, -1, 1, 10.55);
		case SERVO_IDX_MIDDLE_JOINT:
			return new ServoRotationFB(SERVO_IDX_MIDDLE_JOINT, "middle joint", 430, 2520, 1, -1, 10.55);
		case SERVO_IDX_UPPER_JOINT:
			return new ServoRotationFB(SERVO_IDX_UPPER_JOINT, "upper joint", 430, 2520, -1, 1, 10.55);
		case SERVO_IDX_CLAW_ROTATE:
			return new ServoRotationLR(SERVO_IDX_CLAW_ROTATE, "claw rotation", 430, 2520, 1, -1, 10.55);
		case SERVO_IDX_CLAWS:
			return new ServoRange(SERVO_IDX_CLAWS, "claws", 1500, 2520, 1, -1, 20.04, 500);
		}

		return NULL;
	}
