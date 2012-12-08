/*
 * definitions.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_DEFINITIONS_H_
#define FL_DEFINITIONS_H_

#include <iostream>
#include <sstream>
#include <iomanip>

#define FL_FORMAT_NUMBERS std::setprecision(3) << std::fixed

#ifdef FL_NO_LOG
#define FL_LOG(message)
#define FL_LOGP(message)
#else

#define FL_LOG_PREFIX __FILE__ << " [" << __LINE__ << "]:"
#define FL_LOG(message) std::cout << FL_LOG_PREFIX << message << std::endl
#define FL_LOGP(message) std::cout << message << std::endl
#endif

#endif /* FL_DEFINITIONS_H_ */