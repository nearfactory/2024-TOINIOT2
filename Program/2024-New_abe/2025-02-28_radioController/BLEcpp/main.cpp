#include<iostream>
#include<sstream>
#include<iomanip>
#include<condition_variable>
#include<mutex>

#include<algorithm>
#include<vector>

#include<winrt/windows.foundation.h>
#include<winrt/windows.foundation.collections.h>
#include<winrt/windows.devices.bluetooth.h>
#include<winrt/windows.devices.bluetooth.advertisement.h>
#include<winrt/windows.devices.bluetooth.genericattributeprofile.h>
#include<winrt/windows.devices.enumeration.h>
#include<winrt/windows.storage.streams.h>


using namespace std;

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Devices::Bluetooth::Advertisement;
using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;
using namespace Windows::Storage::Streams;


auto esp_addr = "125783837854410";


std::wstring UUIDToString(const winrt::guid& uuid)
{
	std::wstringstream str;
	str << std::uppercase << std::hex;
	str << std::setw(8) << std::setfill(L'0') << uuid.Data1 << L"-";
	str << std::setw(4) << std::setfill(L'0') << uuid.Data2 << L"-";
	str << std::setw(4) << std::setfill(L'0') << uuid.Data3 << L"-";
	str << std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[0])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[1])
		<< '-'
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[2])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[3])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[4])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[5])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[6])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[7]);
	str << std::nouppercase;
	return str.str();

}

std::wstring ServiceToString(const winrt::guid& uuid)
{
	if (uuid == GattServiceUuids::AlertNotification())
	{
		return L"Alert Notification";
	}
	if (uuid == GattServiceUuids::Battery())
	{
		return L"Battery";
	}
	if (uuid == GattServiceUuids::BloodPressure())
	{
		return L"Blood Pressure";
	}
	if (uuid == GattServiceUuids::CurrentTime())
	{
		return L"Current Time";
	}
	if (uuid == GattServiceUuids::CyclingPower())
	{
		return L"Cycling Power";
	}
	if (uuid == GattServiceUuids::CyclingSpeedAndCadence())
	{
		return L"Cycling Speed and Cadence";
	}
	if (uuid == GattServiceUuids::DeviceInformation())
	{
		return L"Device Information";
	}
	if (uuid == GattServiceUuids::GenericAccess())
	{
		return L"Generic Access";
	}
	if (uuid == GattServiceUuids::GenericAttribute())
	{
		return L"Generic Attribute";
	}
	if (uuid == GattServiceUuids::Glucose())
	{
		return L"Glucose";
	}
	if (uuid == GattServiceUuids::HealthThermometer())
	{
		return L"Health Thermometer";
	}
	if (uuid == GattServiceUuids::HeartRate())
	{
		return L"Heart Rate";
	}
	if (uuid == GattServiceUuids::HumanInterfaceDevice())
	{
		return L"Human Interface Device";
	}
	if (uuid == GattServiceUuids::ImmediateAlert())
	{
		return L"Immediate Alert";
	}
	if (uuid == GattServiceUuids::LinkLoss())
	{
		return L"Link Loss";
	}
	if (uuid == GattServiceUuids::LocationAndNavigation())
	{
		return L"Location and Navigation";
	}
	if (uuid == GattServiceUuids::NextDstChange())
	{
		return L"Next Dst Change";
	}
	if (uuid == GattServiceUuids::PhoneAlertStatus())
	{
		return L"Phone Alert Status";
	}
	if (uuid == GattServiceUuids::ReferenceTimeUpdate())
	{
		return L"Reference Time Update";
	}
	if (uuid == GattServiceUuids::RunningSpeedAndCadence())
	{
		return L"Running Speed and Cadence";
	}
	if (uuid == GattServiceUuids::ScanParameters())
	{
		return L"Scan Parameters";
	}
	if (uuid == GattServiceUuids::TxPower())
	{
		return L"Tx Power";
	}
	return UUIDToString(uuid);
}


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


// 受診時の動作
vector<uint64_t> addr_list(0);

void onReceived(BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs) {
	//cout << "Received Advertisement!" << endl;

	auto addr = eventArgs.BluetoothAddress();
	if (addr_list.empty() || !(find(addr_list.begin(), addr_list.end(), addr) != addr_list.end())) {
		// デバイス名
		cout << "(" << addr_list.size() << ") ";
		wcout << getDeviceName(addr) << endl;


		// 基本情報
		cout << "  addr        :" << addr << endl;
		cout << "  connectable?:" << eventArgs.IsConnectable() << endl;
		cout << "  strength    :" << eventArgs.RawSignalStrengthInDBm() << "(dbm)" << endl;

		//// サービスを取得
		//auto device = BluetoothLEDevice::FromBluetoothAddressAsync(addr).get();
		//auto result = device.GetGattServicesAsync(BluetoothCacheMode::Uncached).get();
		
		//// 成功時
		//if (result.Status() == GattCommunicationStatus::Success) {
		//	auto services = result.Services();
		//	// サービス内のキャラクテリスティックを全て吐き出す
		//	for (auto service : services) {
		//		//wcout << L"UUID:" << service.Uuid() << endl;
		//		auto uuid = service.Uuid();
		//		wcout << L"  characteristic:" << ServiceToString(uuid) << endl;
		//	}
		//}

		cout << endl;


		addr_list.push_back(addr);
	}

	
}


int main() {
	cout << "pid:" << this_thread::get_id() << endl;
	cout << "initializing..." << endl;
	init_apartment();


	// スキャン準備
	BluetoothLEAdvertisementWatcher watcher;
	watcher.Received(&onReceived);	// 受信時の動作を設定


	// アドバタイズメントをスキャン
	watcher.Start();				// 受信を開始 Enterが押されるまで続ける
	while (getchar() != '\n');
	watcher.Stop();

	// 通信


	return 0;

}