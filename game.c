#include "cprocessing.h"
#include"game.h"

float  gridWidth, gridHeight, gameWidth, gameHeight;
void grid(void);

void game_init(void)
{
	CP_System_Fullscreen();
	float unusableScreenHeight/*Height not used for game, example menu*/, unusableScreenWidth/*Width not used for game, example menu*/;
	unusableScreenWidth = 960.0f;
	unusableScreenHeight = 540.0f;
	gameWidth = (float)CP_System_GetWindowWidth() - unusableScreenWidth;
	gameHeight = (float)CP_System_GetWindowHeight() - unusableScreenHeight;
	gridWidth = gameWidth / grid_Cols;
	gridHeight = gameHeight / grid_Rows;
}

void game_update(void)
{
	grid();
}


void game_exit(void)
{

}