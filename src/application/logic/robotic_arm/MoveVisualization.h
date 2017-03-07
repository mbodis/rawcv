/*
 * MoveVisualization.h
 *
 *  Created on: Mar 6, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_ROBOTIC_ARM_MOVEVISUALIZATION_H_
#define SRC_APPLICATION_LOGIC_ROBOTIC_ARM_MOVEVISUALIZATION_H_

using namespace std;
using namespace cv;

#include "RoboticArm.h"
#include "RoboticArmMove.h"
#include "../../config/AppConfig.h"

static bool MOVE_VISUALIZATION_LOCAL_DEBUG = false;

class MoveVisualization{
private:

public:

	/*
	 * TOP VIEW
	 * show last move - skip default (not interesting)
	 */
	static void showArmPositionTopView(ImageStorage *mImageStorage, RoboticArm *arm, ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		if (!mImagePreprocessItem->hasContent()) return;
		if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "MainLogic::showArmPosition" << endl;

		/*
		 * TOP VIEW
		 * show last move - skip default (not interesting)
		 */
		int objectIdx = arm->getNextMove()->getObjectIndex();
		if (!arm->getNextMove()->isMoveDefault() && objectIdx != -1){

			Mat viewTopMat = Mat::zeros(Size(mImagePreprocessItem->preprocessFrame.cols, mImagePreprocessItem->preprocessFrame.rows), 16);

			// left corner "last move" text
			cv::putText(viewTopMat, "last move (top-view)", cvPoint(10, 20), FONT_HERSHEY_COMPLEX_SMALL, 0.6, colorWhite, 1, CV_AA);

			// arm center, point circle (white)
			circle(viewTopMat, mImagePreprocessItem->armCenter, 2, colorWhite, 2);
			circle(viewTopMat, mImagePreprocessItem->armCenter, 10, colorRed, 2);
			circle(viewTopMat, mImagePreprocessItem->armCenter, 35, colorWhite, 2);
			rectangle(viewTopMat,
				Point(mImagePreprocessItem->armCenter.x - 50, mImagePreprocessItem->armCenter.y - 50),
				Point(mImagePreprocessItem->armCenter.x + 50, viewTopMat.rows), colorWhite, 2);

			// arm text (white)
			cv::putText(viewTopMat, "arm center", Point(mImagePreprocessItem->armCenter.x - 35, mImagePreprocessItem->armCenter.y + 45),
					FONT_HERSHEY_COMPLEX_SMALL, 0.5, colorWhite, 1, CV_AA);

			// neutral direction arrow (white)
			//Point arrowEnd = Point(mImagePreprocessItem->armCenter.x, mImagePreprocessItem->armCenter.y - 100);
			//line(viewTopMat, mImagePreprocessItem->armCenter, arrowEnd, colorWhite, 2, 8);

			// object (green)
			circle(viewTopMat, mImagePreprocessItem->detectedObjects[objectIdx].center, 2, colorGreen, 2);
			line(viewTopMat, RotateBBHelper::getLtRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), RotateBBHelper::getRtRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), colorGreen, 2, 8);
			line(viewTopMat, RotateBBHelper::getLtRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), RotateBBHelper::getLbRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), colorGreen, 2, 8);
			line(viewTopMat, RotateBBHelper::getLbRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), RotateBBHelper::getRbRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), colorGreen, 2, 8);
			line(viewTopMat, RotateBBHelper::getRtRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), RotateBBHelper::getRbRotatedRect(mImagePreprocessItem->detectedObjects[objectIdx]), colorGreen, 2, 8);

			// direction for object (white)
			line(viewTopMat, mImagePreprocessItem->armCenter, mImagePreprocessItem->detectedObjects[objectIdx].center, colorWhite, 2, 8);

			// draw angle
			stringstream angleTxt;
			angleTxt << arm->getNextMove()->getAngleForServo(SERVO_IDX_BASE);
			angleTxt << "'";
			cv::putText(viewTopMat, angleTxt.str(), Point(mImagePreprocessItem->armCenter.x - 5, mImagePreprocessItem->armCenter.y - 20),
					FONT_HERSHEY_COMPLEX_SMALL, 0.7, colorRed, 1, CV_AA);

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
	static void showArmPositionSideView(ImageStorage *mImageStorage, RoboticArm *arm, ImagePreprocessItem *mImagePreprocessItem, RoboticArmMove *mRoboticArmMove){
		if (!mImagePreprocessItem->hasContent()) return;

		if (!arm->getNextMove()->isMoveDefault()){

			int ARM_PART_1_PX = ARM_PART_1_MM * mImagePreprocessItem->oneMmInPx;
			int ARM_PART_2_PX = ARM_PART_2_MM * mImagePreprocessItem->oneMmInPx;
			int ARM_PART_3_PX = ARM_PART_3_MM * mImagePreprocessItem->oneMmInPx;

			// constants for drawing
			int baseWidthPx = 100;
			int marginTopPx = 30;
			int marginBottomPx = 30;
			int marginLeftPx = 300;
			int marginRightPx = 300;

			// object px -> mm
			int objectWidthPx = max(mImagePreprocessItem->detectedObjects[mRoboticArmMove->getObjectIndex()].boundingRect().width, mImagePreprocessItem->detectedObjects[mRoboticArmMove->getObjectIndex()].boundingRect().height);
			int objectWidthMm = objectWidthPx * mImagePreprocessItem->oneMmInPx;
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "objectWidthPx: " << objectWidthPx << endl;
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "objectWidthMm: " << objectWidthMm << endl;

			// object distance px -> mm
			int objectDistanceFromArmPx = cv::norm((Point)mImagePreprocessItem->armCenter - (Point) mImagePreprocessItem->detectedObjects[mRoboticArmMove->getObjectIndex()].center );
			int objectDistanceFromArmMm = objectDistanceFromArmPx * mImagePreprocessItem->oneMmInPx;
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "objectDistanceFromArmPx: " << objectDistanceFromArmPx << endl;
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "objectDistanceFromArmMm: " << objectDistanceFromArmMm << endl;

			// angle
			int armAngle1 = mRoboticArmMove->getAngleForServo(SERVO_IDX_BOTTOM_JOINT);
			int armAngle2 = mRoboticArmMove->getAngleForServo(SERVO_IDX_MIDDLE_JOINT);
			int armAngle3 = mRoboticArmMove->getAngleForServo(SERVO_IDX_UPPER_JOINT);

			// use previous angle rotation
			armAngle2 = armAngle1 + armAngle2;
			armAngle3 = armAngle2 + armAngle3;

			// frame size (side margins, arm height, distance from object)
			Mat viewSideMat = Mat::zeros(Size(
							marginLeftPx + marginRightPx + baseWidthPx/2 + objectDistanceFromArmPx + objectWidthPx/2,
							marginTopPx + marginBottomPx + ARM_PART_1_PX + ARM_PART_2_PX + ARM_PART_3_PX + BASE_HEIGHT_MM),
							16);
			int baseJointX = marginLeftPx + baseWidthPx/2;
			int baseJointY = viewSideMat.rows - marginBottomPx - BASE_HEIGHT_MM;
			Point baseCenter = Point(baseJointX, baseJointY);

			// left corner "last move" text
			cv::putText(viewSideMat, "last move (side-view)", cvPoint(10, 20), FONT_HERSHEY_COMPLEX_SMALL, 0.6, colorWhite, 1, CV_AA);

			// background - table (just margin)
			rectangle(viewSideMat,
					Point(0, viewSideMat.rows-marginBottomPx),
					Point(viewSideMat.cols, viewSideMat.rows), colorBlue, CV_FILLED);

			// draw base
			rectangle(viewSideMat,
					Point(marginLeftPx, viewSideMat.rows - marginBottomPx - BASE_HEIGHT_MM),
					Point(marginLeftPx + baseWidthPx, viewSideMat.rows - marginBottomPx), colorWhite, 2);

			//arm part 1
			int joint1X = MathHelper::sinDeg(180+armAngle1) * ARM_PART_1_PX; // -1 because of visualization
			int joint1Y = MathHelper::sinDeg(90+armAngle1) * ARM_PART_1_PX;
			Point joint1Center = Point(baseJointX - joint1X, baseJointY - joint1Y);
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "joint1X: " << joint1X << " joint1Y: " << joint1Y << endl;
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "joint1CenterX: " << joint1Center.x << " joint1CenterY: " << joint1Center.y << endl;
			line(viewSideMat, baseCenter, joint1Center, colorWhite, 4, 8);

			//arm part 2
			int joint2X = MathHelper::sinDeg(180+armAngle2) * ARM_PART_2_PX; // -1 because of visualization
			int joint2Y = MathHelper::sinDeg(90+armAngle2) * ARM_PART_2_PX;
			Point joint2Center = Point(joint1Center.x - joint2X, joint1Center.y - joint2Y);
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "joint2X: " << joint2X << " joint2Y: " << joint2Y << endl;
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "joint2CenterX: " << joint2Center.x << " joint2CenterY: " << joint2Center.y << endl;
			line(viewSideMat, joint1Center, joint2Center, colorWhite, 4, 8);

			//arm part 3
			int joint3X = MathHelper::sinDeg(180+armAngle3) * ARM_PART_3_PX; // -1 because of visualization
			int joint3Y = MathHelper::sinDeg(90+armAngle3) * ARM_PART_3_PX;
			Point joint3Center = Point(joint2Center.x - joint3X, joint2Center.y - joint3Y);
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "joint3X: " << joint3X << " joint3Y: " << joint3Y << endl;
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "joint3CenterX: " << joint3Center.x << " joint3CenterY: " << joint3Center.y << endl;
			line(viewSideMat, joint2Center, joint3Center, colorWhite, 4, 8);

			double armObjectDistanceFromArmPx = cv::norm((Point)joint3Center - Point(marginLeftPx + baseWidthPx/2 + objectDistanceFromArmPx, viewSideMat.rows - marginBottomPx - OBJECT_DEPTH_MM) );
			double armObjectDistanceFromArmMm = armObjectDistanceFromArmPx * mImagePreprocessItem->oneMmInPx;
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "armObjectDistanceFromArmPx: " << armObjectDistanceFromArmPx << endl;
			if (MOVE_VISUALIZATION_LOCAL_DEBUG) cout << "armObjectDistanceFromArmMm: " << armObjectDistanceFromArmMm << endl;

			// draw base rotate center
			circle(viewSideMat, baseCenter, 7, colorRed, 2);

			// draw joint 1
			circle(viewSideMat, joint1Center, 7, colorRed, 2);
			stringstream angleTxt1;
			angleTxt1 << arm->getNextMove()->getAngleForServo(SERVO_IDX_BOTTOM_JOINT);
			angleTxt1 << "'";
			cv::putText(viewSideMat, angleTxt1.str(), Point(baseCenter.x - 5, baseCenter.y - 20), FONT_HERSHEY_COMPLEX_SMALL, 0.7, colorRed, 1, CV_AA);

			// draw joint 1
			circle(viewSideMat, joint2Center, 7, colorRed, 2);
			stringstream angleTxt2;
			angleTxt2 << arm->getNextMove()->getAngleForServo(SERVO_IDX_MIDDLE_JOINT);
			angleTxt2 << "'";
			cv::putText(viewSideMat, angleTxt2.str(), Point(joint1Center.x - 5, joint1Center.y - 20), FONT_HERSHEY_COMPLEX_SMALL, 0.7, colorRed, 1, CV_AA);

			// draw joint 3
			stringstream angleTxt3;
			angleTxt3 << arm->getNextMove()->getAngleForServo(SERVO_IDX_UPPER_JOINT);
			angleTxt3 << "'";
			cv::putText(viewSideMat, angleTxt3.str(), Point(joint2Center.x - 5, joint2Center.y - 20), FONT_HERSHEY_COMPLEX_SMALL, 0.7, colorRed, 1, CV_AA);

			// draw object
			rectangle(viewSideMat,
					Point(marginLeftPx + baseWidthPx/2 + objectDistanceFromArmPx - objectWidthPx/2, viewSideMat.rows - marginBottomPx - OBJECT_DEPTH_MM),
					Point(marginLeftPx + baseWidthPx/2 + objectDistanceFromArmPx + objectWidthPx/2, viewSideMat.rows - marginBottomPx),
					colorGreen, 2);
			circle(viewSideMat,
					Point(marginLeftPx + baseWidthPx/2 + objectDistanceFromArmPx, viewSideMat.rows - marginBottomPx - OBJECT_DEPTH_MM),
					2, colorGreen, 2);


			mImageStorage->addToDisplayQueue("viewSideMat", viewSideMat);
		}
	}
};



#endif /* SRC_APPLICATION_LOGIC_ROBOTIC_ARM_MOVEVISUALIZATION_H_ */
