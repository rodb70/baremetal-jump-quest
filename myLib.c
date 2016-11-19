#include "myLib.h"


unsigned short *videoBuffer = (unsigned short *) 0x6000000;
//Methods to implement
/*
	void setPixel(int r, int c, unsigned short color);
	void drawAnimation(int x, int y, int width, int height, int animationCount, int animationCountMax, const unsigned short *image);
	void drawBackground(const unsigned short *image);
	void drawImage3(int x, int y, int width, int height, const unsigned short *image);
	void drawRect(int x, int y, int width, int height, unsigned short color);
	void waitForVblank();
	void fillScreen(u16 color);
*/

void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawImage(int x, int y, int width, int height, const unsigned short *image) {
	
	//Prevent wrap around
	int newX = x;
	int offScreen = 0;

	//Left side
	if (x < 0) {
		offScreen = -x;
		newX = x + offScreen;
	} 
	//Right side
	else if (x + width > 240) {
		offScreen = x + width - 240;
	}

	for (int row = 0; row < height; row++) {
		DMA[3].src = &(image[OFFSET(row, offScreen, width)]);
		DMA[3].dst = &(videoBuffer[OFFSET(y + row, newX, 240)]);
		DMA[3].cnt = (width - offScreen) | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
	}
}

void drawImageTrans(int x, int y, int width, int height, const unsigned short *image) {
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			u16 pixelColor = image[OFFSET(row, col, width)];
			if (pixelColor != MAGENTA) {
				setPixel(row + y, col + x, pixelColor);
			}
		}
	}
}

void drawAnimation(int x, int y, int width, int height, int animationCount, int numFrames, const unsigned short *image) {

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			u16 pixelColor = image[OFFSET(row, (animationCount * width) + col, numFrames * width)];
			if (pixelColor != MAGENTA) {
				setPixel(row + y, col + x, pixelColor);
			}
		}
	}

}

void redrawBackground(int x, int y, int width, int height, const unsigned short *image) {
	//Prevent negatives


	for (int row = 0; row < height; row++) {
		DMA[3].src = &(image[OFFSET(y + row, x, 240)]);
		DMA[3].dst = &(videoBuffer[OFFSET(y + row, x, 240)]);
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
	}
}

void drawBackground(const unsigned short *image) {
	DMA[3].src = image;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt =  38400 | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
}

void fillScreen(volatile u16 color) {
	DMA[3].src = &(color);
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED;
}

void waitForVBlank()
{
	while (SCANLINECOUNTER > 160);
	while (SCANLINECOUNTER < 160);
}
