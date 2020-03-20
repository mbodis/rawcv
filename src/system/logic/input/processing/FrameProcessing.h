
#ifndef TYRERECOGNITION_VIDEOFRAMEPROCESSING_H
#define TYRERECOGNITION_VIDEOFRAMEPROCESSING_H

#include <opencv2/highgui/highgui.hpp>
#include "../path/SourcePath.h"
#include "../../../config/Constants.h"
#include "../../../../application/config/AppConfig.h"


using namespace std;
using namespace cv;


class FrameProcessing {
protected:
	SourcePath* sourcePath;
	AppConfig* c;

public:

	virtual void start() { throw std::logic_error("FrameProcessing start Use subclass"); };
	virtual ~FrameProcessing(){ throw std::logic_error("FrameProcessing ~FrameProcessing Use subclass"); };

	FrameProcessing(SourcePath* sourcePathInput, AppConfig* mAppConfig){
        this->sourcePath = sourcePathInput;
        this->c = mAppConfig;
	}
};

#endif //TYRERECOGNITION_RTVFRAMEPROCESSING_H
