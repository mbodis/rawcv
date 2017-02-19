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

	this->arm->resetNextMove();

	if (mImageStorage->getProcessingQueueSize() > 0){
		if (DEBUG_LOCAL) cout << "MainLogic::process new IMG " << endl;

		Mat activeFrame = (mImageStorage->getImgFromProcessingQueue()).mMat;
		if (DEBUG_LOCAL) cout << "process img : " << activeFrame.cols << " " << activeFrame.rows << endl;

		if (isAnyModulActive()){
			continueInActiveModule(&activeFrame, this->arm->getNextMove());

		}else{
			detectModuleToStartWith(&activeFrame, this->arm->getNextMove());
		}
	}

	if (this->arm->getNextMove()->hasChanged()){
		// move arm
		this->mRoboticArmController->addToStack(this->arm->composeNextMove());

		// show arm
		showArmPosition(this->arm->getNextMove());
	}
}

bool MainLogic::isAnyModulActive(){
	if (DEBUG_LOCAL) cout << "MainLogic::isAnyModulActive" << endl;
	for (int i=0; i<1;i++){
		if (this->modules[i]->isModulActive()){
			return true;
		}
	}
	return false;
}

void MainLogic::continueInActiveModule(Mat *frame, RoboticArmMove *mRoboticArmMove){
	if (DEBUG_LOCAL) cout << "MainLogic::continueInActiveModule" << endl;
	for (int i=0; i<1;i++){
		if (this->modules[i]->isModulActive()){
			this->modules[i]->processNextState(frame, mRoboticArmMove);
		}
	}
}

void MainLogic::detectModuleToStartWith(Mat *frame, RoboticArmMove *mRoboticArmMove){
	if (DEBUG_LOCAL) cout << "MainLogic::detectModuleToStartWith" << endl;
	for (int i=0; i<1;i++){
		if (DEBUG_LOCAL) cout << "i="<<i << endl;
		if (this->modules[i]->initialObjectDetection(frame, mRoboticArmMove)){
			this->modules[i]->setModulState(MODULE_STATE_START);
		}
	}
}


/*
 *
 */
void MainLogic::showArmPosition(RoboticArmMove *mRoboticArmMove){
	if (DEBUG_LOCAL) cout << "MainLogic::showArmPosition" << endl;

	Mat viewTopMat = Mat::zeros(Size(640, 480), 16);
	Mat viewSideMat = Mat::zeros(Size(640, 480), 16);;

	// TODO draw table, arm
	// TODO inverses kinematics - logic


	// TODO remove this sleep
	usleep(2000000);
	xx += 20;
	line(viewTopMat, Point(10,0), Point(10, xx), Scalar(255,255,255), 2);

	// image is show in main thread
	mImageStorage->addToDisplayQueue("viewTopMat", viewTopMat);
	mImageStorage->addToDisplayQueue("viewSideMat", viewSideMat);

}
























