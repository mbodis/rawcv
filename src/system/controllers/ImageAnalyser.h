
#ifndef IMAGEANALYSER_H
#define IMAGEANALYSER_H


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "DebugFrames.h"


/*
 * main logic 
 */
class ImageAnalyser {
private:
    int inputMode, printMode;
    DebugFrames* mDebugFrames;
    
public:
    Mat inputFrame;
    ImageAnalyser(ConfigExample *config);

    bool analyse(Mat, int videoTime);
    bool keyaboardInput(Mat*);
    void initCustom();
    virtual void executeCustomLogic(Mat, int);

};


#endif //IMAGEANALYSER_H
