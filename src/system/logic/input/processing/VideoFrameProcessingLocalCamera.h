/*
 * VideoFrameProcessingLocalCamera.h
 *
 *  Created on: Jan 8, 2017
 *      Author: mbodis
 */

#ifndef SRC_SYSTEM_LOGIC_INPUT_PROCESSING_VIDEOFRAMEPROCESSINGLOCALCAMERA_H_
#define SRC_SYSTEM_LOGIC_INPUT_PROCESSING_VIDEOFRAMEPROCESSINGLOCALCAMERA_H_

#include <thread>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <string>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../path/SourcePath.h"
#include "../../../config/Constants.h"
#include "FrameProcessing.h"
#include "../../../controllers/ImageAnalyser.h"

using namespace std;
using namespace cv;

class VideoFrameProcessingLocalCamera : public FrameProcessing{
private:

public:
	VideoFrameProcessingLocalCamera(SourcePath* sourcePath, AppConfig* mAppConfig, ImageAnalyser* mImageAnalyser):
        FrameProcessing(sourcePath, mAppConfig){
		this->mImageAnalyser = mImageAnalyser;
	}

	void start();
	void startEveryFrame();
    static void runRTV(SourcePath *sourcePath);
    ImageAnalyser* mImageAnalyser;
};



#endif /* SRC_SYSTEM_LOGIC_INPUT_PROCESSING_VIDEOFRAMEPROCESSINGLOCALCAMERA_H_ */
