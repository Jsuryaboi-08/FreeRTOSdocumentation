
#include <Arduino.h>
// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
//#include "freertos/mutex.h"
#include <task.h>
#include "freertos/semphr.h"

//#include <semphr.h>

SemaphoreHandle_t mutex; // Declare the mutex globally(important step to remeber)

int counter = 0; // giving a global variable

void Task1(void *pvParameters) {
  for (int i = 0; i < 100; i++) {
    // Acquire the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
      counter++; // Increment counter in critical section
      xSemaphoreGive(mutex); // Release the mutex
    }
    vTaskDelay(100/ portTICK_PERIOD_MS); // Delay non blocking using FreeRtos
  }
  vTaskDelete(NULL); // Delete the task(setup and loop tasks are deleted)
}

void Task2(void *pvParameters) {
  for (int i = 0; i < 100; i++) {
    // Acquire the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
      counter--; // Decrement counter in critical section
      xSemaphoreGive(mutex); // Release the mutex
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); // Delay for 10ms
  }
  vTaskDelete(NULL); // Delete the task
}

void setup() {
  Serial.begin(115200);

  // Create the mutex
  mutex = xSemaphoreCreateMutex();

  // Creating tasks
  xTaskCreate(Task1, "Task 1", 2048, NULL, 1, NULL);
  xTaskCreate(Task2, "Task 2", 2048, NULL, 1, NULL);
}

void loop() {
  // Do nothing (tasks handle everything)
}
