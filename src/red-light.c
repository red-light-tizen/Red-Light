#include <tizen.h>
#include <service_app.h>
#include "red-light.h"

//-------for test--------
void testFunc(void* data) {
	_I("test func printed %lf ", *(double* )data);
}

void testFunc2(void* data) {
	_I("test func2 printed %lf ", 1000- *(double* )data);
}

void testfuncAdder() {
	static double data = 0.0;
	data += 10.0;

	registerEvent(double,data,testFunc,testFunc2);

}

//-------for test--------

bool service_app_create(void *data) {
	// Todo: add your code here.
	startEventDelivery();
	init_bluetooth();
	open_sensor_pulse();
	open_sensor_temp();
	open_sensor_gps();
	set_sensor_pulse_getting();
	set_sensor_temp_getting();
	set_sensor_gps_getting();
	_I("Created");
	return true;
}

void service_app_terminate(void *data) {
	// Todo: add your code here.
	deinit_bluetooth();
	unset_sensor_gps_getting();
	unset_sensor_temp_getting();
	unset_sensor_pulse_getting();
	close_sensor_gps();
	close_sensor_temp();
	close_sensor_pulse();
	finishEventDelivery();
	_I("terminated");
	return;
}

void service_app_control(app_control_h app_control, void *data) {
	// Todo: add your code here.
	int i;
	for(i = 0; i<100;++i)
		registerEvent(void,*NULL,testfuncAdder);

//	finishEventDelivery();
	return;
}

static void service_app_lang_changed(app_event_info_h event_info,
		void *user_data) {
	/*APP_EVENT_LANGUAGE_CHANGED*/
	return;
}

static void service_app_region_changed(app_event_info_h event_info,
		void *user_data) {
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void service_app_low_battery(app_event_info_h event_info,
		void *user_data) {
	/*APP_EVENT_LOW_BATTERY*/
}

static void service_app_low_memory(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[]) {
	char ad[50] = { 0, };
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = { NULL, };

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY],
			APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY],
			APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED],
			APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED],
			APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);

	return service_app_main(argc, argv, &event_callback, ad);
}
