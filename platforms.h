typedef struct {
	int x;
	int y;
	int prevX;
	int prevY;
	bool onScreen;
} Platform;

#define NUM_PLATFORMS 3

extern Platform allPlatforms[NUM_PLATFORMS];
