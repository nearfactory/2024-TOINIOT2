#include "general.hpp"


/*

UUID etc...
#define CENTRAL_NAME "TOINIOT2_CONTROLLER"
#define SERVICE_UUID "6a10c527-bac9-485e-bdb8-462f3b72bd59"
#define NOTIFY_UUID "07edec9a-716c-4379-b18b-2bb8eb1a2bea"
#define CIPO_UUID "d4b0e22e-2597-48e3-886e-ea984f3f7db0"
#define COPI_UUID "2234bf79-5ff8-4b47-a139-b9a20801eaf4"

*/


bool	 is_found = false;
uint64_t esp_addr = 260422049255626;


// ��M���̓���
vector<uint64_t> addr_list(0);

void onReceived(BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs) {
	//cout << "Received Advertisement!" << endl;

	auto addr = eventArgs.BluetoothAddress();
	if (addr_list.empty() || !(find(addr_list.begin(), addr_list.end(), addr) != addr_list.end()) && eventArgs.RawSignalStrengthInDBm() > -80) {
		if (addr == esp_addr) {
			cout << "esp32 found!" << endl;
		}


		// �f�o�C�X��
		cout << "(" << addr_list.size() << ") ";
		if (addr == esp_addr) {
			wcout << getDeviceName(addr) << endl;
		}
		else {
			cout << endl;
		}


		// ��{���
		cout << "  addr        :" << addr << endl;
		cout << "  connectable?:" << eventArgs.IsConnectable() << endl;
		cout << "  strength    :" << eventArgs.RawSignalStrengthInDBm() << "(dbm)" << endl;


		cout << endl;
		addr_list.push_back(addr);


		if (addr == esp_addr) {
			is_found = true;
		}
	}


}


int main() {
	cout << "pid:" << this_thread::get_id() << endl;
	cout << "initializing..." << endl;
	init_apartment();


	// �X�L��������
	BluetoothLEAdvertisementWatcher watcher;
	watcher.Received(&onReceived);	// ��M���̓����ݒ�



	// �A�h�o�^�C�Y�����g���X�L����
	watcher.Start();				// ��M���J�n Enter���������܂ő�����
	while (is_found == false);
	watcher.Stop();



	// �ʐM
	auto device = BluetoothLEDevice::FromBluetoothAddressAsync(esp_addr).get();
	auto result = device.GetGattServicesAsync(BluetoothCacheMode::Uncached).get();

	vector<void*> characteristics(0);
	

	// ������
	if (result.Status() == GattCommunicationStatus::Success) {
		auto services = result.Services();

		// �T�[�r�X���擾
		for (auto service : services) {
			auto uuid = service.Uuid();
			wcout << endl << L"  Service:" << ServiceToString(uuid) << endl;

			// �L�����N�^���X�e�B�b�N���擾
			auto result = service.GetCharacteristicsAsync().get();
			if (result.Status() == GattCommunicationStatus::Success) {
				auto _characteristics = result.Characteristics();

				for (auto characteristic : _characteristics) {
					auto uuid = characteristic.Uuid();
					wcout << L"  Characteristic:" << ServiceToString(uuid) << endl;

					// ��������
					DataWriter writer;
					writer.WriteString(L"fujiking!!\n");

					auto status = characteristic.WriteValueWithResultAsync(writer.DetachBuffer()).get();
					if (status.Status() == GattCommunicationStatus::Success) {
						cout << "write success!" << endl;
					}
					else {
						cout << "write failed!" << endl;
					}

					//characteristics.push_back(reinterpret_cast<void*>(&characteristic));
				}
			}


		}
	}
	else {
		cerr << "Failed to connect" << endl;
	}

	// �ʐM���I��
	device.Close();

	for (auto c : characteristics) {
		
	}
	


	return 0;

}