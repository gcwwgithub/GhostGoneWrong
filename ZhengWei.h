#pragma once
#include"game.h"

// Max Number of Turret Type
void mouse_init(void);
void environment_init(LevelData* LevelX);
void powerup_price_init(void);
void level1_init(void);
void level2_init(void);
void level3_init(void);
void level4_init(void);
void level5_init(void);

void pathfinding_reset(LevelData* LevelX);
void pathfinding_calculate_cost(LevelData* LevelX);
void render_environment(void);
void render_game_grid(void);
void render_path(LevelData* LevelX);
void render_button_pressed(void);
void pathfinding_update(LevelData* LevelX);

int isPlacingTurret;
int powerUpMenu;

#define COLOR_BLUE CP_Color_Create(0, 0, 255, 255)

#define LEVEL1_COLS 6
#define LEVEL1_ROWS 7
#define LEVEL2_COLS 6
#define LEVEL2_ROWS 7
#define LEVEL3_COLS 6
#define LEVEL3_ROWS 7
#define LEVEL4_COLS 6
#define LEVEL4_ROWS 7
#define LEVEL5_COLS 8
#define LEVEL5_ROWS 7

#define LEFT_MENU_X_END 140.714294f
#define RIGHT_MENU_X_START 998.571411f

//Environment
#define MAX_ENVIRONMENT_OBJECT 10
Coordinates Environment[MAX_ENVIRONMENT_OBJECT];