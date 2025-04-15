/*******************************************************************************
* Include
*******************************************************************************/
#include <DHT.h>
/*******************************************************************************
* Define
*******************************************************************************/
#define LED_PIN 2
#define DHTPIN  15
#define DHTTYPE DHT11
typedef struct {
  float temp;
  float hum;
} SensorData_t;
/*******************************************************************************
* Global variable
*******************************************************************************/
SemaphoreHandle_t xBinarySemaphore ;
const int         interruptPin   = 4;
volatile bool     buttonPressed  = false;
DHT dht(DHTPIN, DHTTYPE);

/*******************************************************************************
* Code
*******************************************************************************/

void IRAM_ATTR buttonInterruptHandler() {
  BaseType_t xWakeupHigherTask = pdFALSE;
  static unsigned long lastPressed = 0;
  unsigned long now = millis();
  if ((now - lastPressed) > 100)
  {
    if (digitalRead(interruptPin) == LOW)
    {
      buttonPressed = true;
      xSemaphoreGiveFromISR(xBinarySemaphore, &xWakeupHigherTask);
      Serial.println("ISR give semaphore");
      if (xWakeupHigherTask == pdTRUE) {
        portYIELD_FROM_ISR();
      }
    }
    lastPressed = now;
  }
}

void vTask(void *pvParameters) {
  while (1) {
    if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
      Serial.println("Task take semaphore");
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      //do sơmething like read tem and hum from sensor
      SensorData_t sensorData;
      sensorData.temp = dht.readTemperature();
      sensorData.hum  = dht.readHumidity();
      Serial.printf("Read sensor: temp: %.1f, hum: %.1f\n", 
                     sensorData.temp, sensorData.hum);

      buttonPressed = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  xBinarySemaphore = xSemaphoreCreateBinary();
  if (xBinarySemaphore == NULL) {
    Serial.println("Failed to create binarySemaphore!");
    while (1);
  }
  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonInterruptHandler, FALLING);
  xTaskCreatePinnedToCore(vTask, "Task", 2048, NULL, 1, NULL, 0);
  Serial.println("begin binarysemaphore ! ");
}

void loop() {
  
}
/*******************************************************************************
* EOF
*******************************************************************************/