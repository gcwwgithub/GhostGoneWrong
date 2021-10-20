#include "cprocessing.h"
#include"game.h"

//Graphics
void draw_game_grid(void)
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

void color_game_grid(const LevelData Level) {
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

//Level
void game_square_color(LevelData* Level) {
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

//CollisionDetection;
void Initialize_object(void) {
	MouseInput.circleRadius = 0.0f;
	MouseInput.objectType = objectCircle;
	MouseInput.objectPositionX = 0.0f;
	MouseInput.objectPositionY = 0.0f;
	objectGameGrid.objectType = objectRectangle;
	objectGameGrid.objectPositionX = GAME_X_ORIGIN + (((GAME_GRID_COLS - 1) / 2.0f + 0.5f) * Game_Grid_Width);
	objectGameGrid.objectPositionY = GAME_Y_ORIGIN + (((GAME_GRID_ROWS - 1) / 2.0f + 0.5f) * Game_Grid_Height);
	objectGameGrid.rectLengthX = Game_Width;
	objectGameGrid.rectLengthY = Game_Height;
}

int Collision_Detection(ObjectData object1, ObjectData object2) {
	float objectDistanceSquared = (object1.objectPositionX - object2.objectPositionX) * (object1.objectPositionX - object2.objectPositionX) + (object1.objectPositionY - object2.objectPositionY) * (object1.objectPositionY - object2.objectPositionY);//Get the squared distance between the two centre points
	if (object1.objectType == objectCircle && object2.objectType == objectCircle) {
		if (objectDistanceSquared <= (object1.circleRadius + object2.circleRadius) * (object1.circleRadius + object2.circleRadius))//Compare with squared Radius of both object
			return 1;
		else
			return 0;
	}
	else {
		float rectLengthSquared;
		if (object1.objectType == objectCircle) {
			rectLengthSquared = (0.25f * object2.rectLengthX * object2.rectLengthX) + (0.25f * object2.rectLengthY * object2.rectLengthY);
			if (objectDistanceSquared <= (rectLengthSquared) + (object1.circleRadius * object1.circleRadius))
				return 1;
			else
				return 0;
		}
		else if (object2.objectType == objectCircle) {
			rectLengthSquared = (0.25f * object1.rectLengthX * object1.rectLengthX) + (0.25f * object1.rectLengthY * object1.rectLengthY);
			if (objectDistanceSquared <= (rectLengthSquared) + (object2.circleRadius * object2.circleRadius))
				return 1;
			else
				return 0;
		}
		else {
			float rectLengthSquared2;
			rectLengthSquared = (0.25f * object1.rectLengthX * object1.rectLengthX) + (0.25f * object1.rectLengthY * object1.rectLengthY);
			rectLengthSquared2 = (0.25f * object2.rectLengthX * object2.rectLengthX) + (0.25f * object2.rectLengthY * object2.rectLengthY);
			if (objectDistanceSquared <= (rectLengthSquared) + (rectLengthSquared2))
				return 1;
			else
				return 0;
		}
	}
}
