/*******************************************************************************
* Global variable
*******************************************************************************/
SemaphoreHandle_t xMutex;
/*******************************************************************************
* Code
*******************************************************************************/
void vlowTask(void *pvParameters) {
  if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE ) {
    Serial.println("lowTask take mutex");
    Serial.println("lowTask working ...");

    for (volatile int i = 0; i < 1000000; i++) {
      
    }
    Serial.println("lowTask finish");
    Serial.println("lowTask give mutex");
    xSemaphoreGive(xMutex);
  }
  vTaskDelete(NULL);
}

void vmediumTask(void *pvParameters) {
  vTaskDelay(50 / portTICK_PERIOD_MS);
  Serial.println("mediumTask begin !");
  Serial.println("mediumTask working ...");
  
  for (volatile int i = 0; i < 1000000; i++) {
    
  }

  Serial.println("mediumTask finish");
  vTaskDelete(NULL);
}

void vhighTask(void *pvParameters) {
  vTaskDelay(50 / portTICK_PERIOD_MS);
  if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE ) {
    Serial.println("highTask begin !");
    Serial.println("highTask take mutex");
    Serial.println("highTask working ...");

    for (volatile int i = 0; i < 1000000; i++) {
    
    }

    Serial.println("highTask finish");
    Serial.println("highTask give mutex");
    xSemaphoreGive(xMutex);
  }
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  xMutex = xSemaphoreCreateMutex();
  if (xMutex == NULL) {
    Serial.println("Failed to create mutex!");
    while (1);
  }
  Serial.println("begin 3 task ! ");
  xTaskCreatePinnedToCore(vlowTask, "lowTask", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(vmediumTask, "mediumTask", 2048, NULL, 2, NULL, 0);
  xTaskCreatePinnedToCore(vhighTask, "highTask", 2048, NULL, 3, NULL, 0);

}

void loop() {
  
}
/*******************************************************************************
* EOF
*******************************************************************************/