/*
 * quaternion.h
 *
 *  Created on: Apr 3, 2015
 *      Author: root
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "ahrs/vector.h"
#include "ahrs/matrix.h"

//double is 4 bytes here
typedef struct quaternion {
	double w;
	double x;
	double y;
	double z;
} quaternion;

//function prefix: quaternion
void quaternion_norm(quaternion *q);
quaternion quaternion_product(quaternion q_a, quaternion q_b);
quaternion quaternion_from_axis_angle(vector3d axis, double angle);
quaternion quaternion_conjugate(quaternion q);
matrix3x3d quaternion_to_matrix(quaternion q);
quaternion quaternion_from_matrix(matrix3x3d *mat);


#endif /* QUATERNION_H_ */
