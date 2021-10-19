#include "cprocessing.h"
#include"game.h"

float  gridWidth, gridHeight, gameWidth, gameHeight;
void draw_grid(void);
void color_square(int rectRow, int rectCol, CP_Color squareColor);

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
	draw_grid();
	int currentRow = 1;
	color_square(0, (grid_Cols - 1) / 2, color_Red);
	color_square(grid_Rows - 1, (grid_Cols - 1) / 2, color_Blue);
	while (currentRow < (grid_Rows - 1)) {
		color_square(currentRow, (grid_Cols - 1) / 2, color_Grey);
		currentRow++;
	}

}
void game_exit(void)
{

}