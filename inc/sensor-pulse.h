/*
 * sensor-pulse.h
 *
 *  Created on: Sep 19, 2019
 *      Author: Seokjin Yoon
 */

#ifndef SENSOR_PULSE_H_
#define SENSOR_PULSE_H_

#include "sensor.h"

#define DATA_GETTING_CYCLE 0.1

#define MAX30100_I2C_ADDRESS                    0x57
#define MAX30100_REG_INTERRUPT_STATUS           0x00
#define MAX30100_IS_PWR_RDY                     (1 << 0)
#define MAX30100_IS_SPO2_RDY                    (1 << 4)
#define MAX30100_IS_HR_RDY                      (1 << 5)
#define MAX30100_IS_TEMP_RDY                    (1 << 6)
#define MAX30100_IS_A_FULL                      (1 << 7)
#define MAX30100_REG_INTERRUPT_ENABLE           0x01
#define MAX30100_IE_ENB_SPO2_RDY                (1 << 4)
#define MAX30100_IE_ENB_HR_RDY                  (1 << 5)
#define MAX30100_IE_ENB_TEMP_RDY                (1 << 6)
#define MAX30100_IE_ENB_A_FULL                  (1 << 7)
#define MAX30100_REG_FIFO_WRITE_POINTER         0x02
#define MAX30100_REG_FIFO_OVERFLOW_COUNTER      0x03
#define MAX30100_REG_FIFO_READ_POINTER          0x04
#define MAX30100_REG_FIFO_DATA                  0x05
#define MAX30100_REG_MODE_CONFIGURATION         0x06
#define MAX30100_MC_TEMP_EN                     (1 << 3)
#define MAX30100_MC_RESET                       (1 << 6)
#define MAX30100_MC_SHDN                        (1 << 7)
#define MAX30100_MODE_HRONLY 0x02
#define MAX30100_MODE_SPO2_HR 0x03
#define MAX30100_REG_SPO2_CONFIGURATION         0x07
#define MAX30100_SPC_SPO2_HI_RES_EN             (1 << 6)
#define MAX30100_SAMPRATE_50HZ 0x00
#define MAX30100_SAMPRATE_100HZ 0x01
#define MAX30100_SAMPRATE_167HZ 0x02
#define MAX30100_SAMPRATE_200HZ 0x03
#define MAX30100_SAMPRATE_400HZ 0x04
#define MAX30100_SAMPRATE_600HZ 0x05
#define MAX30100_SAMPRATE_800HZ 0x06
#define MAX30100_SAMPRATE_1000HZ 0x07
#define MAX30100_SPC_PW_200US_13BITS 0x00
#define MAX30100_SPC_PW_400US_14BITS 0x01
#define MAX30100_SPC_PW_800US_15BITS 0x02
#define MAX30100_SPC_PW_1600US_16BITS 0x03
#define MAX30100_REG_LED_CONFIGURATION          0x09
#define MAX30100_LED_CURR_0MA 0x00
#define MAX30100_LED_CURR_4_4MA 0x01
#define MAX30100_LED_CURR_7_6MA 0x02
#define MAX30100_LED_CURR_11MA 0x03
#define MAX30100_LED_CURR_14_2MA 0x04
#define MAX30100_LED_CURR_17_4MA 0x05
#define MAX30100_LED_CURR_20_8MA 0x06
#define MAX30100_LED_CURR_24MA 0x07
#define MAX30100_LED_CURR_27_1MA 0x08
#define MAX30100_LED_CURR_30_6MA 0x09
#define MAX30100_LED_CURR_33_8MA 0x0a
#define MAX30100_LED_CURR_37MA 0x0b
#define MAX30100_LED_CURR_40_2MA 0x0c
#define MAX30100_LED_CURR_43_6MA 0x0d
#define MAX30100_LED_CURR_46_8MA 0x0e
#define MAX30100_LED_CURR_50MA 0x0f
#define MAX30100_REG_TEMPERATURE_DATA_INT       0x16
#define MAX30100_REG_TEMPERATURE_DATA_FRAC      0x17
#define MAX30100_REG_REVISION_ID                0xfe
#define MAX30100_REG_PART_ID                    0xff
#define MAX30100_FIFO_DEPTH                     0x10

peripheral_error_e open_sensor_pulse();
void close_sensor_pulse();
static void init_sensor_pulse();
peripheral_error_e read_sensor_pulse(uint16_t *, uint16_t *);
bool set_sensor_pulse_getting();
void unset_sensor_pulse_getting();
static Eina_Bool sensor_get_timed_cb(void *);

#endif /* SENSOR_PULSE_H_ */
