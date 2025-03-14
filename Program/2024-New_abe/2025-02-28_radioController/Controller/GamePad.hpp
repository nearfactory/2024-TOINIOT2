#pragma once

#include <cmath>

#include "DxLib.h"

#include "General.hpp"


#define BUTTON_DOWN		(1)
#define BUTTON_LEFT		(2)
#define BUTTON_RIGHT	(4)
#define BUTTON_UP		(8)

#define BUTTON_A		(16)
#define BUTTON_B		(32)
#define BUTTON_X		(64)
#define BUTTON_Y		(128)

#define BUTTON_L		(256)
#define BUTTON_R		(512)

#define BUTTON_MINUS	(1024)
#define BUTTON_PLUS		(2048)

#define BUTTON_L_STICK	(4096)
#define BUTTON_R_STICK	(8192)


struct Stick {
	int x = 0, y = 0;
	double rad = 0, len = 0;
};


void DrawGamepadScreen(int num, int input, Stick L, Stick R);