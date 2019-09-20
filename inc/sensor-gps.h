/*
 * sensor-gps.h
 *
 *  Created on: Sep 20, 2019
 *      Author: Seokjin Yoon
 */

#ifndef SENSOR_GPS_H_
#define SENSOR_GPS_H_

#include "sensor.h"

#define DATA_GETTING_CYCLE 1.0

peripheral_error_e open_sensor_gps();
void close_sensor_gps();
static void init_sensor_gps();
peripheral_error_e read_sensor_gps(double *, double *);
bool set_sensor_gps_getting();
void unset_sensor_gps_getting();
static Eina_Bool sensor_get_timed_cb(void *);

#endif /* SENSOR_GPS_H_ */
