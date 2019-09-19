/*
 * input-getter.c
 *
 *  Created on: Sep 12, 2019
 *      Author: Seokjin Yoon
 */

#include "input-getter.h"

int get_pulse();
double get_temperature();
double get_spo2();
double get_activity();


body_data get_sensor_data(){
	body_data datas;

	datas.pulse = get_pulse();
	datas.activity = get_activity();
	datas.temprature = get_temperature();
	datas.spo2 = get_spo2();

	return datas;
}

int get_pulse() {
	int pulse = rand() % 100 + 50;
	_I("Pulse: %d", pulse);
	return pulse;
}

double get_temperature() {
	double temperature = (rand() % 30 + 350) * 10;
	_I("Temperature: %lf", temperature);
	return temperature;
}

double get_spo2() {
	double spo2 = (rand() % 10 + 90);
	_I("SPO2: %lf", spo2);
	return spo2;
}

double get_activity() {
	double activity = 23.0;
	_I("Activity: %lf", activity);
	return activity;
}

location_data get_location() {
	double latitude = 36.23422;
	_I("Latitude: %lf", latitude);
	double longitude = -128.35869;
	_I("Longitude: %lf", longitude);

	location_data location;
	location.latitude = latitude;
	location.longitude = longitude;

	return location;
}

