/*
 * MyImageAnalyser.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#include "../controllers/MyImageAnalyser.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


void MyImageAnalyser::executeCustomLogic(Mat frame, int videoTime){
	imshow("my camera", frame);
	inputFrame = frame.clone();

	// TODO has something move for 1 sec
	// TODO crop table
	// TODO remove arm from image
	// TODO is there object ?

	// save image for main logic processing
	//saveImageForProcessing(frame);


	i++;
	if (i == 100){
		i=0;
		cout<< "save imgg" << endl;
		saveImageForProcessing(frame);
	}
}

void MyImageAnalyser::saveImageForProcessing(Mat frame){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

	stringstream path, min, sec, mili;
	path << FOLDER_IMG_TO_PROCESS;
	path << ms;
	path << ".jpg";
	imwrite(path.str(), frame);
	imshow("image to process", frame);
}

