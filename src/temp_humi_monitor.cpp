#include "temp_humi_monitor.h"
DHT20 dht20;
LiquidCrystal_I2C lcd(33,16,2);
void temp_humi_monitor(void *pvParameters){
    lcd.begin();         // Khởi tạo LCD
    lcd.backlight();    // Bật đèn nền LCD
    Wire.begin(11, 12);
    Serial.begin(115200);
    dht20.begin();

    while (1){
        /* code */
        
        dht20.read();
        // Reading temperature in Celsius
        float temperature = dht20.getTemperature();
        // Reading humidity
        float humidity = dht20.getHumidity();

        

        // Check if any reads failed and exit early
        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity =  -1;
            //return;
        }

        //Update global variables for temperature and humidity
        glob_temperature = temperature;
        glob_humidity = humidity;

        // Print the results
        lcd.clear(); // Xóa dữ liệu cũ trên màn hình
        
        // Dòng 1: Hiển thị Nhiệt độ
        lcd.setCursor(0, 0); // Đặt con trỏ ở cột 0, dòng 0
        lcd.print("Temperature: ");
        lcd.print(glob_temperature);
        lcd.print("°C");

        // Dòng 2: Hiển thị Độ ẩm
        lcd.setCursor(0, 1); // Đặt con trỏ ở cột 0, dòng 1
        lcd.print("Humidity: ");
        lcd.print(glob_humidity);
        lcd.print(" %");

        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
        
        vTaskDelay(5000);
    }
}

