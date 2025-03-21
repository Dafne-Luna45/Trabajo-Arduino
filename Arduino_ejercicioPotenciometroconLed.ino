#include <BluetoothSerial.h>

#define pin 2
#define pA 32

String nombreDisp = "ESP32-DyL", lecaux = " ";
int cont = 0;
const float vmax = 100, vmin= 0, posmax= 4095, posmin= 0;  

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
  int lecPot = analogRead(pA);
  Serial.println(lecPot);
  Serial.println(calcularValor(lecPot));
  delay(3000);
  if (lecPot >= 60) {
    digitalWrite(pin,1);
  }
  else {
    digitalWrite(pin,0);
  }
}
  /*String lectura = " ";
  if(SerialBT.available()){
    lectura = leerBT();
    lectura.trim();
    Serial.println(lectura);
  }
}

else {
  
}

if (lectura == "encendido"){
  digitalWrite(pin,1);
}
else if (lectura == "apagado"){
  digitalWrite(pin,0);
}
else {
  //Serial.println("Valor no valido");
}*/

String leerBT(){
  lecaux = " ";
  char a = SerialBT.read();
  lecaux = lecaux + a;
  while (SerialBT.available()){
   a = SerialBT.read(); 
   lecaux = lecaux + a;
  }

  return lecaux;
}

float calcularValor(int x){
  float m = (vmax - vmin)/(posmax - posmin);
  float b = vmax - m*posmax;
  float y = m*x+b;
  return y;
}
