/*
 * MyImageAnalyser.h
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_CONTROLLERS_MYIMAGEANALYSER_H_
#define SRC_APPLICATION_CONTROLLERS_MYIMAGEANALYSER_H_

#include "../controllers/MyImageAnalyser.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../../system/controllers/ImageAnalyser.h"
#include "../config/ConfigExample.h"
#include "../logic/image/ImageStorage.h"
#include "../../system/helper/RotateBBHelper.h"

using namespace cv;
using namespace std;

static double THRESHOLD_MOVE_PERCENTAGE = 0.02;



class MyImageAnalyser: public ImageAnalyser{
private:

	bool DEBUG_LOCAL = false;

	ImageStorage *mImageStorage;

	// video time
	long video_time = 0;
	long last_move_time = 0;
	bool resiseInput = true;

	// resize
	double resizeRatio = 2;
	int resizedWidth = -1;
	int resizedHeight = -1;

	// input
	Mat originalColorFrame;
	Mat outputColorFrame, outputColorStretchFrame;
	void saveInputFrame(Mat, long);
	long startTime = 0;

	//output
	void showImg(Mat, String);
	void showOutput();
	void showImageFromBackground();

	// logic detect movement
	int THRESHOLD_MOVE_MILIS = 800;
	double THRESHOLD_MOVE_PERCENTAGE = 0.03;
	int THRESHOLD_MOVEMENT_BW_VALUE = 120;
	bool isFrameMoving = true;
	double whitePercentage = 0.0;
	Mat detectMovementGrayFrame; // gray scale frame
	Mat detectMovementLastGrayFrame; // previous gray scale frame
	bool interestingFrame = false; // state between moving and not moving
	void detectMovement();
	void detectMovementResetStates();
	void setFrameIsMoving(bool);

	// logic - crop and stretch table
	int TABLE_CORNER_OFFSER_PERCENTAGE = 7;
	void cropAndStretchTableBg();
	bool tableDetected = false;
	Rect tableBb;
	Mat binaryTableCrop;
	Point2f lt, lb, rt, rb;

	// logic - recalculate px -> mm
	bool mmToPxCalculated = false;
	int TABLE_WIDTH_MM = 655;
	int TABLE_HEIGHT_MM = 955;
	double oneMmInPx = 0;
	int oneMmInPxframeWidth = 0;
	int oneMmInPxframeHeight = 0;
	void recalculatePxToMm(Point, Point, int, int);

	// logic - detect arm
	void detectArm();
	bool armDetected = false;
	Point armCenter = Point(0,0);
	Rect armBb;

	// logic - is there an object
	int processImgCount = 0;
	bool isObjectDetected = false;
	void detectObjects();
	vector<RotatedRect> detectedObjects;
	Mat binaryWithoutArm;

	// logic - if there is something interesting
	void saveImageForProcessing();

	//output
	void drawOutputInfo(Mat);

	// GUI trackball slider
	const int slider_max = 255;
	int sliderThresholdBg = 43;
	void initTrackball();
	static void on_trackbar( int val, void* ){
		// this is not necessary
		//cout<< "v: "<< val << endl;
	}

public:
	MyImageAnalyser() : ImageAnalyser(){
		this->mImageStorage = &ImageStorage::getInstance();
		initTrackball();
	}
	~MyImageAnalyser(){ }

	void executeCustomLogic(Mat, long);

};



#endif /* SRC_APPLICATION_CONTROLLERS_MYIMAGEANALYSER_H_ */
