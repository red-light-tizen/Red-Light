/*
 * sensor-gps.c
 *
 *  Created on: Sep 20, 2019
 *      Author: Seokjin Yoon
 */

#include "sensor-gps.h"

static peripheral_uart_h gps_h;
static bool flag = false;
static Ecore_Timer *get_timer;

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

static void init_sensor_gps() {
	peripheral_uart_set_baud_rate(gps_h, PERIPHERAL_UART_BAUD_RATE_9600);
}

peripheral_error_e read_sensor_gps(double *latitude, double *longitude) {
	peripheral_error_e ret;
	uint8_t buf[100] = {0, };
	ret = read_sensor_uart(gps_h, buf, 100);
	double la = buf[0];
	double lo = buf[1];
	_D("Latitude: %f, Longitude: %f", la, lo);
	*latitude = la;
	*longitude = lo;
	return ret;
}

bool set_sensor_gps_getting() {
	if (!flag) {
		_E("[set_sensor_gps_getting] GPS sensor is not opened.");
		return false;
	}

	get_timer = ecore_timer_add(DATA_GETTING_CYCLE, sensor_get_timed_cb, NULL);
	if (!get_timer)
		return false;
	return true;
}

void unset_sensor_gps_getting() {
	if (get_timer) {
		ecore_timer_del(get_timer);
		get_timer = NULL;
	}
}

static Eina_Bool sensor_get_timed_cb(void *data) {
	peripheral_error_e ret;
	double latitude = 0, longitude = 0;
	ret = read_sensor_gps(&latitude, &longitude);

	if (ret != PERIPHERAL_ERROR_NONE) {
		get_timer = NULL;
		return ECORE_CALLBACK_CANCEL;
	}
	return ECORE_CALLBACK_RENEW;
}
