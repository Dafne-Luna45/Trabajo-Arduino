#include <HTTPClient.h>
#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2
#define DHTTYPE DHT22

float h=0.0, t=0.0, f=0.0;

//Credenciales de acceso a WiFi
const char* ssid= "Red-gatoooo";
const char* cont= "12345678";


DHT dht(DHTPIN, DHTTYPE);
TaskHandle_t Task1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  crearTareas();
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("La humedad relativa es: ");
  Serial.println(h);
  Serial.print("La temperatura relativa en ºC es: ");
  Serial.println(t);
  Serial.print("La temperatura relativa en ºF es: ");
  Serial.println(f);
  vTaskDelay(pdMS_TO_TICKS (2000));
}

void leerSensor (void * parameter){
  for (;;){
  delay(2000);
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);
  }
}

 void crearTareas(){
 xTaskCreatePinnedToCore (
    leerSensor, //Funcion que implementa la tarea
    "Task1", //Nombre dela tarea
    1000, //Tamaño de bytes en memoria (pila de memoria);
    NULL, //Parametros de entrada de la funcion 
    0, //Prioridad de la tarea en el programa
    &Task1, //Puntero en memoria
    0); //Nucle en el que se ejecuta la tarea 
}

void conxionWiFi(){
  Serial.println("WiFi Desconectadooo...");
  while (WiFi.status() != WL_CONNECTED){
    WiFi.begin (ssid,cont);
    Serial.print(".");
    delay(500);
  }
}
