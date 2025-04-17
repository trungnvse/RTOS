/*******************************************************************************
* Global variable
*******************************************************************************/
SemaphoreHandle_t xMutex;
/*******************************************************************************
* Code
*******************************************************************************/
void vTask(void *pvParameters) {
  while (1) {
    xSemaphoreTake(xMutex, portMAX_DELAY); 
    Serial.println("vTask take xMutex first time");

    // deadlock because task lock itself
    xSemaphoreTake(xMutex, portMAX_DELAY); 
    Serial.println("vTask take xMutex second time");

    xSemaphoreGive(xMutex);
    Serial.println("vTask give xMutex");
  }  
}

void setup() {
  Serial.begin(115200);
  xMutex = xSemaphoreCreateMutex();
  xTaskCreate(vTask, "Task", 1024, NULL, 1, NULL);
}

void loop() {
  
}

/*******************************************************************************
* Eof
*******************************************************************************/