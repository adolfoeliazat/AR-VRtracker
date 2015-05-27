/*
 * ahrs.h
 *
 *  Created on: Apr 4, 2015
 *      Author: root
 */

#ifndef AHRS_H_
#define AHRS_H_

#include "ahrs/vector.h"
#include "ahrs/quaternion.h"
#include "data/data.h"
#include "u8g_src/u8g.h"

params_t *calib_ptr;

//function prefix: ahrs_
void ahrs_init(dataexchange_t *data, u8g_t *disp, params_t *calib_params);
quaternion ahrs_orientation(dataexchange_t *data);
quaternion ahrs_orientation_from_gyro(dataexchange_t *data);
quaternion ahrs_orientation_from_accel_mag(dataexchange_t *data);
vector3d ahrs_drift_correction(dataexchange_t *data);

#endif /* AHRS_H_ */
