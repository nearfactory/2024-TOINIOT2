#include "DxLib.h"

#include "General.hpp"

#include "GamePad.hpp"


using namespace std;


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

		// gamepad
		DrawGamepadScreen(num, input, L, R);


		ScreenFlip();
	}

	DxLib_End();    // DXライブラリ終了処理

	return 0;
}