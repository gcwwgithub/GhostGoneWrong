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

	init_main_menu();
	init_level_select_buttons();

	init_pause_screen();
	init_end_screen();
	init_skip_wave_button();

	//Initialize Objects
	mouse_init();
	

	// initialize price for powerups
	powerUpPrice.morePhantomQuartz = 10;
	powerUpPrice.reduceEnemySpeed = 10;
	powerUpPrice.reduceEnemyHealth = 10;
	powerUpPrice.increasedMineDamage = 10;

	
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

		CP_Settings_NoTint();
		render_turret_menu_object(GameMenuObject[NUMBER_OF_MENU_OBJECTS - 2], NUMBER_OF_MENU_OBJECTS - 2);// Render Upgrade menu first
		for (int i = 0; i < NUMBER_OF_MENU_OBJECTS - 2; i++) {// Last object will double render game grid. Second last object is rendered seperately
			render_turret_menu_object(GameMenuObject[i], i);
		}
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
		update_portal();
		render_environment();
		render_turret();
		render_projectile();

		render_bullet_circles();

		render_button_pressed();

		CP_Settings_NoTint();
		render_wave_timer();
		render_ui_button(SkipWaveButton);

		render_turret_menu_object(GameMenuObject[NUMBER_OF_MENU_OBJECTS - 2], NUMBER_OF_MENU_OBJECTS - 2);// Render Upgrade menu first
		for (int i = 0; i < NUMBER_OF_MENU_OBJECTS - 2; i++) {// Last object will double render game grid. Second last object is rendered seperately
			render_turret_menu_object(GameMenuObject[i], i);
		}


		//setting enemies
		Reset_enemies(currentGameLevel);


	}
	else if (currentGameState == Win || currentGameState == Lose)
	{
		if (btn_is_pressed(EndScreenButtons[0].buttonData))
		{
			currentGameState = MainMenu;
		}
		else if (btn_is_pressed(EndScreenButtons[1].buttonData))
		{
			init_next_level(currentGameLevel);
			currentGameState = Building;
		}
		else if (btn_is_pressed(EndScreenButtons[2].buttonData))
		{
			init_next_level(currentGameLevel + 1);
			//exit_to_desktop();
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

		render_turret_menu_object(GameMenuObject[NUMBER_OF_MENU_OBJECTS - 2], NUMBER_OF_MENU_OBJECTS - 2);// Render Upgrade menu first
		for (int i = 0; i < NUMBER_OF_MENU_OBJECTS - 2; i++) {// Last object will double render game grid. Second last object is rendered seperately
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

		// free memory
		for (int i = 0; i < gameGridRows; i++) {
			free(Level[currentGameLevel].grid[i]);
		}
		free(Level[currentGameLevel].grid);
		currentGameState = MainMenu;
		//Free memory for turret_on_grid
		for (int i = 0; i < gameGridCols; i++) {
			free(turret_on_grid[i]);
		}
		free(turret_on_grid);

	}
	else if (currentGameState == MainMenu)
	{
		CP_Settings_NoTint();
		if (btn_is_pressed(PlayButton.buttonData))
		{
			PlayButton.isMoving = QuitButton.isMoving = levelButtons->isMoving = 1;
		}
		else if (btn_is_pressed(QuitButton.buttonData))
		{
			currentGameState = MainMenu;
			exit_to_desktop();
		}

		if (PlayButton.isMoving) // clicked on play
		{
			PlayButton = ui_button_movement(PlayButton, -BUTTON_WIDTH, PlayButton.buttonData.yOrigin);
		}

		if (QuitButton.isMoving) // clicked on play
		{
			QuitButton = ui_button_movement(QuitButton, CP_System_GetWindowWidth(), QuitButton.buttonData.yOrigin);
		}

		if (levelButtons->isMoving)
		{
			move_level_select();
		}

		if (button_has_finished_moving(PlayButton, -BUTTON_WIDTH, PlayButton.buttonData.yOrigin) &&
			button_has_finished_moving(QuitButton, CP_System_GetWindowWidth(), QuitButton.buttonData.yOrigin))
		{
			// This presumes the play button was clicked already.
			PlayButton.isMoving = QuitButton.isMoving = levelButtons->isMoving = 0;
			currentGameState = LevelSelect;
		}

		CP_Graphics_ClearBackground(COLOR_GREY);
		render_title_screen();
		render_start_menu();
		if (levelButtons->isMoving) { render_level_select_buttons(); }
	}
	else if (currentGameState == LevelSelect)
	{
		CP_Settings_NoTint();
		// Level 1
		if (!levelButtons->isMoving) // hotfix for accidental misclick
		{
			if (btn_is_pressed(levelButtons[0].buttonData))
			{
				level1_init();
				//game grid 
				game_grid_init();
				isPlacingTurret = T_MAX;
				turretSelectedToUpgrade = NO_TURRET_SELECTED;
				powerUpMenu = FALSE;
				pathfinding_init(&Level[currentGameLevel]);
				environment_init(&Level[currentGameLevel]);

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

			turret_init();

			//Initialise Enemies
			Enemies_init();

			pathfinding_reset(&Level[currentGameLevel]);
			pathfinding_calculate_cost(&Level[currentGameLevel]);
			pathfinding_update(&Level[currentGameLevel]);
			set_building_time(BUILDING_PHASE_TIME);
			currentGameState = Building;
		}
		else if (btn_is_pressed(levelButtons[1].buttonData)) {
			level2_init();
			//game grid 
			game_grid_init();
			isPlacingTurret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			powerUpMenu = FALSE;
			pathfinding_init(&Level[currentGameLevel]);
			environment_init(&Level[currentGameLevel]);

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

			turret_init();

			//Initialise Enemies
			Enemies_init();

			pathfinding_reset(&Level[currentGameLevel]);
			pathfinding_calculate_cost(&Level[currentGameLevel]);
			pathfinding_update(&Level[currentGameLevel]);
			set_building_time(BUILDING_PHASE_TIME);
			currentGameState = Building;
		}

		else if (btn_is_pressed(levelButtons[2].buttonData)) {
			level3_init();
			//game grid 
			game_grid_init();
			isPlacingTurret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			powerUpMenu = FALSE;
			pathfinding_init(&Level[currentGameLevel]);
			environment_init(&Level[currentGameLevel]);

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

			turret_init();

			//Initialise Enemies
			Enemies_init();

			pathfinding_reset(&Level[currentGameLevel]);
			pathfinding_calculate_cost(&Level[currentGameLevel]);
			pathfinding_update(&Level[currentGameLevel]);
			set_building_time(BUILDING_PHASE_TIME);
			currentGameState = Building;
		}

		else if (btn_is_pressed(levelButtons[3].buttonData)) {
			level4_init();
			//game grid 
			game_grid_init();
			isPlacingTurret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			powerUpMenu = FALSE;
			pathfinding_init(&Level[currentGameLevel]);
			environment_init(&Level[currentGameLevel]);

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

			turret_init();

			//Initialise Enemies
			Enemies_init();

			pathfinding_reset(&Level[currentGameLevel]);
			pathfinding_calculate_cost(&Level[currentGameLevel]);
			pathfinding_update(&Level[currentGameLevel]);
			set_building_time(BUILDING_PHASE_TIME);
			currentGameState = Building;
		}

		else if (btn_is_pressed(levelButtons[4].buttonData)) {
			level5_init();
			//game grid 
			game_grid_init();
			isPlacingTurret = T_MAX;
			turretSelectedToUpgrade = NO_TURRET_SELECTED;
			powerUpMenu = FALSE;
			pathfinding_init(&Level[currentGameLevel]);
			environment_init(&Level[currentGameLevel]);

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

			turret_init();

			//Initialise Enemies
			Enemies_init();

				pathfinding_reset(&Level[currentGameLevel]);
				pathfinding_calculate_cost(&Level[currentGameLevel]);
				pathfinding_update(&Level[currentGameLevel]);
				set_building_time(BUILDING_PHASE_TIME);
				currentGameState = Building;
			}
			else if (btn_is_pressed(BackButton.buttonData))
			{
				PlayButton.isMoving = QuitButton.isMoving = levelButtons->isMoving = 1;
			}
		}

		if (PlayButton.isMoving)
		{
			PlayButton = ui_button_movement(PlayButton, CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.5f);
		}

		if (QuitButton.isMoving)
		{
			QuitButton = ui_button_movement(QuitButton, CP_System_GetWindowWidth() * 0.6f, CP_System_GetWindowHeight() * 0.5f);
		}

		if (levelButtons->isMoving)
		{
			move_level_select();
		}

		if (button_has_finished_moving(PlayButton, CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.5f) &&
			button_has_finished_moving(QuitButton, CP_System_GetWindowWidth() * 0.6f, CP_System_GetWindowHeight() * 0.5f))
		{
			PlayButton.isMoving = QuitButton.isMoving = levelButtons->isMoving = 0;
			currentGameState = MainMenu;
		}

		CP_Graphics_ClearBackground(COLOR_GREY);
		render_title_screen();
		if (PlayButton.isMoving || QuitButton.isMoving) { render_start_menu(); }
		render_level_select_buttons();
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
			//free memory
			for (int i = 0; i < gameGridRows; i++) {
				free(Level[currentGameLevel].grid[i]);
			}
			free(Level[currentGameLevel].grid);
			currentGameState = MainMenu;
			//Free memory for turret_on_grid
			for (int i = 0; i < gameGridCols; i++) {
				free(turret_on_grid[i]);
			}
			free(turret_on_grid);

			currentGameState = MainMenu;
		}
		else if (btn_is_pressed(PauseQuitButton.buttonData))
		{
			//free memory
			for (int i = 0; i < gameGridRows; i++) {
				free(Level[currentGameLevel].grid[i]);
			}
			free(Level[currentGameLevel].grid);
			currentGameState = MainMenu;
			//Free memory for turret_on_grid
			for (int i = 0; i < gameGridCols; i++) {
				free(turret_on_grid[i]);
			}
			free(turret_on_grid);

			exit_to_desktop();
		}
		render_pause_screen();
	}
}

void game_exit(void)
{
}