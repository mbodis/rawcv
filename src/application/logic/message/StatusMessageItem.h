/*
 * MyMessageItem.h
 *
 *  Created on: Mar 12, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_LOGIC_STATUSMESSAGEITEM_H_
#define SRC_APPLICATION_LOGIC_STATUSMESSAGEITEM_H_
using namespace std;

#include "../../../system/helper/TimeHelper.h"

static int MSG_STATUS_UNKNOWN = -1;
static int MSG_STATUS_INIT = 0;
static int MSG_STATUS_IN_PROGRESS = 1;
static int MSG_STATUS_FINISHED = 2;

/*
 *
 */
class StatusMessageItem{

public:
	int status = MSG_STATUS_UNKNOWN;
	string key;
	long timeEnd = -1 ;

	StatusMessageItem(int status, string key){
		this->status = status;
		this->key = key;
	}
};



#endif /* SRC_APPLICATION_LOGIC_STATUSMESSAGEITEM_H_ */
