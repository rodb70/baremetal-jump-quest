#ifndef TEXT_H
#define TEXT_H
#include "mylib.h"
extern const unsigned char fontdata_6x8[12288];

void drawString(int row, int col, char *str, uint16_t color);
void drawChar(int row, int col, char ch, uint16_t color);

#endif
