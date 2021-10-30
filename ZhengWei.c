#include "cprocessing.h"
#include"ZhengWei.h"
#include "game.h"
#include "Gabriel.h"
#include"Samuel.h"

//Tools

//Return positive value of float
float myabs(float x) {
	return x < 0 ? x * -1 : x;
}

//Check if button is pressed
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

//Path Finding
int is_destination_updated(LevelData* LevelX) {
	return LevelX->grid[LevelX->exitRow][LevelX->exitCol].visited;
}


//Update neighbors cost
void pathfinding_update_neighbor_cost(int gridRow, int gridCol, int generation, LevelData* LevelX) {
	//Update Row Neighbor
	for (int i = -1; i <= 1; i++) {
		if (gridRow + i >= 0 && gridRow + i < GAME_GRID_ROWS) {
			if (LevelX->grid[gridRow + i][gridCol].visited == 0) {
				LevelX->grid[gridRow + i][gridCol].cost = generation + 1;
				LevelX->grid[gridRow + i][gridCol].parentRow = gridRow;
				LevelX->grid[gridRow + i][gridCol].parentCol = gridCol;
				LevelX->grid[gridRow + i][gridCol].visited = 1;
			}
		}
	}
	//Update Col Neighbor
	for (int i = -1; i <= 1; i++) {
		if (gridCol + i >= 0 && gridCol + i < GAME_GRID_COLS) {
			if (LevelX->grid[gridRow][gridCol + i].visited == 0) {
				LevelX->grid[gridRow][gridCol + i].cost = generation + 1;
				LevelX->grid[gridRow][gridCol + i].parentRow = gridRow;
				LevelX->grid[gridRow][gridCol + i].parentCol = gridCol;
				LevelX->grid[gridRow][gridCol + i].visited = 1;
			}
		}
	}
}

//Calculate all grid cost. Find the squares in the same generation and call a function to update neighbors.
void pathfinding_calculate_cost(LevelData* LevelX) {
	for (int currentCost = 0; !is_destination_updated(LevelX) && currentCost <= GAME_GRID_ROWS * GAME_GRID_COLS; currentCost++) {
		for (int i = 0; i < GAME_GRID_ROWS; i++) {
			for (int j = 0; j < GAME_GRID_COLS; j++) {
				if (LevelX->grid[i][j].cost == currentCost) {
					pathfinding_update_neighbor_cost(i, j, currentCost, LevelX);
				}
			}
		}
	}
}


//Collision Detection between circles and squares
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

//Check which button is pressed
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

void turret_triangle_button_init(void) {
	float unusableButtonHeight = TurretMenu.height / 4;
	GameButton[TurretButtonBasic].xOrigin = TurretMenu.xOrigin;
	GameButton[TurretButtonBasic].yOrigin = TurretMenu.yOrigin + unusableButtonHeight; //1/4 of the space is for other uses
	GameButton[TurretButtonBasic].width = TurretMenu.width;
	GameButton[TurretButtonBasic].height = (TurretMenu.height - unusableButtonHeight) / 4;
	GameButton[TurretButtonBasic].objectType = objectRectangle;
	//GameButton[TurretButtonBasic].imageOfButton = slowTurretImageArray[0];
}

void turret_circle_button_init(void) {
	GameButton[TurretButtonSlow].xOrigin = TurretMenu.xOrigin;
	GameButton[TurretButtonSlow].yOrigin = GameButton[TurretButtonBasic].yOrigin + GameButton[TurretButtonBasic].height; //1/4 of the space is for other uses
	GameButton[TurretButtonSlow].width = TurretMenu.width;
	GameButton[TurretButtonSlow].height = GameButton[TurretButtonBasic].height;
	GameButton[TurretButtonSlow].objectType = objectRectangle;
	GameButton[TurretButtonSlow].imageOfButton = slowTurretImageArray[0];
}

void turret_star_button_init(void) {
	GameButton[TurretButtonHoming].xOrigin = TurretMenu.xOrigin;
	GameButton[TurretButtonHoming].yOrigin = GameButton[TurretButtonSlow].yOrigin + GameButton[TurretButtonSlow].height; //1/4 of the space is for other uses
	GameButton[TurretButtonHoming].width = TurretMenu.width;
	GameButton[TurretButtonHoming].height = GameButton[TurretButtonBasic].height;
	GameButton[TurretButtonHoming].objectType = objectRectangle;
	//GameButton[TurretButtonHoming].imageOfButton = tempStar;
}

void turret_percentage_button_init(void) {
	GameButton[TurretButtonMine].xOrigin = TurretMenu.xOrigin;
	GameButton[TurretButtonMine].yOrigin = GameButton[TurretButtonHoming].yOrigin + GameButton[TurretButtonHoming].height; //1/4 of the space is for other uses
	GameButton[TurretButtonMine].width = TurretMenu.width;
	GameButton[TurretButtonMine].height = GameButton[TurretButtonBasic].height;
	GameButton[TurretButtonMine].objectType = objectRectangle;
	//GameButton[TurretButtonMine].imageOfButton = tempPercentage;
}

void pause_button_init(void) {
	GameButton[PauseButton].xOrigin = TurretMenu.xOrigin;
	GameButton[PauseButton].yOrigin = TurretMenu.yOrigin;
	GameButton[PauseButton].width = TurretMenu.width / 2;
	GameButton[PauseButton].height = GameButton[TurretButtonBasic].yOrigin;
	GameButton[PauseButton].objectType = objectRectangle;
	GameButton[PauseButton].imageOfButton = pauseButtonImage;
}

void render_button_pressed(void) {
	if (NoButton == check_game_button_pressed()) {
		isPlacingTurret = NOT_PLACING_TURRET;
	}
	else if (PauseButton == check_game_button_pressed()) {
		if (currentGameState == Pause)
		{
			currentGameState = Wave;
		}
		else // if the game is not paused
		{
			currentGameState = Pause;
		}
		MouseInput.xOrigin = (float)CP_System_GetWindowWidth() / 2;
		MouseInput.yOrigin = (float)CP_System_GetWindowHeight() / 2;
	}
	else {
		if (TurretButtonBasic == check_game_button_pressed()) {
			isPlacingTurret = T_BASIC;
		}
		else if (TurretButtonSlow == check_game_button_pressed()) {
			isPlacingTurret = T_SLOW;
		}
		else if (TurretButtonHoming == check_game_button_pressed()) {
			isPlacingTurret = T_HOMING;
		}
		else if (TurretButtonMine == check_game_button_pressed()) {
			isPlacingTurret = T_MINE;
		}
		CP_Image_DrawAdvanced(GameButton[check_game_button_pressed()].imageOfButton, CP_Input_GetMouseX(), CP_Input_GetMouseY(), Game.gridWidth, Game.gridHeight, 255, 0);
	}
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

void render_button(Coordinates ButtonX, int type) {
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(COLOR_WHITE);
	CP_Graphics_DrawRect(ButtonX.xOrigin, ButtonX.yOrigin, ButtonX.width, ButtonX.height);

	switch (type)
	{
	case 0:
		RenderNormal(basicTurretSpriteSheet, basicTurretArray[0], ButtonX.width / 4, (ButtonX.yOrigin + ButtonX.height / 2), 128, 128);
		break;
	case 1:
		CP_Image_DrawAdvanced(ButtonX.imageOfButton, ButtonX.width / 4,
			(ButtonX.yOrigin + ButtonX.height / 2), 128,
			128, 255, 90);
		break;	
	case 2:
		RenderNormal(homingMissleTurretSpriteSheet, homingMissleTurretArray[0], ButtonX.width / 4, (ButtonX.yOrigin + ButtonX.height / 2), 128, 128);
		break;
	case 3:
		RenderNormal(mineSpriteSheet, mineArray[0], ButtonX.width / 4, (ButtonX.yOrigin + ButtonX.height / 2), 128, 128);
		break;
	case 4:
		CP_Image_DrawAdvanced(ButtonX.imageOfButton, ButtonX.width / 2,
			(ButtonX.yOrigin + ButtonX.height / 2), 128,
			128, 255, 0);
		break;
	}



}

void render_new_turret(LevelData* LevelX) {
	Coordinates GameTemp;
	GameTemp.xOrigin = Game.xOrigin;
	GameTemp.yOrigin = Game.yOrigin;
	GameTemp.width = Game.width;
	GameTemp.height = Game.height;
	if (btn_is_pressed(GameTemp) && isPlacingTurret != NOT_PLACING_TURRET) {
		int drawX, drawY;
		drawX = (int)((CP_Input_GetMouseX() - Game.xOrigin) / Game.gridWidth);
		drawY = (int)((CP_Input_GetMouseY() - Game.yOrigin) / Game.gridHeight);
		if (LevelX->grid[drawY][drawX].type == Clear || LevelX->grid[drawY][drawX].type == Path) {
			LevelX->grid[drawY][drawX].type = Blocked;
			pathfinding_reset(LevelX);
			pathfinding_calculate_cost(LevelX);
			if (!is_destination_updated(LevelX)) {
				LevelX->grid[drawY][drawX].type = Clear;
				pathfinding_reset(LevelX);
				pathfinding_calculate_cost(LevelX);
			}
			else {
				place_turret(isPlacingTurret, drawX, drawY);
				isPlacingTurret = NOT_PLACING_TURRET;
			}
			pathfinding_update(LevelX);
		}
	}
}


//Level

void pathfinding_init(LevelData* LevelX) {
	for (int i = 0; i < GAME_GRID_ROWS; i++) {
		for (int j = 0; j < GAME_GRID_COLS; j++) {
			LevelX->grid[i][j].type = Clear;
			LevelX->grid[i][j].visited = 0;
			LevelX->grid[i][j].cost = -1;
			LevelX->grid[i][j].parentRow = -1;
			LevelX->grid[i][j].parentCol = -1;
		}
	}
	LevelX->grid[LevelX->spawnRow][LevelX->spawnCol].cost = 0;
	LevelX->grid[LevelX->spawnRow][LevelX->spawnCol].visited = 1;
	LevelX->grid[LevelX->spawnRow][LevelX->spawnCol].type = Spawn;
	LevelX->grid[LevelX->exitRow][LevelX->exitCol].type = Exit;
}
//reset data in pathfinding
void pathfinding_reset(LevelData* LevelX) {
	for (int i = 0; i < GAME_GRID_ROWS; i++) {
		for (int j = 0; j < GAME_GRID_COLS; j++) {
			LevelX->grid[i][j].visited = 0;
			LevelX->grid[i][j].cost = -1;
			LevelX->grid[i][j].parentRow = -1;
			LevelX->grid[i][j].parentCol = -1;
			if (LevelX->grid[i][j].type == Path) {
				LevelX->grid[i][j].type = Clear;
			}
			if (LevelX->grid[i][j].type == Blocked) {
				LevelX->grid[i][j].visited = 1;
			}

		}
	}
	LevelX->grid[LevelX->spawnRow][LevelX->spawnCol].cost = 0;
	LevelX->grid[LevelX->spawnRow][LevelX->spawnCol].visited = 1;
}
//update path for pathfinding
void pathfinding_update(LevelData* LevelX) {
	int pathRow, pathCol;
	pathRow = LevelX->grid[LevelX->exitRow][LevelX->exitCol].parentRow;
	pathCol = LevelX->grid[LevelX->exitRow][LevelX->exitCol].parentCol;
	//To check if path have no neighbors
	if (pathRow != -1 && pathCol != -1) {
		while (LevelX->grid[pathRow][pathCol].cost) {
			LevelX->grid[pathRow][pathCol].type = Path;
			int newPathRow, newPathCol;
			newPathRow = LevelX->grid[pathRow][pathCol].parentRow;
			newPathCol = LevelX->grid[pathRow][pathCol].parentCol;
			pathRow = newPathRow;
			pathCol = newPathCol;
		}
	}
}
void render_path(LevelData* LevelX) {
	for (int i = 0; i < GAME_GRID_ROWS; i++) {
		for (int j = 0; j < GAME_GRID_COLS; j++) {
			if (LevelX->grid[i][j].type == Path) {
				color_game_square(i, j, COLOR_GREY);
			}
			else if (LevelX->grid[i][j].type == Spawn) {
				color_game_square(i, j, COLOR_RED);
				portalVariablesArray[1].portalXPos = ((Game.xOrigin + Game.gridWidth * j) +
					(Game.xOrigin + Game.gridWidth * (j + 1))) / 2;
				portalVariablesArray[1].portalYPos = ((Game.yOrigin + Game.gridHeight * i) +
					(Game.yOrigin + Game.gridHeight * (i + 1))) / 2;
				portalVariablesArray[1].sizeX = Game.gridWidth;
				portalVariablesArray[1].sizeY = Game.gridHeight;
			}
			else if (LevelX->grid[i][j].type == Exit) {
				color_game_square(i, j, COLOR_BLUE);
				portalVariablesArray[0].portalXPos = ((Game.xOrigin + Game.gridWidth * j) +
					(Game.xOrigin + Game.gridWidth * (j + 1))) / 2;
				portalVariablesArray[0].portalYPos = ((Game.yOrigin + Game.gridHeight * i) +
					(Game.yOrigin + Game.gridHeight * (i + 1))) / 2;
				portalVariablesArray[0].sizeX = Game.gridWidth;
				portalVariablesArray[0].sizeY = Game.gridHeight;
			}
		}
	}
}

