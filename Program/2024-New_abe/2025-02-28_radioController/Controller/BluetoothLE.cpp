#include"BluetoothLE.hpp"



uint64_t esp_addr = 125783837854410;

vector<Deviceinfo_t> device_list(0);
vector<uint64_t> addr_list(0);



// デバイス名を取得
wstring getDeviceName(uint64_t addr) {
	auto dev = BluetoothLEDevice::FromBluetoothAddressAsync(addr).get();
	if (dev == nullptr) {
		return L"Unnamed";
	}


	// これやると次のキャラクテリスティックの取得が早くなる（全部の情報をここで取得している？）
	auto gapServicesResult = dev.GetGattServicesForUuidAsync(GattServiceUuids::GenericAccess(), BluetoothCacheMode::Uncached).get();
	if (gapServicesResult.Status() == GattCommunicationStatus::Success) {
		IVectorView<GattDeviceService> gapServices = gapServicesResult.Services();
		if (gapServices.Size() > 0) {
			GattDeviceService genericAccessSvc = gapServices.GetAt(0);
			if (genericAccessSvc) {
				IVectorView<GattCharacteristic> gapDeviceNameChrs = genericAccessSvc.GetCharacteristics(GattCharacteristicUuids::GapDeviceName());
				if (gapDeviceNameChrs.Size() == 1) {
					GattCharacteristic gapDeviceNameChr = gapDeviceNameChrs.GetAt(0);

					GattReadResult readRes = gapDeviceNameChr.ReadValueAsync().get();
					if (readRes.Status() == GattCommunicationStatus::Success) {
						DataReader reader = DataReader::FromBuffer(readRes.Value());
						return reader.ReadString(reader.UnconsumedBufferLength()).c_str();
					}
				}
			}
		}
	}

	wstring name = dev.Name().c_str();
	if (name.empty()) {
		name = L"Unnamed Device";
	}

	return name;
}



// 受信時の動作
void onReceived(BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs) {
	auto addr = eventArgs.BluetoothAddress();
	if (addr_list.empty() || !(find(addr_list.begin(), addr_list.end(), addr) != addr_list.end())) {

		Deviceinfo_t device;

		// デバイス名
		device.name = getDeviceName(addr);

		// 基本情報
		device.uuid = addr;
		device.is_connectable = eventArgs.IsConnectable();
		device.strength = eventArgs.RawSignalStrengthInDBm();

		device_list.push_back(device);
		addr_list.push_back(addr);
	}


}