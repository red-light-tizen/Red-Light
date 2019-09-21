/*
 * bluetooth-common.c
 *
 *  Created on: Aug 31, 2019
 *      Author: Seokjin Yoon
 */

#include "bluetooth-common.h"

static int server_socket_fd;

bt_error_e init_bluetooth() {
	bt_error_e ret;

	ret = bt_initialize();
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_initialize] failed.");

		return ret;
	}

	bt_adapter_set_name("Red Light12");

	char *local_name;
	bt_adapter_get_name(&local_name);
	_I("Bluetooth adapter name: %s", local_name);
	if (local_name)
		free(local_name);

	char *local_address;
	bt_adapter_get_address(&local_address);
	_I("Bluetooth adapter address: %s", local_address);
	if (local_address)
		free(local_address);

	set_bluetooth_adapter_state_changed();
	set_bluetooth_adapter_visibility_changed();

	if (get_bluetooth_adapter_state() && get_bluetooth_adapter_visibility(1)) {
		create_bluetooth_socket();
		listen_and_accept_bluetooth_socket();
		set_bluetooth_socket_connection_requested();
		set_bluetooth_socket_connection_state_changed();
	}

	return ret;
}

bt_error_e deinit_bluetooth() {
	bt_error_e ret;

	unset_bluetooth_data_sending();
	unset_bluetooth_data_receiving();
	unset_bluetooth_socket_connection_state_changed();
	unset_bluetooth_socket_connection_requested();
	destroy_bluetooth_socket();
	unset_bluetooth_adapter_visibility_changed();
	unset_bluetooth_adapter_state_changed();

	/* Deinitialize Bluetooth */
	ret = bt_deinitialize();
	if (ret != BT_ERROR_NONE)
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_deinitialize] failed.");

	return ret;
}

bool get_bluetooth_adapter_state() {
	bt_error_e ret;
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

bt_error_e set_bluetooth_adapter_state_changed() {
	bt_error_e ret;

	ret = bt_adapter_set_state_changed_cb(adapter_state_changed_cb, NULL);
	if (ret != BT_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_set_state_changed_cb()] failed.");

	return ret;
}

void unset_bluetooth_adapter_state_changed() {
	bt_adapter_unset_state_changed_cb();
}

static void adapter_state_changed_cb(int result, bt_adapter_state_e adapter_state, void* user_data) {
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

bt_error_e set_bluetooth_adapter_visibility_changed() {
	bt_error_e ret;

	ret = bt_adapter_set_visibility_mode_changed_cb(adapter_visibility_mode_changed_cb, NULL);
	if (ret != BT_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_set_visibility_mode_changed_cb] failed.");

	return ret;
}

void unset_bluetooth_adapter_visibility_changed() {
	bt_adapter_unset_visibility_mode_changed_cb();
}

static void adapter_visibility_mode_changed_cb(int result, bt_adapter_visibility_mode_e visibility_mode, void* user_data) {
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

int get_bluetooth_socket_fd() {
	return server_socket_fd;
}

bt_error_e create_bluetooth_socket() {
	bt_error_e ret;

	server_socket_fd = -1;
	ret = bt_socket_create_rfcomm(BLUETOOTH_UUID, &server_socket_fd);
	if (ret != BT_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "bt_socket_create_rfcomm() failed.");

	return ret;
}

bt_error_e destroy_bluetooth_socket() {
	bt_error_e ret;

	ret = bt_socket_destroy_rfcomm(server_socket_fd);
	if (ret != BT_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_socket_destroy_rfcomm] failed.");
	else
		dlog_print(DLOG_INFO, LOG_TAG, "[bt_socket_destroy_rfcomm] succeeded. socket_fd = %d", server_socket_fd);

	return ret;
}

bt_error_e listen_and_accept_bluetooth_socket() {
	bt_error_e ret;
	ret = bt_socket_listen_and_accept_rfcomm(server_socket_fd, MAX_PENDING_CONNECTIONS);
	if (ret != BT_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_socket_listen_and_accept_rfcomm] failed.");
	else {
		dlog_print(DLOG_INFO, LOG_TAG, "[bt_socket_listen_and_accept_rfcomm] Succeeded. bt_socket_connection_state_changed_cb will be called.");
		/* Waiting for incoming connections */
	}

	return ret;
}

bt_error_e set_bluetooth_socket_connection_requested() {
	bt_error_e ret;
	ret = bt_socket_set_connection_requested_cb(socket_connection_requested_cb, NULL);
	if (ret != BT_ERROR_NONE)
		_E("[bt_socket_set_connection_requested_cb] failed.");

	return ret;
}

void unset_bluetooth_socket_connection_requested() {
	bt_socket_unset_connection_requested_cb();
}

static void socket_connection_requested_cb(int socket_fd, const char *remote_address, void *user_data) {
	_I("Callback: Socket of request - %d", socket_fd);
	_I("Callback: Address of request - %s", remote_address);
}

bt_error_e set_bluetooth_socket_connection_state_changed() {
	bt_error_e ret;
	ret = bt_socket_set_connection_state_changed_cb(socket_connection_state_changed, NULL);
	if (ret != BT_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_socket_set_connection_state_changed_cb] failed.");

	return ret;
}

void unset_bluetooth_socket_connection_state_changed() {
	bt_socket_unset_connection_state_changed_cb();
}

static void socket_connection_state_changed(int result, bt_socket_connection_state_e connection_state, bt_socket_connection_s *connection, void *user_data) {
	if (result != BT_ERROR_NONE) {
        dlog_print(DLOG_ERROR, LOG_TAG, "[socket_connection_state_changed_cb] failed. result =%d.", result);

        return;
    }

    if (connection_state == BT_SOCKET_CONNECTED) {
        dlog_print(DLOG_INFO, LOG_TAG, "Callback: Connected.");
        if (connection != NULL) {

        	/*
        	 * Check device is paired.
        	 * Add device to trusted device list after pairing.
        	 * bonded == trusted.
        	 * If who know this app's UUID, he can add device to trusted device list.
        	 * I think there is a security problem.
        	 * TODO : Add Handshake and replace authorization function's position into handshake function.
        	*/

        	bt_device_info_s* device;
        	bt_error_e ret =  bt_adapter_get_bonded_device_info(connection->remote_address, &device);

        	if(ret == BT_ERROR_REMOTE_DEVICE_NOT_BONDED){
        		bt_device_set_bond_created_cb(add_bluetooth_device_authorization, NULL);
        		bt_device_create_bond(connection->remote_address);
        	}


            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Socket of connection - %d.", connection->socket_fd);
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Role of connection - %d.", connection->local_role);
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Address of connection - %s.", connection->remote_address);
            /* socket_fd is used for sending data and disconnecting a device */
            server_socket_fd = connection->socket_fd;

            set_bluetooth_data_receiving();
            set_bluetooth_data_sending();


        } else {
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: No connection data");
        }
    } else {
        dlog_print(DLOG_INFO, LOG_TAG, "Callback: Disconnected.");
        if (connection != NULL) {
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Socket of disconnection - %d.", connection->socket_fd);
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Address of connection - %s.", connection->remote_address);

            unset_bluetooth_data_receiving();
            unset_bluetooth_data_sending();
        } else {
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: No connection data");
        }
    }
}

static void add_bluetooth_device_authorization(int result, bt_device_info_s *device_info, void *user_data){
	if(result == BT_ERROR_NONE){
		bt_device_set_authorization(device_info->remote_address, BT_DEVICE_AUTHORIZED);
	}
}
