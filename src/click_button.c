/*
 * click_button.c
 *
 *  Created on: Sep 22, 2019
 *      Author: 나상혁
 */

#include "click_button.h"

static peripheral_gpio_h g_sensor_h = NULL;
static int g_pin_num = -1;

int _resource_validate_click_button(int pin_num)
{
	int ret = MOTION_HANDLE_ERROR_NONE;

	if (!g_sensor_h)
	{
		ret = MOTION_HANDLE_ERROR_NOT_OPEN;
	} else if (g_pin_num != pin_num) {
		ret = MOTION_HANDLE_ERROR_INVALID_PIN;
	}

	return ret;
}

int resource_open_click_button(int pin_num)
{
	peripheral_gpio_h temp = NULL;

	int ret = peripheral_gpio_open(pin_num, &temp);
	if (ret) {
		peripheral_gpio_close(temp);
		_E("peripheral_gpio_open failed.");
		return -1;
	}

	ret = peripheral_gpio_set_direction(temp, PERIPHERAL_GPIO_DIRECTION_IN);
	if (ret) {
		peripheral_gpio_close(temp);
		_E("peripheral_gpio_set_direction failed.");
		return -1;
	}

	g_sensor_h = temp;
	g_pin_num = pin_num;

	return 0;
}

int resource_read_click_button(int pin_num, uint32_t *out_value)
{
	int ret = PERIPHERAL_ERROR_NONE;

	if (_resource_validate_click_button(pin_num) == MOTION_HANDLE_ERROR_NOT_OPEN) {
		ret = resource_open_click_button(pin_num);
		retv_if(ret < 0, -1);
	}
	if (_resource_validate_click_button(pin_num) == MOTION_HANDLE_ERROR_INVALID_PIN) {
		_E("Invalid pin number.");
		return -1;
	}

	ret = peripheral_gpio_read(g_sensor_h, out_value);
	retv_if(ret < 0, -1);

	return 0;
}

void resource_close_click_button(void)
{
	if (!g_sensor_h) return;

	_I("Click Button is finishing...");

	peripheral_gpio_close(g_sensor_h);

	g_pin_num = -1;
}
