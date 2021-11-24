#include "game.h"
#include "zhengwei.h"
#include "Gabriel.h"

#include "cprocessing.h"
//Tools
//Reset Mouse

const float kLeftGameMenuXWidth = 140.714294f;
const float kRightGameMenuXOrigin = 998.571411f;

void MouseReset(void) {
	MouseInput.x_origin = -1;
	MouseInput.y_origin = -1;
}
//Return positive value of float
float myabs(float x) {
	return x < 0 ? x * -1 : x;
}

//Check if button is pressed
int BtnIsPressed(Coordinates object1) {
	if (((object1.x_origin <= MouseInput.x_origin) && (MouseInput.x_origin <= object1.x_origin + object1.width)) && ((object1.y_origin <= MouseInput.y_origin) && (MouseInput.y_origin <= object1.y_origin + object1.height))) {
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
	CP_Graphics_DrawRect((game.x_origin + game.grid_width * rectCol), (game.y_origin + game.grid_height * rectRow), (game.grid_width), (game.grid_height));
}

//Path Finding
//Check if destination is reachable
int is_destination_updated(LevelData* LevelX) {
	return LevelX->grid[LevelX->exitRow][LevelX->exitCol].visited;
}

//Update neighbors cost
void pathfinding_update_neighbor_cost(int gridRow, int gridCol, int generation, LevelData* LevelX) {
	//Update Row Neighbor
	for (int i = -1; i <= 1; i++) {
		if (gridRow + i >= 0 && gridRow + i < level_grid_rows) {
			if (LevelX->grid[gridRow + i][gridCol].visited == 0) {
				LevelX->grid[gridRow + i][gridCol].cost = generation + 1;
				LevelX->grid[gridRow + i][gridCol].parent_row = gridRow;
				LevelX->grid[gridRow + i][gridCol].parent_col = gridCol;
				LevelX->grid[gridRow + i][gridCol].visited = 1;
			}
		}
	}
	//Update Col Neighbor
	for (int i = -1; i <= 1; i++) {
		if (gridCol + i >= 0 && gridCol + i < level_grid_cols) {
			if (LevelX->grid[gridRow][gridCol + i].visited == 0) {
				LevelX->grid[gridRow][gridCol + i].cost = generation + 1;
				LevelX->grid[gridRow][gridCol + i].parent_row = gridRow;
				LevelX->grid[gridRow][gridCol + i].parent_col = gridCol;
				LevelX->grid[gridRow][gridCol + i].visited = 1;
			}
		}
	}
}

//Calculate all grid cost. Find the squares in the same generation and call a function to update neighbors.
void PathFindingCalculateCost(LevelData* LevelX) {
	for (int currentCost = 0; !is_destination_updated(LevelX) && currentCost <= level_grid_rows * level_grid_cols; currentCost++) {
		for (int i = 0; i < level_grid_rows; i++) {
			for (int j = 0; j < level_grid_cols; j++) {
				if (LevelX->grid[i][j].cost == currentCost) {
					pathfinding_update_neighbor_cost(i, j, currentCost, LevelX);
				}
			}
		}
	}
}

void pathfinding_init(LevelData* LevelX) {
	LevelX->grid[LevelX->spawnRow][LevelX->spawnCol].cost = 0;
	LevelX->grid[LevelX->spawnRow][LevelX->spawnCol].visited = 1;
	LevelX->grid[LevelX->spawnRow][LevelX->spawnCol].type = kSpawn;
	LevelX->grid[LevelX->exitRow][LevelX->exitCol].type = kExit;
}

//Collision Detection between circles and squares
int CollisionDetection(Coordinates object1, Coordinates object2) {
	if (object1.object_type == kObjectCircle && object2.object_type == kObjectCircle) {
		float collisionDistanceSquared, distanceSquared, circle1Radius, circle2Radius;
		circle1Radius = 0.5f * object1.width;
		circle2Radius = 0.5f * object2.width;
		collisionDistanceSquared = (circle1Radius + circle2Radius) * (circle1Radius + circle2Radius);
		distanceSquared = ((object1.x_origin - object2.x_origin) * (object1.x_origin - object2.x_origin)) + ((object1.y_origin - object2.y_origin) * (object1.y_origin - object2.y_origin));
		if (collisionDistanceSquared >= distanceSquared) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (object1.object_type == kObjectCircle || object2.object_type == kObjectCircle) {
		float  distanceX, distanceY, circleRadius, rectWidthtoCheck, rectHeightToCheck, collisionDistanceX, collisionDistanceY, radiusSquared, distanceToCornerSquaredX, distanceToCornerSquaredY;
		distanceX = object1.x_origin - object2.x_origin;
		distanceX = myabs(distanceX);
		distanceY = object1.y_origin - object2.y_origin;
		distanceY = myabs(distanceY);
		if (object1.object_type == kObjectCircle) {
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
		distanceX = (object1.x_origin - object2.x_origin);
		distanceX = myabs(distanceX);
		distanceY = (object1.y_origin - object2.y_origin);
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
	drawX = (int)((MouseInput.x_origin - game.x_origin) / game.grid_width);
	drawY = (int)((MouseInput.y_origin - game.y_origin) / game.grid_height);
	Coordinates GridTemp;
	GridTemp.width = game.grid_width;
	GridTemp.height = game.grid_height;
	GridTemp.x_origin = game.x_origin + (drawX + 0.5f) * game.grid_width;
	GridTemp.y_origin = game.y_origin + (drawY + 0.5f) * game.grid_height;

	int isPositionEmpty = kTrue;
	for (int i = 0; i < MAX_TURRET; i++) {
		if (turret[i].data.x_origin == GridTemp.x_origin && turret[i].data.y_origin == GridTemp.y_origin && turret[i].isActive == kTrue) {
			isPositionEmpty = kFalse;
		}
	}
	if (is_placing_turret != T_MAX) {
		if (isPositionEmpty == kTrue && LevelX->grid[drawY][drawX].type != kBlocked) {
			if (is_placing_turret != T_MINE) {
				LevelX->grid[drawY][drawX].type = kBlocked;
				PathFindingReset(LevelX);
				PathFindingCalculateCost(LevelX);
			}
			if (!is_destination_updated(LevelX)) {
				LevelX->grid[drawY][drawX].type = kClear;
				PathFindingReset(LevelX);
				PathFindingCalculateCost(LevelX);
			}
			else {
				place_turret(is_placing_turret, drawX, drawY);
				Level[currentGameLevel].phantomQuartz -= turret_purchasing[TP_PRICE][is_placing_turret];
				is_placing_turret = T_MAX;
			}
			PathFindingUpdate(LevelX);
		}
		MouseReset();
	}
	else {
		for (int i = 0; i < MAX_TURRET; i++) {
			if (turret[i].data.x_origin == GridTemp.x_origin && turret[i].data.y_origin == GridTemp.y_origin && turretSelectedToUpgrade != NO_TURRET_SELECTED && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				turretSelectedToUpgrade = NO_TURRET_SELECTED;
				MouseReset();
			}
			else if (turret[i].data.x_origin == GridTemp.x_origin && turret[i].data.y_origin == GridTemp.y_origin && turret[i].isActive == kTrue) {
				turretSelectedToUpgrade = i;
			}
		}
	}

}

//Check which button is pressed
int CheckGameButtonPressed(void) {
	for (int i = 0; i < ButtonMax; i++) {
		if (BtnIsPressed(GameMenuObject[i])) {
			return i;
		}
	}
	return ButtonMax;
}

//initialize enemies
void general_level_enemies_init(int level, int wave, int basic, int fast, int fat, int grim)
{
	Level[level].waveEnemies[wave][kBasic] = basic;
	Level[level].waveEnemies[wave][kFastGhost] = fast;
	Level[level].waveEnemies[wave][kFatGhost] = fat;
	Level[level].waveEnemies[wave][kGrimReaper] = grim;
}

//function to assign environment object
void init_environment_object(int arrayIndex, int row, int col,int object_type,LevelData* LevelX)
{
	Environment[arrayIndex].image = grid_environment_objects_spritesheet;
	Environment[arrayIndex].x_origin = game.x_origin + game.grid_width * (col + 0.5f);
	Environment[arrayIndex].y_origin = game.y_origin + game.grid_height * (row + 0.5f);
	Environment[arrayIndex].width = game.grid_width;
	Environment[arrayIndex].height = game.grid_height;
	Environment[arrayIndex].object_type = object_type;

	LevelX->grid[row][col].type = kBlocked;
	LevelX->grid[row][col].type = kBlocked;
}

//Graphics
void MouseInit(void) {
	MouseInput.width = 0.0f;
	MouseInput.height = 0.0f;
	MouseInput.object_type = kObjectCircle;
	MouseReset();
}

void game_grid_init(void) {
	float unusableScreenHeight, unusableScreenWidth;/*Height and Width not used for game, example menu*/
	unusableScreenHeight = (float)CP_System_GetWindowHeight() / 2; //Half the screeen is used for the game
	game.height = (float)CP_System_GetWindowHeight() - unusableScreenHeight;
	game.grid_height = game.height / level_grid_rows;
	game.grid_width = game.height / level_grid_rows; //Grid is a Square
	game.width = game.grid_width * level_grid_cols;
	unusableScreenWidth = (float)CP_System_GetWindowWidth() - game.width;
	game.x_origin = unusableScreenWidth / 2; //To centralise the Grid
	game.y_origin = unusableScreenHeight / 2; //Centre the game
	GameMenuObject[GameGrid].x_origin = game.x_origin;
	GameMenuObject[GameGrid].y_origin = game.y_origin;
	GameMenuObject[GameGrid].width = game.width;
	GameMenuObject[GameGrid].height = game.height;
	GameMenuObject[GameGrid].object_type = kObjectRectangle;
}

void pause_button_init(void) {
	GameMenuObject[PauseButton].x_origin = 0.0f;
	GameMenuObject[PauseButton].y_origin = 0.0f;
	GameMenuObject[PauseButton].width = kLeftGameMenuXWidth;
	GameMenuObject[PauseButton].height = (float)CP_System_GetWindowHeight() / 10;
	GameMenuObject[PauseButton].object_type = kObjectRectangle;
	GameMenuObject[PauseButton].image = pause_button_image;
}

void turret_basic_button_init(void) {

	GameMenuObject[TurretButtonBasic].x_origin = GameMenuObject[PauseButton].x_origin;
	GameMenuObject[TurretButtonBasic].y_origin = GameMenuObject[PauseButton].height;
	GameMenuObject[TurretButtonBasic].width = GameMenuObject[PauseButton].width;
	GameMenuObject[TurretButtonBasic].height = GameMenuObject[PauseButton].height * 2;
	GameMenuObject[TurretButtonBasic].object_type = kObjectRectangle;
	GameMenuObject[TurretButtonBasic].image = slow_turret_image_array[0];
}

void turret_slow_button_init(void) {
	GameMenuObject[TurretButtonSlow].x_origin = GameMenuObject[TurretButtonBasic].x_origin;
	GameMenuObject[TurretButtonSlow].y_origin = GameMenuObject[TurretButtonBasic].y_origin + GameMenuObject[TurretButtonBasic].height;
	GameMenuObject[TurretButtonSlow].width = GameMenuObject[TurretButtonBasic].width;
	GameMenuObject[TurretButtonSlow].height = GameMenuObject[TurretButtonBasic].height;
	GameMenuObject[TurretButtonSlow].object_type = kObjectRectangle;
	GameMenuObject[TurretButtonSlow].image = slow_turret_image_array[0];
}

void turret_homing_button_init(void) {
	GameMenuObject[TurretButtonHoming].x_origin = GameMenuObject[TurretButtonSlow].x_origin;
	GameMenuObject[TurretButtonHoming].y_origin = GameMenuObject[TurretButtonSlow].y_origin + GameMenuObject[TurretButtonSlow].height;
	GameMenuObject[TurretButtonHoming].width = GameMenuObject[TurretButtonSlow].width;
	GameMenuObject[TurretButtonHoming].height = GameMenuObject[TurretButtonSlow].height;
	GameMenuObject[TurretButtonHoming].object_type = kObjectRectangle;
	GameMenuObject[TurretButtonHoming].image = slow_turret_image_array[0];
}

void turret_mine_button_init(void) {
	GameMenuObject[TurretButtonMine].x_origin = GameMenuObject[TurretButtonHoming].x_origin;
	GameMenuObject[TurretButtonMine].y_origin = GameMenuObject[TurretButtonHoming].y_origin + GameMenuObject[TurretButtonHoming].height;
	GameMenuObject[TurretButtonMine].width = GameMenuObject[TurretButtonHoming].width;
	GameMenuObject[TurretButtonMine].height = GameMenuObject[TurretButtonBasic].height;
	GameMenuObject[TurretButtonMine].object_type = kObjectRectangle;
	GameMenuObject[TurretButtonMine].image = slow_turret_image_array[0];
}

void EnvironmentInit(LevelData* LevelX) {
	switch (currentGameLevel)
	{
	case 0:
		init_environment_object(0, 2, 2,8, LevelX);
		init_environment_object(1, 3, 4, 8, LevelX);
		init_environment_object(2, 5, 1, 8, LevelX);
		break;
	case 1:
		init_environment_object(0, 0, 1,8, LevelX);
		init_environment_object(1, 1, 4,8,  LevelX);
		init_environment_object(2, 5, 1,8, LevelX);
		init_environment_object(3, 5, 5,8, LevelX);
		init_environment_object(4, 2, 2,8,  LevelX);
		break;
	case 2:
		init_environment_object(0, 3, 3,8,  LevelX);
		init_environment_object(1, 3, 4,9,  LevelX);
		init_environment_object(2, 2, 1, 8, LevelX);
		init_environment_object(3, 5, 5,8,  LevelX);
		init_environment_object(4, 2, 2, 9, LevelX);
		init_environment_object(5, 4, 0, 8, LevelX);
		break;
	case 3:
	init_environment_object(0, 2, 2, 8, LevelX);
		init_environment_object(1, 3, 3, 4, LevelX);
		init_environment_object(2, 1, 0, 8, LevelX);
		init_environment_object(3, 5, 2, 3, LevelX);
		init_environment_object(4, 1, 1, 9, LevelX);
		init_environment_object(5, 3, 1, 9, LevelX);
		init_environment_object(6, 4, 4, 3, LevelX);
		break;
	case 4:
		init_environment_object(0, 0, 2, 0, LevelX);
		init_environment_object(1, 3, 4, 1, LevelX);
		init_environment_object(2, 2, 1, 4, LevelX);
		init_environment_object(3, 5, 5, 3, LevelX);
		init_environment_object(4, 3, 2, 0, LevelX);
		init_environment_object(5, 6, 2, 1, LevelX);
		init_environment_object(6, 5, 1, 4, LevelX);
		init_environment_object(7, 6, 4, 3, LevelX);
		init_environment_object(8, 2, 6, 0, LevelX);
		break;
	}

}

void RenderEnvironment(void) {
	for (int i = 0; i < kMaxEnvironmentObject; i++) {
		if (Environment[i].image == grid_environment_objects_spritesheet)
		{
			RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[Environment[i].object_type], Environment[i].x_origin, Environment[i].y_origin, Environment[i].width, Environment[i].height);
		}

	}
}

void phantomQuartz_init(void) {
	GameMenuObject[PhantomQuartzMenu].y_origin = 0.0f;
	GameMenuObject[PhantomQuartzMenu].x_origin = kRightGameMenuXOrigin;
	GameMenuObject[PhantomQuartzMenu].height = (float)CP_System_GetWindowHeight() / 15;
	GameMenuObject[PhantomQuartzMenu].width = ((float)CP_System_GetWindowWidth() - GameMenuObject[PhantomQuartzMenu].x_origin) / 2;
	GameMenuObject[PhantomQuartzMenu].object_type = kObjectRectangle;
}

void goldQuartz_init(void) {
	GameMenuObject[GoldQuartzMenu].width = GameMenuObject[PhantomQuartzMenu].width;
	GameMenuObject[GoldQuartzMenu].height = GameMenuObject[PhantomQuartzMenu].height;
	GameMenuObject[GoldQuartzMenu].x_origin = (float)CP_System_GetWindowWidth() - GameMenuObject[PhantomQuartzMenu].width;
	GameMenuObject[GoldQuartzMenu].y_origin = GameMenuObject[PhantomQuartzMenu].y_origin;
	GameMenuObject[GoldQuartzMenu].object_type = kObjectRectangle;
}

void health_init(void) {
	GameMenuObject[HealthMenu].x_origin = GameMenuObject[PhantomQuartzMenu].x_origin;
	GameMenuObject[HealthMenu].y_origin = GameMenuObject[PhantomQuartzMenu].y_origin + GameMenuObject[PhantomQuartzMenu].height;
	GameMenuObject[HealthMenu].width = GameMenuObject[PhantomQuartzMenu].width;
	GameMenuObject[HealthMenu].height = GameMenuObject[PhantomQuartzMenu].height;
	GameMenuObject[HealthMenu].object_type = kObjectRectangle;


	/*GameMenuObject[HealthMenu].xOrigin = GameMenuObject[GoldQuartzMenu].xOrigin;
	GameMenuObject[HealthMenu].yOrigin = GameMenuObject[GoldQuartzMenu].yOrigin + GameMenuObject[GoldQuartzMenu].height;
	GameMenuObject[HealthMenu].width = GameMenuObject[GoldQuartzMenu].width;
	GameMenuObject[HealthMenu].height = GameMenuObject[GoldQuartzMenu].height;
	GameMenuObject[HealthMenu].objectType = objectRectangle;*/
}

void menu_swap_init(void) {
	GameMenuObject[SwapButton].x_origin = GameMenuObject[TurretButtonMine].x_origin;
	GameMenuObject[SwapButton].y_origin = GameMenuObject[TurretButtonMine].y_origin + GameMenuObject[TurretButtonMine].height;
	GameMenuObject[SwapButton].width = GameMenuObject[TurretButtonMine].width;
	GameMenuObject[SwapButton].height = GameMenuObject[TurretButtonMine].height / 2;
	GameMenuObject[SwapButton].object_type = kObjectRectangle;
	GameMenuObject[SwapButton].image = CP_Image_Load("Assets/dummy");
}

void wave_number_display_init(void) {
	GameMenuObject[WaveDisplay].x_origin = GameMenuObject[GoldQuartzMenu].x_origin;
	GameMenuObject[WaveDisplay].y_origin = GameMenuObject[HealthMenu].y_origin;
	GameMenuObject[WaveDisplay].width = GameMenuObject[GoldQuartzMenu].width;
	GameMenuObject[WaveDisplay].height = GameMenuObject[GoldQuartzMenu].height;
	GameMenuObject[WaveDisplay].object_type = kObjectRectangle;
}

void battlefield_effects_display_init(void) {
	GameMenuObject[BattlefieldEffects].x_origin = GameMenuObject[HealthMenu].x_origin;
	GameMenuObject[BattlefieldEffects].y_origin = GameMenuObject[HealthMenu].y_origin + GameMenuObject[HealthMenu].height;
	GameMenuObject[BattlefieldEffects].width = GameMenuObject[HealthMenu].width;
	GameMenuObject[BattlefieldEffects].height = GameMenuObject[HealthMenu].height * 3;
	GameMenuObject[BattlefieldEffects].object_type = kObjectRectangle;

	/*GameMenuObject[BattlefieldEffects].yOrigin = GameMenuObject[WaveDisplay].yOrigin + GameMenuObject[WaveDisplay].height;
	GameMenuObject[BattlefieldEffects].width = ((float)CP_System_GetWindowWidth() - Game.xOrigin - Game.width) / 2;
	GameMenuObject[BattlefieldEffects].height = GameMenuObject[GoldQuartzMenu].height * 2;
	GameMenuObject[BattlefieldEffects].xOrigin = (float)CP_System_GetWindowWidth() - GameMenuObject[BattlefieldEffects].width;
	GameMenuObject[BattlefieldEffects].objectType = objectRectangle;*/

}

void monster_remaining_display_init(void) {
	GameMenuObject[MonsterRemainingDisplay].x_origin = GameMenuObject[WaveDisplay].x_origin;
	GameMenuObject[MonsterRemainingDisplay].y_origin = GameMenuObject[WaveDisplay].y_origin + GameMenuObject[WaveDisplay].height;
	GameMenuObject[MonsterRemainingDisplay].width = GameMenuObject[WaveDisplay].width;
	GameMenuObject[MonsterRemainingDisplay].height = GameMenuObject[WaveDisplay].height * 3;
	GameMenuObject[MonsterRemainingDisplay].object_type = kObjectRectangle;


	/*GameMenuObject[MonsterRemainingDisplay].xOrigin = GameMenuObject[BattlefieldEffects].xOrigin;
	GameMenuObject[MonsterRemainingDisplay].yOrigin = GameMenuObject[BattlefieldEffects].yOrigin + GameMenuObject[BattlefieldEffects].height;
	GameMenuObject[MonsterRemainingDisplay].width = GameMenuObject[BattlefieldEffects].width;
	GameMenuObject[MonsterRemainingDisplay].height = GameMenuObject[BattlefieldEffects].height;
	GameMenuObject[MonsterRemainingDisplay].objectType = objectRectangle;*/
}

void upgrade_menu_init(void) {
	GameMenuObject[UpgradeMenu].x_origin = GameMenuObject[BattlefieldEffects].x_origin;
	GameMenuObject[UpgradeMenu].y_origin = GameMenuObject[BattlefieldEffects].y_origin + GameMenuObject[BattlefieldEffects].height;
	GameMenuObject[UpgradeMenu].width = GameMenuObject[BattlefieldEffects].width + GameMenuObject[MonsterRemainingDisplay].width;
	GameMenuObject[UpgradeMenu].height = (CP_System_GetWindowHeight() - GameMenuObject[BattlefieldEffects].y_origin);
	GameMenuObject[UpgradeMenu].object_type = kObjectRectangle;
}

void upgrade_button_init(void) {
	GameMenuObject[UpgradeButton].x_origin = GameMenuObject[BattlefieldEffects].x_origin + GameMenuObject[BattlefieldEffects].width / 5;
	GameMenuObject[UpgradeButton].y_origin = GameMenuObject[SwapButton].y_origin - GameMenuObject[SwapButton].height / 5;
	GameMenuObject[UpgradeButton].width = GameMenuObject[UpgradeMenu].width / 3;
	GameMenuObject[UpgradeButton].height = GameMenuObject[SwapButton].height;
	GameMenuObject[UpgradeButton].object_type = kObjectRectangle;


	/*GameMenuObject[UpgradeButton].xOrigin = GameMenuObject[BattlefieldEffects].xOrigin;
	GameMenuObject[UpgradeButton].yOrigin = GameMenuObject[BattlefieldEffects].yOrigin + GameMenuObject[BattlefieldEffects].height;
	GameMenuObject[UpgradeButton].width = GameMenuObject[BattlefieldEffects].width + GameMenuObject[MonsterRemainingDisplay].width;
	GameMenuObject[UpgradeButton].height = (CP_System_GetWindowHeight() - GameMenuObject[UpgradeButton].yOrigin) / 2;
	GameMenuObject[UpgradeButton].objectType = objectRectangle;*/
}

void sell_button_init(void) {
	GameMenuObject[SellButton].x_origin = GameMenuObject[MonsterRemainingDisplay].x_origin + GameMenuObject[MonsterRemainingDisplay].width / 5;
	GameMenuObject[SellButton].y_origin = GameMenuObject[SwapButton].y_origin - GameMenuObject[SwapButton].height / 5;
	GameMenuObject[SellButton].width = GameMenuObject[UpgradeMenu].width / 3;
	GameMenuObject[SellButton].height = GameMenuObject[SwapButton].height;
	GameMenuObject[SellButton].object_type = kObjectRectangle;


	/*GameMenuObject[SellButton].xOrigin = GameMenuObject[UpgradeButton].xOrigin;
	GameMenuObject[SellButton].yOrigin = GameMenuObject[UpgradeButton].yOrigin + GameMenuObject[UpgradeButton].height;
	GameMenuObject[SellButton].width = GameMenuObject[UpgradeButton].width;
	GameMenuObject[SellButton].height = GameMenuObject[UpgradeButton].height;
	GameMenuObject[SellButton].objectType = objectRectangle;*/
}

void PowerUpPriceInit(void) {
	powerUpPrice.more_phantom_quartz = 10;
	powerUpPrice.reduce_enemy_speed = 10;
	powerUpPrice.reduce_enemy_health = 10;
	powerUpPrice.increased_mine_damage = 10;
}

void turret_details_init(enum MenuObjectType turretButton) {
	GameMenuObject[TurretDetailsDisplay].x_origin = GameMenuObject[turretButton].x_origin + GameMenuObject[turretButton].width;
	GameMenuObject[TurretDetailsDisplay].y_origin = GameMenuObject[turretButton].y_origin + GameMenuObject[turretButton].width/4;
	GameMenuObject[TurretDetailsDisplay].width = GameMenuObject[turretButton].width*1.25f;
	GameMenuObject[TurretDetailsDisplay].height = GameMenuObject[turretButton].height/2;
	GameMenuObject[TurretDetailsDisplay].object_type = kObjectRectangle;
}

void RenderTurretDetailsDisplay(void) {
	float tempMouseX = MouseInput.x_origin, tempMouseY = MouseInput.y_origin;
	MouseInput.x_origin = CP_Input_GetMouseX();
	MouseInput.y_origin = CP_Input_GetMouseY();
	turret_details_init(CheckGameButtonPressed());
	switch (CheckGameButtonPressed()) {
	case TurretButtonBasic:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(GameMenuObject[TurretDetailsDisplay].x_origin, GameMenuObject[TurretDetailsDisplay].y_origin, GameMenuObject[TurretDetailsDisplay].width, GameMenuObject[TurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scalingFactor);
		CP_Font_DrawText("Basic Turret", GameMenuObject[TurretDetailsDisplay].x_origin + GameMenuObject[TurretDetailsDisplay].width / 2, (GameMenuObject[TurretDetailsDisplay].y_origin + GameMenuObject[TurretDetailsDisplay].height / 3));
		CP_Font_DrawText("Single Target Damage", GameMenuObject[TurretDetailsDisplay].x_origin + GameMenuObject[TurretDetailsDisplay].width / 2, (GameMenuObject[TurretDetailsDisplay].y_origin + GameMenuObject[TurretDetailsDisplay].height * 2 / 3));
		break;
	case TurretButtonSlow:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(GameMenuObject[TurretDetailsDisplay].x_origin, GameMenuObject[TurretDetailsDisplay].y_origin, GameMenuObject[TurretDetailsDisplay].width, GameMenuObject[TurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scalingFactor);
		CP_Font_DrawText("Slow Turret", GameMenuObject[TurretDetailsDisplay].x_origin + GameMenuObject[TurretDetailsDisplay].width / 2, (GameMenuObject[TurretDetailsDisplay].y_origin + GameMenuObject[TurretDetailsDisplay].height / 3));
		CP_Font_DrawText("Slow Enemies", GameMenuObject[TurretDetailsDisplay].x_origin + GameMenuObject[TurretDetailsDisplay].width / 2, (GameMenuObject[TurretDetailsDisplay].y_origin + GameMenuObject[TurretDetailsDisplay].height * 2 / 3)); 
		break;
	case TurretButtonHoming:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(GameMenuObject[TurretDetailsDisplay].x_origin, GameMenuObject[TurretDetailsDisplay].y_origin, GameMenuObject[TurretDetailsDisplay].width, GameMenuObject[TurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scalingFactor);
		CP_Font_DrawText("Homing Turret", GameMenuObject[TurretDetailsDisplay].x_origin + GameMenuObject[TurretDetailsDisplay].width / 2, (GameMenuObject[TurretDetailsDisplay].y_origin + GameMenuObject[TurretDetailsDisplay].height / 3));
		CP_Font_DrawText("Splash Damage", GameMenuObject[TurretDetailsDisplay].x_origin + GameMenuObject[TurretDetailsDisplay].width / 2, (GameMenuObject[TurretDetailsDisplay].y_origin + GameMenuObject[TurretDetailsDisplay].height * 2 / 3)); 
		break;
	case TurretButtonMine:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(GameMenuObject[TurretDetailsDisplay].x_origin, GameMenuObject[TurretDetailsDisplay].y_origin, GameMenuObject[TurretDetailsDisplay].width, GameMenuObject[TurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scalingFactor);
		CP_Font_DrawText("Mine", GameMenuObject[TurretDetailsDisplay].x_origin + GameMenuObject[TurretDetailsDisplay].width / 2, (GameMenuObject[TurretDetailsDisplay].y_origin + GameMenuObject[TurretDetailsDisplay].height / 3));
		CP_Font_DrawText("Single Use", GameMenuObject[TurretDetailsDisplay].x_origin + GameMenuObject[TurretDetailsDisplay].width / 2, (GameMenuObject[TurretDetailsDisplay].y_origin + GameMenuObject[TurretDetailsDisplay].height * 2 / 3)); 
		break;
	}
	MouseInput.x_origin = tempMouseX;
	MouseInput.y_origin = tempMouseY;
}

void Level1Init(void) {
	level_grid_cols = kLevel1Cols;
	level_grid_rows = kLevel1Rows;
	currentGameLevel = 0;
	Level[currentGameLevel].grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));// using size of pointers so that lower bits operating system do not require so much malloc
	for (int i = 0; i < level_grid_rows; i++) {
		if (Level[currentGameLevel].grid != NULL) {
			Level[currentGameLevel].grid[i] = (struct Grids*)calloc(level_grid_cols, sizeof(struct Grids));
			if (Level[currentGameLevel].grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}



	Level[0].spawnRow = 0;
	Level[0].spawnCol = 0;
	Level[0].exitRow = level_grid_rows - 1;
	Level[0].exitCol = (level_grid_cols - 1);
	Level[0].health = 100;
	Level[0].phantomQuartz = 50000;
	Level[0].goldQuartz = 0;
	Level[0].currentWave = 0;
	Level[0].currentEffect = kNoEnvironmentalEffects;
	Level[0].currentPowerUpLevel.more_phantom_quartz = 0;
	Level[0].currentPowerUpLevel.reduce_enemy_speed = 0;
	Level[0].currentPowerUpLevel.reduce_enemy_health = 0;
	Level[0].currentPowerUpLevel.increased_mine_damage = 0;

	general_level_enemies_init(0, 0, 10, 0, 0, 0);
	general_level_enemies_init(0, 1, 10, 0, 0, 0);
	general_level_enemies_init(0, 2, 15, 0, 0, 0);
	general_level_enemies_init(0, 3, 15, 0, 0, 0);
	general_level_enemies_init(0, 4, 15, 0, 0, 0);
	general_level_enemies_init(0, 5, 20, 0, 0, 0);
	general_level_enemies_init(0, 6, 20, 0, 0, 0);
	general_level_enemies_init(0, 7, 25, 0, 0, 0);
	general_level_enemies_init(0, 8, 30, 0, 0, 0);
	general_level_enemies_init(0, 9, 30, 0, 0, 0);

	game_grid_init();
	is_placing_turret = T_MAX;
	turretSelectedToUpgrade = NO_TURRET_SELECTED;
	power_up_menu = kFalse;
	pathfinding_init(&Level[currentGameLevel]);
	EnvironmentInit(&Level[currentGameLevel]);

	//turret menu items
	pause_button_init();
	turret_basic_button_init();
	turret_slow_button_init();
	turret_homing_button_init();
	turret_mine_button_init();

	phantomQuartz_init();
	goldQuartz_init();
	health_init();
	menu_swap_init();
	wave_number_display_init();
	battlefield_effects_display_init();
	monster_remaining_display_init();
	upgrade_menu_init();
	upgrade_button_init();
	sell_button_init();

	turret_init();
	Enemies_init();

	PathFindingReset(&Level[currentGameLevel]);
	PathFindingCalculateCost(&Level[currentGameLevel]);
	PathFindingUpdate(&Level[currentGameLevel]);

	SetBuildingTime(BUILDING_PHASE_TIME);
	current_game_state = kBuilding;
}

void Level2Init(void) {
	level_grid_cols = kLevel2Cols;
	level_grid_rows = kLevel2Rows;
	currentGameLevel = 1;
	Level[currentGameLevel].grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));// using size of pointers so that lower bits operating system do not require so much malloc
	for (int i = 0; i < level_grid_rows; i++) {
		if (Level[currentGameLevel].grid != NULL) {
			Level[currentGameLevel].grid[i] = (struct Grids*)calloc(level_grid_cols, sizeof(struct Grids));
			if (Level[currentGameLevel].grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	Level[1].spawnRow = 0;
	Level[1].spawnCol = 0;
	Level[1].exitRow = level_grid_rows - 1;
	Level[1].exitCol = (level_grid_cols - 1);
	Level[1].health = 100;
	Level[1].phantomQuartz = 200;
	Level[1].goldQuartz = 0;
	Level[1].currentWave = 0;
	Level[1].currentEffect = kNoEnvironmentalEffects;
	Level[1].currentPowerUpLevel.more_phantom_quartz = 0;
	Level[1].currentPowerUpLevel.reduce_enemy_speed = 0;
	Level[1].currentPowerUpLevel.reduce_enemy_health = 0;
	Level[1].currentPowerUpLevel.increased_mine_damage = 0;


	general_level_enemies_init(1, 0, 10, 0, 0, 0);
	general_level_enemies_init(1, 1, 10, 0, 0, 0);
	general_level_enemies_init(1, 2, 15, 1, 0, 0);
	general_level_enemies_init(1, 3, 15, 5, 0, 0);
	general_level_enemies_init(1, 4, 20, 5, 0, 0);
	general_level_enemies_init(1, 5, 20, 10, 0, 0);
	general_level_enemies_init(1, 6, 25, 10, 0, 0);
	general_level_enemies_init(1, 7, 25, 10, 0, 0);
	general_level_enemies_init(1, 8, 30, 10, 0, 0);
	general_level_enemies_init(1, 9, 25, 15, 0, 0);

	game_grid_init();
	is_placing_turret = T_MAX;
	turretSelectedToUpgrade = NO_TURRET_SELECTED;
	power_up_menu = kFalse;
	pathfinding_init(&Level[currentGameLevel]);
	EnvironmentInit(&Level[currentGameLevel]);

	//turret menu items
	pause_button_init();
	turret_basic_button_init();
	turret_slow_button_init();
	turret_homing_button_init();
	turret_mine_button_init();

	phantomQuartz_init();
	goldQuartz_init();
	health_init();
	menu_swap_init();
	wave_number_display_init();
	battlefield_effects_display_init();
	monster_remaining_display_init();
	upgrade_menu_init();
	upgrade_button_init();
	sell_button_init();

	turret_init();
	Enemies_init();

	PathFindingReset(&Level[currentGameLevel]);
	PathFindingCalculateCost(&Level[currentGameLevel]);
	PathFindingUpdate(&Level[currentGameLevel]);

	SetBuildingTime(BUILDING_PHASE_TIME);
	current_game_state = kBuilding;
}

void Level3Init(void) {
	level_grid_cols = kLevel3Cols;
	level_grid_rows = kLevel3Rows;
	currentGameLevel = 2;
	Level[currentGameLevel].grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));// using size of pointers so that lower bits operating system do not require so much malloc
	for (int i = 0; i < level_grid_rows; i++) {
		if (Level[currentGameLevel].grid != NULL) {
			Level[currentGameLevel].grid[i] = (struct Grids*)calloc(level_grid_cols, sizeof(struct Grids));
			if (Level[currentGameLevel].grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	Level[2].spawnRow = level_grid_rows - 1;
	Level[2].spawnCol = (level_grid_cols - 1);
	Level[2].exitRow = 0;
	Level[2].exitCol = 0;
	Level[2].health = 100;
	Level[2].phantomQuartz = 200;
	Level[2].goldQuartz = 0;
	Level[2].currentWave = 0;
	Level[2].currentEffect = kNoEnvironmentalEffects;
	Level[2].currentPowerUpLevel.more_phantom_quartz = 0;
	Level[2].currentPowerUpLevel.reduce_enemy_speed = 0;
	Level[2].currentPowerUpLevel.reduce_enemy_health = 0;
	Level[2].currentPowerUpLevel.increased_mine_damage = 0;

	general_level_enemies_init(2, 0, 10, 0, 0, 0);
	general_level_enemies_init(2, 1, 10, 5, 0, 0);
	general_level_enemies_init(2, 2, 10, 5, 0, 0);
	general_level_enemies_init(2, 3, 15, 5, 0, 0);
	general_level_enemies_init(2, 4, 15, 10, 1, 0);
	general_level_enemies_init(2, 5, 15, 10, 5, 0);
	general_level_enemies_init(2, 6, 15, 10, 5, 0);
	general_level_enemies_init(2, 7, 20, 10, 5, 0);
	general_level_enemies_init(2, 8, 20, 10, 10, 0);
	general_level_enemies_init(2, 9, 25, 10, 10, 0);

	game_grid_init();
	is_placing_turret = T_MAX;
	turretSelectedToUpgrade = NO_TURRET_SELECTED;
	power_up_menu = kFalse;
	pathfinding_init(&Level[currentGameLevel]);
	EnvironmentInit(&Level[currentGameLevel]);

	//turret menu items
	pause_button_init();
	turret_basic_button_init();
	turret_slow_button_init();
	turret_homing_button_init();
	turret_mine_button_init();

	phantomQuartz_init();
	goldQuartz_init();
	health_init();
	menu_swap_init();
	wave_number_display_init();
	battlefield_effects_display_init();
	monster_remaining_display_init();
	upgrade_menu_init();
	upgrade_button_init();
	sell_button_init();

	turret_init();
	Enemies_init();

	PathFindingReset(&Level[currentGameLevel]);
	PathFindingCalculateCost(&Level[currentGameLevel]);
	PathFindingUpdate(&Level[currentGameLevel]);

	SetBuildingTime(BUILDING_PHASE_TIME);
	current_game_state = kBuilding;

}

void Level4Init(void) {
	level_grid_cols = kLevel4Cols;
	level_grid_rows = kLevel4Rows;
	currentGameLevel = 3;
	Level[currentGameLevel].grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));// using size of pointers so that lower bits operating system do not require so much malloc
	for (int i = 0; i < level_grid_rows; i++) {
		if (Level[currentGameLevel].grid != NULL) {
			Level[currentGameLevel].grid[i] = (struct Grids*)calloc(level_grid_cols, sizeof(struct Grids));
			if (Level[currentGameLevel].grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	Level[3].spawnRow = 0;
	Level[3].spawnCol = 0;
	Level[3].exitRow = level_grid_rows - 1;
	Level[3].exitCol = 0;
	Level[3].health = 100;
	Level[3].phantomQuartz = 200;
	Level[3].goldQuartz = 0;
	Level[3].currentWave = 0;
	Level[3].currentEffect = kNoEnvironmentalEffects;
	Level[3].currentPowerUpLevel.more_phantom_quartz = 0;
	Level[3].currentPowerUpLevel.reduce_enemy_speed = 0;
	Level[3].currentPowerUpLevel.reduce_enemy_health = 0;
	Level[3].currentPowerUpLevel.increased_mine_damage = 0;


	general_level_enemies_init(3, 0, 10, 0, 0, 0);
	general_level_enemies_init(3, 1, 10, 5, 0, 0);
	general_level_enemies_init(3, 2, 10, 3, 2, 0);
	general_level_enemies_init(3, 3, 10, 5, 5, 0);
	general_level_enemies_init(3, 4, 15, 10, 5, 0);
	general_level_enemies_init(3, 5, 15, 10, 5, 0);
	general_level_enemies_init(3, 6, 15, 10, 10, 0);
	general_level_enemies_init(3, 7, 10, 10, 10, 1);
	general_level_enemies_init(3, 8, 15, 15, 10, 2);
	general_level_enemies_init(3, 9, 20, 10, 15, 3);

	game_grid_init();
	is_placing_turret = T_MAX;
	turretSelectedToUpgrade = NO_TURRET_SELECTED;
	power_up_menu = kFalse;
	pathfinding_init(&Level[currentGameLevel]);
	EnvironmentInit(&Level[currentGameLevel]);

	//turret menu items
	pause_button_init();
	turret_basic_button_init();
	turret_slow_button_init();
	turret_homing_button_init();
	turret_mine_button_init();

	phantomQuartz_init();
	goldQuartz_init();
	health_init();
	menu_swap_init();
	wave_number_display_init();
	battlefield_effects_display_init();
	monster_remaining_display_init();
	upgrade_menu_init();
	upgrade_button_init();
	sell_button_init();

	turret_init();
	Enemies_init();

	PathFindingReset(&Level[currentGameLevel]);
	PathFindingCalculateCost(&Level[currentGameLevel]);
	PathFindingUpdate(&Level[currentGameLevel]);

	SetBuildingTime(BUILDING_PHASE_TIME);
	current_game_state = kBuilding;
}

void Level5Init(void) {
	level_grid_cols = kLevel5Cols;
	level_grid_rows = kLevel5Rows;
	currentGameLevel = 4;
	Level[currentGameLevel].grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));// using size of pointers so that lower bits operating system do not require so much malloc
	for (int i = 0; i < level_grid_rows; i++) {
		if (Level[currentGameLevel].grid != NULL) {
			Level[currentGameLevel].grid[i] = (struct Grids*)calloc(level_grid_cols, sizeof(struct Grids));
			if (Level[currentGameLevel].grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	Level[4].spawnRow = 0;
	Level[4].spawnCol = (level_grid_cols - 1) / 2;
	Level[4].exitRow = level_grid_rows - 1;
	Level[4].exitCol = (level_grid_cols - 1) / 2;
	Level[4].health = 100;
	Level[4].phantomQuartz = 200;
	Level[4].goldQuartz = 0;
	Level[4].currentWave = 0;
	Level[4].currentEffect = kNoEnvironmentalEffects;
	Level[4].currentPowerUpLevel.more_phantom_quartz = 0;
	Level[4].currentPowerUpLevel.reduce_enemy_speed = 0;
	Level[4].currentPowerUpLevel.reduce_enemy_health = 0;
	Level[4].currentPowerUpLevel.increased_mine_damage = 0;

	general_level_enemies_init(4, 0, 10, 0, 0, 0);
	general_level_enemies_init(4, 1, 15, 0, 0, 0);
	general_level_enemies_init(4, 2, 15, 5, 0, 0);
	general_level_enemies_init(4, 3, 10, 5, 5, 0);
	general_level_enemies_init(4, 4, 15, 5, 5, 0);
	general_level_enemies_init(4, 5, 10, 10, 10, 0);
	general_level_enemies_init(4, 6, 15, 10, 10, 0);
	general_level_enemies_init(4, 7, 15, 10, 10, 1);
	general_level_enemies_init(4, 8, 15, 15, 15, 2);
	general_level_enemies_init(4, 9, 10, 10, 10, 5);

	game_grid_init();

	//turret menu items
	pause_button_init();
	turret_basic_button_init();
	turret_slow_button_init();
	turret_homing_button_init();
	turret_mine_button_init();

	phantomQuartz_init();
	goldQuartz_init();
	health_init();
	menu_swap_init();
	wave_number_display_init();
	battlefield_effects_display_init();
	monster_remaining_display_init();
	upgrade_menu_init();
	upgrade_button_init();
	sell_button_init();

	is_placing_turret = T_MAX;
	turretSelectedToUpgrade = NO_TURRET_SELECTED;
	power_up_menu = kFalse;
	pathfinding_init(&Level[currentGameLevel]);
	EnvironmentInit(&Level[currentGameLevel]);
	turret_init();
	Enemies_init();

	PathFindingReset(&Level[currentGameLevel]);
	PathFindingCalculateCost(&Level[currentGameLevel]);
	PathFindingUpdate(&Level[currentGameLevel]);

	SetBuildingTime(BUILDING_PHASE_TIME);
	current_game_state = kBuilding;
}

void ButtonPressedUpdate(void) {
	switch (CheckGameButtonPressed())
	{
	case GameGrid:
		render_game_grid_press(&Level[currentGameLevel]);
		break;
	case PauseButton:
		current_game_state = current_game_state == kPause ? kWave : kPause;
		MouseReset();
		break;

	case TurretButtonBasic:
		if (turret_purchasing[TP_PRICE][T_BASIC] <= Level[currentGameLevel].phantomQuartz && power_up_menu == kFalse) { // Currently hardcoded 
			is_placing_turret = T_BASIC;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			RenderImageFromSpriteSheet(basic_turret_spritesheet, basic_turret_spritesheet_array[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), game.grid_width, game.grid_height);
		}
		else if (power_up_menu == kTrue && Level[currentGameLevel].goldQuartz >= powerUpPrice.more_phantom_quartz) {
			Level[currentGameLevel].currentPowerUpLevel.more_phantom_quartz += 1;
			Level[currentGameLevel].goldQuartz -= powerUpPrice.more_phantom_quartz;
			MouseReset();
			is_placing_turret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
		}
		else {
			is_placing_turret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
		}
		break;

	case TurretButtonSlow:
		if (turret_purchasing[TP_PRICE][T_SLOW] <= Level[currentGameLevel].phantomQuartz && power_up_menu == kFalse) {
			is_placing_turret = T_SLOW;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			CP_Image_DrawAdvanced(GameMenuObject[CheckGameButtonPressed()].image, CP_Input_GetMouseX(), CP_Input_GetMouseY(), game.grid_width, game.grid_height, 255, 0);
		}
		else if (power_up_menu == kTrue && Level[currentGameLevel].goldQuartz >= powerUpPrice.reduce_enemy_speed) {
			Level[currentGameLevel].currentPowerUpLevel.reduce_enemy_speed += 1;
			Level[currentGameLevel].goldQuartz -= powerUpPrice.reduce_enemy_speed;
			MouseReset();
			is_placing_turret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
		}
		else {
			is_placing_turret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
		}
		break;

	case TurretButtonHoming:
		if (turret_purchasing[TP_PRICE][T_HOMING] <= Level[currentGameLevel].phantomQuartz && power_up_menu == kFalse) {
			is_placing_turret = T_HOMING;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			RenderImageFromSpriteSheet(homing_missle_turret_spritesheet, homing_missle_turret_spritesheet_array[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), game.grid_width, game.grid_height);
		}
		else if (power_up_menu == kTrue && Level[currentGameLevel].goldQuartz >= powerUpPrice.reduce_enemy_health) {
			Level[currentGameLevel].currentPowerUpLevel.reduce_enemy_health += 1;
			Level[currentGameLevel].goldQuartz -= powerUpPrice.reduce_enemy_health;
			MouseReset();
			is_placing_turret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
		}
		else {
			is_placing_turret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
		}
		break;
	case TurretButtonMine:
		if (turret_purchasing[TP_PRICE][T_MINE] <= Level[currentGameLevel].phantomQuartz && power_up_menu == kFalse) {
			is_placing_turret = T_MINE;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			RenderImageFromSpriteSheet(mine_spritesheet, mine_spritesheet_array[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), game.grid_width, game.grid_height);
		}
		else if (power_up_menu == kTrue && Level[currentGameLevel].goldQuartz >= powerUpPrice.increased_mine_damage) {
			Level[currentGameLevel].currentPowerUpLevel.increased_mine_damage += 1;
			Level[currentGameLevel].goldQuartz -= powerUpPrice.increased_mine_damage;
			MouseReset();
			is_placing_turret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
		}
		else {
			is_placing_turret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
		}
		break;

	case SwapButton:
		is_placing_turret = T_MAX;
		turretSelectedToUpgrade = NO_TURRET_SELECTED;
		power_up_menu = !power_up_menu;
		MouseReset();
		break;

	case GoldQuartzMenu:
		is_placing_turret = T_MAX;
		turretSelectedToUpgrade = NO_TURRET_SELECTED;
		if (Level[currentGameLevel].phantomQuartz >= 1000) {
			Level[currentGameLevel].phantomQuartz -= 1000;
			Level[currentGameLevel].goldQuartz += 10;
		}
		MouseReset();
		break;

	case UpgradeMenu:
		is_placing_turret = T_MAX;
		break;

	case UpgradeButton:
		if (turretSelectedToUpgrade != NO_TURRET_SELECTED) {
			if (turret[turretSelectedToUpgrade].type != T_MINE) {
				if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) && Level[currentGameLevel].phantomQuartz >= turret[turretSelectedToUpgrade].upgrade_price) {
					if (turret[turretSelectedToUpgrade].level < 10)
					{
						Level[currentGameLevel].phantomQuartz -= turret[turretSelectedToUpgrade].upgrade_price;
						upgrade_turret(turretSelectedToUpgrade);
					}

					is_placing_turret = T_MAX;
					//call upgrade function
				}
			}
			else {
				is_placing_turret = T_MAX;
			}
		}
		else {
			is_placing_turret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
		}
		break;

	case SellButton:
		if (turretSelectedToUpgrade != NO_TURRET_SELECTED) {
			int drawX, drawY;
			drawX = (int)((turret[turretSelectedToUpgrade].data.x_origin - game.x_origin) / game.grid_width);
			drawY = (int)((turret[turretSelectedToUpgrade].data.y_origin - game.y_origin) / game.grid_height);
			sell_turret(turretSelectedToUpgrade);
			Level[currentGameLevel].grid[drawY][drawX].type = kClear;
			PathFindingReset(&Level[currentGameLevel]);
			PathFindingCalculateCost(&Level[currentGameLevel]);
			PathFindingUpdate(&Level[currentGameLevel]);
			MouseReset();
			is_placing_turret = T_MAX;
		}
		else {
			is_placing_turret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
		}
		break;

	default:
		is_placing_turret = T_MAX;
		turretSelectedToUpgrade = NO_TURRET_SELECTED;
		break;
	}
}

void RenderGameGrid(void)
{
	int currentGridRow = 0, currentGridCol = 0;
	CP_Graphics_ClearBackground(COLOR_WHITE);
	CP_Settings_Fill(COLOR_BLACK);
	while (currentGridRow <= level_grid_rows) {
		CP_Graphics_DrawLine((game.x_origin), (game.y_origin + game.grid_height * currentGridRow), (game.x_origin + game.width), (game.y_origin + game.grid_height * currentGridRow));
		currentGridRow++;
	}
	while (currentGridCol <= level_grid_cols) {
		CP_Graphics_DrawLine((game.x_origin + game.grid_width * currentGridCol), (game.y_origin), (game.x_origin + game.grid_width * currentGridCol), (game.y_origin + game.height));
		currentGridCol++;
	}
}

void render_turret_menu_object(Coordinates menuObjectX, enum MenuObjectType type) {
	char temp[100];
	if (turretSelectedToUpgrade == NO_TURRET_SELECTED && (type == UpgradeButton || type == SellButton || type == UpgradeMenu)) {// only render upgrade and sell button when turret selected
	//empty by design
	}
	else if (turret[turretSelectedToUpgrade].type == T_MINE && type == UpgradeButton) { //Hide the upgrade menu for mine
		//empty by design
	}
	else {
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(menuObjectX.x_origin, menuObjectX.y_origin, menuObjectX.width, menuObjectX.height);
	}
	CP_Settings_TextSize(35.0f * scalingFactor);
	switch (type)
	{

	case TurretButtonBasic:
		CP_Image_Draw(turret_button_background, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 2),
			138 * scalingFactor, 144 * scalingFactor, 255);
		if (power_up_menu == kFalse) {
			RenderImageFromSpriteSheet(basic_turret_spritesheet, basic_turret_spritesheet_array[0], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 128 * scalingFactor, 128 * scalingFactor);
			sprintf_s(temp, sizeof(temp), "%d", turret_purchasing[TP_PRICE][T_BASIC]);
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.width / 1.45f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scalingFactor, 45 * scalingFactor);
		}

		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet, power_up_spritesheet_array[0], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 100 * scalingFactor, 100 * scalingFactor);
			sprintf_s(temp, sizeof(temp), "Lv:%-2d", Level[currentGameLevel].currentPowerUpLevel.more_phantom_quartz);
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 7));

			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[0], menuObjectX.width / 1.65f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scalingFactor, 45 * scalingFactor);
		}
		break;

	case TurretButtonSlow:
		CP_Image_Draw(turret_button_background, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 2),
			138 * scalingFactor, 144 * scalingFactor, 255);
		if (power_up_menu == kFalse) {
			CP_Image_DrawAdvanced(menuObjectX.image, menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 128 * scalingFactor,
				128 * scalingFactor, 255, 90);
			sprintf_s(temp, sizeof(temp), "%d", turret_purchasing[TP_PRICE][T_SLOW]);
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.width / 1.45f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scalingFactor, 45 * scalingFactor);
		}
		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet, power_up_spritesheet_array[1], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 100 * scalingFactor, 100 * scalingFactor);


			sprintf_s(temp, sizeof(temp), "Lv:%-2d", Level[currentGameLevel].currentPowerUpLevel.reduce_enemy_speed);
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 7));

			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[0], menuObjectX.width / 1.65f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scalingFactor, 45 * scalingFactor);
		}
		break;

	case  TurretButtonHoming:
		CP_Image_Draw(turret_button_background, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 2),
			138 * scalingFactor, 144 * scalingFactor, 255);
		if (power_up_menu == kFalse) {
			RenderImageFromSpriteSheet(homing_missle_turret_spritesheet, homing_missle_turret_spritesheet_array[0], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 128 * scalingFactor, 128 * scalingFactor);
			sprintf_s(temp, sizeof(temp), "%d", turret_purchasing[TP_PRICE][T_HOMING]);
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.width / 1.45f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scalingFactor, 45 * scalingFactor);
		}
		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet, power_up_spritesheet_array[2], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 100 * scalingFactor, 100 * scalingFactor);
			sprintf_s(temp, sizeof(temp), "Lv:%-2d", Level[currentGameLevel].currentPowerUpLevel.reduce_enemy_health);
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 7));

			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[0], menuObjectX.width / 1.65f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scalingFactor, 45 * scalingFactor);

		}
		break;

	case TurretButtonMine:
		CP_Image_Draw(turret_button_background, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 2),
			138 * scalingFactor, 144 * scalingFactor, 255);
		if (power_up_menu == kFalse) {
			RenderImageFromSpriteSheet(mine_spritesheet, mine_spritesheet_array[0], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 128 * scalingFactor, 128 * scalingFactor);
			sprintf_s(temp, sizeof(temp), "%d", turret_purchasing[TP_PRICE][T_MINE]);
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.width / 1.45f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scalingFactor, 45 * scalingFactor);
		}
		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet, power_up_spritesheet_array[3], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 100 * scalingFactor, 100 * scalingFactor);

			sprintf_s(temp, sizeof(temp), "Lv:%-2d", Level[currentGameLevel].currentPowerUpLevel.increased_mine_damage);
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 7));


			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[0], menuObjectX.width / 1.65f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scalingFactor, 45 * scalingFactor);
		}

		break;

	case PauseButton:
		CP_Image_DrawAdvanced(menuObjectX.image, menuObjectX.width / 2,
			(menuObjectX.y_origin + menuObjectX.height / 2), 64,
			64, 255, 0);
		break;
	case SwapButton:
		RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet, interactable_UI_buttons_spritesheet_array[2],
			menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 137, 72);
		break;
	case GoldQuartzMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scalingFactor);
		CP_Image_Draw(thin_UI_background, menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scalingFactor, 46 * scalingFactor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[0], menuObjectX.x_origin + menuObjectX.width / 5.5f,
			menuObjectX.y_origin + menuObjectX.height / 2, 30 * scalingFactor, 30 * scalingFactor);
		RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[4], menuObjectX.x_origin + menuObjectX.width / 1.3f,
			menuObjectX.y_origin + menuObjectX.height / 2, 28 * scalingFactor, 28 * scalingFactor);
		sprintf_s(temp, 100, "x%-10d", Level[currentGameLevel].goldQuartz);
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.55f, menuObjectX.y_origin + menuObjectX.height / 2);
		break;
	case PhantomQuartzMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scalingFactor);
		CP_Image_Draw(thin_UI_background, menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scalingFactor, 46 * scalingFactor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 5,
			menuObjectX.y_origin + menuObjectX.height / 2, 30 * scalingFactor, 30 * scalingFactor);
		sprintf_s(temp, 100, "x%-10d", Level[currentGameLevel].phantomQuartz);
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.45f, menuObjectX.y_origin + menuObjectX.height / 2);
		break;
	case HealthMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scalingFactor);
		sprintf_s(temp, 100, "x%-10d", Level[currentGameLevel].health);
		CP_Image_Draw(thin_UI_background, menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scalingFactor, 46 * scalingFactor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[2], menuObjectX.x_origin + menuObjectX.width / 5,
			menuObjectX.y_origin + menuObjectX.height / 2, 30 * scalingFactor, 30 * scalingFactor);
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.45f, menuObjectX.y_origin + menuObjectX.height / 2);
		break;
	case WaveDisplay:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scalingFactor);
		sprintf_s(temp, 100, "%2d/%d", Level[currentGameLevel].currentWave + 1, MAX_NUMBER_OF_WAVES);
		CP_Image_Draw(thin_UI_background, menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scalingFactor, 46 * scalingFactor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[3], menuObjectX.x_origin + menuObjectX.width / 5,
			menuObjectX.y_origin + menuObjectX.height / 2, 30 * scalingFactor, 30 * scalingFactor);
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2.5f, menuObjectX.y_origin + menuObjectX.height / 2);
		break;
	case BattlefieldEffects:
		RenderImageFromSpriteSheet(fat_UI_background_spritesheet, fat_UI_background_spritesheet_array[0], menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scalingFactor, 142 * scalingFactor);
		CP_Settings_TextSize(35.0f * scalingFactor);
		CP_Settings_Fill(COLOR_WHITE);
		sprintf_s(temp, sizeof(temp), "Effects");
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 5);
		printf("%d", Level[currentGameLevel].currentEffect);
		RenderImageFromSpriteSheet(battlefield_effect_spritesheet, battlefield_effect_spritesheet_array[Level[currentGameLevel].currentEffect],
			menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 1.65f,
			85 * scalingFactor, 85 * scalingFactor);
		break;
	case MonsterRemainingDisplay:
		RenderImageFromSpriteSheet(fat_UI_background_spritesheet, fat_UI_background_spritesheet_array[0], menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scalingFactor, 142 * scalingFactor);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(35.0f * scalingFactor);
		sprintf_s(temp, sizeof(temp), "Enemies");
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 5);
		int totalEnemies = 0;
		for (int i = 0; i < kMaxEnemyType; i++) {
			if (current_game_state == kWave) {
				totalEnemies += Level[currentGameLevel].waveEnemies[Level[currentGameLevel].currentWave][i];
			}
			else if (current_game_state == kBuilding) { // Forecast for next wave instead of current empty wave
				totalEnemies += Level[currentGameLevel].waveEnemies[Level[currentGameLevel].currentWave][i];
			}
		}
		sprintf_s(temp, sizeof(temp), "%d/%d", enemiesLeft, totalEnemies);
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 2);
		break;
	case UpgradeMenu:
		if (turretSelectedToUpgrade != NO_TURRET_SELECTED) { //Only render when upgrading

			CP_Image_Draw(upgrade_menu_background, menuObjectX.x_origin + menuObjectX.width / 2,
				menuObjectX.y_origin + menuObjectX.height / 2.6f, 275 * scalingFactor, 480 * scalingFactor, 255);

			CP_Settings_Fill(COLOR_WHITE);
			CP_Settings_TextSize(50.0f * scalingFactor);


			//Level Icon
			RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[0], menuObjectX.x_origin + menuObjectX.width / 1.75f,
				menuObjectX.y_origin + menuObjectX.height / 5.75f, 64 * scalingFactor, 64 * scalingFactor);
			if (turret[turretSelectedToUpgrade].level == 10)
			{
				sprintf_s(temp, sizeof(temp), "MAX");
			}

			else
			{
				sprintf_s(temp, sizeof(temp), "%-3d", turret[turretSelectedToUpgrade].level);
			}

			CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.2f,
				menuObjectX.y_origin + menuObjectX.height / 5.75f);

			switch (turret[turretSelectedToUpgrade].type)
			{
			case T_BASIC:
				sprintf_s(temp, sizeof(temp), "Basic");
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 18);
				RenderImageFromSpriteSheet(basic_turret_spritesheet, basic_turret_spritesheet_array[0],
					menuObjectX.x_origin + menuObjectX.width / 4,
					menuObjectX.y_origin + menuObjectX.height / 5.75f, 110 * scalingFactor, 110 * scalingFactor);
				RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 5,
					menuObjectX.y_origin + menuObjectX.height / 3.25f, 64 * scalingFactor, 64 * scalingFactor);
				sprintf_s(temp, sizeof(temp), "%.2f", turret[turretSelectedToUpgrade].mod.damage);
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.95f, menuObjectX.y_origin + menuObjectX.height / 3.25f);
				break;

			case T_SLOW:
				sprintf_s(temp, sizeof(temp), "Slow");
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 18);
				CP_Image_DrawAdvanced(slow_turret_image_array[0], menuObjectX.x_origin + menuObjectX.width / 4,
					menuObjectX.y_origin + menuObjectX.height / 5.75f, 110 * scalingFactor, 110 * scalingFactor, 255, 90);
				RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[2], menuObjectX.x_origin + menuObjectX.width / 5,
					menuObjectX.y_origin + menuObjectX.height / 3.25f, 64 * scalingFactor, 64 * scalingFactor);
				sprintf_s(temp, sizeof(temp), "%.2f", turret[turretSelectedToUpgrade].mod.slow_amt);
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.95f, menuObjectX.y_origin + menuObjectX.height / 3.25f);
				break;

			case T_HOMING:
				sprintf_s(temp, sizeof(temp), "Homing");
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 18);
				RenderImageFromSpriteSheet(homing_missle_turret_spritesheet, homing_missle_turret_spritesheet_array[0],
					menuObjectX.x_origin + menuObjectX.width / 4,
					menuObjectX.y_origin + menuObjectX.height / 5.85f, 110 * scalingFactor, 110 * scalingFactor);
				RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 5,
					menuObjectX.y_origin + menuObjectX.height / 3.25f, 64 * scalingFactor, 64 * scalingFactor);
				sprintf_s(temp, sizeof(temp), "%.2f", turret[turretSelectedToUpgrade].mod.damage);
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.95f, menuObjectX.y_origin + menuObjectX.height / 3.25f);
				break;

			case T_MINE:
				sprintf_s(temp, sizeof(temp), "Mine");
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 18);
				RenderImageFromSpriteSheet(mine_spritesheet, mine_spritesheet_array[0],
					menuObjectX.x_origin + menuObjectX.width / 4,
					menuObjectX.y_origin + menuObjectX.height / 5.75f, 110 * scalingFactor, 110 * scalingFactor);
				RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 5,
					menuObjectX.y_origin + menuObjectX.height / 3.25f, 64 * scalingFactor, 64 * scalingFactor);
				sprintf_s(temp, sizeof(temp), "%.2f", turret[turretSelectedToUpgrade].mod.damage);
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.95f, menuObjectX.y_origin + menuObjectX.height / 3.25f);
				break;

			}
			//Range Icon
			RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[3], menuObjectX.x_origin + menuObjectX.width / 5,
				menuObjectX.y_origin + menuObjectX.height / 2.44f, 64 * scalingFactor, 64 * scalingFactor);
			sprintf_s(temp, sizeof(temp), "%-5.2f", turret[turretSelectedToUpgrade].mod.range);
			CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.7f, menuObjectX.y_origin + menuObjectX.height / 2.44f);

			//Attack Speed Icon
			RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[4], menuObjectX.x_origin + menuObjectX.width / 5,
				menuObjectX.y_origin + menuObjectX.height / 1.95f, 64 * scalingFactor, 64 * scalingFactor);
			sprintf_s(temp, sizeof(temp), "%.2f", turret[turretSelectedToUpgrade].mod.shoot_rate);
			CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.95f, menuObjectX.y_origin + menuObjectX.height / 1.95f);

			CP_Settings_TextSize(35.0f * scalingFactor);

			//Upgrade Price
			if (turret[turretSelectedToUpgrade].level == 10)
			{
				sprintf_s(temp, sizeof(temp), "MAX");
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 3.75f, menuObjectX.y_origin + menuObjectX.height / 1.65f);
			}

			else
			{
				if (turret[turretSelectedToUpgrade].type != T_MINE) {
					sprintf_s(temp, sizeof(temp), "%4d", turret[turretSelectedToUpgrade].upgrade_price);
					CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 5.5f, menuObjectX.y_origin + menuObjectX.height / 1.65f);
					RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 2.95f,
						menuObjectX.y_origin + menuObjectX.height / 1.65f, 40 * scalingFactor, 40 * scalingFactor);
				}

			}
			//Sell Price
			sprintf_s(temp, sizeof(temp), "%4d", turret[turretSelectedToUpgrade].sell_price);
			CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.45f, menuObjectX.y_origin + menuObjectX.height / 1.65f);
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 1.15f,
				menuObjectX.y_origin + menuObjectX.height / 1.65f, 40 * scalingFactor, 40 * scalingFactor);



		}
		break;
	case UpgradeButton:
		if (turretSelectedToUpgrade != NO_TURRET_SELECTED && turret[turretSelectedToUpgrade].type != T_MINE) { //Only render when upgrading or when mine is not selected

			RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet, interactable_UI_buttons_spritesheet_array[1],
				menuObjectX.x_origin + menuObjectX.width / 2,
				menuObjectX.y_origin + menuObjectX.height / 2, 95 * scalingFactor, 72 * scalingFactor);

		}
		break;
	case SellButton:
		if (turretSelectedToUpgrade != NO_TURRET_SELECTED) {//Only render when upgrading

			RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet, interactable_UI_buttons_spritesheet_array[0],
				menuObjectX.x_origin + menuObjectX.width / 2,
				menuObjectX.y_origin + menuObjectX.height / 2, 95 * scalingFactor, 72 * scalingFactor);
		}
		break;
	}

}

//Level
//reset data in pathfinding
void PathFindingReset(LevelData* LevelX) {
	for (int i = 0; i < level_grid_rows; i++) {
		for (int j = 0; j < level_grid_cols; j++) {
			LevelX->grid[i][j].visited = 0;
			LevelX->grid[i][j].cost = -1;
			LevelX->grid[i][j].parent_row = -1;
			LevelX->grid[i][j].parent_col = -1;
			if (LevelX->grid[i][j].type == kPath) {
				LevelX->grid[i][j].type = kClear;
			}
			if (LevelX->grid[i][j].type == kBlocked) {
				LevelX->grid[i][j].visited = 1;
			}

		}
	}
	LevelX->grid[LevelX->spawnRow][LevelX->spawnCol].cost = 0;
	LevelX->grid[LevelX->spawnRow][LevelX->spawnCol].visited = 1;
}
//update path for pathfinding
void PathFindingUpdate(LevelData* LevelX) {
	int pathRow, pathCol;
	pathRow = LevelX->grid[LevelX->exitRow][LevelX->exitCol].parent_row;
	pathCol = LevelX->grid[LevelX->exitRow][LevelX->exitCol].parent_col;
	//To check if path have no neighbors
	if (pathRow != -1 && pathCol != -1) {
		while (LevelX->grid[pathRow][pathCol].cost) {
			LevelX->grid[pathRow][pathCol].type = kPath;
			int newPathRow, newPathCol;
			newPathRow = LevelX->grid[pathRow][pathCol].parent_row;
			newPathCol = LevelX->grid[pathRow][pathCol].parent_col;
			pathRow = newPathRow;
			pathCol = newPathCol;
		}
	}
}
void RenderEnemyPath(LevelData* LevelX) {
	for (int i = 0; i < level_grid_rows; i++) {
		for (int j = 0; j < level_grid_cols; j++) {
			if (LevelX->grid[i][j].type == kPath) {
				color_game_square(i, j, COLOR_RED);
			}
			else if (LevelX->grid[i][j].type == kSpawn) {
				color_game_square(i, j, COLOR_RED);
				portal_variables_array[1].portal_x_coordinate = ((game.x_origin + game.grid_width * j) +
					(game.x_origin + game.grid_width * (j + 1))) / 2;
				portal_variables_array[1].portal_y_coordinate = ((game.y_origin + game.grid_height * i) +
					(game.y_origin + game.grid_height * (i + 1))) / 2;
				portal_variables_array[1].portal_image_width = game.grid_width;
				portal_variables_array[1].portal_image_height = game.grid_height;
			}
			else if (LevelX->grid[i][j].type == kExit) {
				color_game_square(i, j, COLOR_BLUE);
				portal_variables_array[0].portal_x_coordinate = ((game.x_origin + game.grid_width * j) +
					(game.x_origin + game.grid_width * (j + 1))) / 2;
				portal_variables_array[0].portal_y_coordinate = ((game.y_origin + game.grid_height * i) +
					(game.y_origin + game.grid_height * (i + 1))) / 2;
				portal_variables_array[0].portal_image_width = game.grid_width;
				portal_variables_array[0].portal_image_height = game.grid_height;
			}
		}
	}
}

