#include "cprocessing.h"
#include"game.h"
#include "Samuel.h"
#include "John.h"
#include"Anderson.h"
#include "Gabriel.h"

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


void game_init(void)
{
	init_all_images();

	//CP_System_ShowConsole(); //pls dont delete this cause scrub me uses printf to debug -gabriel
	//CP_System_Fullscreen();
	CP_System_SetWindowSize(1280, 780);
	currentGameState = MainMenu;
	
	//Main menu, level select
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
	isPlacingTurret = FALSE;

	//Level Data
	Tutorial.spawnRow = 0;
	Tutorial.spawnCol = (GAME_GRID_COLS - 1) / 2;
	Tutorial.exitRow = GAME_GRID_ROWS - 1;
	Tutorial.exitCol = (GAME_GRID_COLS - 1) / 2;

	game_grid_color_init(&Tutorial);
	turret_init();
	enemy_test_init();
	Enemies_init();
	//Enemies_spawn();
}

void game_update(void)
{
	//Input
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
		MouseInput.xOrigin = CP_Input_GetMouseX();
		MouseInput.yOrigin = CP_Input_GetMouseY();
	}


	if (currentGameState == Wave)
	{
		//do enemy update first
		//enemy_move(&test, Xarray, Yarray, 2);
		update_enemy();
		EnemyDeath(&test);

		//do turret & projectile update next
		update_turret();
		update_projectile();

		//render all the stuff
		render_game_grid();
		render_game_grid_color(Tutorial);
		render_turret_menu();

		render_button(GameButton[4], COLOR_PURPLE);
		render_button(GameButton[0], COLOR_WHITE);
		render_button(GameButton[1], COLOR_WHITE);
		render_button(GameButton[2], COLOR_WHITE);
		render_button(GameButton[3], COLOR_WHITE);

		render_turret();
		render_projectile();

		render_bullet_circles();

		render_button_pressed();
		render_new_turret();

		//test enemy
		Draw_enemy(&test);
		draw_multiple_enemies();
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
	else if (currentGameState == Pause) {
		if (check_game_button_pressed() == PauseButton) {
			if (currentGameState == Pause) {
				currentGameState = Wave;
				MouseInput.xOrigin = (float)CP_System_GetWindowWidth() / 2;
				MouseInput.yOrigin = (float)CP_System_GetWindowHeight() / 2;
			}
			else {
				currentGameState = Pause;
				MouseInput.xOrigin = (float)CP_System_GetWindowWidth() / 2;
				MouseInput.yOrigin = (float)CP_System_GetWindowHeight() / 2;
			}
		}
	}
}
void game_exit(void)
{

}