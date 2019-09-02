/*
 * bluetooth-receiver.c
 *
 *  Created on: Sep 2, 2019
 *      Author: Seokjin Yoon
 */

#include "bluetooth-receiver.h"

void socket_data_received_cb(bt_socket_received_data_s* data, void* user_data) {
    if (data == NULL) {
        dlog_print(DLOG_INFO, LOG_TAG, "No received data!");

        return;
    }
    dlog_print(DLOG_INFO, LOG_TAG, "Socket fd: %d", data->socket_fd);
    dlog_print(DLOG_INFO, LOG_TAG, "Data: %s", data->data);
    dlog_print(DLOG_INFO, LOG_TAG, "Size: %d", data->data_size);
}
