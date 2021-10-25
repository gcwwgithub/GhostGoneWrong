#include "cprocessing.h"
#include "game.h"
#include "Gabriel.h"

//Tools
float myabs(float x) {
	return x < 0 ? x * -1 : x;
}

int btn_is_pressed(Coordinates object1) {
	if (((object1.xOrigin <= MouseInput.xOrigin) && (MouseInput.xOrigin <= object1.xOrigin + object1.width)) && ((object1.yOrigin <= MouseInput.yOrigin) && (MouseInput.yOrigin <= object1.yOrigin + object1.height))) {
		return 1;
	}
	else {
		return 0;
	}
}

void color_game_square(int rectRow, int rectCol, CP_Color squareColor)
{
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(squareColor);
	CP_Graphics_DrawRect((Game.xOrigin + Game.gridWidth * rectCol), (Game.yOrigin + Game.gridHeight * rectRow), (Game.gridWidth), (Game.gridHeight));
}

int Collision_Detection(Coordinates object1, Coordinates object2) {
	if (object1.objectType == objectCircle && object2.objectType == objectCircle) {
		float collisionDistanceSquared, distanceSquared, circle1Radius, circle2Radius;
		circle1Radius = 0.5f * object1.width;
		circle2Radius = 0.5f * object2.width;
		collisionDistanceSquared = (circle1Radius + circle2Radius) * (circle1Radius + circle2Radius);
		distanceSquared = ((object1.xOrigin - object2.xOrigin) * (object1.xOrigin - object2.xOrigin)) + ((object1.yOrigin - object2.yOrigin) * (object1.yOrigin - object2.yOrigin));
		if (collisionDistanceSquared >= distanceSquared) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (object1.objectType == objectCircle || object2.objectType == objectCircle) {
		float  distanceX, distanceY, circleRadius, rectWidthtoCheck, rectHeightToCheck, collisionDistanceX, collisionDistanceY, radiusSquared, distanceToCornerSquaredX, distanceToCornerSquaredY;
		distanceX = object1.xOrigin - object2.xOrigin;
		distanceX = myabs(distanceX);
		distanceY = object1.yOrigin - object2.yOrigin;
		distanceY = myabs(distanceY);
		if (object1.objectType == objectCircle) {
			circleRadius = 0.5f * object1.width;
			rectWidthtoCheck = 0.5f * object2.width;
			rectHeightToCheck = 0.5f * object2.height;
		}
		else {
			circleRadius = 0.5f * object2.width;
			rectWidthtoCheck = 0.5f * object1.width;
			rectHeightToCheck = 0.5f * object1.height;
		}
		collisionDistanceX = circleRadius + rectWidthtoCheck;
		collisionDistanceY = circleRadius + rectHeightToCheck;
		radiusSquared = circleRadius * circleRadius;
		distanceToCornerSquaredX = (distanceX - rectWidthtoCheck) * (distanceX - rectWidthtoCheck);
		distanceToCornerSquaredY = (distanceY - rectHeightToCheck) * (distanceX - rectHeightToCheck);
		if (collisionDistanceX >= distanceX && collisionDistanceY >= distanceY) {
			if (distanceX <= rectWidthtoCheck) {
				return 1;
			}
			else if (distanceY <= rectHeightToCheck) {
				return 1;
			}
			else if ((distanceToCornerSquaredX + distanceToCornerSquaredY) <= radiusSquared) {
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			return 0;
		}
	}
	else {
		float rect1WidthtoCheck, rect1HeighttoCheck, rect2WidthtoCheck, rect2HeighttoCheck, collisionDistanceX, collisionDistanceY, distanceX, distanceY;
		rect1WidthtoCheck = 0.5f * object1.width;
		rect1HeighttoCheck = 0.5f * object1.height;
		rect2WidthtoCheck = 0.5f * object2.width;
		rect2HeighttoCheck = 0.5f * object2.height;
		collisionDistanceX = (rect1WidthtoCheck + rect2WidthtoCheck);
		collisionDistanceY = (rect1HeighttoCheck + rect2HeighttoCheck);
		distanceX = (object1.xOrigin - object2.xOrigin);
		distanceX = myabs(distanceX);
		distanceY = (object1.yOrigin - object2.yOrigin);
		distanceY = myabs(distanceY);
		if (collisionDistanceX >= distanceX && collisionDistanceY >= distanceY) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

int check_game_button_pressed(void) {
	for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
		if (btn_is_pressed(GameButton[i])) {
			return i;
		}
	}
	return NoButton;

}

//Graphics
void mouse_init(void) {
	MouseInput.width = 0.0f;
	MouseInput.height = 0.0f;
	MouseInput.objectType = objectCircle;
	MouseInput.xOrigin = (float)CP_System_GetWindowWidth() / 2;
	MouseInput.yOrigin = (float)CP_System_GetWindowHeight() / 2;
}

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
	TurretMenu.width = Game.xOrigin / 2;
	TurretMenu.height = (float)CP_System_GetWindowHeight();
}

void turret0_button_init(void) {
	float unusableButtonHeight = TurretMenu.height / 4;
	GameButton[TurretButton0].xOrigin = TurretMenu.xOrigin;
	GameButton[TurretButton0].yOrigin = TurretMenu.yOrigin + unusableButtonHeight; //1/4 of the space is for other uses
	GameButton[TurretButton0].width = TurretMenu.width;
	GameButton[TurretButton0].height = (TurretMenu.height - unusableButtonHeight) / 4;
	GameButton[TurretButton0].objectType = objectRectangle;
	GameButton[TurretButton0].imageOfButton = triangleTurretImageArray[0];
}

void turret1_button_init(void) {
	GameButton[TurretButton1].xOrigin = TurretMenu.xOrigin;
	GameButton[TurretButton1].yOrigin = GameButton[0].yOrigin + GameButton[0].height; //1/4 of the space is for other uses
	GameButton[TurretButton1].width = TurretMenu.width;
	GameButton[TurretButton1].height = GameButton[0].height;
	GameButton[TurretButton1].objectType = objectRectangle;
	GameButton[TurretButton1].imageOfButton = tempCircle;
}

void turret2_button_init(void) {
	GameButton[TurretButton2].xOrigin = TurretMenu.xOrigin;
	GameButton[TurretButton2].yOrigin = GameButton[1].yOrigin + GameButton[1].height; //1/4 of the space is for other uses
	GameButton[TurretButton2].width = TurretMenu.width;
	GameButton[TurretButton2].height = GameButton[0].height;
	GameButton[TurretButton2].objectType = objectRectangle;
	GameButton[TurretButton2].imageOfButton = tempStar;
}

void turret3_button_init(void) {
	GameButton[TurretButton3].xOrigin = TurretMenu.xOrigin;
	GameButton[TurretButton3].yOrigin = GameButton[2].yOrigin + GameButton[2].height; //1/4 of the space is for other uses
	GameButton[TurretButton3].width = TurretMenu.width;
	GameButton[TurretButton3].height = GameButton[0].height;
	GameButton[TurretButton3].objectType = objectRectangle;
	GameButton[TurretButton3].imageOfButton = tempPercentage;
}

void pause_button_init(void) {
	GameButton[PauseButton].xOrigin = TurretMenu.xOrigin;
	GameButton[PauseButton].yOrigin = TurretMenu.yOrigin;
	GameButton[PauseButton].width = TurretMenu.width / 2;
	GameButton[PauseButton].height = GameButton[0].yOrigin;
	GameButton[PauseButton].objectType = objectRectangle;
	GameButton[PauseButton].imageOfButton = CP_Image_Load("Assets/Dummy");
}

void render_button_pressed(void) {
	if (NoButton == check_game_button_pressed()) {
		isPlacingTurret = FALSE;
	}
	else {
		isPlacingTurret = TRUE;
		CP_Image_DrawAdvanced(GameButton[check_game_button_pressed()].imageOfButton, CP_Input_GetMouseX(), CP_Input_GetMouseY(), Game.gridWidth, Game.gridHeight, 255, 0);
	}
}

//void render_button_pressed(void) {
//	if (check_game_button_pressed() == TurretButton0) {
//		isPlacingTurret = TRUE;
//		CP_Settings_Fill(COLOR_GREEN);
//		CP_Graphics_DrawCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 20.0f);
//	}
//	else if (check_game_button_pressed() == TurretButton1) {
//		isPlacingTurret = TRUE;
//		CP_Settings_Fill(COLOR_GREEN);
//		CP_Graphics_DrawCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 20.0f);
//	}
//	else if (check_game_button_pressed() == TurretButton2) {
//		isPlacingTurret = TRUE;
//		CP_Settings_Fill(COLOR_GREEN);
//		CP_Graphics_DrawCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 20.0f);
//	}
//	else if (check_game_button_pressed() == TurretButton3) {
//		isPlacingTurret = TRUE;
//		CP_Settings_Fill(COLOR_GREEN);
//		CP_Graphics_DrawCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 20.0f);
//	}
//	else if (check_game_button_pressed() == PauseButton) {
//		if (currentGameState == Pause) {
//			currentGameState = Wave;
//			MouseInput.xOrigin = (float)CP_System_GetWindowWidth() / 2;
//			MouseInput.yOrigin = (float)CP_System_GetWindowHeight() / 2;
//		}
//		else {
//			currentGameState = Pause;
//			MouseInput.xOrigin = (float)CP_System_GetWindowWidth() / 2;
//			MouseInput.yOrigin = (float)CP_System_GetWindowHeight() / 2;
//		}
//	}
//	else {
//		;
//	}
//}

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

void render_button(Coordinates ButtonX, CP_Color Color) {
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(Color);
	CP_Graphics_DrawRect(ButtonX.xOrigin, ButtonX.yOrigin, ButtonX.width, ButtonX.height);

	if (ButtonX.imageOfButton != GameButton[4].imageOfButton)
	{
		CP_Image_DrawAdvanced(ButtonX.imageOfButton, ButtonX.width / 4,
			(ButtonX.yOrigin + ButtonX.height / 2), 128,
			128, 255, 0);
	}


}

void render_new_turret(void) {
	Coordinates GameTemp;
	GameTemp.xOrigin = Game.xOrigin;
	GameTemp.yOrigin = Game.yOrigin;
	GameTemp.width = Game.width;
	GameTemp.height = Game.height;
	if (btn_is_pressed(GameTemp)) {
		if (isPlacingTurret == TRUE) {
			float drawX, drawY;
			drawX = ((int)((CP_Input_GetMouseX() - Game.xOrigin) / Game.gridWidth) + 0.5f) * Game.gridWidth + Game.xOrigin;
			drawY = ((int)((CP_Input_GetMouseY() - Game.yOrigin) / Game.gridHeight) + 0.5f) * Game.gridHeight + Game.yOrigin;
			CP_Image_DrawAdvanced(GameButton[check_game_button_pressed()].imageOfButton, drawX, drawY, Game.gridWidth, Game.gridHeight, 255, 0);
			isPlacingTurret = FALSE;
		}
	}
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

