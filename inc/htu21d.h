/*
 * htu21d.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Gordon
 */

#ifndef HTU21D_H_
#define HTU21D_H_

#include <stdint.h>

#define HTU21D_I2C_SLAVE_ADDR 					0x40

#define HTU21D_I2C_TRIG_TEMP_MEAS_CMD_HLD		0xE3
#define HTU21D_I2C_TRIG_HUM_MEAS_CMD_HLD		0xE5
#define HTU21D_I2C_TRIG_TEMP_MEAS_CMD_NO_HLD	0xF3
#define HTU21D_I2C_TRIG_HUM_MEAS_CMD_NO_HLD		0xF5

#define HTU21D_I2C_WRITE_USR_REG_CMD			0xE6
#define HTU21D_I2C_READ_USR_REG_CMD				0xE6

#define HTU21D_I2C_SOFT_RESET_CMD				0xFE

static inline float htu21d_convert_temperature(uint32_t data)
{
	data = (data & 0xffff) >> 2;
	uint16_t corrected_data = ((data & 0xff00) >> 8) | ((data & 0xff) << 8);

	return (175.72 * ((float)corrected_data / (1 << 16))) - 46.85;
}

static inline float htu21d_convert_humidity(uint32_t data)
{
	data >>= (data & 0xffff) >> 2;
	uint16_t corrected_data = ((data & 0xff00) >> 8) | ((data & 0xff) << 8);

	return (125 * ((float)corrected_data / (1 << 16))) - 6;
}

#endif /* HTU21D_H_ */
