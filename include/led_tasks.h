#ifndef __LED_TASKS_H__
#define __LED_TASKS_H__

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "global.h" // Để lấy biến glob_temperature và glob_humidity

void task_neopixel(void *pvParameters);
void task_led(void *pvParameters);

#endif