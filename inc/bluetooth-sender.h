/*
 * bluetooth-sender.h
 *
 *  Created on: Sep 2, 2019
 *      Author: Seokjin Yoon
 */

#ifndef BLUETOOTH_SENDER_H_
#define BLUETOOTH_SENDER_H_

#include <time.h>
#include <Ecore.h>
#include "bluetooth-common.h"

#define DATA_SENDING_CYCLE 1.0
#define DATA_SIZE 62

void set_bluetooth_data_sending();
void unset_bluetooth_data_sending();
bt_error_e send_bluetooth_data(const char *, int);
static Eina_Bool bluetooth_send_timed_cb(void *);
static void create_data_packet(char *);

#endif /* BLUETOOTH_SENDER_H_ */
