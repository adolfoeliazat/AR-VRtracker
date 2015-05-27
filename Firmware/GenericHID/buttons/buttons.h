/*
 * buttons.h
 *
 *  Created on: Apr 3, 2015
 *      Author: root
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "data/data.h"
#include <avr/io.h>

//hardware defines
#define BUTTONS_DIR DDRF //buttons port direction
#define BUTTONS_PORT PORTF //buttons port
#define BUTTONS_PIN PINF //buttons pin - for state checking
#define BUTTON_SIDE PF4 //individual ports
#define BUTTON_FRONT PF5
#define BUTTON_TOP PF6
#define BUTTON_SIDE_PIN_NUM 4 //individual pins
#define BUTTON_FRONT_PIN_NUM 5
#define BUTTON_TOP_PIN_NUM 6
#define BUTTON_SIDE_BIN_POS 1 //positions in binary flag data field
#define BUTTON_FRONT_BIN_POS 0
#define BUTTON_TOP_BIN_POS 2

//function prefix: buttons_
void buttons_init(void);
void buttons_read(dataexchange_t *);


#endif /* BUTTONS_H_ */
