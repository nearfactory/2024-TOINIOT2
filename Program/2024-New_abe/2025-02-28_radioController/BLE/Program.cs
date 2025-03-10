using System;
using System.IO;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.Advertisement;
using Windows.Devices.Bluetooth.GenericAttributeProfile;

namespace gatt_client
{
    class Program
    {
        static void Main(string[] args)
        {
            Task.Run(AsyncMain).Wait();
        }

        static async Task AsyncMain()
        {
            var deviceName = "ここにデバイス名を入れること";
            try
            {
                //アドバタイズメントを検索する
                var watcher = new BluetoothLEAdvertisementWatcher();
                ulong btaddr = 0;

                watcher.Received += (BluetoothLEAdvertisementWatcher sender, BluetoothLEAdvertisementReceivedEventArgs args) =>
                {
                    //検出時コールバック
                    Console.WriteLine("+ " + args.Advertisement.LocalName);
                    if (args.Advertisement.LocalName == deviceName)
                    {
                        //デバイスを見つけた
                        Console.WriteLine("Found: " + args.BluetoothAddress);
                        btaddr = args.BluetoothAddress;
                    }
                };
                watcher.ScanningMode = BluetoothLEScanningMode.Active;
                watcher.Start();

                Console.WriteLine("Scan Start.");

                //5秒間スキャンする
                await Task.Delay(5000);
                watcher.Stop();

                Console.WriteLine("Scan Stop.");

                if (btaddr == 0)
                {
                    Console.WriteLine("Not found.");
                    return;
                }

                //デバイスに接続する
                Console.WriteLine("Connect...");
                var device = await BluetoothLEDevice.FromBluetoothAddressAsync(btaddr);


                //UUIDからサービスを取得する
                Console.Write("Service: ");
                var services = await device.GetGattServicesForUuidAsync(new Guid("00000000-0000-4000-A000-000000000000"));
                Console.WriteLine(services.Status);

                //UUIDからキャラクタリスティックを取得する
                Console.Write("Characteristic: ");
                var characteristics = await services.Services[0].GetCharacteristicsForUuidAsync(new Guid("00000000-0000-4000-A000-000000000001"));
                Console.WriteLine(characteristics.Status);

                var c = characteristics.Characteristics[0];

                //通知を受け取るコールバックを設定
                c.ValueChanged += (GattCharacteristic sender, GattValueChangedEventArgs args) =>
                {
                    Console.Write("Notify: ");
                    var streamNotify = args.CharacteristicValue.AsStream();
                    PrintFromStream(streamNotify);
                };
                //通知購読登録
                await c.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue.Notify);

                //単純な値読み込み
                Console.Write("Read: ");
                var d = await c.ReadValueAsync();
                var streamRead = d.Value.AsStream();
                PrintFromStream(streamRead);

                //単純な値書き込み
                Console.WriteLine("Write");
                await c.WriteValueAsync(new byte[1] { 0xAA }.AsBuffer());


                //60秒待つ(通知が飛んでくるはず)
                await Task.Delay(60 * 1000);

                //開放すると自動で切断される
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }

        static void PrintFromStream(Stream s)
        {
            //1byteずつ読み込んで表示
            int d = 0;
            while ((d = s.ReadByte()) != -1)
            {
                Console.Write(d.ToString("X"));
                Console.Write(",");
            }
            Console.WriteLine();
        }

    }
}

