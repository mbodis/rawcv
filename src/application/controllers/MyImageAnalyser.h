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

using namespace cv;
using namespace std;


class MyImageAnalyser: public ImageAnalyser{
private:
	int video_time = 0;
	int last_move_time = 0;
	bool resiseInput = true;

	double resizeRatio = 2;
	int resizedWidth = -1;
	int resizedHeight = -1;

	// BGR original frame
	Mat originalColorFrame;
	// gray scale frame
	Mat grayFrame;
	// previous gray scale frame
	Mat lastGrayFrame;

	Mat outputColorFrame;

	// logic
	bool isFrameMoving = true;

	// state when changing between
	bool interestingFrame = false;

	// result of pre-processing - object was
	bool isObjectDetected = false;


	ImageStorage *mImageStorage;
public:
	MyImageAnalyser() : ImageAnalyser(){
		this->mImageStorage = &ImageStorage::getInstance();
	}
	~MyImageAnalyser(){ }

	void setFrameIsMoving(bool);

	void executeCustomLogic(Mat, int);
	void saveImageForProcessing();

	void saveInputFrame(Mat, int);
	void resetStates();

	void detectMovement(Mat*, Mat*);

	void showImg(Mat, String);
	void showOutput();
	void showImageFromBackground();
};



#endif /* SRC_APPLICATION_CONTROLLERS_MYIMAGEANALYSER_H_ */
