/*
 * data-storage.c
 *
 *  Created on: Sep 21, 2019
 *      Author: Seokjin Yoon
 */

#include "data-storage.h"

static int internal_storage_id;

static bool storage_cb(int storage_id, storage_type_e type, storage_state_e state, const char *path, void *user_data) {
    if (type == STORAGE_TYPE_INTERNAL) {
        internal_storage_id = storage_id;

        return false;
    }

    return true;
}


void get_storage_directory(char *path) {
	int error;
	char *p;
	error = storage_get_directory(internal_storage_id, STORAGE_DIRECTORY_IMAGES, &p);
	if (error != STORAGE_ERROR_NONE) {
		_E("[storage_get_directory] failed. %d %s", error, get_error_message(error));
		return;
	}
	_I("[storage_get_directory] %s", p);
	if (path != NULL)
		strncpy(path, p, strlen(p) + 1);
	free(p);
}

storage_type_e get_storage_type() {
	int error;
	storage_type_e type;
	error = storage_get_type(internal_storage_id, &type);
	if (error != STORAGE_ERROR_NONE) {
		_E("[storage_get_type] failed. %d %s", error, get_error_message(error));
		return error;
	}
	switch (type) {
	case STORAGE_TYPE_INTERNAL:
		_I("Internal device storage (built-in storage in a device, non-removable)");
		break;
	case STORAGE_TYPE_EXTERNAL:
		_I("External storage");
		break;
	case STORAGE_TYPE_EXTENDED_INTERNAL:
		_I("Extended internal storage (External storage used as internal storage)");
		break;
	}
	return type;
}

bool get_storage_state() {
	int error;
	storage_state_e state;
	error = storage_get_state(internal_storage_id, &state);
	if (error != STORAGE_ERROR_NONE) {
		_E("[storage_get_state] failed. %d %s", error, get_error_message(error));
		return false;
	}
	switch (state) {
	case STORAGE_STATE_UNMOUNTABLE:
		_I("Storage is present but cannot be mounted. Typically it happens if the file system of the storage is corrupted");
		break;
	case STORAGE_STATE_REMOVED:
		_I("Storage is not present");
		break;
	case STORAGE_STATE_MOUNTED:
		_I("Storage is present and mounted with read/write access");
		return true;
	case STORAGE_STATE_MOUNTED_READ_ONLY:
		_I("Storage is present and mounted with read only access");
		break;
	}
	return false;
}

storage_error_e set_storage_state_changed() {
	storage_error_e error;
	error = storage_set_state_changed_cb(internal_storage_id, storage_changed, NULL);
	return error;
}

storage_error_e unset_storage_state_changed() {
	storage_error_e error;
	error = storage_unset_state_changed_cb(internal_storage_id, storage_changed);
	return error;
}

static void storage_changed(int storage_id, storage_state_e state, void *user_data) {
    if (storage_id != internal_storage_id)
        return;
    dlog_print(DLOG_DEBUG, LOG_TAG, "state changed to %d", state);
}

double get_storage_total_space() {
	int error;
	unsigned long long bytes;
	error = storage_get_total_space(internal_storage_id, &bytes);
	if (error != STORAGE_ERROR_NONE) {
		_E("[storage_get_total_space] failed. %d %s", error, get_error_message(error));
		return 0;
	}
	double mib = Byte2MiB(bytes);
	_I("Total space: %fMiB", mib);
	return mib;
}

double get_storage_available_space() {
	int error;
	unsigned long long bytes;
	error = storage_get_available_space(internal_storage_id, &bytes);
	if (error != STORAGE_ERROR_NONE) {
		_E("[storage_get_available_space] failed. %d %s", error, get_error_message(error));
		return 0;
	}
	double mib = Byte2MiB(bytes);
	_I("Available space: %fMiB", mib);
	return bytes;
}

double get_storage_used_space() {
	unsigned long long total, available;
	storage_get_total_space(internal_storage_id, &total);
	storage_get_available_space(internal_storage_id, &available);
	unsigned long long used = total - available;
	double mib = Byte2MiB(used);
	_I("Used space: %fMiB", mib);
	return mib;
}

double Byte2MiB(unsigned long long byte) {
	return (double)byte / 1048576;
}
