/*
 * bluetooth-common.c
 *
 *  Created on: Aug 31, 2019
 *      Author: Seokjin Yoon
 */

#include "bluetooth-common.h"

bt_error_e ret;

void init_bluetooth() {
	ret = bt_initialize();
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_initialize] failed.");

		return;
	}
}
