#include "cprocessing.h"
#include"game.h"

void game_square_color(LevelData* Level);
void draw_game_grid(void);
void color_game_grid(LevelData Level);
void click_on_square(void);



void game_init(void)
{
	//CP_System_Fullscreen();
	currentGameState = MainMenu;
	
	//game grid 
	float unusableScreenHeight, unusableScreenWidth;/*Height and Width not used for game, example menu*/
	unusableScreenHeight = (float)CP_System_GetWindowHeight() / 2; //Half the screeen is used for the game
	Game_Height = (float)CP_System_GetWindowHeight() - unusableScreenHeight;
	Game_Grid_Height = Game_Height / GAME_GRID_ROWS;
	Game_Grid_Width = Game_Height / GAME_GRID_ROWS; //Grid is a Square
	Game_Width = Game_Grid_Width * GAME_GRID_COLS;
	unusableScreenWidth = (float)CP_System_GetWindowWidth() - Game_Width;
	GAME_X_ORIGIN = unusableScreenWidth / 2; //To centralise the Grid
	GAME_Y_ORIGIN = unusableScreenHeight / 2; //Centre the game
	
	//Initialize Objects
	Initialize_object();

	//Level Data
	Tutorial.spawnRow = 0;
	Tutorial.spawnCol = (GAME_GRID_COLS - 1) / 2;
	Tutorial.exitRow = GAME_GRID_ROWS - 1;
	Tutorial.exitCol = (GAME_GRID_COLS - 1) / 2;

	game_square_color(&Tutorial);
}

void game_update(void)
{
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
		MouseInput.objectPositionX = CP_Input_GetMouseX();
		MouseInput.objectPositionY = CP_Input_GetMouseY();
	}
	if (Collision_Detection(MouseInput, objectGameGrid)) {

		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(COLOR_BLACK);
		CP_Graphics_DrawRect((GAME_X_ORIGIN), (GAME_Y_ORIGIN), (Game_Grid_Width), (Game_Grid_Height));
	}
	click_on_square();

	draw_game_grid();
	color_game_grid(Tutorial);

}
void game_exit(void)
{

}