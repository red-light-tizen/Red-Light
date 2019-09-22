/*
 * input-getter.h
 *
 *  Created on: Sep 12, 2019
 *      Author: Seokjin Yoon
 */

#ifndef INPUT_GETTER_H_
#define INPUT_GETTER_H_

#include "utils.h"
#include "sensor-pulse.h"
#include "sensor-gps.h"
/**
 * All of "get_?" functions are returning last stored data in the buffer.
 *
 * TODO: Add buffer. (Hardware spec is required.)
 */

typedef struct body_datatype{
	int heartbeat;
	double spo2;
	double temprature;
	double activity;
}body_data;

//#define updatePerSecond 1.0 / 60.0

#define Max_body_data (int)(60*10)
body_data body_datas[Max_body_data];

typedef struct location_datatype{
	double latitude;
	double longitude;
}location_data;

void start_data_update();

Eina_Bool update_sensor_data();

void finish_data_update();

body_data get_sensor_data();
location_data get_location();

#endif /* INPUT_GETTER_H_ */
