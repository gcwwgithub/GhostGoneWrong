#pragma once
#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>


struct SpriteSheetImage
{
	int pixelSizeX,pixelSizeY;
	float leftXPixel, rightXPixel, topYPixel, bottomYPixel;
};

struct node
{
	struct node* next;
	int key;
	float xPos;
	float yPos;
	int imageAlphaValue;
	float internalTimer;
	int internalCounter;
};


struct PortalVariables
{
	float portalXPos;
	float portalYPos;
	float sizeX;
	float sizeY;
};

CP_Image slowTurretImageArray[6];

CP_Image pauseButtonImage;
CP_Image titleWordImage;
CP_Image turretUIButton;
CP_Image backgroundUIThin;
CP_Image turretUpgradeBackground;

CP_Image basicGhostSpriteSheet;
CP_Image fastGhostSpriteSheet;
CP_Image fatGhostSpriteSheet;
CP_Image grimReaperSpriteSheet;
CP_Image bluePortalSpriteSheet;
CP_Image redPortalSpriteSheet;
CP_Image basicTurretSpriteSheet;
CP_Image homingMissleTurretSpriteSheet;
CP_Image mineSpriteSheet;
CP_Image energyWallSpriteSheet;
CP_Image bulletSpriteSheet;
CP_Image bulletRadiusSpriteSheet;
CP_Image currencySpriteSheet;
CP_Image environmentObjectsSpriteSheet;
CP_Image backgroundSpriteSheet;
CP_Image portalEnterEffectSpriteSheet;
CP_Image portalSpawnEffectSpriteSheet;
CP_Image powerUpIconSpriteSheet;
CP_Image battlefieldEffectIconSpriteSheet;
CP_Image backgroundUIFatSpriteSheet;
CP_Image interactableButtonsImageSpriteSheet;
CP_Image turretStatsIconSpriteSheet;
CP_Image nonGridEnvironmentObjectsSpriteSheet;

struct SpriteSheetImage basicGhostSpriteArray[3];
struct SpriteSheetImage fastGhostSpriteArray[3];
struct SpriteSheetImage fatGhostSpriteArray[3];
struct SpriteSheetImage grimReaperSpriteArray[3];
struct SpriteSheetImage bluePortalArray[3];
struct SpriteSheetImage redPortalArray[3];
struct SpriteSheetImage basicTurretArray[6];
struct SpriteSheetImage homingMissleTurretArray[6];
struct SpriteSheetImage mineArray[2];
struct SpriteSheetImage energyWallArray[2];
struct SpriteSheetImage bulletArray[3];
struct SpriteSheetImage bulletRadiusArray[4];
struct SpriteSheetImage currencyArray[5];
struct SpriteSheetImage environmentObjectArray[9];
struct SpriteSheetImage backgroundArray[5];
struct SpriteSheetImage portalEnterEffectArray[4];
struct SpriteSheetImage portalSpawnEffectArray[4];
struct SpriteSheetImage powerUpIconArray[4];
struct SpriteSheetImage battlefieldEffectIconArray[12];
struct SpriteSheetImage backgroundUIFatArray[2];
struct SpriteSheetImage interactableButtonsImageArray[3];
struct SpriteSheetImage turretStatsIconArray[5];
struct SpriteSheetImage nonGridEnvironmentObjectsArray[3];

struct PortalVariables portalVariablesArray[2];

struct node* bulletRadiusFirstNode;
int bulletRadiusNodeKeyNumber;

struct node* portalEnterFirstNode;
struct node* portalSpawnFirstNode;
int portalEnterNodeKeyNumber;
int portalSpawnNodeKeyNumber;

int portalCounter;
float portalTimer;
int isPortalEffectSpawn;

void insert_new_node(struct node**list, float xPos, float yPos, int typeOfBullet);
void insert_new_node_portal(struct node** list, float xPosInput, float yPosInput, int portalEffect);
struct node* delete_node(struct node* list,int key);
int isEmpty(void);

void init_all_images(void);
void init_linkedlist_variables(void);
void render_bullet_circles(void);
void render_portal_effect(struct node* nodeToChange, int portalEffect);
void render_all_portal_effects(void);
void init_spritesheet_array(void);

void SpriteSheetCalculation(struct SpriteSheetImage* s, CP_Image image, int pixelWidth, int pixelHeight, int stopPoint);
void RenderPortal(struct SpriteSheetImage s, struct PortalVariables* pv, CP_Image image);
void RenderNormal(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY);
void RenderWithAlphaChanged(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY, int alphaValue);
void update_portal(void);
void render_game_background(int currentLevel);

