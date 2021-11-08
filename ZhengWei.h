#pragma once
#include"game.h"

// Max Number of Turret Type
#define NOT_PLACING_TURRET 4

void game_grid_init(void);
void pathfinding_init(LevelData* LevelX);
void pause_button_init(void);
void turret_basic_button_init(void);
void turret_slow_button_init(void);
void turret_homing_button_init(void);
void turret_mine_button_init(void);
void mouse_init(void);
void environment_init(LevelData* LevelX);
void phantomQuartz_init(void);
void goldQuartz_init(void);
void health_init(void);
void menu_swap_init(void);
void wave_number_display_init(void);
void battlefield_effects_display_init(void);
void monster_remaining_display_init(void);
void upgrade_button_init(void);
void sell_button_init(void);
void level1_init(void);

void pathfinding_reset(LevelData* LevelX);
void pathfinding_calculate_cost(LevelData* LevelX);
void render_environment(void);
void render_game_grid(void);
void render_path(LevelData* LevelX);
void render_button_pressed(void);
void pathfinding_update(LevelData* LevelX);

int isPlacingTurret;
int isUpgradingTurret;
int powerUpMenu;