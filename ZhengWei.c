#include "cprocessing.h"
#include "game.h"
#include "vector.h"
#include <math.h>

//Tools
int Collision_Detection(ObjectData object1, ObjectData object2) {
	float objectDistanceSquared = (object1.objectPositionX - object2.objectPositionX) * (object1.objectPositionX - object2.objectPositionX) + (object1.objectPositionY - object2.objectPositionY) * (object1.objectPositionY - object2.objectPositionY);//Get the squared distance between the two centre points
	if (object1.objectType == objectCircle && object2.objectType == objectCircle) {
		if (objectDistanceSquared <= (object1.circleRadius + object2.circleRadius) * (object1.circleRadius + object2.circleRadius))//Compare with squared Radius of both object
			return 1;
		else
			return 0;
	}
	else {
		float collisionDistanceSquared;
		Vector2 RectVectorX, RectVectorY, DistanceFromPoint;
		if (object1.objectType == objectCircle) {
			RectVectorX.pos_x = 0.5f * object2.rectLengthX;
			RectVectorX.pos_y = 0.0f;
			RectVectorY.pos_x = 0.0f;
			RectVectorY.pos_y = 0.5f * object2.rectLengthY;
			DistanceFromPoint.pos_x = object1.objectPositionX - object2.objectPositionX;
			DistanceFromPoint.pos_y = object1.objectPositionY - object2.objectPositionY;
			collisionDistanceSquared = (dot(RectVectorX, DistanceFromPoint) + dot(RectVectorY, DistanceFromPoint)) / magnitude(DistanceFromPoint) + (object1.circleRadius);
			collisionDistanceSquared = collisionDistanceSquared * collisionDistanceSquared;
			
			if (objectDistanceSquared <= collisionDistanceSquared)
				return 1;
			else
				return 0;
		}
		else if (object2.objectType == objectCircle) {
			
			RectVectorX.pos_x = 0.5f * object1.rectLengthX;
			RectVectorX.pos_y = 0.0f;
			RectVectorY.pos_x = 0.0f;
			RectVectorY.pos_y = 0.5f * object1.rectLengthY;
			DistanceFromPoint.pos_x = object2.objectPositionX - object1.objectPositionX;
			DistanceFromPoint.pos_y = object2.objectPositionY - object1.objectPositionY;
			collisionDistanceSquared = (dot(RectVectorX, DistanceFromPoint) + dot(RectVectorY, DistanceFromPoint)) / magnitude(DistanceFromPoint) + (object1.circleRadius);
			collisionDistanceSquared = collisionDistanceSquared * collisionDistanceSquared;
			if (objectDistanceSquared <= collisionDistanceSquared)
				return 1;
			else
				return 0;
		}
		else {
			
			Vector2 Rect2VectorX, Rect2VectorY;
			RectVectorX.pos_x = 0.5f * object1.rectLengthX;
			RectVectorX.pos_y = 0.0f;
			RectVectorY.pos_x = 0.0f;
			RectVectorY.pos_y = 0.5f * object1.rectLengthY;
			DistanceFromPoint.pos_x = object2.objectPositionX - object1.objectPositionX;
			DistanceFromPoint.pos_y = object2.objectPositionY - object1.objectPositionY;

			Rect2VectorX.pos_x = -0.5f * object2.rectLengthX;
			Rect2VectorX.pos_y = 0.0f;
			Rect2VectorY.pos_x = 0.0f;
			Rect2VectorY.pos_y = -0.5f * object2.rectLengthY;

			collisionDistanceSquared = (dot(RectVectorX, DistanceFromPoint) + dot(RectVectorY, DistanceFromPoint)) / magnitude(DistanceFromPoint) + (object1.circleRadius);
			collisionDistanceSquared = collisionDistanceSquared * collisionDistanceSquared;

			if (objectDistanceSquared <= collisionDistanceSquared)
				return 1;
			else
				return 0;
		}
	}
}

void color_game_square(int rectRow, int rectCol, CP_Color squareColor)
{
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(squareColor);
	CP_Graphics_DrawRect((Game.xOrigin + Game.gridWidth * rectCol), (Game.yOrigin + Game.gridHeight * rectRow), (Game.gridWidth), (Game.gridHeight));
}

//Graphics
void game_grid_init(void) {
	float unusableScreenHeight, unusableScreenWidth;/*Height and Width not used for game, example menu*/
	unusableScreenHeight = (float)CP_System_GetWindowHeight() / 2; //Half the screeen is used for the game
	Game.height = (float)CP_System_GetWindowHeight() - unusableScreenHeight;
	Game.gridHeight = Game.height / GAME_GRID_ROWS;
	Game.gridWidth = Game.height / GAME_GRID_ROWS; //Grid is a Square
	Game.width = Game.gridWidth * GAME_GRID_COLS;
	unusableScreenWidth = (float)CP_System_GetWindowWidth() - Game.width;
	Game.xOrigin = unusableScreenWidth / 2; //To centralise the Grid
	Game.yOrigin = unusableScreenHeight / 2; //Centre the game

}

void turret_menu_init(void) {
	TurretMenu.xOrigin = 0.0f;
	TurretMenu.yOrigin = 0.0f;
	TurretMenu.width = Game.xOrigin;
	TurretMenu.height = (float)CP_System_GetWindowHeight();
}

void turret0_button_init(void) {
	float unusableButtonHeight = TurretMenu.height / 4;
	TurretButton0.xOrigin = TurretMenu.xOrigin;
	TurretButton0.yOrigin = TurretMenu.yOrigin + unusableButtonHeight; //1/4 of the space is for other uses
	TurretButton0.width = TurretMenu.width;
	TurretButton0.height = (TurretMenu.height - unusableButtonHeight) / 4;
}

void turret1_button_init(void) {
	TurretButton1.xOrigin = TurretMenu.xOrigin;
	TurretButton1.yOrigin = TurretButton0.yOrigin + TurretButton0.height; //1/4 of the space is for other uses
	TurretButton1.width = TurretMenu.width;
	TurretButton1.height = TurretButton0.height;
}

void turret2_button_init(void) {
	TurretButton2.xOrigin = TurretMenu.xOrigin;
	TurretButton2.yOrigin = TurretButton1.yOrigin + TurretButton1.height; //1/4 of the space is for other uses
	TurretButton2.width = TurretMenu.width;
	TurretButton2.height = TurretButton0.height;
}

void turret3_button_init(void) {
	TurretButton3.xOrigin = TurretMenu.xOrigin;
	TurretButton3.yOrigin = TurretButton2.yOrigin + TurretButton2.height; //1/4 of the space is for other uses
	TurretButton3.width = TurretMenu.width;
	TurretButton3.height = TurretButton0.height;
}

void render_game_grid(void)
{
	int currentGridRow = 0, currentGridCol = 0;
	CP_Graphics_ClearBackground(COLOR_WHITE);
	CP_Settings_Fill(COLOR_BLACK);
	while (currentGridRow <= GAME_GRID_ROWS) {
		CP_Graphics_DrawLine((Game.xOrigin), (Game.yOrigin + Game.gridHeight * currentGridRow), (Game.xOrigin + Game.width), (Game.yOrigin + Game.gridHeight * currentGridRow));
		currentGridRow++;
	}
	while (currentGridCol <= GAME_GRID_COLS) {
		CP_Graphics_DrawLine((Game.xOrigin + Game.gridWidth * currentGridCol), (Game.yOrigin), (Game.xOrigin + Game.gridWidth * currentGridCol), (Game.yOrigin + Game.height));
		currentGridCol++;
	}
}

void render_turret_menu(void) {
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(COLOR_BLACK);
	CP_Graphics_DrawRect(TurretMenu.xOrigin, TurretMenu.yOrigin, TurretMenu.width, TurretMenu.height);
}

void render_turret_button(Coordinates TurretButtonX, ObjectData ObjectTurretButtonX) {
	CP_Settings_RectMode(CP_POSITION_CORNER);
	if (Collision_Detection(MouseInput, ObjectTurretButtonX)) {
		CP_Settings_Fill(COLOR_GREEN);
	}
	else {
		CP_Settings_Fill(COLOR_WHITE);
	}
	CP_Graphics_DrawRect(TurretButtonX.xOrigin, TurretButtonX.yOrigin, TurretButtonX.width, TurretButtonX.height);
}

//Level
void game_grid_color_init(LevelData* Level) {
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

void render_game_grid_color(LevelData Level) {
	for (int i = 0; i < GAME_GRID_ROWS; i++) {
		for (int j = 0; j < GAME_GRID_COLS; j++) {
			if (Level.gridColor[i][j] == Grid_Color_Grey)
				color_game_square(i, j, COLOR_GREY);
			else if (Level.gridColor[i][j] == Grid_Color_Red)
				color_game_square(i, j, COLOR_RED);
			else if (Level.gridColor[i][j] == Grid_Color_Blue)
				color_game_square(i, j, COLOR_BLUE);
			else {}
		}
	}
}

void object_init(void) {
	MouseInput.circleRadius = 0.0f;
	MouseInput.objectType = objectCircle;
	MouseInput.objectPositionX = (float)CP_System_GetWindowWidth() / 2;
	MouseInput.objectPositionY = (float)CP_System_GetWindowHeight() / 2;
	
	ObjectTurretButton0.objectPositionX = TurretButton0.xOrigin + 0.5f * TurretButton0.width;
	ObjectTurretButton0.objectPositionY = TurretButton0.yOrigin + 0.5f * TurretButton0.height;
	ObjectTurretButton0.rectLengthX = TurretButton0.width;
	ObjectTurretButton0.rectLengthY = TurretButton0.height;
	ObjectTurretButton0.objectType = objectRectangle;
	
	ObjectTurretButton1.objectPositionX = TurretButton1.xOrigin + 0.5f * TurretButton1.width;
	ObjectTurretButton1.objectPositionY = TurretButton1.yOrigin + 0.5f * TurretButton1.height;
	ObjectTurretButton1.rectLengthX = TurretButton1.width;
	ObjectTurretButton1.rectLengthY = TurretButton1.height;
	ObjectTurretButton1.objectType = objectRectangle;

	ObjectTurretButton2.objectPositionX = TurretButton2.xOrigin + 0.5f * TurretButton2.width;
	ObjectTurretButton2.objectPositionY = TurretButton2.yOrigin + 0.5f * TurretButton2.height;
	ObjectTurretButton2.rectLengthX = TurretButton2.width;
	ObjectTurretButton2.rectLengthY = TurretButton2.height;
	ObjectTurretButton2.objectType = objectRectangle;

	ObjectTurretButton3.objectPositionX = TurretButton3.xOrigin + 0.5f * TurretButton3.width;
	ObjectTurretButton3.objectPositionY = TurretButton3.yOrigin + 0.5f * TurretButton3.height;
	ObjectTurretButton3.rectLengthX = TurretButton3.width;
	ObjectTurretButton3.rectLengthY = TurretButton3.height;
	ObjectTurretButton3.objectType = objectRectangle;

}
