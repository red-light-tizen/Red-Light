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
	if (ret == PERIPHERAL_ERROR_NONE) {
		init_sensor_pulse();
	}
	return ret;
}

void close_sensor_pulse() {
	close_sensor_i2c(pulse_h);
	flag = false;
}

static void init_sensor_pulse() {
	uint8_t previous;
	peripheral_i2c_write_register_byte(pulse_h, MAX30100_REG_MODE_CONFIGURATION, MAX30100_MODE_SPO2_HR);
	read_sensor_i2c_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, &previous);
	peripheral_i2c_write_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | MAX30100_SPC_PW_1600US_16BITS);
	read_sensor_i2c_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, &previous);
	peripheral_i2c_write_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (MAX30100_SAMPRATE_1000HZ << 2));
	peripheral_i2c_write_register_byte(pulse_h, MAX30100_REG_LED_CONFIGURATION, MAX30100_LED_CURR_50MA << 4 | MAX30100_LED_CURR_50MA);
	read_sensor_i2c_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, &previous);
	peripheral_i2c_write_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, previous | MAX30100_SPC_SPO2_HI_RES_EN);
	flag = true;
}

peripheral_error_e read_sensor_pulse(uint16_t *pulse, uint16_t *spo2) {
//	_D("[read_sensor_pulse]");
	peripheral_error_e ret;
	uint8_t t1, t2;
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_WRITE_POINTER, &t1);
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_READ_POINTER, &t2);
	uint8_t toRead = (t1 - t2) & (MAX30100_FIFO_DEPTH - 1);
//	uint8_t buffer[MAX30100_FIFO_DEPTH * 4];
	uint16_t p, s;
	uint8_t ph, pl, sh, sl;
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_DATA, &ph);
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_DATA, &pl);
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_DATA, &sh);
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_DATA, &sl);
	p = ((ph << 8) | pl) / 512;
	s = (sh << 8) | sl;
/*	if (s > 30000)
		s = (uint16_t)(32768.0 / s * 100);*/
	if (p > 10)
		_D("Pulse: %d %d %d", p, ph, pl);
	if (s > 1000 && sh > 70 && sh < 180)
		_D("SpO2: %d %d %d", s, sh, sl);
/*	if (toRead) {
		ret = read_sensor_i2c(pulse_h, buffer, 4 * toRead);

		for (uint8_t i = 0; i < toRead; ++i) {
			p = (uint16_t)((buffer[i * 4] << 8) | buffer[i * 4 + 1]);
			s = (uint16_t)((buffer[i * 4 + 2] << 8) | buffer[i * 4 + 3]);
		}
	}*/
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
	peripheral_error_e ret = 0;
	uint16_t pulse = 0, spo2 = 0;
//	_Test(read_sensor_pulse, PERIPHERAL_ERROR_NONE, "", &pulse, &spo2);
	ret = read_sensor_pulse(&pulse, &spo2);

	if (ret != PERIPHERAL_ERROR_NONE) {
		get_timer = NULL;
		return ECORE_CALLBACK_CANCEL;
	}
	return ECORE_CALLBACK_RENEW;
}
