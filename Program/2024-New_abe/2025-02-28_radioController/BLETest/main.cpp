#include <simpleble/SimpleBLE.h>
#include <iostream>

int main() {
    auto adapter_list = SimpleBLE::Adapter::get_adapters();
    if (adapter_list.empty()) {
        std::cerr << "No BLE adapter found!" << std::endl;
        return 1;
    }

    auto adapter = adapter_list[0]; // 最初のアダプターを取得
    adapter.set_callback_on_scan_found([](SimpleBLE::Peripheral peripheral) {
        std::cout << "Found device: " << peripheral.identifier() << " [" << peripheral.address() << "]" << std::endl;
        });

    std::cout << "Scanning for BLE devices..." << std::endl;
    adapter.scan_for(5000);  // 5秒間スキャン
    return 0;
}
