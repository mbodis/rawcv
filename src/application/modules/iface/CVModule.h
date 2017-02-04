/*
 * CVModul.h
 *
 *  Created on: Jan 30, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_MODULES_IFACE_CVMODULE_H_
#define SRC_APPLICATION_MODULES_IFACE_CVMODULE_H_

const int MODULE_STATE_NONE = 0;
const int MODULE_STATE_START = 1;

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class CVModule{

protected:
	bool enabled = true;
	string moduleName;
	int numberStates = MODULE_STATE_NONE;
	int currentState = MODULE_STATE_NONE;

public:

	CVModule(string moduleName){
		this->moduleName = moduleName;
	}

	~CVModule(){

	}

	virtual bool initialObjectDetection(Mat *newFrame){
		cout << "CVmodule initialObjectDetection" << endl;
		throw std::logic_error(" CVmodule initialObjectDeection - method not implemented");
	}

	virtual void processNextState(Mat *newFrame){
		throw std::logic_error(" CVmodule processNextState - method not implemented ");
	}

	virtual void resetContent(){
		throw std::logic_error(" CVmodule resetContent - method not implemented");
	}

	bool isEnabled(){
		return enabled;
	}

	string getName(){
		return this->moduleName;
	}

	bool isModulActive(){
		return (currentState > 0);
	}

	void setModulState(int state){
		currentState = state;
	}

	void checkIfModuleHasEnded(){
		if (currentState >= numberStates){
			currentState = 0;
			resetContent();
		}
	}
};




#endif /* SRC_APPLICATION_MODULES_IFACE_CVMODULE_H_ */
