/*******************************************************************************
* Global variable
*******************************************************************************/

/*******************************************************************************
* Code
*******************************************************************************/
const int g_numberofsemaphores = 3;
const int g_numberoftask       = 5;
SemaphoreHandle_t xSemaphore;

void vTask(void *pvParameters) {
  int idTask = (int)pvParameters + 1;
  while (1) {
    Serial.printf("Task %d want to take semaphore\n", idTask);
    if (uxSemaphoreGetCount(xSemaphore) == 0)
    {
      Serial.printf("Task %d have to waiting\n", idTask);
    }
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      Serial.printf("Task %d take semaphore\n", idTask);
      Serial.printf("have %u available semaphores\n", uxSemaphoreGetCount(xSemaphore));

      /* Do something */
      vTaskDelay(pdMS_TO_TICKS(500 - idTask));

      xSemaphoreGive(xSemaphore);
      Serial.printf("Task %d give semaphore\n", idTask);
      Serial.printf("have %u available semaphores\n", uxSemaphoreGetCount(xSemaphore));
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void setup() {
  Serial.begin(115200);
  xSemaphore = xSemaphoreCreateCounting(g_numberofsemaphores, g_numberofsemaphores);

  if (xSemaphore == NULL) {
    Serial.println("can't create semaphore");
    while (1);
  }

  for (int i = 0; i <  g_numberoftask; i++) {
    xTaskCreate(
      vTask, 
      ("Task" + String(i)).c_str(), 
      2048,              
      (void *)i,       
      1,                
      NULL
    );
  }

}

void loop() {

}
/*******************************************************************************
* EOF
*******************************************************************************/