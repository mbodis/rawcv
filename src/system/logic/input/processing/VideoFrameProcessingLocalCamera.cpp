/*
 * VideoFrameProcessingLocalCamera.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: mbodis
 */

#include "VideoFrameProcessingLocalCamera.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <string>

using namespace std;
using namespace cv;

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <thread>
#include "../path/SourcePath.h"
#include "../../../config/Constants.h"

/* SHARED PROCESSING LABEL*/
bool isInputFinished2 = false;
Mat frameShared2, processingFrame2;
long videoTimeShared2 = 0;
thread videoThread;
/*
 * sourcePath   ==> path to .avi or url
 * isRT         ==> (RT like Real Time) if is urlPath then have true value
 * */
void VideoFrameProcessingLocalCamera::runRTV(SourcePath *sourcePath) {

	VideoCapture stream1(sourcePath->cameraIdx);

	if (!stream1.isOpened()) {
		cout << "camera index " << sourcePath->cameraIdx << " is not working please try different idx" << endl;
		throw std::logic_error(" VideoFrameProcessingLocalCamera Error opening camera stream");
	}

	stream1.set(CV_CAP_PROP_FRAME_WIDTH,1920);
	stream1.set(CV_CAP_PROP_FRAME_HEIGHT,1080);

	while (!isInputFinished2) {

		// use cuurent time - time form camera is 0
        // videoTimeShared2 = stream1.get(CV_CAP_PROP_POS_MSEC);
        struct timeval tp;
		gettimeofday(&tp, NULL);
		long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		videoTimeShared2 = tp.tv_sec * 1000 + tp.tv_usec / 1000;

		if (!stream1.read(frameShared2)) {
			stream1.release();
			isInputFinished2 = true;
		}
	}
}

void VideoFrameProcessingLocalCamera::start() {
	videoThread = thread(runRTV, sourcePath);
    bool isRunningLocal = true;
	while (!isInputFinished2) {
        if (frameShared2.dims > 0){

            processingFrame2 = frameShared2.clone();

            isRunningLocal = mImageAnalyser->analyse(processingFrame2, videoTimeShared2);
        }

        if (!isRunningLocal){
            isInputFinished2  = true;
            break;
        }
    }

	videoThread.join();
}


void VideoFrameProcessingLocalCamera::startEveryFrame() {

	VideoCapture stream1(sourcePath->cameraIdx);

	stream1.set(CV_CAP_PROP_BUFFERSIZE, 1);
	if (!stream1.isOpened())
		throw "Error opening video stream or file";

	int v_ts;
	Mat mat;

	bool isRunning = true;
	while(isRunning){
		if (!stream1.isOpened()) { //check if video device has been initialized
			cout << "cannot open video file" << endl;
			isRunning = false;
		}else{
			v_ts = stream1.get(CV_CAP_PROP_POS_MSEC);
			stream1 >> mat;
			if (mat.dims == 0){
				cout << "end of video file" << endl;
				break;
			}
			isRunning = mImageAnalyser->analyse(mat, v_ts);
		}
	}
}
