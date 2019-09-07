/*
 * bluetooth-common.c
 *
 *  Created on: Aug 31, 2019
 *      Author: Seokjin Yoon
 */

#include "bluetooth-common.h"

bt_error_e ret;
int server_socket_fd;
extern Ecore_Timer *send_timer;

void init_bluetooth() {
	ret = bt_initialize();
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_initialize] failed.");

		return;
	}

	char *local_name, *local_address;
	bt_adapter_set_name("Red Light");
	bt_adapter_get_name(&local_name);
	_I("Bluetooth adapter name: %s", local_name);
	if (local_name)
		free(local_name);
	bt_adapter_get_address(&local_address);
	_I("Bluetooth adapter address: %s", local_address);
	if (local_address)
		free(local_address);
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
	if (adapter_state == BT_ADAPTER_DISABLED) {
		dlog_print(DLOG_ERROR, LOG_TAG,	"Bluetooth adapter is not enabled. You should enable Bluetooth!!");
		return false;
	} else {
		_I("Bluetooth adapter is enabled.");
		return true;
	}
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
	if (mode == BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE) {
	    dlog_print(DLOG_INFO, LOG_TAG, "The device is not discoverable.");
	    return false;
	} else if (mode == BT_ADAPTER_VISIBILITY_MODE_GENERAL_DISCOVERABLE)
	    dlog_print(DLOG_INFO, LOG_TAG, "The device is discoverable. No time limit.");
	else
	    dlog_print(DLOG_INFO, LOG_TAG, "The device is discoverable for a set period of time.");
	return true;
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

void socket_connection_state_changed(int result, bt_socket_connection_state_e connection_state, bt_socket_connection_s *connection, void *user_data) {
    if (result != BT_ERROR_NONE) {
        dlog_print(DLOG_ERROR, LOG_TAG, "[socket_connection_state_changed_cb] failed. result =%d.", result);

        return;
    }

    if (connection_state == BT_SOCKET_CONNECTED) {
        dlog_print(DLOG_INFO, LOG_TAG, "Callback: Connected.");
        if (connection != NULL) {
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Socket of connection - %d.", connection->socket_fd);
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Role of connection - %d.", connection->local_role);
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Address of connection - %s.", connection->remote_address);
            /* socket_fd is used for sending data and disconnecting a device */
            server_socket_fd = connection->socket_fd;

            send_timer = ecore_timer_add(1.0, bluetooth_send_timed_cb, NULL);
            if (!send_timer) {
            	_E("Failed to add send_timer.");
            	return;
            }
        } else {
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: No connection data");
        }
    } else {
        dlog_print(DLOG_INFO, LOG_TAG, "Callback: Disconnected.");
        if (connection != NULL) {
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Socket of disconnection - %d.", connection->socket_fd);
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Address of connection - %s.", connection->remote_address);

            if (send_timer) {
            	ecore_timer_del(send_timer);
            	send_timer = NULL;
            }
        } else {
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: No connection data");
        }
    }
}
