/*
 * bluetooth-common.h
 *
 *  Created on: Aug 31, 2019
 *      Author: Seokjin Yoon
 */

#ifndef BLUETOOTH_COMMON_H_
#define BLUETOOTH_COMMON_H_

#include <bluetooth.h>
#include "red-light.h"
#include "bluetooth-receiver.h"
#include "bluetooth-sender.h"

#define BLUETOOTH_UUID "E9E2ED52-12AA-405A-AB1F-0C70878EFFD9"
#define MAX_PENDING_CONNECTIONS 5

bt_error_e init_bluetooth();
bt_error_e deinit_bluetooth();
bool get_bluetooth_adapter_state();
bt_error_e set_bluetooth_adapter_state_changed();
void unset_bluetooth_adapter_state_changed();
bt_error_e set_bluetooth_adapter_visibility_changed();
void unset_bluetooth_adapter_visibility_changed();
bool get_bluetooth_adapter_visibility(int);
bt_error_e create_bluetooth_socket();
bt_error_e destroy_bluetooth_socket();
bt_error_e listen_and_accept_bluetooth_socket();
bt_error_e set_bluetooth_socket_connection_requested();
void unset_bluetooth_socket_connection_requested();
bt_error_e set_bluetooth_socket_connection_state_changed();
void unset_bluetooth_socket_connection_state_changed();

static void adapter_state_changed_cb(int result, bt_adapter_state_e adapter_state, void* user_data);
static void adapter_visibility_mode_changed_cb(int result, bt_adapter_visibility_mode_e visibility_mode, void* user_data);
static void socket_connection_requested_cb(int, const char *, void *);
static void socket_connection_state_changed(int result, bt_socket_connection_state_e connection_state, bt_socket_connection_s *connection, void *user_data);
static void add_bluetooth_device_authorization(int result, bt_device_info_s *device_info, void *user_data);

#endif /* BLUETOOTH_COMMON_H_ */
