/*
 * RotateBBHelper.h
 *
 *  Created on: Feb 22, 2017
 *      Author: mbodis
 */

#ifndef SRC_SYSTEM_HELPER_ROTATEBBHELPER_H_
#define SRC_SYSTEM_HELPER_ROTATEBBHELPER_H_

#include <opencv2/core.hpp>
#include "../../application/config/ConfigExample.h"
#include <opencv2/highgui/highgui.hpp>
#include "MathHelper.h"

using namespace cv;

/*
 * Bounding Box helper methods
 */
class RotateBBHelper{
private:

public:
	static Point getLbRotatedRect(RotatedRect);
	static Point getLtRotatedRect(RotatedRect);
	static Point getRbRotatedRect(RotatedRect);
	static Point getRtRotatedRect(RotatedRect);
};



#endif /* SRC_SYSTEM_HELPER_ROTATEBBHELPER_H_ */
