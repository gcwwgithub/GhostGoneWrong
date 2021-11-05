#include "cprocessing.h"
#include"ZhengWei.h"
#include "game.h"
#include "Gabriel.h"
#include"Samuel.h"
#include "Anderson.h"
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
	for (int i = 0; i < NUMBER_OF_MENU_OBJECTS; i++) {
		if (btn_is_pressed(GameMenuObject[i])) {
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

void pause_button_init(void) {
	GameMenuObject[PauseButton].xOrigin = 0.0f;
	GameMenuObject[PauseButton].yOrigin = 0.0f;
	GameMenuObject[PauseButton].width = Game.xOrigin / 4;
	GameMenuObject[PauseButton].height = (float)CP_System_GetWindowHeight() / 10;
	GameMenuObject[PauseButton].objectType = objectRectangle;
	GameMenuObject[PauseButton].image = pauseButtonImage;
}

void turret_basic_button_init(void) {

	GameMenuObject[TurretButtonBasic].xOrigin = GameMenuObject[PauseButton].xOrigin;
	GameMenuObject[TurretButtonBasic].yOrigin = GameMenuObject[PauseButton].height;
	GameMenuObject[TurretButtonBasic].width = GameMenuObject[PauseButton].width;
	GameMenuObject[TurretButtonBasic].height = GameMenuObject[PauseButton].height * 2;
	GameMenuObject[TurretButtonBasic].objectType = objectRectangle;
	GameMenuObject[TurretButtonBasic].image = slowTurretImageArray[0];
}

void turret_slow_button_init(void) {
	GameMenuObject[TurretButtonSlow].xOrigin = GameMenuObject[TurretButtonBasic].xOrigin;
	GameMenuObject[TurretButtonSlow].yOrigin = GameMenuObject[TurretButtonBasic].yOrigin + GameMenuObject[TurretButtonBasic].height;
	GameMenuObject[TurretButtonSlow].width = GameMenuObject[TurretButtonBasic].width;
	GameMenuObject[TurretButtonSlow].height = GameMenuObject[TurretButtonBasic].height;
	GameMenuObject[TurretButtonSlow].objectType = objectRectangle;
	GameMenuObject[TurretButtonSlow].image = slowTurretImageArray[0];
}

void turret_homing_button_init(void) {
	GameMenuObject[TurretButtonHoming].xOrigin = GameMenuObject[TurretButtonSlow].xOrigin;
	GameMenuObject[TurretButtonHoming].yOrigin = GameMenuObject[TurretButtonSlow].yOrigin + GameMenuObject[TurretButtonSlow].height;
	GameMenuObject[TurretButtonHoming].width = GameMenuObject[TurretButtonSlow].width;
	GameMenuObject[TurretButtonHoming].height = GameMenuObject[TurretButtonSlow].height;
	GameMenuObject[TurretButtonHoming].objectType = objectRectangle;
	GameMenuObject[TurretButtonHoming].image = slowTurretImageArray[0];
}

void turret_mine_button_init(void) {
	GameMenuObject[TurretButtonMine].xOrigin = GameMenuObject[TurretButtonHoming].xOrigin;
	GameMenuObject[TurretButtonMine].yOrigin = GameMenuObject[TurretButtonHoming].yOrigin + GameMenuObject[TurretButtonHoming].height;
	GameMenuObject[TurretButtonMine].width = GameMenuObject[TurretButtonHoming].width;
	GameMenuObject[TurretButtonMine].height = GameMenuObject[TurretButtonBasic].height;
	GameMenuObject[TurretButtonMine].objectType = objectRectangle;
	GameMenuObject[TurretButtonMine].image = slowTurretImageArray[0];
}

void environment_init(LevelData* LevelX) {
	int row = 1, col = 1;
	Environment[0].image = environmentObjectsSpriteSheet;
	Environment[0].xOrigin = Game.xOrigin + Game.gridWidth * (col + 0.5f);
	Environment[0].yOrigin = Game.yOrigin + Game.gridHeight * (row + 0.5f);
	Environment[0].width = Game.gridWidth;
	Environment[0].height = Game.gridHeight;
	LevelX->grid[row][col].type = Blocked;
}

void render_environment(void) {
	for (int i = 0; i < MAX_ENVIRONMENT_OBJECT; i++) {
		RenderNormal(Environment[i].image, environmentObjectArray[0], Environment[i].xOrigin, Environment[i].yOrigin, Environment[i].width, Environment[i].height);
	}
}

void phantomQuartz_init(void) {
	GameMenuObject[PhantomQuartzMenu].yOrigin = 0.0f;
	GameMenuObject[PhantomQuartzMenu].width = ((float)CP_System_GetWindowWidth() - Game.xOrigin - Game.width) / 4;
	GameMenuObject[PhantomQuartzMenu].height = (float)CP_System_GetWindowHeight() / 15;
	GameMenuObject[PhantomQuartzMenu].xOrigin = (float)CP_System_GetWindowWidth() - 2*GameMenuObject[PhantomQuartzMenu].width;
	GameMenuObject[PhantomQuartzMenu].objectType = objectRectangle;
}


void goldQuartz_init(void) {
	GameMenuObject[GoldQuartzMenu].width = GameMenuObject[PhantomQuartzMenu].width;
	GameMenuObject[GoldQuartzMenu].height = GameMenuObject[PhantomQuartzMenu].height;
	GameMenuObject[GoldQuartzMenu].xOrigin = (float)CP_System_GetWindowWidth() - GameMenuObject[PhantomQuartzMenu].width;
	GameMenuObject[GoldQuartzMenu].yOrigin = GameMenuObject[PhantomQuartzMenu].yOrigin;
	GameMenuObject[GoldQuartzMenu].objectType = objectRectangle;
}


void health_init(void) {
	GameMenuObject[HealthMenu].xOrigin = GameMenuObject[PhantomQuartzMenu].xOrigin;
	GameMenuObject[HealthMenu].yOrigin = GameMenuObject[PhantomQuartzMenu].yOrigin + GameMenuObject[PhantomQuartzMenu].height;
	GameMenuObject[HealthMenu].width = GameMenuObject[PhantomQuartzMenu].width;
	GameMenuObject[HealthMenu].height = GameMenuObject[PhantomQuartzMenu].height;
	GameMenuObject[HealthMenu].objectType = objectRectangle;


	/*GameMenuObject[HealthMenu].xOrigin = GameMenuObject[GoldQuartzMenu].xOrigin;
	GameMenuObject[HealthMenu].yOrigin = GameMenuObject[GoldQuartzMenu].yOrigin + GameMenuObject[GoldQuartzMenu].height;
	GameMenuObject[HealthMenu].width = GameMenuObject[GoldQuartzMenu].width;
	GameMenuObject[HealthMenu].height = GameMenuObject[GoldQuartzMenu].height;
	GameMenuObject[HealthMenu].objectType = objectRectangle;*/
}

void currency_swap_init(void) {
	GameMenuObject[SwapButton].xOrigin = GameMenuObject[TurretButtonMine].xOrigin;
	GameMenuObject[SwapButton].yOrigin = GameMenuObject[TurretButtonMine].yOrigin + GameMenuObject[TurretButtonMine].height;
	GameMenuObject[SwapButton].width = GameMenuObject[TurretButtonMine].width;
	GameMenuObject[SwapButton].height = GameMenuObject[TurretButtonMine].height / 2;
	GameMenuObject[SwapButton].objectType = objectRectangle;
	GameMenuObject[SwapButton].image = CP_Image_Load("Assets/dummy");
}

void wave_number_display_init(void) {
	GameMenuObject[WaveDisplay].xOrigin = GameMenuObject[GoldQuartzMenu].xOrigin;
	GameMenuObject[WaveDisplay].yOrigin = GameMenuObject[HealthMenu].yOrigin;
	GameMenuObject[WaveDisplay].width = GameMenuObject[GoldQuartzMenu].width;
	GameMenuObject[WaveDisplay].height = GameMenuObject[GoldQuartzMenu].height;
	GameMenuObject[WaveDisplay].objectType = objectRectangle;
}

void battlefield_effects_display_init(void) {
	GameMenuObject[BattlefieldEffects].yOrigin = GameMenuObject[WaveDisplay].yOrigin + GameMenuObject[WaveDisplay].height;
	GameMenuObject[BattlefieldEffects].width = ((float)CP_System_GetWindowWidth() - Game.xOrigin - Game.width) / 2;
	GameMenuObject[BattlefieldEffects].height = GameMenuObject[GoldQuartzMenu].height;
	GameMenuObject[BattlefieldEffects].xOrigin = (float)CP_System_GetWindowWidth() - GameMenuObject[BattlefieldEffects].width;
	GameMenuObject[BattlefieldEffects].objectType = objectRectangle;

	/*GameMenuObject[BattlefieldEffects].xOrigin = GameMenuObject[WaveDisplay].xOrigin;
	GameMenuObject[BattlefieldEffects].yOrigin = GameMenuObject[WaveDisplay].yOrigin + GameMenuObject[WaveDisplay].height;
	GameMenuObject[BattlefieldEffects].width = GameMenuObject[WaveDisplay].width;
	GameMenuObject[BattlefieldEffects].height = GameMenuObject[WaveDisplay].height;
	GameMenuObject[BattlefieldEffects].objectType = objectRectangle;*/
}

void monster_remaining_display_init(void) {

	GameMenuObject[MonsterRemainingDisplay].xOrigin = GameMenuObject[BattlefieldEffects].xOrigin;
	GameMenuObject[MonsterRemainingDisplay].yOrigin = GameMenuObject[BattlefieldEffects].yOrigin + GameMenuObject[BattlefieldEffects].height;
	GameMenuObject[MonsterRemainingDisplay].width = GameMenuObject[BattlefieldEffects].width;
	GameMenuObject[MonsterRemainingDisplay].height = GameMenuObject[BattlefieldEffects].height * 5;
	GameMenuObject[MonsterRemainingDisplay].objectType = objectRectangle;
}

void render_button_pressed(void) {
	switch (check_game_button_pressed())
	{
	case PauseButton:
		currentGameState = currentGameState == Pause ? Wave : Pause;
		MouseInput.xOrigin = (float)CP_System_GetWindowWidth() / 2;
		MouseInput.yOrigin = (float)CP_System_GetWindowHeight() / 2;
		break;
	case TurretButtonBasic:
		isPlacingTurret = T_BASIC;
		RenderNormal(basicTurretSpriteSheet, basicTurretArray[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), Game.gridWidth, Game.gridHeight);
		break;
	case TurretButtonSlow:
		isPlacingTurret = T_SLOW;
		CP_Image_DrawAdvanced(GameMenuObject[check_game_button_pressed()].image, CP_Input_GetMouseX(), CP_Input_GetMouseY(), Game.gridWidth, Game.gridHeight, 255, 0);
		break;
	case TurretButtonHoming:
		isPlacingTurret = T_HOMING;
		RenderNormal(homingMissleTurretSpriteSheet, homingMissleTurretArray[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), Game.gridWidth, Game.gridHeight);
		break;
	case TurretButtonMine:
		isPlacingTurret = T_MINE;
		RenderNormal(mineSpriteSheet, mineArray[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), Game.gridWidth, Game.gridHeight);
		break;
	case SwapButton:
		isPlacingTurret = NOT_PLACING_TURRET;
		if (Level[currentGameLevel].phantomQuartz >= 10) {
			Level[currentGameLevel].phantomQuartz -= 10;
			Level[currentGameLevel].goldQuartz += 1;
		}
		MouseInput.xOrigin = (float)CP_System_GetWindowWidth() / 2;
		MouseInput.yOrigin = (float)CP_System_GetWindowHeight() / 2;
		break;
	case NoButton:
		isPlacingTurret = NOT_PLACING_TURRET;
		break;
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

void render_turret_menu_object(Coordinates menuObjectX, enum MenuObjectType type) {
	char temp[100];
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(COLOR_WHITE);
	CP_Graphics_DrawRect(menuObjectX.xOrigin, menuObjectX.yOrigin, menuObjectX.width, menuObjectX.height);
		CP_Settings_TextSize(35.0f * scalingFactor);
	switch (type)
	{
	case TurretButtonBasic:
		CP_Image_Draw(turretUIButton, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2),
			128 * scalingFactor, 128 * scalingFactor, 255);
		RenderNormal(basicTurretSpriteSheet, basicTurretArray[0], menuObjectX.width / 2, 
			(menuObjectX.yOrigin + menuObjectX.height / 2), 128 * scalingFactor, 128 * scalingFactor);
		sprintf_s(temp, sizeof(temp), "25");
		CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2 + 128/3));
		break;

	case TurretButtonSlow:
		CP_Image_Draw(turretUIButton, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2),
			128 * scalingFactor, 128 * scalingFactor, 255);
		CP_Image_DrawAdvanced(menuObjectX.image, menuObjectX.width / 2,
			(menuObjectX.yOrigin + menuObjectX.height / 2), 128 * scalingFactor,
			128 * scalingFactor, 255, 90);
		sprintf_s(temp, sizeof(temp), "25");
		CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2 + 128 / 3));
		break;

	case  TurretButtonHoming:
		CP_Image_Draw(turretUIButton, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2),
			128 * scalingFactor, 128 * scalingFactor, 255);
		RenderNormal(homingMissleTurretSpriteSheet, homingMissleTurretArray[0], menuObjectX.width / 2, 
			(menuObjectX.yOrigin + menuObjectX.height / 2), 128 * scalingFactor, 128 * scalingFactor);
		sprintf_s(temp, sizeof(temp), "25");
		CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2 + 128 / 3 ));
		break;

	case TurretButtonMine:
		CP_Image_Draw(turretUIButton, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2),
			128 * scalingFactor, 128 * scalingFactor, 255);
		RenderNormal(mineSpriteSheet, mineArray[0], menuObjectX.width / 2, 
			(menuObjectX.yOrigin + menuObjectX.height / 2), 128 * scalingFactor, 128 * scalingFactor);
		sprintf_s(temp, sizeof(temp), "25");
		CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2 + 128 / 3));
		break;

	case PauseButton:
		CP_Image_DrawAdvanced(menuObjectX.image, menuObjectX.width / 2,
			(menuObjectX.yOrigin + menuObjectX.height / 2), 64,
			64, 255, 0);
		break;
	case SwapButton:
		break;
	case GoldQuartzMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scalingFactor);
		CP_Image_Draw(currencyUIRectangle, menuObjectX.xOrigin + menuObjectX.width / 2, 
			menuObjectX.yOrigin + menuObjectX.height / 2, 128 * scalingFactor, 36 * scalingFactor, 255);
		RenderNormal(currencySpriteSheet, currencyArray[0], menuObjectX.xOrigin + menuObjectX.width / 5.5,
			menuObjectX.yOrigin + menuObjectX.height / 2, 30 * scalingFactor, 30 * scalingFactor);
		sprintf_s(temp, 100, "x%-10d", Level[currentGameLevel].goldQuartz);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width /1.55, menuObjectX.yOrigin + menuObjectX.height / 2);
		break;
	case PhantomQuartzMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scalingFactor);
		CP_Image_Draw(currencyUIRectangle, menuObjectX.xOrigin + menuObjectX.width / 2,
			menuObjectX.yOrigin + menuObjectX.height / 2, 128 * scalingFactor, 36 * scalingFactor, 255);
		RenderNormal(currencySpriteSheet, currencyArray[1], menuObjectX.xOrigin + menuObjectX.width / 5,
			menuObjectX.yOrigin + menuObjectX.height / 2, 30 * scalingFactor, 30 * scalingFactor);
		sprintf_s(temp, 100, "x%-10d", Level[currentGameLevel].phantomQuartz);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 1.45, menuObjectX.yOrigin + menuObjectX.height / 2);
		break;
	case HealthMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scalingFactor);
		sprintf_s(temp, 100, "x%-10d", Level[currentGameLevel].health);
		CP_Image_Draw(currencyUIRectangle, menuObjectX.xOrigin + menuObjectX.width / 2,
			menuObjectX.yOrigin + menuObjectX.height / 2, 128 * scalingFactor, 36 * scalingFactor, 255);
		RenderNormal(currencySpriteSheet, currencyArray[2], menuObjectX.xOrigin + menuObjectX.width / 5,
			menuObjectX.yOrigin + menuObjectX.height / 2, 30 * scalingFactor, 30 * scalingFactor);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 1.45, menuObjectX.yOrigin + menuObjectX.height / 2);
		break;
	case WaveDisplay:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scalingFactor);
		sprintf_s(temp, 100, "%2d/20", Level[currentGameLevel].currentWave);
		CP_Image_Draw(currencyUIRectangle, menuObjectX.xOrigin + menuObjectX.width / 2,
			menuObjectX.yOrigin + menuObjectX.height / 2, 128 * scalingFactor, 36 * scalingFactor, 255);
		RenderNormal(currencySpriteSheet, currencyArray[3], menuObjectX.xOrigin + menuObjectX.width / 5,
			menuObjectX.yOrigin + menuObjectX.height / 2, 30 * scalingFactor, 30 * scalingFactor);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 2.5, menuObjectX.yOrigin + menuObjectX.height / 2);
		break;
	case BattlefieldEffects:
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(30.0f * scalingFactor);
		switch (Level[currentGameLevel].currentEffect) {
		case MoreHP:
			CP_Font_DrawText("Environment Effect:", menuObjectX.xOrigin + menuObjectX.width / 2, menuObjectX.yOrigin + menuObjectX.height / 3);
			CP_Font_DrawText("Enemies have more HP", menuObjectX.xOrigin + menuObjectX.width / 2, menuObjectX.yOrigin + menuObjectX.height / 3 * 2);
			break;
		case FasterEnemies:
			CP_Font_DrawText("Environment Effect:", menuObjectX.xOrigin + menuObjectX.width / 2, menuObjectX.yOrigin + menuObjectX.height / 3);
			CP_Font_DrawText("Enemies move Faster", menuObjectX.xOrigin + menuObjectX.width / 2, menuObjectX.yOrigin + menuObjectX.height / 3 * 2);
			break;
		}
		break;
	case MonsterRemainingDisplay:
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(35.0f * scalingFactor);
		sprintf_s(temp, sizeof(temp), "Enemies Left: %d", enemiesLeft);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 2.5, menuObjectX.yOrigin + menuObjectX.height / 10);

		CP_Settings_TextSize(50.0f * scalingFactor);
		RenderNormal(basicGhostSpriteSheet, basicGhostSpriteArray[0], menuObjectX.xOrigin + menuObjectX.width / 6,
			menuObjectX.yOrigin + menuObjectX.height / 10 * 3, menuObjectX.height / 4, menuObjectX.height / 4);
		sprintf_s(temp, sizeof(temp), "x%d", basicEnemyNum);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 2.25, menuObjectX.yOrigin + menuObjectX.height / 10 * 3);
		
		RenderNormal(fastGhostSpriteSheet, fastGhostSpriteArray[0], menuObjectX.xOrigin + menuObjectX.width / 6,
			menuObjectX.yOrigin + menuObjectX.height / 10 * 5.5, menuObjectX.height / 4, menuObjectX.height / 4);
		sprintf_s(temp, sizeof(temp), "x%d", fastEnemyNum);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 2.25, menuObjectX.yOrigin + menuObjectX.height / 10 * 5.5);

		RenderNormal(fatGhostSpriteSheet, fatGhostSpriteArray[0], menuObjectX.xOrigin + menuObjectX.width / 6,
			menuObjectX.yOrigin + menuObjectX.height / 10 * 8, menuObjectX.height / 4, menuObjectX.height / 4);
		sprintf_s(temp, sizeof(temp), "x%d", fatEnemyNum);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 2.25, menuObjectX.yOrigin + menuObjectX.height / 10 * 8);
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
			if (isPlacingTurret != T_MINE) {
				LevelX->grid[drawY][drawX].type = Blocked;
				pathfinding_reset(LevelX);
				pathfinding_calculate_cost(LevelX);
			}
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

