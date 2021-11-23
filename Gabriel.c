
#include "Gabriel.h"
#include "game.h"

void init_all_images(void)
{
	slowTurretImageArray[0] = CP_Image_Load("./Assets/SlowTurret1.png");
	slowTurretImageArray[1] = CP_Image_Load("./Assets/SlowTurret2.png");
	slowTurretImageArray[2] = CP_Image_Load("./Assets/SlowTurret3.png");
	slowTurretImageArray[3] = CP_Image_Load("./Assets/SlowTurret1.png");
	slowTurretImageArray[4] = CP_Image_Load("./Assets/SlowTurret4.png");
	slowTurretImageArray[5] = CP_Image_Load("./Assets/SlowTurret5.png");


	pauseButtonImage = CP_Image_Load("./Assets/Pause.png");
	titleWordImage = CP_Image_Load("./Assets/Title.png");
	turretUIButton = CP_Image_Load("./Assets/TurretIcon.png");
	backgroundUIThin = CP_Image_Load("./Assets/CurrencyIcon.png");
	turretUpgradeBackground = CP_Image_Load("./Assets/UpgradeMenu.png");

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
	environmentObjectsSpriteSheet = CP_Image_Load("./Assets/GridEnvironmentObject.png");
	backgroundSpriteSheet = CP_Image_Load("./Assets/Background.png");
	portalEnterEffectSpriteSheet = CP_Image_Load("./Assets/PortalEnterEffect.png");
	portalSpawnEffectSpriteSheet = CP_Image_Load("./Assets/EnemySpawnEffect.png");
	powerUpIconSpriteSheet = CP_Image_Load("./Assets/Powerup.png");
	battlefieldEffectIconSpriteSheet = CP_Image_Load("./Assets/BattlefieldEffects.png");
	backgroundUIFatSpriteSheet = CP_Image_Load("./Assets/BigUIDisplay.png");
	interactableButtonsImageSpriteSheet = CP_Image_Load("./Assets/InteractableButtons.png");
	turretStatsIconSpriteSheet = CP_Image_Load("./Assets/TurretStatsIcons.png");
	nonGridEnvironmentObjectsSpriteSheet = CP_Image_Load("./Assets/EnvBackgroundObject.png");
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
	battleFieldSizeTextMax = 45 * scalingFactor;
	battleFieldSizeTextMin = 0;
	battleFieldTimer = 0;
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
		newNode->next = *list;
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
	SpriteSheetCalculation(environmentObjectArray, environmentObjectsSpriteSheet, 128, 128, 2);
	SpriteSheetCalculation(backgroundArray, backgroundSpriteSheet,1920,1080, 0);
	SpriteSheetCalculation(portalEnterEffectArray, portalEnterEffectSpriteSheet, 128, 128, 0);
	SpriteSheetCalculation(portalSpawnEffectArray, portalSpawnEffectSpriteSheet, 128, 128, 0);
	SpriteSheetCalculation(powerUpIconArray, powerUpIconSpriteSheet, 128, 128, 0);
	SpriteSheetCalculation(battlefieldEffectIconArray, battlefieldEffectIconSpriteSheet, 128, 128, 0);
	SpriteSheetCalculation(backgroundUIFatArray, backgroundUIFatSpriteSheet, 128, 128, 0);
	SpriteSheetCalculation(interactableButtonsImageArray, interactableButtonsImageSpriteSheet, 128, 72, 1);
	SpriteSheetCalculation(turretStatsIconArray, turretStatsIconSpriteSheet, 128, 128, 1);
	SpriteSheetCalculation(nonGridEnvironmentObjectsArray, nonGridEnvironmentObjectsSpriteSheet, 512, 512, 0);
}

void SpriteSheetCalculation(struct SpriteSheetImage* s, CP_Image image, int pixelWidth, int pixelHeight, int stopPoint)
{

	int width = CP_Image_GetWidth(image);
	int height = CP_Image_GetHeight(image);
	int counter = 0;
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

void RenderWithAlphaChanged(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY, int alphaValue)
{
	//put size of image as the turret size
	CP_Image_DrawSubImage(image, xPos, yPos, sizeOfImageX, sizeOfImageY,
		s.leftXPixel, s.topYPixel, s.rightXPixel, s.bottomYPixel, alphaValue);
}

void RenderNormal(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY)
{
	RenderWithAlphaChanged(image, s, xPos, yPos, sizeOfImageX, sizeOfImageY,255);
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

void update_portal(void)
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

void render_game_background(int currentLevel)
{
	switch (currentLevel)
	{
	case 0:
		# pragma region Level 1 Objects
		CP_Settings_Tint(CP_Color_Create(0, 0, 0, 75));
		RenderWithAlphaChanged(backgroundSpriteSheet, backgroundArray[1], CP_System_GetWindowWidth() * 0.5f,
			CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[1], 
			CP_System_GetWindowWidth() * 0.85f, CP_System_GetWindowHeight() * 0.85f, 
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowWidth() * 0.3f);

		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.11f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.65f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.75f, CP_System_GetWindowHeight() * 0.11f,
			128 * scalingFactor, 128 * scalingFactor, 200);

		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.89f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.45f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.55f, CP_System_GetWindowHeight() * 0.89f,
			128 * scalingFactor, 128 * scalingFactor, 200);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[2],
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.3f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[2],
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.5f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[2],
			CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.6f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[2],
			CP_System_GetWindowWidth() * 0.85f, CP_System_GetWindowHeight() * 0.5f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.95f, CP_System_GetWindowHeight() * 0.55f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.69f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.75f,
			64 * scalingFactor, 64 * scalingFactor);
#pragma endregion
		break;
	case 1:
		#pragma region Level 2 Objects
		CP_Settings_Tint(CP_Color_Create(0, 0, 0, 75));
		RenderWithAlphaChanged(backgroundSpriteSheet, backgroundArray[2], CP_System_GetWindowWidth() * 0.5f,
			CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderWithAlphaChanged(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[0],
			CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);


		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[2],
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.3f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[2],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.4f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[2],
			CP_System_GetWindowWidth() * 0.22f, CP_System_GetWindowHeight() * 0.72f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[2],
			CP_System_GetWindowWidth() * 0.65f, CP_System_GetWindowHeight() * 0.50f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[4],
			CP_System_GetWindowWidth() * 0.85f, CP_System_GetWindowHeight() * 0.45f,
			128 * scalingFactor, 128 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[4],
			CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[4],
			CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[6],
			CP_System_GetWindowWidth() * 0.45f, CP_System_GetWindowHeight() * 0.85f,
			80 * scalingFactor, 64 * scalingFactor);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[6],
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.75f,
			80 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.55f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.5f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.75f, CP_System_GetWindowHeight() * 0.3f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.85f, CP_System_GetWindowHeight() * 0.65f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.6f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[9],
			CP_System_GetWindowWidth() * 0.8f, CP_System_GetWindowHeight() * 0.8f,
			64 * scalingFactor, 64 * scalingFactor);
#pragma endregion
		break;
	case 2:
		#pragma region Level 3 Objects
		CP_Settings_Tint(CP_Color_Create(0, 0, 0, 75));
		RenderWithAlphaChanged(backgroundSpriteSheet, backgroundArray[3], CP_System_GetWindowWidth() * 0.5f,
			CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 200);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[3],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.19f,
			300* scalingFactor, 300 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[7],
			CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.65f,
			200 * scalingFactor, 200 * scalingFactor);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.5f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.65f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.85f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.2f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.75f, CP_System_GetWindowHeight() * 0.4f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.8f, CP_System_GetWindowHeight() * 0.85f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.65f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[9],
			CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.8f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[9],
			CP_System_GetWindowWidth() * 0.65f, CP_System_GetWindowHeight() * 0.5f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[9],
			CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[9],
			CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.1f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[9],
			CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.6f, CP_System_GetWindowHeight() * 0.1f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.1f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.4f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.4f,
			64 * scalingFactor, 64 * scalingFactor);
#pragma endregion
		break;
	case 3:
		#pragma region Level 4 Objects
		CP_Settings_Tint(CP_Color_Create(0, 0, 0, 75));
		RenderWithAlphaChanged(backgroundSpriteSheet, backgroundArray[4], CP_System_GetWindowWidth() * 0.5f,
			CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 200);
		
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.15f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.25f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.4f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.14f, CP_System_GetWindowHeight() * 0.65f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[5],
			CP_System_GetWindowWidth() * 0.23f, CP_System_GetWindowHeight() * 0.8f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[9],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.65f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[9],
			CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[9],
			CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.1f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.1f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[8],
			CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);

		for (int i = 0; i < 8; i++)
		{
			RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[4],
				CP_System_GetWindowWidth() * 0.7f, (CP_System_GetWindowHeight() * (0.05f + i*0.12f)),
				48 * scalingFactor, 48 * scalingFactor);
			RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[5],
				CP_System_GetWindowWidth() * 0.7f, (CP_System_GetWindowHeight() * (0.1f + i * 0.12f)),
				48 * scalingFactor, 48 * scalingFactor);
		}
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[1],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.85f),
			48 * scalingFactor, 48 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[1],
			CP_System_GetWindowWidth() * 0.87f, (CP_System_GetWindowHeight() * 0.85f),
			48 * scalingFactor, 48 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[1],
			CP_System_GetWindowWidth() * 0.89f, (CP_System_GetWindowHeight() * 0.85f),
			48 * scalingFactor, 48 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[3],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.65f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[3],
			CP_System_GetWindowWidth() * 0.75f, (CP_System_GetWindowHeight() * 0.45f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[0],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.45f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[2],
			CP_System_GetWindowWidth() * 0.3f, (CP_System_GetWindowHeight() * 0.5f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[6],
			CP_System_GetWindowWidth() * 0.5f, (CP_System_GetWindowHeight() * 0.15f),
			56 * scalingFactor, 56 * scalingFactor);

#pragma endregion
		break;
	case 4:
		#pragma region Level 5 Objects

		CP_Settings_Tint(CP_Color_Create(0, 0, 0, 75));
		RenderWithAlphaChanged(backgroundSpriteSheet, backgroundArray[5], CP_System_GetWindowWidth() * 0.5f,
			CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 200);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[8],
			CP_System_GetWindowWidth() * 0.85f, CP_System_GetWindowHeight() * 0.85f,
			75 * scalingFactor, 75 * scalingFactor);

		RenderNormal(nonGridEnvironmentObjectsSpriteSheet, nonGridEnvironmentObjectsArray[8],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.35f,
			75 * scalingFactor, 75 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[3],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.45f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[7],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.55f),
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[6],
			CP_System_GetWindowWidth() * 0.9f, (CP_System_GetWindowHeight() * 0.6f),
			48 * scalingFactor, 48 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[6],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.65f),
			48 * scalingFactor, 48 * scalingFactor);


		for (int i = 0; i < 8; i++)
		{
			RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[4],
				CP_System_GetWindowWidth() * 0.3f, (CP_System_GetWindowHeight() * (0.05f + i * 0.12f)),
				48 * scalingFactor, 48 * scalingFactor);
			RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[5],
				CP_System_GetWindowWidth() * 0.3f, (CP_System_GetWindowHeight() * (0.1f + i * 0.12f)),
				48 * scalingFactor, 48 * scalingFactor);
			RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[4],
				CP_System_GetWindowWidth() * 0.7f, (CP_System_GetWindowHeight() * (0.05f + i * 0.12f)),
				48 * scalingFactor, 48 * scalingFactor);
			RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[5],
				CP_System_GetWindowWidth() * 0.7f, (CP_System_GetWindowHeight() * (0.1f + i * 0.12f)),
				48 * scalingFactor, 48 * scalingFactor);

		}
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[7],
			CP_System_GetWindowWidth() * 0.15f, (CP_System_GetWindowHeight() * 0.1f),
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[6],
			CP_System_GetWindowWidth() * 0.2f, (CP_System_GetWindowHeight() * 0.1f),
			48 * scalingFactor, 48 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[6],
			CP_System_GetWindowWidth() * 0.15f, (CP_System_GetWindowHeight() * 0.15f),
			48 * scalingFactor, 48 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[7],
			CP_System_GetWindowWidth() * 0.15f, (CP_System_GetWindowHeight() * 0.8f),
			64 * scalingFactor, 64 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[6],
			CP_System_GetWindowWidth() * 0.2f, (CP_System_GetWindowHeight() * 0.8f),
			48 * scalingFactor, 48 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[6],
			CP_System_GetWindowWidth() * 0.15f, (CP_System_GetWindowHeight() * 0.85f),
			48 * scalingFactor, 48 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[0],
			CP_System_GetWindowWidth() * 0.4f, (CP_System_GetWindowHeight() * 0.15f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[1],
			CP_System_GetWindowWidth() * 0.35f, (CP_System_GetWindowHeight() * 0.2f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[1],
			CP_System_GetWindowWidth() * 0.45f, (CP_System_GetWindowHeight() * 0.15f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[0],
			CP_System_GetWindowWidth() * 0.5f, (CP_System_GetWindowHeight() * 0.1f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[2],
			CP_System_GetWindowWidth() * 0.6f, (CP_System_GetWindowHeight() * 0.15f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[3],
			CP_System_GetWindowWidth() * 0.55f, (CP_System_GetWindowHeight() * 0.15f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[3],
			CP_System_GetWindowWidth() * 0.65f, (CP_System_GetWindowHeight() * 0.12f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[1],
			CP_System_GetWindowWidth() * 0.41f, (CP_System_GetWindowHeight() * 0.85f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[0],
			CP_System_GetWindowWidth() * 0.35f, (CP_System_GetWindowHeight() * 0.9f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[1],
			CP_System_GetWindowWidth() * 0.42f, (CP_System_GetWindowHeight() * 0.94f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[3],
			CP_System_GetWindowWidth() * 0.52f, (CP_System_GetWindowHeight() * 0.8f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[2],
			CP_System_GetWindowWidth() * 0.63f, (CP_System_GetWindowHeight() * 0.91f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[1],
			CP_System_GetWindowWidth() * 0.51f, (CP_System_GetWindowHeight() * 0.83f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderNormal(environmentObjectsSpriteSheet, environmentObjectArray[3],
			CP_System_GetWindowWidth() * 0.65f, (CP_System_GetWindowHeight() * 0.82f),
			56 * scalingFactor, 56 * scalingFactor);
#pragma endregion
		break;
		
	}
	
}


float BattlefieldEffectLinear(float start, float end, float value)
{
	return (1.f - value) * start + value * end;
}

void StartBattleFieldEffectTimer(int effect)
{
	battleFieldTimer = 0;
	battleFieldSizeTextMin = 0;
	switch (effect)
	{
	case 1:
		battleFieldSizeTextMax = 60 * scalingFactor;//
		break;
	case 2:
		battleFieldSizeTextMax = 60 * scalingFactor;//
		break;
	case 3:
		battleFieldSizeTextMax = 60 * scalingFactor;
		break;
	case 4:
		battleFieldSizeTextMax = 60 * scalingFactor;
		break;
	case 5:
		battleFieldSizeTextMax = 50 * scalingFactor;
		break;
	case 6:
		battleFieldSizeTextMax = 50* scalingFactor;
		break;
	case 7:
		battleFieldSizeTextMax = 60 * scalingFactor;
		break;
	case 8:
		battleFieldSizeTextMax = 60 * scalingFactor;
		break;
	case 9:
		battleFieldSizeTextMax = 45 * scalingFactor;
		break;
	case 10:
		battleFieldSizeTextMax = 45 * scalingFactor;
		break;
	case 11:
		battleFieldSizeTextMax = 55 * scalingFactor;
		break;
	}
}
void RenderBattlefieldEffect(int effect)
{
	char temp[100];
	static float timer = 0;
	float sizeTimer = 0;
	float duration = 0.5;
	float stayOnScreenDuration = 3;

	if (battleFieldTimer <= (stayOnScreenDuration + 2 * duration))
	{
		timer += CP_System_GetDt();
		battleFieldTimer += CP_System_GetDt();
		sizeTimer = timer;
		if (timer >= duration)
		{
			sizeTimer = duration;
			if (timer >= duration + stayOnScreenDuration)
			{
				float temp = 0;
				temp = battleFieldSizeTextMax;
				battleFieldSizeTextMax = battleFieldSizeTextMin;
				battleFieldSizeTextMin = temp;
				timer = 0;
			}
		}

		float currentSize = BattlefieldEffectLinear(battleFieldSizeTextMin, battleFieldSizeTextMax, sizeTimer/ duration);

		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(currentSize);
		switch (effect)
		{
		case 1:
			sprintf_s(temp, sizeof(temp), "MORE QUARTZ EARNED THIS WAVE");
			break;
		case 2:
			sprintf_s(temp, sizeof(temp), "LESS QUARTZ EARNED THIS WAVE");
			break;
		case 3:
			sprintf_s(temp, sizeof(temp), "FASTER ENEMIES THIS WAVE");
			break;
		case 4:
			sprintf_s(temp, sizeof(temp), "SLOWER ENEMIES THIS WAVE");
			break;
		case 5:
			sprintf_s(temp, sizeof(temp), "TURRET DAMAGE INCREASED THIS WAVE");
			break;
		case 6:
			sprintf_s(temp, sizeof(temp), "TURRET DAMAGE DECREASED THIS WAVE");
			break;
		case 7:
			sprintf_s(temp, sizeof(temp), "GHOST HAS MORE HP THIS WAVE");
			break;
		case 8:
			sprintf_s(temp, sizeof(temp), "GHOST HAS LESS HP THIS WAVE");
			break;
		case 9:
			sprintf_s(temp, sizeof(temp), "TURRET ATTACK SPEED INCREASED THIS WAVE");
			break;
		case 10:
			sprintf_s(temp, sizeof(temp), "TURRET ATTACK SPEED DECREASED THIS WAVE");
			break;
		case 11:
			sprintf_s(temp, sizeof(temp), "NO QUARTZ WILL DROP THIS WAVE");
			break;
		}


		CP_Font_DrawText(temp, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 5);
	}

	else
	{
		timer = 0;
		
	}




}