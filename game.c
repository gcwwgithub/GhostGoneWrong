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
	init_end_screen();
	init_skip_wave_button();

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
	menu_swap_init();
	wave_number_display_init();
	battlefield_effects_display_init();
	monster_remaining_display_init();
	upgrade_menu_init();
	upgrade_button_init();
	sell_button_init();

	//Initialize Objects
	mouse_init();
	turret_init();

	//Initialise Enemies
	Enemies_init();
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
		game_win_lose_check();
		//do enemy update first
		update_enemy();

		//do turret & projectile update next
		update_turret();
		update_projectile();


		//render all the stuff
		render_game_background(currentGameLevel);
		render_game_grid();
		render_path(&Level[currentGameLevel]);
		for (int i = 0; i < NUMBER_OF_MENU_OBJECTS - 1; i++) {// Last object will double render game grid
			render_turret_menu_object(GameMenuObject[i], i);
		}
		//display_enemies_left(); //Already done by my code render turret menu object
		update_portal();

		draw_multiple_enemies();
		render_all_portal_effects();
		render_turret();
		render_projectile();

		render_bullet_circles();

		if (!turret[turretSelectedToUpgrade].isActive) { // Close mine menu when it explodes
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
		}
		render_button_pressed();

		render_environment();

	}
	else if (currentGameState == Building)
	{
		reduce_building_phase_time();
		if (btn_is_pressed(SkipWaveButton.buttonData))
		{
			set_building_time(0.0f);
		}

		//do turret & projectile update next
		update_turret();
		update_projectile();

		//render all the stuff
		render_game_background(currentGameLevel);
		render_game_grid();
		render_path(&Level[currentGameLevel]);

		render_wave_timer_text();
		render_ui_button(SkipWaveButton);

		for (int i = 0; i < NUMBER_OF_MENU_OBJECTS - 1; i++) {// Last object will double render game grid
			render_turret_menu_object(GameMenuObject[i], i);
		}

		render_turret();
		render_projectile();

		render_bullet_circles();

		render_button_pressed();

		//setting enemies
		Reset_enemies(currentGameLevel);


		render_environment();
		update_portal();
	}
	else if (currentGameState == Win || currentGameState == Lose)
	{
		if (btn_is_pressed(EndScreenButtons[0].buttonData))
		{
			currentGameState = MainMenu;
		}
		else if (btn_is_pressed(EndScreenButtons[1].buttonData))
		{
			init_level(currentGameLevel);
			currentGameState = Building;
		}
		else if (btn_is_pressed(EndScreenButtons[2].buttonData))
		{
			exit_to_desktop();
		}
		//do enemy update first
		update_enemy();

		//do turret & projectile update next
		update_turret();
		update_projectile();


		//render all the stuff
		render_game_background(currentGameLevel);
		render_game_grid();
		render_path(&Level[currentGameLevel]);
		for (int i = 0; i < NUMBER_OF_MENU_OBJECTS - 1; i++) {// Last object will double render game grid
			render_turret_menu_object(GameMenuObject[i], i);
		}
		update_portal();

		draw_multiple_enemies();
		render_all_portal_effects();
		render_turret();
		render_projectile();

		render_bullet_circles();

		render_button_pressed();


		render_environment();

		render_end_screen(); // this should pause the game by way of gameLost.
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
			level1_init();
			isPlacingTurret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			powerUpMenu = FALSE;
			pathfinding_init(&Level[currentGameLevel]);
			environment_init(&Level[currentGameLevel]);

			Enemies_init(2, 2, 2, &Level[currentGameLevel]);

			pathfinding_reset(&Level[currentGameLevel]);
			pathfinding_calculate_cost(&Level[currentGameLevel]);
			pathfinding_update(&Level[currentGameLevel]);
			set_building_time(BUILDING_PHASE_TIME);
			currentGameState = Building;
		}
		else if (btn_is_pressed(levelButtons[1].buttonData)) {
			level2_init();
			isPlacingTurret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			powerUpMenu = FALSE;
			pathfinding_init(&Level[currentGameLevel]);
			environment_init(&Level[currentGameLevel]);

			Enemies_init(2, 2, 2, &Level[currentGameLevel]);

			pathfinding_reset(&Level[currentGameLevel]);
			pathfinding_calculate_cost(&Level[currentGameLevel]);
			pathfinding_update(&Level[currentGameLevel]);
			set_building_time(BUILDING_PHASE_TIME);
			currentGameState = Building;
		}

		else if (btn_is_pressed(levelButtons[2].buttonData)) {
			level3_init();
			isPlacingTurret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			powerUpMenu = FALSE;
			pathfinding_init(&Level[currentGameLevel]);
			environment_init(&Level[currentGameLevel]);

			Enemies_init(2, 2, 2, &Level[currentGameLevel]);

			pathfinding_reset(&Level[currentGameLevel]);
			pathfinding_calculate_cost(&Level[currentGameLevel]);
			pathfinding_update(&Level[currentGameLevel]);
			set_building_time(BUILDING_PHASE_TIME);
			currentGameState = Building;
		}

		else if (btn_is_pressed(levelButtons[3].buttonData)) {
			level4_init();
			isPlacingTurret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			powerUpMenu = FALSE;
			pathfinding_init(&Level[currentGameLevel]);
			environment_init(&Level[currentGameLevel]);

			Enemies_init(2, 2, 2, &Level[currentGameLevel]);

			pathfinding_reset(&Level[currentGameLevel]);
			pathfinding_calculate_cost(&Level[currentGameLevel]);
			pathfinding_update(&Level[currentGameLevel]);
			set_building_time(BUILDING_PHASE_TIME);
			currentGameState = Building;
		}

		else if (btn_is_pressed(levelButtons[4].buttonData)) {
			level5_init();
			isPlacingTurret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			powerUpMenu = FALSE;
			pathfinding_init(&Level[currentGameLevel]);
			environment_init(&Level[currentGameLevel]);

			Enemies_init(2, 2, 2, &Level[currentGameLevel]);

			pathfinding_reset(&Level[currentGameLevel]);
			pathfinding_calculate_cost(&Level[currentGameLevel]);
			pathfinding_update(&Level[currentGameLevel]);
			set_building_time(BUILDING_PHASE_TIME);
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
				currentGameState = (buildingTime) ? Building : Wave;
			}
			else // if the game is not paused
			{
				currentGameState = Pause;
			}
			mouse_reset();
		}
		else if (btn_is_pressed(PauseBackButton.buttonData))
		{
			currentGameState = MainMenu;
		}
		else if (btn_is_pressed(PauseQuitButton.buttonData))
		{
			exit_to_desktop();
		}
		render_pause_screen();
	}
}

void game_exit(void)
{

}