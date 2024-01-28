// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

static const int nochange = 2;
int rate = 100;

void StartTask1(void *parameter ){
  while(1){
    digitalWrite( nochange, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(nochange,LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void StartTask2(void *parameter){
  while(1){
    digitalWrite( nochange, HIGH);
    vTaskDelay(rate / portTICK_PERIOD_MS);
    digitalWrite(nochange,LOW);
    vTaskDelay(rate / portTICK_PERIOD_MS);
  }
}

void setup(){
  Serial.begin(19200);
  vTaskDelay(1000/ portTICK_RATE_MS);
  Serial.println();
  Serial.println("==============================FreeRTOS Self Task2=======================================");

  xTaskCreatePinnedToCore(StartTask1,"task1", 1024, NULL, 1, &task_1, app_cpu);
  xTaskCreatePinnedToCore(StartTask2,"task2", 1024, NULL, 2, &task_2, app_cpu);
}

void loop(){
  if(Serial.available()){
    Serial.println("enter the time for the LED blink rate in ms: ");
    rate = Serial.parseInt();
  }
}
