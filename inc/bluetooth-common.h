/*
 * bluetooth-common.h
 *
 *  Created on: Aug 31, 2019
 *      Author: Seokjin Yoon
 */

#ifndef BLUETOOTH_COMMON_H_
#define BLUETOOTH_COMMON_H_

#include <dlog.h>
#include <bluetooth.h>
#include "red-light.h"

void init_bluetooth();
bool get_bluetooth_adapter_state();

#endif /* BLUETOOTH_COMMON_H_ */
