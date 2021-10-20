#pragma once
#include "game.h"

int withinBoundaries(float minX, float minY, float maxX, float maxY);

void click_on_square(LevelData level);

void click_on_pause(float x, float y);

void exit_game(void);