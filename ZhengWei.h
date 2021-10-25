#pragma once
#include"game.h"

void game_grid_init(void);
void game_grid_color_init(LevelData* Level);
void turret_menu_init(void);
void pause_button_init(void);
void turret0_button_init(void);
void turret1_button_init(void);
void turret2_button_init(void);
void turret3_button_init(void);
void mouse_init(void);

void render_game_grid(void);
void render_game_grid_color(LevelData Level);
void render_turret_menu(void);
void render_button_pressed(void);
void render_new_turret(void);

int isPlacingTurret;