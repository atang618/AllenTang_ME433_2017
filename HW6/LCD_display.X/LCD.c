#include "LCD.h"

void LCD_drawchar(unsigned short x, unsigned short y, char c, unsigned short color_active) {
    int i,j;
    int r = c - 0x20;
    unsigned short color;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; i++) {
            char mask = 0x01;
            char level = (ASCII[r][i] >> j) & mask; // only keeping the LSB
            if (level) {
                color = color_active;
            } else {
                color = BACKGROUND;
            }
            LCD_drawPixel(i+x,j+y,color);
        }
    }
}