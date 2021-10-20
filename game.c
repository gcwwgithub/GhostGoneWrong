#include "cprocessing.h"
#include"game.h"
#include "Samuel.h"
#include "John.h"
void game_grid_init(void);
void game_grid_color_init(LevelData* Level);
void turret_menu_init(void);
void turret0_button_init(void);
void turret1_button_init(void);
void turret2_button_init(void);
void turret3_button_init(void);

void render_game_grid(void);
void render_game_grid_color(LevelData Level);
void render_turret_menu(void);
void render_turret_button(Coordinates TurretButtonX, ObjectData ObjectTurretButtonX);

void game_init(void)
{
	//CP_System_Fullscreen();
	currentGameState = MainMenu;

	//game grid 
	game_grid_init();
	
	//turret menu
	turret_menu_init();

	//turret buttons
	turret0_button_init();
	turret1_button_init();
	turret2_button_init();
	turret3_button_init();

	//Initialize Objects
	object_init();
	turret_init();
	enemy_test_init();

	//Level Data
	Tutorial.spawnRow = 0;
	Tutorial.spawnCol = (GAME_GRID_COLS - 1) / 2;
	Tutorial.exitRow = GAME_GRID_ROWS - 1;
	Tutorial.exitCol = (GAME_GRID_COLS - 1) / 2;

	game_grid_color_init(&Tutorial);
}

void game_update(void)
{
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
		MouseInput.objectPositionX = CP_Input_GetMouseX();
		MouseInput.objectPositionY = CP_Input_GetMouseY();
	}

	update_turret();
	update_projectile();

	render_game_grid();
	render_game_grid_color(Tutorial);
	render_turret_menu();

	render_turret_button(TurretButton0, ObjectTurretButton0);
	render_turret_button(TurretButton1, ObjectTurretButton1);
	render_turret_button(TurretButton2, ObjectTurretButton2);
	render_turret_button(TurretButton3, ObjectTurretButton3);

	render_turret();
	render_projectile();

	//test enemy
	enemy_move(&test, Xarray, Yarray, 2);
	Draw_enemy(&test);
	EnemyDeath(&test);
	//printf("%d %f ", count,test.health);
}
void game_exit(void)
{

}