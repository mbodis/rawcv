/*
 * Utilities.h
 *
 *  Created on: Jun 1, 2016
 *      Author: mbodis
 */

#ifndef SRC_HELPER_FILESYSTEMHELPER_H_
#define SRC_HELPER_FILESYSTEMHELPER_H_

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include "MyLog.h"

using namespace cv;
using namespace std;

class FileSystemHelper {
private:

public:
	static bool DirectoryExists(const char*);
	static int getNumberFilesInFolder(const char*);
	static string getFileInFolder(char*, int);
	static string getFileContent(char*);

};

#endif /* SRC_HELPER_FILESYSTEMHELPER_H_ */
