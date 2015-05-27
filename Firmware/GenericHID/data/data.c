/*
 * data.c
 *
 *  Created on: Apr 3, 2015
 *      Author: root
 */
#include "GenericHID.h"
#include "data/data.h"
#include "sensors/hmc5883l.h"
#include <avr/eeprom.h>

void data_init(dataexchange_t *data, params_t *calib_params) {
	//put some values into structure for testing

	(*data).qr.w = 1;
	(*data).qr.x = 1;
	(*data).qr.y = 1;
	(*data).qr.z = 1;

	(*data).mag_x = 123;
	(*data).mag_y = 124;
	(*data).mag_z = 125;

	(*data).acc_x = 223;
	(*data).acc_y = 224;
	(*data).acc_z = 225;

	(*data).gyr_x = 323;
	(*data).gyr_y = 324;
	(*data).gyr_z = 325;

	(*data).time_period = 326;

	(*data).btn = 6; //110 in binary - second and third buttons pressed

	//read first byte from eeprom
	uint8_t ind;
	ind = eeprom_read_float(0x00);

	(*data).d1 = ind;

	if (ind == 7) {

		uint8_t tmp_data[sizeof(params_t)];
		uint8_t *addr = 0x00;

		for (uint8_t i = 0; i < sizeof(params_t); i++) {
			tmp_data[i] = eeprom_read_byte((void *)addr);
			addr++;
		}

		memcpy(calib_params, &tmp_data, sizeof(params_t));

	} else {

		(*calib_params).indicator = 13;

		(*calib_params).offset_x = M_OFFSET_X;
		(*calib_params).offset_y = M_OFFSET_Y;
		(*calib_params).offset_z = M_OFFSET_Z;

		(*calib_params).rmat_r1_x = R1_X;
		(*calib_params).rmat_r1_y = R1_Y;
		(*calib_params).rmat_r1_z = R1_Z;

		(*calib_params).rmat_r2_x = R2_X;
		(*calib_params).rmat_r2_y = R2_Y;
		(*calib_params).rmat_r2_z = R2_Z;

		(*calib_params).rmat_r3_x = R3_X;
		(*calib_params).rmat_r3_y = R3_Y;
		(*calib_params).rmat_r3_z = R3_Z;

		(*calib_params).scale_x = SCALE_X;
		(*calib_params).scale_y = SCALE_Y;
		(*calib_params).scale_z = SCALE_Z;
	}

}
