/*
 * matrix.c
 *
 *  Created on: Apr 3, 2015
 *      Author: root
 */

#include "ahrs/matrix.h"

vector3d matrix_row_to_vector(matrix3x3d *mat, uint8_t row_num) {
	vector3d ret;
	switch (row_num) {
	case 1:
		ret.x = (*mat).r1[0];
		ret.y = (*mat).r1[1];
		ret.z = (*mat).r1[2];
		break;
	case 2:
		ret.x = (*mat).r2[0];
		ret.y = (*mat).r2[1];
		ret.z = (*mat).r2[2];
		break;
	case 3:
		ret.x = (*mat).r3[0];
		ret.y = (*mat).r3[1];
		ret.z = (*mat).r3[2];
		break;
	}
	return ret;
}

void matrix_vector_to_row(matrix3x3d *mat, vector3d vec, uint8_t row_num) {
	switch (row_num) {
	case 1:
		(*mat).r1[0] = vec.x;
		(*mat).r1[1] = vec.y;
		(*mat).r1[2] = vec.z;
		break;
	case 2:
		(*mat).r2[0] = vec.x;
		(*mat).r2[1] = vec.y;
		(*mat).r2[2] = vec.z;
		break;
	case 3:
		(*mat).r3[0] = vec.x;
		(*mat).r3[1] = vec.y;
		(*mat).r3[2] = vec.z;
		break;
	}
}

vector3d matrix_vector_prod(matrix3x3d *mat, vector3d v) {
	vector3d res;

	res.x = ((*mat).r1[0] * v.x) + ((*mat).r1[1] * v.y) + ((*mat).r1[2] * v.z);
	res.y = ((*mat).r2[0] * v.x) + ((*mat).r2[1] * v.y) + ((*mat).r2[2] * v.z);
	res.z = ((*mat).r3[0] * v.x) + ((*mat).r3[1] * v.y) + ((*mat).r3[2] * v.z);

	return res;
}

matrix3x3d matrix_transpose(matrix3x3d *mat) {
	matrix3x3d res;

	res.r1[0] = (*mat).r1[0];
	res.r1[1] = (*mat).r2[0];
	res.r1[2] = (*mat).r3[0];

	res.r2[0] = (*mat).r1[1];
	res.r2[1] = (*mat).r2[1];
	res.r2[2] = (*mat).r3[1];

	res.r3[0] = (*mat).r1[2];
	res.r3[1] = (*mat).r2[2];
	res.r3[2] = (*mat).r3[2];

	return res;
}
