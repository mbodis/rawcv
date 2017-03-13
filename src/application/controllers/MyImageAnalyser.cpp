/*
 * MyImageAnalyser.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#include "../controllers/MyImageAnalyser.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

/*
 * Main logic - repeating loop with new input from camera
 *
 * 0) save input frame
 * 1) has something move for 1 sec
 * 2) crop table
 * 3) recalculate px -> mm
 * 4) detect arm
 * 5) is there object ?
 * 6) save image for main logic processing
 *
 *
 */
void MyImageAnalyser::executeCustomLogic(Mat frame, long videoTime){

	// 0) save input frame
	saveInputFrame(frame, videoTime);

	// 1) has something move
	detectMovement();

	// 2) crop / stretch table
	cropAndStretchTableBg();

	// 3) recalculate px -> mm (inside point 2)

	// 4) detect arm center
	detectArm();

	// 5) find object on the table
	detectObjects();

	// 6) save image for main logic processing
	saveImageForProcessing();

	drawOutputInfo(outputColorStretchFrame);

	// show output
	showOutput();
	showImageFromBackground();

	// 1) has something move - reset
	detectMovementResetStates();

	propagatePresedKeyToBackend();
}

/**
 * detect if image has moved
 * - comparing last two images
 * - CONSTANT threshold for movement in percentage
 * - time to leave this state CONSTANT in milis
 * return set local variable true/false [isFrameMoving]
 */
void MyImageAnalyser::detectMovement(){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::detectMovement" << endl;

	if (outputColorFrame.data == 0) return;

	if (mDebugFrames->c->inputMode == INPUT_MODE_IMG_FOLDER){
		setFrameIsMoving(false);
		return;
	}

	cvtColor(outputColorFrame, detectMovementGrayFrame, CV_BGR2GRAY);
	equalizeHist(detectMovementGrayFrame, detectMovementGrayFrame);
	Mat diffGray, actualDiffBw;

	if (detectMovementGrayFrame.dims != 0 && detectMovementLastGrayFrame.dims != 0) {

		absdiff(detectMovementLastGrayFrame, detectMovementGrayFrame, diffGray);
		actualDiffBw = diffGray > THRESHOLD_MOVEMENT_BW_VALUE;

		whitePercentage = (double)countNonZero(actualDiffBw) * 100
				/ (detectMovementGrayFrame.cols * detectMovementGrayFrame.rows);
		if (DEBUG_LOCAL) cout << "display moves percentage: " << whitePercentage << endl;
		if (DEBUG_LOCAL) cout << "video_time: " << video_time << " " << (video_time - last_move_time) << endl;
		if (DEBUG_LOCAL) cout << "last_move_tim: " << last_move_time << endl;
		if (DEBUG_LOCAL) cout << "----------------------" << endl;

		if (whitePercentage > THRESHOLD_MOVE_PERCENTAGE) {
			setFrameIsMoving(true);
			last_move_time = video_time;
		}else{
			if (video_time - last_move_time < THRESHOLD_MOVE_MILIS){
				setFrameIsMoving(true);
			}else{
				setFrameIsMoving(false);
			}
		}
	}else{
		setFrameIsMoving(true);
	}

}

void MyImageAnalyser::drawOutputInfo(Mat frame){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::drawOutputInfo" << endl;

	// draw moving info
	string movingTxt = isFrameMoving ? "moving" : "not moving";
	Scalar color = isFrameMoving ? Scalar(0,0,255) : Scalar(0,255,0);

	cv::putText(frame, movingTxt, cvPoint(15, 55),
					FONT_HERSHEY_COMPLEX_SMALL, 0.8, color, 1, CV_AA);

	stringstream moveStr;
		moveStr << whitePercentage << " %";
	cv::putText(frame, moveStr.str(), cvPoint(15, 75),
						FONT_HERSHEY_COMPLEX_SMALL, 0.8, colorWhite, 1, CV_AA);

	// draw arm  + center
	if (armDetected){
		circle(frame, armCenter, 4, colorRed, -1, 8, 0 );
		rectangle(frame, armBb, colorRed, 2);
	}

	//draw objects
	for (int i = 0; i < detectedObjects.size(); i++) {
		line(frame, RotateBBHelper::getLtRotatedRect(detectedObjects[i]), RotateBBHelper::getRtRotatedRect(detectedObjects[i]), colorGreen, 3, 8);
		line(frame, RotateBBHelper::getLtRotatedRect(detectedObjects[i]), RotateBBHelper::getLbRotatedRect(detectedObjects[i]), colorGreen, 3, 8);
		line(frame, RotateBBHelper::getLbRotatedRect(detectedObjects[i]), RotateBBHelper::getRbRotatedRect(detectedObjects[i]), colorGreen, 3, 8);
		line(frame, RotateBBHelper::getRtRotatedRect(detectedObjects[i]), RotateBBHelper::getRbRotatedRect(detectedObjects[i]), colorGreen, 3, 8);
	}

}

void MyImageAnalyser::setFrameIsMoving(bool newValue){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::setFrameIsMoving" << endl;

	if (this->isFrameMoving && newValue == false){
		this->interestingFrame = true;
	}

	this->isFrameMoving = newValue;
}

/*
 * input: color image
 *  - find table
 *  - crop / stretch table
 *
 * output: binary image of table [binaryTableCrop]
 *
 */
void MyImageAnalyser::cropAndStretchTableBg(){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::cropAndStretchTableBg" << endl;

	if (originalColorFrame.data == 0) return;


	Mat resizedFrame, grayFrame, diffGray, actualDiffBw, thBinaryFrame,
		morphBinaryFrame, stretchTable;

	resize(originalColorFrame, resizedFrame, Size(originalColorFrame.cols/2, originalColorFrame.rows/2));

	// gray
	cvtColor(resizedFrame, grayFrame, CV_BGR2GRAY);
	equalizeHist(grayFrame, grayFrame);

	// blur
	blur(grayFrame, thBinaryFrame, Size( 3, 3 ) );
	thBinaryFrame = thBinaryFrame > sliderThresholdBg;

	// draw 1 px on borders
	line(thBinaryFrame, Point(0,0), Point(thBinaryFrame.cols, 0), colorBlack, 1);
	line(thBinaryFrame, Point(0,0), Point(0, thBinaryFrame.rows), colorBlack, 1);
	line(thBinaryFrame, Point(thBinaryFrame.cols-1, 0), Point(thBinaryFrame.cols-1, thBinaryFrame.rows-1), colorBlack, 1);
	line(thBinaryFrame, Point(0, thBinaryFrame.rows-1), Point(thBinaryFrame.cols-1, thBinaryFrame.rows-1), colorBlack, 1);

	//morph
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
	morphologyEx(thBinaryFrame, morphBinaryFrame, MORPH_OPEN, element);

	if (!tableDetected){

		// find countours
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		cv::findContours(morphBinaryFrame.clone(), contours, hierarchy, CV_RETR_EXTERNAL,
				CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		// find table
		bool findTable = false;
		for (int i = 0; i < contours.size(); i++) {
			Rect bbox = boundingRect(contours[i]);
			int bbSize = bbox.height * bbox.width;
			int frameSize = morphBinaryFrame.cols * morphBinaryFrame.rows;
			double perc = bbSize * 100 / frameSize;
			if (DEBUG_LOCAL) cout << "contour[" << i << "] =  " << perc <<endl;
			if (perc > 60 ){
				tableBb = bbox;
				findTable = true;
			}else{
				 rectangle(morphBinaryFrame, Point(bbox.x, bbox.y), Point(bbox.x+bbox.width, bbox.y+bbox.height), colorBlack, CV_FILLED);
			}
		}
		if (DEBUG_LOCAL) showImg(morphBinaryFrame, "morphBinaryFrame");

		if (!findTable) return;

		// corner detect // Parameters for Shi-Tomasi algorithm
		vector<Point2f> corners;
		double qualityLevel = 0.21;
		double minDistance = 50;
		int blockSize = 5;//3;
		int maxCorners = 15;//4;
		// Apply corner detection
		goodFeaturesToTrack( morphBinaryFrame, corners,
						   maxCorners, qualityLevel,
						   minDistance, Mat(),
						   blockSize);
		// Draw corners detected
		lt = Point(999, 999); lb = Point(999, -1); rt = Point(-1, 999); rb = Point(-1, -1);


		for( int i = 0; i < corners.size(); i++ ){
			if (lt.x >= corners[i].x && lt.y >= corners[i].y) {
				lt = corners[i];
			}
			if (lb.x >= corners[i].x && lb.y <= corners[i].y) {
				lb = corners[i];
			}
			if (rt.x <= corners[i].x && rt.y >= corners[i].y) {
				rt = corners[i];
			}
			if (rb.x <= corners[i].x && rb.y <= corners[i].y) {
				rb = corners[i];
			}
		}

		if (DEBUG_LOCAL) cout << "lt.x= " << lt.x << " lt.y= " << lt.y << endl;
		if (DEBUG_LOCAL) cout << "lb.x= " << lb.x << " lb.y= " << lb.y << endl;
		if (DEBUG_LOCAL) cout << "rt.x= " << rt.x << " rt.y= " << rt.y << endl;
		if (DEBUG_LOCAL) cout << "rb.x= " << rb.x << " rb.y= " << rb.y << endl;
		if (DEBUG_LOCAL) cout << "-----------------------------------" << endl;

		// corner validation -> points should be in 10% position in frame corners
		double d_ltx = lt.x / morphBinaryFrame.cols * 100;
		double d_lty = lt.y / morphBinaryFrame.rows * 100;
		double d_lbx = lb.x / morphBinaryFrame.cols * 100;
		double d_lby = 100 - lb.y / morphBinaryFrame.rows * 100;
		double d_rtx = 100 - rt.x / morphBinaryFrame.cols * 100;
		double d_rty = rt.y / morphBinaryFrame.rows * 100;
		double d_rbx = 100 - rb.x / morphBinaryFrame.cols * 100;
		double d_rby = 100 - rb.y / morphBinaryFrame.rows * 100;

		if (d_ltx > TABLE_CORNER_OFFSER_PERCENTAGE || d_lty > TABLE_CORNER_OFFSER_PERCENTAGE
				|| d_lbx > TABLE_CORNER_OFFSER_PERCENTAGE || d_lby > TABLE_CORNER_OFFSER_PERCENTAGE
				|| d_rtx > TABLE_CORNER_OFFSER_PERCENTAGE || d_rty > TABLE_CORNER_OFFSER_PERCENTAGE
				|| d_rbx > TABLE_CORNER_OFFSER_PERCENTAGE || d_rby > TABLE_CORNER_OFFSER_PERCENTAGE	){
			return;
		}

		if (DEBUG_LOCAL) cout << "d_ltx= " << d_ltx << " d_lty= " << d_lty << endl;
		if (DEBUG_LOCAL) cout << "d_lbx= " << d_lbx << " d_lby= " << d_lby << endl;
		if (DEBUG_LOCAL) cout << "d_rtx= " << d_rtx << " d_rty= " << d_rty << endl;
		if (DEBUG_LOCAL) cout << "d_rbx= " << d_rbx << " d_rby= " << d_rby << endl;
		if (DEBUG_LOCAL) cout << "-----------------------------------" << endl;

		// skip if corners where detected wrong
		if (abs(lt.x - lb.x) > 30 || abs (lt.y - rt.y) > 30
				|| abs(rt.x - rb.x) > 30 || abs (lb.y - rb.y) > 30){
			return;
		}

		// debug - draw corners
		if (DEBUG_LOCAL){
			Mat cornerFrame = resizedFrame.clone();
			for( int i = 0; i < corners.size(); i++ ){
				circle( cornerFrame, corners[i], 4, Scalar(255,0,0), -1, 8, 0 );
			}
			showImg(cornerFrame, "cornerFrame");
		}

		tableDetected = true;

		recalculatePxToMm(lt, rt, resizedFrame.cols, resizedFrame.rows);
	}

	// if table was found
	if (tableDetected){

		Point2f ptsTableCorners[4];
		ptsTableCorners[0] = lt;
		ptsTableCorners[1] = rt;
		ptsTableCorners[2] = lb;
		ptsTableCorners[3] = rb;

		Point2f ptsCroppedTable[4];
		ptsCroppedTable[0] = Point2f(0,0);
		ptsCroppedTable[1] = Point2f(tableBb.width,0);
		ptsCroppedTable[2] = Point2f(0,tableBb.height);
		ptsCroppedTable[3] = Point2f(tableBb.width,tableBb.height);

		Mat perspectiveMat = getPerspectiveTransform(ptsTableCorners, ptsCroppedTable);
		warpPerspective(morphBinaryFrame, stretchTable, perspectiveMat, Size(tableBb.width, tableBb.height));
		warpPerspective(outputColorFrame, outputColorStretchFrame, perspectiveMat, Size(tableBb.width, tableBb.height));
		if (DEBUG_LOCAL) showImg(stretchTable, "stretchTable");

		binaryTableCrop = stretchTable;
	}
}

/*
 * input - binary img with table
 * output: set local variable - px for one mm
 */
void MyImageAnalyser::recalculatePxToMm(Point p1, Point p2, int frameW, int frameH){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::recalculatePxToMm" << endl;

	if (tableDetected){
		if (!mmToPxCalculated){
			Point difference = p1-p2;
			double distance = sqrt( difference.ddot(difference));
			oneMmInPx = (double)TABLE_WIDTH_MM / distance;
			oneMmInPxframeWidth = frameW;
			oneMmInPxframeHeight = frameH;
			mmToPxCalculated = true;
			if (DEBUG_LOCAL) cout << "one MM to PX = " << TABLE_WIDTH_MM << "/" << distance << " = " <<  oneMmInPx << endl;
		}
	}
}

/*
 * input: binary image of table
 * output: center point(x, y) of robotic arm
 */
void MyImageAnalyser::detectArm(){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::detectArm" << endl;

	if (tableDetected){

		bitwise_not(binaryTableCrop, binaryWithoutArm);

		if (armDetected){
			// remove arm from image
			rectangle(binaryWithoutArm, Point(armBb.x, armBb.y), Point(armBb.x+armBb.width, armBb.y+armBb.height), colorBlack, CV_FILLED);

		}else{
			// find countours
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			cv::findContours(binaryWithoutArm.clone(), contours, hierarchy, CV_RETR_EXTERNAL,
					CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

			// find arm
			for (int i = 0; i < contours.size(); i++) {
				Rect bbox = boundingRect(contours[i]);
				int bbSize = bbox.height * bbox.width;
				int frameSize = binaryWithoutArm.cols * binaryWithoutArm.rows;
				double perc = bbSize * 100 / frameSize;
				if (DEBUG_LOCAL) cout << "contour[" << i << "] =  " << perc << endl;

				double verticalPos = (double) (bbox.y + bbox.height) / binaryWithoutArm.rows * 100;
				double horizontalPos = (double) (bbox.x + bbox.width/2) / binaryWithoutArm.cols * 100;
				if (DEBUG_LOCAL) cout << "pos: " << horizontalPos << " ver: " << verticalPos << endl;

				// arm founded
				if (horizontalPos > 48 && horizontalPos < 52 && verticalPos > 98){
					armBb = bbox;

					// TODO find center - continue here
					Mat myArm = binaryWithoutArm(bbox);

					int selectedRow = 84, selectedColumn = 0, rowCount = 0, lastRowCount = 0;
//					for (int row = 0; row < myArm.rows; row++) {
//
//						cout << "row: " << row << " rowCount: " << countNonZero(myArm.row(row)) << endl;
//						rowCount = countNonZero(myArm.row(row));
//						if (lastRowCount <= rowCount){
//							lastRowCount = rowCount;
//							selectedRow = row;
//						}else{
//							break;
//						}
//					}
//					cout << "selectedRow: " << selectedRow << endl;

					// find middle point in binary lines - horizontal
					int firstIdx = 0, lastIdx = 0;
					for (int col = 0; col < myArm.cols; col++) {
						if (firstIdx == 0 && myArm.at<bool>(col, selectedRow) == 1){
							firstIdx = col;
						}
						if (myArm.at<bool>(col, selectedRow) == 1){
							lastIdx = col;
						}
					}

					selectedColumn = (firstIdx + lastIdx) /2;
					armCenter = Point (bbox.x + selectedColumn, bbox.y + selectedRow);
					rectangle(binaryWithoutArm, Point(armBb.x, armBb.y), Point(armBb.x+armBb.width, armBb.y+armBb.height), colorBlack, CV_FILLED);
					armDetected = true;

					if (DEBUG_LOCAL) showImg(myArm, "aarm");
					if (DEBUG_LOCAL) cout << "arm detected" << endl;
				}
			}
			if (DEBUG_LOCAL) cout << "-----------------------" << endl;
		}
	}
}

/*
 * input: binary image of table
 * output: true/false if there is any other object expect arm on table
 */
void MyImageAnalyser::detectObjects(){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::detectObjects" << endl;

	detectedObjects.clear();

	if (armDetected){

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		cv::findContours(binaryWithoutArm.clone(), contours, hierarchy, CV_RETR_EXTERNAL,
				CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		vector<RotatedRect> minRect( contours.size() );

		// loop through founded contours
		for (int i = 0; i < contours.size(); i++) {
			// test draw contours
			//drawContours(outputColorStretchFrame, contours, i, colorGreen, 3, 8, hierarchy, 0, Point());


			// filter small thrash
			Rect bbox = boundingRect(contours[i]);
			if (bbox.width < 5 || bbox.height < 5){
				if (DEBUG_LOCAL) cout << "skipping rect TOO SMALL " << endl;
				continue;
			}

			// create rotate rect
			minRect[i] = minAreaRect( Mat(contours[i]) );

			// add rotate rect to found objects
			detectedObjects.push_back(minRect[i]);
		}
	}

	isObjectDetected = (detectedObjects.size() > 0);

}

void MyImageAnalyser::saveImageForProcessing(){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::saveImageForProcessing" << endl;

	if (interestingFrame && isObjectDetected){
		mInputStorage->addToProcessingQueue(originalColorFrame, binaryWithoutArm, armCenter, detectedObjects, oneMmInPx);


		// show next processing frame
		processImgCount++;
		Mat nextFrameToPrcess = outputColorStretchFrame.clone();
		stringstream str;
		str << "process img: " <<  processImgCount;
		cv::putText(nextFrameToPrcess, str.str(), cvPoint(15, 75), FONT_HERSHEY_COMPLEX_SMALL, 0.8, colorRed, 1, CV_AA);
		imshow("new image to process", nextFrameToPrcess);
	}
}

void MyImageAnalyser::saveInputFrame(Mat frame, long videoTime){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::saveInputFrame" << endl;

	if (startTime == 0){
		startTime = videoTime;
	}

	// camera is starting wait some time
	if (videoTime - startTime < 2000){
		return;
	}

    if (resizedWidth == -1 || resizedHeight == -1){
        resizedWidth = frame.cols/resizeRatio;
        resizedHeight = frame.rows/resizeRatio;
    }

    originalColorFrame = frame.clone();

    if (resiseInput){
        resize(frame, outputColorFrame, Size(resizedWidth, resizedHeight));
    }

    video_time = videoTime;

    // save img size and setup font size
    mDebugFrames->c->initFontSize(&frame);
}

void MyImageAnalyser::detectMovementResetStates(){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::detectMovementResetStates" << endl;

	interestingFrame = false;
	detectMovementLastGrayFrame = detectMovementGrayFrame.clone();
}

void MyImageAnalyser::showOutput(){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::showOutput" << endl;

//    this->showImg(originalColorFrame, "originalColorFrame");
//    this->showImg(outputColorFrame, "outpuColorFrame");
    this->showImg(outputColorStretchFrame, "outputColorStretchFrame");
//    this->showImg(binaryTableCrop, "binaryTableCrop");
//    this->showImg(binaryWithoutArm, "binaryWithoutArm");
}

/*
 * show images saved through application and show them on main thread - GUI
 */
void MyImageAnalyser::showImageFromBackground(){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::showOutput" << endl;

	while (mInputStorage->getDisplayQueueSize() >0){
		ImageStoreItem mImageStoreItem = mInputStorage->getImgFromDiplayQueue();
		this->showImg(mImageStoreItem.mMat, mImageStoreItem.name);
	}
}

/*
 * show image with fallback for possible empty image
 */
void MyImageAnalyser::showImg(Mat frame, String frameName){
	if (DEBUG_LOCAL) cout << "MyImageAnalyser::showOutput" << endl;

	if (frame.dims != 0) {
		imshow(frameName, frame);
	} else {
		if (DEBUG_LOCAL) cout << frameName << " is EMPTY !" << endl;
	}
}

void MyImageAnalyser::initTrackball(){
    namedWindow("tools", 1);
    createTrackbar( "th background", "tools", &sliderThresholdBg, slider_max, on_trackbar);
}

void MyImageAnalyser::propagatePresedKeyToBackend(){
	if (lastKey != -1){
		mInputStorage->addToKeyPressQueue(lastKey);
		lastKey = -1;
	}
}
