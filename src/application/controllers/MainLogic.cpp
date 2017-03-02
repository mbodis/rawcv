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

		if (isAnyModulActive()){
			continueInActiveModule(&mImagePreprocessItem, arm->getNextMove());

		}else{
			detectModuleToStartWith(&mImagePreprocessItem, arm->getNextMove());
		}

		if (arm->getNextMove()->hasChanged()){
			// move arm
			mRoboticArmController->addToStack(arm->composeNextMove());

			// show arm
			showArmPositionTopView(&mImagePreprocessItem, this->arm->getNextMove());
			showArmPositionSideView(&mImagePreprocessItem, this->arm->getNextMove());
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

/*
 * TOP VIEW
 * show last move - skip default (not interesting)
 */
void MainLogic::showArmPositionTopView(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
	if (DEBUG_LOCAL) cout << "MainLogic::showArmPosition" << endl;

	/*
	 * TOP VIEW
	 * show last move - skip default (not interesting)
	 */
	int objectIdx = arm->getNextMove()->getObjectIndex();
	if (!arm->getNextMove()->isMoveDefault() && objectIdx != -1){

		Mat viewTopMat = Mat::zeros(Size(mImagePreprocessItem->preprocessFrame.cols, mImagePreprocessItem->preprocessFrame.rows), 16);

		// left corner "last move" text
		cv::putText(viewTopMat, "last move (top-view)", cvPoint(10, 20), FONT_HERSHEY_COMPLEX_SMALL, 0.7, colorWhite, 1, CV_AA);

		// arm center, point circle (white)
		circle(viewTopMat, mImagePreprocessItem->armCenter, 2, colorWhite, 2);
		circle(viewTopMat, mImagePreprocessItem->armCenter, 20, colorWhite, 2);

		// arm text (white)
		cv::putText(viewTopMat, "arm center", Point(mImagePreprocessItem->armCenter.x - 35, mImagePreprocessItem->armCenter.y + 45),
				FONT_HERSHEY_COMPLEX_SMALL, 0.5, colorWhite, 1, CV_AA);

		// neutral direction arrow (white)
		Point arrowEnd = Point(mImagePreprocessItem->armCenter.x, mImagePreprocessItem->armCenter.y - 100);
		line(viewTopMat, mImagePreprocessItem->armCenter, arrowEnd, colorWhite, 2, 8);

		// object (green)
		circle(viewTopMat, mImagePreprocessItem->detectedObjects[objectIdx].center, 2, colorGreen, 2);
		line(viewTopMat, RotateBBHelper::getLtRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), RotateBBHelper::getRtRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), colorGreen, 2, 8);
		line(viewTopMat, RotateBBHelper::getLtRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), RotateBBHelper::getLbRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), colorGreen, 2, 8);
		line(viewTopMat, RotateBBHelper::getLbRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), RotateBBHelper::getRbRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), colorGreen, 2, 8);
		line(viewTopMat, RotateBBHelper::getRtRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), RotateBBHelper::getRbRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), colorGreen, 2, 8);

		// direction for object (green)
		line(viewTopMat, mImagePreprocessItem->armCenter, mImagePreprocessItem->detectedObjects[objectIdx].center, colorGreen, 2, 8);

		// draw angle
		Point objectCenter = mImagePreprocessItem->detectedObjects[objectIdx].center;
		stringstream angleTxt;
		angleTxt << arm->getNextMove()->getAngleForServo(SERVO_IDX_BASE);
		angleTxt << "'";
		cv::putText(viewTopMat, angleTxt.str(), Point(objectCenter.x - 5, objectCenter.y + 20), FONT_HERSHEY_COMPLEX_SMALL, 0.7, colorGreen, 1, CV_AA);

		// image is show in main thread
		mImageStorage->addToDisplayQueue("viewTopMat", viewTopMat);
	}
}

/*
 * SIDE VIEW
 * show last move - skip default (not interesting)
 *
 * note: input values are in millimeters -> scaled with some ratio for better visualization
 */
void MainLogic::showArmPositionSideView(ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
	if (!arm->getNextMove()->isMoveDefault()){

		// constants for drawing
		int baseWidthMm = 100;
		int marginTopPx = 30;
		int marginBottomPx = 30;
		int marginLeftPx = 300;
		int marginRightPx = 300;

		// object px -> mm
		int objectWidthPx = max(mImagePreprocessItem->detectedObjects[mRoboticArmMove->getObjectIndex()].boundingRect().width, mImagePreprocessItem->detectedObjects[mRoboticArmMove->getObjectIndex()].boundingRect().height);
		int objectWidthMm = objectWidthPx * mImagePreprocessItem->oneMmInPx;
		if (DEBUG_LOCAL) cout << "objectWidthPx: " << objectWidthPx << endl;
		if (DEBUG_LOCAL) cout << "objectWidthMm: " << objectWidthMm << endl;

		// object distance px -> mm
		int objectDistanceFromArmPx = cv::norm((Point)mImagePreprocessItem->armCenter - (Point) mImagePreprocessItem->detectedObjects[mRoboticArmMove->getObjectIndex()].center );
		int objectDistanceFromArmMm = objectDistanceFromArmPx * mImagePreprocessItem->oneMmInPx;
		if (DEBUG_LOCAL) cout << "objectDistanceFromArmPx: " << objectDistanceFromArmPx << endl;
		if (DEBUG_LOCAL) cout << "objectDistanceFromArmMm: " << objectDistanceFromArmMm << endl;

		// angle
		int armAngle1 = mRoboticArmMove->getAngleForServo(SERVO_IDX_BOTTOM_JOINT);
		int armAngle2 = mRoboticArmMove->getAngleForServo(SERVO_IDX_MIDDLE_JOINT);
		int armAngle3 = mRoboticArmMove->getAngleForServo(SERVO_IDX_UPPER_JOINT);

		// use previous angle rotation
		armAngle2 = armAngle1 + armAngle2;
		armAngle3 = armAngle2 + armAngle3;

		// frame size (side margins, arm height, distance from object)
		Mat viewSideMat = Mat::zeros(Size(
						marginLeftPx + marginRightPx + baseWidthMm/2 + objectDistanceFromArmMm + objectWidthMm/2,
						marginTopPx + marginBottomPx + ARM_PART_1_MM + ARM_PART_2_MM + ARM_PART_3_MM + BASE_HEIGHT_MM),
						16);
		int baseJointX = marginLeftPx + baseWidthMm/2;
		int baseJointY = viewSideMat.rows - marginBottomPx - BASE_HEIGHT_MM;
		Point baseCenter = Point(baseJointX, baseJointY);

		// left corner "last move" text
		cv::putText(viewSideMat, "last move (side-view)", cvPoint(10, 20), FONT_HERSHEY_COMPLEX_SMALL, 0.7, colorWhite, 1, CV_AA);

		// background - table (just margin)
		rectangle(viewSideMat,
				Point(0, viewSideMat.rows-marginBottomPx),
				Point(viewSideMat.cols, viewSideMat.rows), colorBlue, CV_FILLED);

		// draw base
		rectangle(viewSideMat,
				Point(marginLeftPx, viewSideMat.rows - marginBottomPx - BASE_HEIGHT_MM),
				Point(marginLeftPx + baseWidthMm, viewSideMat.rows - marginBottomPx), colorWhite, 2);

		//arm part 1
		int joint1X = MathHelper::sinDeg(180+armAngle1) * ARM_PART_1_MM;
		int joint1Y = MathHelper::sinDeg(90+armAngle1) * ARM_PART_1_MM;
		Point joint1Center = Point(baseJointX - joint1X, baseJointY - joint1Y);
		line(viewSideMat, baseCenter, joint1Center, colorWhite, 4, 8);

		//arm part 2
		int joint2X = MathHelper::sinDeg(180+armAngle2) * ARM_PART_2_MM;
		int joint2Y = MathHelper::sinDeg(90+armAngle2) * ARM_PART_2_MM;
		Point joint2Center = Point(joint1Center.x - joint2X, joint1Center.y - joint2Y);
		line(viewSideMat, joint1Center, joint2Center, colorWhite, 4, 8);

		//arm part 3
		int joint3X = MathHelper::sinDeg(180+armAngle3) * ARM_PART_3_MM;
		int joint3Y = MathHelper::sinDeg(90+armAngle3) * ARM_PART_3_MM;
		Point joint3Center = Point(joint2Center.x - joint3X, joint2Center.y - joint3Y);
		line(viewSideMat, joint2Center, joint3Center, colorWhite, 4, 8);

		// draw base rotate center
		circle(viewSideMat, baseCenter, 7, colorRed, 2);

		// draw 3 joints - geometry needed
		circle(viewSideMat, joint1Center, 7, colorRed, 2);
		circle(viewSideMat, joint2Center, 7, colorRed, 2);
		circle(viewSideMat, joint3Center, 7, colorRed, 2);

		// draw object
		rectangle(viewSideMat,
				Point(marginLeftPx + baseWidthMm/2 + objectDistanceFromArmMm - objectWidthMm/2, viewSideMat.rows - marginBottomPx - OBJECT_DEPTH_MM),
				Point(marginLeftPx + baseWidthMm/2 + objectDistanceFromArmMm + objectWidthMm/2, viewSideMat.rows - marginBottomPx),
				colorGreen, 2);
		circle(viewSideMat,
				Point(marginLeftPx + baseWidthMm/2 + objectDistanceFromArmMm, viewSideMat.rows - marginBottomPx - OBJECT_DEPTH_MM),
				2, colorGreen, 2);


		mImageStorage->addToDisplayQueue("viewSideMat", viewSideMat);
	}
}
























