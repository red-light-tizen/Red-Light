/*
 * bluetooth-sender.h
 *
 *  Created on: Sep 2, 2019
 *      Author: Seokjin Yoon
 */

#ifndef BLUETOOTH_SENDER_H_
#define BLUETOOTH_SENDER_H_

#include <Ecore.h>
#include "bluetooth-common.h"

#define DATA_SENDING_CYCLE 1.0

void set_bluetooth_data_sending();
void unset_bluetooth_data_sending();
void send_bluetooth_data(const char *, int);
Eina_Bool bluetooth_send_timed_cb(void *);

#endif /* BLUETOOTH_SENDER_H_ */
