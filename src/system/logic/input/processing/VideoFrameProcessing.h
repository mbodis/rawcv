/*
 * VideoFrameProcessing.h
 *
 *  Created on: Jul 14, 2016
 *      Author: mbodis
 */

#ifndef SRC_LOGIC_FRAME_PROCESSING_VIDEOFRAMEPROCESSING_H_
#define SRC_LOGIC_FRAME_PROCESSING_VIDEOFRAMEPROCESSING_H_

#include <thread>

#include <opencv2/highgui/highgui.hpp>

#include "../path/SourcePath.h"
#include "../../../controllers/ImageAnalyser.h"
#include "../../../config/Constants.h"
#include "FrameProcessing.h"

using namespace std;
using namespace cv;

class VideoFrameProcessing : public FrameProcessing{
private:    
	string TAG = "VideoFrameProcessing";
    ImageAnalyser *mImageAnalyser;
    static void runRTV(SourcePath* path);
    
public:

	VideoFrameProcessing(SourcePath* sourcePath, AppConfig* mAppConfig, ImageAnalyser* mImageAnalyser):
        FrameProcessing(sourcePath, mAppConfig){
		this->mImageAnalyser = mImageAnalyser;
	}

	void start();
};



#endif /* SRC_LOGIC_FRAME_PROCESSING_VIDEOFRAMEPROCESSING_H_ */
