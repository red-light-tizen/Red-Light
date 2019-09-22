/*
 * input-getter.c
 *
 *  Created on: Sep 12, 2019
 *      Author: Seokjin Yoon
 */

#include "input-getter.h"

double get_activity();

int next_bodydata_pos = 0;
int filled_body_data_size = 0;

int isUpdate;
Ecore_Timer* data_updater;

location_data last_location = { 37.482160, 126.885480 };

double updatePerSecond = 1 / 60.0;

void start_data_update() {
	isUpdate = 1;
	data_updater = ecore_timer_add(updatePerSecond, update_sensor_data, NULL);

}
void finish_data_update() {
	isUpdate = 0;
	if (data_updater)
		ecore_timer_del(data_updater);
}

Eina_Bool update_sensor_data() {
	if (!isUpdate) {
		_I("cancel updated!");
		return ECORE_CALLBACK_CANCEL;
	}

	body_data datas;
	pulse_data pulse = read_sensor_pulse();

	if (pulse.heartbeat <= 29 || pulse.spo2 < 0 || pulse.spo2 >= 110) {
		pulse.heartbeat = body_datas[next_bodydata_pos - 1].heartbeat;
		return ECORE_CALLBACK_RENEW;
	}

	datas.heartbeat = pulse.heartbeat;
	datas.spo2 = pulse.spo2;
	datas.activity = get_activity();
	datas.temprature = read_sensor_temperature();

	if (next_bodydata_pos >= Max_body_data) {
		next_bodydata_pos = 0;
	}

	body_datas[next_bodydata_pos++] = datas;

	if (filled_body_data_size < Max_body_data) {
		++filled_body_data_size;
	}

	location_data get_location();

	return ECORE_CALLBACK_RENEW;

}

body_data get_sensor_data_average() {
	body_data datas;

	double sum_heartbeat = 0;
	double sum_spo2 = 0;
	double sum_activity = 0;
	double sum_temprature = 0;

	for (int i = 0; i < filled_body_data_size; ++i) {
		sum_heartbeat += body_datas[i].heartbeat;
		sum_spo2 += body_datas[i].spo2;
		sum_activity += body_datas[i].activity;
		sum_temprature += body_datas[i].temprature;
	}

	datas.heartbeat = sum_heartbeat / filled_body_data_size;
	datas.spo2 = sum_spo2 / filled_body_data_size;
	datas.activity = sum_activity / filled_body_data_size;
	datas.temprature = sum_temprature / filled_body_data_size;

	return datas;
}

body_data get_sensor_data() {
	int lastpos = next_bodydata_pos-1;
	if(lastpos < 0) lastpos = Max_body_data;
	body_data datas = body_datas[lastpos];
	return datas;
}

double get_activity() {
	double activity = 23.0;
	//
	return activity;
}

location_data get_location() {

	double latitude, longitude;

	read_sensor_gps(&latitude, &longitude);

	location_data location;

	if (latitude == 0.0 && longitude == 0.0) {
		location.latitude = last_location.latitude;
		location.longitude = last_location.longitude;

	} else {
		last_location.latitude = latitude;
		last_location.longitude = longitude;
		location.latitude = latitude;
		location.longitude = longitude;
	}
	return location;
}

