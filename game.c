#include <stdlib.h>
#include <stdio.h>

#include "game.h"
#include "myLib.h"
#include "text.h"


#include "idleRight.h"
#include "idleLeft.h"
#include "walkRight.h"
#include "walkLeft.h"
#include "jumpRight.h"
#include "jumpLeft.h"
#include "introScreen.h"
#include "background.h"
#include "platform.h"

//Scaled screen variables
#define SCREEN_HEIGHT_SCALED 160000
#define SCREEN_WIDTH_SCALED 240000

//Player variables
#define PLAYER_WIDTH 25000 //actually 25
#define PLAYER_HEIGHT 31000 //actually 31

//NOTE: These variables are kept as thousands to simulate decimals. I.E. 1234 = 1.234
int playerX, playerY; //Player x and y are centered
int prevX, prevY;
int velX, velY;
bool onPlatform;
bool faceRight;

//Animation variables
unsigned int time;
int animation; //selects the animation -> 1/2/3 for idle/move/jump
int currentFrame; //the current frame
int totalFrames;

//Variables
#define GRAVITY 1300
#define FALL_SPEED 2000 //Because we want to fall faster when not jumping. Who needs physics?
#define AIR_SPEED 3000
#define GROUND_SPEED 3000 //Because we move wayyyy too fast on the ground
#define JUMP_HEIGHT 13000

#define IDLE 0
#define MOVE 1

//Platforms
#define NUM_PLATFORMS 7

//Initialize platforms
Platform allPlatforms[NUM_PLATFORMS];
int platformSpeed;

//Score counter
int score;
char scoreChar[25];
//Timer for score, score increases faster as platforms move faster
int scoreIncrease;

char tester[25];

void initPlatforms() {
	allPlatforms[0].x = 25000;
	allPlatforms[0].y = 45000;
	allPlatforms[0].prevX = 0;
	allPlatforms[0].onScreen = true;

	allPlatforms[1].x = 180000;
	allPlatforms[1].y = 53000;
	allPlatforms[1].prevX = 0;
	allPlatforms[1].onScreen = true;

	allPlatforms[2].x = 65000;
	allPlatforms[2].y = 65000;
	allPlatforms[2].prevX = 0;
	allPlatforms[2].onScreen = true;

	allPlatforms[3].x = 330000;
	allPlatforms[3].y = 80000;
	allPlatforms[3].prevX = 0;
	allPlatforms[3].onScreen = false;

	allPlatforms[4].x = 110000;
	allPlatforms[4].y = 100000;
	allPlatforms[4].prevX = 0;
	allPlatforms[4].onScreen = true;

	allPlatforms[5].x = 270000;
	allPlatforms[5].y = 120000;
	allPlatforms[5].prevX = 0;
	allPlatforms[5].onScreen = false;

	allPlatforms[6].x = 100000;
	allPlatforms[6].y = 135000;
	allPlatforms[6].prevX = 0;
	allPlatforms[6].onScreen = true;
}

//Create player and initialize starting values
void init() {

	//Put player on bottom of screen
	playerX = (1000 + PLAYER_WIDTH / 2);
	playerY = (1000 - PLAYER_HEIGHT / 2);

	onPlatform = false;
	prevX = playerX;
	prevY = playerY;
	faceRight = true;
	velX = 0;
	velY = 0;

	initPlatforms();

	score = 0;
	scoreIncrease = 30;
	platformSpeed = 2700;

	//Pseudo random yay
	srand(1);

	//default animation: idle
	time = 0;
	animation = IDLE;
	currentFrame = 0;
	totalFrames = IDLERIGHT_FRAMES;
}

void update() {

	//Update player position
	prevX = playerX;
	prevY = playerY;

	playerX += velX;
	playerY += velY;

	//Update platform locations1
	for (int k = 0; k < NUM_PLATFORMS; k++) {
		allPlatforms[k].prevX = allPlatforms[k].x;

		//Check if the platform is on the screen
		if (!allPlatforms[k].onScreen && allPlatforms[k].x - PLATFORM_WIDTH / 2 < 240000) {
			allPlatforms[k].onScreen = true;
		}

		allPlatforms[k].x = (allPlatforms[k].x  - platformSpeed);
		if (allPlatforms[k].x + PLATFORM_WIDTH / 2 < 0) {
			//generate new x location, then offset it so that it's off the screen.
			if (time % 4 == 0) {
				allPlatforms[k].x = 240000 + generateRandom(0, 60) * 1000;
			} else if (time % 4 == 1) {
				allPlatforms[k].x = 240000 + generateRandom(61, 120) * 1000;
			} else if (time % 4 == 2) {
				allPlatforms[k].x = 240000 + generateRandom(121, 180) * 1000;
			} else {
				allPlatforms[k].x = 240000 + generateRandom(181, 230) * 1000;
			}
			allPlatforms[k].onScreen = false;
		}
	}

	velX = 0;

	//Update animation counter
	time++;

	if (time % 20 == 0 || (animation == MOVE && time % 7 == 0)) {
		currentFrame = (currentFrame + 1) % totalFrames;
	}

	//Update score
	if (time % scoreIncrease == 0) {
		score += 500;
	}

	//Update speed
	if (time % 250 == 0) {
		if (platformSpeed < 5000) {
			platformSpeed += platformSpeed / 4;
		}
		if (scoreIncrease > 5) {
			scoreIncrease -= 5;
		}
	}

	//Prevent player from going off screen
	playerX = MAX(playerX, (PLAYER_WIDTH / 2));
	playerX = MIN(playerX, (SCREEN_WIDTH_SCALED - PLAYER_WIDTH / 2));

	//Apply gravity if in air
	velY += onPlatform ? 0 : GRAVITY;


	if (velY >= 0) {//Check if player has hit the ground or a platform while falling
		onPlatform = false;

		for (int i = 0; i < NUM_PLATFORMS; i++) {

			if (allPlatforms[i].onScreen == true
			        && playerX >= (allPlatforms[i].x - PLATFORM_WIDTH / 2)
			        && playerX <= (allPlatforms[i].x + PLATFORM_WIDTH / 2)
			        && playerY >= (allPlatforms[i].y - PLATFORM_HEIGHT / 2 - PLAYER_HEIGHT / 2 - 1000)
			        && prevY <= (allPlatforms[i].y - PLATFORM_HEIGHT / 2) - PLAYER_HEIGHT / 2) {
				//drawString(10, 10, "hi", WHITE);
				velY = 0;
				onPlatform = true;
				playerY = ((allPlatforms[i].y - PLATFORM_HEIGHT / 2) - PLAYER_HEIGHT / 2) - 1000;
				velX = -platformSpeed;
				//currentPlatform = allPlatforms[i];
			}

			//Check to see if player fell under screen
			if (playerY >= (SCREEN_HEIGHT_SCALED - PLAYER_HEIGHT / 2)) {
				endGame(score);

				//God Mode Tester
				// velY = 0;
				// onPlatform = true;
				// playerY = (SCREEN_HEIGHT_SCALED - PLAYER_HEIGHT / 2);
			}
		}
	}

	//Handle input
	if (KEY_DOWN_NOW(BUTTON_LEFT)) {
		velX = onPlatform ? -GROUND_SPEED : -AIR_SPEED;
		faceRight = false;
	}

	if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
		velX = onPlatform ? GROUND_SPEED : AIR_SPEED;
		//1.80 speed
		faceRight = true;
	}

	//Switch animations if needed
	if (onPlatform) {
		if (KEY_DOWN_NOW(BUTTON_UP)) {
			velY = -JUMP_HEIGHT; //5.00 speed
			onPlatform = false;
		} else if (velX != -platformSpeed && animation != MOVE) { //If starting to move, set animation to move
			animation = MOVE;
			currentFrame = 0;
			totalFrames = faceRight ? WALKRIGHT_FRAMES : WALKLEFT_FRAMES;
		} else if (velX == -platformSpeed && velY == 0 && animation != IDLE) { //If not moving
			animation = IDLE;
			currentFrame = 0;
			totalFrames = faceRight ? IDLERIGHT_FRAMES : IDLELEFT_FRAMES;
		}
	}

	//Return to menu
	if (KEY_DOWN_NOW(BUTTON_SELECT)) {
		resetStart();
	}

	waitForVBlank();
	draw();
}

//Use this to generate new x values between a min and a max
int generateRandom(int min, int max) {
	return rand() % (min - max + 1) + min;
}


void draw() {

	//Redraw background behind player
	redrawBackground((prevX - PLAYER_WIDTH / 2) / 1000, (prevY - PLAYER_HEIGHT / 2) / 1000, PLAYER_WIDTH / 1000, PLAYER_HEIGHT / 1000, background);

	if (time % scoreIncrease == 0 || time == 1) {
		//Draw background behind score
		redrawBackground(150, 0, 100, 10, background);

		//Print score
		sprintf(scoreChar, "Score:%d", score);
		drawString(160, 0, scoreChar, WHITE);
	}

	//Draw platforms
	for (int i = 0; i < NUM_PLATFORMS; i++) {


		//Don't draw/redraw platforms that aren't on the screen
		if (allPlatforms[i].prevX - PLATFORM_WIDTH / 2 <= 240000 || allPlatforms[i].prevX + PLATFORM_WIDTH / 2 >= 0) {
			redrawBackground((allPlatforms[i].prevX - PLATFORM_WIDTH / 2) / 1000, (allPlatforms[i].y - PLATFORM_HEIGHT / 2) / 1000, PLATFORM_WIDTH / 1000, PLATFORM_HEIGHT / 1000, background);
		}

		if (allPlatforms[i].onScreen) {
			drawImage((allPlatforms[i].x - PLATFORM_WIDTH / 2) / 1000, (allPlatforms[i].y - PLATFORM_HEIGHT / 2) / 1000, PLATFORM_WIDTH / 1000, PLATFORM_HEIGHT / 1000, platform);
		}
	}

	//Change animation
	if (onPlatform) {
		if (velX == -platformSpeed) {
			drawAnimation((playerX - PLAYER_WIDTH / 2) / 1000, (playerY - PLAYER_HEIGHT / 2) / 1000, PLAYER_WIDTH / 1000, PLAYER_HEIGHT / 1000, currentFrame, totalFrames, faceRight ? idleRight : idleLeft);
		} else {
			drawAnimation((playerX - PLAYER_WIDTH / 2) / 1000, (playerY - PLAYER_HEIGHT / 2) / 1000, PLAYER_WIDTH / 1000, PLAYER_HEIGHT / 1000, currentFrame, totalFrames, faceRight ? walkRight : walkLeft);
		}
	} else {
		drawImageTrans((playerX - PLAYER_WIDTH / 2) / 1000, (playerY - PLAYER_HEIGHT / 2) / 1000, PLAYER_WIDTH / 1000, PLAYER_HEIGHT / 1000, faceRight ? jumpRight : jumpLeft);
	}

}