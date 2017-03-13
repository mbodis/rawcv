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
	this->mRoboticArmController->executeCommand(arm->composeNextMove());

	// modules
	this->modules.clear();
	this->modules.push_back(new PickUpObject("PickUpObject", mRoboticArmController));

	// img storage
	this->mImageStorage = &InputStorage::getInstance();
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
	ImagePreprocessItem mImagePreprocessItem;
	bool showArmView = false;

	executeInputFromKeyboard();

	// IMAGE TRIGGER: new content from pro-processing
	if (mImageStorage->getProcessingQueueSize() > 0){
		if (DEBUG_LOCAL) cout << "MainLogic::IMAGE TRIGGER----------------" << endl;

		showArmView = true;
		mImagePreprocessItem = (mImageStorage->getImgFromProcessingQueue());

		// continue / start module
		if (isAnyModulActive()){
			continueInActiveModuleFrameTrigger(&mImagePreprocessItem, arm);
		}else{
			detectModuleToStartWith(&mImagePreprocessItem, arm);
		}

	// TIME TRIGGER: change move by time
	}else{
		if (DEBUG_LOCAL) cout << "MainLogic::TIME TRIGGER----------------" << endl;
		if (isAnyModulActive()){
			continueInActiveModuleTimeTrigger(arm);
		}
	}

	// execute possible changes
	if (arm->getNextMove()->hasChanged()){
		if (DEBUG_LOCAL) cout << "move has changed" << endl;

		if (showArmView){
			// show arm
			MoveVisualization::showArmPositionTopView(mImageStorage, arm, &mImagePreprocessItem, this->arm->getNextMove());
			MoveVisualization::showArmPositionSideView(mImageStorage, arm, &mImagePreprocessItem, this->arm->getNextMove());
		}
	}

	usleep(LOOP_SLEEP_TIME);
}

void MainLogic::executeInputFromKeyboard(){
	if (mImageStorage->getKeyPressQueueSize() > 0 ){
		char key = mImageStorage->getKeyFromKeyPressQueue();
		cout << "new key input: " << (int)key << endl;
		switch((int)key){
		case 114: // 'r'
			cout << "reset active modules " << endl;
			for (int i=0; i<1;i++){
				if (modules[i]->isModulActive()){
					modules[i]->resetContent();
				}
			}
			break;
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
void MainLogic::continueInActiveModuleFrameTrigger(ImagePreprocessItem *mImagePreprocessItem, RoboticArm *mRoboticArm){
	if (DEBUG_LOCAL) cout << "MainLogic::continueInActiveModule" << endl;
	for (int i=0; i<1;i++){
		if (modules[i]->isModulActive()){
			modules[i]->processNextStateFrameTrigger(mImagePreprocessItem, mRoboticArm);
		}
	}
}

/*
 * loop through all modules, continue in active module
 */
void MainLogic::continueInActiveModuleTimeTrigger(RoboticArm *mRoboticArm){
	if (DEBUG_LOCAL) cout << "MainLogic::continueInActiveModule" << endl;
	for (int i=0; i<1;i++){
		if (modules[i]->isModulActive()){
			modules[i]->processNextStateTimeTrigger(mRoboticArm);
		}
	}
}

/*
 * loop through all modules, try to start some module
 */
void MainLogic::detectModuleToStartWith(ImagePreprocessItem *mImagePreprocessItem, RoboticArm *mRoboticArm){
	if (DEBUG_LOCAL) cout << "MainLogic::detectModuleToStartWith" << endl;
	for (int i=0; i<1;i++){
		if (DEBUG_LOCAL) cout << "i="<<i << endl;
		if (modules[i]->initialObjectDetection(mImagePreprocessItem, mRoboticArm)){
			if (DEBUG_LOCAL) cout << "starting module "<< modules[i]->getName() << endl;
		}
	}
}


























