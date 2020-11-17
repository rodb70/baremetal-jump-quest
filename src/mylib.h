#ifndef MY_LIB_H
#define MY_LIB_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware.h"

#define COLOR(r, g, b)  ((r) | (g) << 5 | (b) << 10)
#define RED     COLOR(31,0,0)
#define GREEN   COLOR(0,31,0)
#define BLUE    COLOR(0,0,31)
#define YELLOW  COLOR(31,31,0)
#define CYAN    COLOR(0, 31, 31)
#define MAGENTA COLOR(31,0,31)
#define WHITE   COLOR(31,31,31)
#define BLACK   0
#define GRAY    COLOR(15,15,15)

#define OFFSET(r, c, rowlen) ((r)*(rowlen) + (c))

#define SCREEN_HEIGHT 160
#define SCREEN_WIDTH 240

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)

// State enum definition
enum GBAState {
	START,
	START_NODRAW,
	PLAY,
	PLAY_NODRAW,
	END,
	END_NODRAW,
};

//Platform struct
typedef struct {
	int x;
	int y;
	int prevX;
	int prevY;
	bool onScreen;
} Platform;

// Math
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Prototypes
void setPixel(int row, int col, uint16_t color);
void drawAnimation(int x, int y, int width, int height, int animationCount, int numFrames, const uint16_t *image);
void drawBackground(const uint16_t *image);
void drawImage(int x, int y, int width, int height, const uint16_t *image);
void drawImageTrans(int x, int y, int width, int height, const uint16_t *image);
void redrawBackground(int x, int y, int width, int height, const uint16_t *image);
void drawRect(int x, int y, int width, int height, uint16_t color);
void waitForVBlank(void);
void fillScreen(volatile uint16_t color);

//Main file prototypes
void resetStart(void);
void endGame(int finalScore);

#endif
