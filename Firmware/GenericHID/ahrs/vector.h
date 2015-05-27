/*
 * vector.h
 *
 *  Created on: Apr 3, 2015
 *      Author: root
 */

#ifndef VECTOR_H_
#define VECTOR_H_

//double is 4 bytes here
typedef struct vector3d {
	double x;
	double y;
	double z;
} vector3d;

//function prefix: vector_

double vector_magnitude(vector3d v);
vector3d vector_sum(vector3d v_a, vector3d v_b);
vector3d vector_dif(vector3d v_a, vector3d v_b);
void vector_norm(vector3d *v);
vector3d vector_inv(vector3d v);
vector3d vector_scale(vector3d v, double scalar);
double vector_dot(vector3d v_a, vector3d v_b);
vector3d vector_cross(vector3d v_a, vector3d v_b);


#endif /* VECTOR_H_ */
