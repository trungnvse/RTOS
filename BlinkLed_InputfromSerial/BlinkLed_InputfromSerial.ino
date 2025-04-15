/*******************************************************************************
* Define
*******************************************************************************/
#define LED_PIN 2
volatile int led_delay = 500;
/*******************************************************************************
* Code
*******************************************************************************/
void vReadSerialTask(void *pvParameters) {
  Serial.begin(115200);
  while (1) {
    if (Serial.available() > 0) {
      int new_delay = Serial.parseInt(); 
      if (new_delay > 0) {              
        led_delay = new_delay;          
      }
      while (Serial.available() > 0) Serial.read();
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void vBlinkLEDTask(void *pvParameters) {
  pinMode(LED_PIN, OUTPUT);
  while (1) {
    digitalWrite(LED_PIN, HIGH);           
    vTaskDelay(led_delay / portTICK_PERIOD_MS); 
    digitalWrite(LED_PIN, LOW);            
    vTaskDelay(led_delay / portTICK_PERIOD_MS); 
  }
}

void setup() {
  xTaskCreate(
    vReadSerialTask,    
    "ReadSerialTask",      
    2048,              
    NULL,              
    1,                 
    NULL              
  );

  xTaskCreate(
    vBlinkLEDTask,     
    "BlinkLEDTask",       
    2048,             
    NULL,              
    2,               
    NULL            
  );
}

void loop() {
  
}
/*******************************************************************************
* EOF
*******************************************************************************/