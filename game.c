#include "cprocessing.h"
#include"game.h"
#include "Samuel.h"
#include "John.h"
#include"Anderson.h"

void game_grid_init(void);
void game_grid_color_init(LevelData* Level);
void turret_menu_init(void);
void pause_button_init(void);
void turret0_button_init(void);
void turret1_button_init(void);
void turret2_button_init(void);
void turret3_button_init(void);
void mouse_init(void);
int btn_is_pressed(Coordinates objectButtonX);
void render_game_grid(void);
void render_game_grid_color(LevelData Level);
void render_turret_menu(void);
void render_button(Coordinates TurretButtonX, CP_Color Color);

void game_init(void)
{
	//CP_System_Fullscreen();
	currentGameState = Wave;

	init_play_button();
	init_quit_button();
	init_level_select_buttons();
	init_back_button();

	//game grid 
	game_grid_init();

	//turret menu
	turret_menu_init();

	//buttons
	turret0_button_init();
	turret1_button_init();
	turret2_button_init();
	turret3_button_init();
	pause_button_init();

	//Initialize Objects
	mouse_init();

	//Level Data
	Tutorial.spawnRow = 0;
	Tutorial.spawnCol = (GAME_GRID_COLS - 1) / 2;
	Tutorial.exitRow = GAME_GRID_ROWS - 1;
	Tutorial.exitCol = (GAME_GRID_COLS - 1) / 2;

	game_grid_color_init(&Tutorial);
	turret_init();
	enemy_test_init();
}

void game_update(void)
{
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
		MouseInput.xOrigin = CP_Input_GetMouseX();
		MouseInput.yOrigin = CP_Input_GetMouseY();
	}
	if (currentGameState == Wave)
	{
		update_turret();
		update_projectile();

		render_game_grid();
		render_game_grid_color(Tutorial);
		render_turret_menu();

		render_button(PauseButton, COLOR_PURPLE);
		render_button(TurretButton0, COLOR_WHITE);
		render_button(TurretButton1, COLOR_WHITE);
		render_button(TurretButton2, COLOR_WHITE);
		render_button(TurretButton3, COLOR_WHITE);

		render_turret();
		render_projectile();


		//test enemy
		enemy_move(&test, Xarray, Yarray, 2);
		Draw_enemy(&test);
		EnemyDeath(&test);
	}
	else if (currentGameState == MainMenu)
	{
		if (btn_is_pressed(PlayButton.buttonData))
		{
			currentGameState = LevelSelect;
		}
		else if (btn_is_pressed(QuitButton.buttonData))
		{
			exit_game();
		}

		CP_Graphics_ClearBackground(COLOR_GREY);
		render_game_title();
		render_ui_button(PlayButton);
		render_ui_button(QuitButton);
	}
	else if (currentGameState == LevelSelect)
	{
		// Level 1
		if (btn_is_pressed(levelButtons[0].buttonData))
		{
			currentGameState = Wave;
		}
		else if (btn_is_pressed(BackButton.buttonData))
		{
			currentGameState = MainMenu;
		}

		CP_Graphics_ClearBackground(COLOR_GREY);
		render_game_title();
		render_level_select_buttons();
		render_ui_button(BackButton);
	}
}
void game_exit(void)
{

}