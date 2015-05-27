/*
 * hmc5883l.h
 *
 *  Created on: Mar 30, 2015
 *      Author: root
 */

#ifndef HMC5883L_H_
#define HMC5883L_H_

#include "data/data.h"
#include <stdint.h>

//device i2c address
#define MAG_ADDR 0x3C

//register map
#define REG_CONFIG_A 0x00
#define REG_CONFIG_B 0x01
#define REG_MODE 0x02
#define REG_OUT_X_M 0x03
#define REG_OUT_X_L 0x04
#define REG_OUT_Z_M 0x05
#define REG_OUT_Z_L 0x06
#define REG_OUT_Y_M 0x07
#define REG_OUT_Y_L 0x08
#define REG_STATUS 0x09
#define REG_IDENT_A 0x0A
#define REG_IDENT_B 0x0B
#define REG_IDENT_C 0x0C

//modes
#define NORM_75HZ_8AV 0x78 //8-average, 75 Hz default, normal measurement - 01111000 in binary
#define MODE_CONT 0x00 //continuous measurement

//gain
#define GAIN_0_88 0x00  // +/- 0.88 Ga
#define GAIN_1_3 0x20  // +/- 1.3  Ga (default)
#define GAIN_1_9 0x40  // +/- 1.9  Ga
#define GAIN_2_5 0x60  // +/- 2.5  Ga
#define GAIN_4_0 0x80  // +/- 4.0  Ga
#define GAIN_4_7 0xA0  // +/- 4.7  Ga
#define GAIN_5_6 0xC0  // +/- 5.6  Ga
#define GAIN_8_1 0xE0  // +/- 8.1  Ga

//temporary calibration defines
#define M_OFFSET_X -39.0f
#define M_OFFSET_Y -40.0f
#define M_OFFSET_Z 29.0f

#define SCALE_X 0.858012f
#define SCALE_Y 1.1371f
#define SCALE_Z 1.04703f

#define R1_X 0.517363f
#define R1_Y -0.159236f
#define R1_Z 0.84082f

#define R2_X -0.838209f
#define R2_Y 0.103708f
#define R2_Z 0.535397f

#define R3_X 0.172455f
#define R3_Y 0.981778f
#define R3_Z 0.0798181f

//function prefix: hmc5883l_

void hmc5883l_init(uint8_t mode, uint8_t gain, uint8_t rate);
void hmc5883l_read(dataexchange_t *);
void hmc5883l_applyCalibration(vector3d *vec, params_t *calib_params);

#endif /* HMC5883L_H_ */
