/*
 * ImagePreprocessItem.h
 *
 *  Created on: Feb 23, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_IMAGE_IMAGEPREPROCESSITEM_H_
#define SRC_APPLICATION_LOGIC_IMAGE_IMAGEPREPROCESSITEM_H_



class ImagePreprocessItem{
private:
	bool contentIsSet;
	int objectIdx = -1;

public:

	ImagePreprocessItem(){
		this->contentIsSet = false;
	}

	ImagePreprocessItem(Mat fullInputFrame, Mat preprocessFrame, Point armCenter, vector<RotatedRect> detectedObjects, double oneMmInPx ){
		this->fullInputFrame = fullInputFrame;
		this->preprocessFrame = preprocessFrame;
		this->armCenter = armCenter;
		this->detectedObjects = detectedObjects;
		this->oneMmInPx = oneMmInPx;

		this->contentIsSet = true;
	}

	bool hasContent(){
		return this->contentIsSet;
	}

	void setObjectIndex(int objectIdx){
		this->objectIdx = objectIdx;
	}

	int getObjectIndex(){
		return this->objectIdx;
	}

	// raw input
	Mat fullInputFrame;

	// preprocessed content
	Mat preprocessFrame;
	Point armCenter;
	vector<RotatedRect> detectedObjects;
	double oneMmInPx;
};


#endif /* SRC_APPLICATION_LOGIC_IMAGE_IMAGEPREPROCESSITEM_H_ */
