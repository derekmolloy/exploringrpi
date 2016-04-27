/*
 * GPIO.h  Created on: 29 Apr 2015
 * Copyright (c) 2015 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 * See: www.exploringrpi.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European
 * Union Public License (the "EUPL") version 1.1 as published by the
 * European Union. Any use of this Software, other than as authorized
 * under this License is strictly prohibited (to the extent such use
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#ifndef GPIO_H_
#define GPIO_H_
#include<string>
#include<fstream>
using std::string;
using std::ofstream;

#define GPIO_PATH "/sys/class/gpio/"

namespace exploringRPi {

typedef int (*CallbackType)(int);
enum GPIO_DIRECTION{ INPUT, OUTPUT };
enum GPIO_VALUE{ LOW=0, HIGH=1 };
enum GPIO_EDGE{ NONE, RISING, FALLING, BOTH };

class GPIO {
private:
	int number, debounceTime;
	string name, path;
public:
	GPIO(int number);                     // constructor exports pin
	virtual int getNumber() { return number; }

	// General Input and Output Settings
	virtual int  setDirection(GPIO_DIRECTION);
	virtual GPIO_DIRECTION getDirection();
	virtual int  setValue(GPIO_VALUE);
	virtual int  toggleOutput();
	virtual GPIO_VALUE getValue();
	virtual int  setActiveLow(bool isLow=true);  // low=1, high=0
	virtual int  setActiveHigh();                // default state
	virtual void setDebounceTime(int time) { this->debounceTime = time; }

	// Advanced output: faster by keeping the stream open (~20x)
	virtual int  streamOpen();
	virtual int  streamWrite(GPIO_VALUE);
	virtual int  streamClose();
	virtual int  toggleOutput(int time); // thread invert output every X ms
	virtual int  toggleOutput(int numberOfTimes, int time);
	virtual void changeToggleTime(int time) { this->togglePeriod = time; }
	virtual void toggleCancel() { this->threadRunning = false; }

	// Advanced input: detect input edges -- threaded and non-threaded
	virtual int  setEdgeType(GPIO_EDGE);
	virtual GPIO_EDGE getEdgeType();
	virtual int  waitForEdge();        // waits until button is pressed
	virtual int  waitForEdge(CallbackType callback); // threaded callback
	virtual void waitForEdgeCancel() { this->threadRunning = false; }

	virtual ~GPIO();  // destructor unexports the pin
private:
	int write(string path, string filename, string value);
	int write(string path, string filename, int value);
	string read(string path, string filename);
	int exportGPIO();
	int unexportGPIO();
	ofstream stream;
	pthread_t thread;
	CallbackType callbackFunction;
	bool threadRunning;
	int togglePeriod;  // default 100ms
	int toggleNumber;  // default -1 (infinite)
	friend void* threadedPoll(void *value);
	friend void* threadedToggle(void *value);
};

void* threadedPoll(void *value);      // callback functions for threads
void* threadedToggle(void *value);    // callback functions for threads

} /* namespace exploringRPi */

#endif /* GPIO_H_ */
