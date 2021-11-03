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

CP_Image basicGhostImageArray[3];
CP_Image fastGhostImageArray[3];
CP_Image fatGhostImageArray[3];
CP_Image grimReaperImageArray[3];

CP_Image tempStar;
CP_Image tempCircle;
CP_Image tempPercentage;
CP_Image tempBulletRadius;
CP_Image tempBullet;
CP_Image dirtTileImage;
CP_Image pauseButtonImage;
CP_Image titleWordImage;

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

struct SpriteSheetImage bluePortalArray[3];
struct SpriteSheetImage redPortalArray[3];
struct SpriteSheetImage basicTurretArray[6];
struct SpriteSheetImage homingMissleTurretArray[6];
struct SpriteSheetImage mineArray[2];
struct SpriteSheetImage energyWallArray[2];
struct SpriteSheetImage bulletArray[3];
struct SpriteSheetImage bulletRadiusArray[4];
struct SpriteSheetImage currencyArray[3];
struct SpriteSheetImage environmentObjectArray[8];
struct SpriteSheetImage backgroundArray[5];
struct SpriteSheetImage portalEnterEffectArray[5];
struct SpriteSheetImage portalSpawnEffectArray[5];
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
void RenderTurret(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY);
void RenderNormal(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY);
void RenderWithAlphaChanged(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY, int alphaValue);
void UpdatePortal(void);


