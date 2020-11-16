#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "text.h"

#include "introScreen.h"
#include "background.h"
#include "endScreen.h"
#include "mylib.h"

enum GBAState state = START;
int score = 0;
char scorePrint[25];


int main(void) {

	REG_DISPCNT = MODE_3 | BG2_EN;

	// TODO add any additional variables here that you need
	// Hint: You probably need one to do accurate detection of pressing A without
	// rapidly cycling through all your states!
	int pressed = 0; //0 means not pressed in the last state
	int flashing = 300; //Counter to make the intro string flash

	while (1) {
		waitForVBlank();
		switch (state) {

		case START:
			drawBackground(introScreen);
			drawString(55, 140, "Press A to start game", WHITE);
			state = START_NODRAW;
			break;

		case START_NODRAW:
			flashing--;
			if (flashing == 120) {
				redrawBackground(55, 140, 150, 10, introScreen);
			}

			if (flashing == 0) {
				drawString(55, 140, "Press A to start game", WHITE);
				flashing = 300;
			}

			// TODO make it change to another state by pressing A
			if (KEY_DOWN_NOW(BUTTON_A) & (!pressed)) {
				state = PLAY;
			}
			break;

		// TODO add more cases (states) to your switch statement!
		case PLAY:
			drawBackground(background);

			init();
			update();

			state = PLAY_NODRAW;
			break;

		case PLAY_NODRAW:
			update();
			break;

		case END:
			drawBackground(endScreen);
			sprintf(scorePrint, "Your Score: %d", score);
			drawString(70, 90, scorePrint, WHITE);
			drawString(85, 140, "Play Again?", WHITE);

			state = END_NODRAW;
			break;

		case END_NODRAW:
			if (KEY_DOWN_NOW(BUTTON_A) & (!pressed)) {
				state = START;
			}
			break;
		}
		//end switch

		// TODO you should probably do something here with one of your variables

		//If you are holding down button, pressed is 1
		pressed = KEY_DOWN_NOW(BUTTON_A);
	}

	return 0;
}

void resetStart() {
	state = START;
}

void endGame(int finalScore) {
	state = END;
	score = finalScore;
}
