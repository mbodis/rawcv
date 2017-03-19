/*
 * BBHelper.h
 *
 *  Created on: Jun 13, 2016
 *      Author: mbodis
 */

#ifndef SRC_HELPER_BBHELPER_H_
#define SRC_HELPER_BBHELPER_H_

#include <iostream>
#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../../application/config/AppConfig.h"

using namespace std;
using namespace cv;

/*
 * Bounding Box helper methods
 */
class BBHelper{
private:

public:
	bool isBBFillingAllHeight(Mat*, Rect);
	bool isBBAboveBB(AppConfig*, Mat*, Rect, Rect);
};




#endif /* SRC_HELPER_BBHELPER_H_ */
