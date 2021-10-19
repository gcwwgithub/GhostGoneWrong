#include "cprocessing.h"
#include"game.h"

void draw_grid(void);
void color_grid(LevelData Level);
void click_on_square(void);

void game_init(void)
{
	CP_System_Fullscreen();
	float unusableScreenHeight/*Height not used for game, example menu*//*, unusableScreenWidth*//*Width not used for game, example menu*/;
	//unusableScreenWidth = 960.0f;
	unusableScreenHeight = 540.0f;
	//gameWidth = (float)CP_System_GetWindowWidth() - unusableScreenWidth;
	gameHeight = (float)CP_System_GetWindowHeight() - unusableScreenHeight;
	//gridWidth = gameWidth / Grid_Cols;
	gridWidth = gameHeight / GRID_ROWS;
	gridHeight = gameHeight / GRID_ROWS;
	gameWidth = gridWidth * GRID_COLS;
	X_ORIGIN = ((float)CP_System_GetWindowWidth() - gameWidth) / 2; //To centralise the Grid
	currentGameState = MainMenu;
	Tutorial.spawnRow = 0;
	Tutorial.spawnCol = (GRID_COLS - 1) / 2;
	Tutorial.exitRow = GRID_ROWS - 1;
	Tutorial.exitCol = (GRID_COLS - 1) / 2;
	Level1.spawnRow = 3;
	Level1.spawnCol = 2;
	Level1.exitRow = 0;
	Level1.exitCol = 0;
}

void game_update(void)
{
	click_on_square();

	draw_grid();
	color_grid(Level1);

}
void game_exit(void)
{

}