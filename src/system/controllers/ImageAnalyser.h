
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
    
public:
    DebugFrames* mDebugFrames;
    Mat inputFrame;
    ImageAnalyser();
    ~ImageAnalyser();

    bool analyse(Mat, long videoTime);
    bool keyaboardInput(Mat*);
    void initCustom();
    virtual void executeCustomLogic(Mat, long);

};


#endif //IMAGEANALYSER_H
