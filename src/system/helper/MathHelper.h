/*
 * MathHelper.h
 *
 *  Created on: Jun 27, 2016
 *      Author: mbodis
 */

#ifndef SRC_HELPER_MATHHELPER_H_
#define SRC_HELPER_MATHHELPER_H_

#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class MathHelper{
private:

public:
	static double calcMHWScore(vector<int> );
	static vector<Point2f> sortPointsByYAxe(vector<Point2f>);

};


#endif /* SRC_HELPER_MATHHELPER_H_ */
