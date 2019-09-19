/*
 * sensor-pulse.c
 *
 *  Created on: Sep 19, 2019
 *      Author: Seokjin Yoon
 */

#include "sensor-pulse.h"

static peripheral_i2c_h pulse_h;

static Ecore_Timer *get_timer;

peripheral_error_e open_sensor_pulse() {
	return open_sensor_i2c(MAX30100_I2C_READ_ADDRESS, &pulse_h);
}

void close_sensor_pulse() {
	close_sensor_i2c(pulse_h);
}

peripheral_error_e read_sensor_pulse(int *pulse, double *spo2) {
	peripheral_error_e ret;
	static unsigned char buf[10] = {0, };

	ret = read_sensor_i2c(pulse_h, buf, DATA_SIZE);
	int p = buf[0] << 8 | buf[1];
	double s = buf[0] << 8 | buf[1];
	_D("Pulse: %d", p);
	_D("SPO2: %f", s);
	*pulse = p;
	*spo2 = s;
	return ret;
}

bool set_sensor_pulse_getting() {
	get_timer = ecore_timer_add(DATA_GETTING_CYCLE, sensor_get_timed_cb, NULL);
	if (!get_timer)
		return false;
	return true;
}

void unset_sensor_pulse_getting() {
	if (get_timer) {
		ecore_timer_del(get_timer);
		get_timer = NULL;
	}
}

static Eina_Bool sensor_get_timed_cb(void *data) {
	int pulse = 0;
	double spo2 = 0;
	read_sensor_pulse(&pulse, &spo2);

	return ECORE_CALLBACK_RENEW;
}
