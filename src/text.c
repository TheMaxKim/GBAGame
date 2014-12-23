#include "myLib.h"
#include "text.h"

void drawChar(int x, int y, char ch, u16 color) {
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 6; c++) {
			if (fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
				setPixel(x + r, y + c, color);
			}
		}
	}
}

void drawString(int x, int y, char *str, u16 color) {
	while (*str) {
		drawChar(x, y, *str++, color);
		y += 6;
	}
}