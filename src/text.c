#include "text.h"
#include "mylib.h"

void drawString(int x, int y, char *str, u16 color) {
	while(*str) {
		drawChar(x, y, *str, color);
		x += 6;
		str++;
	}
}

void drawChar(int x, int y, char ch, u16 color) {
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 6; c++) {
			//Gives either a 0 or a 1 out of the array
			if (fontdata_6x8[OFFSET(r, c, 6) + (ch*48) ] == 1) {
				setPixel(y + r, x + c, color);
			}
		}
	}
}