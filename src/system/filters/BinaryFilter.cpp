/*
 * BBHelper.cpp
 *
 *  Created on: Jun 13, 2016
 *      Author: mbodis
 */

#include "../filters/BinaryFilter.h"

/*
 * inBinMat : binary image 
 * outBinMat: binary image with filled small holes
 * percentageLimit: filter all small object by percentage
 *      (e.g. 4% means it will remove all white blobs that BB size is smaller than 4% of full img size )
 *
 * description:
 * 		1) find contours in bwImage
 * 		2) filter only small detected objects by limit
 * 		3) fill object with white color
 */
void BinaryFilter::fillHoles(Mat *inBinMat, Mat *outBinMat, double percentageLimit) {
    bool LOCAL_DEBUG = false;
    
    int sizeSq = inBinMat->cols * inBinMat->rows;
    MyLog::log(LOG_DEBUG, TAG, "input sizeSqPx: " +  std::to_string(sizeSq) + " limit:" + to_string(percentageLimit));
    
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	cv::findContours(inBinMat->clone(), contours, hierarchy, CV_RETR_TREE,
			CV_CHAIN_APPROX_NONE, Point(0, 0));

	// bounding box
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
    MyLog::log(LOG_DEBUG, TAG, "contours.size(): " + to_string(contours.size()));

	// filling holes
	Mat holesMat = Mat::zeros(inBinMat->size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++) {

		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
       	MyLog::log(LOG_DEBUG, TAG, "contours.at(i).size(): " + to_string(contours.at(i).size()));

		if ((double)((double)(boundRect[i].width*boundRect[i].height)/sizeSq *100) < percentageLimit) {

			vector<Point> hull;
			vector<Point> pts;
			for (size_t j = 0; j < contours[i].size(); j++) {
				pts.push_back(contours[i][j]);
			}
			convexHull(pts, hull);
			*outBinMat = inBinMat->clone();
			fillConvexPoly(*outBinMat, hull, 255);
			fillConvexPoly(holesMat, hull, Scalar(255, 255, 255));
		}
	}
    
    if (LOCAL_DEBUG){
        imshow("fill Holes - input", *inBinMat);
        imshow("fill Holes - pure holes", holesMat);
        imshow("fill Holes - output", *outBinMat);
    }
}


/*
 * bwMat: binary image
 * output: binary image with removed noise
 * percentageLimit: filter all small object by percentage
 *      (e.g. 4% means it will remove all white blobs that BB size is smaller than 4% of full img size )
 * description:
 * 		1) find contours
 * 		2) get BB on contours
 * 		3) filter contours and by size 1-2px
 * 		4) filter contours and by percentage Limit
 *      5) remove outer 1px borders
 */
void BinaryFilter::removeNoise(Mat *inBinMat, Mat *outBinMat, double percentageLimit) {
    bool LOCAL_DEBUG = false;

    Mat debugSmallNoise = Mat::zeros(inBinMat->size(), CV_8UC3);
    Mat debugLargeNoise = Mat::zeros(inBinMat->size(), CV_8UC3);
    
    *outBinMat = inBinMat->clone();
    
    int sizeSq = inBinMat->cols * inBinMat->rows;
    MyLog::log(LOG_DEBUG, TAG, "input sizeSqPx: " + to_string(sizeSq) + " limit:" + to_string(percentageLimit));
    
	Mat filteredContours = Mat::zeros(inBinMat->size(), CV_8UC3);
	Mat boundingBoxes = Mat::zeros(inBinMat->size(), CV_8UC3);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	cv::findContours(inBinMat->clone(), contours, hierarchy, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// create bounding box
	vector<vector<Point> > contours_poly(contours.size());    
	vector<Rect> boundRect(contours.size());
    for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
       	MyLog::log(LOG_DEBUG, TAG, "contours.at(i).size(): " + to_string(contours.at(i).size())
            + " bb: " + to_string((boundRect[i].width*boundRect[i].height))
            + " tot: " + to_string((double)((double)(boundRect[i].width*boundRect[i].height)/sizeSq *100)) +  "%");

    }
  	MyLog::log(LOG_DEBUG, TAG, "contours.size(): " + to_string(contours.size()));

	// filter 1-2 px noise
    for (int i = 0; i < contours.size(); i++) {
        if (contours.at(i).size() < 4) {
			for (int j = 0; j < contours[i].size(); j++) {
				cv::rectangle(debugSmallNoise, Point(contours[i][j].x, contours[i][j].y),
						Point(contours[i][j].x, contours[i][j].y), colorWhite,
						CV_FILLED);

				cv::rectangle(*outBinMat, Point(contours[i][j].x, contours[i][j].y),
						Point(contours[i][j].x, contours[i][j].y), colorBlack,
						CV_FILLED);
			}
		}
	}

    // filter object by percentage limit
	for (int i = 0; i < contours.size(); i++) {
		if ((double)((double)(boundRect[i].width*boundRect[i].height)/sizeSq *100) < percentageLimit) {
			cv::rectangle(debugLargeNoise, boundRect[i].tl(), boundRect[i].br(),
					colorWhite, CV_FILLED);
            cv::rectangle(*outBinMat, boundRect[i].tl(), boundRect[i].br(),
                colorBlack, CV_FILLED);
		}
	}
    
    // remove 1px border - (outer border - contourc cannot locate there)
	cv::rectangle(*outBinMat, Point(0, 0), Point(1, outBinMat->rows), colorBlack, CV_FILLED);
	cv::rectangle(*outBinMat, Point(0, 0), Point(outBinMat->cols, 1), colorBlack, CV_FILLED);
	cv::rectangle(*outBinMat, Point(outBinMat->cols - 1, 0), Point(outBinMat->cols, outBinMat->rows), colorBlack, CV_FILLED);
	cv::rectangle(*outBinMat, Point(0, outBinMat->rows - 1), Point(outBinMat->cols, outBinMat->rows), colorBlack, CV_FILLED);
    
    if (LOCAL_DEBUG){
        imshow("remove noise - input", *inBinMat);
        imshow("remove 1-2px noise", debugSmallNoise);
        imshow("remove object noise", debugLargeNoise);
        imshow("remove noise - output", *outBinMat);
    }
}

/*
 * bwMat: binary image
 * output: binary image with filled areas
 * 
 * description:
 *      we have a binary image with large object inside, but it has some error and 
 *      it's not 100% filled, we want to to find short gap in vertical direction and fill them
 * 
 * 		1) count nonZero pixels in columns
 * 		2) find first and last pixel
 * 		3) join founded pixels with line 
 */
void BinaryFilter::fillVerticalHoles(Mat *inBinMat, Mat *outBinMat, double rowWhitePerc) {
    bool LOCAL_DEBUG = false;
    
    Mat debugMat = Mat::zeros(inBinMat->size(), CV_8UC3);
    *outBinMat = inBinMat->clone();

    int sizeSq = inBinMat->cols * inBinMat->rows;
    MyLog::log(LOG_DEBUG, TAG, "input sizeSqPx: " + to_string(sizeSq) + " limit: " + to_string(rowWhitePerc));
    
    for (int col = 0; col < outBinMat->cols; col++) {
        double columnPercen = (double) countNonZero(outBinMat->col(col)) / inBinMat->rows * 100;
        
        if (columnPercen > rowWhitePerc){  
        
            int firstRow =-1, lastRow = -1;
            for (int row = 0; row < outBinMat->rows; row++) {
                if ( col != 0 && col < outBinMat->cols 
                && row != 0 && row < outBinMat->rows
                &&  (outBinMat->at<bool>(row, col) == 1)
                &&  (outBinMat->at<bool>(row, col+1) == 1)
                &&  (outBinMat->at<bool>(row+1, col) == 1)
                &&  (outBinMat->at<bool>(row+1, col+1) == 1)
                &&  (outBinMat->at<bool>(row-1, col) == 1)
                &&  (outBinMat->at<bool>(row, col-1) == 1)
                &&  (outBinMat->at<bool>(row-1, col-1) == 1)
                &&  (outBinMat->at<bool>(row-1, col+1) == 1)
                &&  (outBinMat->at<bool>(row+1, col-1) == 1) ){                
                    if (firstRow == -1) firstRow = row;
                    lastRow = row; 
                }
            }
            if (firstRow != -1 && lastRow != -1){
                cv::rectangle(*outBinMat, Point(col, firstRow), Point(col, lastRow), colorWhite, CV_FILLED);
                cv::rectangle(debugMat, Point(col, firstRow), Point(col, lastRow), colorWhite, CV_FILLED);                
            }
        }
    }    

    if (LOCAL_DEBUG){
        imshow("fillVerticalHoles - input", *inBinMat);
        imshow("fillVerticalHoles - filling", debugMat);
        imshow("fillVerticalHoles - output", *outBinMat);
    }

}

/*
 * does input binary matrix contains any zeros in rectangle area by p1 and p2
 */
bool BinaryFilter::hasBBAnyZeros(Mat *bwMat, Point p1, Point p2) {

	cv::Rect area(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y);
	Mat m = bwMat->clone();

	Mat crop = m(area);

	bool find = false;
	for (int col = 0; col < crop.cols; col++) {
		for (int row = 0; row < crop.rows; row++) {
			if (crop.at<char>(row, col) > 0) {
				find = true;
			}
		}
	}

	return find;
}
