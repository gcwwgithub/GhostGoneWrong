#include"game.h"

#ifndef GhostGoneWrong_CURRENTHEADERFILES_ZHENGWEI_H
#define GhostGoneWrong_CURRENTHEADERFILES_ZHENGWEI_H

#define COLOR_BLUE CP_Color_Create(0, 0, 255, 255)
// The turret selected to place
TurretType is_placing_turret;
//Check if power up menu is selected
int power_up_menu;
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
	kMaxEnvironmentObject = 10
};//Using enum instead of define for const int
extern const float kLeftGameMenuXWidth;
extern const float kRightGameMenuXOrigin;
//Environment
Coordinates Environment[kMaxEnvironmentObject];

void MouseInit(void);
void EnvironmentInit(LevelData* LevelX);
void PowerUpPriceInit(void);
void Level1Init(void);
void Level2Init(void);
void Level3Init(void);
void Level4Init(void);
void Level5Init(void);
void PathFindingReset(LevelData* LevelX);
void PathFindingCalculateCost(LevelData* LevelX);
void PathFindingUpdate(LevelData* LevelX);
void ButtonPressedUpdate(void);
void RenderGameGrid(void);
void RenderEnemyPath(LevelData* LevelX);
void RenderEnvironment(void);
void RenderTurretDetailsDisplay(void);
void render_turret_menu_object(
	Coordinates menuObjectX, enum MenuObjectType type);

#endif // ! GhostGoneWrong_CURRENTHEADERFILES_ZHENGWEI_H
