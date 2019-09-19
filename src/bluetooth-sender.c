/*
 * bluetooth-sender.c
 *
 *  Created on: Sep 2, 2019
 *      Author: Seokjin Yoon
 */

#include "bluetooth-sender.h"

static Ecore_Timer *send_timer;

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

bt_error_e send_bluetooth_data(void* data) {
	bt_error_e ret;
	int client_socket_fd = get_bluetooth_socket_fd();

	//ret is length of sent data
	bt_data btdata = *(bt_data*)data;

	ret = bt_socket_send_data(client_socket_fd, btdata.message, btdata.length);
	if (ret <= -1)
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_socket_send_data] failed.");

	return ret;
}

static Eina_Bool bluetooth_send_timed_cb(void *data) {
//	char my_data[DATA_SIZE];
	bt_data my_data;
	create_data_packet(&my_data);
//	send_bluetooth_data(my_data);

	return ECORE_CALLBACK_RENEW;
}

void create_data_packet(void *data) {
	time_t t = time(NULL);
	struct tm *timestamp = localtime(&t);
	int year = timestamp->tm_year + 1900;
	int day_of_year = timestamp->tm_yday + 1;
	int second_of_day = timestamp->tm_hour * 3600 + timestamp->tm_min * 60 + timestamp->tm_sec;

	condition_data condition = *(condition_data*)data;

	int status = condition.status;
	int pulse = condition.pulse;
	int temperature = condition.temprature*100;
	int spo2 = condition.spo2*100;
	int activity = condition.activity;
	double latitude = condition.latitude;
	double longitude = condition.longitude;

	bt_data my_data;

	snprintf(my_data.message, DATA_SIZE, "%d;%d;%d;%d;%d;%d;%d;%d;%.5f;%.5f;", year, day_of_year, second_of_day, status, pulse, temperature, spo2, activity, latitude, longitude);
	my_data.length = strlen(my_data.message) + 1;

	registerEvent(bt_data,my_data,send_bluetooth_data);

}
