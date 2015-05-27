/*
 * ahrs.c
 *
 *  Created on: Apr 4, 2015
 *      Author: root
 */
#include "ahrs/ahrs.h"
#include "sensors/l3g4200d.h"
#include "sensors/adxl345.h"
#include "sensors/hmc5883l.h"
#include "ahrs/display.h"
#include <math.h>
#include <stdio.h>
#include <util/delay.h>

void ahrs_init(dataexchange_t *data, u8g_t *disp, params_t *calib_params) {
	//vectors for absolute rotation matrix
	vector3d acc_v, mag_v;
	matrix3x3d initial_rmat;
	quaternion initial_q;

	calib_ptr = calib_params;

	int16_t amount = 100;

	//show init start message
	_delay_ms(2000);
	display_draw_line2x("Initialization,", "please stand still");
	_delay_ms(2000);

	acc_v.x = 0.0;
	acc_v.y = 0.0;
	acc_v.z = 0.0;

	mag_v.x = 0.0;
	mag_v.y = 0.0;
	mag_v.z = 0.0;

	for (int i = 0; i < amount; i++) {
		adxl345_read(data); //accelerometer read
		hmc5883l_read(data); //magnetometer read

		acc_v.x += (*data).acc_x;
		acc_v.y += (*data).acc_y;
		acc_v.z += (*data).acc_z;

		mag_v.x += (*data).mag_x;
		mag_v.y += (*data).mag_y;
		mag_v.z += (*data).mag_z;

//		if ((i % 10) == 0) {
//			char str[3];
//			sprintf(str, "ready: %d%%", i);
//			display_draw_line2x("Initialization:", str);
//		}

	}

	display_draw_line2x("Initialization:", "...done!");
	_delay_ms(2000);
	display_draw_logo();

	acc_v.x /= amount;
	acc_v.y /= amount;
	acc_v.z /= amount;

	mag_v.x /= amount;
	mag_v.y /= amount;
	mag_v.z /= amount;

	hmc5883l_applyCalibration(&mag_v, calib_ptr);

	vector3d down = vector_inv(acc_v);
	vector3d east = vector_cross(down, mag_v);
	vector3d north = vector_cross(east, down);

	//normalize vectors
	vector_norm(&down);
	vector_norm(&east);
	vector_norm(&north);

	//vectors to matrix
	matrix_vector_to_row(&initial_rmat, north, 1);
	matrix_vector_to_row(&initial_rmat, east, 2);
	matrix_vector_to_row(&initial_rmat, down, 3);

	//matrix to quaternion
	initial_q = quaternion_from_matrix(&initial_rmat);

	//normalize
	quaternion_norm(&initial_q);

	//initialize
	(*data).qr.w = initial_q.w;
	(*data).qr.x = initial_q.x;
	(*data).qr.y = initial_q.y;
	(*data).qr.z = initial_q.z;

}

quaternion ahrs_orientation(dataexchange_t *data) {

	//angle component
	vector3d gyr_rad_s = vector_sum(l3g4200d_raw_to_rad(data), ahrs_drift_correction(data));

	//vector3d gyr_rad_s = vector_sum(l3g4200d_raw_to_rad(data), ahrs_drift_correction(data));
	double angle = vector_magnitude(gyr_rad_s) * (*data).time_period;

	//axis, normalized
	vector_norm(&gyr_rad_s);

	//quaternion from axis/angle
	quaternion q = quaternion_from_axis_angle(gyr_rad_s, angle);

	//normalize
	quaternion_norm(&q);

	(*data).qr = quaternion_product((*data).qr, q);

	return q;
}

quaternion ahrs_orientation_from_gyro(dataexchange_t *data) {

	//angle component
	vector3d gyr_rad_s = l3g4200d_raw_to_rad(data);
	double angle = vector_magnitude(gyr_rad_s) * (*data).time_period;

	//axis, normalized
	vector_norm(&gyr_rad_s);

	//quaternion from axis/angle
	quaternion q = quaternion_from_axis_angle(gyr_rad_s, angle);

	//normalize
	quaternion_norm(&q);

	(*data).qr = quaternion_product((*data).qr, q);



	return q;
}

quaternion ahrs_orientation_from_accel_mag(dataexchange_t *data) {

	//vectors for rotation matrix
	vector3d acc_v, mag_v;
	matrix3x3d rmat;
	quaternion q;

	acc_v.x = (*data).acc_x;
	acc_v.y = (*data).acc_y;
	acc_v.z = (*data).acc_z;

	mag_v.x = (*data).mag_x;
	mag_v.y = (*data).mag_y;
	mag_v.z = (*data).mag_z;

	hmc5883l_applyCalibration(&mag_v, calib_ptr);

	vector3d down = vector_inv(acc_v);
	vector3d east = vector_cross(down, mag_v);
	vector3d north = vector_cross(east, down);

	//normalize vectors
	vector_norm(&down);
	vector_norm(&east);
	vector_norm(&north);

	//vectors to matrix
	matrix_vector_to_row(&rmat, north, 1);
	matrix_vector_to_row(&rmat, east, 2);
	matrix_vector_to_row(&rmat, down, 3);

	//matrix to quaternion
	q = quaternion_from_matrix(&rmat);

	//normalize
	quaternion_norm(&q);

	(*data).qr = q;

	return q;
}

vector3d ahrs_drift_correction(dataexchange_t *data) {
	vector3d corr_vector, acc_g;

	corr_vector.x = 0.0;
	corr_vector.y = 0.0;
	corr_vector.z = 0.0;

	//do correction only then acceleration is close to 1G (unreliable if greater)
	acc_g = adxl345_raw_to_g(data, SCALE_2G_10B);
	double acc_magnitude = vector_magnitude(acc_g);
	if (fabs(acc_magnitude - 1.0) <= 0.15) {
		float corr_strength = 0.15;

		//vectors for rotation matrix
		vector3d acc_v, mag_v;

		acc_v.x = (*data).acc_x;
		acc_v.y = (*data).acc_y;
		acc_v.z = (*data).acc_z;

		mag_v.x = (*data).mag_x;
		mag_v.y = (*data).mag_y;
		mag_v.z = (*data).mag_z;

		hmc5883l_applyCalibration(&mag_v, calib_ptr);

		vector3d down = vector_inv(acc_v);
		vector3d east = vector_cross(down, mag_v);
		vector3d north = vector_cross(east, down);

		//normalize vectors
		vector_norm(&down);
		vector_norm(&east);
		vector_norm(&north);

		//matrix from rotation quaternion
		matrix3x3d rot_matrix = quaternion_to_matrix((*data).qr);

		//correction vector calculation
		vector3d sum1 = vector_sum(vector_cross(north, matrix_row_to_vector(&rot_matrix, 1)),
				vector_cross(east, matrix_row_to_vector(&rot_matrix, 2)));

		vector3d sum2 = vector_sum(sum1, vector_cross(down, matrix_row_to_vector(&rot_matrix, 3)));

		corr_vector = vector_scale(sum2, corr_strength);
	}

	return corr_vector;
}
