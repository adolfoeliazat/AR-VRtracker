/*
 * data.h
 *
 *  Created on: Apr 3, 2015
 *      Author: root
 */

#ifndef DATA_H_
#define DATA_H_

#include <stdint.h>
#include "ahrs/quaternion.h"
#include "ahrs/vector.h"

typedef struct dataexchange {
	quaternion qr; //16 bytes, rotation quaternion
	int16_t mag_x, mag_y, mag_z; //6 bytes
	int16_t acc_x, acc_y, acc_z;
	int16_t gyr_x, gyr_y, gyr_z;
	float time_period; //4 bytes
    float d1, d2, d3, d4, d5;
    uint8_t d6;
	uint8_t btn; //1 bytes
} dataexchange_t;

typedef struct params {
	float indicator;
	float offset_x, offset_y, offset_z;
	float rmat_r1_x, rmat_r1_y, rmat_r1_z;
	float rmat_r2_x, rmat_r2_y, rmat_r2_z;
	float rmat_r3_x, rmat_r3_y, rmat_r3_z;
	float scale_x, scale_y, scale_z;
} params_t; //64 bytes

void data_init(dataexchange_t*, params_t*);

#endif /* DATA_H_ */
