/*
 * l3g4200d.c
 *
 *  Created on: Mar 30, 2015
 *      Author: root
 */

#include "l3g4200d.h"
#include <LUFA/Drivers/Peripheral/TWI.h>
#include <stdint.h>

//function prefix: l3g4200d_

void l3g4200d_init(uint8_t mode, uint8_t rate) {

	if (TWI_StartTransmission(DEV_ADDR | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
	{
		TWI_SendByte(CTRL_REG1);
		TWI_SendByte(rate);
		// Must stop transmission afterwards to release the bus
		TWI_StopTransmission();
	}

	if (TWI_StartTransmission(DEV_ADDR | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
	{
		TWI_SendByte(CTRL_REG5);
		TWI_SendByte(mode);
		// Must stop transmission afterwards to release the bus
		TWI_StopTransmission();
	}
}

uint8_t l3g4200d_read_byte(uint8_t dev_addr, uint8_t reg_addr) {
	uint8_t buf;

	// Start a read session to device at address 0xA0, internal address 0xDC with a 10ms timeout
	if (TWI_StartTransmission(dev_addr | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
	{
		TWI_SendByte(reg_addr); //set pointer to X axis MSB
		TWI_StopTransmission();
		if (TWI_StartTransmission(dev_addr | TWI_ADDRESS_READ, 10) == TWI_ERROR_NoError)
		{
			// Read 6 bytes, acknowledge after the third byte is received
			TWI_ReceiveByte(&buf, true);

			// Must stop transmission afterwards to release the bus
			TWI_StopTransmission();
		}
	}

	return buf;
}

void l3g4200d_read(dataexchange_t *data) {
	//can't read in sequence
	//note the order

	//check if new data available
	uint8_t status = l3g4200d_read_byte(DEV_ADDR, STATUS_REG);

	if ((status & (1<<3)) && (status & (1<<7))) { //new data available
		(*data).gyr_x = (l3g4200d_read_byte(DEV_ADDR, OUT_X_H)) << 8;
		(*data).gyr_x |= l3g4200d_read_byte(DEV_ADDR, OUT_X_L);

		(*data).gyr_y = (l3g4200d_read_byte(DEV_ADDR, OUT_Y_H)) << 8;
		(*data).gyr_y |= l3g4200d_read_byte(DEV_ADDR, OUT_Y_L);

		(*data).gyr_z = (l3g4200d_read_byte(DEV_ADDR, OUT_Z_H)) << 8;
		(*data).gyr_z |= l3g4200d_read_byte(DEV_ADDR, OUT_Z_L);
	}
}

void l3g4200d_read_seq(dataexchange_t *data) {
	uint8_t buf[6] = {0};

	// Start a read session to device at address 0xA6, internal address 0x32 with a 10ms timeout
	if (TWI_StartTransmission(DEV_ADDR | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
	{
		TWI_SendByte(OUT_X_L | BURST_MODE); //set pointer to X axis MSB and OR with burst modifier
		TWI_StopTransmission();
		if (TWI_StartTransmission(DEV_ADDR | TWI_ADDRESS_READ, 10) == TWI_ERROR_NoError)
		{
			// Read 6 bytes, acknowledge after the third byte is received
			TWI_ReceiveByte(&buf[0], false);
			TWI_ReceiveByte(&buf[1], false);
			TWI_ReceiveByte(&buf[2], false);

			TWI_ReceiveByte(&buf[3], false);
			TWI_ReceiveByte(&buf[4], false);
			TWI_ReceiveByte(&buf[5], true);
			// Must stop transmission afterwards to release the bus
			TWI_StopTransmission();
		}
	}

	//note the order
	(*data).gyr_x = ((uint8_t)buf[1]) << 8;
	(*data).gyr_x |= buf[0];

	(*data).gyr_y = ((uint8_t)buf[3]) << 8;
	(*data).gyr_y |= buf[2];

	(*data).gyr_z = ((uint8_t)buf[5]) << 8;
	(*data).gyr_z |= buf[4];
}

vector3d l3g4200d_raw_to_rad(dataexchange_t *data) {
	vector3d ret;

	ret.x = gyroToRad(((*data).gyr_x - OFFSET_X) * GYRO_SCALE); //offset?
	ret.y = gyroToRad(((*data).gyr_y - OFFSET_Y) * GYRO_SCALE);
	ret.z = gyroToRad(((*data).gyr_z - OFFSET_Z) * GYRO_SCALE);

	return ret;
}
