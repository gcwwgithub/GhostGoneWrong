#include "cprocessing.h"
#include"game.h"

void draw_grid(void)
{
	int currentGridRow = 0, currentGridCol = 0;
	CP_Graphics_ClearBackground(color_White);
	CP_Settings_Fill(color_Black);
	while (currentGridRow <= grid_Rows) {
		CP_Graphics_DrawLine((x_Origin), (y_Origin + gridHeight * currentGridRow), (x_Origin + gameWidth), (y_Origin + gridHeight * currentGridRow));
		currentGridRow++;
	}
	while (currentGridCol <= grid_Cols) {
		CP_Graphics_DrawLine((x_Origin + gridWidth * currentGridCol), (y_Origin), (x_Origin + gridWidth * currentGridCol), (y_Origin + gameHeight));
		currentGridCol++;
	}
}
void color_square(int rectRow, int rectCol, CP_Color squareColor)
{
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(squareColor);
	CP_Graphics_DrawRect((x_Origin + gridWidth * rectCol), (y_Origin + gridHeight * rectRow), (gridWidth), (gridHeight));
}