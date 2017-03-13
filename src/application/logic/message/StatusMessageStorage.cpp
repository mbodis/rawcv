/*
 * StatusMessageStorage.cpp
 *
 *  Created on: Mar 13, 2017
 *      Author: mbodis
 */

#include "StatusMessageStorage.h"

// Global static pointer used to ensure a single instance of the class.
StatusMessageStorage* StatusMessageStorage::m_pInstance = NULL;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/

StatusMessageStorage* StatusMessageStorage::Instance()
{
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new StatusMessageStorage;

   return m_pInstance;
}

StatusMessageStorage::StatusMessageStorage(){

}

/*
 * add status Message to vector
 */
void StatusMessageStorage::addToStatusMessageQueue(StatusMessageItem mStatusMessageItem){
	statusMessages.push_back(mStatusMessageItem);
}

/*
 * get status by key for statusMessage
 */
int StatusMessageStorage::getStatusMessageByKey(string key){
	for(int i=0; i<statusMessages.size(); i++){
		if (statusMessages.at(i).key == key){
			return statusMessages.at(i).status;
		}
	}
	return MSG_STATUS_UNKNOWN;
}

/*
 * set status by key for stausMessage
 */
void StatusMessageStorage::setStatusMessageByKey(string key, int newStatus, long milisWait){
	for(int i=0; i<statusMessages.size(); i++){
		if (statusMessages.at(i).key == key){
			statusMessages.at(i).status = newStatus;
			statusMessages.at(i).timeEnd = TimeHelper::getSystemTimeMilis() + milisWait;
		}
	}
}

void StatusMessageStorage::checkMessages(){
	long now = TimeHelper::getSystemTimeMilis();

	for(int i=0; i<statusMessages.size(); i++){
		if (statusMessages.at(i).timeEnd > 0
				&& statusMessages.at(i).status == MSG_STATUS_IN_PROGRESS){
			if (now > statusMessages.at(i).timeEnd){
				statusMessages.at(i).status = MSG_STATUS_FINISHED;
			}
		}
	}
}

/*
 * get status message by key
 */
StatusMessageItem StatusMessageStorage::getStatusMessage(string key){
	for(int i=0; i<statusMessages.size(); i++){
		if (statusMessages.at(i).key == key){
			StatusMessageItem mStatusMessageItem = statusMessages.at(i);
			return mStatusMessageItem;
		}
	}

	return StatusMessageItem(-1, "empty");
}

void StatusMessageStorage::removeStatusMessage(string key){
	for(int i=0; i<statusMessages.size(); i++){
		if (statusMessages.at(i).key == key){
			statusMessages.erase(statusMessages.begin() + i);
		}
	}
}

/*
 * get size of status messages
 */
int StatusMessageStorage::getStatusMessageQueueSize(){
	return statusMessages.size();
}

