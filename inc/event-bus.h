/*
 * event-bus.h
 *
 *  Created on: Sep 11, 2019
 *      Author: 나상혁
 */

#ifndef EVENT_BUS_H_
#define EVENT_BUS_H_
#include <stdlib.h>
#include <Ecore.h>
#include "utils.h"

typedef struct eventType{
	long expireTime;
	void (*subsicriber)(void*);
	void* data;
}r_event;

void startEventDelivery();
void stopEventDelivery();

r_event* create_Event(void (*)(void*), int delayMill, void* data, size_t datalen);
int registerEvent(r_event *event);

#endif /* EVENT_BUS_H_ */
