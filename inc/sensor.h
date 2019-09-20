/*
 * sensor.h
 *
 *  Created on: Sep 19, 2019
 *      Author: Seokjin Yoon
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <peripheral_io.h>
#include <Ecore.h>
#include "sensor-pulse.h"
#include "utils.h"

peripheral_error_e open_sensor_i2c(const int, peripheral_i2c_h *);
void close_sensor_i2c(peripheral_i2c_h);
peripheral_error_e read_sensor_i2c(peripheral_i2c_h, uint8_t *, uint32_t);
peripheral_error_e write_sensor_i2c(peripheral_i2c_h, uint8_t *, uint32_t);
peripheral_error_e read_sensor_i2c_register_byte(peripheral_i2c_h, uint8_t, uint8_t *);
peripheral_error_e write_sensor_i2c_register_byte(peripheral_i2c_h, uint8_t, uint8_t);
peripheral_error_e read_sensor_i2c_register_word(peripheral_i2c_h, uint8_t, uint16_t *);
peripheral_error_e write_sensor_i2c_register_word(peripheral_i2c_h, uint8_t, uint16_t);

#endif /* SENSOR_H_ */
