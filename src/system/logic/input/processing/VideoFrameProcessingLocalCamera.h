/*
 * VideoFrameProcessingLocalCamera.h
 *
 *  Created on: Jan 8, 2017
 *      Author: mbodis
 */

#ifndef SRC_SYSTEM_LOGIC_INPUT_PROCESSING_VIDEOFRAMEPROCESSINGLOCALCAMERA_H_
#define SRC_SYSTEM_LOGIC_INPUT_PROCESSING_VIDEOFRAMEPROCESSINGLOCALCAMERA_H_

#include "../path/SourcePath.h"
#include "FrameProcessing.h"
#include "../../../controllers/ImageAnalyser.h"

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
