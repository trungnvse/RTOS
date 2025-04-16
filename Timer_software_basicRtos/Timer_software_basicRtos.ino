/*******************************************************************************
* Include
*******************************************************************************/
#include <DHT.h>
/*******************************************************************************
* Define
*******************************************************************************/
#define LED_PIN  2
#define DHTPIN  15
#define DHTTYPE DHT11
typedef struct {
  float temp;
  float hum;
} SensorData_t;
/*******************************************************************************
* Global variable
*******************************************************************************/
TimerHandle_t xOneShotTimer;
TimerHandle_t xledTimer;
TimerHandle_t xsensorTimer;
int ledState = LOW;
DHT dht(DHTPIN, DHTTYPE);
/*******************************************************************************
* Code
*******************************************************************************/
void onShotTimerCallback(TimerHandle_t xTimer) {
  uint32_t id = (uint32_t)pvTimerGetTimerID(xTimer);
  Serial.printf("OneShotTimer expire after %u seconds\n", id);
}

void ledblinkTimerCallback(TimerHandle_t xTimer) {
  uint32_t id = (uint32_t)pvTimerGetTimerID(xTimer);
  Serial.printf("xledTimer reload after %u seconds\n", id);
  Serial.println("led blink");
  if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = LOW;
  }
  digitalWrite(LED_PIN, ledState);
}

void readsensorTimerCallback(TimerHandle_t xTimer) {
  uint32_t id = (uint32_t)pvTimerGetTimerID(xTimer);
  Serial.printf("sensorTimer reload after %u seconds\n", id);
  SensorData_t sensorData;
  sensorData.temp = dht.readTemperature();
  sensorData.hum  = dht.readHumidity();
  Serial.printf("Read sensor: temp: %.1f, hum: %.1f\n", 
                    sensorData.temp, sensorData.hum);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();

  xOneShotTimer = xTimerCreate("OneShotTimer", pdMS_TO_TICKS(10000), pdFALSE, (void*)10, onShotTimerCallback);
  xledTimer     = xTimerCreate("ledTimer", pdMS_TO_TICKS(2000), pdTRUE, (void*)2, ledblinkTimerCallback);
  xsensorTimer  = xTimerCreate("sensorTimer", pdMS_TO_TICKS(4000), pdTRUE, (void*)4, readsensorTimerCallback);

  if (xOneShotTimer == NULL) {
    Serial.println("Can't create xOneShotTimer!");
  }
  if (xledTimer == NULL) {
    Serial.println("Can't create xledTimer!");
  }
  if (xsensorTimer == NULL) {
    Serial.println("Can't create xsensorTimer!");
  }

  if (xTimerStart(xOneShotTimer, 0) == pdFAIL) {
    Serial.println("Can't start xOneShotTimer!");
  }
  if (xTimerStart(xledTimer, 0) == pdFAIL) {
  Serial.println("Can't start xledTimer!");
  }
  if (xTimerStart(xsensorTimer, 0) == pdFAIL) {
  Serial.println("Can't start xsensorTimer!");
  }

  Serial.println("start timer!");
}

void loop() {

}
/*******************************************************************************
* Eof
*******************************************************************************/
