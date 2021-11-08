#include "cprocessing.h"
#include"ZhengWei.h"
#include "game.h"
#include "Gabriel.h"
#include"Samuel.h"
#include "Anderson.h"
//Tools

//Return positive value of float
void mouse_reset(void) {
	MouseInput.xOrigin = -1;
	MouseInput.yOrigin = -1;
}

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

//Function for doing all the stuff when clicking on game grid
void render_game_grid_press(LevelData* LevelX) {
	int drawX, drawY;
	drawX = (int)((MouseInput.xOrigin - Game.xOrigin) / Game.gridWidth);
	drawY = (int)((MouseInput.yOrigin - Game.yOrigin) / Game.gridHeight);
	if (isPlacingTurret != NOT_PLACING_TURRET) {
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
				Level[currentGameLevel].phantomQuartz -= turret[0].price;
			}
			pathfinding_update(LevelX);
		}
		mouse_reset();
	}
	else {
		Coordinates GridTemp;
		GridTemp.width = Game.gridWidth;
		GridTemp.height = Game.gridHeight;
		GridTemp.xOrigin = Game.xOrigin + (drawX + 0.5f) * Game.gridWidth;
		GridTemp.yOrigin = Game.yOrigin + (drawY + 0.5f) * Game.gridHeight;
		for (int i = 0; i < MAX_TURRET; i++) {
			if (turret[i].data.xOrigin == GridTemp.xOrigin && turret[i].data.yOrigin == GridTemp.yOrigin && isUpgradingTurret == TRUE && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				isUpgradingTurret = FALSE;
				mouse_reset();
			}
			else if (turret[i].data.xOrigin == GridTemp.xOrigin && turret[i].data.yOrigin == GridTemp.yOrigin && turret[i].isActive == TRUE) {
				isUpgradingTurret = TRUE;
			}
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
	mouse_reset();
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
	GameMenuObject[GameGrid].xOrigin = Game.xOrigin;
	GameMenuObject[GameGrid].yOrigin = Game.yOrigin;
	GameMenuObject[GameGrid].width = Game.width;
	GameMenuObject[GameGrid].height = Game.height;
	GameMenuObject[GameGrid].objectType = objectRectangle;
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
	GameMenuObject[PhantomQuartzMenu].xOrigin = (float)CP_System_GetWindowWidth() - 2 * GameMenuObject[PhantomQuartzMenu].width;
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

void menu_swap_init(void) {
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
	GameMenuObject[BattlefieldEffects].xOrigin = GameMenuObject[HealthMenu].xOrigin;
	GameMenuObject[BattlefieldEffects].yOrigin = GameMenuObject[HealthMenu].yOrigin + GameMenuObject[HealthMenu].height;
	GameMenuObject[BattlefieldEffects].width = GameMenuObject[HealthMenu].width;
	GameMenuObject[BattlefieldEffects].height = GameMenuObject[HealthMenu].height * 3;
	GameMenuObject[BattlefieldEffects].objectType = objectRectangle;

	/*GameMenuObject[BattlefieldEffects].yOrigin = GameMenuObject[WaveDisplay].yOrigin + GameMenuObject[WaveDisplay].height;
	GameMenuObject[BattlefieldEffects].width = ((float)CP_System_GetWindowWidth() - Game.xOrigin - Game.width) / 2;
	GameMenuObject[BattlefieldEffects].height = GameMenuObject[GoldQuartzMenu].height * 2;
	GameMenuObject[BattlefieldEffects].xOrigin = (float)CP_System_GetWindowWidth() - GameMenuObject[BattlefieldEffects].width;
	GameMenuObject[BattlefieldEffects].objectType = objectRectangle;*/

}

void monster_remaining_display_init(void) {
	GameMenuObject[MonsterRemainingDisplay].xOrigin = GameMenuObject[WaveDisplay].xOrigin;
	GameMenuObject[MonsterRemainingDisplay].yOrigin = GameMenuObject[WaveDisplay].yOrigin + GameMenuObject[WaveDisplay].height;
	GameMenuObject[MonsterRemainingDisplay].width = GameMenuObject[WaveDisplay].width;
	GameMenuObject[MonsterRemainingDisplay].height = GameMenuObject[WaveDisplay].height * 3;
	GameMenuObject[MonsterRemainingDisplay].objectType = objectRectangle;


	/*GameMenuObject[MonsterRemainingDisplay].xOrigin = GameMenuObject[BattlefieldEffects].xOrigin;
	GameMenuObject[MonsterRemainingDisplay].yOrigin = GameMenuObject[BattlefieldEffects].yOrigin + GameMenuObject[BattlefieldEffects].height;
	GameMenuObject[MonsterRemainingDisplay].width = GameMenuObject[BattlefieldEffects].width;
	GameMenuObject[MonsterRemainingDisplay].height = GameMenuObject[BattlefieldEffects].height;
	GameMenuObject[MonsterRemainingDisplay].objectType = objectRectangle;*/
}

void upgrade_button_init(void) {
	GameMenuObject[UpgradeButton].xOrigin = GameMenuObject[BattlefieldEffects].xOrigin;
	GameMenuObject[UpgradeButton].yOrigin = GameMenuObject[BattlefieldEffects].yOrigin + GameMenuObject[BattlefieldEffects].height;
	GameMenuObject[UpgradeButton].width = GameMenuObject[BattlefieldEffects].width + GameMenuObject[MonsterRemainingDisplay].width;
	GameMenuObject[UpgradeButton].height = (CP_System_GetWindowHeight() - GameMenuObject[UpgradeButton].yOrigin) / 2;
	GameMenuObject[UpgradeButton].objectType = objectRectangle;
}

void sell_button_init(void) {
	GameMenuObject[SellButton].xOrigin = GameMenuObject[UpgradeButton].xOrigin;
	GameMenuObject[SellButton].yOrigin = GameMenuObject[UpgradeButton].yOrigin + GameMenuObject[UpgradeButton].height;
	GameMenuObject[SellButton].width = GameMenuObject[UpgradeButton].width;
	GameMenuObject[SellButton].height = GameMenuObject[UpgradeButton].height;
	GameMenuObject[SellButton].objectType = objectRectangle;
}

void level1_init(void) {
	currentGameLevel = 0;
	Level[0].spawnRow = 0;
	Level[0].spawnCol = (GAME_GRID_COLS - 1) / 2;
	Level[0].exitRow = GAME_GRID_ROWS - 1;
	Level[0].exitCol = (GAME_GRID_COLS - 1) / 2;
	Level[0].health = 100;
	Level[0].phantomQuartz = 50;
	Level[0].goldQuartz = 0;
	Level[0].currentWave = 0;
	Level[0].currentEffect = 0;
}

void render_button_pressed(void) {
	switch (check_game_button_pressed())
	{
	case GameGrid:
		render_game_grid_press(&Level[currentGameLevel]);
		break;
	case PauseButton:
		currentGameState = currentGameState == Pause ? Wave : Pause;
		mouse_reset();
		break;
	case TurretButtonBasic:
		if (turret[0].price <= Level[currentGameLevel].phantomQuartz && powerUpMenu==FALSE) { // Currently hardcoded 
			isPlacingTurret = T_BASIC;
			isUpgradingTurret = FALSE;
			RenderNormal(basicTurretSpriteSheet, basicTurretArray[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), Game.gridWidth, Game.gridHeight);
		}
		break;
	case TurretButtonSlow:
		if (turret[0].price <= Level[currentGameLevel].phantomQuartz && powerUpMenu == FALSE) {
			isPlacingTurret = T_SLOW;
			isUpgradingTurret = FALSE;
			CP_Image_DrawAdvanced(GameMenuObject[check_game_button_pressed()].image, CP_Input_GetMouseX(), CP_Input_GetMouseY(), Game.gridWidth, Game.gridHeight, 255, 0);
		}
		break;
	case TurretButtonHoming:
		if (turret[0].price <= Level[currentGameLevel].phantomQuartz && powerUpMenu == FALSE) {
			isPlacingTurret = T_HOMING;
			isUpgradingTurret = FALSE;
			RenderNormal(homingMissleTurretSpriteSheet, homingMissleTurretArray[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), Game.gridWidth, Game.gridHeight);
		}
		break;
	case TurretButtonMine:
		if (turret[0].price <= Level[currentGameLevel].phantomQuartz && powerUpMenu == FALSE) {
			isPlacingTurret = T_MINE;
			isUpgradingTurret = FALSE;
			RenderNormal(mineSpriteSheet, mineArray[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), Game.gridWidth, Game.gridHeight);
		}
		break;
	case SwapButton:
		isPlacingTurret = NOT_PLACING_TURRET;
		isUpgradingTurret = FALSE;
		powerUpMenu = !powerUpMenu;
		mouse_reset();
		break;

	case GoldQuartzMenu:
		isPlacingTurret = NOT_PLACING_TURRET;
		isUpgradingTurret = FALSE;
		if (Level[currentGameLevel].phantomQuartz >= 10) {
			Level[currentGameLevel].phantomQuartz -= 10;
			Level[currentGameLevel].goldQuartz += 1;
		}
		mouse_reset();
		break;
	case UpgradeButton:
		if (isUpgradingTurret == TRUE) {
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) && Level[currentGameLevel].phantomQuartz >= 50) {
				Level[currentGameLevel].phantomQuartz -= 50;//hard coded to cost 50 for upgrade
				//call upgrade function
			}
		}
		else {
			isPlacingTurret = NOT_PLACING_TURRET;
			isUpgradingTurret = FALSE;
		}
		break;
	case SellButton:
		if (isUpgradingTurret == TRUE) {
			Level[currentGameLevel].phantomQuartz += 10;//hard coded to sell for 10
			mouse_reset();
			//call upgrade function
		}
		else {
			isPlacingTurret = NOT_PLACING_TURRET;
			isUpgradingTurret = FALSE;
		}
		break;

	default:
		isPlacingTurret = NOT_PLACING_TURRET;
		isUpgradingTurret = FALSE;
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
	if (isUpgradingTurret == FALSE && (type == UpgradeButton || type == SellButton)) {// only render upgrade and sell button when turret selected
	//empty by design
	}
	else {
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(menuObjectX.xOrigin, menuObjectX.yOrigin, menuObjectX.width, menuObjectX.height);
	}
	CP_Settings_TextSize(35.0f * scalingFactor);
	switch (type)
	{
	case TurretButtonBasic:
		if (powerUpMenu == FALSE) {
			CP_Image_Draw(turretUIButton, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2),
				128 * scalingFactor, 128 * scalingFactor, 255);
			RenderNormal(basicTurretSpriteSheet, basicTurretArray[0], menuObjectX.width / 2,
				(menuObjectX.yOrigin + menuObjectX.height / 2), 128 * scalingFactor, 128 * scalingFactor);
			sprintf_s(temp, sizeof(temp), "25");
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2 + 128 / 3));
		}
		break;

	case TurretButtonSlow:
		if (powerUpMenu == FALSE) {
			CP_Image_Draw(turretUIButton, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2),
				128 * scalingFactor, 128 * scalingFactor, 255);
			CP_Image_DrawAdvanced(menuObjectX.image, menuObjectX.width / 2,
				(menuObjectX.yOrigin + menuObjectX.height / 2), 128 * scalingFactor,
				128 * scalingFactor, 255, 90);
			sprintf_s(temp, sizeof(temp), "25");
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2 + 128 / 3));
		}
		break;

	case  TurretButtonHoming:
		if (powerUpMenu == FALSE) {
			CP_Image_Draw(turretUIButton, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2),
				128 * scalingFactor, 128 * scalingFactor, 255);
			RenderNormal(homingMissleTurretSpriteSheet, homingMissleTurretArray[0], menuObjectX.width / 2,
				(menuObjectX.yOrigin + menuObjectX.height / 2), 128 * scalingFactor, 128 * scalingFactor);
			sprintf_s(temp, sizeof(temp), "25");
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2 + 128 / 3));
		}
		break;

	case TurretButtonMine:
		if (powerUpMenu == FALSE) {
			CP_Image_Draw(turretUIButton, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2),
				128 * scalingFactor, 128 * scalingFactor, 255);
			RenderNormal(mineSpriteSheet, mineArray[0], menuObjectX.width / 2,
				(menuObjectX.yOrigin + menuObjectX.height / 2), 128 * scalingFactor, 128 * scalingFactor);
			sprintf_s(temp, sizeof(temp), "25");
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.yOrigin + menuObjectX.height / 2 + 128 / 3));
		}
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
		CP_Image_Draw(backgroundUIThin, menuObjectX.xOrigin + menuObjectX.width / 2,
			menuObjectX.yOrigin + menuObjectX.height / 2, 128 * scalingFactor, 36 * scalingFactor, 255);
		RenderNormal(currencySpriteSheet, currencyArray[0], menuObjectX.xOrigin + menuObjectX.width / 5.5,
			menuObjectX.yOrigin + menuObjectX.height / 2, 30 * scalingFactor, 30 * scalingFactor);
		sprintf_s(temp, 100, "x%-10d", Level[currentGameLevel].goldQuartz);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 1.55, menuObjectX.yOrigin + menuObjectX.height / 2);
		break;
	case PhantomQuartzMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scalingFactor);
		CP_Image_Draw(backgroundUIThin, menuObjectX.xOrigin + menuObjectX.width / 2,
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
		CP_Image_Draw(backgroundUIThin, menuObjectX.xOrigin + menuObjectX.width / 2,
			menuObjectX.yOrigin + menuObjectX.height / 2, 128 * scalingFactor, 36 * scalingFactor, 255);
		RenderNormal(currencySpriteSheet, currencyArray[2], menuObjectX.xOrigin + menuObjectX.width / 5,
			menuObjectX.yOrigin + menuObjectX.height / 2, 30 * scalingFactor, 30 * scalingFactor);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 1.45, menuObjectX.yOrigin + menuObjectX.height / 2);
		break;
	case WaveDisplay:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scalingFactor);
		sprintf_s(temp, 100, "%2d/%d", Level[currentGameLevel].currentWave, MAX_NUMBER_OF_WAVES);
		CP_Image_Draw(backgroundUIThin, menuObjectX.xOrigin + menuObjectX.width / 2,
			menuObjectX.yOrigin + menuObjectX.height / 2, 128 * scalingFactor, 36 * scalingFactor, 255);
		RenderNormal(currencySpriteSheet, currencyArray[3], menuObjectX.xOrigin + menuObjectX.width / 5,
			menuObjectX.yOrigin + menuObjectX.height / 2, 30 * scalingFactor, 30 * scalingFactor);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 2.5, menuObjectX.yOrigin + menuObjectX.height / 2);
		break;
	case BattlefieldEffects:
		CP_Image_Draw(backgroundUIFat, menuObjectX.xOrigin + menuObjectX.width / 2,
			menuObjectX.yOrigin + menuObjectX.height / 2, 132 * scalingFactor, 132 * scalingFactor, 255);
		CP_Settings_TextSize(35.0f * scalingFactor);
		CP_Settings_Fill(COLOR_WHITE);
		sprintf_s(temp, sizeof(temp), "Effects");
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 2, menuObjectX.yOrigin + menuObjectX.height / 5);
		RenderNormal(battlefieldEffectIconSpriteSheet, battlefieldEffectIconArray[Level[currentGameLevel].currentEffect],
			menuObjectX.xOrigin + menuObjectX.width / 2, menuObjectX.yOrigin + menuObjectX.height / 2,
			110 * scalingFactor, 110 * scalingFactor);
		break;
	case MonsterRemainingDisplay:
		CP_Image_Draw(backgroundUIFat, menuObjectX.xOrigin + menuObjectX.width / 2,
			menuObjectX.yOrigin + menuObjectX.height / 2, 132 * scalingFactor, 132 * scalingFactor, 255);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(35.0f * scalingFactor);
		sprintf_s(temp, sizeof(temp), "Enemies");
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 2, menuObjectX.yOrigin + menuObjectX.height / 5);
		CP_Settings_TextSize(45.0f * scalingFactor);
		sprintf_s(temp, sizeof(temp), "%d/%d", enemiesLeft, enemiesInLevel);
		CP_Font_DrawText(temp, menuObjectX.xOrigin + menuObjectX.width / 2, menuObjectX.yOrigin + menuObjectX.height / 2);
		break;
	case UpgradeButton:
		if (isUpgradingTurret == TRUE) { //Only render when upgrading
			CP_Settings_TextSize(35.0f * scalingFactor);
			CP_Font_DrawText("Upgrade", menuObjectX.xOrigin + menuObjectX.width / 2, menuObjectX.yOrigin + menuObjectX.height / 2);
		}
		break;
	case SellButton:
		if (isUpgradingTurret == TRUE) {//Only render when upgrading
			CP_Settings_TextSize(35.0f * scalingFactor);
			CP_Font_DrawText("Sell", menuObjectX.xOrigin + menuObjectX.width / 2, menuObjectX.yOrigin + menuObjectX.height / 2);
		}
		break;
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

