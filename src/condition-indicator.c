/*
 * condition-indicator.c
 *
 *  Created on: Sep 19, 2019
 *      Author: 나상혁
 */

#include "condition-indicator.h"


void checkCondition(){
	 body_data bodydata = get_sensor_data();
	 int status = evaluateStatus(bodydata);
	 location_data location = get_location();

	 condition_data condition;
	 condition.status = status;
	 condition.activity = bodydata.activity;
	 condition.pulse = bodydata.pulse;
	 condition.temprature = bodydata.temprature;
	 condition.spo2 = bodydata.spo2;
	 condition.latitude = location.latitude;
	 condition.longitude = location.longitude;

	 registerEvent(condition_data, condition, create_data_packet);
//	 registerEvent(void,*NULL,checkCondition);
}


int evaluateStatus(body_data data){
	//TODO : Calculate condition.

	return 3;
}
