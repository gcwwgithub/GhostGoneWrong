#include "cprocessing.h"
#include"game.h"

void draw_grid(void)
{
	int currentGridRow = 0, currentGridCol = 0;
	CP_Graphics_ClearBackground(COLOR_WHITE);
	CP_Settings_Fill(COLOR_BLACK);
	while (currentGridRow <= GAME_GRID_ROWS) {
		CP_Graphics_DrawLine((GAME_X_ORIGIN), (GAME_Y_ORIGIN + Game_Grid_Height * currentGridRow), (GAME_X_ORIGIN + Game_Width), (GAME_Y_ORIGIN + Game_Grid_Height * currentGridRow));
		currentGridRow++;
	}
	while (currentGridCol <= GAME_GRID_COLS) {
		CP_Graphics_DrawLine((GAME_X_ORIGIN + Game_Grid_Width * currentGridCol), (GAME_Y_ORIGIN), (GAME_X_ORIGIN + Game_Grid_Width * currentGridCol), (GAME_Y_ORIGIN + Game_Height));
		currentGridCol++;
	}
}
void color_square(int rectRow, int rectCol, CP_Color squareColor)
{
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(squareColor);
	CP_Graphics_DrawRect((GAME_X_ORIGIN + Game_Grid_Width * rectCol), (GAME_Y_ORIGIN + Game_Grid_Height * rectRow), (Game_Grid_Width), (Game_Grid_Height));
}

void color_grid(const LevelData Level) {
	for (int i = 0; i < GAME_GRID_ROWS; i++) {
		for (int j = 0; j < GAME_GRID_COLS; j++) {
			if (Level.gridColor[i][j] == Grid_Color_Grey)
				color_square(i, j, COLOR_GREY);
			else if (Level.gridColor[i][j] == Grid_Color_Red)
				color_square(i, j, COLOR_RED);
			else if (Level.gridColor[i][j] == Grid_Color_Blue)
				color_square(i, j, COLOR_BLUE);
			else {}
		}
	}
}

void square_color(LevelData* Level) {
	for (int i = 0; i < GAME_GRID_ROWS; i++) {
		for (int j = 0; j < GAME_GRID_COLS; j++) {
			Level->gridColor[i][j] = Grid_Color_White;
		}
	}
	Level->gridColor[Level->spawnRow][Level->spawnCol] = Grid_Color_Red;
	Level->gridColor[Level->exitRow][Level->exitCol] = Grid_Color_Blue;
	int colApart, rowApart, counter = 1;
	colApart = Level->exitCol - Level->spawnCol;
	rowApart = Level->exitRow - Level->spawnRow;
	if (rowApart < 0) {
		counter = rowApart;
		rowApart = -1;
	}
	while (counter <= rowApart) {
		if (Level->spawnCol != Level->exitCol || Level->spawnRow + counter != Level->exitRow) {
			Level->gridColor[Level->spawnRow + counter][Level->spawnCol] = Grid_Color_Grey;
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
			Level->gridColor[Level->exitRow][Level->spawnCol + counter] = Grid_Color_Grey;
		}
		counter++;
	}
}