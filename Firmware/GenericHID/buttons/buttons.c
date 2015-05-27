/*
 * buttons.c
 *
 *  Created on: Apr 3, 2015
 *      Author: root
 */

#include "buttons/buttons.h"

void buttons_init(void) {
	//button ports init - input with pull up
	BUTTONS_DIR = 0x00; //analog port to input
	BUTTONS_PORT |= 1 << BUTTON_TOP_PIN_NUM; //pull up PF6 to vcc - A1 on board
	BUTTONS_PORT |= 1 << BUTTON_FRONT_PIN_NUM; //pull up to vcc - A2 on board
	BUTTONS_PORT |= 1 << BUTTON_SIDE_PIN_NUM; //pull up to vcc - A3 on board
}

void buttons_read(dataexchange_t *data) {
	(*data).btn = 0; //default
	if ((BUTTONS_PIN & (1 << BUTTON_TOP)) == 0) (*data).btn |= 1 << BUTTON_TOP_BIN_POS; //& (1 << PF6) act as mask, put 1 on pos 2
	if ((BUTTONS_PIN & (1 << BUTTON_FRONT)) == 0) (*data).btn |= 1 << BUTTON_FRONT_BIN_POS;
	if ((BUTTONS_PIN & (1 << BUTTON_SIDE)) == 0) (*data).btn |= 1 << BUTTON_SIDE_BIN_POS;
}
