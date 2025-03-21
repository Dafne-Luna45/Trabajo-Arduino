#define pin 2
String clave = "Beni", lec = " "; 

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
  // put your setup code here, to run once:
  crearTareas ();
  pinMode (pin,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
   vTaskDelay(pdMS_TO_TICKS(10));
}

void tareaUno(void * parameter){
  for (;;){
    Serial.println("Escribe la palabra clave: ");
    while(true){
      if(Serial.available()){
         lec = Serial.readString();
         break;
      }
    else {
     
   }
 }
  vTaskDelay(pdMS_TO_TICKS (500));

  }
}

void tareaDos(void * parameter){
  for (;;){
    if (lec == clave) {
      digitalWrite (pin,1);
      vTaskDelay(pdMS_TO_TICKS(1000));
      digitalWrite(pin,0);
      vTaskDelay(pdMS_TO_TICKS(1000));
      lec = " ";
    }
    else {
      digitalWrite (pin,0);
    }
  }
}

void crearTareas (){
  xTaskCreatePinnedToCore (
    tareaUno, //Funcion que implementa la tarea
    "Task1", //Nombre dela tarea
    1000, //Tamaño de bytes en memoria (pila de memoria);
    NULL, //Parametros de entrada de la funcion 
    0, //Prioridad de la tarea en el programa
    &Task1, //Puntero en memoria
    0); //Nucle en el que se ejecuta la tarea


    xTaskCreatePinnedToCore (
    tareaDos, //Funcion que implementa la tarea
    "Task2", //Nombre dela tarea
    1000, //Tamaño de bytes en memoria (pila de memoria);
    NULL, //Parametros de entrada de la funcion 
    0, //Prioridad de la tarea en el programa
    &Task2, //Puntero en memoria
    1); //Nucle en el que se ejecuta la tarea 
}
