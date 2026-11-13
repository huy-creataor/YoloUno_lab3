#include "led_tasks.h"

#define NEO_PIN 45
#define LED_COUNT 1

void task_neopixel(void *pvParameters) {
    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.clear();
    strip.show();

    while(1) {
        if (glob_temperature < 20.0) {
            // Nhỏ hơn 20 độ: Xanh dương
            strip.setPixelColor(0, strip.Color(0, 0, 255)); 
        } 
        else if (glob_temperature >= 20.0 && glob_temperature < 28.0) {
            // Từ 20 đến dưới 28 độ: Xanh lá
            strip.setPixelColor(0, strip.Color(0, 255, 0)); 
        } 
        else if (glob_temperature >= 28.0) {
            // Lớn hơn hoặc bằng 28 độ: Đỏ
            strip.setPixelColor(0, strip.Color(255, 0, 0)); 
        }
        
        strip.show(); // Cập nhật màu ra đèn





        // [THÀNH VIÊN 2 - BÀI TẬP 3]: Thêm logic nháy vàng nếu nhiệt độ và độ ẩm đều bằng 0 tại đây
        
        vTaskDelay(500); // Tốc độ cập nhật màu
    }
}

void task_led(void *pvParameters) {
pinMode(GPIO_NUM_48, OUTPUT);

    while(1) {
        // [THÀNH VIÊN 2 - BÀI TẬP 2]: Viết logic thay đổi tốc độ vTaskDelay theo glob_humidity tại đây
        // [THÀNH VIÊN 2 - BÀI TẬP 3]: Thêm logic sáng liên tục nếu nhiệt độ và độ ẩm đều bằng 0 tại đây

        digitalWrite(GPIO_NUM_48, HIGH);
        vTaskDelay(1000); // Thay thế 1000 bằng biến tốc độ của bạn
        digitalWrite(GPIO_NUM_48, LOW);
        vTaskDelay(1000); // Thay thế 1000 bằng biến tốc độ của bạn
    }
}