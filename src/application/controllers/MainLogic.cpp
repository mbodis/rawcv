/*
 * MainLogic.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: mbodis
 */

#include "MainLogic.h"


MainLogic::MainLogic(){
	cout << "-- -- -- CONSTRUCTOR: MainLogic -- -- --" << endl;

	// start arm communication
	this->mRoboticArmController = new RoboticArmController;
	this->mRoboticArmController->start();

	// setup ARM
	this->arm = new RoboticArm();

	// modules
	this->modules.push_back(new PickUpObject("PickUpObject"));

	// img storage
	this->mImageStorage = &ImageStorage::getInstance();

}


MainLogic::~MainLogic(){
	cout << "-- -- -- DESTRUCTOR: MainLogic -- -- --" << endl;
	this->mRoboticArmController->end();
}

/*
 * look for new input images to process
 * - if some module is active -> continue inn this module
 * - else find module to start
 *
 * - show arm position
 */
void MainLogic::process(){

	arm->resetNextMove();

	if (mImageStorage->getProcessingQueueSize() > 0){
		if (DEBUG_LOCAL) cout << "MainLogic::process new IMG " << endl;

		ImagePreprocessItem mImagePreprocessItem = (mImageStorage->getImgFromProcessingQueue());

		if (isAnyModulActive()){
			continueInActiveModule(&mImagePreprocessItem, arm->getNextMove());

		}else{
			detectModuleToStartWith(&mImagePreprocessItem, arm->getNextMove());
		}
	}

	if (arm->getNextMove()->hasChanged()){
		// move arm
		mRoboticArmController->addToStack(arm->composeNextMove());

		// show arm
		showArmPosition(this->arm->getNextMove());
	}
}

/*
 * TODO desc
 */
bool MainLogic::isAnyModulActive(){
	if (DEBUG_LOCAL) cout << "MainLogic::isAnyModulActive" << endl;
	for (int i=0; i<1;i++){
		if (modules[i]->isModulActive()){
			return true;
		}
	}
	return false;
}

/*
 * TODO desc
 */
void MainLogic::continueInActiveModule(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
	if (DEBUG_LOCAL) cout << "MainLogic::continueInActiveModule" << endl;
	for (int i=0; i<1;i++){
		if (modules[i]->isModulActive()){
			modules[i]->processNextState(mImagePreprocessItem, mRoboticArmMove);
		}
	}
}

/*
 * TODO desc
 */
void MainLogic::detectModuleToStartWith(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
	if (DEBUG_LOCAL) cout << "MainLogic::detectModuleToStartWith" << endl;
	for (int i=0; i<1;i++){
		if (DEBUG_LOCAL) cout << "i="<<i << endl;
		if (modules[i]->initialObjectDetection(mImagePreprocessItem, mRoboticArmMove)){
			modules[i]->setModulState(MODULE_STATE_START);
		}
	}
}

/*
 * TODO desc
 */
void MainLogic::showArmPosition(RoboticArmMove *mRoboticArmMove){
	if (DEBUG_LOCAL) cout << "MainLogic::showArmPosition" << endl;

	Mat viewTopMat = Mat::zeros(Size(640, 480), 16);
	// TODO draw table, top view

	Mat viewSideMat = Mat::zeros(Size(640, 480), 16);;
	// TODO draw table, side view

	// image is show in main thread
	mImageStorage->addToDisplayQueue("viewTopMat", viewTopMat);
	mImageStorage->addToDisplayQueue("viewSideMat", viewSideMat);

}
























