#include "cprocessing.h"
#include"game.h"

void draw_grid(void);
void color_square(int rectRow, int rectCol, CP_Color squareColor);
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
	gridWidth = gameHeight / Grid_Rows;
	gridHeight = gameHeight / Grid_Rows;
	gameWidth = gridWidth * Grid_Cols;
	X_ORIGIN = ((float)CP_System_GetWindowWidth() - gameWidth) / 2; //To centralise the Grid
	currentGameState = MainMenu;
}

void game_update(void)
{
	click_on_square();

	draw_grid();
	int currentRow = 1;
	color_square(0, (Grid_Cols - 1) / 2, COLOR_RED);
	color_square(Grid_Rows - 1, (Grid_Cols - 1) / 2, COLOR_BLUE);
	while (currentRow < (Grid_Rows - 1)) {
		color_square(currentRow, (Grid_Cols - 1) / 2, COLOR_GREY);
		currentRow++;
	}

}
void game_exit(void)
{

}