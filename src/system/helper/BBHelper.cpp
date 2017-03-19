/*
 * BBHelper.cpp
 *
 *  Created on: Jun 13, 2016
 *      Author: mbodis
 */

#include "BBHelper.h"


/*
 * bwMat : binary image
 * bb: rectangle
 *
 * result: if BB is more than 90% of binaryInputMat
 */
bool BBHelper::isBBFillingAllHeight(Mat *bwMat, Rect bb){

	int heightPercentage = ((double)bb.height / bwMat->rows) *100;

	return heightPercentage > 90;
}

/*
 * bwMat : binary image
 * bb1 - bounding box selecting one part of line
 * bb2 - bounding box selecting second part of line
 */
bool BBHelper::isBBAboveBB(AppConfig *c, Mat *bwMat, Rect bb1, Rect bb2){

	// bb1 starts upper than bb2
	if (bb1.y < bb2.y){

		// left of right border is the same column
		if (bb1.x == bb2.x ||
				(bb1.x +bb1.width == bb2.x + bb2.width) ){
			return true;
		}

		// left border bb1 is close to bb2
		if (bb1.x < bb2.x &&
				bb1.x + bb1.width > bb2.x){
			return true;
		}

	}

	return false;
}

