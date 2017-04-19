#ifndef _LCD_H    /* Guard against multiple inclusion */
#define _LCD_H

#include <xc.h>
#include "ILI9163C.h"

#define BACKGROUND WHITE

void LCD_drawChar(unsigned short, unsigned short, char, unsigned short);
void LCD_drawString(unsigned short, unsigned short, char *, unsigned short);
void LCD_drawBar(unsigned short, unsigned short, char, char, unsigned short);

#endif 
