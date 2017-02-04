/*
 * Utilities.h
 *
 *  Created on: Jun 1, 2016
 *      Author: mbodis
 */

#ifndef SRC_HELPER_FILESYSTEMHELPER_H_
#define SRC_HELPER_FILESYSTEMHELPER_H_

#include <iostream>
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
