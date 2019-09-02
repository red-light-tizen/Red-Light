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
bool get_bluetooth_adapter_visibility(int);

void adapter_state_changed_cb(int result, bt_adapter_state_e adapter_state, void* user_data);

#endif /* BLUETOOTH_COMMON_H_ */
