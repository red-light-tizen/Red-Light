/*
 * sensor-temp.h
 *
 *  Created on: Sep 20, 2019
 *      Author: Seokjin Yoon
 */

#ifndef SENSOR_TEMP_H_
#define SENSOR_TEMP_H_

#include "sensor.h"

#define DATA_GETTING_CYCLE 1.0

#define MCP3008_CH_NUM 1

int open_sensor_temp();
void close_sensor_temp();
int read_sensor_temp(unsigned int *);
bool set_sensor_temp_getting();
void unset_sensor_temp_getting();
static Eina_Bool sensor_get_timed_cb(void *);

#endif /* SENSOR_TEMP_H_ */
