#include "cprocessing.h"
#include"game.h"

void draw_grid(void);
<<<<<<< HEAD
void color_grid(LevelData Level);
=======
void color_square(int rectRow, int rectCol, CP_Color squareColor);
void click_on_square(void);
>>>>>>> 36cd720fc61bc865324a4d9aad9d3b027fbef5fb

void game_init(void)
{
	CP_System_Fullscreen();
	float unusableScreenHeight/*Height not used for game, example menu*//*, unusableScreenWidth*//*Width not used for game, example menu*/;
	//unusableScreenWidth = 960.0f;
	unusableScreenHeight = 540.0f;
	//gameWidth = (float)CP_System_GetWindowWidth() - unusableScreenWidth;
	gameHeight = (float)CP_System_GetWindowHeight() - unusableScreenHeight;
	//gridWidth = gameWidth / Grid_Cols;
	gridWidth = gameHeight / Grid_Rows;
	gridHeight = gameHeight / Grid_Rows;
	gameWidth = gridWidth * Grid_Cols;
	X_ORIGIN = ((float)CP_System_GetWindowWidth() - gameWidth) / 2; //To centralise the Grid
	currentGameState = MainMenu;
	Tutorial.spawnRow = 0;
	Tutorial.spawnCol = (Grid_Cols - 1) / 2;
	Tutorial.exitRow = Grid_Rows - 1;
	Tutorial.exitCol = (Grid_Cols - 1) / 2;
	Level1.spawnRow = 5;
	Level1.spawnCol = 0;
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