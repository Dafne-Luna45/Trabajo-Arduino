#include <HTTPClient.h>
#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 23
#define DHTTYPE DHT22

float h=0.0, t=0.0, f=0.0; 

//cREDENCIALES DE ACCESO A WIFI
const char* ssid = "117496";
const char* cont = "11749612";

//Direccion HTTP de thingspeak 
const char* nombreServidor = "http://api.thingspeak.com/update?api_key=78BWQ255OOROTUMI";

DHT dht (DHTPIN, DHTTYPE); 

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600); 
  crearTareas ();
  dht.begin (); 
  conexionWifi (); 
 
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print ("La humedad relativa es: "); 
  Serial.println (h);
  Serial.print ("La temperatura relativa en C es: "); 
  Serial.println (t);
  Serial.print ("La temperatura relativa en F es: "); 
  Serial.println (f);
  vTaskDelay (pdMS_TO_TICKS (15000) );
}

void leerSensor (void * parameter){
 for (;;) {
  vTaskDelay (4000); 
  h = dht.readHumidity (); 
  t = dht.readTemperature ();
  f = dht.readTemperature (true); 
 }
}

//Envio de datos por posteo HTTP 
void posteoDatos (void * parameter) {
  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      respuestaHTTP (); 
      vTaskDelay (pdMS_TO_TICKS (15000) ); 
    }
  
  else {
    conexionWifi ();
  } 
  }
}

void respuestaHTTP () {
  WiFiClient client; 
  HTTPClient http; 

  //Se establce el inicio del cliente http para el posteo de datos 
  http.begin(client, nombreServidor); 
  http.addHeader("Content=Type", "application/x-www-form-uriencoded");


  //Informacion a enviar al servidor 
  String httpResp = "&field1=" + String(h) + "&field2=" + String(t); 

  int httpCodigo = http.POST (httpResp); 

  //Se valida la respuesta 
  if (httpCodigo == 200) {
    String cuerpo_respuesta = http.getString (); 
    Serial.print ("El servidor respondio: "); 
    Serial.println (cuerpo_respuesta); 
  }
  else {
    Serial.print ("Codigo de respuesta HTTP: "); 
    Serial.println (httpCodigo); 
  }
  http.end ();  //Se liberan recursos del cliente HTTP 
}


void crearTareas () {
  // put your setup code here, to run once:
  xTaskCreatePinnedToCore (
    leerSensor,  //Funcion que implementa la tarea
    "Task1", // Nombre de la tarea 
    1000, //Tamaño de bytes en memoria (pila de memoria) 
    NULL, //Parametros de entrada de la funcion 
    0, // Prioridad de la tarea en el programa 
    &Task1, //Puntero en memoria 
    0); // Nucleo en el que se ejecuta la tarea  

    xTaskCreatePinnedToCore (
    posteoDatos,  //Funcion que implementa la tarea
    "Task2", // Nombre de la tarea 
    3000, //Tamaño de bytes en memoria (pila de memoria) 
    NULL, //Parametros de entrada de la funcion 
    1, // Prioridad de la tarea en el programa 
    &Task2, //Puntero en memoria 
    0); // Nucleo en el que se ejecuta la tarea
}

void conexionWifi () {
  Serial.println ( "Wifi Desconectado"); 
  while (WiFi.status() != WL_CONNECTED){
    WiFi.begin (ssid,cont); 
    Serial.print (".");
    delay (500);
    
  }
  Serial.println (":");
}
