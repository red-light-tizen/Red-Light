/*
 * data-storage.h
 *
 *  Created on: Sep 21, 2019
 *      Author: Seokjin Yoon
 */

#ifndef DATA_STORAGE_H_
#define DATA_STORAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <storage.h>
#include "utils.h"

static bool storage_cb(int storage_id, storage_type_e type, storage_state_e state, const char *path, void *user_data);
void get_storage_directory(char *);
storage_type_e get_storage_type();
bool get_storage_state();
storage_error_e set_storage_state_changed();
storage_error_e unset_storage_state_changed();
static void storage_changed(int storage_id, storage_state_e state, void *user_data);
double get_storage_total_space();
double get_storage_available_space();
double get_storage_used_space();
double Byte2MiB(unsigned long long);

#endif /* DATA_STORAGE_H_ */
