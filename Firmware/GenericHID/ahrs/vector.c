/*
 * vector.c
 *
 *  Created on: Apr 3, 2015
 *      Author: root
 */

#include "ahrs/vector.h"
#include <math.h>

double vector_magnitude(vector3d v) {
	return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vector3d vector_sum(vector3d v_a, vector3d v_b) {
	vector3d ret;

	ret.x = v_a.x + v_b.x;
	ret.y = v_a.y + v_b.y;
	ret.z = v_a.z + v_b.z;

	return ret;
}

vector3d vector_dif(vector3d v_a, vector3d v_b) {
	vector3d ret;

	ret.x = v_a.x - v_b.x;
	ret.y = v_a.y - v_b.y;
	ret.z = v_a.z - v_b.z;

	return ret;
}

void vector_norm(vector3d *v) {
	double mag = vector_magnitude((*v));
	if(mag != 0)
	{
		(*v).x = (*v).x/mag;
		(*v).y = (*v).y/mag;
		(*v).z = (*v).z/mag;
	}
}

vector3d vector_inv(vector3d v) {
	vector3d ret;
	ret.x = v.x * (-1);
	ret.y = v.y * (-1);
	ret.z = v.z * (-1);
	return ret;
}

vector3d vector_scale(vector3d v, double scalar) {
	vector3d ret;
	ret.x = v.x * scalar;
	ret.y = v.y * scalar;
	ret.z = v.z * scalar;
	return ret;
}

double vector_dot(vector3d v_a, vector3d v_b) {
	return (v_a.x * v_b.x) + (v_a.y * v_b.y) + (v_a.z * v_b.z);
}

vector3d vector_cross(vector3d v_a, vector3d v_b) {
	vector3d ret;
	ret.x = (v_a.y * v_b.z) - (v_a.z * v_b.y);
	ret.y = (v_a.z * v_b.x) - (v_a.x * v_b.z);
	ret.z = (v_a.x * v_b.y) - (v_a.y * v_b.x);
	return ret;
}
