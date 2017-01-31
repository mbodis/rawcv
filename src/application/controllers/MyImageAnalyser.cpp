/*
 * MyImageAnalyser.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#include "../controllers/MyImageAnalyser.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


void MyImageAnalyser::executeCustomLogic(Mat frame, int videoTime){
//	imshow("camera", frame);
	inputFrame = frame.clone();
	// TODO
}

