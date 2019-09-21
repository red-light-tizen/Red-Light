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
#include "condition-indicator.h"
#include "bluetooth-common.h"


#define DATA_SENDING_CYCLE 0.05
#define DATA_SENDING_CYCLE_HZ 1/6.0
#define DATA_SIZE 62

typedef struct bt_datatype{
	char message[DATA_SIZE];
	int length;
}bt_data;

void set_bluetooth_data_sending();
void unset_bluetooth_data_sending();
bt_error_e send_bluetooth_data(void *);
static Eina_Bool bluetooth_send_timed_cb(void *);
void create_data_packet(void*);

#endif /* BLUETOOTH_SENDER_H_ */
