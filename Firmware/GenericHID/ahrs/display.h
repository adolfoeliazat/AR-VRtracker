/*
 * display.h
 *
 *  Created on: May 17, 2015
 *      Author: root
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "u8g_src/u8g.h"

u8g_t u8g; //display
char *line;

void display_init(void);
void display_draw_logo(void);
void display_draw_line(char *line);
void display_draw_line2x(char *line, char *line2);
void display_draw_line3x(char *line, char *line2, char *line3);
#endif /* DISPLAY_H_ */
