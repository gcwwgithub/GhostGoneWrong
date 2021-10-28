#include "cprocessing.h"
#include"game.h"
#include "Samuel.h"
#include "John.h"
#include"Anderson.h"
#include "Gabriel.h"
#include"ZhengWei.h"

void game_init(void)
{
	
	//CP_System_ShowConsole(); //pls dont delete this cause scrub me uses printf to debug -gabriel
	//CP_System_Fullscreen();
	CP_System_SetWindowSize(1280, 780);

	init_all_images();
	SpriteSheetInit();

	currentGameState = MainMenu;
	
	//Main menu, level select
	init_play_button();
	init_quit_button();
	init_level_select_buttons();
	init_back_button();

	init_pause_screen();


	//game grid 
	game_grid_init();

	//turret menu
	turret_menu_init();

	//buttons
	turret_triangle_button_init();
	turret_circle_button_init();
	turret_star_button_init();
	turret_percentage_button_init();
	pause_button_init();

	//Initialize Objects
	mouse_init();
	isPlacingTurret = NOT_PLACING_TURRET;

	//Level Data
	Tutorial.spawnRow = 0;
	Tutorial.spawnCol = (GAME_GRID_COLS - 1) / 2;
	Tutorial.exitRow = GAME_GRID_ROWS - 1;
	Tutorial.exitCol = (GAME_GRID_COLS - 1) / 2;
	
	pathfinding_init(&Tutorial);

	turret_init();
	enemy_test_init();
	Enemies_init(2,2,2);

	pathfinding_calculate_cost(&Tutorial);
	pathfinding_update(&Tutorial);
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

		//do turret & projectile update next
		update_turret();
		update_projectile();

		//render all the stuff
		render_game_grid();
		render_path(&Tutorial);
		render_turret_menu();

		render_button(GameButton[PauseButton], COLOR_PURPLE);
		render_button(GameButton[TurretButtonTriangle], COLOR_WHITE);
		render_button(GameButton[TurretButtonCircle], COLOR_WHITE);
		render_button(GameButton[TurretButtonStar], COLOR_WHITE);
		render_button(GameButton[TurretButtonPercentage], COLOR_WHITE);

		draw_multiple_enemies();

		render_turret();
		render_projectile();

		render_bullet_circles();

		render_new_turret(&Tutorial);
		render_button_pressed();//Must be after render_new_turret

		//test enemy


		UpdatePortal();
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
	else if (currentGameState == Pause) 
	{
		if (check_game_button_pressed() == PauseButton) 
		{
			if (currentGameState == Pause)
			{
				currentGameState = Wave;
			}
			else // if the game is not paused
			{
				currentGameState = Pause;
			}
			MouseInput.xOrigin = (float)CP_System_GetWindowWidth() / 2;
			MouseInput.yOrigin = (float)CP_System_GetWindowHeight() / 2;
		}
		else if (btn_is_pressed(PauseBackButton.buttonData))
		{
			currentGameState = MainMenu;
		}
		else if (btn_is_pressed(PauseQuitButton.buttonData))
		{
			exit_game(); // placing a button in the bottom half of the screen automatically activates it.
		}
		render_pause_screen();
	}
}

void game_exit(void)
{

}