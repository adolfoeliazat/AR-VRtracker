/*
 * matrix.h
 *
 *  Created on: Apr 3, 2015
 *      Author: root
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "ahrs/vector.h"
#include <stdint.h>

//double is 4 bytes here
typedef struct matrix3x3d {
	double r1[3];
	double r2[3];
	double r3[3];
} matrix3x3d;

//function prefix: matrix
vector3d matrix_row_to_vector(matrix3x3d *mat, uint8_t row_num);
void matrix_vector_to_row(matrix3x3d *mat, vector3d v, uint8_t row_num);
vector3d matrix_vector_prod(matrix3x3d *mat, vector3d v);
matrix3x3d matrix_transpose(matrix3x3d *mat);
//vector3d matrix_col_to_vector(uint8_t col_num)


#endif /* MATRIX_H_ */
