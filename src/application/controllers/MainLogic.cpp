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
	// go to default position
	this->arm->getNextMove()->setDefaultPosition();
	this->mRoboticArmController->addToStack(arm->composeNextMove());

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
 * - if some module is active -> continue in this module
 * - else find module to start
 *
 * - show arm position
 */
void MainLogic::process(){

	arm->resetNextMove();

	// new content from preprocessing
	if (mImageStorage->getProcessingQueueSize() > 0){
		cout << "MainLogic::process new IMG ----------------" << endl;

		ImagePreprocessItem mImagePreprocessItem = (mImageStorage->getImgFromProcessingQueue());

		// continue / start module
		if (isAnyModulActive()){
			continueInActiveModule(&mImagePreprocessItem, arm->getNextMove());
		}else{
			detectModuleToStartWith(&mImagePreprocessItem, arm->getNextMove());
		}

		// execute possible changes
		if (arm->getNextMove()->hasChanged()){
			// move arm
			mRoboticArmController->addToStack(arm->composeNextMove());

			// show arm
			MoveVisualization::showArmPositionTopView(mImageStorage, arm, &mImagePreprocessItem, this->arm->getNextMove());
			MoveVisualization::showArmPositionSideView(mImageStorage, arm, &mImagePreprocessItem, this->arm->getNextMove());
		}
	}
}

/*
 * loop through all modules, find any module is active
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
 * loop through all modules, continue in active module
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
 * loop through all modules, try to start some module
 */
void MainLogic::detectModuleToStartWith(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
	if (DEBUG_LOCAL) cout << "MainLogic::detectModuleToStartWith" << endl;
	for (int i=0; i<1;i++){
		if (DEBUG_LOCAL) cout << "i="<<i << endl;
		if (modules[i]->initialObjectDetection(mImagePreprocessItem, mRoboticArmMove)){
			if (DEBUG_LOCAL) cout << "starting module "<< modules[i]->getName() << endl;
		}
	}
}


























