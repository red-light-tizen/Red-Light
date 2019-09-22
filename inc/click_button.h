/*
 * click_button.h
 *
 *  Created on: Sep 22, 2019
 *      Author: 나상혁
 */

#ifndef CLICK_BUTTON_H_
#define CLICK_BUTTON_H_
#include "utils.h"
#include "as-log.h"
#include <peripheral_io.h>
#include <stdlib.h>

typedef enum {
	MOTION_HANDLE_ERROR_NONE = 0,
	MOTION_HANDLE_ERROR_NOT_OPEN,
	MOTION_HANDLE_ERROR_INVALID_PIN
} resource_click_button_handle_error_e;

extern int resource_read_click_button(int pin_num, uint32_t *out_value);

extern void resource_close_click_button(void);

#endif /* CLICK_BUTTON_H_ */
