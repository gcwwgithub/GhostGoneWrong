#include "game.h"
#include "zhengwei.h"
#include "Gabriel.h"

#include "cprocessing.h"

//Initialize global variables
const float kLeftGameMenuXWidth = 140.714294f;
const float kRightGameMenuXOrigin = 998.571411f;
//Tools
//Reset Mouse
void MouseReset(void) {
	mouse_input.x_origin = -1;
	mouse_input.y_origin = -1;
}
//Return positive value of float
float FloatAbs(float x) {
	return x < 0 ? x * -1 : x;
}

//Check if button is pressed
Boolean BtnIsPressed(Coordinates object) {
	if (((object.x_origin <= mouse_input.x_origin)
		&& (mouse_input.x_origin <= object.x_origin + object.width))
		&& ((object.y_origin <= mouse_input.y_origin)
			&& (mouse_input.y_origin <= object.y_origin + object.height))) {
		return 1;
	}
	else {
		return 0;
	}
}
//Color game square to square_color
static void ColorGameSquare(
	int rect_row, int rect_col, CP_Color square_color)
{
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(square_color);
	CP_Graphics_DrawRect((game.x_origin + game.grid_width * rect_col),
		(game.y_origin + game.grid_height * rect_row),
		(game.grid_width), (game.grid_height));
}

//Path Finding
//Check if destination is reachable
Boolean isDestinationUpdated(void) {
	return Level.grid[Level.exit_row][Level.exit_col].visited;
}

//Update cost of neighbor base on own cost
void PathFindingUpdateNeighborCost(
	int grid_row, int grid_col, int generation) {
	//Update Row Neighbor
	for (int i = -1; i <= 1; i++) {
		if (grid_row + i >= 0 && grid_row + i < level_grid_rows) {
			if (Level.grid[grid_row + i][grid_col].visited == 0) {
				Level.grid[grid_row + i][grid_col].cost = generation + 1;
				Level.grid[grid_row + i][grid_col].parent_row = grid_row;
				Level.grid[grid_row + i][grid_col].parent_col = grid_col;
				Level.grid[grid_row + i][grid_col].visited = 1;
			}
		}
	}
	//Update Col Neighbor
	for (int i = -1; i <= 1; i++) {
		if (grid_col + i >= 0 && grid_col + i < level_grid_cols) {
			if (Level.
				grid[grid_row][grid_col + i].visited == 0) {
				Level.
					grid[grid_row][grid_col + i].cost = generation + 1;
				Level.
					grid[grid_row][grid_col + i].parent_row = grid_row;
				Level.
					grid[grid_row][grid_col + i].parent_col = grid_col;
				Level.
					grid[grid_row][grid_col + i].visited = 1;
			}
		}
	}
}

//Calculate all grid cost. Find the squares in the same generation 
//and call a function to update neighbors.
void PathFindingCalculateCost(void) {
	for (int current_cost = 0; !isDestinationUpdated()
		&& current_cost <= level_grid_rows * level_grid_cols; current_cost++) {
		for (int i = 0; i < level_grid_rows; i++) {
			for (int j = 0; j < level_grid_cols; j++) {
				if (Level.grid[i][j].cost == current_cost) {
					PathFindingUpdateNeighborCost(i, j, current_cost);
				}
			}
		}
	}
}
//init the spawn and exit cols for pathfinding
void PathFindingInit(void) {
	Level.grid[Level.spawn_row][Level.spawn_col].cost = 0;
	Level.grid[Level.spawn_row][Level.spawn_col].visited = 1;
	Level.grid[Level.spawn_row][Level.spawn_col].type = kSpawn;
	Level.grid[Level.exit_row][Level.exit_col].type = kExit;
}

//Collision Detection between circles and squares
Boolean CollisionDetection(Coordinates object1, Coordinates object2) {
	//Collision for circle to circle
	if (object1.object_type == kObjectCircle
		&& object2.object_type == kObjectCircle)
	{
		float collision_distance_squared, distance_squared,
			circle1_radius, circle2_radius;
		circle1_radius = 0.5f * object1.width;
		circle2_radius = 0.5f * object2.width;
		collision_distance_squared = (circle1_radius + circle2_radius) *
			(circle1_radius + circle2_radius);
		distance_squared = ((object1.x_origin - object2.x_origin) *
			(object1.x_origin - object2.x_origin)) +
			((object1.y_origin - object2.y_origin) *
				(object1.y_origin - object2.y_origin));
		if (collision_distance_squared >= distance_squared) {
			return kTrue;
		}
		else {
			return kFalse;
		}
	}
	//Collision for circle and square
	else if (object1.object_type == kObjectCircle
		|| object2.object_type == kObjectCircle) {
		float  distance_x, distance_y, circle_radius,
			rect_width_to_check, rect_height_to_check,
			collision_distance_x, collision_distance_y, radius_squared,
			distance_to_corner_squared_x, distance_to_corner_squared_y;
		distance_x = object1.x_origin - object2.x_origin;
		distance_x = FloatAbs(distance_x);
		distance_y = object1.y_origin - object2.y_origin;
		distance_y = FloatAbs(distance_y);
		if (object1.object_type == kObjectCircle) {
			circle_radius = 0.5f * object1.width;
			rect_width_to_check = 0.5f * object2.width;
			rect_height_to_check = 0.5f * object2.height;
		}
		else {
			circle_radius = 0.5f * object2.width;
			rect_width_to_check = 0.5f * object1.width;
			rect_height_to_check = 0.5f * object1.height;
		}
		collision_distance_x = circle_radius + rect_width_to_check;
		collision_distance_y = circle_radius + rect_height_to_check;
		radius_squared = circle_radius * circle_radius;
		distance_to_corner_squared_x =
			(distance_x - rect_width_to_check) *
			(distance_x - rect_width_to_check);
		distance_to_corner_squared_y =
			(distance_y - rect_height_to_check) *
			(distance_x - rect_height_to_check);
		//Checking for collision of Big square with circle
		if (collision_distance_x >= distance_x
			&& collision_distance_y >= distance_y) {
			if (distance_x <= rect_width_to_check) {
				return kTrue;
			}
			else if (distance_y <= rect_height_to_check) {
				return kTrue;
			}
			else if ((distance_to_corner_squared_x + distance_to_corner_squared_y) <= radius_squared) {
				return kTrue;
			}
			else {
				return kFalse;
			}
		}
		else {
			return kFalse;
		}
	}
	//Collision for square to square
	else {
		float rect1_width_to_check, rect1_height_to_check,
			rect2_width_to_check, rect2_height_to_check,
			collision_distance_x, collision_distance_y, distance_x, distance_y;
		rect1_width_to_check = 0.5f * object1.width;
		rect1_height_to_check = 0.5f * object1.height;
		rect2_width_to_check = 0.5f * object2.width;
		rect2_height_to_check = 0.5f * object2.height;
		collision_distance_x = (rect1_width_to_check + rect2_width_to_check);
		collision_distance_y = (rect1_height_to_check + rect2_height_to_check);
		distance_x = (object1.x_origin - object2.x_origin);
		distance_x = FloatAbs(distance_x);
		distance_y = (object1.y_origin - object2.y_origin);
		distance_y = FloatAbs(distance_y);
		if (collision_distance_x >= distance_x
			&& collision_distance_y >= distance_y) {
			return kTrue;
		}
		else {
			return kFalse;
		}
	}
}

//Function for doing all the stuff when clicking on game grid
void GameGridPressUpdate(LevelData* LevelX) {
	int drawX, drawY;
	drawX = (int)((mouse_input.x_origin - game.x_origin) / game.grid_width);
	drawY = (int)((mouse_input.y_origin - game.y_origin) / game.grid_height);
	Coordinates GridTemp;
	GridTemp.width = game.grid_width;
	GridTemp.height = game.grid_height;
	GridTemp.x_origin = game.x_origin + (drawX + 0.5f) * game.grid_width;
	GridTemp.y_origin = game.y_origin + (drawY + 0.5f) * game.grid_height;

	int isPositionEmpty = kTrue;
	for (int i = 0; i < kMaxTurret; i++) {
		if (turret[i].data.x_origin == GridTemp.x_origin && turret[i].data.y_origin == GridTemp.y_origin && turret[i].is_active == kTrue) {
			isPositionEmpty = kFalse;
		}
	}
	if (is_placing_turret != kTMax) {
		if (isPositionEmpty == kTrue && LevelX->grid[drawY][drawX].type != kBlocked) {
			if (is_placing_turret != kTMine) {
				LevelX->grid[drawY][drawX].type = kBlocked;
				PathFindingReset(LevelX);
				PathFindingCalculateCost();
			}
			if (!isDestinationUpdated()) {
				LevelX->grid[drawY][drawX].type = kClear;
				PathFindingReset(LevelX);
				PathFindingCalculateCost();
			}
			else {
				place_turret(is_placing_turret, drawX, drawY);
				Level.phantom_quartz -= turret_purchasing[kTPPrice][is_placing_turret];
				is_placing_turret = kTMax;
			}
			PathFindingUpdate(LevelX);
		}
		MouseReset();
	}
	else {
		for (int i = 0; i < kMaxTurret; i++) {
			if (turret[i].data.x_origin == GridTemp.x_origin && turret[i].data.y_origin == GridTemp.y_origin && turretSelectedToUpgrade == i && turret[i].is_active == kTrue && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				turretSelectedToUpgrade = kNoTurretSelected;
				MouseReset();
			}
			else if (turret[i].data.x_origin == GridTemp.x_origin && turret[i].data.y_origin == GridTemp.y_origin && turret[i].is_active == kTrue) {
				turretSelectedToUpgrade = i;
			}
		}
	}

}

//Check which button is pressed
int CheckGameButtonPressed(void) {
	for (int i = 0; i < kButtonMax; i++) {
		if (BtnIsPressed(game_menu_object[i])) {
			return i;
		}
	}
	return kButtonMax;
}

//initialize enemies
void general_level_enemies_init(int level, int wave, int basic, int fast, int fat, int grim)
{
	Level.wave_enemies[wave][kBasic] = basic;
	Level.wave_enemies[wave][kFastGhost] = fast;
	Level.wave_enemies[wave][kFatGhost] = fat;
	Level.wave_enemies[wave][kGrimReaper] = grim;
}

//function to assign environment object
void init_environment_object(int arrayIndex, int row, int col, int object_type, LevelData* LevelX)
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
	mouse_input.width = 0.0f;
	mouse_input.height = 0.0f;
	mouse_input.object_type = kObjectCircle;
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
	game_menu_object[kGameGrid].x_origin = game.x_origin;
	game_menu_object[kGameGrid].y_origin = game.y_origin;
	game_menu_object[kGameGrid].width = game.width;
	game_menu_object[kGameGrid].height = game.height;
	game_menu_object[kGameGrid].object_type = kObjectRectangle;
}

void pause_button_init(void) {
	game_menu_object[kPauseButton].x_origin = 0.0f;
	game_menu_object[kPauseButton].y_origin = 0.0f;
	game_menu_object[kPauseButton].width = kLeftGameMenuXWidth;
	game_menu_object[kPauseButton].height = (float)CP_System_GetWindowHeight() / 10;
	game_menu_object[kPauseButton].object_type = kObjectRectangle;
	game_menu_object[kPauseButton].image = pause_button_image;
}

void turret_basic_button_init(void) {

	game_menu_object[kTurretButtonBasic].x_origin = game_menu_object[kPauseButton].x_origin;
	game_menu_object[kTurretButtonBasic].y_origin = game_menu_object[kPauseButton].height;
	game_menu_object[kTurretButtonBasic].width = game_menu_object[kPauseButton].width;
	game_menu_object[kTurretButtonBasic].height = game_menu_object[kPauseButton].height * 2;
	game_menu_object[kTurretButtonBasic].object_type = kObjectRectangle;
	game_menu_object[kTurretButtonBasic].image = slow_turret_image_array[0];
}

void turret_slow_button_init(void) {
	game_menu_object[kTurretButtonSlow].x_origin = game_menu_object[kTurretButtonBasic].x_origin;
	game_menu_object[kTurretButtonSlow].y_origin = game_menu_object[kTurretButtonBasic].y_origin + game_menu_object[kTurretButtonBasic].height;
	game_menu_object[kTurretButtonSlow].width = game_menu_object[kTurretButtonBasic].width;
	game_menu_object[kTurretButtonSlow].height = game_menu_object[kTurretButtonBasic].height;
	game_menu_object[kTurretButtonSlow].object_type = kObjectRectangle;
	game_menu_object[kTurretButtonSlow].image = slow_turret_image_array[0];
}

void turret_homing_button_init(void) {
	game_menu_object[kTurretButtonHoming].x_origin = game_menu_object[kTurretButtonSlow].x_origin;
	game_menu_object[kTurretButtonHoming].y_origin = game_menu_object[kTurretButtonSlow].y_origin + game_menu_object[kTurretButtonSlow].height;
	game_menu_object[kTurretButtonHoming].width = game_menu_object[kTurretButtonSlow].width;
	game_menu_object[kTurretButtonHoming].height = game_menu_object[kTurretButtonSlow].height;
	game_menu_object[kTurretButtonHoming].object_type = kObjectRectangle;
	game_menu_object[kTurretButtonHoming].image = slow_turret_image_array[0];
}

void turret_mine_button_init(void) {
	game_menu_object[kTurretButtonMine].x_origin = game_menu_object[kTurretButtonHoming].x_origin;
	game_menu_object[kTurretButtonMine].y_origin = game_menu_object[kTurretButtonHoming].y_origin + game_menu_object[kTurretButtonHoming].height;
	game_menu_object[kTurretButtonMine].width = game_menu_object[kTurretButtonHoming].width;
	game_menu_object[kTurretButtonMine].height = game_menu_object[kTurretButtonBasic].height;
	game_menu_object[kTurretButtonMine].object_type = kObjectRectangle;
	game_menu_object[kTurretButtonMine].image = slow_turret_image_array[0];
}

void EnvironmentInit(LevelData* LevelX) {
	for (int i = 0; i < kMaxEnvironmentObject; i++) {
		Environment[i].image = NULL;
	}


	switch (current_game_level)
	{
	case 0:
		init_environment_object(0, 2, 2, 8, LevelX);
		init_environment_object(1, 3, 4, 8, LevelX);
		init_environment_object(2, 5, 1, 8, LevelX);
		break;
	case 1:
		init_environment_object(0, 0, 1, 8, LevelX);
		init_environment_object(1, 1, 4, 8, LevelX);
		init_environment_object(2, 5, 1, 8, LevelX);
		init_environment_object(3, 5, 5, 8, LevelX);
		init_environment_object(4, 2, 2, 8, LevelX);
		break;
	case 2:
		init_environment_object(0, 3, 3, 8, LevelX);
		init_environment_object(1, 3, 4, 9, LevelX);
		init_environment_object(2, 2, 1, 8, LevelX);
		init_environment_object(3, 5, 5, 8, LevelX);
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
	game_menu_object[kPhantomQuartzMenu].y_origin = 0.0f;
	game_menu_object[kPhantomQuartzMenu].x_origin = kRightGameMenuXOrigin;
	game_menu_object[kPhantomQuartzMenu].height = (float)CP_System_GetWindowHeight() / 15;
	game_menu_object[kPhantomQuartzMenu].width = ((float)CP_System_GetWindowWidth() - game_menu_object[kPhantomQuartzMenu].x_origin) / 2;
	game_menu_object[kPhantomQuartzMenu].object_type = kObjectRectangle;
}

void goldQuartz_init(void) {
	game_menu_object[kGoldQuartzMenu].width = game_menu_object[kPhantomQuartzMenu].width;
	game_menu_object[kGoldQuartzMenu].height = game_menu_object[kPhantomQuartzMenu].height;
	game_menu_object[kGoldQuartzMenu].x_origin = (float)CP_System_GetWindowWidth() - game_menu_object[kPhantomQuartzMenu].width;
	game_menu_object[kGoldQuartzMenu].y_origin = game_menu_object[kPhantomQuartzMenu].y_origin;
	game_menu_object[kGoldQuartzMenu].object_type = kObjectRectangle;
}

void health_init(void) {
	game_menu_object[kHealthMenu].x_origin = game_menu_object[kPhantomQuartzMenu].x_origin;
	game_menu_object[kHealthMenu].y_origin = game_menu_object[kPhantomQuartzMenu].y_origin + game_menu_object[kPhantomQuartzMenu].height;
	game_menu_object[kHealthMenu].width = game_menu_object[kPhantomQuartzMenu].width;
	game_menu_object[kHealthMenu].height = game_menu_object[kPhantomQuartzMenu].height;
	game_menu_object[kHealthMenu].object_type = kObjectRectangle;


	/*GameMenuObject[HealthMenu].xOrigin = GameMenuObject[GoldQuartzMenu].xOrigin;
	GameMenuObject[HealthMenu].yOrigin = GameMenuObject[GoldQuartzMenu].yOrigin + GameMenuObject[GoldQuartzMenu].height;
	GameMenuObject[HealthMenu].width = GameMenuObject[GoldQuartzMenu].width;
	GameMenuObject[HealthMenu].height = GameMenuObject[GoldQuartzMenu].height;
	GameMenuObject[HealthMenu].objectType = objectRectangle;*/
}

void menu_swap_init(void) {
	game_menu_object[kSwapButton].x_origin = game_menu_object[kTurretButtonMine].x_origin;
	game_menu_object[kSwapButton].y_origin = game_menu_object[kTurretButtonMine].y_origin + game_menu_object[kTurretButtonMine].height;
	game_menu_object[kSwapButton].width = game_menu_object[kTurretButtonMine].width;
	game_menu_object[kSwapButton].height = game_menu_object[kTurretButtonMine].height / 2;
	game_menu_object[kSwapButton].object_type = kObjectRectangle;
	game_menu_object[kSwapButton].image = CP_Image_Load("Assets/dummy");
}

void wave_number_display_init(void) {
	game_menu_object[kWaveDisplay].x_origin = game_menu_object[kGoldQuartzMenu].x_origin;
	game_menu_object[kWaveDisplay].y_origin = game_menu_object[kHealthMenu].y_origin;
	game_menu_object[kWaveDisplay].width = game_menu_object[kGoldQuartzMenu].width;
	game_menu_object[kWaveDisplay].height = game_menu_object[kGoldQuartzMenu].height;
	game_menu_object[kWaveDisplay].object_type = kObjectRectangle;
}

void battlefield_effects_display_init(void) {
	game_menu_object[kEnvironmentalEffects].x_origin = game_menu_object[kHealthMenu].x_origin;
	game_menu_object[kEnvironmentalEffects].y_origin = game_menu_object[kHealthMenu].y_origin + game_menu_object[kHealthMenu].height;
	game_menu_object[kEnvironmentalEffects].width = game_menu_object[kHealthMenu].width;
	game_menu_object[kEnvironmentalEffects].height = game_menu_object[kHealthMenu].height * 3;
	game_menu_object[kEnvironmentalEffects].object_type = kObjectRectangle;

	/*GameMenuObject[BattlefieldEffects].yOrigin = GameMenuObject[WaveDisplay].yOrigin + GameMenuObject[WaveDisplay].height;
	GameMenuObject[BattlefieldEffects].width = ((float)CP_System_GetWindowWidth() - Game.xOrigin - Game.width) / 2;
	GameMenuObject[BattlefieldEffects].height = GameMenuObject[GoldQuartzMenu].height * 2;
	GameMenuObject[BattlefieldEffects].xOrigin = (float)CP_System_GetWindowWidth() - GameMenuObject[BattlefieldEffects].width;
	GameMenuObject[BattlefieldEffects].objectType = objectRectangle;*/

}

void monster_remaining_display_init(void) {
	game_menu_object[kMonsterRemainingDisplay].x_origin = game_menu_object[kWaveDisplay].x_origin;
	game_menu_object[kMonsterRemainingDisplay].y_origin = game_menu_object[kWaveDisplay].y_origin + game_menu_object[kWaveDisplay].height;
	game_menu_object[kMonsterRemainingDisplay].width = game_menu_object[kWaveDisplay].width;
	game_menu_object[kMonsterRemainingDisplay].height = game_menu_object[kWaveDisplay].height * 3;
	game_menu_object[kMonsterRemainingDisplay].object_type = kObjectRectangle;


	/*GameMenuObject[MonsterRemainingDisplay].xOrigin = GameMenuObject[BattlefieldEffects].xOrigin;
	GameMenuObject[MonsterRemainingDisplay].yOrigin = GameMenuObject[BattlefieldEffects].yOrigin + GameMenuObject[BattlefieldEffects].height;
	GameMenuObject[MonsterRemainingDisplay].width = GameMenuObject[BattlefieldEffects].width;
	GameMenuObject[MonsterRemainingDisplay].height = GameMenuObject[BattlefieldEffects].height;
	GameMenuObject[MonsterRemainingDisplay].objectType = objectRectangle;*/
}

void upgrade_menu_init(void) {
	game_menu_object[kUpgradeMenu].x_origin = game_menu_object[kEnvironmentalEffects].x_origin;
	game_menu_object[kUpgradeMenu].y_origin = game_menu_object[kEnvironmentalEffects].y_origin + game_menu_object[kEnvironmentalEffects].height;
	game_menu_object[kUpgradeMenu].width = game_menu_object[kEnvironmentalEffects].width + game_menu_object[kMonsterRemainingDisplay].width;
	game_menu_object[kUpgradeMenu].height = (CP_System_GetWindowHeight() - game_menu_object[kEnvironmentalEffects].y_origin);
	game_menu_object[kUpgradeMenu].object_type = kObjectRectangle;
}

void upgrade_button_init(void) {
	game_menu_object[kUpgradeButton].x_origin = game_menu_object[kEnvironmentalEffects].x_origin + game_menu_object[kEnvironmentalEffects].width / 5;
	game_menu_object[kUpgradeButton].y_origin = game_menu_object[kSwapButton].y_origin - game_menu_object[kSwapButton].height / 5;
	game_menu_object[kUpgradeButton].width = game_menu_object[kUpgradeMenu].width / 3;
	game_menu_object[kUpgradeButton].height = game_menu_object[kSwapButton].height;
	game_menu_object[kUpgradeButton].object_type = kObjectRectangle;


	/*GameMenuObject[UpgradeButton].xOrigin = GameMenuObject[BattlefieldEffects].xOrigin;
	GameMenuObject[UpgradeButton].yOrigin = GameMenuObject[BattlefieldEffects].yOrigin + GameMenuObject[BattlefieldEffects].height;
	GameMenuObject[UpgradeButton].width = GameMenuObject[BattlefieldEffects].width + GameMenuObject[MonsterRemainingDisplay].width;
	GameMenuObject[UpgradeButton].height = (CP_System_GetWindowHeight() - GameMenuObject[UpgradeButton].yOrigin) / 2;
	GameMenuObject[UpgradeButton].objectType = objectRectangle;*/
}

void sell_button_init(void) {
	game_menu_object[kSellButton].x_origin = game_menu_object[kMonsterRemainingDisplay].x_origin + game_menu_object[kMonsterRemainingDisplay].width / 5;
	game_menu_object[kSellButton].y_origin = game_menu_object[kSwapButton].y_origin - game_menu_object[kSwapButton].height / 5;
	game_menu_object[kSellButton].width = game_menu_object[kUpgradeMenu].width / 3;
	game_menu_object[kSellButton].height = game_menu_object[kSwapButton].height;
	game_menu_object[kSellButton].object_type = kObjectRectangle;


	/*GameMenuObject[SellButton].xOrigin = GameMenuObject[UpgradeButton].xOrigin;
	GameMenuObject[SellButton].yOrigin = GameMenuObject[UpgradeButton].yOrigin + GameMenuObject[UpgradeButton].height;
	GameMenuObject[SellButton].width = GameMenuObject[UpgradeButton].width;
	GameMenuObject[SellButton].height = GameMenuObject[UpgradeButton].height;
	GameMenuObject[SellButton].objectType = objectRectangle;*/
}

void PowerUpPriceInit(void) {
	power_up_price.more_phantom_quartz = 10;
	power_up_price.reduce_enemy_speed = 10;
	power_up_price.reduce_enemy_health = 10;
	power_up_price.increased_mine_damage = 10;
}

void turret_details_init(enum MenuObjectType turretButton) {
	game_menu_object[kTurretDetailsDisplay].x_origin = game_menu_object[turretButton].x_origin + game_menu_object[turretButton].width;
	game_menu_object[kTurretDetailsDisplay].y_origin = game_menu_object[turretButton].y_origin + game_menu_object[turretButton].width / 4;
	game_menu_object[kTurretDetailsDisplay].width = game_menu_object[turretButton].width * 1.25f;
	game_menu_object[kTurretDetailsDisplay].height = game_menu_object[turretButton].height / 2;
	game_menu_object[kTurretDetailsDisplay].object_type = kObjectRectangle;
}

void RenderTurretDetailsDisplay(void) {
	float tempMouseX = mouse_input.x_origin, tempMouseY = mouse_input.y_origin;
	mouse_input.x_origin = CP_Input_GetMouseX();
	mouse_input.y_origin = CP_Input_GetMouseY();
	turret_details_init(CheckGameButtonPressed());
	switch (CheckGameButtonPressed()) {
	case kTurretButtonBasic:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(game_menu_object[kTurretDetailsDisplay].x_origin, game_menu_object[kTurretDetailsDisplay].y_origin, game_menu_object[kTurretDetailsDisplay].width * 1.25f, game_menu_object[kTurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scaling_factor);
		if (power_up_menu == kFalse)
		{
			CP_Font_DrawText("Basic Turret", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.55f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("Single Target Damage", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.55f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}

		else
		{
			CP_Font_DrawText("Gem Upgrade", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.57f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("More Phantom Quartz Earned", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.57f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}

		break;
	case kTurretButtonSlow:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(game_menu_object[kTurretDetailsDisplay].x_origin, game_menu_object[kTurretDetailsDisplay].y_origin, game_menu_object[kTurretDetailsDisplay].width * 1.15f, game_menu_object[kTurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scaling_factor);
		if (power_up_menu == kFalse)
		{
			CP_Font_DrawText("Slow Turret", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("Slow Enemies", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}

		else
		{
			CP_Font_DrawText("Speed Upgrade", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("All ghosts move slower", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}

		
		break;
	case kTurretButtonHoming:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(game_menu_object[kTurretDetailsDisplay].x_origin, game_menu_object[kTurretDetailsDisplay].y_origin, game_menu_object[kTurretDetailsDisplay].width * 1.15f, game_menu_object[kTurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scaling_factor);
		if (power_up_menu == kFalse)
		{
			CP_Font_DrawText("Homing Turret", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("Splash Damage", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}

		else
		{
			CP_Font_DrawText("Health Upgrade", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("All ghosts have less HP", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}

		
		break;
	case kTurretButtonMine:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(game_menu_object[kTurretDetailsDisplay].x_origin, game_menu_object[kTurretDetailsDisplay].y_origin, game_menu_object[kTurretDetailsDisplay].width * 1.15f, game_menu_object[kTurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scaling_factor);
		if (power_up_menu == kFalse)
		{
			CP_Font_DrawText("Mine", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("Single Use", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}

		else
		{
			CP_Font_DrawText("Mine Upgrade", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("Mines deal more damage", game_menu_object[kTurretDetailsDisplay].x_origin + game_menu_object[kTurretDetailsDisplay].width / 1.75f, (game_menu_object[kTurretDetailsDisplay].y_origin + game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}

		
		break;
	}
	mouse_input.x_origin = tempMouseX;
	mouse_input.y_origin = tempMouseY;
}

void Level1Init(void) {
	level_grid_cols = kLevel1Cols;
	level_grid_rows = kLevel1Rows;
	current_game_level = 0;
	Level.grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));// using size of pointers so that lower bits operating system do not require so much malloc
	for (int i = 0; i < level_grid_rows; i++) {
		if (Level.grid != NULL) {
			Level.grid[i] = (struct Grids*)calloc(level_grid_cols, sizeof(struct Grids));
			if (Level.grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}



	Level.spawn_row = 0;
	Level.spawn_col = 0;
	Level.exit_row = level_grid_rows - 1;
	Level.exit_col = (level_grid_cols - 1);
	Level.health = 100;
	Level.phantom_quartz = 50000;
	Level.gold_quartz = 0;
	Level.current_wave = 0;
	Level.current_effect = kNoEnvironmentalEffects;
	Level.current_power_up_level.more_phantom_quartz = 0;
	Level.current_power_up_level.reduce_enemy_speed = 0;
	Level.current_power_up_level.reduce_enemy_health = 0;
	Level.current_power_up_level.increased_mine_damage = 0;

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
	is_placing_turret = kTMax;
	turretSelectedToUpgrade = kNoTurretSelected;
	power_up_menu = kFalse;
	PathFindingInit();
	EnvironmentInit(&Level);

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

	PathFindingReset(&Level);
	PathFindingCalculateCost();
	PathFindingUpdate(&Level);

	SetBuildingTime(kFullBuildingPhaseTime);
	current_game_state = kBuilding;
	//BGM Sound
	CP_Sound_StopGroup(CP_SOUND_GROUP_1);
	CP_Sound_PlayAdvanced(BuildingBGM, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
}

void Level2Init(void) {
	level_grid_cols = kLevel2Cols;
	level_grid_rows = kLevel2Rows;
	current_game_level = 1;
	Level.grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));// using size of pointers so that lower bits operating system do not require so much malloc
	for (int i = 0; i < level_grid_rows; i++) {
		if (Level.grid != NULL) {
			Level.grid[i] = (struct Grids*)calloc(level_grid_cols, sizeof(struct Grids));
			if (Level.grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	Level.spawn_row = 0;
	Level.spawn_col = 0;
	Level.exit_row = level_grid_rows - 1;
	Level.exit_col = (level_grid_cols - 1);
	Level.health = 100;
	Level.phantom_quartz = 300;
	Level.gold_quartz = 0;
	Level.current_wave = 0;
	Level.current_effect = kNoEnvironmentalEffects;
	Level.current_power_up_level.more_phantom_quartz = 0;
	Level.current_power_up_level.reduce_enemy_speed = 0;
	Level.current_power_up_level.reduce_enemy_health = 0;
	Level.current_power_up_level.increased_mine_damage = 0;


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
	is_placing_turret = kTMax;
	turretSelectedToUpgrade = kNoTurretSelected;
	power_up_menu = kFalse;
	PathFindingInit();
	EnvironmentInit(&Level);

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

	PathFindingReset(&Level);
	PathFindingCalculateCost();
	PathFindingUpdate(&Level);

	SetBuildingTime(kFullBuildingPhaseTime);
	current_game_state = kBuilding;
	//BGM Sound
	CP_Sound_StopGroup(CP_SOUND_GROUP_1);
	CP_Sound_PlayAdvanced(BuildingBGM, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
}

void Level3Init(void) {
	level_grid_cols = kLevel3Cols;
	level_grid_rows = kLevel3Rows;
	current_game_level = 2;
	Level.grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));// using size of pointers so that lower bits operating system do not require so much malloc
	for (int i = 0; i < level_grid_rows; i++) {
		if (Level.grid != NULL) {
			Level.grid[i] = (struct Grids*)calloc(level_grid_cols, sizeof(struct Grids));
			if (Level.grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	Level.spawn_row = level_grid_rows - 1;
	Level.spawn_col = (level_grid_cols - 1);
	Level.exit_row = 0;
	Level.exit_col = 0;
	Level.health = 100;
	Level.phantom_quartz = 300;
	Level.gold_quartz = 0;
	Level.current_wave = 0;
	Level.current_effect = kNoEnvironmentalEffects;
	Level.current_power_up_level.more_phantom_quartz = 0;
	Level.current_power_up_level.reduce_enemy_speed = 0;
	Level.current_power_up_level.reduce_enemy_health = 0;
	Level.current_power_up_level.increased_mine_damage = 0;

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
	is_placing_turret = kTMax;
	turretSelectedToUpgrade = kNoTurretSelected;
	power_up_menu = kFalse;
	PathFindingInit();
	EnvironmentInit(&Level);

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

	PathFindingReset(&Level);
	PathFindingCalculateCost();
	PathFindingUpdate(&Level);

	SetBuildingTime(kFullBuildingPhaseTime);
	current_game_state = kBuilding;

	//BGM Sound
	CP_Sound_StopGroup(CP_SOUND_GROUP_1);
	CP_Sound_PlayAdvanced(BuildingBGM, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
}

void Level4Init(void) {
	level_grid_cols = kLevel4Cols;
	level_grid_rows = kLevel4Rows;
	current_game_level = 3;
	Level.grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));// using size of pointers so that lower bits operating system do not require so much malloc
	for (int i = 0; i < level_grid_rows; i++) {
		if (Level.grid != NULL) {
			Level.grid[i] = (struct Grids*)calloc(level_grid_cols, sizeof(struct Grids));
			if (Level.grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	Level.spawn_row = 0;
	Level.spawn_col = 0;
	Level.exit_row = level_grid_rows - 1;
	Level.exit_col = 0;
	Level.health = 100;
	Level.phantom_quartz = 300;
	Level.gold_quartz = 0;
	Level.current_wave = 0;
	Level.current_effect = kNoEnvironmentalEffects;
	Level.current_power_up_level.more_phantom_quartz = 0;
	Level.current_power_up_level.reduce_enemy_speed = 0;
	Level.current_power_up_level.reduce_enemy_health = 0;
	Level.current_power_up_level.increased_mine_damage = 0;


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
	is_placing_turret = kTMax;
	turretSelectedToUpgrade = kNoTurretSelected;
	power_up_menu = kFalse;
	PathFindingInit();
	EnvironmentInit(&Level);

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

	PathFindingReset(&Level);
	PathFindingCalculateCost();
	PathFindingUpdate(&Level);

	SetBuildingTime(kFullBuildingPhaseTime);
	current_game_state = kBuilding;
	//BGM Sound
	CP_Sound_StopGroup(CP_SOUND_GROUP_1);
	CP_Sound_PlayAdvanced(BuildingBGM, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
}

void Level5Init(void) {
	level_grid_cols = kLevel5Cols;
	level_grid_rows = kLevel5Rows;
	current_game_level = 4;
	Level.grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));// using size of pointers so that lower bits operating system do not require so much malloc
	for (int i = 0; i < level_grid_rows; i++) {
		if (Level.grid != NULL) {
			Level.grid[i] = (struct Grids*)calloc(level_grid_cols, sizeof(struct Grids));
			if (Level.grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	Level.spawn_row = 0;
	Level.spawn_col = (level_grid_cols - 1) / 2;
	Level.exit_row = level_grid_rows - 1;
	Level.exit_col = (level_grid_cols - 1) / 2;
	Level.health = 100;
	Level.phantom_quartz = 300;
	Level.gold_quartz = 0;
	Level.current_wave = 0;
	Level.current_effect = kNoEnvironmentalEffects;
	Level.current_power_up_level.more_phantom_quartz = 0;
	Level.current_power_up_level.reduce_enemy_speed = 0;
	Level.current_power_up_level.reduce_enemy_health = 0;
	Level.current_power_up_level.increased_mine_damage = 0;

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

	is_placing_turret = kTMax;
	turretSelectedToUpgrade = kNoTurretSelected;
	power_up_menu = kFalse;
	PathFindingInit();
	EnvironmentInit(&Level);
	turret_init();
	Enemies_init();

	PathFindingReset(&Level);
	PathFindingCalculateCost();
	PathFindingUpdate(&Level);

	SetBuildingTime(kFullBuildingPhaseTime);
	current_game_state = kBuilding;
	//BGM Sound
	CP_Sound_StopGroup(CP_SOUND_GROUP_1);
	CP_Sound_PlayAdvanced(BuildingBGM, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
}

void ButtonPressedUpdate(void) {
	switch (CheckGameButtonPressed())
	{
	case kGameGrid:
		GameGridPressUpdate(&Level);
		break;
	case kPauseButton:
		current_game_state = current_game_state == kPause ? kWave : kPause;
		MouseReset();
		//Button SFX
		CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
		break;

	case kTurretButtonBasic:
		if (turret_purchasing[kTPPrice][kTBasic] <= Level.phantom_quartz && power_up_menu == kFalse) { // Currently hardcoded 
			is_placing_turret = kTBasic;
			turretSelectedToUpgrade = kNoTurretSelected;
			RenderImageFromSpriteSheet(basic_turret_spritesheet, basic_turret_spritesheet_array[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), game.grid_width, game.grid_height);
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
			}
		}
		else if (power_up_menu == kTrue && Level.gold_quartz >= power_up_price.more_phantom_quartz) {
			Level.current_power_up_level.more_phantom_quartz += 1;
			Level.gold_quartz -= power_up_price.more_phantom_quartz;
			MouseReset();
			is_placing_turret = kTMax;
			turretSelectedToUpgrade = kNoTurretSelected;
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
			}
		}
		else {
			is_placing_turret = kTMax;
			turretSelectedToUpgrade = kNoTurretSelected;
		}
		break;

	case kTurretButtonSlow:
		if (turret_purchasing[kTPPrice][kTSlow] <= Level.phantom_quartz && power_up_menu == kFalse) {
			is_placing_turret = kTSlow;
			turretSelectedToUpgrade = kNoTurretSelected;
			CP_Image_DrawAdvanced(game_menu_object[CheckGameButtonPressed()].image, CP_Input_GetMouseX(), CP_Input_GetMouseY(), game.grid_width, game.grid_height, 255, 0);
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
			}
		}
		else if (power_up_menu == kTrue && Level.gold_quartz >= power_up_price.reduce_enemy_speed) {
			Level.current_power_up_level.reduce_enemy_speed += 1;
			Level.gold_quartz -= power_up_price.reduce_enemy_speed;
			MouseReset();
			is_placing_turret = kTMax;
			turretSelectedToUpgrade = kNoTurretSelected;
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
			}
		}
		else {
			is_placing_turret = kTMax;
			turretSelectedToUpgrade = kNoTurretSelected;
		}
		break;

	case kTurretButtonHoming:
		if (turret_purchasing[kTPPrice][kTHoming] <= Level.phantom_quartz && power_up_menu == kFalse) {
			is_placing_turret = kTHoming;
			turretSelectedToUpgrade = kNoTurretSelected;
			RenderImageFromSpriteSheet(homing_missle_turret_spritesheet, homing_missle_turret_spritesheet_array[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), game.grid_width, game.grid_height);
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
			}
		}
		else if (power_up_menu == kTrue && Level.gold_quartz >= power_up_price.reduce_enemy_health) {
			Level.current_power_up_level.reduce_enemy_health += 1;
			Level.gold_quartz -= power_up_price.reduce_enemy_health;
			MouseReset();
			is_placing_turret = kTMax;
			turretSelectedToUpgrade = kNoTurretSelected;
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
			}
		}
		else {
			is_placing_turret = kTMax;
			turretSelectedToUpgrade = kNoTurretSelected;
		}
		break;
	case kTurretButtonMine:
		if (turret_purchasing[kTPPrice][kTMine] <= Level.phantom_quartz && power_up_menu == kFalse) {
			is_placing_turret = kTMine;
			turretSelectedToUpgrade = kNoTurretSelected;
			RenderImageFromSpriteSheet(mine_spritesheet, mine_spritesheet_array[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), game.grid_width, game.grid_height);
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
			}
		}
		else if (power_up_menu == kTrue && Level.gold_quartz >= power_up_price.increased_mine_damage) {
			Level.current_power_up_level.increased_mine_damage += 1;
			Level.gold_quartz -= power_up_price.increased_mine_damage;
			MouseReset();
			is_placing_turret = kTMax;
			turretSelectedToUpgrade = kNoTurretSelected;
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
			}
		}
		else {
			is_placing_turret = kTMax;
			turretSelectedToUpgrade = kNoTurretSelected;
		}
		break;

	case kSwapButton:
		is_placing_turret = kTMax;
		turretSelectedToUpgrade = kNoTurretSelected;
		power_up_menu = !power_up_menu;
		MouseReset();
		//Button SFX
		CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
		break;

	case kGoldQuartzMenu:
		is_placing_turret = kTMax;
		turretSelectedToUpgrade = kNoTurretSelected;
		if (Level.phantom_quartz >= 1000) {
			Level.phantom_quartz -= 1000;
			Level.gold_quartz += 10;
			//Button SFX 
			CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
		}
		MouseReset();
		break;

	case kUpgradeMenu:
		is_placing_turret = kTMax;
		break;

	case kUpgradeButton:
		if (turretSelectedToUpgrade != kNoTurretSelected) {
			if (turret[turretSelectedToUpgrade].type != kTMine) {
				if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) && Level.phantom_quartz >= turret[turretSelectedToUpgrade].upgrade_price) {
					if (turret[turretSelectedToUpgrade].level < 10)
					{
						Level.phantom_quartz -= turret[turretSelectedToUpgrade].upgrade_price;
						upgrade_turret(turretSelectedToUpgrade);
						//Button SFX
						if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
							CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
						}
					}

					is_placing_turret = kTMax;
					//call upgrade function
				}
			}
			else {
				is_placing_turret = kTMax;
			}
		}
		else {
			is_placing_turret = kTMax;
			turretSelectedToUpgrade = kNoTurretSelected;
		}
		break;

	case kSellButton:
		if (turretSelectedToUpgrade != kNoTurretSelected) {
			int drawX, drawY;
			drawX = (int)((turret[turretSelectedToUpgrade].data.x_origin - game.x_origin) / game.grid_width);
			drawY = (int)((turret[turretSelectedToUpgrade].data.y_origin - game.y_origin) / game.grid_height);
			sell_turret(turretSelectedToUpgrade);
			Level.grid[drawY][drawX].type = kClear;
			PathFindingReset(&Level);
			PathFindingCalculateCost();
			PathFindingUpdate(&Level);
			MouseReset();
			is_placing_turret = kTMax;
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
			}
		}
		else {
			is_placing_turret = kTMax;
			turretSelectedToUpgrade = kNoTurretSelected;
		}
		break;

	default:
		is_placing_turret = kTMax;
		turretSelectedToUpgrade = kNoTurretSelected;
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
	if (turretSelectedToUpgrade == kNoTurretSelected && (type == kUpgradeButton || type == kSellButton || type == kUpgradeMenu)) {// only render upgrade and sell button when turret selected
	//empty by design
	}
	else if (turret[turretSelectedToUpgrade].type == kTMine && type == kUpgradeButton) { //Hide the upgrade menu for mine
		//empty by design
	}
	else {
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(menuObjectX.x_origin, menuObjectX.y_origin, menuObjectX.width, menuObjectX.height);
	}
	CP_Settings_TextSize(35.0f * scaling_factor);
	switch (type)
	{

	case kTurretButtonBasic:
		CP_Image_Draw(turret_button_background, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 2),
			138 * scaling_factor, 144 * scaling_factor, 255);
		if (power_up_menu == kFalse) {
			RenderImageFromSpriteSheet(basic_turret_spritesheet, basic_turret_spritesheet_array[0], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 128 * scaling_factor, 128 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "%d", turret_purchasing[kTPPrice][kTBasic]);
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.width / 1.45f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scaling_factor, 45 * scaling_factor);
		}

		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet, power_up_spritesheet_array[0], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 100 * scaling_factor, 100 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "Lv:%-2d", Level.current_power_up_level.more_phantom_quartz);
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 7));

			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[0], menuObjectX.width / 1.65f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scaling_factor, 45 * scaling_factor);
		}
		break;

	case kTurretButtonSlow:
		CP_Image_Draw(turret_button_background, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 2),
			138 * scaling_factor, 144 * scaling_factor, 255);
		if (power_up_menu == kFalse) {
			CP_Image_DrawAdvanced(menuObjectX.image, menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 128 * scaling_factor,
				128 * scaling_factor, 255, 90);
			sprintf_s(temp, sizeof(temp), "%d", turret_purchasing[kTPPrice][kTSlow]);
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.width / 1.45f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scaling_factor, 45 * scaling_factor);
		}
		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet, power_up_spritesheet_array[1], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 100 * scaling_factor, 100 * scaling_factor);


			sprintf_s(temp, sizeof(temp), "Lv:%-2d", Level.current_power_up_level.reduce_enemy_speed);
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 7));

			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[0], menuObjectX.width / 1.65f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scaling_factor, 45 * scaling_factor);
		}
		break;

	case  kTurretButtonHoming:
		CP_Image_Draw(turret_button_background, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 2),
			138 * scaling_factor, 144 * scaling_factor, 255);
		if (power_up_menu == kFalse) {
			RenderImageFromSpriteSheet(homing_missle_turret_spritesheet, homing_missle_turret_spritesheet_array[0], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 128 * scaling_factor, 128 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "%d", turret_purchasing[kTPPrice][kTHoming]);
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.width / 1.45f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scaling_factor, 45 * scaling_factor);
		}
		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet, power_up_spritesheet_array[2], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 100 * scaling_factor, 100 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "Lv:%-2d", Level.current_power_up_level.reduce_enemy_health);
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 7));

			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[0], menuObjectX.width / 1.65f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scaling_factor, 45 * scaling_factor);

		}
		break;

	case kTurretButtonMine:
		CP_Image_Draw(turret_button_background, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 2),
			138 * scaling_factor, 144 * scaling_factor, 255);
		if (power_up_menu == kFalse) {
			RenderImageFromSpriteSheet(mine_spritesheet, mine_spritesheet_array[0], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 128 * scaling_factor, 128 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "%d", turret_purchasing[kTPPrice][kTMine]);
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.width / 1.45f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scaling_factor, 45 * scaling_factor);
		}
		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet, power_up_spritesheet_array[3], menuObjectX.width / 2,
				(menuObjectX.y_origin + menuObjectX.height / 2), 100 * scaling_factor, 100 * scaling_factor);

			sprintf_s(temp, sizeof(temp), "Lv:%-2d", Level.current_power_up_level.increased_mine_damage);
			CP_Font_DrawText(temp, menuObjectX.width / 2, (menuObjectX.y_origin + menuObjectX.height / 7));


			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menuObjectX.width / 2.5f, (menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[0], menuObjectX.width / 1.65f,
				(menuObjectX.y_origin + menuObjectX.height / 2 + 128 / 3), 45 * scaling_factor, 45 * scaling_factor);
		}

		break;

	case kPauseButton:
		CP_Image_DrawAdvanced(menuObjectX.image, menuObjectX.width / 2,
			(menuObjectX.y_origin + menuObjectX.height / 2), 64,
			64, 255, 0);
		break;
	case kSwapButton:
		RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet, interactable_UI_buttons_spritesheet_array[2],
			menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 137, 72);
		break;
	case kGoldQuartzMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scaling_factor);
		CP_Image_Draw(thin_UI_background, menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scaling_factor, 46 * scaling_factor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[0], menuObjectX.x_origin + menuObjectX.width / 5.5f,
			menuObjectX.y_origin + menuObjectX.height / 2, 30 * scaling_factor, 30 * scaling_factor);
		RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[4], menuObjectX.x_origin + menuObjectX.width / 1.3f,
			menuObjectX.y_origin + menuObjectX.height / 2, 28 * scaling_factor, 28 * scaling_factor);
		sprintf_s(temp, 100, "x%-10d", Level.gold_quartz);
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.55f, menuObjectX.y_origin + menuObjectX.height / 2);
		break;
	case kPhantomQuartzMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scaling_factor);
		CP_Image_Draw(thin_UI_background, menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scaling_factor, 46 * scaling_factor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 5,
			menuObjectX.y_origin + menuObjectX.height / 2, 30 * scaling_factor, 30 * scaling_factor);
		sprintf_s(temp, 100, "x%-10d", Level.phantom_quartz);
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.45f, menuObjectX.y_origin + menuObjectX.height / 2);
		break;
	case kHealthMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scaling_factor);
		sprintf_s(temp, 100, "x%-10d", Level.health);
		CP_Image_Draw(thin_UI_background, menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scaling_factor, 46 * scaling_factor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[2], menuObjectX.x_origin + menuObjectX.width / 5,
			menuObjectX.y_origin + menuObjectX.height / 2, 30 * scaling_factor, 30 * scaling_factor);
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.45f, menuObjectX.y_origin + menuObjectX.height / 2);
		break;
	case kWaveDisplay:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scaling_factor);
		sprintf_s(temp, 100, "%2d/%d", Level.current_wave + 1, kMaxNumberOfWave);
		CP_Image_Draw(thin_UI_background, menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scaling_factor, 46 * scaling_factor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[3], menuObjectX.x_origin + menuObjectX.width / 5,
			menuObjectX.y_origin + menuObjectX.height / 2, 30 * scaling_factor, 30 * scaling_factor);
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2.5f, menuObjectX.y_origin + menuObjectX.height / 2);
		break;
	case kEnvironmentalEffects:
		RenderImageFromSpriteSheet(fat_UI_background_spritesheet, fat_UI_background_spritesheet_array[0], menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scaling_factor, 142 * scaling_factor);
		CP_Settings_TextSize(35.0f * scaling_factor);
		CP_Settings_Fill(COLOR_WHITE);
		sprintf_s(temp, sizeof(temp), "Effects");
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 5);
		printf("%d", Level.current_effect);
		RenderImageFromSpriteSheet(battlefield_effect_spritesheet, battlefield_effect_spritesheet_array[Level.current_effect],
			menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 1.65f,
			85 * scaling_factor, 85 * scaling_factor);
		break;
	case kMonsterRemainingDisplay:
		RenderImageFromSpriteSheet(fat_UI_background_spritesheet, fat_UI_background_spritesheet_array[0], menuObjectX.x_origin + menuObjectX.width / 2,
			menuObjectX.y_origin + menuObjectX.height / 2, 138 * scaling_factor, 142 * scaling_factor);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(35.0f * scaling_factor);
		sprintf_s(temp, sizeof(temp), "Enemies");
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 5);
		int totalEnemies = 0;
		for (int i = 0; i < kMaxEnemyType; i++) {
			if (current_game_state == kWave) {
				totalEnemies += Level.wave_enemies[Level.current_wave][i];
			}
			else if (current_game_state == kBuilding) { // Forecast for next wave instead of current empty wave
				totalEnemies += Level.wave_enemies[Level.current_wave][i];
			}
		}
		sprintf_s(temp, sizeof(temp), "%d/%d", enemies_left, totalEnemies);
		CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 2);
		break;
	case kUpgradeMenu:
		if (turretSelectedToUpgrade != kNoTurretSelected) { //Only render when upgrading

			CP_Image_Draw(upgrade_menu_background, menuObjectX.x_origin + menuObjectX.width / 2,
				menuObjectX.y_origin + menuObjectX.height / 2.6f, 275 * scaling_factor, 480 * scaling_factor, 255);

			CP_Settings_Fill(COLOR_WHITE);
			CP_Settings_TextSize(50.0f * scaling_factor);


			//Level Icon
			RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[0], menuObjectX.x_origin + menuObjectX.width / 1.75f,
				menuObjectX.y_origin + menuObjectX.height / 5.75f, 64 * scaling_factor, 64 * scaling_factor);
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
			case kTBasic:
				sprintf_s(temp, sizeof(temp), "Basic");
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 18);
				RenderImageFromSpriteSheet(basic_turret_spritesheet, basic_turret_spritesheet_array[0],
					menuObjectX.x_origin + menuObjectX.width / 4,
					menuObjectX.y_origin + menuObjectX.height / 5.75f, 110 * scaling_factor, 110 * scaling_factor);
				RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 5,
					menuObjectX.y_origin + menuObjectX.height / 3.25f, 64 * scaling_factor, 64 * scaling_factor);
				sprintf_s(temp, sizeof(temp), "%.2f", turret[turretSelectedToUpgrade].mod.damage);
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.95f, menuObjectX.y_origin + menuObjectX.height / 3.25f);
				break;

			case kTSlow:
				sprintf_s(temp, sizeof(temp), "Slow");
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 18);
				CP_Image_DrawAdvanced(slow_turret_image_array[0], menuObjectX.x_origin + menuObjectX.width / 4,
					menuObjectX.y_origin + menuObjectX.height / 5.75f, 110 * scaling_factor, 110 * scaling_factor, 255, 90);
				RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[2], menuObjectX.x_origin + menuObjectX.width / 5,
					menuObjectX.y_origin + menuObjectX.height / 3.25f, 64 * scaling_factor, 64 * scaling_factor);
				sprintf_s(temp, sizeof(temp), "%.2f", turret[turretSelectedToUpgrade].mod.slow_amt);
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.95f, menuObjectX.y_origin + menuObjectX.height / 3.25f);
				break;

			case kTHoming:
				sprintf_s(temp, sizeof(temp), "Homing");
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 18);
				RenderImageFromSpriteSheet(homing_missle_turret_spritesheet, homing_missle_turret_spritesheet_array[0],
					menuObjectX.x_origin + menuObjectX.width / 4,
					menuObjectX.y_origin + menuObjectX.height / 5.85f, 110 * scaling_factor, 110 * scaling_factor);
				RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 5,
					menuObjectX.y_origin + menuObjectX.height / 3.25f, 64 * scaling_factor, 64 * scaling_factor);
				sprintf_s(temp, sizeof(temp), "%.2f", turret[turretSelectedToUpgrade].mod.damage);
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.95f, menuObjectX.y_origin + menuObjectX.height / 3.25f);
				break;

			case kTMine:
				sprintf_s(temp, sizeof(temp), "Mine");
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 2, menuObjectX.y_origin + menuObjectX.height / 18);
				RenderImageFromSpriteSheet(mine_spritesheet, mine_spritesheet_array[0],
					menuObjectX.x_origin + menuObjectX.width / 4,
					menuObjectX.y_origin + menuObjectX.height / 5.75f, 110 * scaling_factor, 110 * scaling_factor);
				RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 5,
					menuObjectX.y_origin + menuObjectX.height / 3.25f, 64 * scaling_factor, 64 * scaling_factor);
				sprintf_s(temp, sizeof(temp), "%.2f", turret[turretSelectedToUpgrade].mod.damage);
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.95f, menuObjectX.y_origin + menuObjectX.height / 3.25f);
				break;

			}
			//Range Icon
			RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[3], menuObjectX.x_origin + menuObjectX.width / 5,
				menuObjectX.y_origin + menuObjectX.height / 2.44f, 64 * scaling_factor, 64 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "%-5.2f", turret[turretSelectedToUpgrade].mod.range);
			CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.7f, menuObjectX.y_origin + menuObjectX.height / 2.44f);

			//Attack Speed Icon
			RenderImageFromSpriteSheet(turret_stats_spritesheet, turret_stats_spritesheet_array[4], menuObjectX.x_origin + menuObjectX.width / 5,
				menuObjectX.y_origin + menuObjectX.height / 1.95f, 64 * scaling_factor, 64 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "%.2f", turret[turretSelectedToUpgrade].mod.shoot_rate);
			CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.95f, menuObjectX.y_origin + menuObjectX.height / 1.95f);

			CP_Settings_TextSize(35.0f * scaling_factor);

			//Upgrade Price
			if (turret[turretSelectedToUpgrade].level == 10)
			{
				sprintf_s(temp, sizeof(temp), "MAX");
				CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 3.75f, menuObjectX.y_origin + menuObjectX.height / 1.65f);
			}

			else
			{
				if (turret[turretSelectedToUpgrade].type != kTMine) {
					sprintf_s(temp, sizeof(temp), "%4d", turret[turretSelectedToUpgrade].upgrade_price);
					CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 5.5f, menuObjectX.y_origin + menuObjectX.height / 1.65f);
					RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 2.95f,
						menuObjectX.y_origin + menuObjectX.height / 1.65f, 40 * scaling_factor, 40 * scaling_factor);
				}

			}
			//Sell Price
			sprintf_s(temp, sizeof(temp), "%4d", turret[turretSelectedToUpgrade].sell_price);
			CP_Font_DrawText(temp, menuObjectX.x_origin + menuObjectX.width / 1.45f, menuObjectX.y_origin + menuObjectX.height / 1.65f);
			RenderImageFromSpriteSheet(currency_spritesheet, currency_spritesheet_array[1], menuObjectX.x_origin + menuObjectX.width / 1.15f,
				menuObjectX.y_origin + menuObjectX.height / 1.65f, 40 * scaling_factor, 40 * scaling_factor);



		}
		break;
	case kUpgradeButton:
		if (turretSelectedToUpgrade != kNoTurretSelected && turret[turretSelectedToUpgrade].type != kTMine) { //Only render when upgrading or when mine is not selected

			RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet, interactable_UI_buttons_spritesheet_array[1],
				menuObjectX.x_origin + menuObjectX.width / 2,
				menuObjectX.y_origin + menuObjectX.height / 2, 95 * scaling_factor, 72 * scaling_factor);

		}
		break;
	case kSellButton:
		if (turretSelectedToUpgrade != kNoTurretSelected) {//Only render when upgrading

			RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet, interactable_UI_buttons_spritesheet_array[0],
				menuObjectX.x_origin + menuObjectX.width / 2,
				menuObjectX.y_origin + menuObjectX.height / 2, 95 * scaling_factor, 72 * scaling_factor);
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
	LevelX->grid[LevelX->spawn_row][LevelX->spawn_col].cost = 0;
	LevelX->grid[LevelX->spawn_row][LevelX->spawn_col].visited = 1;
}
//update path for pathfinding
void PathFindingUpdate(LevelData* LevelX) {
	int pathRow, pathCol;
	pathRow = LevelX->grid[LevelX->exit_row][LevelX->exit_col].parent_row;
	pathCol = LevelX->grid[LevelX->exit_row][LevelX->exit_col].parent_col;
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
				ColorGameSquare(i, j, COLOR_RED);
			}
			else if (LevelX->grid[i][j].type == kSpawn) {
				ColorGameSquare(i, j, COLOR_RED);
				portal_variables_array[1].portal_x_coordinate = ((game.x_origin + game.grid_width * j) +
					(game.x_origin + game.grid_width * (j + 1))) / 2;
				portal_variables_array[1].portal_y_coordinate = ((game.y_origin + game.grid_height * i) +
					(game.y_origin + game.grid_height * (i + 1))) / 2;
				portal_variables_array[1].portal_image_width = game.grid_width;
				portal_variables_array[1].portal_image_height = game.grid_height;
			}
			else if (LevelX->grid[i][j].type == kExit) {
				ColorGameSquare(i, j, COLOR_BLUE);
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
void HowToPlayButtonsInit(void) {
	TutorialButtons[kHowToPlayBack].x_origin = (float)CP_System_GetWindowWidth() / 50;
	TutorialButtons[kHowToPlayBack].y_origin = (float)CP_System_GetWindowHeight() / 50;
	TutorialButtons[kHowToPlayBack].width = (float)CP_System_GetWindowWidth() / 10;
	TutorialButtons[kHowToPlayBack].height = (float)CP_System_GetWindowHeight() / 10;

	TutorialButtons[kHowToPlayPrevious].x_origin = (float)CP_System_GetWindowWidth()/50;
	TutorialButtons[kHowToPlayPrevious].y_origin = (float)CP_System_GetWindowHeight() / 50;
	TutorialButtons[kHowToPlayPrevious].width = (float)CP_System_GetWindowWidth() / 10;
	TutorialButtons[kHowToPlayPrevious].height = (float)CP_System_GetWindowHeight() / 10;

	TutorialButtons[kHowToPlayNext].x_origin = (float)CP_System_GetWindowWidth()/1.12f;
	TutorialButtons[kHowToPlayNext].y_origin = (float)CP_System_GetWindowHeight()/50;
	TutorialButtons[kHowToPlayNext].width = (float)CP_System_GetWindowWidth() / 10;
	TutorialButtons[kHowToPlayNext].height = (float)CP_System_GetWindowHeight() / 10;
}
void RenderHowToPlayPages(void) {
	CP_Graphics_ClearBackground(COLOR_WHITE);
	
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Image_Draw(tutorial_image_array[current_how_to_play_page], CP_System_GetWindowWidth() * 0.5f,
		CP_System_GetWindowHeight() * 0.5f, 1280 * scaling_factor, 720 * scaling_factor, 255);

	
	
	if (current_how_to_play_page != 0) {
		CP_Graphics_DrawRect(TutorialButtons[kHowToPlayPrevious].x_origin,
			TutorialButtons[kHowToPlayPrevious].y_origin,
			TutorialButtons[kHowToPlayPrevious].width,
			TutorialButtons[kHowToPlayPrevious].height);
		RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet, interactable_UI_buttons_spritesheet_array[4],
			TutorialButtons[kHowToPlayPrevious].x_origin + TutorialButtons[kHowToPlayPrevious].width / 2,
			TutorialButtons[kHowToPlayPrevious].y_origin + TutorialButtons[kHowToPlayPrevious].height / 2,
			TutorialButtons[kHowToPlayPrevious].width,
			TutorialButtons[kHowToPlayPrevious].height);

	}

	else
	{
		CP_Graphics_DrawRect(TutorialButtons[kHowToPlayBack].x_origin,
			TutorialButtons[kHowToPlayBack].y_origin,
			TutorialButtons[kHowToPlayBack].width,
			TutorialButtons[kHowToPlayBack].height);
		RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet, interactable_UI_buttons_spritesheet_array[5],
			TutorialButtons[kHowToPlayBack].x_origin + TutorialButtons[kHowToPlayBack].width / 2,
			TutorialButtons[kHowToPlayBack].y_origin + TutorialButtons[kHowToPlayBack].height / 2,
			TutorialButtons[kHowToPlayBack].width,
			TutorialButtons[kHowToPlayBack].height);
	}

	if (current_how_to_play_page != kMaxHowToPlayPages - 1) {
		CP_Graphics_DrawRect(TutorialButtons[kHowToPlayNext].x_origin,
			TutorialButtons[kHowToPlayNext].y_origin,
			TutorialButtons[kHowToPlayNext].width,
			TutorialButtons[kHowToPlayNext].height);
		RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet, interactable_UI_buttons_spritesheet_array[3],
			TutorialButtons[kHowToPlayNext].x_origin + TutorialButtons[kHowToPlayNext].width/2,
			TutorialButtons[kHowToPlayNext].y_origin + TutorialButtons[kHowToPlayNext].height/ 2,
			TutorialButtons[kHowToPlayNext].width,
			TutorialButtons[kHowToPlayNext].height);
		
	}
	
	/*switch (current_how_to_play_page) {
	case 0:
		CP_Graphics_DrawCircle(100, 100, 100);
		break;
	case 1:
		CP_Graphics_DrawCircle(200, 200, 200);
		break;
	case 2:
		CP_Graphics_DrawCircle(300, 300, 300);
		break;
	case 3:
		CP_Graphics_DrawCircle(400, 400, 400);
		break;
	}*/

	if (BtnIsPressed(TutorialButtons[kHowToPlayBack]) && current_how_to_play_page == 0) {
		current_how_to_play_page = 0;
		current_game_state = kMainMenu;
	}
	else if (BtnIsPressed(TutorialButtons[kHowToPlayPrevious])) {
		if (current_how_to_play_page != 0) {
			current_how_to_play_page -= 1;
		}
	}
	else if (BtnIsPressed(TutorialButtons[kHowToPlayNext])) {
		if (current_how_to_play_page != kMaxHowToPlayPages - 1) {
			current_how_to_play_page += 1;
		}
	}
	MouseReset();
}