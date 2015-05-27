/*
 * adxl345.h
 *
 *  Created on: Mar 30, 2015
 *      Author: root
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#include <stdint.h>
#include "data/data.h"
#include "ahrs/vector.h"

//device i2c address
#define ALT_ADDR 0xA6

//register map
#define DEVID 0x00
#define RESERVED1 0x01
#define THRESH_TAP 0x1d
#define OFSX 0x1e
#define OFSY 0x1f
#define OFSZ 0x20
#define DUR 0x21
#define LATENT 0x22
#define WINDOW 0x23
#define THRESH_ACT 0x24
#define THRESH_INACT 0x25
#define TIME_INACT 0x26
#define ACT_INACT_CTL 0x27
#define THRESH_FF 0x28
#define TIME_FF 0x29
#define TAP_AXES 0x2a
#define ACT_TAP_STATUS 0x2b
#define BW_RATE 0x2c
#define POWER_CTL 0x2d
#define INT_ENABLE 0x2e
#define INT_MAP 0x2f
#define INT_SOURCE 0x30
#define DATA_FORMAT 0x31
#define DATAX0 0x32
#define DATAX1 0x33
#define DATAY0 	0x34
#define DATAY1 	0x35
#define DATAZ0 	0x36
#define DATAZ1 	0x37
#define FIFO_CTL 	0x38
#define FIFO_STATUS 0x39

//measure mode
#define MEASURE_ON 0x08 //normal mode, autosleep off, 8hz measure rate in sleep mode

//measure ranges
#define RANGE_2G 0x00 	//00 in binary
#define RANGE_4G 0x01 	//01 in binary
#define RANGE_8G 0x02 	//10 in binary
#define RANGE_16G 0x03 	//11 in binary

//output rates
#define ODR_1600 0xF 	//1111 in binary
#define ODR_800  0xE 	//1110 in binary
#define ODR_400  0xD 	//1101 in binary
#define ODR_200  0xC 	//1100 in binary
#define ODR_100  0xB 	//1011 in binary

//sensitivity
#define SENS_2G_10B 256 //sensitivity at 2g range with 10-bit mode, LSB/g

//scale
#define SCALE_2G_10B 0.0039 //scale factor at 2g range with 10-bit mode =  3.9, mg/LSB
#define SCALE_4G_10B 0.0078
#define SCALE_8G_10B 0.0156
#define SCALE_16G_10B 0.0312

//function prefix: adxl345_

void adxl345_init(uint8_t mode, uint8_t range, uint8_t rate);
void adxl345_read(dataexchange_t *);
vector3d adxl345_raw_to_g(dataexchange_t *, float scale);

#endif /* ADXL345_H_ */
