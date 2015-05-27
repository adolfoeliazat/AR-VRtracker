/*
 * display.c
 *
 *  Created on: May 17, 2015
 *      Author: root
 */

#include "display.h"
#include <util/delay.h>

void display_init(void) {
	u8g_InitSPI(&u8g, &u8g_dev_sh1106_128x64_sw_spi, PN(1, 1), PN(1, 2), U8G_PIN_NONE, PN(1, 5), U8G_PI_RESET);
}

void display_draw_logo(void) {
	u8g_FirstPage(&u8g);
	while (u8g_NextPage(&u8g)) {
		u8g_SetFont(&u8g, u8g_font_gdr20r);
		u8g_DrawStr(&u8g, 10, 45, "[AR^VR]");
	}
}

void display_draw_line(char *line) {
	u8g_FirstPage(&u8g);
	while (u8g_NextPage(&u8g)) {
		u8g_SetFont(&u8g, u8g_font_gdr10r);
		u8g_DrawStr(&u8g, 2, 20, line);
	}
}

void display_draw_line2x(char *line, char *line2) {
	u8g_FirstPage(&u8g);
	while (u8g_NextPage(&u8g)) {
		u8g_SetFont(&u8g, u8g_font_gdr10r);
		u8g_DrawStr(&u8g, 2, 20, line);
		u8g_DrawStr(&u8g, 2, 45, line2);
	}
}

void display_draw_line3x(char *line, char *line2, char *line3) {
	u8g_FirstPage(&u8g);
	while (u8g_NextPage(&u8g)) {
		u8g_SetFont(&u8g, u8g_font_gdr10r);
		u8g_DrawStr(&u8g, 2, 20, line);
		u8g_DrawStr(&u8g, 2, 45, line2);
		u8g_DrawStr(&u8g, 2, 64, line3);
	}
}
