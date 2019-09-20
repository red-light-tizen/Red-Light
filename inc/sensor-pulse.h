/*
 * sensor-pulse.h
 *
 *  Created on: Sep 19, 2019
 *      Author: Seokjin Yoon
 */

#ifndef SENSOR_PULSE_H_
#define SENSOR_PULSE_H_

#include "sensor.h"

#define DATA_GETTING_CYCLE 1.0

#define MAX30100_I2C_ADDRESS 0x57
#define MAX30100_INT_STATUS 0x00 // Which interrupts are tripped
#define MAX30100_INT_ENABLE 0x01 // Which interrupts are active
#define MAX30100_FIFO_WR_PTR 0x02 // Where data is being written
#define MAX30100_OVRFLOW_CTR 0x03 // Number of lost samples
#define MAX30100_FIFO_RD_PTR 0x04 // Where to read from
#define MAX30100_FIFO_DATA 0x05 // Ouput data buffer
#define MAX30100_MODE_CONFIG 0x06 // Control register
#define MAX30100_SPO2_CONFIG 0x07 // Oximetry settings
#define MAX30100_LED_CONFIG 0x09 // Pulse width and power of LEDs
#define MAX30100_TEMP_INTG 0x16 // Temperature value, whole number
#define MAX30100_TEMP_FRAC 0x17 // Temperature value, fraction
#define MAX30100_REV_ID 0xFE // Part revision
#define MAX30100_PART_ID 0xFF // Part ID, normally 0x11

peripheral_error_e open_sensor_pulse();
void close_sensor_pulse();
static void init_sensor_pulse();
peripheral_error_e read_sensor_pulse(int16_t *, int16_t *);
bool set_sensor_pulse_getting();
void unset_sensor_pulse_getting();
static Eina_Bool sensor_get_timed_cb(void *);

#endif /* SENSOR_PULSE_H_ */
