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
//Direccion HTTP de Thingspeak
const char* nombreServidor = "https://api.thingspeak.com/update?api_key=TYMPFJPEEAPY442W";


DHT dht(DHTPIN, DHTTYPE);
TaskHandle_t Task1;
TaskHandle_t Task2;

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
  vTaskDelay(pdMS_TO_TICKS (15000));
}

void leerSensor (void * parameter){
  for (;;){
  vTaskDelay(pdMS_TO_TICKS (2000));
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);
  }
}

//Envio de datos por posteo HTTP
void posteoDatos(){
  for(;;){
    if (WiFi.status() == WL_CONNECTED){
      respuestaHTTP();
      vTaskDelay(pdMS_TO_TICKS (15000));
    }
    else {
      conexionWifi();
    }
  }
}

void respuestaHTTP(){
 WiFiClient client;
 HTTPClient http;

  //Se establece el inicio del cliente HTTP para posteo de datos
  http.begin();
  http.addHeader("Content-Type","application/x-www-form-urlencoded");

  //Informacion a enviar al servidor
  String httpResp ="&field1=" + floatToString(h) + "&field2=" + floatToString(t);
  int httpCodigo = http.POST(httpResp);

   //Se valida la respuesta
   if(httpCodigo ==200){
    String cuerpo_respuesta = http.getString();
    Serial.print("El servidor respondio: ");
    Serial.println(cuerpo_respuesta);
   }
   else {
    Serial.print("Codigo de respuesta HTTP: ");
    Serial.println(httpCodigo);
   }
   http.end(); //Se liberan recursos del cliente HTTP
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

    xTaskCreatePinnedToCore (
    posteoDatos, //Funcion que implementa la tarea
    "Task2", //Nombre dela tarea
    3000, //Tamaño de bytes en memoria (pila de memoria);
    NULL, //Parametros de entrada de la funcion 
    1, //Prioridad de la tarea en el programa
    &Task2, //Puntero en memoria
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
