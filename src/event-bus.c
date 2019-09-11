/*
 * event-bus.c
 *
 *  Created on: Sep 11, 2019
 *      Author: 나상혁
 */
#include "event-bus.h"

static const int eventMax = 100000;

static r_event* events[eventMax] = { 0 };
static Ecore_Idler *bus_excutor;

r_event* create_Event(void (*subs)(void*), int delayMill, void* data, size_t datalen) {
	r_event *event = malloc(sizeof(r_event));

	event->subsicriber = subs;

	event->data = malloc(datalen);
	memcpy(event->data , data, datalen);
	event->expireTime = clock() + delayMill;

	return event;
}

int registerEvent(r_event *event) {
	for (int i = 0; i < eventMax; ++i) {
		if (events[i] == NULL) {
			events[i] = event;
			return 1;
		}
	}
	_E("EventQueue is full");
	return 0;
}

Eina_Bool delivery() {
	long currentTime = clock();
//	_I("current %ld", currentTime);
	for (int i = 0; i < eventMax; ++i) {

		if (events[i]) {
			if (events[i]->expireTime < currentTime) {
				events[i]->subsicriber(events[i]->data);
				free(events[i]->data);
				events[i]->data = NULL;
				free(events[i]);
				events[i] = NULL;
			}
		}

	}
	return ECORE_CALLBACK_RENEW;
}

void startEventDelivery() {
	bus_excutor = ecore_idler_add(delivery, NULL);
	if (!bus_excutor) {
		_E("Error on StartDelivery");
		return;
	}
}

void stopEventDelivery() {
	ecore_idler_del(bus_excutor);
}
