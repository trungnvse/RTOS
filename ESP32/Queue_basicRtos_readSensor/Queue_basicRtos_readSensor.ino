/*******************************************************************************
* Include
*******************************************************************************/
#include <DHT.h>
/*******************************************************************************
* Define
*******************************************************************************/
#define DHTPIN  15
#define DHTTYPE DHT11

typedef struct {
  float temp;
  float hum;
} SensorData_t;
/*******************************************************************************
* Global variable
*******************************************************************************/
QueueHandle_t xQueue = NULL;
DHT dht(DHTPIN, DHTTYPE);
/*******************************************************************************
* Code
*******************************************************************************/
void vReadSensor_SendDataTask(void *pvParameters) {
  SensorData_t sensorData;
  
  while(1) {
    sensorData.temp = dht.readTemperature();
    sensorData.hum  = dht.readHumidity();
    
    if(xQueueSend(xQueue, &sensorData, pdMS_TO_TICKS(100))){
      Serial.printf("Send data: temp: %.1f, hum: %.1f\n", 
                     sensorData.temp, sensorData.hum);
    } else {
      Serial.println("Queue full");
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void vReceiveDataTask(void *pvParameters) {
  SensorData_t receivedData;
  
  while(1) {
    if(xQueueReceive(xQueue, &receivedData, portMAX_DELAY)) {
      Serial.printf("Received: temp: %.1f, hum: %.1f\n",
                     receivedData.temp, receivedData.hum);
    }
  }
}

void setup() {
  Serial.begin(115200);
  xQueue = xQueueCreate(5, sizeof(SensorData_t));
  dht.begin();

  xTaskCreatePinnedToCore(
  vReadSensor_SendDataTask,    
  "ReadSensor_SendDataTask",   
  2048,               
  NULL,               
  1,                  
  NULL,              
  0                
  );

  xTaskCreatePinnedToCore(
  vReceiveDataTask,
  "ReceiveDataTask",
  2048,
  NULL,
  1,
  NULL,
  0
  );

}

void loop() {

}
/*******************************************************************************
* EOF
*******************************************************************************/