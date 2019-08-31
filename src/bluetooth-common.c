/*
 * bluetooth-common.c
 *
 *  Created on: Aug 31, 2019
 *      Author: Seokjin Yoon
 */

#include "bluetooth-common.h"

bt_error_e ret;
bt_adapter_visibility_mode_e mode; // Visibility mode of the Bluetooth device

void init_bluetooth() {
	ret = bt_initialize();
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_initialize] failed.");

		return;
	}
}

bool get_bluetooth_adapter_state() {
	bt_adapter_state_e adapter_state;

	/* Check whether the Bluetooth adapter is enabled */
	ret = bt_adapter_get_state(&adapter_state);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_get_state] failed");

		return false;
	}
	/* If the Bluetooth adapter is not enabled */
	if (adapter_state == BT_ADAPTER_DISABLED)
		dlog_print(DLOG_ERROR, LOG_TAG,	"Bluetooth adapter is not enabled. You should enable Bluetooth!!");
	else
		_I("Bluetooth adapter is enabled.");
	return !(adapter_state == BT_ADAPTER_DISABLED);
}

bool get_bluetooth_adapter_visibility(int duration) {
	/*
	   Duration until the visibility mode is changed
	   so that other devices cannot find your device
	*/
//	int duration = 1;
	bt_adapter_get_visibility(&mode, &duration);
	if (mode == BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE)
	    dlog_print(DLOG_INFO, LOG_TAG, "The device is not discoverable.");
	else if (mode == BT_ADAPTER_VISIBILITY_MODE_GENERAL_DISCOVERABLE)
	    dlog_print(DLOG_INFO, LOG_TAG, "The device is discoverable. No time limit.");
	else
	    dlog_print(DLOG_INFO, LOG_TAG, "The device is discoverable for a set period of time.");
	return !(mode == BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE);
}
