#include <stdlib.h>
#include <stdio.h>

#include "myLib.h"
#include "text.h"
#include "CloudBackground.h"

enum {SPLASH, HELP, GAME, WIN, LOSE};

extern const u16 CloudBackgroundBitmap[38400];

u16 __key_curr = 0;
u16 __key_prev = 0;

void splash();
int help();
int game(int seed);
void win();
void lose();

int main(void) {
	int seed = 0;
	int state = SPLASH;
	while(1) {
		switch(state) {
		case SPLASH:
			splash();
			state = HELP;
			break;
		case HELP:
			seed = help();
			state = GAME;
			break;
		case GAME:
			state = game(seed);
			break;
		case WIN:
			win();
			state = SPLASH;
			break;
		case LOSE:
			lose();
			state = SPLASH;
			break;
		}
	}
}

void splash() {
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	u16 *ptr = videoBuffer;

	for (int i = 0; i < 38400; i++) {
		*ptr++ = 0;
	}
	drawString(50, 5, "Welcome to Max's Game!", RGB(50, 30, 20));
	drawString(70, 5, "Press Start (Enter)", RGB(50, 30, 20));
	while(!(KEY_DOWN_NOW(BUTTON_START)));
	while(KEY_DOWN_NOW(BUTTON_START));
}

int help() {
	REG_DISPCNT = MODE3 | BG2_ENABLE;

	int seed = 0;

	drawPictureBackground(CloudBackgroundBitmap);

	drawString(0, 5, "Controls:", RGB(50, 30, 20));
	drawString(10, 5, "Enter -> Start/Pause", RGB(50, 30, 20));
	drawString(20, 5, "Backspace -> Quit", RGB(50, 30, 20));
	drawString(30, 5, "Z -> Shoot", RGB(50, 30, 20));
	drawString(40, 5, "S -> Hold to move faster", RGB(50, 30, 20));
	drawString(50, 5, "Arrow Keys -> Up/Down/Left/Right", RGB(50, 30, 20));
	drawString(60, 5, "Shoot the enemies to earn points.", RGB(50, 30, 20));
	drawString(70, 5, "Earn 100 points to win!", RGB(50, 30, 20));
	drawString(80, 5, "Press Start to Continue!", RGB(50, 30, 20));
	while(!KEY_DOWN_NOW(BUTTON_START)) {
		seed++;
	}
	while(KEY_DOWN_NOW(BUTTON_START));
	return seed;
}

int game(int seed) {
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	int pause = 1;
	int score = 0;
	int lives = 5;
	int numberOfenemies = 5;
	char buffer[30];

	drawPictureBackground(CloudBackgroundBitmap);

	PLAYEROBJ player;
	PLAYEROBJ oldPlayer;
	player.row = 120;
	player.col = 50;
	player.size = 5;
	player.color = RGB(5, 10, 50);


	int numberOfBullets = 0;
	int maxBullets = 15;

	BULLET bullets[maxBullets];
	BULLET oldBullets[maxBullets];

	ENEMY enemies[numberOfenemies];
	ENEMY oldEnemies[numberOfenemies];

	srand(seed);
	for (int i = 0; i < numberOfenemies; i++) {
		enemies[i].row = -20;
		enemies[i].col = rand()%(159) + 1;
		enemies[i].speed = rand()%(3) + 1;
		enemies[i].size = rand()%(10) + 5;
		enemies[i].color = RGB(5, 0, 0);
		enemies[i].isAlive = 1;
		enemies[i].health = 5;
	}
	for (int i = 0; i < maxBullets; i++) {
		bullets[i].isAlive = 0;
	}
	
	while(1) {
		key_poll();
		oldPlayer = player;
		if (KEY_DOWN_NOW(BUTTON_SELECT) || lives == 0) {
			return LOSE;
		}
		if (score == 100) {
			return WIN;
		}
		if (KEY_DOWN_NOW(BUTTON_A)) {
			if (numberOfBullets < maxBullets) {
				int done = 0;
				for (int i = 0; i < maxBullets; i++) {
					if (bullets[i].isAlive == 0 && done == 0) {
						bullets[i].row = player.row + 1;
						bullets[i].col = player.col + 2;
						bullets[i].color = player.color;
						bullets[i].isAlive = 1;
						done = 1;
						numberOfBullets++;
					}
				}
			}
		}
		if (KEY_DOWN_NOW(BUTTON_B)) {

		}
		if (KEY_HIT(BUTTON_START)) {
			while(pause) {

				drawString(70, 100, "PAUSED", RGB(50, 30, 20));
				key_poll();
				if (KEY_HIT(BUTTON_START) && key_was_up(BUTTON_START)) {
					pause = 0;
				}
			}
			drawImage3(70, 100, 7, 36, CloudBackgroundBitmap);
			pause = 1;
		}
		if (KEY_DOWN_NOW(BUTTON_UP)) {
			if ((player.row) > 0) {
				if (KEY_DOWN_NOW(BUTTON_R)) {
					player.row -= 2;
				} else {
					player.row -= 1;
				}
			}
		}
		if (KEY_DOWN_NOW(BUTTON_DOWN)) {
			if ((player.row + player.size - 1) < 159) {
				if (KEY_DOWN_NOW(BUTTON_R)) {
					player.row += 2;
				} else {
					player.row += 1;
				}
			}
		}
		if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
			if ((player.col + player.size - 1) < 239) {
				if (KEY_DOWN_NOW(BUTTON_R)) {
					player.col += 2;
				} else {
					player.col += 1;
				}
			}
		}
		if (KEY_DOWN_NOW(BUTTON_LEFT)) {
			if ((player.col - player.size + 1) > 0) {
				if (KEY_DOWN_NOW(BUTTON_R)) {
					player.col -= 2;
				} else {
					player.col -= 1;
				}
			}
		}


		sprintf(buffer, "Lives: %d Score: %d", lives, score);



		drawImage3(150, 48, 7, 5, CloudBackgroundBitmap);
		drawImage3(150, 102, 7, 15, CloudBackgroundBitmap);
		drawString(150, 5, buffer, RGB(5,5,5));
		waitForVblank();

		for (int i = 0; i < numberOfenemies; i++) {
			oldEnemies[i] = enemies[i];

			drawImage3(oldEnemies[i].row, oldEnemies[i].col, oldEnemies[i].size, oldEnemies[i].size, CloudBackgroundBitmap);
			
		}

		for (int i = 0; i < numberOfenemies; i++) {
			enemies[i].row += enemies[i].speed;
			if (enemies[i].row > 159) {
				enemies[i].isAlive = 0;
			}
			if (enemies[i].isAlive == 1) {
				drawRect(enemies[i].row, enemies[i].col, enemies[i].size, enemies[i].size, enemies[i].color);
			} else {
				enemies[i].row = -20;
				enemies[i].col = rand()%(159) + 1;
				enemies[i].speed = rand()%(3) + 1;
				enemies[i].size = rand()%(10) + 5;
				enemies[i].health = 5;
				enemies[i].isAlive = 1;
			}
		}

		for (int i = 0; i < maxBullets; i++) {
			oldBullets[i] = bullets[i];
			drawImage3(oldBullets[i].row, oldBullets[i].col, 1, 1, CloudBackgroundBitmap);
		}
		for (int i = 0; i < maxBullets; i++) {
			bullets[i].row -= 2;
			if (bullets[i].row < 0) {
				bullets[i].isAlive = 0;
				numberOfBullets--;
			}		

			if (bullets[i].isAlive == 1) {
				setPixel(bullets[i].row, bullets[i].col, bullets[i].color);
			}
		}

		drawImage3(oldPlayer.row, oldPlayer.col, oldPlayer.size, oldPlayer.size, CloudBackgroundBitmap);
		drawRect(player.row, player.col, oldPlayer.size, player.size, player.color);

		for (int i = 0; i < numberOfenemies; i++) {
			if ((player.row < enemies[i].row + enemies[i].size) && (player.row + player.size > enemies[i].row) &&
				(player.col < enemies[i].col + enemies[i].size) && (player.col + player.size > enemies[i].col) && enemies[i].isAlive == 1) {
				lives--;
				enemies[i].isAlive = 0;
				drawImage3(enemies[i].row, enemies[i].col, enemies[i].size, enemies[i].size, CloudBackgroundBitmap);
				drawImage3(player.row, player.col, player.size, player.size, CloudBackgroundBitmap);

				player.row = 120;
				player.col = 0;
			}
		}

		for (int i = 0; i < numberOfenemies; i++) {
			for (int j = 0; j < maxBullets; j++) {
				if ((bullets[j].row <= enemies[i].row + enemies[i].size) && (bullets[j].row >= enemies[i].row) &&
					(bullets[j].col <= enemies[i].col + enemies[i].size) && (bullets[j].col >= enemies[i].col) && enemies[i].isAlive == 1 && bullets[j].isAlive == 1) {
						bullets[j].isAlive = 0;
						drawImage3(bullets[j].row, bullets[j].col, 1, 1, CloudBackgroundBitmap);
						numberOfBullets--;
						enemies[i].health--;
						if (enemies[i].health == 0) {
							score++;
							enemies[i].isAlive = 0;
							drawImage3(enemies[i].row, enemies[i].col, enemies[i].size, enemies[i].size, CloudBackgroundBitmap);
						}
				}
			}
		}

	}
}

void win() {
	unsigned short *ptr = videoBuffer;
	for (int i = 0; i < 38400; i++) {
		*ptr++ = RGB(5, 50, 5);
	}
	drawString(50, 5, "You win!", 0);
	drawString(70, 5, "Press Start (Enter)", 0);
	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}

void lose() {
	unsigned short *ptr = videoBuffer;
	for (int i = 0; i < 38400; i++) {
		*ptr++ = RGB(25, 50, 30);
	}
	drawString(50, 5, "You lose!", 0);
	drawString(70, 5, "Press Start (Enter)", 0);
	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}
