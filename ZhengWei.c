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

void color_grid(const LevelData Level) {
	color_square(Level.spawnRow, Level.spawnCol, COLOR_RED);
	color_square(Level.exitRow, Level.exitCol, COLOR_BLUE);
	int gridsApart, counter;
	if (Level.spawnRow == Level.exitRow) {
		gridsApart = Level.exitCol - Level.spawnCol;
		if (gridsApart < 0) {
			counter = -1;
			while (counter > gridsApart) {
				color_square(Level.spawnRow, Level.spawnCol + counter, COLOR_GREY);
				counter--;
			}
		}
		else {
			counter = 1;
			while (counter < gridsApart) {
				color_square(Level.spawnRow, Level.spawnCol + counter, COLOR_GREY);
				counter++;
			}
		}
	}
	else if (Level.spawnCol == Level.exitCol) {
		gridsApart = Level.exitRow - Level.spawnRow;
		if (gridsApart < 0) {
			counter = -1;
			while (counter > gridsApart) {
				color_square(Level.spawnRow + counter, Level.spawnCol, COLOR_GREY);
				counter--;
			}
		}
		else {
			counter = 1;
			while (counter < gridsApart) {
				color_square(Level.spawnRow + counter, Level.spawnCol, COLOR_GREY);
				counter++;
			}
		}
	}
}