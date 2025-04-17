/*******************************************************************************
* Global variable
*******************************************************************************/
SemaphoreHandle_t xMutex_1;
SemaphoreHandle_t xMutex_2;
/*******************************************************************************
* Code
*******************************************************************************/
void vTask_1(void *pvParameters) {
  while (1) {
    xSemaphoreTake(xMutex_1, portMAX_DELAY);
    Serial.println("Task_1 take xMutex_1");

    // deadlock because wait each other
    xSemaphoreTake(xMutex_2, portMAX_DELAY);
    Serial.println("Task_1 take xMutex_2");

    xSemaphoreGive(xMutex_1);
    Serial.println("Task_1 give xMutex_1");

    xSemaphoreGive(xMutex_2);
    Serial.println("Task_1 give xMutex_2");
  }
}

void vTask_2(void *pvParameters) {
  while (1) {
    xSemaphoreTake(xMutex_2, portMAX_DELAY);
    Serial.println("vTask_2 take xMutex_2");

    // deadlock because wait each other
    xSemaphoreTake(xMutex_1, portMAX_DELAY);
    Serial.println("vTask_2 take xMutex_1");

    xSemaphoreGive(xMutex_1);
    Serial.println("vTask_2 give xMutex_2");

    xSemaphoreGive(xMutex_2);
    Serial.println("vTask_2 give xMutex_1");
  }
}

void setup() {
  Serial.begin(115200);
  xMutex_1 = xSemaphoreCreateMutex();
  xMutex_2 = xSemaphoreCreateMutex();
  xTaskCreate(vTask_1, "Task_1", 1024, NULL, 1, NULL);
  xTaskCreate(vTask_2, "Task_2", 1024, NULL, 1, NULL);
}

void loop() {

}
/*******************************************************************************
* Eof
*******************************************************************************/