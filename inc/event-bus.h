/*
 * event-bus.h
 *
 *  Created on: Sep 11, 2019
 *      Author: 나상혁
 */

#ifndef EVENT_BUS_H_
#define EVENT_BUS_H_
#include <stdlib.h>
#include <stdarg.h>
#include <Ecore.h>
#include "utils.h"

#define MAX_SUBS_PER_EVENT 5

typedef struct eventType{
	void (*subsicriber[MAX_SUBS_PER_EVENT])(void*);
	int subsNumber;
	void* data;
}r_event;

void startEventDelivery();
void finishEventDelivery();

int _register_Event(size_t datalen, void* data , int funcCount, ...);

typedef void(*fp_rvpv)(void*);

#pragma GCC diagnostic ignored "-Wint-conversion"
#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))

/**
 * @param dataType 	: Type name of data. If data is string, set to data name.
 * @param data 		: Data for functions. (NOT POINTER, NOT R-VALUE)
 * @param ... 		: Function pointers to execute.(MAX = MAX_SUBS_PER_EVENT)
 *
 * @return int 		: ID number of event. If registering is fail, returns -1;
 *
 * @note : If you don't want pass data, set data to "*NULL".
 *
 *
 * @usage : registerEvent(double, data, functions...);
 */
#define registerEvent(dataType,data,...) _register_Event(sizeof(dataType), &(data), NUMARGS(__VA_ARGS__), __VA_ARGS__)

#endif /* EVENT_BUS_H_ */
