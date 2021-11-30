#include "game.h"
#include "zhengwei.h"
#include "Gabriel.h"

#include "cprocessing.h"

//Initialize global variables
const float kLeftGameMenuXWidth = 140.714294f;
const float kRightGameMenuXOrigin = 998.571411f;
//Tools
//Reset the current click position
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
//Check which button is pressed in the game
MenuObjectType CheckGameButtonPressed(void) {
	for (int i = 0; i < kButtonMax; i++) {
		if (BtnIsPressed(game_menu_object[i])) {
			return i;
		}
	}
	return kButtonMax;
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
//init the spawn and exit cols for pathfinding
static void InitPathFinding(void) {
	level.grid[level.spawn_row][level.spawn_col].cost = 0;
	level.grid[level.spawn_row][level.spawn_col].visited = 1;
	level.grid[level.spawn_row][level.spawn_col].type = kSpawn;
	level.grid[level.exit_row][level.exit_col].type = kExit;
}
//reset Data in Pathfinding to calculate new path
static void ResetPathFinding(void) {
	for (int i = 0; i < level_grid_rows; i++) {
		for (int j = 0; j < level_grid_cols; j++) {
			level.grid[i][j].visited = 0;
			level.grid[i][j].cost = -1;
			level.grid[i][j].parent_row = -1;
			level.grid[i][j].parent_col = -1;
			if (level.grid[i][j].type == kPath) {
				level.grid[i][j].type = kClear;
			}
			if (level.grid[i][j].type == kBlocked) {
				level.grid[i][j].visited = 1;
				//Keep blocked grid blocked
			}
		}
	}
	level.grid[level.spawn_row][level.spawn_col].cost = 0;
	level.grid[level.spawn_row][level.spawn_col].visited = 1;
}
//Check if destination is reachable
static Boolean isDestinationUpdated(void) {
	return level.grid[level.exit_row][level.exit_col].visited;
}
//Update cost of neighbor base on own cost
static void UpdatePathFindingNeighborCost(
	int grid_row, int grid_col, int generation) {
	//Update Row Neighbor
	for (int i = -1; i <= 1; i++) {
		if (grid_row + i >= 0 && grid_row + i < level_grid_rows) {
			if (level.grid[grid_row + i][grid_col].visited == 0) {
				level.grid[grid_row + i][grid_col].cost = generation + 1;
				level.grid[grid_row + i][grid_col].parent_row = grid_row;
				level.grid[grid_row + i][grid_col].parent_col = grid_col;
				level.grid[grid_row + i][grid_col].visited = 1;
			}
		}
	}
	//Update Col Neighbor
	for (int i = -1; i <= 1; i++) {
		if (grid_col + i >= 0 && grid_col + i < level_grid_cols) {
			if (level.
				grid[grid_row][grid_col + i].visited == 0) {
				level.
					grid[grid_row][grid_col + i].cost = generation + 1;
				level.
					grid[grid_row][grid_col + i].parent_row = grid_row;
				level.
					grid[grid_row][grid_col + i].parent_col = grid_col;
				level.
					grid[grid_row][grid_col + i].visited = 1;
			}
		}
	}
}
//Calculate all grid cost. Find the squares in the same generation 
//and call a function to update neighbors.
static void CalculatePathFindingCost(void) {
	for (int current_cost = 0; !isDestinationUpdated()
		&& current_cost <= level_grid_rows * level_grid_cols; current_cost++) {
		for (int i = 0; i < level_grid_rows; i++) {
			for (int j = 0; j < level_grid_cols; j++) {
				if (level.grid[i][j].cost == current_cost) {
					UpdatePathFindingNeighborCost(i, j, current_cost);
				}
			}
		}
	}
}
//update path for pathfinding
static void UpdatePathFinding(void) {
	int path_row, path_col;
	path_row = level.grid[level.exit_row][level.exit_col].parent_row;
	path_col = level.grid[level.exit_row][level.exit_col].parent_col;
	//To check if path have no neighbors
	if (path_row != -1 && path_col != -1) {
		while (level.grid[path_row][path_col].cost) {
			level.grid[path_row][path_col].type = kPath;
			int newPathRow, newPathCol;
			newPathRow = level.grid[path_row][path_col].parent_row;
			newPathCol = level.grid[path_row][path_col].parent_col;
			path_row = newPathRow;
			path_col = newPathCol;
		}
	}
}
//Function for doing all the stuff when clicking on game grid
static void UpdateGameGridPress(void) {
	int draw_x, draw_y;//Grid rows and cols
	draw_x = (int)((mouse_input.x_origin - game.x_origin) / game.grid_width);
	draw_y = (int)((mouse_input.y_origin - game.y_origin) / game.grid_height);
	Coordinates grid_temp;//temp game grid to test for collision detection
	grid_temp.width = game.grid_width;
	grid_temp.height = game.grid_height;
	grid_temp.x_origin = game.x_origin + (draw_x + 0.5f) * game.grid_width;
	grid_temp.y_origin = game.y_origin + (draw_y + 0.5f) * game.grid_height;

	int is_position_empty = kTrue;//To prevent building on turret
	for (int i = 0; i < kMaxTurret; i++) {
		if (turret[i].data.x_origin == grid_temp.x_origin
			&& turret[i].data.y_origin == grid_temp.y_origin
			&& turret[i].is_active == kTrue)
		{
			is_position_empty = kFalse;
		}
	}
	if (is_placing_turret != kTMax) {//if a turret type is selected
		if (is_position_empty == kTrue
			&& level.grid[draw_y][draw_x].type != kBlocked)
			//Prevent building on environment
		{
			if (is_placing_turret != kTMine) {
				level.grid[draw_y][draw_x].type = kBlocked;
				ResetPathFinding();
				CalculatePathFindingCost();
				//Set grid to blocked and check if end is reachable
			}
			if (!isDestinationUpdated()) {//Check if end is reachable
				//if Unreachable
				level.grid[draw_y][draw_x].type = kClear;
				ResetPathFinding();
				CalculatePathFindingCost();
				//Set the grid back to unblocked and do not place turret
			}
			else {
				//Else place turret
				place_turret(is_placing_turret, draw_x, draw_y);
				level.phantom_quartz -=
					turret_purchasing[kTPPrice][is_placing_turret];
				is_placing_turret = kTMax;
			}
			UpdatePathFinding();//Find the new shortest path
		}
		MouseReset();
	}
	else {//No turret type selected use to check for selecting existing turret
		for (int i = 0; i < kMaxTurret; i++) {
			if (turret[i].data.x_origin == grid_temp.x_origin
				&& turret[i].data.y_origin == grid_temp.y_origin
				&& turret[i].is_active == kTrue
				&& turret_selected_to_upgrade == i
				&& CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
			{
				//If the current turret is selected and is selected again,
				//Deselect the turret and close the upgrade menu.
				turret_selected_to_upgrade = kNoTurretSelected;
				MouseReset();
			}
			else if (turret[i].data.x_origin == grid_temp.x_origin
				&& turret[i].data.y_origin == grid_temp.y_origin
				&& turret[i].is_active == kTrue)
			{
				//When a existing turret is selected open the upgrade menu
				turret_selected_to_upgrade = i;
				if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
					CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
						CP_SOUND_GROUP_0);
				}
			}
		}
	}
}
//initialize Number of each type of enemies in the level using given variable
static void InitGeneralLevelEnemies(
	int wave, int basic, int fast, int fat, int grim)
{
	level.wave_enemies[wave][kBasic] = basic;
	level.wave_enemies[wave][kFastGhost] = fast;
	level.wave_enemies[wave][kFatGhost] = fat;
	level.wave_enemies[wave][kGrimReaper] = grim;
}
//function to initialize environment object data base on given inputs
static void InitEnvironmentObject(
	int array_index, int row, int col, int object_type)
{
	environment[array_index].image = grid_environment_objects_spritesheet;
	environment[array_index].x_origin =
		game.x_origin + game.grid_width * (col + 0.5f);
	environment[array_index].y_origin =
		game.y_origin + game.grid_height * (row + 0.5f);
	environment[array_index].width = game.grid_width;
	environment[array_index].height = game.grid_height;
	environment[array_index].object_type = object_type;

	level.grid[row][col].type = kBlocked;
	level.grid[row][col].type = kBlocked;
}
//Initialize positions of Buttons and Display
static void InitGameGrid(void) {
	/*Height and Width not used for game, example menus*/
	float unusable_screen_height;
	//Half the screeen height is used for the gamegrid itself
	unusable_screen_height = (float)CP_System_GetWindowHeight() / 2;
	game.height = (float)CP_System_GetWindowHeight() - unusable_screen_height;
	game.grid_height = game.height / level_grid_rows;
	game.grid_width = game.grid_height; //Grid is a Square
	game.width = game.grid_width * level_grid_cols;
	//To centralise the Grid
	game.x_origin = ((float)CP_System_GetWindowWidth() - game.width) / 2;
	game.y_origin = unusable_screen_height / 2;
	//Coordinates game grid used to check for pressing on game grid
	game_menu_object[kGameGrid].x_origin = game.x_origin;
	game_menu_object[kGameGrid].y_origin = game.y_origin;
	game_menu_object[kGameGrid].width = game.width;
	game_menu_object[kGameGrid].height = game.height;
	game_menu_object[kGameGrid].object_type = kObjectRectangle;
}
static void InitPauseButton(void) {
	game_menu_object[kPauseButton].x_origin = 0.0f;
	game_menu_object[kPauseButton].y_origin = 0.0f;
	game_menu_object[kPauseButton].width = kLeftGameMenuXWidth;
	game_menu_object[kPauseButton].height =
		(float)CP_System_GetWindowHeight() / 10;
	game_menu_object[kPauseButton].image = pause_button_image;
}
static void InitBasicTurretButton(void) {
	game_menu_object[kTurretButtonBasic].x_origin =
		game_menu_object[kPauseButton].x_origin;
	game_menu_object[kTurretButtonBasic].y_origin =
		game_menu_object[kPauseButton].height;
	game_menu_object[kTurretButtonBasic].width =
		game_menu_object[kPauseButton].width;
	game_menu_object[kTurretButtonBasic].height =
		game_menu_object[kPauseButton].height * 2;
	game_menu_object[kTurretButtonBasic].image = slow_turret_image_array[0];
}
static void InitSlowTurretButton(void) {
	game_menu_object[kTurretButtonSlow].x_origin =
		game_menu_object[kTurretButtonBasic].x_origin;
	game_menu_object[kTurretButtonSlow].y_origin =
		game_menu_object[kTurretButtonBasic].y_origin
		+ game_menu_object[kTurretButtonBasic].height;
	game_menu_object[kTurretButtonSlow].width =
		game_menu_object[kTurretButtonBasic].width;
	game_menu_object[kTurretButtonSlow].height =
		game_menu_object[kTurretButtonBasic].height;
	game_menu_object[kTurretButtonSlow].image = slow_turret_image_array[0];
}
static void InitHomingTurretButton(void) {
	game_menu_object[kTurretButtonHoming].x_origin =
		game_menu_object[kTurretButtonSlow].x_origin;
	game_menu_object[kTurretButtonHoming].y_origin =
		game_menu_object[kTurretButtonSlow].y_origin
		+ game_menu_object[kTurretButtonSlow].height;
	game_menu_object[kTurretButtonHoming].width =
		game_menu_object[kTurretButtonSlow].width;
	game_menu_object[kTurretButtonHoming].height =
		game_menu_object[kTurretButtonSlow].height;
	game_menu_object[kTurretButtonHoming].image = slow_turret_image_array[0];
}
static void InitTurretMineButton(void) {
	game_menu_object[kTurretButtonMine].x_origin =
		game_menu_object[kTurretButtonHoming].x_origin;
	game_menu_object[kTurretButtonMine].y_origin =
		game_menu_object[kTurretButtonHoming].y_origin
		+ game_menu_object[kTurretButtonHoming].height;
	game_menu_object[kTurretButtonMine].width =
		game_menu_object[kTurretButtonHoming].width;
	game_menu_object[kTurretButtonMine].height =
		game_menu_object[kTurretButtonBasic].height;
	game_menu_object[kTurretButtonMine].image = slow_turret_image_array[0];
}
static void InitPhantomQuartzDisplay(void) {
	game_menu_object[kPhantomQuartzMenu].y_origin = 0.0f;
	game_menu_object[kPhantomQuartzMenu].x_origin = kRightGameMenuXOrigin;
	game_menu_object[kPhantomQuartzMenu].height =
		(float)CP_System_GetWindowHeight() / 15;
	game_menu_object[kPhantomQuartzMenu].width =
		((float)CP_System_GetWindowWidth()
			- game_menu_object[kPhantomQuartzMenu].x_origin) / 2;
}
static void InitGoldQuartzDisplay(void) {
	game_menu_object[kGoldQuartzMenu].width =
		game_menu_object[kPhantomQuartzMenu].width;
	game_menu_object[kGoldQuartzMenu].height =
		game_menu_object[kPhantomQuartzMenu].height;
	game_menu_object[kGoldQuartzMenu].x_origin =
		(float)CP_System_GetWindowWidth()
		- game_menu_object[kPhantomQuartzMenu].width;
	game_menu_object[kGoldQuartzMenu].y_origin =
		game_menu_object[kPhantomQuartzMenu].y_origin;
}
static void InitHealthDisplay(void) {
	game_menu_object[kHealthMenu].x_origin =
		game_menu_object[kPhantomQuartzMenu].x_origin;
	game_menu_object[kHealthMenu].y_origin =
		game_menu_object[kPhantomQuartzMenu].y_origin
		+ game_menu_object[kPhantomQuartzMenu].height;
	game_menu_object[kHealthMenu].width =
		game_menu_object[kPhantomQuartzMenu].width;
	game_menu_object[kHealthMenu].height =
		game_menu_object[kPhantomQuartzMenu].height;
}
static void InitMenuSwapButton(void) {
	game_menu_object[kSwapButton].x_origin = game_menu_object[kTurretButtonMine].x_origin;
	game_menu_object[kSwapButton].y_origin = game_menu_object[kTurretButtonMine].y_origin + game_menu_object[kTurretButtonMine].height;
	game_menu_object[kSwapButton].width = game_menu_object[kTurretButtonMine].width;
	game_menu_object[kSwapButton].height = game_menu_object[kTurretButtonMine].height / 2;
}
static void InitWaveNumberDisplay(void) {
	game_menu_object[kWaveDisplay].x_origin = game_menu_object[kGoldQuartzMenu].x_origin;
	game_menu_object[kWaveDisplay].y_origin = game_menu_object[kHealthMenu].y_origin;
	game_menu_object[kWaveDisplay].width = game_menu_object[kGoldQuartzMenu].width;
	game_menu_object[kWaveDisplay].height = game_menu_object[kGoldQuartzMenu].height;
}
static void InitEnvironmentEffectsDisplay(void) {
	game_menu_object[kEnvironmentalEffects].x_origin = game_menu_object[kHealthMenu].x_origin;
	game_menu_object[kEnvironmentalEffects].y_origin =
		game_menu_object[kHealthMenu].y_origin
		+ game_menu_object[kHealthMenu].height;
	game_menu_object[kEnvironmentalEffects].width =
		game_menu_object[kHealthMenu].width;
	game_menu_object[kEnvironmentalEffects].height =
		game_menu_object[kHealthMenu].height * 3;
}
static void InitRemainingMonsterDisplay(void) {
	game_menu_object[kMonsterRemainingDisplay].x_origin =
		game_menu_object[kWaveDisplay].x_origin;
	game_menu_object[kMonsterRemainingDisplay].y_origin =
		game_menu_object[kWaveDisplay].y_origin
		+ game_menu_object[kWaveDisplay].height;
	game_menu_object[kMonsterRemainingDisplay].width =
		game_menu_object[kWaveDisplay].width;
	game_menu_object[kMonsterRemainingDisplay].height =
		game_menu_object[kWaveDisplay].height * 3;
}
static void InitUpgradeMenuDisplay(void) {
	game_menu_object[kUpgradeMenu].x_origin =
		game_menu_object[kEnvironmentalEffects].x_origin;
	game_menu_object[kUpgradeMenu].y_origin =
		game_menu_object[kEnvironmentalEffects].y_origin
		+ game_menu_object[kEnvironmentalEffects].height;
	game_menu_object[kUpgradeMenu].width =
		game_menu_object[kEnvironmentalEffects].width
		+ game_menu_object[kMonsterRemainingDisplay].width;
	game_menu_object[kUpgradeMenu].height =
		(CP_System_GetWindowHeight()
			- game_menu_object[kEnvironmentalEffects].y_origin);
}
static void InitUpgradeButton(void) {
	game_menu_object[kUpgradeButton].x_origin =
		game_menu_object[kEnvironmentalEffects].x_origin
		+ game_menu_object[kEnvironmentalEffects].width / 5;
	game_menu_object[kUpgradeButton].y_origin =
		game_menu_object[kSwapButton].y_origin
		- game_menu_object[kSwapButton].height / 5;
	game_menu_object[kUpgradeButton].width =
		game_menu_object[kUpgradeMenu].width / 3;
	game_menu_object[kUpgradeButton].height =
		game_menu_object[kSwapButton].height;
}
static void InitSellButton(void) {
	game_menu_object[kSellButton].x_origin =
		game_menu_object[kMonsterRemainingDisplay].x_origin
		+ game_menu_object[kMonsterRemainingDisplay].width / 5;
	game_menu_object[kSellButton].y_origin =
		game_menu_object[kSwapButton].y_origin -
		game_menu_object[kSwapButton].height / 5;
	game_menu_object[kSellButton].width =
		game_menu_object[kUpgradeMenu].width / 3;
	game_menu_object[kSellButton].height =
		game_menu_object[kSwapButton].height;
}
static void InitTurretDetailsDisplay
(MenuObjectType turret_button_selected) {
	game_menu_object[kTurretDetailsDisplay].x_origin =
		game_menu_object[turret_button_selected].x_origin
		+ game_menu_object[turret_button_selected].width;
	game_menu_object[kTurretDetailsDisplay].y_origin =
		game_menu_object[turret_button_selected].y_origin
		+ game_menu_object[turret_button_selected].width / 4;
	game_menu_object[kTurretDetailsDisplay].width =
		game_menu_object[turret_button_selected].width * 1.25f;
	game_menu_object[kTurretDetailsDisplay].height =
		game_menu_object[turret_button_selected].height / 2;
}
static void InitEnvironment(void) {
	//Removes all  previous environment when changing levels
	for (int i = 0; i < kMaxEnvironmentObject; i++) {
		environment[i].image = NULL;
	}
	switch (level.current_game_level)
	{
	case 0:
		InitEnvironmentObject(0, 2, 2, 8);
		InitEnvironmentObject(1, 3, 4, 8);
		InitEnvironmentObject(2, 5, 1, 8);
		break;
	case 1:
		InitEnvironmentObject(0, 0, 1, 8);
		InitEnvironmentObject(1, 1, 4, 8);
		InitEnvironmentObject(2, 5, 1, 8);
		InitEnvironmentObject(3, 5, 5, 8);
		InitEnvironmentObject(4, 2, 2, 8);
		break;
	case 2:
		InitEnvironmentObject(0, 3, 3, 8);
		InitEnvironmentObject(1, 3, 4, 9);
		InitEnvironmentObject(2, 2, 1, 8);
		InitEnvironmentObject(3, 5, 5, 8);
		InitEnvironmentObject(4, 2, 2, 9);
		InitEnvironmentObject(5, 4, 0, 8);
		break;
	case 3:
		InitEnvironmentObject(0, 2, 2, 8);
		InitEnvironmentObject(1, 3, 3, 4);
		InitEnvironmentObject(2, 1, 0, 8);
		InitEnvironmentObject(3, 5, 2, 3);
		InitEnvironmentObject(4, 1, 1, 9);
		InitEnvironmentObject(5, 3, 1, 9);
		InitEnvironmentObject(6, 4, 4, 3);
		break;
	case 4:
		InitEnvironmentObject(0, 0, 2, 0);
		InitEnvironmentObject(1, 3, 4, 1);
		InitEnvironmentObject(2, 2, 1, 4);
		InitEnvironmentObject(3, 5, 5, 3);
		InitEnvironmentObject(4, 3, 2, 0);
		InitEnvironmentObject(5, 6, 2, 1);
		InitEnvironmentObject(6, 5, 1, 4);
		InitEnvironmentObject(7, 6, 4, 3);
		InitEnvironmentObject(8, 2, 6, 0);
		break;
	}
}
void InitPowerUpPrice(void) {
	power_up_price.more_phantom_quartz = 10;
	power_up_price.reduce_enemy_speed = 10;
	power_up_price.reduce_enemy_health = 10;
	power_up_price.increased_mine_damage = 10;
}
void InitLevel1(void) {
	level_grid_cols = kLevel1Cols;
	level_grid_rows = kLevel1Rows;
	//Counting from 0
	level.current_game_level = 0;
	// using size of pointers so that lower bits operating system 
	//do not require so much memory usage
	level.grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));
	for (int i = 0; i < level_grid_rows; i++) {
		//memory allocating for a 2d array
		if (level.grid != NULL) {
			level.grid[i] = (struct Grids*)
				calloc(level_grid_cols, sizeof(struct Grids));
			if (level.grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	level.spawn_row = 0;
	level.spawn_col = 0;
	level.exit_row = level_grid_rows - 1;
	level.exit_col = level_grid_cols - 1;
	level.health = 100;
	level.phantom_quartz = 50000;
	level.gold_quartz = 0;
	level.current_wave = 0;
	level.current_effect = kNoEnvironmentalEffects;
	level.current_power_up_level.more_phantom_quartz = 0;
	level.current_power_up_level.reduce_enemy_speed = 0;
	level.current_power_up_level.reduce_enemy_health = 0;
	level.current_power_up_level.increased_mine_damage = 0;
	InitGeneralLevelEnemies(0, 10, 0, 0, 0);
	InitGeneralLevelEnemies(1, 10, 0, 0, 0);
	InitGeneralLevelEnemies(2, 15, 0, 0, 0);
	InitGeneralLevelEnemies(3, 15, 0, 0, 0);
	InitGeneralLevelEnemies(4, 15, 0, 0, 0);
	InitGeneralLevelEnemies(5, 20, 0, 0, 0);
	InitGeneralLevelEnemies(6, 20, 0, 0, 0);
	InitGeneralLevelEnemies(7, 25, 0, 0, 0);
	InitGeneralLevelEnemies(8, 30, 0, 0, 0);
	InitGeneralLevelEnemies(9, 30, 0, 0, 0);
	InitGameGrid();
	is_placing_turret = kTMax;
	turret_selected_to_upgrade = kNoTurretSelected;
	power_up_menu = kFalse;
	InitPathFinding();
	InitEnvironment();
	InitPauseButton();
	InitBasicTurretButton();
	InitSlowTurretButton();
	InitHomingTurretButton();
	InitTurretMineButton();
	InitPhantomQuartzDisplay();
	InitGoldQuartzDisplay();
	InitHealthDisplay();
	InitMenuSwapButton();
	InitWaveNumberDisplay();
	InitEnvironmentEffectsDisplay();
	InitRemainingMonsterDisplay();
	InitUpgradeMenuDisplay();
	InitUpgradeButton();
	InitSellButton();
	turret_init();
	Enemies_init();
	ResetPathFinding();
	CalculatePathFindingCost();
	UpdatePathFinding();
	SetBuildingTime(kFullBuildingPhaseTime);
	current_game_state = kBuilding;
	//BGM Sound
	CP_Sound_StopGroup(CP_SOUND_GROUP_1);
	CP_Sound_PlayAdvanced(BuildingBGM, BGM_Volume, 1.0f, TRUE,
		CP_SOUND_GROUP_1);
}
void InitLevel2(void) {
	level_grid_cols = kLevel2Cols;
	level_grid_rows = kLevel2Rows;
	//Counting from 0
	level.current_game_level = 1;
	// using size of pointers so that lower bits operating system
	//do not require so much memory usage
	level.grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));
	for (int i = 0; i < level_grid_rows; i++) {
		if (level.grid != NULL) {
			//Memory allocating for a 2d array
			level.grid[i] = (struct Grids*)
				calloc(level_grid_cols, sizeof(struct Grids));
			if (level.grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	level.spawn_row = 0;
	level.spawn_col = 0;
	level.exit_row = level_grid_rows - 1;
	level.exit_col = level_grid_cols - 1;
	level.health = 100;
	level.phantom_quartz = 300;
	level.gold_quartz = 0;
	level.current_wave = 0;
	level.current_effect = kNoEnvironmentalEffects;
	level.current_power_up_level.more_phantom_quartz = 0;
	level.current_power_up_level.reduce_enemy_speed = 0;
	level.current_power_up_level.reduce_enemy_health = 0;
	level.current_power_up_level.increased_mine_damage = 0;
	InitGeneralLevelEnemies(0, 10, 0, 0, 0);
	InitGeneralLevelEnemies(1, 10, 0, 0, 0);
	InitGeneralLevelEnemies(2, 15, 1, 0, 0);
	InitGeneralLevelEnemies(3, 15, 5, 0, 0);
	InitGeneralLevelEnemies(4, 20, 5, 0, 0);
	InitGeneralLevelEnemies(5, 20, 10, 0, 0);
	InitGeneralLevelEnemies(6, 25, 10, 0, 0);
	InitGeneralLevelEnemies(7, 25, 10, 0, 0);
	InitGeneralLevelEnemies(8, 30, 10, 0, 0);
	InitGeneralLevelEnemies(9, 25, 15, 0, 0);
	InitGameGrid();
	is_placing_turret = kTMax;
	turret_selected_to_upgrade = kNoTurretSelected;
	power_up_menu = kFalse;
	InitPathFinding();
	InitEnvironment();
	InitPauseButton();
	InitBasicTurretButton();
	InitSlowTurretButton();
	InitHomingTurretButton();
	InitTurretMineButton();
	InitPhantomQuartzDisplay();
	InitGoldQuartzDisplay();
	InitHealthDisplay();
	InitMenuSwapButton();
	InitWaveNumberDisplay();
	InitEnvironmentEffectsDisplay();
	InitRemainingMonsterDisplay();
	InitUpgradeMenuDisplay();
	InitUpgradeButton();
	InitSellButton();
	turret_init();
	Enemies_init();
	ResetPathFinding();
	CalculatePathFindingCost();
	UpdatePathFinding();
	SetBuildingTime(kFullBuildingPhaseTime);
	current_game_state = kBuilding;
	//BGM Sound
	CP_Sound_StopGroup(CP_SOUND_GROUP_1);
	CP_Sound_PlayAdvanced(BuildingBGM, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
}
void InitLevel3(void) {
	level_grid_cols = kLevel3Cols;
	level_grid_rows = kLevel3Rows;
	//Counting from 0
	level.current_game_level = 2;
	// using size of pointers so that lower bits operating system 
	//do not require so much memory usage
	level.grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));
	for (int i = 0; i < level_grid_rows; i++) {
		if (level.grid != NULL) {
			//Memory allocating for a 2d array
			level.grid[i] = (struct Grids*)
				calloc(level_grid_cols, sizeof(struct Grids));
			if (level.grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	level.spawn_row = level_grid_rows - 1;
	level.spawn_col = level_grid_cols - 1;
	level.exit_row = 0;
	level.exit_col = 0;
	level.health = 100;
	level.phantom_quartz = 300;
	level.gold_quartz = 0;
	level.current_wave = 0;
	level.current_effect = kNoEnvironmentalEffects;
	level.current_power_up_level.more_phantom_quartz = 0;
	level.current_power_up_level.reduce_enemy_speed = 0;
	level.current_power_up_level.reduce_enemy_health = 0;
	level.current_power_up_level.increased_mine_damage = 0;
	InitGeneralLevelEnemies(0, 10, 0, 0, 0);
	InitGeneralLevelEnemies(1, 10, 5, 0, 0);
	InitGeneralLevelEnemies(2, 10, 5, 0, 0);
	InitGeneralLevelEnemies(3, 15, 5, 0, 0);
	InitGeneralLevelEnemies(4, 15, 10, 1, 0);
	InitGeneralLevelEnemies(5, 15, 10, 5, 0);
	InitGeneralLevelEnemies(6, 15, 10, 5, 0);
	InitGeneralLevelEnemies(7, 20, 10, 5, 0);
	InitGeneralLevelEnemies(8, 20, 10, 10, 0);
	InitGeneralLevelEnemies(9, 25, 10, 10, 0);
	InitGameGrid();
	is_placing_turret = kTMax;
	turret_selected_to_upgrade = kNoTurretSelected;
	power_up_menu = kFalse;
	InitPathFinding();
	InitEnvironment();
	InitPauseButton();
	InitBasicTurretButton();
	InitSlowTurretButton();
	InitHomingTurretButton();
	InitTurretMineButton();
	InitPhantomQuartzDisplay();
	InitGoldQuartzDisplay();
	InitHealthDisplay();
	InitMenuSwapButton();
	InitWaveNumberDisplay();
	InitEnvironmentEffectsDisplay();
	InitRemainingMonsterDisplay();
	InitUpgradeMenuDisplay();
	InitUpgradeButton();
	InitSellButton();
	turret_init();
	Enemies_init();
	ResetPathFinding();
	CalculatePathFindingCost();
	UpdatePathFinding();
	SetBuildingTime(kFullBuildingPhaseTime);
	current_game_state = kBuilding;
	//BGM Sound
	CP_Sound_StopGroup(CP_SOUND_GROUP_1);
	CP_Sound_PlayAdvanced(BuildingBGM, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
}
void InitLevel4(void) {
	level_grid_cols = kLevel4Cols;
	level_grid_rows = kLevel4Rows;
	//Counting from 0
	level.current_game_level = 3;
	// using size of pointers so that lower bits operating system 
	//do not require so much memory usage
	level.grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));
	for (int i = 0; i < level_grid_rows; i++) {
		if (level.grid != NULL) {
			//Memory allocating for a 2d array
			level.grid[i] = (struct Grids*)
				calloc(level_grid_cols, sizeof(struct Grids));
			if (level.grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	level.spawn_row = 0;
	level.spawn_col = 0;
	level.exit_row = level_grid_rows - 1;
	level.exit_col = 0;
	level.health = 100;
	level.phantom_quartz = 300;
	level.gold_quartz = 0;
	level.current_wave = 0;
	level.current_effect = kNoEnvironmentalEffects;
	level.current_power_up_level.more_phantom_quartz = 0;
	level.current_power_up_level.reduce_enemy_speed = 0;
	level.current_power_up_level.reduce_enemy_health = 0;
	level.current_power_up_level.increased_mine_damage = 0;
	InitGeneralLevelEnemies(0, 10, 0, 0, 0);
	InitGeneralLevelEnemies(1, 10, 5, 0, 0);
	InitGeneralLevelEnemies(2, 10, 3, 2, 0);
	InitGeneralLevelEnemies(3, 10, 5, 5, 0);
	InitGeneralLevelEnemies(4, 15, 10, 5, 0);
	InitGeneralLevelEnemies(5, 15, 10, 5, 0);
	InitGeneralLevelEnemies(6, 15, 10, 10, 0);
	InitGeneralLevelEnemies(7, 10, 10, 10, 1);
	InitGeneralLevelEnemies(8, 15, 15, 10, 2);
	InitGeneralLevelEnemies(9, 20, 10, 15, 3);
	InitGameGrid();
	is_placing_turret = kTMax;
	turret_selected_to_upgrade = kNoTurretSelected;
	power_up_menu = kFalse;
	InitPathFinding();
	InitEnvironment();
	InitPauseButton();
	InitBasicTurretButton();
	InitSlowTurretButton();
	InitHomingTurretButton();
	InitTurretMineButton();
	InitPhantomQuartzDisplay();
	InitGoldQuartzDisplay();
	InitHealthDisplay();
	InitMenuSwapButton();
	InitWaveNumberDisplay();
	InitEnvironmentEffectsDisplay();
	InitRemainingMonsterDisplay();
	InitUpgradeMenuDisplay();
	InitUpgradeButton();
	InitSellButton();
	turret_init();
	Enemies_init();
	ResetPathFinding();
	CalculatePathFindingCost();
	UpdatePathFinding();
	SetBuildingTime(kFullBuildingPhaseTime);
	current_game_state = kBuilding;
	//BGM Sound
	CP_Sound_StopGroup(CP_SOUND_GROUP_1);
	CP_Sound_PlayAdvanced(BuildingBGM, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
}
void InitLevel5(void) {
	level_grid_cols = kLevel5Cols;
	level_grid_rows = kLevel5Rows;
	//Counting from 0
	level.current_game_level = 4;
	//using size of pointers so that lower bits operating system 
	//do not require so much memory usage
	level.grid = (struct Grids**)calloc(level_grid_rows, sizeof(int*));
	for (int i = 0; i < level_grid_rows; i++) {
		if (level.grid != NULL) {
			//Memory allocation for a 2d array
			level.grid[i] = (struct Grids*)
				calloc(level_grid_cols, sizeof(struct Grids));
			if (level.grid[i] == NULL) {
				exit_to_desktop();
			}
		}
		else {
			exit_to_desktop();
		}
	}
	level.spawn_row = 0;
	level.spawn_col = (level_grid_cols - 1) / 2;
	level.exit_row = level_grid_rows - 1;
	level.exit_col = (level_grid_cols - 1) / 2;
	level.health = 100;
	level.phantom_quartz = 300;
	level.gold_quartz = 0;
	level.current_wave = 0;
	level.current_effect = kNoEnvironmentalEffects;
	level.current_power_up_level.more_phantom_quartz = 0;
	level.current_power_up_level.reduce_enemy_speed = 0;
	level.current_power_up_level.reduce_enemy_health = 0;
	level.current_power_up_level.increased_mine_damage = 0;
	InitGeneralLevelEnemies(0, 10, 0, 0, 0);
	InitGeneralLevelEnemies(1, 15, 0, 0, 0);
	InitGeneralLevelEnemies(2, 15, 5, 0, 0);
	InitGeneralLevelEnemies(3, 10, 5, 5, 0);
	InitGeneralLevelEnemies(4, 15, 5, 5, 0);
	InitGeneralLevelEnemies(5, 10, 10, 10, 0);
	InitGeneralLevelEnemies(6, 15, 10, 10, 0);
	InitGeneralLevelEnemies(7, 15, 10, 10, 1);
	InitGeneralLevelEnemies(8, 15, 15, 15, 2);
	InitGeneralLevelEnemies(9, 10, 10, 10, 5);
	InitGameGrid();
	InitPauseButton();
	InitBasicTurretButton();
	InitSlowTurretButton();
	InitHomingTurretButton();
	InitTurretMineButton();
	InitPhantomQuartzDisplay();
	InitGoldQuartzDisplay();
	InitHealthDisplay();
	InitMenuSwapButton();
	InitWaveNumberDisplay();
	InitEnvironmentEffectsDisplay();
	InitRemainingMonsterDisplay();
	InitUpgradeMenuDisplay();
	InitUpgradeButton();
	InitSellButton();
	is_placing_turret = kTMax;
	turret_selected_to_upgrade = kNoTurretSelected;
	power_up_menu = kFalse;
	InitPathFinding();
	InitEnvironment();
	turret_init();
	Enemies_init();
	ResetPathFinding();
	CalculatePathFindingCost();
	UpdatePathFinding();
	SetBuildingTime(kFullBuildingPhaseTime);
	current_game_state = kBuilding;
	//BGM Sound
	CP_Sound_StopGroup(CP_SOUND_GROUP_1);
	CP_Sound_PlayAdvanced(BuildingBGM, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
}
void InitHowToPlayButtons(void) {
	TutorialButtons[kHowToPlayBack].x_origin =
		(float)CP_System_GetWindowWidth() / 50;
	TutorialButtons[kHowToPlayBack].y_origin =
		(float)CP_System_GetWindowHeight() / 50;
	TutorialButtons[kHowToPlayBack].width =
		(float)CP_System_GetWindowWidth() / 10;
	TutorialButtons[kHowToPlayBack].height =
		(float)CP_System_GetWindowHeight() / 10;
	TutorialButtons[kHowToPlayPrevious].x_origin =
		(float)CP_System_GetWindowWidth() / 50;
	TutorialButtons[kHowToPlayPrevious].y_origin =
		(float)CP_System_GetWindowHeight() / 50;
	TutorialButtons[kHowToPlayPrevious].width =
		(float)CP_System_GetWindowWidth() / 10;
	TutorialButtons[kHowToPlayPrevious].height =
		(float)CP_System_GetWindowHeight() / 10;
	TutorialButtons[kHowToPlayNext].x_origin =
		(float)CP_System_GetWindowWidth() / 1.12f;
	TutorialButtons[kHowToPlayNext].y_origin =
		(float)CP_System_GetWindowHeight() / 50;
	TutorialButtons[kHowToPlayNext].width =
		(float)CP_System_GetWindowWidth() / 10;
	TutorialButtons[kHowToPlayNext].height =
		(float)CP_System_GetWindowHeight() / 10;
}
//Initialize mouse object 
void InitMouse(void) {
	//out of the screen so it does not click anything at startup
	mouse_input.width = -1;
	mouse_input.height = -1;
	mouse_input.object_type = kObjectCircle;
	MouseReset();
}
void UpdateGameButtonPressed(void) {
	switch (CheckGameButtonPressed())
	{
	case kGameGrid:
		UpdateGameGridPress();
		break;
	case kPauseButton:
		current_game_state = current_game_state == kPause ? kWave : kPause;
		MouseReset();
		//Button SFX
		CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
		break;
	case kTurretButtonBasic:
		if (turret_purchasing[kTPPrice][kTBasic] <= level.phantom_quartz
			&& power_up_menu == kFalse) {
			is_placing_turret = kTBasic;
			turret_selected_to_upgrade = kNoTurretSelected;
			RenderImageFromSpriteSheet(basic_turret_spritesheet,
				basic_turret_spritesheet_array[0],
				CP_Input_GetMouseX(), CP_Input_GetMouseY(),
				game.grid_width, game.grid_height);
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (level.gold_quartz >= power_up_price.more_phantom_quartz
			&& power_up_menu == kTrue) {
			is_placing_turret = kTMax;
			turret_selected_to_upgrade = kNoTurretSelected;
			level.current_power_up_level.more_phantom_quartz += 1;
			level.gold_quartz -= power_up_price.more_phantom_quartz;
			MouseReset();
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
			}
		}
		else {
			is_placing_turret = kTMax;
			turret_selected_to_upgrade = kNoTurretSelected;
		}
		break;
	case kTurretButtonSlow:
		if (turret_purchasing[kTPPrice][kTSlow] <= level.phantom_quartz
			&& power_up_menu == kFalse) {
			is_placing_turret = kTSlow;
			turret_selected_to_upgrade = kNoTurretSelected;
			CP_Image_DrawAdvanced(
				game_menu_object[CheckGameButtonPressed()].image,
				CP_Input_GetMouseX(), CP_Input_GetMouseY(),
				game.grid_width, game.grid_height, 255, 0);
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (level.gold_quartz >= power_up_price.reduce_enemy_speed
			&& power_up_menu == kTrue) {
			is_placing_turret = kTMax;
			turret_selected_to_upgrade = kNoTurretSelected;
			level.current_power_up_level.reduce_enemy_speed += 1;
			level.gold_quartz -= power_up_price.reduce_enemy_speed;
			MouseReset();
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else {
			is_placing_turret = kTMax;
			turret_selected_to_upgrade = kNoTurretSelected;
		}
		break;
	case kTurretButtonHoming:
		if (turret_purchasing[kTPPrice][kTHoming] <= level.phantom_quartz
			&& power_up_menu == kFalse) {
			is_placing_turret = kTHoming;
			turret_selected_to_upgrade = kNoTurretSelected;
			RenderImageFromSpriteSheet(homing_missle_turret_spritesheet,
				homing_missle_turret_spritesheet_array[0],
				CP_Input_GetMouseX(), CP_Input_GetMouseY(),
				game.grid_width, game.grid_height);
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (level.gold_quartz >= power_up_price.reduce_enemy_health
			&& power_up_menu == kTrue) {
			is_placing_turret = kTMax;
			turret_selected_to_upgrade = kNoTurretSelected;
			level.current_power_up_level.reduce_enemy_health += 1;
			level.gold_quartz -= power_up_price.reduce_enemy_health;
			MouseReset();
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else {
			is_placing_turret = kTMax;
			turret_selected_to_upgrade = kNoTurretSelected;
		}
		break;
	case kTurretButtonMine:
		if (turret_purchasing[kTPPrice][kTMine] <= level.phantom_quartz
			&& power_up_menu == kFalse) {
			is_placing_turret = kTMine;
			turret_selected_to_upgrade = kNoTurretSelected;
			RenderImageFromSpriteSheet(mine_spritesheet,
				mine_spritesheet_array[0],
				CP_Input_GetMouseX(), CP_Input_GetMouseY(),
				game.grid_width, game.grid_height);
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (level.gold_quartz >= power_up_price.increased_mine_damage
			&& power_up_menu == kTrue) {
			is_placing_turret = kTMax;
			turret_selected_to_upgrade = kNoTurretSelected;
			level.current_power_up_level.increased_mine_damage += 1;
			level.gold_quartz -= power_up_price.increased_mine_damage;
			MouseReset();
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else {
			is_placing_turret = kTMax;
			turret_selected_to_upgrade = kNoTurretSelected;
		}
		break;
	case kSwapButton:
		is_placing_turret = kTMax;
		turret_selected_to_upgrade = kNoTurretSelected;
		power_up_menu = !power_up_menu;
		MouseReset();
		//Button SFX
		CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
			CP_SOUND_GROUP_0);
		break;
	case kGoldQuartzMenu:
		is_placing_turret = kTMax;
		turret_selected_to_upgrade = kNoTurretSelected;
		if (level.phantom_quartz >= 1000) {
			level.phantom_quartz -= 1000;
			level.gold_quartz += 10;
			//Button SFX 
			CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
				CP_SOUND_GROUP_0);
		}
		MouseReset();
		break;
	case kUpgradeMenu:
		is_placing_turret = kTMax;
		break;
	case kUpgradeButton:
		if (turret_selected_to_upgrade != kNoTurretSelected) {
			if (turret[turret_selected_to_upgrade].type != kTMine) {
				if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)
					&& level.phantom_quartz >=
					turret[turret_selected_to_upgrade].upgrade_price) {
					if (turret[turret_selected_to_upgrade].level < 10)
					{
						level.phantom_quartz -=
							turret[turret_selected_to_upgrade].upgrade_price;
						upgrade_turret(turret_selected_to_upgrade);
						//Button SFX
						if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
							CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
						}
					}
					is_placing_turret = kTMax;
				}
			}
			else {
				is_placing_turret = kTMax;
			}
		}
		else {
			is_placing_turret = kTMax;
			turret_selected_to_upgrade = kNoTurretSelected;
		}
		break;
	case kSellButton:
		if (turret_selected_to_upgrade != kNoTurretSelected) {
			int draw_x, draw_y;//The grids x and y position
			draw_x = (int)((turret[turret_selected_to_upgrade].data.x_origin
				- game.x_origin) / game.grid_width);
			draw_y = (int)((turret[turret_selected_to_upgrade].data.y_origin
				- game.y_origin) / game.grid_height);
			sell_turret(turret_selected_to_upgrade);
			level.grid[draw_y][draw_x].type = kClear;
			ResetPathFinding();
			CalculatePathFindingCost();
			UpdatePathFinding();
			MouseReset();
			is_placing_turret = kTMax;
			//Button SFX
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
			}
		}
		else {
			is_placing_turret = kTMax;
			turret_selected_to_upgrade = kNoTurretSelected;
		}
		break;
	default:
		is_placing_turret = kTMax;
		turret_selected_to_upgrade = kNoTurretSelected;
		break;
	}
}
void RenderEnvironment(void) {
	for (int i = 0; i < kMaxEnvironmentObject; i++) {
		//Only attempt to render initialized environment
		if (environment[i].image == grid_environment_objects_spritesheet)
		{
			RenderImageFromSpriteSheet(grid_environment_objects_spritesheet,
				grid_environment_objects_spritesheet_array
				[environment[i].object_type],
				environment[i].x_origin, environment[i].y_origin,
				environment[i].width, environment[i].height);
		}
	}
}
void RenderTurretDetailsDisplay(void) {
	//Check if a turretbutton or power up upgrade is hovered over
	//and display the details
	float temp_mouse_x = mouse_input.x_origin,
		temp_mouse_y = mouse_input.y_origin;
	mouse_input.x_origin = CP_Input_GetMouseX();
	mouse_input.y_origin = CP_Input_GetMouseY();
	//Making a Phantom click to check for collision.
	InitTurretDetailsDisplay(CheckGameButtonPressed());
	switch (CheckGameButtonPressed()) {
	case kTurretButtonBasic:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(game_menu_object[kTurretDetailsDisplay].x_origin,
			game_menu_object[kTurretDetailsDisplay].y_origin,
			game_menu_object[kTurretDetailsDisplay].width * 1.25f,
			game_menu_object[kTurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scaling_factor);
		if (power_up_menu == kFalse)
		{
			CP_Font_DrawText("Basic Turret",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.55f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("Single Target Damage",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.55f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}
		else
		{
			CP_Font_DrawText("Gem Upgrade",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.57f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("More Phantom Quartz Earned",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.57f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}
		break;
	case kTurretButtonSlow:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(game_menu_object[kTurretDetailsDisplay].x_origin,
			game_menu_object[kTurretDetailsDisplay].y_origin,
			game_menu_object[kTurretDetailsDisplay].width * 1.15f,
			game_menu_object[kTurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scaling_factor);
		if (power_up_menu == kFalse)
		{
			CP_Font_DrawText("Slow Turret",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("Slow Enemies",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}
		else
		{
			CP_Font_DrawText("Speed Upgrade",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("All ghosts move slower",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}
		break;
	case kTurretButtonHoming:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(game_menu_object[kTurretDetailsDisplay].x_origin,
			game_menu_object[kTurretDetailsDisplay].y_origin,
			game_menu_object[kTurretDetailsDisplay].width * 1.15f,
			game_menu_object[kTurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scaling_factor);
		if (power_up_menu == kFalse)
		{
			CP_Font_DrawText("Homing Turret",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("Splash Damage",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}
		else
		{
			CP_Font_DrawText("Health Upgrade",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("All ghosts have less HP",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}
		break;
	case kTurretButtonMine:
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(game_menu_object[kTurretDetailsDisplay].x_origin,
			game_menu_object[kTurretDetailsDisplay].y_origin,
			game_menu_object[kTurretDetailsDisplay].width * 1.15f,
			game_menu_object[kTurretDetailsDisplay].height);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Settings_TextSize(20.0f * scaling_factor);
		if (power_up_menu == kFalse)
		{
			CP_Font_DrawText("Mine",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("Single Use",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}
		else
		{
			CP_Font_DrawText("Mine Upgrade",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height / 3));
			CP_Font_DrawText("Mines deal more damage",
				game_menu_object[kTurretDetailsDisplay].x_origin
				+ game_menu_object[kTurretDetailsDisplay].width / 1.75f,
				(game_menu_object[kTurretDetailsDisplay].y_origin
					+ game_menu_object[kTurretDetailsDisplay].height * 2 / 3));
		}
		break;
	}
	//Restore original click data
	mouse_input.x_origin = temp_mouse_x;
	mouse_input.y_origin = temp_mouse_y;
}
void RenderGameGrid(void)
{
	int current_grid_row = 0, current_grid_col = 0;
	CP_Graphics_ClearBackground(COLOR_WHITE);
	CP_Settings_Fill(COLOR_BLACK);
	while (current_grid_row <= level_grid_rows) {
		CP_Graphics_DrawLine((game.x_origin),
			(game.y_origin + game.grid_height * current_grid_row),
			(game.x_origin + game.width),
			(game.y_origin + game.grid_height * current_grid_row));
		current_grid_row++;
	}
	while (current_grid_col <= level_grid_cols) {
		CP_Graphics_DrawLine(
			(game.x_origin + game.grid_width * current_grid_col),
			(game.y_origin),
			(game.x_origin + game.grid_width * current_grid_col),
			(game.y_origin + game.height));
		current_grid_col++;
	}
}
void RenderTurretMenuObjects
(Coordinates menu_object_to_print, MenuObjectType type) {
	char temp[100];//temporary storage for string
	if (turret_selected_to_upgrade == kNoTurretSelected
		&& (type == kUpgradeButton || type == kSellButton
			|| type == kUpgradeMenu))
	{
		// only render upgrade and sell button when turret selected
		//empty by design
	}
	else if (turret[turret_selected_to_upgrade].type == kTMine
		&& type == kUpgradeButton)
	{
		//Hide the upgrade menu for mine
		//empty by design
	}
	else {
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Graphics_DrawRect(menu_object_to_print.x_origin,
			menu_object_to_print.y_origin, menu_object_to_print.width,
			menu_object_to_print.height);
	}
	CP_Settings_TextSize(35.0f * scaling_factor);
	switch (type)
	{
	case kTurretButtonBasic:
		CP_Image_Draw(turret_button_background, menu_object_to_print.width / 2,
			(menu_object_to_print.y_origin + menu_object_to_print.height / 2),
			138 * scaling_factor, 144 * scaling_factor, 255);
		if (power_up_menu == kFalse) {
			RenderImageFromSpriteSheet(basic_turret_spritesheet,
				basic_turret_spritesheet_array[0],
				menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2),
				128 * scaling_factor, 128 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "%d",
				turret_purchasing[kTPPrice][kTBasic]);
			CP_Font_DrawText(temp, menu_object_to_print.width / 2.5f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet,
				currency_spritesheet_array[1],
				menu_object_to_print.width / 1.45f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3),
				45 * scaling_factor, 45 * scaling_factor);
		}
		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet,
				power_up_spritesheet_array[0], menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2),
				100 * scaling_factor, 100 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "Lv:%-2d",
				level.current_power_up_level.more_phantom_quartz);
			CP_Font_DrawText(temp, menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 7));
			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menu_object_to_print.width / 2.5f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet,
				currency_spritesheet_array[0],
				menu_object_to_print.width / 1.65f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3),
				45 * scaling_factor, 45 * scaling_factor);
		}
		break;
	case kTurretButtonSlow:
		CP_Image_Draw(turret_button_background, menu_object_to_print.width / 2,
			(menu_object_to_print.y_origin + menu_object_to_print.height / 2),
			138 * scaling_factor, 144 * scaling_factor, 255);
		if (power_up_menu == kFalse) {
			CP_Image_DrawAdvanced(menu_object_to_print.image,
				menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2), 128 * scaling_factor,
				128 * scaling_factor, 255, 90);
			sprintf_s(temp, sizeof(temp), "%d",
				turret_purchasing[kTPPrice][kTSlow]);
			CP_Font_DrawText(temp, menu_object_to_print.width / 2.5f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet,
				currency_spritesheet_array[1],
				menu_object_to_print.width / 1.45f,
				(menu_object_to_print.y_origin +
					menu_object_to_print.height / 2 + 128 / 3),
				45 * scaling_factor, 45 * scaling_factor);
		}
		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet,
				power_up_spritesheet_array[1], menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2), 100 * scaling_factor,
				100 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "Lv:%-2d",
				level.current_power_up_level.reduce_enemy_speed);
			CP_Font_DrawText(temp, menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 7));
			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menu_object_to_print.width / 2.5f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet,
				currency_spritesheet_array[0],
				menu_object_to_print.width / 1.65f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3),
				45 * scaling_factor, 45 * scaling_factor);
		}
		break;
	case  kTurretButtonHoming:
		CP_Image_Draw(turret_button_background, menu_object_to_print.width / 2,
			(menu_object_to_print.y_origin + menu_object_to_print.height / 2),
			138 * scaling_factor, 144 * scaling_factor, 255);
		if (power_up_menu == kFalse) {
			RenderImageFromSpriteSheet(homing_missle_turret_spritesheet,
				homing_missle_turret_spritesheet_array[0],
				menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2),
				128 * scaling_factor, 128 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "%d",
				turret_purchasing[kTPPrice][kTHoming]);
			CP_Font_DrawText(temp, menu_object_to_print.width / 2.5f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet,
				currency_spritesheet_array[1],
				menu_object_to_print.width / 1.45f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3),
				45 * scaling_factor, 45 * scaling_factor);
		}
		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet,
				power_up_spritesheet_array[2],
				menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2),
				100 * scaling_factor, 100 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "Lv:%-2d",
				level.current_power_up_level.reduce_enemy_health);
			CP_Font_DrawText(temp, menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 7));
			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menu_object_to_print.width / 2.5f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet,
				currency_spritesheet_array[0],
				menu_object_to_print.width / 1.65f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3),
				45 * scaling_factor, 45 * scaling_factor);
		}
		break;
	case kTurretButtonMine:
		CP_Image_Draw(turret_button_background, menu_object_to_print.width / 2,
			(menu_object_to_print.y_origin + menu_object_to_print.height / 2),
			138 * scaling_factor, 144 * scaling_factor, 255);
		if (power_up_menu == kFalse) {
			RenderImageFromSpriteSheet(mine_spritesheet,
				mine_spritesheet_array[0], menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2),
				128 * scaling_factor, 128 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "%d",
				turret_purchasing[kTPPrice][kTMine]);
			CP_Font_DrawText(temp, menu_object_to_print.width / 2.5f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet,
				currency_spritesheet_array[1],
				menu_object_to_print.width / 1.45f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3),
				45 * scaling_factor, 45 * scaling_factor);
		}
		else
		{
			RenderImageFromSpriteSheet(power_up_spritesheet,
				power_up_spritesheet_array[3],
				menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2),
				100 * scaling_factor, 100 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "Lv:%-2d",
				level.current_power_up_level.increased_mine_damage);
			CP_Font_DrawText(temp, menu_object_to_print.width / 2,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 7));
			sprintf_s(temp, sizeof(temp), "10");
			CP_Font_DrawText(temp, menu_object_to_print.width / 2.5f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3));
			RenderImageFromSpriteSheet(currency_spritesheet,
				currency_spritesheet_array[0],
				menu_object_to_print.width / 1.65f,
				(menu_object_to_print.y_origin
					+ menu_object_to_print.height / 2 + 128 / 3),
				45 * scaling_factor, 45 * scaling_factor);
		}
		break;
	case kPauseButton:
		CP_Image_DrawAdvanced(menu_object_to_print.image,
			menu_object_to_print.width / 2,
			(menu_object_to_print.y_origin + menu_object_to_print.height / 2),
			64, 64, 255, 0);
		break;
	case kSwapButton:
		RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet,
			interactable_UI_buttons_spritesheet_array[2],
			menu_object_to_print.x_origin + menu_object_to_print.width / 2,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			137, 72);
		break;
	case kGoldQuartzMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scaling_factor);
		CP_Image_Draw(thin_UI_background, menu_object_to_print.x_origin
			+ menu_object_to_print.width / 2,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			138 * scaling_factor, 46 * scaling_factor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet,
			currency_spritesheet_array[0],
			menu_object_to_print.x_origin + menu_object_to_print.width / 5.5f,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			30 * scaling_factor, 30 * scaling_factor);
		RenderImageFromSpriteSheet(currency_spritesheet,
			currency_spritesheet_array[4],
			menu_object_to_print.x_origin + menu_object_to_print.width / 1.3f,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			28 * scaling_factor, 28 * scaling_factor);
		sprintf_s(temp, 100, "x%-10d", level.gold_quartz);
		CP_Font_DrawText(temp, menu_object_to_print.x_origin
			+ menu_object_to_print.width / 1.55f,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2);
		break;
	case kPhantomQuartzMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scaling_factor);
		CP_Image_Draw(thin_UI_background,
			menu_object_to_print.x_origin + menu_object_to_print.width / 2,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			138 * scaling_factor, 46 * scaling_factor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet,
			currency_spritesheet_array[1],
			menu_object_to_print.x_origin + menu_object_to_print.width / 5,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			30 * scaling_factor, 30 * scaling_factor);
		sprintf_s(temp, 100, "x%-10d", level.phantom_quartz);
		CP_Font_DrawText(temp, menu_object_to_print.x_origin
			+ menu_object_to_print.width / 1.45f,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2);
		break;
	case kHealthMenu:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scaling_factor);
		sprintf_s(temp, 100, "x%-10d", level.health);
		CP_Image_Draw(thin_UI_background, menu_object_to_print.x_origin
			+ menu_object_to_print.width / 2,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			138 * scaling_factor, 46 * scaling_factor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet,
			currency_spritesheet_array[2],
			menu_object_to_print.x_origin + menu_object_to_print.width / 5,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			30 * scaling_factor, 30 * scaling_factor);
		CP_Font_DrawText(temp, menu_object_to_print.x_origin
			+ menu_object_to_print.width / 1.45f,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2);
		break;
	case kWaveDisplay:
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(25.0f * scaling_factor);
		sprintf_s(temp, 100, "%2d/%d", level.current_wave + 1,
			kMaxNumberOfWave);
		CP_Image_Draw(thin_UI_background,
			menu_object_to_print.x_origin + menu_object_to_print.width / 2,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			138 * scaling_factor, 46 * scaling_factor, 255);
		RenderImageFromSpriteSheet(currency_spritesheet,
			currency_spritesheet_array[3],
			menu_object_to_print.x_origin + menu_object_to_print.width / 5,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			30 * scaling_factor, 30 * scaling_factor);
		CP_Font_DrawText(temp, menu_object_to_print.x_origin
			+ menu_object_to_print.width / 2.5f,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2);
		break;
	case kEnvironmentalEffects:
		RenderImageFromSpriteSheet(fat_UI_background_spritesheet,
			fat_UI_background_spritesheet_array[0],
			menu_object_to_print.x_origin + menu_object_to_print.width / 2,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			138 * scaling_factor, 142 * scaling_factor);
		CP_Settings_TextSize(35.0f * scaling_factor);
		CP_Settings_Fill(COLOR_WHITE);
		sprintf_s(temp, sizeof(temp), "Effects");
		CP_Font_DrawText(temp, menu_object_to_print.x_origin
			+ menu_object_to_print.width / 2,
			menu_object_to_print.y_origin + menu_object_to_print.height / 5);
		printf("%d", level.current_effect);
		RenderImageFromSpriteSheet(battlefield_effect_spritesheet,
			battlefield_effect_spritesheet_array[level.current_effect],
			menu_object_to_print.x_origin + menu_object_to_print.width / 2,
			menu_object_to_print.y_origin
			+ menu_object_to_print.height / 1.65f,
			85 * scaling_factor, 85 * scaling_factor);
		break;
	case kMonsterRemainingDisplay:
		RenderImageFromSpriteSheet(fat_UI_background_spritesheet,
			fat_UI_background_spritesheet_array[0],
			menu_object_to_print.x_origin + menu_object_to_print.width / 2,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2,
			138 * scaling_factor, 142 * scaling_factor);
		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(35.0f * scaling_factor);
		sprintf_s(temp, sizeof(temp), "Enemies");
		CP_Font_DrawText(temp,
			menu_object_to_print.x_origin + menu_object_to_print.width / 2,
			menu_object_to_print.y_origin + menu_object_to_print.height / 5);
		int total_enemies = 0;
		for (int i = 0; i < kMaxEnemyType; i++) {
			total_enemies += level.wave_enemies[level.current_wave][i];
		}
		sprintf_s(temp, sizeof(temp), "%d/%d", enemies_left, total_enemies);
		CP_Font_DrawText(temp,
			menu_object_to_print.x_origin + menu_object_to_print.width / 2,
			menu_object_to_print.y_origin + menu_object_to_print.height / 2);
		break;
	case kUpgradeMenu:
		if (turret_selected_to_upgrade != kNoTurretSelected) {
			//Only render when upgrading
			CP_Image_Draw(upgrade_menu_background,
				menu_object_to_print.x_origin + menu_object_to_print.width / 2,
				menu_object_to_print.y_origin
				+ menu_object_to_print.height / 2.6f,
				275 * scaling_factor, 480 * scaling_factor, 255);
			CP_Settings_Fill(COLOR_WHITE);
			CP_Settings_TextSize(50.0f * scaling_factor);
			//Level Icon
			RenderImageFromSpriteSheet(turret_stats_spritesheet,
				turret_stats_spritesheet_array[0],
				menu_object_to_print.x_origin
				+ menu_object_to_print.width / 1.75f,
				menu_object_to_print.y_origin
				+ menu_object_to_print.height / 5.75f,
				64 * scaling_factor, 64 * scaling_factor);
			if (turret[turret_selected_to_upgrade].level == 10)
			{
				//Print Level Max at Level 10
				sprintf_s(temp, sizeof(temp), "MAX");
			}
			else
			{
				sprintf_s(temp, sizeof(temp), "%-3d",
					turret[turret_selected_to_upgrade].level);
			}
			CP_Font_DrawText(temp,
				menu_object_to_print.x_origin
				+ menu_object_to_print.width / 1.2f,
				menu_object_to_print.y_origin
				+ menu_object_to_print.height / 5.75f);
			switch (turret[turret_selected_to_upgrade].type)
			{
				//Data to display for each type of turret selected
			case kTBasic:
				sprintf_s(temp, sizeof(temp), "Basic");
				CP_Font_DrawText(temp,
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 2,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 18);
				RenderImageFromSpriteSheet(basic_turret_spritesheet,
					basic_turret_spritesheet_array[0],
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 4,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 5.75f,
					110 * scaling_factor, 110 * scaling_factor);
				RenderImageFromSpriteSheet(turret_stats_spritesheet,
					turret_stats_spritesheet_array[1],
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 5,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 3.25f,
					64 * scaling_factor, 64 * scaling_factor);
				sprintf_s(temp, sizeof(temp), "%.2f",
					turret[turret_selected_to_upgrade].mod.damage);
				CP_Font_DrawText(temp, menu_object_to_print.x_origin
					+ menu_object_to_print.width / 1.95f,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 3.25f);
				break;
			case kTSlow:
				sprintf_s(temp, sizeof(temp), "Slow");
				CP_Font_DrawText(temp,
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 2,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 18);
				CP_Image_DrawAdvanced(slow_turret_image_array[0],
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 4,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 5.75f,
					110 * scaling_factor, 110 * scaling_factor, 255, 90);
				RenderImageFromSpriteSheet(turret_stats_spritesheet,
					turret_stats_spritesheet_array[2],
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 5,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 3.25f,
					64 * scaling_factor, 64 * scaling_factor);
				sprintf_s(temp, sizeof(temp), "%.2f",
					turret[turret_selected_to_upgrade].mod.slow_amt);
				CP_Font_DrawText(temp, menu_object_to_print.x_origin
					+ menu_object_to_print.width / 1.95f,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 3.25f);
				break;
			case kTHoming:
				sprintf_s(temp, sizeof(temp), "Homing");
				CP_Font_DrawText(temp,
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 2,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 18);
				RenderImageFromSpriteSheet(homing_missle_turret_spritesheet,
					homing_missle_turret_spritesheet_array[0],
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 4,
					menu_object_to_print.y_origin +
					menu_object_to_print.height / 5.85f,
					110 * scaling_factor, 110 * scaling_factor);
				RenderImageFromSpriteSheet(turret_stats_spritesheet,
					turret_stats_spritesheet_array[1],
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 5,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 3.25f,
					64 * scaling_factor, 64 * scaling_factor);
				sprintf_s(temp, sizeof(temp), "%.2f",
					turret[turret_selected_to_upgrade].mod.damage);
				CP_Font_DrawText(temp,
					menu_object_to_print.x_origin +
					menu_object_to_print.width / 1.95f,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 3.25f);
				break;
			case kTMine:
				sprintf_s(temp, sizeof(temp), "Mine");
				CP_Font_DrawText(temp,
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 2,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 18);
				RenderImageFromSpriteSheet(mine_spritesheet,
					mine_spritesheet_array[0],
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 4,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 5.75f,
					110 * scaling_factor, 110 * scaling_factor);
				RenderImageFromSpriteSheet(turret_stats_spritesheet,
					turret_stats_spritesheet_array[1],
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 5,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 3.25f,
					64 * scaling_factor, 64 * scaling_factor);
				sprintf_s(temp, sizeof(temp), "%.2f",
					turret[turret_selected_to_upgrade].mod.damage);
				CP_Font_DrawText(temp,
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 1.95f,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 3.25f);
				break;
			}
			//Range Icon
			RenderImageFromSpriteSheet(turret_stats_spritesheet,
				turret_stats_spritesheet_array[3],
				menu_object_to_print.x_origin + menu_object_to_print.width / 5,
				menu_object_to_print.y_origin
				+ menu_object_to_print.height / 2.44f,
				64 * scaling_factor, 64 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "%-5.2f",
				turret[turret_selected_to_upgrade].mod.range);
			CP_Font_DrawText(temp,
				menu_object_to_print.x_origin
				+ menu_object_to_print.width / 1.7f,
				menu_object_to_print.y_origin
				+ menu_object_to_print.height / 2.44f);
			//Attack Speed Icon
			RenderImageFromSpriteSheet(turret_stats_spritesheet,
				turret_stats_spritesheet_array[4],
				menu_object_to_print.x_origin + menu_object_to_print.width / 5,
				menu_object_to_print.y_origin
				+ menu_object_to_print.height / 1.95f,
				64 * scaling_factor, 64 * scaling_factor);
			sprintf_s(temp, sizeof(temp), "%.2f",
				turret[turret_selected_to_upgrade].mod.shoot_rate);
			CP_Font_DrawText(temp, menu_object_to_print.x_origin
				+ menu_object_to_print.width / 1.95f,
				menu_object_to_print.y_origin
				+ menu_object_to_print.height / 1.95f);
			CP_Settings_TextSize(35.0f * scaling_factor);
			//Upgrade Price
			if (turret[turret_selected_to_upgrade].level == 10)
			{
				//Set upgrade price to Max at Max level
				sprintf_s(temp, sizeof(temp), "MAX");
				CP_Font_DrawText(temp,
					menu_object_to_print.x_origin
					+ menu_object_to_print.width / 3.75f,
					menu_object_to_print.y_origin
					+ menu_object_to_print.height / 1.65f);
			}
			else
			{
				if (turret[turret_selected_to_upgrade].type != kTMine) {
					//No upgrade price for mines
					sprintf_s(temp, sizeof(temp), "%4d",
						turret[turret_selected_to_upgrade].upgrade_price);
					CP_Font_DrawText(temp,
						menu_object_to_print.x_origin
						+ menu_object_to_print.width / 5.5f,
						menu_object_to_print.y_origin
						+ menu_object_to_print.height / 1.65f);
					RenderImageFromSpriteSheet(currency_spritesheet,
						currency_spritesheet_array[1],
						menu_object_to_print.x_origin
						+ menu_object_to_print.width / 2.95f,
						menu_object_to_print.y_origin
						+ menu_object_to_print.height / 1.65f,
						40 * scaling_factor, 40 * scaling_factor);
				}
			}
			//Sell Price
			sprintf_s(temp, sizeof(temp), "%4d",
				turret[turret_selected_to_upgrade].sell_price);
			CP_Font_DrawText(temp,
				menu_object_to_print.x_origin
				+ menu_object_to_print.width / 1.45f,
				menu_object_to_print.y_origin
				+ menu_object_to_print.height / 1.65f);
			RenderImageFromSpriteSheet(currency_spritesheet,
				currency_spritesheet_array[1],
				menu_object_to_print.x_origin
				+ menu_object_to_print.width / 1.15f,
				menu_object_to_print.y_origin
				+ menu_object_to_print.height / 1.65f,
				40 * scaling_factor, 40 * scaling_factor);
		}
		break;
	case kUpgradeButton:
		if (turret_selected_to_upgrade != kNoTurretSelected
			&& turret[turret_selected_to_upgrade].type != kTMine)
		{ //Only render when upgrading or when mine is not selected
			RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet,
				interactable_UI_buttons_spritesheet_array[1],
				menu_object_to_print.x_origin + menu_object_to_print.width / 2,
				menu_object_to_print.y_origin
				+ menu_object_to_print.height / 2,
				95 * scaling_factor, 72 * scaling_factor);
		}
		break;
	case kSellButton:
		if (turret_selected_to_upgrade != kNoTurretSelected)
		{//Only render when upgrading
			RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet,
				interactable_UI_buttons_spritesheet_array[0],
				menu_object_to_print.x_origin + menu_object_to_print.width / 2,
				menu_object_to_print.y_origin
				+ menu_object_to_print.height / 2,
				95 * scaling_factor, 72 * scaling_factor);
		}
		break;
	}
}
//Render the path with a specific color according to grid type
void RenderEnemyPath(void) {
	for (int i = 0; i < level_grid_rows; i++) {
		for (int j = 0; j < level_grid_cols; j++) {
			if (level.grid[i][j].type == kPath) {
				ColorGameSquare(i, j, COLOR_RED);
			}
			else if (level.grid[i][j].type == kSpawn) {
				ColorGameSquare(i, j, COLOR_RED);
				portal_variables_array[1].portal_x_coordinate =
					((game.x_origin + game.grid_width * j) +
						(game.x_origin + game.grid_width * (j + 1))) / 2;
				portal_variables_array[1].portal_y_coordinate =
					((game.y_origin + game.grid_height * i) +
						(game.y_origin + game.grid_height * (i + 1))) / 2;
				portal_variables_array[1].portal_image_width =
					game.grid_width;
				portal_variables_array[1].portal_image_height =
					game.grid_height;
			}
			else if (level.grid[i][j].type == kExit) {
				ColorGameSquare(i, j, COLOR_BLUE);
				portal_variables_array[0].portal_x_coordinate =
					((game.x_origin + game.grid_width * j) +
						(game.x_origin + game.grid_width * (j + 1))) / 2;
				portal_variables_array[0].portal_y_coordinate =
					((game.y_origin + game.grid_height * i) +
						(game.y_origin + game.grid_height * (i + 1))) / 2;
				portal_variables_array[0].portal_image_width =
					game.grid_width;
				portal_variables_array[0].portal_image_height =
					game.grid_height;
			}
		}
	}
}
void RenderHowToPlayPages(void) {
	CP_Graphics_ClearBackground(COLOR_WHITE);
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Image_Draw(tutorial_image_array[current_how_to_play_page],
		CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		1280 * scaling_factor, 720 * scaling_factor, 255);
	if (current_how_to_play_page != 0) {
		CP_Graphics_DrawRect(TutorialButtons[kHowToPlayPrevious].x_origin,
			TutorialButtons[kHowToPlayPrevious].y_origin,
			TutorialButtons[kHowToPlayPrevious].width,
			TutorialButtons[kHowToPlayPrevious].height);
		RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet,
			interactable_UI_buttons_spritesheet_array[4],
			TutorialButtons[kHowToPlayPrevious].x_origin 
			+ TutorialButtons[kHowToPlayPrevious].width / 2,
			TutorialButtons[kHowToPlayPrevious].y_origin 
			+ TutorialButtons[kHowToPlayPrevious].height / 2,
			TutorialButtons[kHowToPlayPrevious].width,
			TutorialButtons[kHowToPlayPrevious].height);
	}
	else
	{
		CP_Graphics_DrawRect(TutorialButtons[kHowToPlayBack].x_origin,
			TutorialButtons[kHowToPlayBack].y_origin,
			TutorialButtons[kHowToPlayBack].width,
			TutorialButtons[kHowToPlayBack].height);
		RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet, 
			interactable_UI_buttons_spritesheet_array[5],
			TutorialButtons[kHowToPlayBack].x_origin 
			+ TutorialButtons[kHowToPlayBack].width / 2,
			TutorialButtons[kHowToPlayBack].y_origin 
			+ TutorialButtons[kHowToPlayBack].height / 2,
			TutorialButtons[kHowToPlayBack].width,
			TutorialButtons[kHowToPlayBack].height);
	}
	if (current_how_to_play_page != kMaxHowToPlayPages - 1) {
		CP_Graphics_DrawRect(TutorialButtons[kHowToPlayNext].x_origin,
			TutorialButtons[kHowToPlayNext].y_origin,
			TutorialButtons[kHowToPlayNext].width,
			TutorialButtons[kHowToPlayNext].height);
		RenderImageFromSpriteSheet(interactable_UI_buttons_spritesheet,
			interactable_UI_buttons_spritesheet_array[3],
			TutorialButtons[kHowToPlayNext].x_origin 
			+ TutorialButtons[kHowToPlayNext].width / 2,
			TutorialButtons[kHowToPlayNext].y_origin 
			+ TutorialButtons[kHowToPlayNext].height / 2,
			TutorialButtons[kHowToPlayNext].width,
			TutorialButtons[kHowToPlayNext].height);
	}
	if (BtnIsPressed(TutorialButtons[kHowToPlayBack]) 
		&& current_how_to_play_page == 0) {
		//Return to main menu when back button is clicked
		current_how_to_play_page = 0;
		current_game_state = kMainMenu;
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
			CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
				CP_SOUND_GROUP_0);
		}
	}
	else if (BtnIsPressed(TutorialButtons[kHowToPlayPrevious])) {
		if (current_how_to_play_page != 0) {
			current_how_to_play_page -= 1;
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
	}
	else if (BtnIsPressed(TutorialButtons[kHowToPlayNext])) {
		if (current_how_to_play_page != kMaxHowToPlayPages - 1) {
			current_how_to_play_page += 1;
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(ButtonClickSFX, SFX_Volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
	}
	MouseReset();
}