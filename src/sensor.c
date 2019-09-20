/*
 * sensor.c
 *
 *  Created on: Sep 19, 2019
 *      Author: Seokjin Yoon
 */

#include "sensor.h"

static const int i2c_bus = 1;

peripheral_error_e open_sensor_i2c(const int address, peripheral_i2c_h *i2c) {
	peripheral_error_e ret = PERIPHERAL_ERROR_NONE;
	ret = peripheral_i2c_open(i2c_bus, address, i2c);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_open] failed. %d %s", ret, get_error_message(ret));
	} else {
		_I("[peripheral_i2c_open] succeeded.");
	}
	return ret;
}

void close_sensor_i2c(peripheral_i2c_h i2c) {
	peripheral_i2c_close(i2c);
}

peripheral_error_e read_sensor_i2c(peripheral_i2c_h i2c, uint8_t *data, uint32_t length) {
	peripheral_error_e ret;
	ret = peripheral_i2c_read(i2c, data, length);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_read] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

peripheral_error_e write_sensor_i2c(peripheral_i2c_h i2c, uint8_t *data, uint32_t length) {
	peripheral_error_e ret;
	ret = peripheral_i2c_write(i2c, data, length);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_write] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

peripheral_error_e read_sensor_i2c_register_byte(peripheral_i2c_h i2c, uint8_t reg, uint8_t *data) {
	peripheral_error_e ret;
	ret = peripheral_i2c_read_register_byte(i2c, reg, data);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_read_register_byte] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

peripheral_error_e write_sensor_i2c_register_byte(peripheral_i2c_h i2c, uint8_t reg, uint8_t data) {
	peripheral_error_e ret;
	ret = peripheral_i2c_write_register_byte(i2c, reg, data);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_write_register_byte] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

peripheral_error_e read_sensor_i2c_register_word(peripheral_i2c_h i2c, uint8_t reg, uint16_t *data) {
	peripheral_error_e ret;
	ret = peripheral_i2c_read_register_word(i2c, reg, data);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_read_register_word] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

peripheral_error_e write_sensor_i2c_register_word(peripheral_i2c_h i2c, uint8_t reg, uint16_t data) {
	peripheral_error_e ret;
	ret = peripheral_i2c_write_register_word(i2c, reg, data);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_write_register_word] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}
