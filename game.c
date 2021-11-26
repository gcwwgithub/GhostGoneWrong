#include"game.h"
#include "Samuel.h"
#include "John.h"
#include"Anderson.h"
#include "Gabriel.h"
#include "zhengwei.h"

#include "cprocessing.h"

void game_init(void)
{
	//CP_System_ShowConsole(); //pls dont delete this cause scrub me uses printf to debug -gabriel
	//CP_System_Fullscreen();
	CP_System_SetWindowTitle("Ghost Gone Wrong");
	int gameWindowWidth = 1280;
	int gameWindowHeight = (int)(gameWindowWidth * 1080.0f / 1920.0f);//To apply uniform scaling
	scaling_factor = gameWindowWidth / 1280.0f;//Game is scaled according to 1280 width being 1;
	CP_System_SetWindowSize(gameWindowWidth, gameWindowHeight);//Please change the variables ot change th escreen size

	InitAllImages();
	InitSpritesheetArray();
	InitVariablesForSpriteFunctions();
	init_game_font();
	init_splash_logos();
	current_game_state = kLogoSplash;
	building_time = kFullBuildingPhaseTime;
	dpLogoTime = teamLogoTime = LOGO_DISPLAY_TIME;
	dpLogoFadeTime = teamLogoFadeTime = FADE_OUT_TIME;

	//Main menu, level select

	init_main_menu();
	init_level_select_buttons();
	init_credits_screen();
	init_pause_screen();
	init_end_screen();
	init_skip_wave_button();

	//Initialize Objects
	MouseInit();


	// initialize price for powerups
	PowerUpPriceInit();
}


void game_update(void)
{
	//Input
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		mouse_input.x_origin = CP_Input_GetMouseX();
		mouse_input.y_origin = CP_Input_GetMouseY();
	}


	if (current_game_state == kWave)
	{
		//do enemy update first
		update_enemy();

		//do turret & projectile update next
		update_turret();
		update_projectile();
		update_particle();

		//render all the stuff
		RenderLevelEnvironment(current_game_level);
		RenderGameGrid();
		RenderEnemyPath(&Level[current_game_level]);

		UpdatePortalAnimation();

		draw_multiple_enemies();
		RenderAllPortalEffects();
		render_turret();
		render_projectile();
		render_particle();
		RenderAndUpdateBulletCircles();

		if (!turret[turretSelectedToUpgrade].is_active) { // Close mine menu when it explodes
			turretSelectedToUpgrade = kNoTurretSelected;
		}
		ButtonPressedUpdate();

		RenderEnvironment();
		RenderBattlefieldEffectText(Level[current_game_level].current_effect);
		CP_Settings_NoTint();
		RenderTurretDetailsDisplay(); //render turret description when hovered
		render_turret_menu_object(game_menu_object[kButtonMax - 2], kButtonMax - 2);// Render Upgrade menu first
		for (int i = 0; i < kButtonMax - 3; i++) {// Last object will double render game grid. Second and third last object is rendered seperately
			render_turret_menu_object(game_menu_object[i], i);
		}
		game_win_lose_check();

	}
	else if (current_game_state == kBuilding)
	{
		reduce_building_phase_time();
		if (BtnIsPressed(SkipWaveButton.buttonData))
		{
			SetBuildingTime(0.0f);
			MouseReset();
		}

		//do turret & projectile update next
		update_turret();
		update_projectile();
		update_particle();
		//render all the stuff
		RenderLevelEnvironment(current_game_level);
		RenderGameGrid();
		RenderEnemyPath(&Level[current_game_level]);
		UpdatePortalAnimation();
		RenderEnvironment();
		render_turret();
		render_projectile();
		render_particle();
		RenderAndUpdateBulletCircles();

		ButtonPressedUpdate();

		CP_Settings_NoTint();
		render_wave_timer();
		render_ui_button(SkipWaveButton);

		RenderTurretDetailsDisplay(); //render turret description when hovered
		render_turret_menu_object(game_menu_object[kButtonMax - 2], kButtonMax - 2);// Render Upgrade menu first
		for (int i = 0; i < kButtonMax - 3; i++) {// Last object will double render game grid. Second and third last object is rendered seperately
			render_turret_menu_object(game_menu_object[i], i);
		}


		//setting enemies
		Reset_enemies(current_game_level);


	}
	else if (current_game_state == kWin || current_game_state == kLose)
	{
		if (BtnIsPressed(EndScreenButtons[0].buttonData))
		{
			current_game_state = kMainMenu;
		}
		else if (BtnIsPressed(EndScreenButtons[1].buttonData))
		{
			init_next_level(current_game_level);
			current_game_state = kBuilding;
		}
		else if (BtnIsPressed(EndScreenButtons[2].buttonData))
		{
			init_next_level(current_game_level + 1);
		}

		render_end_screen(); // this should pause the game by way of gameLost.

	}
	else if (current_game_state == kMainMenu)
	{
		CP_Settings_NoTint();

		if (BtnIsPressed(PlayButton.buttonData))
		{
			if (!CreditsBackButton.isMoving)
			{
				PlayButton.isMoving = CreditsButton.isMoving = QuitButton.isMoving = LevelButtons->isMoving = 1;
			}
		}
		else if (BtnIsPressed(QuitButton.buttonData))
		{
			exit_to_desktop();
		}
		else if (BtnIsPressed(CreditsButton.buttonData))
		{
			if (!PlayButton.isMoving)
			{
				CreditsBackButton.isMoving = 1;
			}
		}

		if (PlayButton.isMoving) // clicked on play
		{
			PlayButton = ui_button_movement(PlayButton, -BUTTON_WIDTH, PlayButton.buttonData.y_origin);
		}
		if (CreditsButton.isMoving)
		{
			CreditsButton = ui_button_movement(CreditsButton, CreditsButton.buttonData.x_origin, (float)CP_System_GetWindowHeight());
		}
		if (QuitButton.isMoving) // clicked on play
		{
			QuitButton = ui_button_movement(QuitButton, (float)CP_System_GetWindowWidth(), QuitButton.buttonData.y_origin);
		}
		if (CreditsBackButton.isMoving)
		{
			move_credits_screen();
		}
		if (LevelButtons->isMoving)
		{
			move_level_select();
		}

		// Clicked on Play, and checking if the Play,Quit buttons have left and Level Select buttons have come
		if (button_has_finished_moving(PlayButton, -BUTTON_WIDTH, PlayButton.buttonData.y_origin) &&
			button_has_finished_moving(CreditsButton, CreditsButton.buttonData.x_origin, (float)CP_System_GetWindowHeight()) &&
			button_has_finished_moving(QuitButton, (float)CP_System_GetWindowWidth(), QuitButton.buttonData.y_origin) &&
			level_select_finished_moving())
		{
			PlayButton.isMoving = CreditsButton.isMoving = QuitButton.isMoving = LevelButtons->isMoving = 0;
			PlayButton.movementTime = CreditsButton.movementTime = QuitButton.movementTime = 0.0f;
			current_game_state = kLevelSelect;
		}

		if (button_has_finished_moving(CreditsBackButton, CreditsBackButton.buttonData.x_origin, CP_System_GetWindowHeight() * 0.93f))
		{
			CreditsBackButton.isMoving = 0;
			CreditsBackButton.movementTime = creditTextMoveTime = 0.0f;
			current_game_state = kCredits;
		}


		CP_Graphics_ClearBackground(COLOR_GREY);
		render_title_screen();
		render_start_menu();
		render_level_select_buttons();
		render_credits_screen();
	}
	else if (current_game_state == kLevelSelect)
	{
		CP_Settings_NoTint();
		// Level 1
		if (!LevelButtons->isMoving) // commenting this activates lvl 5 for some reason
		{
			if (BtnIsPressed(LevelButtons[0].buttonData))
			{
				Level1Init();
			}
			else if (BtnIsPressed(LevelButtons[1].buttonData)) {
				Level2Init();
			}
			else if (BtnIsPressed(LevelButtons[2].buttonData)) {
				Level3Init();
			}
			else if (BtnIsPressed(LevelButtons[3].buttonData)) {
				Level4Init();
			}
			else if (BtnIsPressed(LevelButtons[4].buttonData)) {
				Level5Init();
			}
			else if (BtnIsPressed(LevelSelectBackButton.buttonData))
			{
				PlayButton.isMoving = CreditsButton.isMoving = QuitButton.isMoving = LevelButtons->isMoving = 1;
			}
		}

		if (PlayButton.isMoving)
		{
			PlayButton = ui_button_movement(PlayButton, CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, PlayButton.buttonData.y_origin);
		}
		if (CreditsButton.isMoving)
		{
			CreditsButton = ui_button_movement(CreditsButton, CreditsButton.buttonData.x_origin, CP_System_GetWindowHeight() * 0.5f);
		}
		if (QuitButton.isMoving)
		{
			QuitButton = ui_button_movement(QuitButton, CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, QuitButton.buttonData.y_origin);
		}
		if (LevelButtons->isMoving)
		{
			move_level_select();
		}

		if (button_has_finished_moving(PlayButton, CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, PlayButton.buttonData.y_origin) &&
			button_has_finished_moving(CreditsButton, CreditsButton.buttonData.x_origin, CP_System_GetWindowHeight() * 0.5f) &&
			button_has_finished_moving(QuitButton, CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, QuitButton.buttonData.y_origin) &&
			level_select_finished_moving())
		{
			PlayButton.isMoving = CreditsButton.isMoving = QuitButton.isMoving = LevelButtons->isMoving = 0;
			PlayButton.movementTime = CreditsButton.movementTime = QuitButton.movementTime = 0.0f;
			current_game_state = kMainMenu;
		}

		CP_Graphics_ClearBackground(COLOR_GREY);
		render_title_screen();
		render_start_menu();
		render_level_select_buttons();
	}
	else if (current_game_state == kPause)
	{
		if (CheckGameButtonPressed() == kPauseButton)
		{
			if (current_game_state == kPause)
			{
				current_game_state = (building_time) ? kBuilding : kWave;
			}
			else // if the game is not paused
			{
				current_game_state = kPause;
			}
			MouseReset();
		}
		else if (BtnIsPressed(PauseScreenButtons[0].buttonData)) // Resume
		{
			current_game_state = (building_time) ? kBuilding : kWave;
		}
		else if (BtnIsPressed(PauseScreenButtons[1].buttonData)) // Level Select
		{
			//free memory
			for (int i = 0; i < level_grid_rows; i++) {
				free(Level[current_game_level].grid[i]);
			}
			free(Level[current_game_level].grid);
			//Free memory for turret_on_grid
			for (int i = 0; i < level_grid_cols; i++) {
				free(turret_on_grid[i]);
			}
			free(turret_on_grid);

			current_game_state = kLevelSelect;
		}
		render_pause_screen();
	}
	else if (current_game_state == kCredits)
	{
		if (BtnIsPressed(CreditsBackButton.buttonData))
		{
			CreditsBackButton.isMoving = 1;
		}
		if (CreditsBackButton.isMoving)
		{
			move_credits_screen();
		}
		if (button_has_finished_moving(CreditsBackButton, CreditsBackButton.buttonData.x_origin, CP_System_GetWindowHeight() * 2.0f))
		{
			CreditsBackButton.isMoving = 0;
			CreditsBackButton.movementTime = creditTextMoveTime = 0.0f;
			current_game_state = kMainMenu;
		}


		CP_Graphics_ClearBackground(COLOR_GREY);
		render_title_screen();
		render_start_menu();
		render_credits_screen();
	}
	else if (current_game_state == kLogoSplash)
	{
		show_logos();
	}
}

void game_exit(void)
{
}