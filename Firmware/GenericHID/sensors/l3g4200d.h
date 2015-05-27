/*
 * l3g4200d.h
 *
 *  Created on: Mar 30, 2015
 *      Author: root
 */

#ifndef L3G4200D_H_
#define L3G4200D_H_

#include <stdint.h>
#include "data/data.h"
#include "ahrs/vector.h"

//device i2c address
#define DEV_ADDR 0xD2

//register map
#define WHO_AM_I      0x0F
#define CTRL_REG1     0x20
#define CTRL_REG2     0x21
#define CTRL_REG3     0x22
#define CTRL_REG4     0x23
#define CTRL_REG5     0x24
#define REFERENCE     0x25
#define OUT_TEMP      0x26
#define STATUS_REG    0x27
#define OUT_X_L       0x28
#define OUT_X_H       0x29
#define OUT_Y_L       0x2A
#define OUT_Y_H       0x2B
#define OUT_Z_L       0x2C
#define OUT_Z_H       0x2D
#define FIFO_CTRL_REG 0x2E
#define FIFO_SRC_REG  0x2F
#define INT1_CFG      0x30
#define INT1_SRC      0x31
#define INT1_THS_XH   0x32
#define INT1_THS_XL   0x33
#define INT1_THS_YH   0x34
#define INT1_THS_YL   0x35
#define INT1_THS_ZH   0x36
#define INT1_THS_ZL   0x37
#define INT1_DURATION 0x38

#define BURST_MODE 0x80 //OUT_XL address ORed with this turn on burst mode (6 bytes will be read in sequence)

//mode
#define NORM_ODR_100 0x0F	// normal power mode, all axes enabled, 100 Hz, 12.5 cut-off
#define NORM_ODR_400 0x8F	// normal power mode, all axes enabled, 400 Hz, 20 cut-off
#define NORM_ODR_800 0xCF	// normal power mode, all axes enabled, 800 Hz, 30 cut-off

#define HI_PASS_NORM 0x00 	//hi-pass filter in normal mode; CTRL_REG2
#define DPS_2000 0x20 		//2000 dps; CTRL_REG4
#define DPS_250 0x00 		//250 dps; CTRL_REG4
#define FIFO_HI_LO_PASS 0x02 //Data in DataReg and FIFO are high-pass and low-pass-filtered by LPF2, FIFO enabled; CTRL_REG5

#define GYRO_SCALE 0.00875f //sensitivity at 250 dps for dps/digit (8.75 for mdps/digit)
#define gyroToRad(x) ((x) * 0.01745329252)  // *pi/180

//axis scales
#define GYR_SCALE_X 0.739342f
#define GYR_SCALE_Y 0.517852f
#define GYR_SCALE_Z -3.52601f

//offsets
#define OFFSET_X 133.974f
#define OFFSET_Y 191.276f
#define OFFSET_Z -28.092f

//function prefix: l3g4200d_

void l3g4200d_init(uint8_t mode, uint8_t rate);
uint8_t l3g4200d_read_byte(uint8_t dev_addr, uint8_t reg_addr);
void l3g4200d_read(dataexchange_t *);
void l3g4200d_read_seq(dataexchange_t *);
vector3d l3g4200d_raw_to_rad(dataexchange_t *);


#endif /* L3G4200D_H_ */
