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

using namespace std;

/*
 * reload application constants
 */
static char* EXAMPLE_CONFIG = "src/application/config/json/example1.json";

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

/*
 * TODO desc
 */
class ConfigExample{
private:
    bool isFontSizeSet = false;

	ConfigExample();	// Private so that it can  not be called
	ConfigExample(ConfigExample const&){};	// copy constructor is private
	ConfigExample& operator=(ConfigExample const&){};  // assignment operator is private
	static ConfigExample* m_pInstance;

public:

	static ConfigExample* Instance();
	static ConfigExample& getInstance(){
		static ConfigExample instance;
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

};

//class S
//{
//    public:
//        static S& getInstance()
//        {
//            static S    instance;
//            return instance;
//        }
//    private:
//        S() {}
//        S(S const&);              // Don't Implement.
//        void operator=(S const&); // Don't implement
// };

#endif /* SRC_LOGIC_CONFIGEXAMPLE_H_ */
