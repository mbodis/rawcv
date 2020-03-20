/*
 * ImageStorage.cpp
 *
 *  Created on: Feb 8, 2017
 *      Author: mbodis
 */
#include "InputStorage.h"

// Global static pointer used to ensure a single instance of the class.
InputStorage* InputStorage::m_pInstance = NULL;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/

InputStorage* InputStorage::Instance()
{
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new InputStorage;

   return m_pInstance;
}

InputStorage::InputStorage(){

}

/*
 * add image to display queue
 * save image from background thread to be show on main thread
 */
void InputStorage::addToDisplayQueue(String fileName, Mat mMat){
	displayImgQueue.push(ImageStoreItem(fileName, mMat));
}

/*
 * pop first image from queue
 * load image from background and show it on main thread
 */
ImageStoreItem InputStorage::getImgFromDiplayQueue(){
	ImageStoreItem stackItem = displayImgQueue.front();
	displayImgQueue.pop();

	return stackItem;
}

int InputStorage::getDisplayQueueSize(){
	return displayImgQueue.size();
}

/*
 * add image to processing queue
 * save image from preprocessing thread into queue -> main logic will continue with this image
 */
void InputStorage::addToProcessingQueue(Mat rawFrame, Mat preprocessFrame, Point armCenter, vector<RotatedRect> detectedObjects, double oneMmInPx){
	processingImgImgQueue.push(ImagePreprocessItem(rawFrame, preprocessFrame, armCenter, detectedObjects, oneMmInPx));
}

/*
 * pop first image from queue
 * load image from processing queue and continue some logic on it
 */
ImagePreprocessItem InputStorage::getImgFromProcessingQueue(){
	ImagePreprocessItem preprocessItem = processingImgImgQueue.front();
	processingImgImgQueue.pop();

	return preprocessItem;
}

int InputStorage::getProcessingQueueSize(){
	return processingImgImgQueue.size();
}


/*
 * add key press to processing queue
 * save key press from main thread into queue -> send it to backend
 */
void InputStorage::addToKeyPressQueue(char key){
	keyPressQueue.push(key);
}

/*
 * pop first key press from queue
 * load key press from processing queue and continue some logic on it
 */
char InputStorage::getKeyFromKeyPressQueue(){
	char newKeypress = keyPressQueue.front();
	keyPressQueue.pop();

	return newKeypress;
}

int InputStorage::getKeyPressQueueSize(){
	return keyPressQueue.size();
}


