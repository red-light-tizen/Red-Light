/*
 * sensor-pulse.c
 *
 *  Created on: Sep 19, 2019
 *      Author: Seokjin Yoon
 */

#include "sensor-pulse.h"

static peripheral_i2c_h pulse_h;
static bool flag = false;
static Ecore_Timer *get_timer;

peripheral_error_e open_sensor_pulse() {
	peripheral_error_e ret;
	ret = open_sensor_i2c(MAX30100_I2C_ADDRESS, &pulse_h);
	if (ret != PERIPHERAL_ERROR_NONE)
		flag = false;
	else {
		flag = true;
		init_sensor_pulse();
	}
	return ret;
}

void close_sensor_pulse() {
	close_sensor_i2c(pulse_h);
	flag = false;
}

static void init_sensor_pulse() {
	write_sensor_i2c_register_byte(pulse_h, MAX30100_MODE_CONFIG, 0x0B);
	write_sensor_i2c_register_byte(pulse_h, MAX30100_LED_CONFIG, 0x44);
	write_sensor_i2c_register_byte(pulse_h, MAX30100_SPO2_CONFIG, 0x07);
}

peripheral_error_e read_sensor_pulse(int16_t *pulse, int16_t *spo2) {
	peripheral_error_e ret;
	uint8_t pulse_H[1], pulse_L[1];
	uint8_t spo2_H[1], spo2_L[1];
	ret = read_sensor_i2c_register_byte(pulse_h, MAX30100_FIFO_DATA, pulse_H);
	ret = read_sensor_i2c_register_byte(pulse_h, MAX30100_FIFO_DATA, pulse_L);
	ret = read_sensor_i2c_register_byte(pulse_h, MAX30100_FIFO_DATA, spo2_H);
	ret = read_sensor_i2c_register_byte(pulse_h, MAX30100_FIFO_DATA, spo2_L);
	int16_t p = (pulse_H[0] << 8) | pulse_L[0];
	int16_t s = (spo2_H[0] << 8) | spo2_L[0];
	_D("Pulse: %d", p);
	_D("SpO2: %d", s);
	*pulse = p;
	*spo2 = s;
	return ret;
}

bool set_sensor_pulse_getting() {
	if (!flag) {
		_E("[set_sensor_pulse_getting] Pulse sensor is not opened.");
		return false;
	}

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
	peripheral_error_e ret;
	int16_t pulse = 0;
	int16_t spo2 = 0;
	ret = read_sensor_pulse(&pulse, &spo2);

	if (ret != PERIPHERAL_ERROR_NONE)
		return ECORE_CALLBACK_CANCEL;
	return ECORE_CALLBACK_RENEW;
}
