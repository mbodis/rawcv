/*
 * RotateBBHelper.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: mbodis
 */

#include "RotateBBHelper.h"

/*
 * return left-bottom point from RotateRect
 */
Point RotateBBHelper::getLbRotatedRect(RotatedRect rr) {
	Point2f rect_points[4];
	vector<Point2f> rPoints;

	rr.points(rect_points);
	for (int j = 0; j < 4; j++) {
		rPoints.push_back(rect_points[j]);
	}

	rPoints = MathHelper::sortPointsByYAxe(rPoints);
	if(rPoints.at(2).x < rPoints.at(3).x){
		return rPoints.at(2);
	}else{
		return rPoints.at(3);
	}
}

/*
 * return left-top point from RotateRect
 */
Point RotateBBHelper::getLtRotatedRect(RotatedRect rr) {

	Point2f rect_points[4];
	vector<Point2f> rPoints;

	rr.points(rect_points);
	for (int j = 0; j < 4; j++) {
		rPoints.push_back(rect_points[j]);
	}

	rPoints = MathHelper::sortPointsByYAxe(rPoints);
	if(rPoints.at(0).x < rPoints.at(1).x){
		return rPoints.at(0);
	}else{
		return rPoints.at(1);
	}

}

/*
 * return right-bottom point from RotateRect
 */
Point RotateBBHelper::getRbRotatedRect(RotatedRect rr) {
	Point2f rect_points[4];
	vector<Point2f> rPoints;

	rr.points(rect_points);
	for (int j = 0; j < 4; j++) {
		rPoints.push_back(rect_points[j]);
	}

	rPoints = MathHelper::sortPointsByYAxe(rPoints);
	if(rPoints.at(2).x < rPoints.at(3).x){
		return rPoints.at(3);
	}else{
		return rPoints.at(2);
	}
}

/*
 * return right-top point from RotateRect
 */
Point RotateBBHelper::getRtRotatedRect(RotatedRect rr) {
	Point2f rect_points[4];
		vector<Point2f> rPoints;

		rr.points(rect_points);
		for (int j = 0; j < 4; j++) {
			rPoints.push_back(rect_points[j]);
		}

		rPoints = MathHelper::sortPointsByYAxe(rPoints);
		if(rPoints.at(0).x < rPoints.at(1).x){
			return rPoints.at(1);
		}else{
			return rPoints.at(0);
		}
}


