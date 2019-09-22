/*
 * sensor-gps.h
 *
 *  Created on: Sep 20, 2019
 *      Author: Seokjin Yoon
 */

#ifndef SENSOR_GPS_H_
#define SENSOR_GPS_H_

#include "sensor.h"
#include "input-getter.h"

peripheral_error_e open_sensor_gps();
void close_sensor_gps();
void init_sensor_gps();
peripheral_error_e read_sensor_gps(double *, double *);

#endif /* SENSOR_GPS_H_ */
