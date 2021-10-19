#include "cprocessing.h"
#pragma once

#define grid_Cols 3
#define grid_Rows 7
#define x_Origin 480.0f
#define y_Origin 270.0f

#define color_Black  CP_Color_Create(0, 0, 0, 255)
#define color_White CP_Color_Create(255, 255, 255, 255)
#define color_Red CP_Color_Create(255, 0, 0, 255)
#define color_Blue CP_Color_Create(0, 0, 255, 255)
#define color_Grey CP_Color_Create(128, 128, 128, 255)

#define game_Font CP_Font_Load("Assets/VT323-Regular.ttf")

float  gridWidth, gridHeight, gameWidth, gameHeight;