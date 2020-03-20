
#include "../controllers/ImageAnalyser.h"


ImageAnalyser::ImageAnalyser(){

    this->mDebugFrames = new DebugFrames();
    this->inputMode = AppConfig::getInstance().inputMode;
    this->printMode = AppConfig::getInstance().printMode;
}

ImageAnalyser::~ImageAnalyser(){

}

bool ImageAnalyser::analyse(Mat frame, long videoTime) {
    if (frame.dims == 0) return true;

    executeCustomLogic(frame, videoTime);
    //imshow("frame", frame);

    return keyaboardInput(&frame);
}

bool ImageAnalyser::keyaboardInput(Mat *frame){
    // keyboard input action
    char key = (char) waitKey(40);
    mDebugFrames->keyInput(key, frame);
    lastKey = key;
    if (key == 27) return false;
    
    return true;
}

void ImageAnalyser::executeCustomLogic(Mat frame, long videoTime){
	// TODO initial my logic class
}
