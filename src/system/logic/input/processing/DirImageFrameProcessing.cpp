/*
 * DirImageFrameProcessing.cpp
 *
 *  Created on: Jul 14, 2016
 *      Author: mbodis
 */

#include "DirImageFrameProcessing.h"

DirImageFrameProcessing::DirImageFrameProcessing(AppConfig *mAppConfig, ImageAnalyser *mImageAnalyser, SourcePath *sourcePath):
    FrameProcessing(sourcePath, mAppConfig){
    selectedImage = 0;
    mFileSystemHelper = new FileSystemHelper();
    folderSize = mFileSystemHelper->getNumberFilesInFolder(sourcePath->path.c_str());

    selectedImagePath = mFileSystemHelper->getFileInFolder((char*)sourcePath->path.c_str(), 0);
    currentFrame = imread(selectedImagePath);
    MyLog::log(LOG_INFO, TAG, "number files in folder : " + to_string(folderSize));
    MyLog::log(LOG_INFO, TAG, "selected image: " + selectedImagePath);

    this->mImageAnalyser = mImageAnalyser;
}

void DirImageFrameProcessing::start() {

	while (!isFinished) {
        if (evaluateNextFrame){
        	Mat frame = imread(selectedImagePath, 1);
            mImageAnalyser->analyse(frame, 0);
            evaluateNextFrame = false;
        }
        mImageAnalyser->keyaboardInput(&currentFrame);
	}
    
}

/*
 * exit folder mode
 */
void DirImageFrameProcessing::quit() {
	isFinished = true;
}

/*
 * load previous image
 */
void DirImageFrameProcessing::left() {
	if (selectedImage > 0) {
		selectedImage--;

		selectedImagePath = mFileSystemHelper->getFileInFolder(
				(char*) sourcePath->path.c_str(), selectedImage);
		MyLog::log(LOG_INFO, TAG, "selected image: " + selectedImagePath);
        currentFrame = imread(selectedImagePath);		
	}
    
    evaluateNextFrame = true;
}

/*
 * load next image
 */
void DirImageFrameProcessing::right() {
	if (selectedImage != folderSize - 1) {
		selectedImage++;

		selectedImagePath = mFileSystemHelper->getFileInFolder(
				(char*) sourcePath->path.c_str(), selectedImage);
		MyLog::log(LOG_INFO, TAG, "selected image: " + selectedImagePath);
        currentFrame = imread(selectedImagePath);		
	}
    
    evaluateNextFrame = true;
}
