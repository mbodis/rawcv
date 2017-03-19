/*
 * VideoFrameProcessingRunEveryFrame.cpp
 *
 *  Created on: Jan 6, 2017
 *      Author: mbodis
 */

#include "VideoFrameProcessingRunEveryFrame.h"
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#include <thread>
#include "../path/SourcePath.h"
#include "../../../config/Constants.h"

void VideoFrameProcessingRunEveryFrame::start() {

	VideoCapture capture;

	capture.set(CV_CAP_PROP_BUFFERSIZE, 1);
	if (!capture.open(c->VIDEO_NAME)){
		throw std::logic_error("VideoFrameProcessingRunEveryFrame::start Error opening video stream or file");
	}

	int v_ts;
	Mat mat;

	bool isRunning = true;
	while(isRunning){
		if (!capture.isOpened()) { //check if video device has been initialized
			MyLog::log(LOG_ERROR, "VideoFrameProcessingRunEveryFrame", "cannot open video file");
			isRunning = false;
		}else{
			v_ts = capture.get(CV_CAP_PROP_POS_MSEC);
			capture >> mat;
			if (mat.dims == 0){
				MyLog::log(LOG_ERROR, "VideoFrameProcessingRunEveryFrame", "end of video file");
				break;
			}
			isRunning = mImageAnalyser->analyse(mat, v_ts);
		}
	}
}


