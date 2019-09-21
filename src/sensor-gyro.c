/*
 * sensor-gyro.c
 *
 *  Created on: Sep 22, 2019
 *      Author: Seokjin Yoon
 */

#include "sensor-gyro.h"

static peripheral_i2c_h gyro_h;
static bool flag = false;
static Ecore_Timer *get_timer;

peripheral_error_e open_sensor_gyro() {
	peripheral_error_e ret;
	ret = open_sensor_i2c(MPU6050_DEFAULT_ADDRESS, &gyro_h);
	if (ret == PERIPHERAL_ERROR_NONE) {
		init_sensor_gyro();
	}
	return ret;
}

void close_sensor_gyro() {
	close_sensor_i2c(gyro_h);
	flag = false;
}

void init_sensor_gyro() {
	setClockSource(MPU6050_CLOCK_PLL_XGYRO);
	setFullScaleGyroRange(MPU6050_GYRO_FS_250);
	setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
	setSleepEnabled(false);
//	setAccelFIFOEnabled(true);
	flag = true;
}

void read_sensor_accel(unsigned int *accel) {
	int16_t x = 0, y = 0, z = 0;
	getAcceleration(&x, &y, &z);
	unsigned int a = x * x + y * y + z * z;
	_D("Acceleration: %d", a);
	*accel = a;
}

static void setClockSource(uint8_t source) {
	peripheral_error_e ret;
	peripheral_i2c_write_register_byte(gyro_h, MPU6050_RA_PWR_MGMT_1, source);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_write_register_byte] failed. %d %s", ret, get_error_message(ret));
	}
}

static void setFullScaleGyroRange(uint8_t range) {
	peripheral_error_e ret;
	peripheral_i2c_write_register_byte(gyro_h, MPU6050_RA_GYRO_CONFIG, range);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_write_register_byte] failed. %d %s", ret, get_error_message(ret));
	}
}

static void setFullScaleAccelRange(uint8_t range) {
	peripheral_error_e ret;
	peripheral_i2c_write_register_byte(gyro_h, MPU6050_RA_ACCEL_CONFIG, range);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_write_register_byte] failed. %d %s", ret, get_error_message(ret));
	}
}

static void setSleepEnabled(bool enabled) {
	peripheral_error_e ret;
	peripheral_i2c_write_register_byte(gyro_h, MPU6050_RA_PWR_MGMT_1, enabled);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_write_register_byte] failed. %d %s", ret, get_error_message(ret));
	}
}

bool set_sensor_gyro_getting() {
	if (!flag) {
		_E("[set_gyro_pulse_getting] Gyro sensor is not opened.");
		return false;
	}

	get_timer = ecore_timer_add(DATA_GETTING_CYCLE, sensor_get_timed_cb, NULL);
	if (!get_timer)
		return false;
	return true;
}

void unset_sensor_gyro_getting() {
	if (get_timer) {
		ecore_timer_del(get_timer);
		get_timer = NULL;
	}
}

static Eina_Bool sensor_get_timed_cb(void *data) {
//	peripheral_error_e ret;
	unsigned int accel = 0;
	read_sensor_accel(&accel);

/*	if (ret != PERIPHERAL_ERROR_NONE) {
		get_timer = NULL;
		return ECORE_CALLBACK_CANCEL;
	}*/
	return ECORE_CALLBACK_RENEW;
}

static void getAcceleration(int16_t *x, int16_t *y, int16_t *z) {
	uint8_t buffer[6];
	read_sensor_i2c(gyro_h, buffer, 6);
	*x = (((int16_t)buffer[0]) << 8) | buffer[1];
	*y = (((int16_t)buffer[2]) << 8) | buffer[3];
	*z = (((int16_t)buffer[4]) << 8) | buffer[5];
	_D("x: %d, y: %d, z: %d", *x, *y, *z);
}

static void setAccelFIFOEnabled(bool enabled) {
	peripheral_i2c_write_register_byte(gyro_h, MPU6050_RA_FIFO_EN, enabled);
}
