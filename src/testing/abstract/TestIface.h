/*
 * TestIface.h
 *
 *  Created on: Jan 28, 2017
 *      Author: mbodis
 */

#ifndef SRC_TESTING_ABSTRACT_TESTIFACE_H_
#define SRC_TESTING_ABSTRACT_TESTIFACE_H_

class TestIface{

public:
	virtual void runTest(){ throw std::logic_error(" TestIface runTest - method not implemented"); };
};



#endif /* SRC_TESTING_ABSTRACT_TESTIFACE_H_ */
