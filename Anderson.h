#pragma once
#include "game.h"

typedef struct TextButton {
	float buttonPositionX;
	float buttonPositionY;
	float buttonWidth;
	float buttonHeight;

	float textPositionX;
	float textPositionY;
	char textString[];

}TextButton;

void text_button_constructor(float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[]);

int withinBoundaries(float minX, float minY, float maxX, float maxY);

void click_on_square(LevelData level);

void click_on_pause(float x, float y);

void exit_game(void);