/*
 * ImagePreprocessItem.h
 *
 *  Created on: Feb 23, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_IMAGE_IMAGEPREPROCESSITEM_H_
#define SRC_APPLICATION_LOGIC_IMAGE_IMAGEPREPROCESSITEM_H_



class ImagePreprocessItem{
public:

	ImagePreprocessItem(Mat fullInputFrame, Mat preprocessFrame, Point armCenter, vector<RotatedRect> detectedObjects ){
		this->fullInputFrame = fullInputFrame;
		this->preprocessFrame = preprocessFrame;
		this->armCenter = armCenter;
		this->detectedObjects = detectedObjects;
	}

	// raw input
	Mat fullInputFrame;

	// preprocessed content
	Mat preprocessFrame;
	Point armCenter;
	vector<RotatedRect> detectedObjects;
};


#endif /* SRC_APPLICATION_LOGIC_IMAGE_IMAGEPREPROCESSITEM_H_ */
