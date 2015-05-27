/*
 * hmc5883l.c
 *
 *  Created on: Mar 30, 2015
 *      Author: root
 */
#include "hmc5883l.h"
#include "ahrs/matrix.h"
#include <LUFA/Drivers/Peripheral/TWI.h>
#include <stdint.h>

//function prefix: hmc5883l_

void hmc5883l_init(uint8_t mode, uint8_t gain, uint8_t rate){
	// Start a write session to device at device address 0x3C, internal address 0x00 with a 10ms timeout
	if (TWI_StartTransmission(MAG_ADDR | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
	{
		TWI_SendByte(REG_CONFIG_A);
		TWI_SendByte(rate);
		// Must stop transmission afterwards to release the bus
		TWI_StopTransmission();
	}

	// Start a write session to device at device address 0x3C, internal address 0x01 with a 10ms timeout
	if (TWI_StartTransmission(MAG_ADDR | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
	{
		TWI_SendByte(REG_CONFIG_B);
		TWI_SendByte(gain);
		// Must stop transmission afterwards to release the bus
		TWI_StopTransmission();
	}

	// Start a write session to device at device address 0x3C, internal address 0x02 with a 10ms timeout
	if (TWI_StartTransmission(MAG_ADDR| TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
	{
		TWI_SendByte(REG_MODE);
		TWI_SendByte(mode);
		// Must stop transmission afterwards to release the bus
		TWI_StopTransmission();
	}
}

void hmc5883l_read(dataexchange_t *data) {
	uint8_t buf[6] = {0};

	if (TWI_StartTransmission(MAG_ADDR | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError)
	{
		TWI_SendByte(REG_OUT_X_M); //set pointer to X axis MSB
		TWI_StopTransmission();
		if (TWI_StartTransmission(MAG_ADDR | TWI_ADDRESS_READ, 10) == TWI_ERROR_NoError)
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

	(*data).mag_x = ((uint8_t)buf[0]) << 8;
	(*data).mag_x |= buf[1];

	(*data).mag_z = ((uint8_t)buf[2]) << 8;
	(*data).mag_z |= buf[3];

	(*data).mag_y = ((uint8_t)buf[4]) << 8;
	(*data).mag_y |= buf[5];
}

void hmc5883l_applyCalibration(vector3d *vec, params_t *calib_params) {
	//create vectors
	vector3d offset_v, scale_v;
	matrix3x3d rmat;

	offset_v.x = (*calib_params).offset_x;
	offset_v.y = (*calib_params).offset_y;
	offset_v.z = (*calib_params).offset_z;

	scale_v.x = (*calib_params).scale_x;
	scale_v.y = (*calib_params).scale_y;
	scale_v.z = (*calib_params).scale_z;

	rmat.r1[0] = (*calib_params).rmat_r1_x;
	rmat.r1[1] = (*calib_params).rmat_r1_y;
	rmat.r1[2] = (*calib_params).rmat_r1_z;

	rmat.r2[0] = (*calib_params).rmat_r2_x;
	rmat.r2[1] = (*calib_params).rmat_r2_y;
	rmat.r2[2] = (*calib_params).rmat_r2_z;

	rmat.r3[0] = (*calib_params).rmat_r3_x;
	rmat.r3[1] = (*calib_params).rmat_r3_y;
	rmat.r3[2] = (*calib_params).rmat_r3_z;

	//removing offset
	(*vec).x -= offset_v.x;
	(*vec).y -= offset_v.y;
	(*vec).z -= offset_v.z;

	//applying rotation
	(*vec) = matrix_vector_prod(&rmat, (*vec));

	//scaling
	(*vec).x *= scale_v.x;
	(*vec).y *= scale_v.y;
	(*vec).z *= scale_v.z;

	//applying backward rotation - transposed
	matrix3x3d rmat_back = matrix_transpose(&rmat);
	(*vec) = matrix_vector_prod(&rmat_back, (*vec));

}
