/*
 * bluetooth-receiver.h
 *
 *  Created on: Sep 2, 2019
 *      Author: Seokjin Yoon
 */

#ifndef BLUETOOTH_RECEIVER_H_
#define BLUETOOTH_RECEIVER_H_

#include "bluetooth-common.h"

void socket_data_received_cb(bt_socket_received_data_s* data, void* user_data);

#endif /* BLUETOOTH_RECEIVER_H_ */
