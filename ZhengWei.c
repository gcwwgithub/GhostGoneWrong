#include "cprocessing.h"
#include"game.h"

void grid(void)
{
	int currentGridRow = 0, currentGridCol = 0;
	CP_Graphics_ClearBackground(color_Grey);
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