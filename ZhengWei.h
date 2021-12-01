/*!
@file       zhengwei.c
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@co-author	
@course     CSD 1400
@section    C
@date       01/12/2021
@brief    	Contains decleration of functions InitMouse, InitPowerUpPrice,
			InitHowToPlayButtons, UpdateGameButtonPressed, RenderGameGrid,
			RenderEnemyPath, RenderEnvironment, RenderTurretDetailsDisplay,
			RenderTurretMenuObjects, RenderHowToPlayPages. Contains decleration
			of variables is_placing_turret, power_up_menu, how_to_play_buttons,
			environment, kLeftGameMenuXWidth, kRightGameMenuXOrigin. Contains
			defination of enum variables, kLevel1Cols = 6, kLevel1Rows = 7,
			kLevel2Cols = 6, kLevel2Rows = 7, kLevel3Cols = 6, kLevel3Rows = 7,
			kLevel4Cols = 6, kLevel4Rows = 7, kLevel5Cols = 8, kLevel5Rows = 7,
			kMaxEnvironmentObject = 10, kMaxHowToPlayPages = 7 
			kHowToPlayBack = 0, kHowToPlayPrevious = 1, kHowToPlayNext = 2,
			kHowToPlayButtonMax = 3.
*//*__________________________________________________________________________*/
#include "game.h"

#include "cprocessing.h"

#ifndef GhostGoneWrong_CURRENTHEADERFILES_ZHENGWEI_H
#define GhostGoneWrong_CURRENTHEADERFILES_ZHENGWEI_H

#define COLOR_BLUE CP_Color_Create(0, 0, 255, 255)
// The turret type selected to place. kTMax would be used if no turret selected.
enum {
	kLevel1Cols = 6,
	kLevel1Rows = 7,
	kLevel2Cols = 6,
	kLevel2Rows = 7,
	kLevel3Cols = 6,
	kLevel3Rows = 7,
	kLevel4Cols = 6,
	kLevel4Rows = 7,
	kLevel5Cols = 8,
	kLevel5Rows = 7,
	kMaxEnvironmentObject = 10,
	kMaxHowToPlayPages = 7
};//Using enum instead of define for const int
enum {
	kHowToPlayBack,
	kHowToPlayPrevious,
	kHowToPlayNext,
	kHowToPlayButtonMax
};
TurretType is_placing_turret;
Boolean power_up_menu;
Coordinates how_to_play_buttons[kHowToPlayButtonMax];
Coordinates environment[kMaxEnvironmentObject];
extern const float kLeftGameMenuXWidth;
extern const float kRightGameMenuXOrigin;


void InitMouse(void);
void InitPowerUpPrice(void);
void InitHowToPlayButtons(void);
void UpdateGameButtonPressed(void);
void RenderGameGrid(void);
void RenderEnemyPath(void);
void RenderEnvironment(void);
void RenderTurretDetailsDisplay(void);
void RenderTurretMenuObjects(
	Coordinates menu_object_to_print, MenuObjectType type);
void RenderHowToPlayPages(void);
#endif // !GhostGoneWrong_CURRENTHEADERFILES_ZHENGWEI_H
