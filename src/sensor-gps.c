/*
 * sensor-gps.c
 *
 *  Created on: Sep 20, 2019
 *      Author: Seokjin Yoon
 */

#include "sensor-gps.h"

static peripheral_uart_h gps_h;
static bool flag = false;

peripheral_error_e open_sensor_gps() {
	peripheral_error_e ret;
	ret = open_sensor_uart(&gps_h);
	if (ret == PERIPHERAL_ERROR_NONE) {
		flag = true;
		init_sensor_gps();
	}
	return ret;
}

void close_sensor_gps() {
	close_sensor_uart(gps_h);
	flag = false;
}

void init_sensor_gps() {
	peripheral_uart_set_baud_rate(gps_h, PERIPHERAL_UART_BAUD_RATE_9600);
}

peripheral_error_e read_sensor_gps(double *latitude, double *longitude) {
	peripheral_error_e ret;
	uint8_t buf[100] = {0, };
	ret = read_sensor_uart(gps_h, buf, 100);
	double la = buf[0]; //3553.41564
	double lo = buf[1]; //127.53518
	_D("Latitude: %f, Longitude: %f", la, lo);
	*latitude = la;
	*longitude = lo;
	return ret;
}
