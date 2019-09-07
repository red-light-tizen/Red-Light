/*
 * bluetooth-sender.c
 *
 *  Created on: Sep 2, 2019
 *      Author: Seokjin Yoon
 */

#include "bluetooth-sender.h"

extern bt_error_e ret;
extern int server_socket_fd;
Ecore_Timer *send_timer;

void send_bluetooth_data(const char *data, int length) {
	int client_socket_fd = server_socket_fd;

	ret = bt_socket_send_data(client_socket_fd, data, length);
	if (ret != BT_ERROR_NONE)
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_socket_send_data] failed.");
	_D("%s", get_error_message(get_last_result()));
}

Eina_Bool bluetooth_send_timed_cb(void *data) {
	static int count = 0;
	count++;
	if (count <= 100) {
		char *data = "Sending Test by Red Light";
		send_bluetooth_data(data, strlen(data) + 1);

		return ECORE_CALLBACK_RENEW;
	}
	count = 0;
	send_timer = NULL;
	return ECORE_CALLBACK_CANCEL;
}
