/*
 * sensor-pulse.h
 *
 *  Created on: Sep 19, 2019
 *      Author: Seokjin Yoon
 */

#ifndef SENSOR_PULSE_H_
#define SENSOR_PULSE_H_

#include "sensor.h"

#define MAX30100_I2C_READ_ADDRESS 0xAF
#define DATA_GETTING_CYCLE 1.0
#define DATA_SIZE 2

peripheral_error_e open_sensor_pulse();
void close_sensor_pulse();
peripheral_error_e read_sensor_pulse(int *, double *);
bool set_sensor_pulse_getting();
void unset_sensor_pulse_getting();
static Eina_Bool sensor_get_timed_cb(void *);

#endif /* SENSOR_PULSE_H_ */
