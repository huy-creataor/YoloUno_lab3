#include "led_tasks.h"

#define NEO_PIN 45
#define LED_COUNT 1

void task_neopixel(void *pvParameters) {
    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.clear();
    strip.show();

    static bool yellow_toggle = false;

    while(1) {
        // [BÀI TẬP 3]: Kiểm tra trạng thái lỗi (cảm biến trả về 0 hoặc âm)
        if ((glob_temperature == 0.0f && glob_humidity == 0.0f) || 
            (glob_temperature < 0.0f && glob_humidity < 0.0f)) {
            
            if (yellow_toggle) {
                strip.setPixelColor(0, strip.Color(255, 255, 0)); 
            } else {
                strip.setPixelColor(0, strip.Color(0, 0, 0));    
            }
            yellow_toggle = !yellow_toggle; 
        } 
        else {
            // [BÀI TẬP 1]: Hoạt động bình thường theo nhiệt độ
            yellow_toggle = false;

            if (glob_temperature < 20.0) {
                strip.setPixelColor(0, strip.Color(0, 0, 255)); // Xanh dương
            } 
            else if (glob_temperature >= 20.0 && glob_temperature < 28.0) {
                strip.setPixelColor(0, strip.Color(0, 255, 0)); // Xanh lá
            } 
            else if (glob_temperature >= 28.0) {
                strip.setPixelColor(0, strip.Color(255, 0, 0)); // Đỏ
            }
        }
        
        strip.show();
        vTaskDelay(500); 
    }
}

void task_led(void *pvParameters) {
    pinMode(GPIO_NUM_48, OUTPUT);

    while(1) {
        // [BÀI TẬP 3]: Kiểm tra trạng thái lỗi (cảm biến trả về 0 hoặc âm)
        if ((glob_temperature == 0.0f && glob_humidity == 0.0f) || 
            (glob_temperature < 0.0f && glob_humidity < 0.0f)) {
            
            digitalWrite(GPIO_NUM_48, HIGH);
            vTaskDelay(200);
        } 
        else {
            // [BÀI TẬP 2]: Điều chỉnh tốc độ nhấp nháy LED theo độ ẩm (glob_humidity)
            int delay_time = 500;

            if (glob_humidity < 40.0) {
                delay_time = 1000; 
            } 
            else if (glob_humidity >= 40.0 && glob_humidity <= 60.0) {
                delay_time = 500;  
            } 
            else if (glob_humidity > 60.0) {
                delay_time = 250;  
            }

            digitalWrite(GPIO_NUM_48, HIGH);
            vTaskDelay(delay_time);
            digitalWrite(GPIO_NUM_48, LOW);
            vTaskDelay(delay_time);
        }
    }
}