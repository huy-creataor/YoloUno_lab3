#include "temp_humi_monitor.h"
#define NEO_PIN 45
#define LED_COUNT 1 

void setup() {
  // put your setup code here, to run once for task creation
  Serial.begin(115200);
  xTaskCreate(temp_humi_monitor, "TEMP and HUMI", 2048, NULL, 2, NULL);
  xTaskCreate(led_blinky, "LED Control", 2048, NULL, 2, NULL);
  xTaskCreate(neo_blinky, "NEO Control", 2048, NULL, 2, NULL);
}

void loop() {
  // Serial.println("Hello Custom Board");
  // delay(1000);
}