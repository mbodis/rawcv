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

/*
 * 0) save input frame
 * 1) has something move for 1 sec
 * 2) crop table
 * 3) remove arm from image
 * 4) is there object ?
 * 5) save image for main logic processing
 *
 *
 */
void MyImageAnalyser::executeCustomLogic(Mat frame, int videoTime){

	// 0) save input frame
	saveInputFrame(frame, videoTime);

	// 1) has something move for 1 sec
	if (mDebugFrames->c->inputMode == INPUT_MODE_IMG_FOLDER){
		setFrameIsMoving(false);
	}else{
		detectMovement(&grayFrame, &lastGrayFrame);
	}

	// 2) TODO crop table
	// 3) TODO remove arm from image
	// 4) TODO is there object ?

	// 5) save image for main logic processing
	saveImageForProcessing();

	// show output
	showOutput();
	showImageFromBackground();

	resetStates();
}

void MyImageAnalyser::detectMovement(Mat *grayFrame1, Mat *grayFrame2){

	Mat diffGray, actualDiffBw;

	if (grayFrame1->dims != 0 && grayFrame2->dims != 0) {

		absdiff(*grayFrame2, *grayFrame1, diffGray);
		actualDiffBw = diffGray > 120; // TODO constant

		double whitePercentage = (double)countNonZero(actualDiffBw) * 100
				/ (grayFrame1->cols * grayFrame1->rows);
		// cout << "display moves percentage: " << whitePercentage << endl;
		// cout << "video_time: " << video_time << endl;
		// cout << "last_move_tim: " << last_move_time << endl;
		// cout << "----------------------" << endl;
		if (whitePercentage > 0.02) {
			setFrameIsMoving(true);
			last_move_time = video_time;
		}else{
			if (video_time - last_move_time < 200){
				setFrameIsMoving(true);
			}else{
				setFrameIsMoving(false);
			}
		}
	}else{
		setFrameIsMoving(true);
	}

	string movingTxt = isFrameMoving?"moving":"not moving";
	Scalar color = isFrameMoving?Scalar(0,0,255):Scalar(0,255,0);

	cv::putText(outputColorFrame, movingTxt, cvPoint(5, 55),
					FONT_HERSHEY_COMPLEX_SMALL, 0.8, color, 1, CV_AA);
}

void MyImageAnalyser::setFrameIsMoving(bool newValue){
	if (this->isFrameMoving && newValue == false){
		this->interestingFrame = true;
	}

	this->isFrameMoving = newValue;
}

void MyImageAnalyser::saveImageForProcessing(){
	if (this->interestingFrame && this-> isObjectDetected){

		mImageStorage->addToProcessingQueue("newProcssImg", this->originalColorFrame);
		imshow("new image to process", this->originalColorFrame);
	}
}

void MyImageAnalyser::saveInputFrame(Mat frame, int videoTime){
    if (resizedWidth == -1 || resizedHeight == -1){
        resizedWidth = frame.cols/resizeRatio;
        resizedHeight = frame.rows/resizeRatio;
    }
    this->originalColorFrame = frame.clone();

    if (resiseInput){
        resize(frame, frame, Size(resizedWidth, resizedHeight));
    }
    this->outputColorFrame = frame;

    this->video_time = videoTime;
    cvtColor(outputColorFrame, grayFrame, CV_BGR2GRAY);
    equalizeHist(grayFrame, grayFrame);

    // save img size and setup font size
    mDebugFrames->c->initFontSize(&frame);
}

void MyImageAnalyser::resetStates(){
	this->interestingFrame = false;
    lastGrayFrame = grayFrame.clone();
}

void MyImageAnalyser::showOutput(){
    this->showImg(outputColorFrame, "outpuColorFrame");
//    this->showImg(originalColorFrame, "originalColorFrame");
    // cout<< originalColorFrame.cols << " " << originalColorFrame.rows << endl;
}

/*
 * show images saved through application and show them on main thread - GUI
 */
void MyImageAnalyser::showImageFromBackground(){

	while (mImageStorage->getDisplayQueueSize() >0){
		ImageStoreItem mImageStoreItem = mImageStorage->getImgFromDiplayQueue();
		this->showImg(mImageStoreItem.mMat, mImageStoreItem.name);
	}
}

void MyImageAnalyser::showImg(Mat frame, String frameName){
	if (frame.dims != 0) {
		imshow(frameName, frame);
	} else {
		cout << frameName << " is EMPTY !" << endl;
	}
}
