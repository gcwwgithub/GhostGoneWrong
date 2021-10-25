#pragma once
#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>

CP_Image triangleTurretImageArray[6];
CP_Image redArrowImageArray[3];
CP_Image bulletArray[4];
CP_Image bulletRadiusArrray[4];
CP_Image tempStar;
CP_Image tempCircle;
CP_Image tempPercentage;
CP_Image tempBulletRadius;
CP_Image tempBullet;
CP_Image testIMG;
CP_Image myImage;
CP_Image bluePortalSpriteSheet;
CP_Image redPortalSpriteSheet;

int portalCounter;
float portalTimer;

struct node
{
	int circleAlphaValue;
	int key;
	float xPos;
	float yPos;
	struct node* next;
};

struct SpreadSheetImage
{
	int pixelOfImage;
	float leftXPixel, rightXPixel, topYPixel, bottomYPixel;
};

struct SpreadSheetImage bluePortalArray[3];
struct SpreadSheetImage redPortalArray[3];

struct PortalVariables
{
	float portalXPos;
	float portalYPos;
	float sizeX;
	float sizeY;
};
struct PortalVariables portalVariablesArray[2];

struct node* firstNode;
int keyNumber;

void insert_new_node(struct node**list, float xPos, float yPos);
struct node* delete_node(struct node* list,int key);
int isEmpty(void);

void render_bullet_circles();

void SpreadsheetCalculation(struct SpreadSheetImage* s, CP_Image image, int pixel, int stopPoint);
void SpreadsheetUpdate(float* timer, float elapsedTime, CP_Image image, float xPos, float yPos);
void SpreadsheetInit(void);
void SpreadsheetDraw(struct SpreadSheetImage s, struct PortalVariables* pv, CP_Image image);
void UpdatePortal(void);
void init_all_images(void);

