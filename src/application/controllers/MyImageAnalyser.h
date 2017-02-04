/*
 * MyImageAnalyser.h
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_CONTROLLERS_MYIMAGEANALYSER_H_
#define SRC_APPLICATION_CONTROLLERS_MYIMAGEANALYSER_H_

#include "../controllers/MyImageAnalyser.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../../system/controllers/ImageAnalyser.h"
#include "../config/ConfigExample.h"

using namespace cv;
using namespace std;


class MyImageAnalyser: public ImageAnalyser{
public:
	MyImageAnalyser(ConfigExample *config) : ImageAnalyser(config){
		cout << "NEW INIT " << endl;
		// TODO initial my logic class
	}
	void executeCustomLogic(Mat, int);
};



#endif /* SRC_APPLICATION_CONTROLLERS_MYIMAGEANALYSER_H_ */
