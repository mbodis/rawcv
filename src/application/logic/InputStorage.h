/*
 * ImageStorage.h
 *
 *  Created on: Feb 6, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_INPUTSTORAGE_H_
#define SRC_APPLICATION_LOGIC_INPUTSTORAGE_H_


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "image/ImageDisplayItem.h"
#include "image/ImagePreprocessItem.h"
using namespace cv;

#include "../../system/config/Constants.h"

#include <queue>          // std::stack
#include <list>          // std::stack
#include <map>          // std::stack
#include <iostream>
#include <sys/time.h>

using namespace std;

static string IMAGE_VIEW_TOP = "view_top";
static string IMAGE_VIEW_SIDE = "view_side";

/*
 *
 */
class InputStorage{

private:
	InputStorage();	// Private so that it can  not be called
	InputStorage(InputStorage const&){};	// copy constructor is private
	InputStorage& operator=(InputStorage const&){};  // assignment operator is private
	static InputStorage* m_pInstance;

	std::queue<ImageStoreItem> displayImgQueue;
	std::queue<ImagePreprocessItem> processingImgImgQueue;
	std::queue<char> keyPressQueue;

public:

	static InputStorage* Instance();
	static InputStorage& getInstance(){
	static InputStorage instance;
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

	/*
	 * keyboard queue
	 */
	void addToKeyPressQueue(char key);
	char getKeyFromKeyPressQueue();
	int getKeyPressQueueSize();

};

#endif /* SRC_APPLICATION_LOGIC_INPUTSTORAGE_H_ */
