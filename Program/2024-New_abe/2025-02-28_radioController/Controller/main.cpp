#include "DxLib.h"

#include<sstream>

#include "BluetoothLE.hpp"
#include "General.hpp"
#include "GamePad.hpp"
#include "WstringToString.hpp"


using namespace std;



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetOutApplicationLogValidFlag(TRUE);	// output log
	ChangeWindowMode(TRUE);					//�E�B���h�E���[�h�ؑ�
	SetGraphMode(WINDOW_W, WINDOW_H, 32);	//�E�B���h�E�T�C�Y���߂�
	SetMainWindowText("Radio Controller");	//�\�t�g�����߂�
	SetWindowStyleMode(7);					//��ʍő剻�ł���悤�ɂ���
	SetWindowSizeChangeEnableFlag(TRUE);	//�E�B���h�E�T�C�Y�ς����悤�ɂ���
	SetAlwaysRunFlag(TRUE);					//�\�t�g���A�N�e�B�u����Ȃ��Ă��������s����悤�ɂ���
	SetWindowSizeExtendRate(1.0f);			//�N�����̃E�B���h�E�T�C�Y��ݒ� ( 1 = 100%)
	SetUseMaskScreenFlag(TRUE);				//�����Ȃ��Ă��ǂ��B�}�X�N�g���Ƃ�����������


	init_apartment(apartment_type::multi_threaded);	// ������
	if (DxLib_Init() == -1) {
		return -1;
	}

	SetFontSize(font_size);




	// �X�L��������
	BluetoothLEAdvertisementWatcher watcher;
	watcher.Received(&onReceived);	// ��M���̓����ݒ�


	// �A�h�o�^�C�Y�����g���X�L����
	watcher.Start();				// ��M���J�n Enter���������܂ő�����
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_RETURN) == FALSE && CheckHitKey(KEY_INPUT_ESCAPE) == FALSE && is_found == false) {
		//ClearDrawScreen();
		//DrawBox(0, 0, WINDOW_W, WINDOW_H, gray, TRUE);

		//DrawString(8, 8, "Scanning advertisement...", white);
		//wstring str = L"";

		//for (const auto& device : device_list) {

		//	//str += "\nname: ";
		//	//str += device.name;
		//	str += L"\n  uuid: ";
		//	str += to_wstring(device.uuid);
		//	str += L"\n  connectable: ";
		//	str += to_wstring(device.is_connectable);
		//	str += L"\n  strength: ";
		//	str += to_wstring(device.strength);
		//	str += L"\n\n";
		//}

		//DrawString(8, 64, WstringToString(str).c_str(), white);

		//ScreenFlip();
	}
	watcher.Stop();



	// �ʐM
	auto device = BluetoothLEDevice::FromBluetoothAddressAsync(esp_addr).get();	// err
	auto result = device.GetGattServicesAsync(BluetoothCacheMode::Uncached).get();
	IVectorView<GattCharacteristic> characteristics;

	// �ڑ�������
	if (result.Status() == GattCommunicationStatus::Success) {
		auto services = result.Services();

		// �T�[�r�X���擾
		for (auto service : services) {
			auto uuid = service.Uuid();
			//wcout << endl << L"  Service:" << ServiceToString(uuid) << endl;
			//service

			// �L�����N�^���X�e�B�b�N���擾
			auto result = service.GetCharacteristicsAsync().get();
			if (result.Status() == GattCommunicationStatus::Success) {
				characteristics = result.Characteristics();
			}

		}
	}
	else {
		DxLib_End();
		return -1;
	}



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

		// ���M���镶����̏���
		// -1024~1024 (11) -> -128 ~ 128 (8)
		stringstream ss;
		ss << L.x << " " << L.y << " " << R.x << " " << R.y << " " << (bool)(input & BUTTON_R) << " " << (bool)(input & BUTTON_Y);

		string send_str = ss.str();

		for (auto characteristic : characteristics) {
			sendWString(characteristic, L"D4B0E22E-2597-48E3-886E-EA984F3F7DB0", StringToWstring(send_str));
		}



		// draw	
		ClearDrawScreen();
		DrawBox(0, 0, WINDOW_W, WINDOW_H, gray, TRUE);

		//DrawLineAA(0, CENTER_Y, WINDOW_W, CENTER_Y, blue);
		//DrawLineAA(CENTER_X, 0, CENTER_X, WINDOW_H, blue);

		// gamepad
		DrawGamepadScreen(num, input, L, R);


		// send str
		string draw_str = "";
		for (auto c : send_str) {
			for (int i = 0; i < sizeof(c) * 8; i++) {
				if ((c >> (8 - 1 - i)) & 1) {
					draw_str += "1";
				}
				else {
					draw_str += "0";
				}
			}
			draw_str += " ";
		}

		DrawString(8, WINDOW_H - 8 - font_size, draw_str.c_str(), white);

		// frame rate
		double frame_rate = 1000.0 / (GetNowCount() - prev_time);
		DrawFormatString(WINDOW_W - 8 - GetDrawFormatStringWidth("%d fps", (int)frame_rate), 8, white, "%d fps", (int)frame_rate);
		prev_time = GetNowCount();



		ScreenFlip();
	}

	device.Close();
	DxLib_End();    // DX���C�u�����I������

	return 0;
}
