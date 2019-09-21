/*
 * data-file.c
 *
 *  Created on: Sep 21, 2019
 *      Author: Seokjin Yoon
 */

#include "data-file.h"

void read_file(const char *filename, char *data) {
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		int error = get_last_result();
		_E("[fopen] failed. %d %s", error, get_error_message(error));
		return;
	}
	char *buf = malloc(sizeof(char) * ftell(fp));
	char line[1024];
	while (fgets(line, 1023, fp) != NULL) {
		_D("[read_file] %s", line);
		snprintf(buf + strlen(buf), strlen(line) + 1, "%s", line);
	}
//	_D("%s", buf);
	if (data != NULL)
		strncpy(data, buf, strlen(buf) + 1);
	free(buf);
	fclose(fp);
}

void write_file(const char *filename, const char *data) {
	FILE *fp = fopen(filename, "a");
	if (fp == NULL) {
		int error = get_last_result();
		_E("[fopen] failed. %d %s", error, get_error_message(error));
		return;
	}
	fputs(data, fp);
	_D("[write_file] %s", data);
	fclose(fp);
}
