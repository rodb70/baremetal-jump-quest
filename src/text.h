#ifndef TEXT_H
#define TEXT_H
#include "mylib.h"
extern const unsigned char fontdata_6x8[12288];

void drawString(int row, int col, char *str, u16 color);
void drawChar(int row, int col, char ch, u16 color);

#endif
