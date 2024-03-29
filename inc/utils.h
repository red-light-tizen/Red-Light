/*
 * utils.h
 *
 *  Created on: Sep 11, 2019
 *      Author: 나상혁
 */

#ifndef UTILS_H_
#define UTILS_H_


#include <dlog.h>
#include <stdbool.h>
#include <stdlib.h>
#include "as-log.h"

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "red-light"

#define _I(arg...) dlog_print(DLOG_INFO, LOG_TAG, ##arg)
#define _D(arg...) dlog_print(DLOG_DEBUG, LOG_TAG, ##arg)
#define _W(arg...) dlog_print(DLOG_WARN, LOG_TAG, ##arg)
#define _E(arg...) dlog_print(DLOG_ERROR, LOG_TAG, ##arg)
#define _F(arg...) dlog_print(DLOG_FATAL, LOG_TAG, ##arg)

/*
 * Log succeed and return true
 */
#define _Success(format, arg...) (_I("SUCCESS: " format "\n", ##arg) && true)

/*
 * Log failed and return false
 */
#define _Fail(format, arg...) (_D("FAIL:[%s:%d] " format "\n",__FILE__,__LINE__, ##arg) && false)

/*
 * Log and return test result, success: true, fail: false
 */
#define _Test(function,expected,comment,...) (function(__VA_ARGS__) == expected) ? _Success("%s @ %s", #function, comment) : _Fail("%s @ %s", #function,comment)



#endif /* UTILS_H_ */
