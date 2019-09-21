/*
 * sensor.c
 *
 *  Created on: Sep 19, 2019
 *      Author: Seokjin Yoon
 */

#include "sensor.h"

peripheral_error_e open_sensor_i2c(const int address, peripheral_i2c_h *i2c) {
	peripheral_error_e ret = PERIPHERAL_ERROR_NONE;
	ret = peripheral_i2c_open(I2C_BUS, address, i2c);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_open] failed. %d %s", ret, get_error_message(ret));
	} else {
		_I("[peripheral_i2c_open] succeeded.");
	}
	return ret;
}

void close_sensor_i2c(peripheral_i2c_h i2c) {
	peripheral_i2c_close(i2c);
}

peripheral_error_e read_sensor_i2c(peripheral_i2c_h i2c, uint8_t *data, uint32_t length) {
	peripheral_error_e ret;
	ret = peripheral_i2c_read(i2c, data, length);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_read] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

peripheral_error_e write_sensor_i2c(peripheral_i2c_h i2c, uint8_t *data, uint32_t length) {
	peripheral_error_e ret;
	ret = peripheral_i2c_write(i2c, data, length);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_write] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

peripheral_error_e read_sensor_i2c_register_byte(peripheral_i2c_h i2c, uint8_t reg, uint8_t *data) {
	peripheral_error_e ret;
	ret = peripheral_i2c_read_register_byte(i2c, reg, data);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_read_register_byte] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

peripheral_error_e write_sensor_i2c_register_byte(peripheral_i2c_h i2c, uint8_t reg, uint8_t data) {
	peripheral_error_e ret;
	ret = peripheral_i2c_write_register_byte(i2c, reg, data);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_write_register_byte] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

peripheral_error_e read_sensor_i2c_register_word(peripheral_i2c_h i2c, uint8_t reg, uint16_t *data) {
	peripheral_error_e ret;
	ret = peripheral_i2c_read_register_word(i2c, reg, data);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_read_register_word] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

peripheral_error_e write_sensor_i2c_register_word(peripheral_i2c_h i2c, uint8_t reg, uint16_t data) {
	peripheral_error_e ret;
	ret = peripheral_i2c_write_register_word(i2c, reg, data);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_i2c_write_register_word] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

bool writeBit(peripheral_i2c_h i2c, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
//  bcm2835_i2c_setSlaveAddress(devAddr);
  char sendBuf[256];
  char recvBuf[256];
  //first reading registery value
  sendBuf[0] = regAddr;
  uint8_t response = 0x00;
//  uint8_t response = bcm2835_i2c_write_read_rs(sendBuf, 1, recvBuf, 1 );
  if ( response == BCM2835_I2C_REASON_OK ) {
    uint8_t b = recvBuf[0] ;
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    sendBuf[1] = b ;
//    response = bcm2835_i2c_write(sendBuf, 2);
    peripheral_i2c_write(i2c, sendBuf, 2);
  }
  return response == BCM2835_I2C_REASON_OK;
}

bool writeBits(peripheral_i2c_h i2c, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
  //      010 value to write
  // 76543210 bit numbers
  //    xxx   args: bitStart=4, length=3
  // 00011100 mask byte
  // 10101111 original value (sample)
  // 10100011 original & ~mask
  // 10101011 masked | value
  //first reading registery value
  char sendBuf[256];
  char recvBuf[256];
  sendBuf[0] = regAddr;
  uint8_t response = 0x00;
//  uint8_t response = bcm2835_i2c_write_read_rs(sendBuf, 1, recvBuf, 1 );
  if ( response == BCM2835_I2C_REASON_OK ) {
    uint8_t b = recvBuf[0];
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1); // shift data into correct position
    data &= mask; // zero all non-important bits in data
    b &= ~(mask); // zero all important bits in existing byte
    b |= data; // combine data with existing byte
    sendBuf[1] = b ;
//    response = bcm2835_i2c_write(sendBuf, 2);
    	peripheral_i2c_write(i2c, sendBuf, 2);
    }
  return response == BCM2835_I2C_REASON_OK;
}

peripheral_error_e open_sensor_uart(peripheral_uart_h *uart) {
	peripheral_error_e ret;
	ret = peripheral_uart_open(UART_BUS, uart);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_uart_open] failed. %d %s", ret, get_error_message(ret));
	} else {
		_I("[peripheral_uart_open] succeeded.");
	}
	return ret;
}

void close_sensor_uart(peripheral_uart_h uart) {
	peripheral_uart_close(uart);
}

peripheral_error_e read_sensor_uart(peripheral_uart_h uart, uint8_t *data, uint32_t length) {
	peripheral_error_e ret;
	ret = peripheral_uart_read(uart, data, length);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_uart_read] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}

peripheral_error_e write_sensor_uart(peripheral_uart_h uart, uint8_t *data, uint32_t length) {
	peripheral_error_e ret;
	ret = peripheral_uart_write(uart, data, length);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("[peripheral_uart_write] failed. %d %s", ret, get_error_message(ret));
	}
	return ret;
}
