/*
 * bluetooth-sender.c
 *
 *  Created on: Sep 2, 2019
 *      Author: Seokjin Yoon
 */

#include "bluetooth-sender.h"

extern int server_socket_fd;
Ecore_Timer *send_timer;

void set_bluetooth_data_sending() {
	send_timer = ecore_timer_add(DATA_SENDING_CYCLE, bluetooth_send_timed_cb, NULL);
	if (!send_timer) {
		_E("Failed to add send_timer.");

	    return;
	}
}

void unset_bluetooth_data_sending() {
	if (send_timer) {
		ecore_timer_del(send_timer);
		send_timer = NULL;
	}
}

bt_error_e send_bluetooth_data(const char *data, int length) {
	bt_error_e ret;
	int client_socket_fd = server_socket_fd;

	//ret is length of sent data
	ret = bt_socket_send_data(client_socket_fd, data, length);
	if (ret <= -1)
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_socket_send_data] failed.");

	return ret;
}

static Eina_Bool bluetooth_send_timed_cb(void *data) {
	char my_data[62];
	// now u can use 98 instead of 098.
	sprintf(my_data, "0000;000;00000000;1;%d;0000;00000;0000;+00.00000;-000.00000;",(rand()%100+50));
	send_bluetooth_data(my_data, strlen(my_data) + 1);

	return ECORE_CALLBACK_RENEW;
}
