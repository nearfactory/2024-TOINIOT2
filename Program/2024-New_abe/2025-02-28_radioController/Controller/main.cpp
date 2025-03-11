#include "DxLib.h"

#include "BluetoothLE.hpp"
#include "General.hpp"
#include "GamePad.hpp"
#include "WstringToString.hpp"


using namespace std;



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetOutApplicationLogValidFlag(TRUE);	// output log
	ChangeWindowMode(TRUE);					//ウィンドウモード切替
	SetGraphMode(WINDOW_W, WINDOW_H, 32);	//ウィンドウサイズ決める
	SetMainWindowText("Radio Controller");	//ソフト名決める
	SetWindowStyleMode(7);					//画面最大化できるようにする
	SetWindowSizeChangeEnableFlag(TRUE);	//ウィンドウサイズ変えれるようにする
	SetAlwaysRunFlag(TRUE);					//ソフトがアクティブじゃなくても処理続行するようにする
	SetWindowSizeExtendRate(1.0f);			//起動時のウィンドウサイズを設定 ( 1 = 100%)
	SetUseMaskScreenFlag(TRUE);				//書かなくても良い。マスク使うときだけ書こう


	if (DxLib_Init() == -1) {
		return -1;
	}

	SetFontSize(font_size);


	
	// スキャン準備
	BluetoothLEAdvertisementWatcher watcher;
	watcher.Received(&onReceived);	// 受信時の動作を設定


	// アドバタイズメントをスキャン
	watcher.Start();				// 受信を開始 Enterが押されるまで続ける
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_RETURN) == FALSE && CheckHitKey(KEY_INPUT_ESCAPE) == FALSE) {
		ClearDrawScreen();
		DrawBox(0, 0, WINDOW_W, WINDOW_H, gray, TRUE);

		DrawString(8, 8, "Scanning advertisement...", white);

		wstring str = L"";

		for (const auto& device : device_list) {

			//str += "\nname: ";
			str += device.name;
			str += L"\n  uuid: ";
			str += to_wstring(device.uuid);
			str += L"\n  connectable: ";
			str += to_wstring(device.is_connectable);
			str += L"\n  strength: ";
			str += to_wstring(device.strength);
			str += L"\n\n";
		}

		DrawString(8, 64, WstringToString(str).c_str(), white);

		ScreenFlip();
	}
	watcher.Stop();



	SetDrawScreen(DX_SCREEN_BACK);
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == FALSE) {
		// input
		static auto prev_time = GetNowCount();

		auto num = GetJoypadNum();
		auto input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		DINPUT_JOYSTATE dinput;
		GetJoypadDirectInputState(DX_INPUT_PAD1, &dinput);

		Stick L{ dinput.X, dinput.Y }, R{ dinput.Rx, dinput.Ry };



		// compute



		// draw	
		ClearDrawScreen();
		DrawBox(0, 0, WINDOW_W, WINDOW_H, gray, TRUE);

		//DrawLineAA(0, CENTER_Y, WINDOW_W, CENTER_Y, blue);
		//DrawLineAA(CENTER_X, 0, CENTER_X, WINDOW_H, blue);

		// gamepad
		DrawGamepadScreen(num, input, L, R);

		// frame rate
		double frame_rate = 1000.0 / (GetNowCount() - prev_time);
		DrawFormatString(WINDOW_W - 8 - GetDrawFormatStringWidth("%d fps", (int)frame_rate), 8, white, "%d fps", (int)frame_rate);
		prev_time = GetNowCount();


		ScreenFlip();
	}

	DxLib_End();    // DXライブラリ終了処理

	return 0;
}