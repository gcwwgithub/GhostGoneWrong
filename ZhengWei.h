#include "game.h"

#include "cprocessing.h"

#ifndef GhostGoneWrong_CURRENTHEADERFILES_ZHENGWEI_H
#define GhostGoneWrong_CURRENTHEADERFILES_ZHENGWEI_H

#define COLOR_BLUE CP_Color_Create(0, 0, 255, 255)
// The turret type selected to place. kTMax would be used if no turret selected.
TurretType is_placing_turret;
Boolean power_up_menu;
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
Coordinates TutorialButtons[kHowToPlayButtonMax];
extern const float kLeftGameMenuXWidth;
extern const float kRightGameMenuXOrigin;
//Environment
Coordinates environment[kMaxEnvironmentObject];

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
