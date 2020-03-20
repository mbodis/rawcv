/*
 * MyLog.h
 *
 *  Created on: Mar 18, 2017
 *      Author: mbodis
 */

#ifndef SRC_SYSTEM_HELPER_MYLOG_H_
#define SRC_SYSTEM_HELPER_MYLOG_H_

const int LOG_DEBUG = 0;
const int LOG_INFO = 1;
const int LOG_ERROR = 2;

static bool SEVERITY_DEBUG = false;
static bool SEVERITY_INFO = true;
static bool SEVERITY_ERROR = true;

static bool CONSOLE_OUTPUT = true;


using namespace std;

class MyLog{

	private:


	public:

		static void log(int severity, string tag, string message){
			switch(severity){

				case LOG_DEBUG:
					if (SEVERITY_DEBUG){
						if (CONSOLE_OUTPUT){
							cout << "D: " << tag << ": " << message << endl;
						}
					}
					break;

				case LOG_INFO:
					if (SEVERITY_INFO){
						if (CONSOLE_OUTPUT){
							cout << "I: " << tag << ": " << message << endl;
						}
					}
					break;

				case LOG_ERROR:
					if (SEVERITY_ERROR){
						if (CONSOLE_OUTPUT){
							cout << "E: " << tag << ": " << message << endl;
						}
					}
					break;
				}
		}

		static bool isLogEnabled(int severity){
			switch(severity){

				case LOG_DEBUG:
					return SEVERITY_DEBUG;

				case LOG_INFO:
					return SEVERITY_INFO;

				case LOG_ERROR:
					return SEVERITY_ERROR;
				}
			return false;
		}
};



#endif /* SRC_SYSTEM_HELPER_MYLOG_H_ */
