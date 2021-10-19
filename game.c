#include "cprocessing.h"
#include"game.h"

void square_color(LevelData* Level);
void draw_grid(void);
void color_grid(LevelData Level);
void click_on_square(void);


void game_init(void)
{
	CP_System_Fullscreen();
	currentGameState = MainMenu;

	float unusableScreenHeight, unusableScreenWidth;/*Height and Width not used for game, example menu*/
	unusableScreenHeight = (float)CP_System_GetWindowHeight() / 2; //Half the screeen is used for the game
	Game_Height = (float)CP_System_GetWindowHeight() - unusableScreenHeight;
	Game_Grid_Height = Game_Height / GAME_GRID_ROWS;
	Game_Grid_Width = Game_Height / GAME_GRID_ROWS; //Grid is a Square
	Game_Width = Game_Grid_Width * GAME_GRID_COLS;
	unusableScreenWidth = (float)CP_System_GetWindowWidth() - Game_Width;
	GAME_X_ORIGIN = unusableScreenWidth / 2; //To centralise the Grid
	GAME_Y_ORIGIN = unusableScreenHeight / 2; //Centre the game

	Tutorial.spawnRow = 0;
	Tutorial.spawnCol = (GAME_GRID_COLS - 1) / 2;
	Tutorial.exitRow = GAME_GRID_ROWS - 1;
	Tutorial.exitCol = (GAME_GRID_COLS - 1) / 2;

	square_color(&Tutorial);
}

void game_update(void)
{
	click_on_square();

	draw_grid();
	color_grid(Tutorial);

}
void game_exit(void)
{

}