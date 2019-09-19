/*
 * input-getter.h
 *
 *  Created on: Sep 12, 2019
 *      Author: Seokjin Yoon
 */

#ifndef INPUT_GETTER_H_
#define INPUT_GETTER_H_

#include "utils.h"

/**
 * All of "get_?" functions are returning last stored data in the buffer.
 *
 * TODO: Add buffer. (Hardware spec is required.)
 */

typedef struct body_datatype{
	int pulse;
	int temprature;
	double spo2;
	double activity;
}body_data;

typedef struct location_datatype{
	double latitude;
	double longitude;
}location_data;

body_data get_sensor_data();
location_data get_location();

#endif /* INPUT_GETTER_H_ */
