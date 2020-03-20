/*
 * VideoFrameProcessingLocalCamera.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: mbodis
 */

#include "VideoFrameProcessingLocalCamera.h"

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
		MyLog::log(LOG_ERROR, "VideoFrameProcessingLocalCamera", "camera index " + to_string(sourcePath->cameraIdx) + " is not working please try different idx");
		throw std::logic_error(" VideoFrameProcessingLocalCamera Error opening camera stream");
	}

	stream1.set(CV_CAP_PROP_FRAME_WIDTH,1920);
	stream1.set(CV_CAP_PROP_FRAME_HEIGHT,1080);
    struct timeval tp;

	while (!isInputFinished2) {

		// use current time - time form camera is 0
        // videoTimeShared2 = stream1.get(CV_CAP_PROP_POS_MSEC);
		gettimeofday(&tp, NULL);
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
		throw std::logic_error("Error opening video stream or file");

	int v_ts;
	Mat mat;

	bool isRunning = true;
	while(isRunning){
		if (!stream1.isOpened()) { //check if video device has been initialized
			MyLog::log(LOG_ERROR, "VideoFrameProcessingLocalCamera", "cannot open video file");
			isRunning = false;
		}else{
			v_ts = stream1.get(CV_CAP_PROP_POS_MSEC);
			stream1 >> mat;
			if (mat.dims == 0){
				MyLog::log(LOG_ERROR, "VideoFrameProcessingLocalCamera", "end of video file");
				break;
			}
			isRunning = mImageAnalyser->analyse(mat, v_ts);
		}
	}
}
