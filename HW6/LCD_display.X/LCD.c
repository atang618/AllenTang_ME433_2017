#include "LCD.h"

void LCD_drawChar(unsigned short x, unsigned short y, char c, unsigned short color_active) {
    int i,j;
    int r = c - 0x20;
    unsigned short color;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; j++) {
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

void LCD_drawString(unsigned short x, unsigned short y, char * msg, unsigned short color_active) {
    int i = 0;
    unsigned short xpntr = x;
    
    while (msg[i]) {
        LCD_drawChar(xpntr,y,msg[i],color_active);
        xpntr += 5;
    }
}