#include "cprocessing.h"
#include "game.h"

//Tools
int Collision_Detection(Coordinates object1, Coordinates object2) {
	if (object1.objectType == objectCircle && object2.objectType == objectCircle) {
		float objectDistanceSquared = (object1.xOrigin - object2.xOrigin) * (object1.xOrigin - object2.xOrigin) + (object1.yOrigin - object2.yOrigin) * (object1.yOrigin - object2.yOrigin);//Get the squared distance between the two centre points
		if (objectDistanceSquared <= (object1.width + object2.width) * (object1.width + object2.width))//Compare with squared Radius of both object
			return 1;
		else
			return 0;
	}
	else {
		if (object1.objectType == objectCircle) {
			if (((object2.xOrigin <= object1.xOrigin) && (object1.xOrigin <= object2.xOrigin + object2.width)) && ((object2.yOrigin <= object1.yOrigin) && (object1.yOrigin <= object2.yOrigin + object2.height))) {
				return 1;
			}
			else {
				float distanceFromCornerSquared = 0.0f;
				if (object1.xOrigin < object2.xOrigin) {
					distanceFromCornerSquared += (object1.xOrigin - object2.xOrigin) * (object1.xOrigin - object2.xOrigin);
				}
				else {
					distanceFromCornerSquared += (object1.xOrigin - object2.width - object2.xOrigin) * (object1.xOrigin - object2.width - object2.xOrigin);
				}
				if (object1.yOrigin < object2.yOrigin) {
					distanceFromCornerSquared += (object1.yOrigin - object2.yOrigin) * (object1.yOrigin - object2.yOrigin);
				}
				else {
					distanceFromCornerSquared += (object1.yOrigin - object2.height - object2.yOrigin) * (object1.yOrigin - object2.height - object2.yOrigin);
				}
				if (distanceFromCornerSquared <= (object1.width * object1.width)) {
					return 1;
				}
				else {
					return 0;
				}
			}
		}
		else if (object2.objectType == objectCircle) {
			if (((object1.xOrigin <= object2.xOrigin) && (object2.xOrigin <= object1.xOrigin + object1.width)) && ((object1.yOrigin <= object2.yOrigin) && (object2.yOrigin <= object1.yOrigin + object1.height))) {
				return 1;
			}
			else {
				float distanceFromCornerSquared = 0.0f;
				if (object2.xOrigin < object1.xOrigin) {
					distanceFromCornerSquared += (object2.xOrigin - object1.xOrigin) * (object2.xOrigin - object1.xOrigin);
				}
				else {
					distanceFromCornerSquared += (object2.xOrigin - object1.width - object1.xOrigin) * (object2.xOrigin - object1.width - object1.xOrigin);
				}
				if (object2.yOrigin < object1.yOrigin) {
					distanceFromCornerSquared += (object2.yOrigin - object1.yOrigin) * (object2.yOrigin - object1.yOrigin);
				}
				else {
					distanceFromCornerSquared += (object2.yOrigin - object1.height - object1.yOrigin) * (object2.yOrigin - object1.height - object1.yOrigin);
				}
				if (distanceFromCornerSquared <= (object2.width * object2.width)) {
					return 1;
				}
				else {
					return 0;
				}
			}
		}
		else {
			if (object1.xOrigin < object2.xOrigin && object1.yOrigin < object2.yOrigin) {
				if ((object1.xOrigin + object1.width) >= object2.xOrigin && (object1.yOrigin + object1.height) >= object2.yOrigin) {
					return 1;
				}
				else {
					return 0;
				}
			}
			else if (object1.xOrigin < object2.xOrigin && object1.yOrigin > object2.yOrigin) {
				if ((object1.xOrigin + object1.width) >= object2.xOrigin && (object2.yOrigin + object2.height) >= object1.yOrigin) {
					return 1;
				}
				else {
					return 0;
				}
			}
			else if (object1.xOrigin > object2.xOrigin && object1.yOrigin < object2.yOrigin) {
				if ((object2.xOrigin + object2.width) >= object1.xOrigin && (object1.yOrigin + object1.height) >= object2.yOrigin) {
					return 1;
				}
				else {
					return 0;
				}
			}
			else {
				if ((object2.xOrigin + object2.width) >= object1.xOrigin && (object2.yOrigin + object2.height) >= object1.yOrigin) {
					return 1;
				}
				else {
					return 0;
				}
			}
		}
	}
}

void color_game_square(int rectRow, int rectCol, CP_Color squareColor)
{
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(squareColor);
	CP_Graphics_DrawRect((Game.xOrigin + Game.gridWidth * rectCol), (Game.yOrigin + Game.gridHeight * rectRow), (Game.gridWidth), (Game.gridHeight));
}

int btn_is_pressed(Coordinates objectButtonX) {
	return ((objectButtonX.xOrigin) <= MouseInput.xOrigin && MouseInput.xOrigin <= (objectButtonX.xOrigin + objectButtonX.width)) && ((objectButtonX.yOrigin) <= MouseInput.yOrigin && MouseInput.yOrigin <= (objectButtonX.yOrigin + objectButtonX.height)) ? 1 : 0;
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
	TurretButton0.objectType = objectRectangle;
}

void turret1_button_init(void) {
	TurretButton1.xOrigin = TurretMenu.xOrigin;
	TurretButton1.yOrigin = TurretButton0.yOrigin + TurretButton0.height; //1/4 of the space is for other uses
	TurretButton1.width = TurretMenu.width;
	TurretButton1.height = TurretButton0.height;
	TurretButton1.objectType = objectRectangle;
}

void turret2_button_init(void) {
	TurretButton2.xOrigin = TurretMenu.xOrigin;
	TurretButton2.yOrigin = TurretButton1.yOrigin + TurretButton1.height; //1/4 of the space is for other uses
	TurretButton2.width = TurretMenu.width;
	TurretButton2.height = TurretButton0.height;
	TurretButton2.objectType = objectRectangle;
}

void turret3_button_init(void) {
	TurretButton3.xOrigin = TurretMenu.xOrigin;
	TurretButton3.yOrigin = TurretButton2.yOrigin + TurretButton2.height; //1/4 of the space is for other uses
	TurretButton3.width = TurretMenu.width;
	TurretButton3.height = TurretButton0.height;
	TurretButton3.objectType = objectRectangle;
}

void pause_button_init(void){
	PauseButton.xOrigin = 0.0f;
	PauseButton.yOrigin = 0.0f;
	PauseButton.width = TurretMenu.width / 2;
	PauseButton.height = TurretButton0.yOrigin;
	PauseButton.objectType = objectRectangle;
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

void render_button(Coordinates TurretButtonX, CP_Color Color) {
	CP_Settings_RectMode(CP_POSITION_CORNER);
	if (Collision_Detection(MouseInput, TurretButtonX)) {
		CP_Settings_Fill(COLOR_GREEN);
	}
	else {
		CP_Settings_Fill(Color);
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

void mouse_init(void) {
	MouseInput.width = 0.0f;
	MouseInput.height = 0.0f;
	MouseInput.objectType = objectCircle;
	MouseInput.xOrigin = (float)CP_System_GetWindowWidth() / 2;
	MouseInput.yOrigin = (float)CP_System_GetWindowHeight() / 2;
}
