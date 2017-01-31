/*
 * CVModul.h
 *
 *  Created on: Jan 30, 2017
 *      Author: mbodis
 */

#ifndef SRC_APPLICATION_MODULES_IFACE_CVMODUL_H_
#define SRC_APPLICATION_MODULES_IFACE_CVMODUL_H_

class CVModul{
private:
	bool enabled = true;
public:

//	CVModul(){
//		//TODO
//
//	}

//	~CVModul(){
//		//TODO
//	}
//	virtual void detectObject();// TODO


	bool isEnabled(){
		return enabled;
	}
};




#endif /* SRC_APPLICATION_MODULES_IFACE_CVMODUL_H_ */
