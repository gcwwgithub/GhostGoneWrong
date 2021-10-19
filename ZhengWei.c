#include "cprocessing.h"
#include"game.h"

void draw_grid(void)
{
	int currentGridRow = 0, currentGridCol = 0;
	CP_Graphics_ClearBackground(COLOR_WHITE);
	CP_Settings_Fill(color_Black);
	while (currentGridRow <= GRID_ROWS) {
		CP_Graphics_DrawLine((X_ORIGIN), (Y_ORIGIN + gridHeight * currentGridRow), (X_ORIGIN + gameWidth), (Y_ORIGIN + gridHeight * currentGridRow));
		currentGridRow++;
	}
	while (currentGridCol <= GRID_COLS) {
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
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; j++) {
			if (Level.gridColor[i][j] == GRID_COLOR_GREY)
				color_square(i, j, COLOR_GREY);
			else if (Level.gridColor[i][j] == GRID_COLOR_RED)
				color_square(i, j, COLOR_RED);
			else if (Level.gridColor[i][j] == GRID_COLOR_BLUE)
				color_square(i, j, COLOR_BLUE);
			else {}
		}
	}
}

void square_color(LevelData* Level) {
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; j++) {
			Level->gridColor[i][j] = GRID_COLOR_WHITE;
		}
	}
	Level->gridColor[Level->spawnRow][Level->spawnCol] = GRID_COLOR_RED;
	Level->gridColor[Level->exitRow][Level->exitCol] = GRID_COLOR_BLUE;
	int colApart, rowApart, counter = 1;
	colApart = Level->exitCol - Level->spawnCol;
	rowApart = Level->exitRow - Level->spawnRow;
	if (rowApart < 0) {
		counter = rowApart;
		rowApart = -1;
	}
	while (counter <= rowApart) {
		if (Level->spawnCol != Level->exitCol || Level->spawnRow + counter != Level->exitRow) {
			Level->gridColor[Level->spawnRow + counter][Level->spawnCol] = GRID_COLOR_GREY;
		}
		counter++;
	}
	if (colApart < 0) {
		counter = colApart;
		colApart = 0;
	}
	else
		counter = 1;
	while (counter <= colApart) {
		if (Level->spawnCol + counter != Level->exitCol) {
			Level->gridColor[Level->exitRow][Level->spawnCol + counter] = GRID_COLOR_GREY;
		}
		counter++;
	}
}