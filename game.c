#include"game.h"
#include "Samuel.h"
#include "John.h"
#include"Anderson.h"
#include "Gabriel.h"
#include "zhengwei.h"

#include "cprocessing.h"

void game_init(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//CP_System_Fullscreen();
	CP_System_SetWindowTitle("Ghost Gone Wrong");
	int gameWindowWidth = 1280;
	int gameWindowHeight = (int)(gameWindowWidth * 1080.0f / 1920.0f);//To apply uniform scaling
	scaling_factor = gameWindowWidth / 1280.0f;//Game is scaled according to 1280 width being 1;
	CP_System_SetWindowSize(gameWindowWidth, gameWindowHeight);//Please change the variables ot change th escreen size

	InitAllImages();
	InitSpritesheetArray();
	InitVariablesForSpriteFunctions();
	InitGameFont();
	InitSplashLogos();
	current_game_state = kLogoSplash;
	building_time = kFullBuildingPhaseTime;
	dpLogoDisplayTime = teamLogoDisplayTime = LOGO_DISPLAY_TIME;
	dpLogoFadeTime = teamLogoFadeTime = FADE_OUT_TIME;
	bgmSFXEnabled = sfxEnabled = 0;
	current_how_to_play_page = 0;
	InitHowToPlayButtons();

	//Main menu, level select

	InitMainMenu();
	InitLevelSelectButtons();
	InitOptionsScreen();
	InitCreditsScreen();
	InitPauseScreen();
	InitEndScreen();
	InitSkipWaveButton();

	//Initialize Objects
	InitMouse();


	// initialize price for powerups
	InitPowerUpPrice();

	//Initialise all sounds
	MusicInit();
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
		UpdateEnemies();

		//do turret & projectile update next
		update_turret();
		update_projectile();
		update_particle();

		//render all the stuff
		RenderLevelEnvironment(level.current_game_level);
		RenderGameGrid();
		RenderEnemyPath();

		UpdatePortalAnimation();

		RenderAllEnemies();
		RenderAllPortalEffects();
		render_turret();
		render_projectile();
		render_particle();
		RenderAndUpdateBulletCircles();

		if (!turret[turret_selected_to_upgrade].is_active) { // Close mine menu when it explodes
			turret_selected_to_upgrade = kNoTurretSelected;
		}
		UpdateGameButtonPressed();

		RenderEnvironment();
		RenderBattlefieldEffectText(level.current_effect);
		CP_Settings_NoTint();
		RenderTurretDetailsDisplay(); //render turret description when hovered
		RenderTurretMenuObjects(game_menu_object[kButtonMax - 2], kButtonMax - 2);// Render Upgrade menu first
		for (int i = 0; i < kButtonMax - 3; i++) {// Last object will double render game grid. Second and third last object is rendered seperately
			RenderTurretMenuObjects(game_menu_object[i], i);
		}
		GameWinLoseCheck();

	}
	else if (current_game_state == kBuilding)
	{
		ReduceBuildingPhaseTime();
		if (BtnIsPressed(SkipWaveButton.buttonData))
		{
			SetBuildingTime(0.0f);
			MouseReset();
			CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
		}

		//do turret & projectile update next
		update_turret();
		update_projectile();
		update_particle();
		//render all the stuff
		RenderLevelEnvironment(level.current_game_level);
		RenderGameGrid();
		RenderEnemyPath();
		UpdatePortalAnimation();
		RenderEnvironment();
		render_turret();
		render_projectile();
		render_particle();
		RenderAndUpdateBulletCircles();

		UpdateGameButtonPressed();

		CP_Settings_NoTint();
		RenderWaveTimer();
		RenderUIButton(SkipWaveButton);

		RenderTurretDetailsDisplay(); //render turret description when hovered
		RenderTurretMenuObjects(game_menu_object[kButtonMax - 2], kButtonMax - 2);// Render Upgrade menu first
		for (int i = 0; i < kButtonMax - 3; i++) {// Last object will double render game grid. Second and third last object is rendered seperately
			RenderTurretMenuObjects(game_menu_object[i], i);
		}


		//setting enemies
		ResetEnemyInit();


	}
	else if (current_game_state == kWin || current_game_state == kLose)
	{
		if (BtnIsPressed(EndScreenButtons[0].buttonData))
		{
			current_game_state = kMainMenu;
			CP_Sound_StopGroup(CP_SOUND_GROUP_1);
			CP_Sound_PlayAdvanced(main_menu_bgm, bgm_volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
			MouseReset();
		}
		else if (BtnIsPressed(EndScreenButtons[1].buttonData))
		{
			InitNextLevel(level.current_game_level);
			current_game_state = kBuilding;
		}
		else if (BtnIsPressed(EndScreenButtons[2].buttonData))
		{
			InitNextLevel(level.current_game_level + 1);
		}

		RenderEndScreen(); // this should pause the game?

	}
	else if (current_game_state == kMainMenu)
	{
		CP_Settings_NoTint();
		if (BtnIsPressed(MainMenuButtons[StartButton].buttonData))
		{
			// To prevent clicking buttons while transitioning to LevelSelect
			if (!CreditsBackButton.isMoving)
			{
				MainMenuButtons[StartButton].isMoving = MainMenuButtons[CreditsButton].isMoving
					= MainMenuButtons[QuitButton].isMoving = MainMenuButtons[HowToPlayButton].isMoving
					= LevelButtons->isMoving = 1;
			}
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (BtnIsPressed(MainMenuButtons[QuitButton].buttonData))
		{
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
			ExitToDesktop();
		}
		else if (BtnIsPressed(MainMenuButtons[CreditsButton].buttonData))
		{
			// To prevent clicking buttons while transitioning to Credits
			if (!MainMenuButtons[StartButton].isMoving)
			{
				CreditsBackButton.isMoving = 1;
				if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
					CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
						CP_SOUND_GROUP_0);
				}
			}
		}
		else if (BtnIsPressed(MainMenuButtons[OptionsButton].buttonData)) {
			current_game_state = kOptions;
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (BtnIsPressed(MainMenuButtons[HowToPlayButton].buttonData)) {
			current_game_state = kHowToPlay;
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		// All these buttons move altogether.
		if (MainMenuButtons[StartButton].isMoving || MainMenuButtons[CreditsButton].isMoving || MainMenuButtons[QuitButton].isMoving || MainMenuButtons[HowToPlayButton].isMoving)
		{
			MoveMainMenu();
		}
		if (CreditsBackButton.isMoving)
		{
			MoveCreditsScreen();
		}
		if (LevelButtons->isMoving)
		{
			MoveLevelSelect();
		}

		// Clicked on Play, and checking if the Play,Quit buttons have left and Level Select buttons have come
		if (MainMenuFinishedMoving() && LevelSelectFinishedMoving())
		{
			current_game_state = kLevelSelect;
		}

		// Transition to Credits check
		if (ButtonHasFinishedMoving(CreditsBackButton, CreditsBackButton.buttonData.x_origin, CP_System_GetWindowHeight() * 0.93f))
		{
			CreditsBackButton.isMoving = 0;
			CreditsBackButton.movementTime = creditTextMoveTime = 0.0f;
			current_game_state = kCredits;
		}


		CP_Graphics_ClearBackground(COLOR_GREY);
		RenderTitleScreen();
		RenderMainMenu();
		RenderLevelSelectButtons();
		RenderCreditsScreen();
	}
	else if (current_game_state == kHowToPlay) {
		RenderHowToPlayPages();
	}
	else if (current_game_state == kLevelSelect)
	{
		CP_Settings_NoTint();
		// Levels
		if (BtnIsPressed(LevelButtons[0].buttonData))
		{
			InitLevel1();
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (BtnIsPressed(LevelButtons[1].buttonData)) {
			InitLevel2();
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (BtnIsPressed(LevelButtons[2].buttonData)) {
			InitLevel3();
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (BtnIsPressed(LevelButtons[3].buttonData)) {
			InitLevel4();
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (BtnIsPressed(LevelButtons[4].buttonData)) {
			InitLevel5();
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (BtnIsPressed(LevelSelectBackButton.buttonData))
		{
			MainMenuButtons[StartButton].isMoving = MainMenuButtons[CreditsButton].isMoving
				= MainMenuButtons[QuitButton].isMoving = MainMenuButtons[HowToPlayButton].isMoving
				= LevelButtons->isMoving = 1;

			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
			MouseReset();
		}

		// All these buttons move altogether.
		if (MainMenuButtons[StartButton].isMoving || MainMenuButtons[CreditsButton].isMoving || MainMenuButtons[QuitButton].isMoving || MainMenuButtons[HowToPlayButton].isMoving)
		{
			MoveMainMenu();
		}

		if (LevelButtons->isMoving)
		{
			MoveLevelSelect();
		}

		if (MainMenuFinishedMoving() && LevelSelectFinishedMoving())
		{
			current_game_state = kMainMenu;
		}

		CP_Graphics_ClearBackground(COLOR_GREY);
		RenderTitleScreen();
		RenderMainMenu();
		RenderLevelSelectButtons();
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
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (BtnIsPressed(PauseScreenButtons[0].buttonData)) // Resume
		{
			current_game_state = (building_time) ? kBuilding : kWave;
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (BtnIsPressed(PauseScreenButtons[1].buttonData)) // Level Select
		{
			//free memory
			for (int i = 0; i < level_grid_rows; i++) {
				free(level.grid[i]);
			}
			free(level.grid);
			//Free memory for turret_on_grid
			for (int i = 0; i < level_grid_cols; i++) {
				free(turret_on_grid[i]);
			}
			free(turret_on_grid);

			current_game_state = kLevelSelect;
			CP_Sound_StopGroup(CP_SOUND_GROUP_1);
			CP_Sound_PlayAdvanced(main_menu_bgm, bgm_volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		RenderPauseScreen();
	}
	else if (current_game_state == kCredits)
	{
		if (BtnIsPressed(CreditsBackButton.buttonData))
		{
			CreditsBackButton.isMoving = 1;
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		if (CreditsBackButton.isMoving)
		{
			MoveCreditsScreen();
		}
		if (ButtonHasFinishedMoving(CreditsBackButton, CreditsBackButton.buttonData.x_origin, CP_System_GetWindowHeight() * 2.0f))
		{
			CreditsBackButton.isMoving = 0;
			CreditsBackButton.movementTime = creditTextMoveTime = 0.0f;
			current_game_state = kMainMenu;
		}


		CP_Graphics_ClearBackground(COLOR_GREY);
		RenderTitleScreen();
		RenderMainMenu();
		RenderCreditsScreen();
	}
	else if (current_game_state == kOptions)
	{
		if (BtnIsPressed(OptionsBackButton.buttonData))
		{
			current_game_state = kMainMenu;
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (BtnIsPressed(OptionButtons[BackgroundSFX])) // bgm music
		{
			(bgmSFXEnabled) ? CP_Sound_ResumeGroup(CP_SOUND_GROUP_1) : CP_Sound_PauseGroup(CP_SOUND_GROUP_1);
			bgmSFXEnabled = !bgmSFXEnabled;
			MouseReset();
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) 
			{
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}
		else if (BtnIsPressed(OptionButtons[SFX])) // resume all music
		{
			if (sfxEnabled)
			{
				CP_Sound_ResumeAll();
				if (bgmSFXEnabled)
				{
					CP_Sound_PauseGroup(CP_SOUND_GROUP_1);
				}
			}
			else
			{
						CP_Sound_PauseAll();
			}

			sfxEnabled = !sfxEnabled;
			MouseReset();
			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) 
			{
				CP_Sound_PlayAdvanced(button_click_sfx, sfx_volume, 1.0f, FALSE,
					CP_SOUND_GROUP_0);
			}
		}

		CP_Graphics_ClearBackground(COLOR_GREY);
		RenderTitleScreen();
		RenderOptionsScreen();
	}
	else if (current_game_state == kLogoSplash)
	{
		RenderLogos();
	}
}

void game_exit(void)
{
	size_t i = 0;
	for (i = 0; i < sizeof(slow_turret_image_array) / sizeof(CP_Image); ++i)
	{
		CP_Image_Free(&slow_turret_image_array[i]);
	}
	for (i = 0; i < sizeof(tutorial_image_array) / sizeof(CP_Image); ++i)
	{
		CP_Image_Free(&tutorial_image_array[i]);
	}

	CP_Image_Free(&digipen_logo);
	CP_Image_Free(&down_n_out_logo);


	CP_Image_Free(&pause_button_image);
	CP_Image_Free(&game_title_image);
	CP_Image_Free(&turret_button_background);
	CP_Image_Free(&thin_UI_background);
	CP_Image_Free(&upgrade_menu_background);
	CP_Image_Free(&basic_ghost_spritesheet);
	CP_Image_Free(&fast_ghost_spritesheet);
	CP_Image_Free(&fat_ghost_spritesheet);
	CP_Image_Free(&grim_reaper_spritesheet);
	CP_Image_Free(&blue_portal_spritesheet);
	CP_Image_Free(&red_portal_spritesheet);
	CP_Image_Free(&basic_turret_spritesheet);
	CP_Image_Free(&homing_missle_turret_spritesheet);
	CP_Image_Free(&mine_spritesheet);
	CP_Image_Free(&turret_bullet_spritesheet);
	CP_Image_Free(&turret_bullet_radius_spritesheet);
	CP_Image_Free(&currency_spritesheet);
	CP_Image_Free(&grid_environment_objects_spritesheet);
	CP_Image_Free(&background_spritesheet);
	CP_Image_Free(&portal_enter_effect_spritesheet);
	CP_Image_Free(&portal_spawn_effect_spritesheet);
	CP_Image_Free(&power_up_spritesheet);
	CP_Image_Free(&battlefield_effect_spritesheet);
	CP_Image_Free(&fat_UI_background_spritesheet);
	CP_Image_Free(&interactable_UI_buttons_spritesheet);
	CP_Image_Free(&turret_stats_spritesheet);
	CP_Image_Free(&non_grid_environment_objects_spritesheet);


	CP_Sound_Free(&shoot_sfx);
	CP_Sound_Free(&spawn_exit_sfx);
	CP_Sound_Free(&button_click_sfx);
	CP_Sound_Free(&turret_place_sfx);
	CP_Sound_Free(&mine_explosion_sfx);
	CP_Sound_Free(&win_sfx);
	CP_Sound_Free(&lose_sfx);
	CP_Sound_Free(&hit_sfx);
	CP_Sound_Free(&main_menu_bgm);
	CP_Sound_Free(&building_bgm);
	CP_Sound_Free(&wave_bgm);
	CP_Sound_Free(&main_menu_music);

#if _DEBUG
	// MEM LEAK CHECK
	_CrtDumpMemoryLeaks();
#endif
}