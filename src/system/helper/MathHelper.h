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

	static double calcMHWScore(vector<int> scores) {
		if (scores.size() == 0)
			return -1;

		double median;
		size_t size = scores.size();

		sort(scores.begin(), scores.end());

		if (size % 2 == 0) {
			median = (scores[size / 2 - 1] + scores[size / 2]) / 2;
		} else {
			median = scores[size / 2];
		}

		return median;
	}

	/*
	 * comparator for
	 */
	static struct PointCompareYClass {
		bool operator()(cv::Point2f pt1, cv::Point2f pt2) {
			return (pt1.y < pt2.y);
		}
	} mPointCompareYClass;

	/*
	 *
	 */
	static vector<Point2f> sortPointsByYAxe(vector<Point2f> points) {
		std::sort(points.begin(), points.end(), mPointCompareYClass);
		return points;
	}

	static double sinInverseDeg(double val){
		return  ((asin(val)* 180 ) / M_PI);
	}

	static double sinDeg(double val){
		return  sin(val * M_PI / 180);
	}

	static double cosDeg(double val){
		return  cos(val * M_PI / 180);
	}

};


#endif /* SRC_HELPER_MATHHELPER_H_ */
