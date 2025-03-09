#include "DxLib.h"

#include "General.hpp"

#include "GamePad.hpp"


using namespace std;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetOutApplicationLogValidFlag(TRUE);	// output log
	ChangeWindowMode(TRUE);					//�E�B���h�E���[�h�ؑ�
	SetGraphMode(WINDOW_W, WINDOW_H, 32);	//�E�B���h�E�T�C�Y���߂�
	SetMainWindowText("Radio Controller");	//�\�t�g�����߂�
	SetWindowStyleMode(7);					//��ʍő剻�ł���悤�ɂ���
	SetWindowSizeChangeEnableFlag(TRUE);	//�E�B���h�E�T�C�Y�ς����悤�ɂ���
	SetAlwaysRunFlag(TRUE);					//�\�t�g���A�N�e�B�u����Ȃ��Ă��������s����悤�ɂ���
	SetWindowSizeExtendRate(0.7f);			//�N�����̃E�B���h�E�T�C�Y��ݒ� ( 1 = 100%)
	SetUseMaskScreenFlag(TRUE);				//�����Ȃ��Ă��ǂ��B�}�X�N�g���Ƃ�����������

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

	DxLib_End();    // DX���C�u�����I������

	return 0;
}