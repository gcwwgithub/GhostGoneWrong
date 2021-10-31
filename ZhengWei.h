#pragma once
#include"game.h"

// Max Number of Turret Type
#define NOT_PLACING_TURRET 4

void game_grid_init(void);
void pathfinding_init(LevelData* LevelX);
void turret_menu_init(void);
void pause_button_init(void);
void turret_triangle_button_init(void);
void turret_circle_button_init(void);
void turret_star_button_init(void);
void turret_percentage_button_init(void);
void mouse_init(void);
void environment_init(LevelData* LevelX);
void cash1_init(void);
void cash2_init(void);
void health_init(void);

void pathfinding_reset(LevelData* LevelX);
void pathfinding_calculate_cost(LevelData* LevelX);

void render_environment(void);
void render_game_grid(void);
void render_path(LevelData* LevelX);
void render_turret_menu(void);
void render_button_pressed(void);
void render_new_turret(LevelData* LevelX);
void pathfinding_update(LevelData* LevelX);

int isPlacingTurret;