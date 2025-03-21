#define pin 2

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
  Serial.println("Este es el loop: ");
  Serial.println(xPortGetCoreID());
  delay(1000);
}

void tareaUno(void * parameter){
  for (;;){
    Serial.println("Esta es la tarea 1: ");
    Serial.println(xPortGetCoreID());
    delay(1500);
  }
}

void tareaDos(void * parameter){
  for (;;){
    digitalWrite (pin,1);
    delay(2000);
    digitalWrite(pin,0);
    delay(2000);
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
    1); //Nucle en el que se ejecuta la tarea


    xTaskCreatePinnedToCore (
    tareaDos, //Funcion que implementa la tarea
    "Task2", //Nombre dela tarea
    1000, //Tamaño de bytes en memoria (pila de memoria);
    NULL, //Parametros de entrada de la funcion 
    0, //Prioridad de la tarea en el programa
    &Task2, //Puntero en memoria
    1); //Nucle en el que se ejecuta la tarea 
}
