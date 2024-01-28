// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// some string to print
const char msg[] = "Barkadeer hello world.";

// task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//********************************************************************************************************

//Tasks

// Task: print to Serial Terminal with lower priority
void startTask1(void *parameter) {
  // count number of characters in a string
  int msg_len = strlen(msg);

  // print to Terminal
  while (1) {
    Serial.println();
    for (int i = 0; i < msg_len; i++) {
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task: print to Serial Terminal with higher priority
void startTask2(void *parameter) {
  while (1) {
    Serial.print('^');
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

//*********************************************************************************************************

// main (runs as its own task with priority 1 on core 1)
void setup() {
  // configure Serial (going slow so we can watch the preemption)
  Serial.begin(300);

  // wait a moment to start so we don't miss any serial output
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---------------------------FreeRTOS Task DEMO---------------------------------");

  // print self priority
  Serial.print("Setup and loop task running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.println(uxTaskPriorityGet(NULL));

  // Task to run forever
  xTaskCreatePinnedToCore(startTask1, "Task1", 1024, NULL, 1, &task_1, app_cpu);

  // Task to run once with higher priority
  xTaskCreatePinnedToCore(startTask2, "Task2", 1024, NULL, 2, &task_2, app_cpu);
}

void loop() {
  // Suspend the higher priority task for some intervals
  for (int i = 0; i < 3; i++) {
    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  // delete the lower priority task
  if (task_1 != NULL) {
    vTaskDelete(task_1);
    task_1 = NULL;
  }
}
