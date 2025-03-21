#include <BluetoothSerial.h>

#define pin 2

String nombreDisp = "ESP32-DyL";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SerialBT.begin(nombreDisp);
  pinMode(pin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (SerialBT.available()){
   Serial.write(SerialBT.read()); 
  }

  if(Serial.available()){
    Serial.write(SerialBT.read());
  }
}
