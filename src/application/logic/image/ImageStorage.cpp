/*
 * ImageStorage.cpp
 *
 *  Created on: Feb 8, 2017
 *      Author: mbodis
 */
#include "ImageStorage.h"

// Global static pointer used to ensure a single instance of the class.
ImageStorage* ImageStorage::m_pInstance = NULL;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/

ImageStorage* ImageStorage::Instance()
{
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new ImageStorage;

   return m_pInstance;
}

ImageStorage::ImageStorage(){

}

/*
 * add image to display queue
 * save image from background thread to be show on main thread
 */
void ImageStorage::addToDisplayQueue(String fileName, Mat mMat){
	displayImgQueue.push(ImageStoreItem(fileName, mMat));
}

/*
 * pop first image from queue
 * load image from background and show it on main thread
 */
ImageStoreItem ImageStorage::getImgFromDiplayQueue(){
	ImageStoreItem stackItem = displayImgQueue.front();
	displayImgQueue.pop();

	return stackItem;
}

int ImageStorage::getDisplayQueueSize(){
	return displayImgQueue.size();
}

/*
 * add image to processing queue
 * save image from preprocessing thread into queue -> main logic will continue with this image
 */
void ImageStorage::addToProcessingQueue(String fileName, Mat mMat){
	processingImgImgQueue.push(ImageStoreItem(fileName, mMat));
}

/*
 * pop first image from queue
 * load image from processing queue and continue some logic on it
 */
ImageStoreItem ImageStorage::getImgFromProcessingQueue(){
	ImageStoreItem stackItem = processingImgImgQueue.front();
	processingImgImgQueue.pop();

	return stackItem;
}

int ImageStorage::getProcessingQueueSize(){
	return processingImgImgQueue.size();
}
