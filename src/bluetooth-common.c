/*
 * bluetooth-common.c
 *
 *  Created on: Aug 31, 2019
 *      Author: Seokjin Yoon
 */

#include "bluetooth-common.h"

bt_error_e ret;

void init_bluetooth() {
	ret = bt_initialize();
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_initialize] failed.");

		return;
	}
}

void deinit_bluetooth() {
	/* Deinitialize Bluetooth */
	ret = bt_deinitialize();
	if (ret != BT_ERROR_NONE)
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_deinitialize] failed.");
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

void adapter_state_changed_cb(int result, bt_adapter_state_e adapter_state, void* user_data) {
    if (result != BT_ERROR_NONE) {
        dlog_print(DLOG_ERROR, LOG_TAG, "[adapter_state_changed_cb] failed! result=%d", result);

        return;
    }
    if (adapter_state == BT_ADAPTER_ENABLED) {
        dlog_print(DLOG_INFO, LOG_TAG, "[adapter_state_changed_cb] Bluetooth is enabled!");

        /* Get information about Bluetooth adapter */
/*        char *local_address = NULL;
        bt_adapter_get_address(&local_address);
        dlog_print(DLOG_INFO, LOG_TAG, "[adapter_state_changed_cb] Adapter address: %s.", local_address);
        if (local_address)
            free(local_address);
        char *local_name;
        bt_adapter_get_name(&local_name);
        dlog_print(DLOG_INFO, LOG_TAG, "[adapter_state_changed_cb] Adapter name: %s.", local_name);
        if (local_name)
            free(local_name);*/
    } else {
        dlog_print(DLOG_INFO, LOG_TAG, "[adapter_state_changed_cb] Bluetooth is disabled!");
        /*
           When you try to get device information
           by invoking bt_adapter_get_name(), bt_adapter_get_address(),
           or bt_adapter_get_visibility(), BT_ERROR_NOT_ENABLED occurs
        */
    }
}

bool get_bluetooth_adapter_visibility(int duration) {
	/* Visibility mode of the Bluetooth device */
	bt_adapter_visibility_mode_e mode;
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

void adapter_visibility_mode_changed_cb(int result, bt_adapter_visibility_mode_e visibility_mode, void* user_data) {
    if (result != BT_ERROR_NONE) {
        /* Error handling */

        return;
    }
    if (visibility_mode == BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE)
        dlog_print(DLOG_INFO, LOG_TAG, "[visibility_mode_changed_cb] None discoverable mode!");
    else if (visibility_mode == BT_ADAPTER_VISIBILITY_MODE_GENERAL_DISCOVERABLE)
        dlog_print(DLOG_INFO, LOG_TAG, "[visibility_mode_changed_cb] General discoverable mode!");
    else
        dlog_print(DLOG_INFO, LOG_TAG, "[visibility_mode_changed_cb] Limited discoverable mode!");
}
