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
	this->modules[0] = new PickUpObject("PickUpObject");
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
	string newFramePath = getNextFile();

	if (newFramePath.size() > 1){
		if (DEBUG_LOCAL) cout << "MainLogic::process new IMG: " << newFramePath << endl;

		Mat activeFrame = imread(newFramePath, 1);
//		imshow("TEST-A", activeFrame);
//		cv::waitKey(10000);

		// TODO delete old files ?

		if (isAnyModulActive()){
			continueInActiveModule(&activeFrame);

		}else{
			detectModuleToStartWith(&activeFrame);
		}
	}

	showArmPosition();
}


string MainLogic::getNextFile(){

	if (FileSystemHelper::DirectoryExists(FOLDER_IMG_TO_PROCESS)){
		if (FileSystemHelper::getNumberFilesInFolder(FOLDER_IMG_TO_PROCESS) > 0){
			string newFramePath = FileSystemHelper::getFileInFolder(FOLDER_IMG_TO_PROCESS, 0);
			if (this->lastFramePath.compare(newFramePath) != 0){
				this->lastFramePath = newFramePath;
				return newFramePath;
			}
		}
	}else{
		cout << "output processing foleder does not exists: " << FOLDER_IMG_TO_PROCESS << endl;
	}

	return "";
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

void MainLogic::continueInActiveModule(Mat *frame){
	if (DEBUG_LOCAL) cout << "MainLogic::continueInActiveModule" << endl;
	for (int i=0; i<1;i++){
		if (this->modules[i]->isModulActive()){
			this->modules[i]->processNextState(frame);
		}
	}
}

void MainLogic::detectModuleToStartWith(Mat *frame){
	if (DEBUG_LOCAL) cout << "MainLogic::detectModuleToStartWith" << endl;
	for (int i=0; i<1;i++){
	if (DEBUG_LOCAL) cout << "i="<<i << endl;
		if (this->modules[i]->initialObjectDetection(frame)){
			this->modules[i]->setModulState(MODULE_STATE_START);
		}
	}
}

void MainLogic::showArmPosition(){
	//TODO

//	imshow();
//	imshow();
}
