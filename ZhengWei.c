#include "cprocessing.h"
#include"game.h"

void draw_grid(void)
{
	int currentGridRow = 0, currentGridCol = 0;
	CP_Graphics_ClearBackground(COLOR_WHITE);
	CP_Settings_Fill(color_Black);
	while (currentGridRow <= Grid_Rows) {
		CP_Graphics_DrawLine((X_ORIGIN), (Y_ORIGIN + gridHeight * currentGridRow), (X_ORIGIN + gameWidth), (Y_ORIGIN + gridHeight * currentGridRow));
		currentGridRow++;
	}
	while (currentGridCol <= Grid_Cols) {
		CP_Graphics_DrawLine((X_ORIGIN + gridWidth * currentGridCol), (Y_ORIGIN), (X_ORIGIN + gridWidth * currentGridCol), (Y_ORIGIN + gameHeight));
		currentGridCol++;
	}
}
void color_square(int rectRow, int rectCol, CP_Color squareColor)
{
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(squareColor);
	CP_Graphics_DrawRect((X_ORIGIN + gridWidth * rectCol), (Y_ORIGIN + gridHeight * rectRow), (gridWidth), (gridHeight));
}