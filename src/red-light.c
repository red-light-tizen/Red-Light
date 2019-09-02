#include <tizen.h>
#include <service_app.h>
#include "red-light.h"

extern bt_error_e ret;

bool service_app_create(void *data)
{
    // Todo: add your code here.
	init_bluetooth();
	if (get_bluetooth_adapter_state()) {
		char *local_name, *local_address;
		bt_adapter_set_name("Red Light");
		bt_adapter_get_name(&local_name);
		_I("Bluetooth adapter name: %s", local_name);
		if (local_name)
			free(local_name);
		bt_adapter_get_address(&local_address);
		_I("Bluetooth adapter address: %s", local_address);
		if (local_address)
			free(local_address);
		get_bluetooth_adapter_visibility(1);
	}
    return true;
}

void service_app_terminate(void *data)
{
    // Todo: add your code here.
    return;
}

void service_app_control(app_control_h app_control, void *data)
{
    // Todo: add your code here.
	ret = bt_adapter_set_state_changed_cb(adapter_state_changed_cb, NULL);
	if (ret != BT_ERROR_NONE)
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_set_state_changed_cb()] failed.");

	ret = bt_adapter_set_visibility_mode_changed_cb(adapter_visibility_mode_changed_cb, NULL);
	if (ret != BT_ERROR_NONE)
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_set_visibility_mode_changed_cb] failed.");
    return;
}

static void
service_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	return;
}

static void
service_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
service_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
service_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[])
{
    char ad[50] = {0,};
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);

	return service_app_main(argc, argv, &event_callback, ad);
}
