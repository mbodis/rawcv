/*
 * DirImageFrameProcessing.h
 *
 *  Created on: Jul 14, 2016
 *      Author: mbodis
 */

#ifndef SRC_LOGIC_FRAME_PROCESSING_DIRIMAGEFRAMEPROCESSING_H_
#define SRC_LOGIC_FRAME_PROCESSING_DIRIMAGEFRAMEPROCESSING_H_

#include <opencv2/highgui/highgui.hpp>

#include "FrameProcessing.h"
#include "../keyboard/abstract/Move.h"
#include "../../../helper/FileSystemHelper.h"
#include "../../../../application/config/AppConfig.h"
#include "../../../controllers/ImageAnalyser.h"

using namespace cv;
using namespace std;


class DirImageFrameProcessing : public FrameProcessing, public Move{
private:
	string TAG = "DirImageFrameProcessing";
	bool isFinished = false;
	int folderSize;
	int selectedImage;
    bool evaluateNextFrame = true;
    Mat currentFrame;

	FileSystemHelper *mFileSystemHelper;
	string selectedImagePath;

    ImageAnalyser* mImageAnalyser;
public:

	DirImageFrameProcessing(AppConfig* mAppConfig, ImageAnalyser* mImageAnalyser, SourcePath* sourcePath);

    void start();
	void quit();
    
    void left();
	void right();
	
};



#endif /* SRC_LOGIC_FRAME_PROCESSING_DIRIMAGEFRAMEPROCESSING_H_ */
