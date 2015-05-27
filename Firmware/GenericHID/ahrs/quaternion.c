/*
 * quaternion.c
 *
 *  Created on: Apr 3, 2015
 *      Author: root
 */

#include "ahrs/quaternion.h"
#include <math.h>

void quaternion_norm(quaternion *q) {
	double res = ((*q).w * (*q).w) + ((*q).x * (*q).x) + ((*q).y * (*q).y) + ((*q).z * (*q).z);
	if((res > 1.001) || (res < 0.999))
	{
		double mag = sqrt(res);
		if(mag == 0)
			return;
		(*q).w = (*q).w/mag;
		(*q).x = (*q).x/mag;
		(*q).y = (*q).y/mag;
		(*q).z = (*q).z/mag;
	}
}

quaternion quaternion_product(quaternion q_a, quaternion q_b) {
	quaternion ret;
	ret.w = ((q_a.w*q_b.w) - (q_a.x*q_b.x) - (q_a.y*q_b.y) - (q_a.z*q_b.z));
	ret.x = ((q_a.w*q_b.x) + (q_a.x*q_b.w) + (q_a.y*q_b.z) - (q_a.z*q_b.y));
	ret.y = ((q_a.w*q_b.y) - (q_a.x*q_b.z) + (q_a.y*q_b.w) + (q_a.z*q_b.x));
	ret.z = ((q_a.w*q_b.z) + (q_a.x*q_b.y) - (q_a.y*q_b.x) + (q_a.z*q_b.w));
	return ret;
}

quaternion quaternion_from_axis_angle(vector3d axis, double angle) {
	quaternion ret;
	ret.w = cos(angle/2);
	double sht = sin(angle/2);
	ret.x = axis.x * sht;
	ret.y = axis.y * sht;
	ret.z = axis.z * sht;
	return ret;
}

matrix3x3d quaternion_to_matrix(quaternion q) {
	matrix3x3d ret;
	ret.r1[0] = 1 -(2 * (q.y * q.y)) - (2 * (q.z * q.z));
	ret.r1[1] = (2 * q.x * q.y) - (2 * q.w * q.z);
	ret.r1[2] = (2 * q.x * q.z) + (2 * q.w * q.y);

	ret.r2[0] = (2 * q.x * q.y) + (2 * q.w * q.z);
	ret.r2[1] = 1 - (2 * (q.x * q.x)) - (2 * (q.z * q.z));
	ret.r2[2] = (2 * (q.y * q.z)) - (2 * (q.w * q.x));

	ret.r3[0] = (2 * (q.x * q.z)) - (2 * q.w * q.y);
	ret.r3[1] = (2 * q.y * q.z) + (2 * q.w * q.x);
	ret.r3[2] = 1 - (2 * (q.x * q.x)) - (2 * (q.y * q.y));
	return ret;
}

quaternion quaternion_conjugate(quaternion q) {
	quaternion ret;
	ret.w = q.w;
	ret.x = -q.x;
	ret.y = -q.y;
	ret.z = -q.z;
	return ret;
}

quaternion quaternion_from_matrix(matrix3x3d *mat) {
	quaternion res;

	float trace = (*mat).r1[0] + (*mat).r2[1] + (*mat).r3[2]; //on main diagonal

	float scale = 0.0f;
	if (trace > 0)
	{
		scale = sqrt(trace + 1.0) * 2.0;
		res.w = 0.25 * scale;
		res.x = ((*mat).r3[1] - (*mat).r2[2]) / scale;
		res.y = ((*mat).r1[2] - (*mat).r3[0]) / scale;
		res.z = ((*mat).r2[0] - (*mat).r1[1]) / scale;
	}
	else if (((*mat).r1[0] < (*mat).r2[1]) & ((*mat).r1[0] < (*mat).r3[2]))
	{
		scale = sqrt(1.0 + (*mat).r1[0] - (*mat).r2[1] - (*mat).r3[2]) * 2.0;
		res.w = ((*mat).r3[1] - (*mat).r2[2]) / scale;
		res.x = 0.25 * scale;
		res.y = ((*mat).r1[1] + (*mat).r2[0]) / scale;
		res.z = ((*mat).r1[2] + (*mat).r3[0]) / scale;
	}
	else if ((*mat).r2[1] < (*mat).r3[2])
	{
		scale = sqrt(1.0 + (*mat).r2[1] - (*mat).r1[0] - (*mat).r3[2]) * 2.0;
		res.w = ((*mat).r1[2] - (*mat).r3[0]) / scale;
		res.x = ((*mat).r1[1] + (*mat).r2[0]) / scale;
		res.y = 0.25 * scale;
		res.z = ((*mat).r2[2] + (*mat).r3[1]) / scale;
	}
	else
	{
		scale = sqrt(1.0 + (*mat).r3[2] - (*mat).r1[0] - (*mat).r2[1]) * 2.0;
		res.w = ((*mat).r2[0] - (*mat).r1[1]) / scale;
		res.x = ((*mat).r1[2] + (*mat).r3[0]) / scale;
		res.y = ((*mat).r2[2] + (*mat).r3[1]) / scale;
		res.z = 0.25 * scale;
	}

	return res;
}
