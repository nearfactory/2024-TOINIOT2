#include <cmath>

#include "DxLib.h"


#define PI 3.1415


#define WINDOW_W (1920)
#define WINDOW_H (1080)

#define CENTER_X (1920/2)
#define CENTER_Y (1080/2)



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



using namespace std;

struct Stick {
	int x = 0, y = 0;
	double rad = 0, len = 0;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetOutApplicationLogValidFlag(TRUE);	// output log
	ChangeWindowMode(TRUE);					//ウィンドウモード切替
	SetGraphMode(WINDOW_W, WINDOW_H, 32);	//ウィンドウサイズ決める
	SetMainWindowText("Radio Controller");	//ソフト名決める
	SetWindowStyleMode(7);					//画面最大化できるようにする
	SetWindowSizeChangeEnableFlag(TRUE);	//ウィンドウサイズ変えれるようにする
	SetAlwaysRunFlag(TRUE);					//ソフトがアクティブじゃなくても処理続行するようにする
	SetWindowSizeExtendRate(0.7f);			//起動時のウィンドウサイズを設定 ( 1 = 100%)
	SetUseMaskScreenFlag(TRUE);				//書かなくても良い。マスク使うときだけ書こう

	if (DxLib_Init() == -1) {
		return -1;
	}


	auto red = GetColor(230, 32, 32);
	auto blue = GetColor(32, 32, 230);
	auto white = GetColor(230, 230, 230);


	SetDrawScreen(DX_SCREEN_BACK);
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == FALSE) {
		// input
		auto num = GetJoypadNum();
		auto input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		DINPUT_JOYSTATE dinput;
		GetJoypadDirectInputState(DX_INPUT_PAD1, &dinput);

		Stick L{ dinput.X, dinput.Y }, R{ dinput.Rx, dinput.Ry };


		// compute



		// draw
		ClearDrawScreen();

		//DrawLineAA(0, CENTER_Y, WINDOW_W, CENTER_Y, blue);
		//DrawLineAA(CENTER_X, 0, CENTER_X, WINDOW_H, blue);



		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
		int r = 150;
		int ofs = r * 3;
		int font_size = 16;


		// button

		// back
		y = CENTER_Y - ofs;
		h = 50;

		// L
		if (input & BUTTON_L) {
			DrawBoxAA(CENTER_X - ofs - r, y, CENTER_X - ofs + r, y + h, red, TRUE);
		}
		DrawBoxAA(CENTER_X - ofs - r, y, CENTER_X - ofs + r, y + h, white, FALSE);
		DrawString(CENTER_X - ofs - GetDrawStringWidth("L", strlen("L")) / 2, y + h / 2 - font_size / 2, "L", white);

		// R
		if (input & BUTTON_R) {
			DrawBoxAA(CENTER_X + ofs - r, y, CENTER_X + ofs + r, y + h, red, TRUE);
		}
		DrawBoxAA(CENTER_X + ofs - r, y, CENTER_X + ofs + r, y + h, white, FALSE);
		DrawString(CENTER_X + ofs - GetDrawStringWidth("R", strlen("R")) / 2, y + h / 2 - font_size / 2, "R", white);


		// ABXY
		x = CENTER_X + ofs;
		y = CENTER_Y - r - 40;
		w = 40;
		r = 100;

		// A
		if (input & BUTTON_A) {
			DrawCircleAA(x + r, y, w, 96, red, TRUE);
		}
		DrawCircleAA(x + r, y, w, 96, white, FALSE);
		DrawString(x + r - GetDrawStringWidth("A", strlen("A")) / 2, y - font_size / 2, "A", white);

		// Y
		if (input & BUTTON_Y) {
			DrawCircleAA(x - r, y, w, 96, red, TRUE);
		}
		DrawCircleAA(x - r, y, w, 96, white, FALSE);
		DrawString(x - r - GetDrawStringWidth("Y", strlen("Y")) / 2, y - font_size / 2, "Y", white);


		// B
		if (input & BUTTON_B) {
			DrawCircleAA(x, y + r, w, 96, red, TRUE);
		}
		DrawCircleAA(x, y + r, w, 96, white, FALSE);
		DrawString(x - GetDrawStringWidth("B", strlen("B")) / 2, y + r - font_size / 2, "B", white);

		// X
		if (input & BUTTON_X) {
			DrawCircleAA(x, y - r, w, 96, red, TRUE);
		}
		DrawCircleAA(x, y - r, w, 96, white, FALSE);
		DrawString(x - GetDrawStringWidth("X", strlen("X")) / 2, y - r - font_size / 2, "X", white);


		// CROSS
		r = 150;
		x = CENTER_X - ofs;
		y = CENTER_Y + r + 40;
		w = 40;
		r = 100;

		if (L.x == 0 && L.y == 0) {
			// A
			if (input & BUTTON_RIGHT) {
				DrawCircleAA(x + r, y, w, 96, red, TRUE);
			}
			// Y
			if (input & BUTTON_LEFT) {
				DrawCircleAA(x - r, y, w, 96, red, TRUE);
			}
			// B
			if (input & BUTTON_DOWN) {
				DrawCircleAA(x, y + r, w, 96, red, TRUE);
			}
			// X
			if (input & BUTTON_UP) {
				DrawCircleAA(x, y - r, w, 96, red, TRUE);
			}
		}

		// A
		DrawCircleAA(x + r, y, w, 96, white, FALSE);
		DrawString(x + r - GetDrawStringWidth("→", strlen("→")) / 2, y - font_size / 2, "→", white);

		// Y
		DrawCircleAA(x - r, y, w, 96, white, FALSE);
		DrawString(x - r - GetDrawStringWidth("←", strlen("←")) / 2, y - font_size / 2, "←", white);


		// B
		DrawCircleAA(x, y + r, w, 96, white, FALSE);
		DrawString(x - GetDrawStringWidth("↓", strlen("↓")) / 2, y + r - font_size / 2, "↓", white);

		// X
		DrawCircleAA(x, y - r, w, 96, white, FALSE);
		DrawString(x - GetDrawStringWidth("↑", strlen("↑")) / 2, y - r - font_size / 2, "↑", white);


		// PLUS MINUS
		x = CENTER_X - ofs + r * 3;
		y = CENTER_Y - r - 40;
		w = 40;

		// MINUS
		if (input & BUTTON_MINUS) {
			DrawCircleAA(x, y, w, 96, red);
		}
		DrawCircleAA(x, y, w, 96, white, FALSE);
		DrawString(x - GetDrawStringWidth("ー", strlen("ー")) / 2, y - font_size / 2, "ー", white);

		// PLUS
		x = CENTER_X + ofs - r * 3;
		if (input & BUTTON_PLUS) {
			DrawCircleAA(x, y, w, 96, red);
		}
		DrawCircleAA(x, y, w, 96, white, FALSE);
		DrawString(x - GetDrawStringWidth("＋", strlen("＋")) / 2, y - font_size / 2, "＋", white);



		// stick
		// left
		r = 150;
		x = CENTER_X - ofs;
		y = CENTER_Y - r - 40;

		if (input & BUTTON_L_STICK) {
			DrawCircleAA(x, y, r, 96, red, TRUE);
		}
		DrawCircleAA(x, y, r, 96, white, FALSE);
		DrawLineAA(x - r, y, x + r, y, white);
		DrawLineAA(x, y - r, x, y + r, white);

		L.rad = atan2((double)L.y, (double)L.x);
		L.len = sqrt(L.x * L.x + L.y * L.y);
		if (L.len > 1000) L.len = 1000;
		DrawCircleAA(x + cos(L.rad) * L.len * r / 1000, y + sin(L.rad) * L.len * r / 1000, 32, 96, red, TRUE);


		// right
		x = CENTER_X + ofs;
		y = CENTER_Y + r + 40;

		if (input & BUTTON_R_STICK) {
			DrawCircleAA(x, y, r, 96, red, TRUE);
		}
		DrawCircleAA(x, y, r, 96, white, FALSE);
		DrawLineAA(x - r, y, x + r, y, white);
		DrawLineAA(x, y - r, x, y + r, white);

		R.rad = atan2((double)R.y, (double)R.x);
		R.len = sqrt(R.x * R.x + R.y * R.y);
		if (R.len > 1000) R.len = 1000;
		DrawCircleAA(x + cos(R.rad) * R.len * r / 1000, y + sin(R.rad) * R.len * r / 1000, 32, 96, red, TRUE);



		// debug
		char input_str[8 * sizeof(input) + 1] = "";
		for (int i = 0; i < 8 * sizeof(input); i++) {
			if (input >> (8 * sizeof(input) - i - 1) & 1) {
				input_str[i] = '1';
			}
			else {
				input_str[i] = '0';
			}
		}


		DrawFormatString(8, 8, white,
			"Num:%d\nInput:%s\n%d\n\nL\n x:%d\n\n y:%d\ndeg:%lf\nlen:%lf\nR\n x:%d\n y:%d\n",
			num, input_str, input, L.x, L.y, L.rad * 180 / PI, L.len, R.x, R.y);



		ScreenFlip();
	}

	DxLib_End();    // DXライブラリ終了処理

	return 0;
}