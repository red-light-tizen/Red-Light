/*
 * sensor-pulse.c
 *
 *  Created on: Sep 19, 2019
 *      Author: Seokjin Yoon
 */

#include "sensor-pulse.h"

static peripheral_i2c_h pulse_h;
static bool flag = false;

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

void init_sensor_pulse() {
	uint8_t previous;
	peripheral_i2c_write_register_byte(pulse_h, MAX30100_REG_MODE_CONFIGURATION, MAX30100_MODE_SPO2_HR);
	read_sensor_i2c_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, &previous);
	peripheral_i2c_write_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | MAX30100_SPC_PW_1600US_16BITS);
	read_sensor_i2c_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, &previous);
	peripheral_i2c_write_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (MAX30100_SAMPRATE_100HZ << 2));
	peripheral_i2c_write_register_byte(pulse_h, MAX30100_REG_LED_CONFIGURATION, MAX30100_LED_CURR_50MA << 4 | MAX30100_LED_CURR_50MA);
	read_sensor_i2c_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, &previous);
	peripheral_i2c_write_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, previous | MAX30100_SPC_SPO2_HI_RES_EN);
	flag = true;
}

pulse_data read_sensor_pulse() {

	peripheral_error_e ret;
	pulse_data data;

	uint8_t t1, t2;
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_WRITE_POINTER, &t1);
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_READ_POINTER, &t2);
	int heartbeat;
	double spo2;

	uint8_t IR1, IR2, RED1, RED2;

	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_DATA, &IR1);
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_DATA, &IR2);
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_DATA, &RED1);
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_FIFO_DATA, &RED2);

	heartbeat = ((IR1 << 8) | IR2) / 512;

	spo2 = 0;
	if(RED1 != 0 && RED2 != 0)
		spo2 = (110 - (((IR1 + IR2) / 2.0) / ((RED1 + RED2) / 2.0)) * 18);
	else {
		spo2 = -1;
	}
	data.heartbeat= heartbeat;
	data.spo2 = spo2;

	return data;
}

double read_sensor_temperature(){
	peripheral_error_e ret = 0;
	uint8_t temp, tempfrac;

	uint8_t previous;
	peripheral_i2c_write_register_byte(pulse_h, MAX30100_REG_MODE_CONFIGURATION, MAX30100_MODE_SPO2_HR | MAX30100_MC_TEMP_EN);
	read_sensor_i2c_register_byte(pulse_h, MAX30100_REG_SPO2_CONFIGURATION, &previous);

	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_TEMPERATURE_DATA_INT, &temp);
	ret = peripheral_i2c_read_register_byte(pulse_h, MAX30100_REG_TEMPERATURE_DATA_FRAC, &tempfrac);

	return (temp + tempfrac * 0.0625);
}
