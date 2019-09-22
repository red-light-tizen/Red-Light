/*
 * event-bus.c
 *
 *  Created on: Sep 11, 2019
 *      Author: 나상혁
 */
#include "event-bus.h"

#define eventMax 100000

static void stopEventDelivery();
static r_event* create_rEvent(size_t datalen, void* data);

static r_event* events[eventMax] = { 0 };
static Ecore_Idler *bus_excutor;
static int registrable;
static int activeEvent = 0;

r_event* create_rEvent(size_t datalen, void* data) {
	r_event *event = malloc(sizeof(r_event));
	event->data = malloc(datalen);
	event->subsNumber = 0;
	if(data)
		memcpy(event->data, data, datalen);
	return event;
}

r_event* addSubs(r_event* event, void (*subs)(void*)) {
	event->subsicriber[event->subsNumber] = subs;
	++(event->subsNumber);
	return event;
}

int _register_Event(size_t datalen, void* data, int funcCount, ...) {

	if(!registrable){
		_I("RegisterEvent : Registering is rejected.");
		return -1;
	}

	if (funcCount > MAX_SUBS_PER_EVENT) {
		_Fail("RegisterEvent : Too much Number of functions.");
		return -1;

	}

	va_list args;
	va_start(args, funcCount);

	r_event* event = create_rEvent(datalen, data);

	for (int i = 0; i < funcCount; ++i) {
		fp_rvpv subs;
		subs = va_arg(args, fp_rvpv);
		addSubs(event, subs);
	}

	va_end(args);

	for (int i = 0; i < eventMax; ++i) {
		if (events[i] == NULL) {
			events[i] = event;
			return i;
		}
	}
	_I("Event bus is full!");
	return -1;
}

Eina_Bool delivery() {
	for (int i = 0; i < eventMax; ++i) {

		if (events[i] != NULL) {
			for (int j = 0; j < events[i]->subsNumber; ++j) {
				events[i]->subsicriber[j](events[i]->data);
			}
			free(events[i]->data);
			events[i]->data = NULL;

			free(events[i]);
			events[i] = NULL;
			--activeEvent;
		}

	}

	if (!registrable && activeEvent <= 0) {
		_I("%d",activeEvent);
		stopEventDelivery();
		return ECORE_CALLBACK_CANCEL;
	}
	return ECORE_CALLBACK_RENEW;
}

void startEventDelivery() {
	registrable = 1;
	++activeEvent;
	_I("EventBus is Started");
	bus_excutor = ecore_idler_add(delivery, NULL);
	if (!bus_excutor) {
		_E("Error on StartDelivery");
		return;
	}
}

void stopEventDelivery() {
	_I("EventBus is Stopped");
	ecore_idler_del(bus_excutor);
}

void finishEventDelivery() {
	_I("EventBus is close");
	registrable = 0;
}
