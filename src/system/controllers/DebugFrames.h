/*
 * DebugFrames.h
 *
 *  Created on: May 23, 2016
 *      Author: mbodis
 */

#ifndef SRC_LOGIC_DEBUGFRAMES_H_
#define SRC_LOGIC_DEBUGFRAMES_H_


#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../../application/config/AppConfig.h"
#include "../../system/config/Constants.h"
#include "../../system/helper/FileSystemHelper.h"
#include "../../system/helper/DrawMethodHelper.h"
#include "../../system/helper/MyLog.h"

using namespace std;
using namespace cv;



/*
 * debug options:
 * - toggle between multiple filters, images, pre-processed states
 * - draw status information on video
 * - draw video time, image name
 */
class DebugFrames {

private:
	string TAG = "DebugFrames";
	
    // save output
    FileSystemHelper mFileSystemHelper;
	int imageSaveCounter = 0;

	// mode
	int typePrintInfo = PRINT_MODE_DEBUG;	
	bool debugWindow = false;	

    // config    
    int INPUT_MODE;

    void drawStatusTime(Mat*, int);
	void printDebugInfo(Mat*);
	void printPresentationInfo(Mat*);
    
public:
	AppConfig *c;

	DebugFrames();
	~DebugFrames(void);
	
	void drawStatus(Mat*, int);
	void saveFrameToFile(Mat*);
    void keyInput(char, Mat*);

    void toggleDebugWindow();
};


#endif /* SRC_LOGIC_DEBUGFRAMES_H_ */
