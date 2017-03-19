/*
 * StatusMessageStorage.h
 *
 *  Created on: Mar 13, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_MESSAGE_STATUSMESSAGESTORAGE_H_
#define SRC_APPLICATION_LOGIC_MESSAGE_STATUSMESSAGESTORAGE_H_

#include <queue>
#include <list>
#include <map>
#include <iostream>
#include <sys/time.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "StatusMessageItem.h"
#include "../../../system/config/Constants.h"

using namespace cv;
using namespace std;

/*
 *
 */
class StatusMessageStorage{

private:
	StatusMessageStorage();	// Private so that it can  not be called
	StatusMessageStorage(StatusMessageStorage const&){};	// copy constructor is private
	StatusMessageStorage& operator=(StatusMessageStorage const&){};  // assignment operator is private
	static StatusMessageStorage* m_pInstance;

	vector<StatusMessageItem> statusMessages;

public:

	static StatusMessageStorage* Instance();
	static StatusMessageStorage& getInstance(){
	static StatusMessageStorage instance;
		return instance;
	}

	/*
	 * status messages
	 */
	void addToStatusMessageQueue(StatusMessageItem mStatusMessageItem);
	int getStatusMessageByKey(string key);
	void setStatusMessageByKey(string key, int newStatus, long waitMilis);
	StatusMessageItem getStatusMessage(string key);
	void removeStatusMessage(string key);
	int getStatusMessageQueueSize();
	void checkMessages();
};

#endif /* SRC_APPLICATION_LOGIC_MESSAGE_STATUSMESSAGESTORAGE_H_ */
