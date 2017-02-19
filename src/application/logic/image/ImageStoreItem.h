/*
 * ImageStoreItem.h
 *
 *  Created on: Feb 8, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_IMAGE_IMAGESTOREITEM_H_
#define SRC_APPLICATION_LOGIC_IMAGE_IMAGESTOREITEM_H_


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

class ImageStoreItem{
public:
	ImageStoreItem(String name, Mat mMat){
		this->name = name;
		this->mMat = mMat;
	}
	String name;
	Mat mMat;
};



#endif /* SRC_APPLICATION_LOGIC_IMAGE_IMAGESTOREITEM_H_ */
