/*
 * ImageStorage.h
 *
 *  Created on: Feb 6, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_IMAGE_IMAGESTORAGE_H_
#define SRC_APPLICATION_LOGIC_IMAGE_IMAGESTORAGE_H_


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImageDisplayItem.h"
#include "ImagePreprocessItem.h"
using namespace cv;

#include "../../../system/config/Constants.h"

#include <queue>          // std::stack
#include <iostream>
#include <sys/time.h>

using namespace std;

static string IMAGE_VIEW_TOP = "view_top";
static string IMAGE_VIEW_SIDE = "view_side";

/*
 *
 */
class ImageStorage{

private:
	ImageStorage();	// Private so that it can  not be called
	ImageStorage(ImageStorage const&){};	// copy constructor is private
	ImageStorage& operator=(ImageStorage const&){};  // assignment operator is private
	static ImageStorage* m_pInstance;

	std::queue<ImageStoreItem> displayImgQueue;
	std::queue<ImagePreprocessItem> processingImgImgQueue;

public:

	static ImageStorage* Instance();
	static ImageStorage& getInstance(){
	static ImageStorage instance;
		return instance;
	}

	/*
	 * display queue
	 */
	void addToDisplayQueue(String fileName, Mat mMat);
	ImageStoreItem getImgFromDiplayQueue();
	int getDisplayQueueSize();

	/*
	 * processing queue
	 */
	void addToProcessingQueue(Mat rawFrame, Mat preprocessFrame, Point armCenter, vector<RotatedRect> detectedObjects, double oneMmInPx);
	ImagePreprocessItem getImgFromProcessingQueue();
	int getProcessingQueueSize();

};

#endif /* SRC_APPLICATION_LOGIC_IMAGE_IMAGESTORAGE_H_ */
