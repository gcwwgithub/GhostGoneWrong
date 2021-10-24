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

struct node
{
	int circleAlphaValue;
	int key;
	float xPos;
	float yPos;
	struct node* next;
};

struct node* firstNode;
int keyNumber;

void insert_new_node(struct node**list, float xPos, float yPos);
struct node* delete_node(struct node* list,int key);
int isEmpty(void);


void render_bullet_circles();


void init_all_images(void);