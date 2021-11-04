
#include "Gabriel.h"
void init_all_images(void)
{
	slowTurretImageArray[0] = CP_Image_Load("./Assets/SlowTurret1.png");
	slowTurretImageArray[1] = CP_Image_Load("./Assets/SlowTurret2.png");
	slowTurretImageArray[2] = CP_Image_Load("./Assets/SlowTurret3.png");
	slowTurretImageArray[3] = CP_Image_Load("./Assets/SlowTurret1.png");
	slowTurretImageArray[4] = CP_Image_Load("./Assets/SlowTurret4.png");
	slowTurretImageArray[5] = CP_Image_Load("./Assets/SlowTurret5.png");

	basicGhostImageArray[0] = CP_Image_Load("./Assets/BasicGhost1.png");
	basicGhostImageArray[1] = CP_Image_Load("./Assets/BasicGhost2.png");
	basicGhostImageArray[2] = CP_Image_Load("./Assets/BasicGhost3.png");

	fastGhostImageArray[0] = CP_Image_Load("./Assets/FastGhost1.png");
	fastGhostImageArray[1] = CP_Image_Load("./Assets/FastGhost2.png");
	fastGhostImageArray[2] = CP_Image_Load("./Assets/FastGhost3.png");

	fatGhostImageArray[0] = CP_Image_Load("./Assets/FatGhost1.png");
	fatGhostImageArray[1] = CP_Image_Load("./Assets/FatGhost2.png");
	fatGhostImageArray[2] = CP_Image_Load("./Assets/FatGhost3.png");

	grimReaperImageArray[0] = CP_Image_Load("./Assets/GrimReaper1.png");
	grimReaperImageArray[1] = CP_Image_Load("./Assets/GrimReaper2.png");
	grimReaperImageArray[2] = CP_Image_Load("./Assets/GrimReaper3.png");

	dirtTileImage = CP_Image_Load("./Assets/Dirt.png");
	pauseButtonImage = CP_Image_Load("./Assets/Pause.png");
	titleWordImage = CP_Image_Load("./Assets/Title.png");
	turretUIButton = CP_Image_Load("./Assets/TurretIcon.png");
	currencyUIRectangle = CP_Image_Load("./Assets/CurrencyIcon.png");

	basicGhostSpriteSheet = CP_Image_Load("./Assets/BasicGhost.png");
	fastGhostSpriteSheet = CP_Image_Load("./Assets/FastGhost.png");
	fatGhostSpriteSheet = CP_Image_Load("./Assets/FatGhost.png");
	grimReaperSpriteSheet = CP_Image_Load("./Assets/GrimReaper.png");
	bluePortalSpriteSheet = CP_Image_Load("./Assets/FriendlyPortal.png");
	redPortalSpriteSheet = CP_Image_Load("./Assets/GhostPortal.png");
	basicTurretSpriteSheet = CP_Image_Load("./Assets/BasicTurret.png");
	homingMissleTurretSpriteSheet = CP_Image_Load("./Assets/HomingMissleTurret.png");
	mineSpriteSheet = CP_Image_Load("./Assets/Mine.png");
	energyWallSpriteSheet = CP_Image_Load("./Assets/EnergyWall.png");
	bulletSpriteSheet = CP_Image_Load("./Assets/TurretBullets.png");
	bulletRadiusSpriteSheet = CP_Image_Load("./Assets/BulletsRadius.png");
	currencySpriteSheet = CP_Image_Load("./Assets/Currency.png");
	environmentObjectsSpriteSheet = CP_Image_Load("./Assets/Environment.png");
	backgroundSpriteSheet = CP_Image_Load("./Assets/Background.png");
	portalEnterEffectSpriteSheet = CP_Image_Load("./Assets/PortalEnterEffect.png");
	portalSpawnEffectSpriteSheet = CP_Image_Load("./Assets/EnemySpawnEffect.png");

}


void init_linkedlist_variables(void)
{
	portalCounter = 0;
	portalTimer = 0;
	bulletRadiusNodeKeyNumber = 0;
	bulletRadiusFirstNode = NULL;
	portalEnterFirstNode = NULL;
	portalEnterNodeKeyNumber = 0;
	portalSpawnFirstNode = NULL;
	portalSpawnNodeKeyNumber = 0;
	isPortalEffectSpawn = 0;
}

#pragma region LinkedList

void insert_new_node(struct node** list, float xPosInput, float yPosInput, int typeOfBullet)
{

	struct node* newNode;
	newNode = malloc(sizeof(struct node));
	if (newNode != NULL)
	{
		newNode->imageAlphaValue = 255;
		newNode->key = bulletRadiusNodeKeyNumber;
		newNode->next = bulletRadiusFirstNode;
		newNode->xPos = xPosInput;
		newNode->yPos = yPosInput;

		switch (typeOfBullet)
		{
		case 3:
			newNode->internalCounter = 3;
			break;
		case 2:
			newNode->internalCounter = 2;
			break;
		case 1:
			newNode->internalCounter = 1;
			break;
		case 0:
			newNode->internalCounter = 0;
			break;
		}

		newNode->next = *list;
		bulletRadiusNodeKeyNumber++;
		*list = newNode;
	}

}

void insert_new_node_portal(struct node** list, float xPosInput, float yPosInput,int portalEffect)
{

	struct node* newNode;
	newNode = malloc(sizeof(struct node));
	if (newNode != NULL)
	{
		switch (portalEffect)
		{
		case 0:
			newNode->key = portalSpawnNodeKeyNumber;
			portalSpawnNodeKeyNumber++;
			break;
		case 1:
			newNode->key = portalEnterNodeKeyNumber;
			portalEnterNodeKeyNumber++;
			break;
		}
		newNode->next = list;
		newNode->xPos = xPosInput;
		newNode->yPos = yPosInput;
		newNode->internalTimer = 0;
		newNode->internalCounter = 0;
		
		newNode->next = *list;
		*list = newNode;
		
	}

}

//delete a link with given key
struct node* delete_node(struct node* list, int key)
{
	struct node* currentNode;
	struct node* previousNode;
	for (currentNode = list, previousNode = NULL; currentNode != NULL && currentNode->key != key;
		previousNode = currentNode, currentNode = currentNode->next);
	if (currentNode == NULL)
	{
		return list;
	}

	if (previousNode == NULL)
	{
		list = list->next;
	}
	else
	{
		previousNode->next = currentNode->next;
	}
	free(currentNode);
	return list;

}

int isEmpty(struct node* currentNode) {
	return currentNode == NULL;
}
#pragma endregion


#pragma region SpriteSheetRelated

void init_spritesheet_array(void)
{
	SpriteSheetCalculation(basicGhostSpriteArray, basicGhostSpriteSheet, 128, 128, 1);
	SpriteSheetCalculation(fastGhostSpriteArray, fastGhostSpriteSheet, 128, 128, 1);
	SpriteSheetCalculation(fatGhostSpriteArray, fatGhostSpriteSheet, 128, 128, 1);
	SpriteSheetCalculation(grimReaperSpriteArray, grimReaperSpriteSheet, 128, 128, 1);
	SpriteSheetCalculation(bluePortalArray, bluePortalSpriteSheet, 128, 128, 1);
	SpriteSheetCalculation(redPortalArray, redPortalSpriteSheet, 128, 128, 1);
	SpriteSheetCalculation(basicTurretArray, basicTurretSpriteSheet, 128, 128, 0);
	SpriteSheetCalculation(mineArray, mineSpriteSheet, 128, 128, 0);
	SpriteSheetCalculation(energyWallArray, energyWallSpriteSheet, 128, 128, 0);
	SpriteSheetCalculation(homingMissleTurretArray, homingMissleTurretSpriteSheet, 128, 128, 0);
	SpriteSheetCalculation(bulletArray, bulletSpriteSheet, 128, 128, 1);
	SpriteSheetCalculation(bulletRadiusArray, bulletRadiusSpriteSheet, 128, 128, 0);
	SpriteSheetCalculation(currencyArray, currencySpriteSheet, 128, 128, 1);
	SpriteSheetCalculation(environmentObjectArray, environmentObjectsSpriteSheet, 128, 128, 1);
	SpriteSheetCalculation(backgroundArray, backgroundSpriteSheet,1920,1080, 1);
	SpriteSheetCalculation(portalEnterEffectArray, portalEnterEffectSpriteSheet, 128, 128, 0);
	SpriteSheetCalculation(portalSpawnEffectArray, portalSpawnEffectSpriteSheet, 128, 128, 0);
}

void SpriteSheetCalculation(struct SpriteSheetImage* s, CP_Image image, int pixelWidth, int pixelHeight, int stopPoint)
{

	int width = CP_Image_GetWidth(image);
	int height = CP_Image_GetHeight(image);
	int counter = 0;
	printf("test\n");
	for (int j = 0; j < height / pixelHeight; j++)
	{

		for (int i = 0; i < width / pixelWidth; i++)
		{
			if (!(j == height / pixelHeight- 1 && i > width / pixelWidth - stopPoint - 1))
			{
				s[counter].pixelSizeX= pixelWidth;
				s[counter].pixelSizeY = pixelHeight;
				s[counter].leftXPixel = (float)(i * s[counter].pixelSizeX);
				s[counter].rightXPixel = (float)((i + 1) * s[counter].pixelSizeX);
				s[counter].topYPixel = (float)(j * s[counter].pixelSizeY);
				s[counter].bottomYPixel = (float)((j + 1) * s[counter].pixelSizeY);
				counter++;
			}
		}
	}
}


void RenderPortal(struct SpriteSheetImage s, struct PortalVariables* pv, CP_Image image)
{
	CP_Image_DrawSubImage(image, pv->portalXPos, pv->portalYPos, pv->sizeX, pv->sizeY,
		s.leftXPixel, s.topYPixel, s.rightXPixel, s.bottomYPixel, 255);
}


void RenderTurret(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY)
{
	//put size of image as the turret size
	CP_Image_DrawSubImage(image, xPos, yPos, sizeOfImageX, sizeOfImageY,
		s.leftXPixel, s.topYPixel, s.rightXPixel, s.bottomYPixel, 255);
}

void RenderWithAlphaChanged(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY, int alphaValue)
{
	//put size of image as the turret size
	CP_Image_DrawSubImage(image, xPos, yPos, sizeOfImageX, sizeOfImageY,
		s.leftXPixel, s.topYPixel, s.rightXPixel, s.bottomYPixel, alphaValue);
}

void RenderNormal(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY)
{
	CP_Image_DrawSubImage(image, xPos, yPos, sizeOfImageX, sizeOfImageY,
		s.leftXPixel, s.topYPixel, s.rightXPixel, s.bottomYPixel, 255);
}

#pragma endregion

void render_bullet_circles(void)
{
	if (!isEmpty(bulletRadiusFirstNode))
	{
		struct node* current = bulletRadiusFirstNode;
		while (1)
		{

			if (current != NULL)
			{
				RenderWithAlphaChanged(bulletRadiusSpriteSheet, bulletRadiusArray[current->internalCounter], current->xPos, current->yPos,
					100.0f, 100.0f,current->imageAlphaValue);
				current->imageAlphaValue -= 50;
				if (current->imageAlphaValue < 0)
				{
					bulletRadiusFirstNode = delete_node(current, current->key);
					break;
				}
				if (current->next == NULL)
				{
					break;
				}

				else
				{
					current = current->next;
				}
			}

			else
			{
				break;
			}

		}
	}

}

void render_portal_effect(struct node* nodeToChange, int portalEffect)
{
	if (!isEmpty(nodeToChange))
	{
		struct node* current = nodeToChange;
		while (1)
		{

			if (current != NULL)
			{
				switch (portalEffect)
				{
				case 0:
					RenderNormal(portalSpawnEffectSpriteSheet, 
						portalSpawnEffectArray[current->internalCounter], 
						current->xPos, current->yPos,50, 50);
					break;
				case 1:
					RenderNormal(portalEnterEffectSpriteSheet,
						portalEnterEffectArray[current->internalCounter],
						current->xPos, current->yPos, 50, 50);
					break;
				}

				
				current->internalTimer += CP_System_GetDt();
				if (current->internalTimer > 0.25)
				{
					current->internalTimer = 0;
					if (current->internalCounter >= 3)
					{
						
						switch (portalEffect)
						{
						case 0:
							portalSpawnFirstNode = delete_node(current, current->key);
							break;
						case 1:
							portalEnterFirstNode = delete_node(current, current->key);
							break;
						}
						break;
					}
					else
					{
						current->internalCounter++;
					}
					
					
				}
				if (current->next == NULL)
				{
					break;
				}

				else
				{
					current = current->next;
				}
			}

			else
			{
				break;
			}

		}
	}

}

void render_all_portal_effects(void)
{
	render_portal_effect(portalEnterFirstNode,!isPortalEffectSpawn);
	render_portal_effect(portalSpawnFirstNode, isPortalEffectSpawn);
}


void UpdatePortal(void)
{
	if (portalTimer >= 0.25)
	{
		if (portalCounter == 2)
		{
			portalCounter = 0;
			portalTimer = 0;
		}

		else
		{
			portalCounter++;
			portalTimer = 0;
		}
	}
	portalTimer += CP_System_GetDt();
	RenderPortal(bluePortalArray[portalCounter], &portalVariablesArray[0], bluePortalSpriteSheet);
	RenderPortal(redPortalArray[portalCounter], &portalVariablesArray[1], redPortalSpriteSheet);

}