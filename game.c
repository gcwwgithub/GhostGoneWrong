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
	int gameWindowWidth = 1280;
	int gameWindowHeight = (int)(gameWindowWidth * 1080.0f / 1920.0f);//To apply uniform scaling
	scalingFactor = gameWindowWidth / 1280.0f;//Game is scaled according to 1280 width being 1;
	CP_System_SetWindowSize(gameWindowWidth, gameWindowHeight);//Please change the variables ot change th escreen size
	
	init_all_images();
	init_spritesheet_array();
	init_linkedlist_variables();
	init_game_font();
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
	//turret_menu_init();

	//turret menu items
	pause_button_init();
	turret_basic_button_init();
	turret_slow_button_init();
	turret_homing_button_init();
	turret_mine_button_init();
	phantomQuartz_init();
	goldQuartz_init();
	
	
	health_init();
	currency_swap_init();
	wave_number_display_init();
	battlefield_effects_display_init();
	monster_remaining_display_init();

	//Initialize Objects
	mouse_init();
	isPlacingTurret = NOT_PLACING_TURRET;

	//Level Data
	currentGameLevel = 0;
	Level[0].spawnRow = 0;
	Level[0].spawnCol = (GAME_GRID_COLS - 1) / 2;
	Level[0].exitRow = GAME_GRID_ROWS - 1;
	Level[0].exitCol = (GAME_GRID_COLS - 1) / 2;
	Level[0].health = 100;
	Level[0].phantomQuartz = 50;
	Level[0].goldQuartz = 0;
	Level[0].currentWave = 0;
	Level[0].currentEffect = 0;
	
	pathfinding_init(&Level[0]);
	environment_init(&Level[0]);

	turret_init();
	Enemies_init(2,2,2,&Level[0]);

	pathfinding_reset(&Level[0]);
	pathfinding_calculate_cost(&Level[0]);
	pathfinding_update(&Level[0]);
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
		update_enemy();

		//do turret & projectile update next
		update_turret();
		update_projectile();

		//render all the stuff
		render_game_grid();
		render_path(&Level[currentGameLevel]);
		for (int i = 0; i < NUMBER_OF_MENU_OBJECTS; i++) {
			render_turret_menu_object(GameMenuObject[i], i);
		}
		//display_enemies_left(); //Already done by my code render turret menu object
		UpdatePortal();
		
		draw_multiple_enemies();
		render_all_portal_effects();
		render_turret();
		render_projectile();

		render_bullet_circles();
		
		render_new_turret(&Level[currentGameLevel]);
		render_button_pressed();//Must be after render_new_turret


		render_environment();
		
	}
	else if (currentGameState == Building)
	{
		reduce_building_phase_time();

		//do turret & projectile update next
		update_turret();
		update_projectile();

		//render all the stuff
		render_game_grid();
		render_path(&Level[currentGameLevel]);

		render_wave_timer_text();
		
		for (int i = 0; i < NUMBER_OF_MENU_OBJECTS; i++) {
			render_turret_menu_object(GameMenuObject[i], i);
		}

		render_turret();
		render_projectile();

		render_bullet_circles();

		render_new_turret(&Level[currentGameLevel]);
		render_button_pressed();//Must be after render_new_turret

		//test enemy

		render_environment();
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
			exit_to_desktop();
		}

		CP_Graphics_ClearBackground(COLOR_GREY);
		render_title_screen();
		render_ui_button(PlayButton);
		render_ui_button(QuitButton);
	}
	else if (currentGameState == LevelSelect)
	{
		// Level 1
		if (btn_is_pressed(levelButtons[0].buttonData))
		{
			currentGameState = Building;
		}
		else if (btn_is_pressed(BackButton.buttonData))
		{
			currentGameState = MainMenu;
		}

		CP_Graphics_ClearBackground(COLOR_GREY);
		render_title_screen();
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
			exit_to_desktop(); // placing a button in the bottom half of the screen automatically activates it.
		}
		render_pause_screen();
	}
}

void game_exit(void)
{

}