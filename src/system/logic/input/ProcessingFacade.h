/*
 * ProcessingFacade.h
 *
 *  Created on: Jul 14, 2016
 *      Author: mbodis
 */

#ifndef SRC_LOGIC_PROCESSINGFACADE_H_
#define SRC_LOGIC_PROCESSINGFACADE_H_

#include "../../../application/config/ConfigExample.h"
#include "../../controllers/ImageAnalyser.h"

#include "processing/DirImageFrameProcessing.h"
#include "processing/VideoFrameProcessing.h"
#include "processing/VideoFrameProcessingRunEveryFrame.h"
#include "processing/VideoFrameProcessingLocalCamera.h"

using namespace std;

class ProcessingFacade{    
    
private:
    
    void runDirImageFrameProcessing();
	void runVideoFrameProcessing();
    void runVideoFrameProcessingRunEveryFrame();
	void runURLVideoFrameProcessing();    
	void runLocalCameraFrameProcessing();
	void runImageAnalyse();

    
public:
	ConfigExample *mConfigExample;
	ImageAnalyser *mImageAnalyser;
	ProcessingFacade(ImageAnalyser*);
	~ProcessingFacade();
    
    void runAnalyse();
};



#endif /* SRC_LOGIC_PROCESSINGFACADE_H_ */
