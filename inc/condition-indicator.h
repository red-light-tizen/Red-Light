/*
 * condition-indicator.h
 *
 *  Created on: Sep 19, 2019
 *      Author: 나상혁
 */

#ifndef CONDITION_INDICATOR_H_
#define CONDITION_INDICATOR_H_

#include "utils.h"
#include "event-bus.h"
#include "input-getter.h"
#include "bluetooth-sender.h"

#define STATUS_NORMAL 0
#define STATUS_CAUTION 1
#define STATUS_DANGER 2
#define STATUS_USERACTION 3

typedef struct condition_datatype {
	int status;

	int pulse;
	double temprature;
	double spo2;
	double activity;

	double latitude;
	double longitude;

} condition_data;

void checkCondition();
int evaluateStatus(body_data data);

#endif /* CONDITION_INDICATOR_H_ */
