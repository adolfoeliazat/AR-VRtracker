/*
 * adxl345.c
 *
 *  Created on: Apr 2, 2015
 *      Author: root
 */


#include "adxl345.h"
#include <LUFA/Drivers/Peripheral/TWI.h>

//function prefix: adxl345_

void adxl345_init(uint8_t mode, uint8_t range, uint8_t rate) {
		//Put the accelerometer in MEASURE mode
		if (TWI_StartTransmission(ALT_ADDR | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
		{
			TWI_SendByte(POWER_CTL);
			TWI_SendByte(mode); //(measure mode - 0001000 in binary)
			// Must stop transmission afterwards to release the bus
			TWI_StopTransmission();
		}

		//Set the Range to +/- 4G
		if (TWI_StartTransmission(0xA6 | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
		{
			TWI_SendByte(DATA_FORMAT);
			TWI_SendByte(range); //(4g - 0000001 in binary)
			// Must stop transmission afterwards to release the bus
			TWI_StopTransmission();
		}

		//set rate to 800 hz, max for 400khz i2c mode
		// Start a write session to device at device address 0x53, internal address 0x2C with a 10ms timeout
		if (TWI_StartTransmission(0xA6 | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
		{
			TWI_SendByte(BW_RATE);
			TWI_SendByte(rate);
			// Must stop transmission afterwards to release the bus
			TWI_StopTransmission();
		}
}

void adxl345_read(dataexchange_t *data) {
		uint8_t buf[6] = {0};

		// Start a read session to device at address 0xA6, internal address 0x32 with a 10ms timeout
		if (TWI_StartTransmission(ALT_ADDR | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
		{
			TWI_SendByte(DATAX0); //set pointer to X axis MSB
			TWI_StopTransmission();
			if (TWI_StartTransmission(ALT_ADDR | TWI_ADDRESS_READ, 10) == TWI_ERROR_NoError)
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
		(*data).acc_x = ((uint8_t)buf[1]) << 8;
		(*data).acc_x |= buf[0];

		(*data).acc_y = ((uint8_t)buf[3]) << 8;
		(*data).acc_y |= buf[2];

		(*data).acc_z = ((uint8_t)buf[5]) << 8;
		(*data).acc_z |= buf[4];
}

vector3d adxl345_raw_to_g(dataexchange_t *data, float scale) {
	vector3d ret;

	ret.x = (*data).acc_x * scale;
	ret.y = (*data).acc_y * scale;
	ret.z = (*data).acc_z * scale;

	return ret;
}
