/*
 * sensor-temp.c
 *
 *  Created on: Sep 20, 2019
 *      Author: Seokjin Yoon
 */

#include "sensor-temp.h"

static bool flag = false;
static Ecore_Timer *get_timer;

int open_sensor_temp() {
	int ret;
	ret = adc_mcp3008_init();
	retv_if(ret != 0, -1);
	flag = true;
	return ret;
}

void close_sensor_temp() {
	adc_mcp3008_fini();
	flag = false;
}

int read_sensor_temp(unsigned int *temp) {
	int ret;
	if (!flag)
		ret = open_sensor_temp();
	unsigned int t;
	ret = adc_mcp3008_read(MCP3008_CH_NUM, &t);
	retv_if(ret != 0, -1);
	t = (3 * t * 100) / 1024;
	_D("Temperature: %d", t);
	*temp = t;
	return ret;
}

bool set_sensor_temp_getting() {
	if (!flag) {
		_E("set_sensor_temp_getting] Temp Sensor is not opened.");
		return false;
	}

	get_timer = ecore_timer_add(DATA_GETTING_CYCLE, sensor_get_timed_cb, NULL);
	if (!get_timer)
		return false;
	return true;
}

void unset_sensor_temp_getting() {
	if (get_timer) {
		ecore_timer_del(get_timer);
		get_timer = NULL;
	}
}

static Eina_Bool sensor_get_timed_cb(void *data) {
	int ret;
	unsigned int temp = 0;
	ret = read_sensor_temp(&temp);
	retv_if(ret != 0, ECORE_CALLBACK_RENEW);
	return ECORE_CALLBACK_RENEW;
}
