#include "myLib.h"

u16 *videoBuffer = (u16 *) 0x6000000;

extern const unsigned char fontdata_6x8[12288];


u16 __key_curr;
u16 __key_prev;

void setPixel(int x, int y, u16 color) {
	*(videoBuffer + OFFSET(x, y, 240)) = color;
}

void drawRect(int x, int y, int height, int width, volatile u16 color) {
	for (int r = 0; r < height; r++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(x + r, y, 240)];
		DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
	}
}

void fillScreen(volatile u16 color) {
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240 * 160) | DMA_SOURCE_FIXED | DMA_ON;
}

void drawPictureBackground(const u16 arr[]) {
	DMA[3].src = arr;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240 * 160) | DMA_ON;
}

void drawImage3(int x, int y, int height, int width, const u16* image) {
	for (int r = 0; r < height; r++) {
		DMA[3].src = &image[OFFSET(x + r, y, 240)];
		DMA[3].dst = &videoBuffer[OFFSET(x + r, y, 240)];
		DMA[3].cnt = width | DMA_ON;
	}
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void key_poll() {
	__key_prev = __key_curr;
	__key_curr = ~BUTTONS & KEY_MASK;
}

u32 key_curr_state() {
	return __key_curr;
}

u32 key_prev_state() {
	return __key_prev;
}

u32 key_is_down(u32 key) {
	return __key_curr & key;
}

u32 key_is_up(u32 key) {
	return ~__key_curr & key;
}

u32 key_was_down(u32 key) {
	return __key_prev & key;
}

u32 key_was_up(u32 key) {
	return ~__key_prev & key;
}

u32 KEY_HIT(u32 key) {
	return (__key_curr & ~__key_prev) & key;
}

u32 KEY_RELEASED(u32 key) {
	return (~__key_curr & __key_prev) & key;
}
