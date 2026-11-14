#include "temp_humi_monitor.h"

// Chọn chế độ lọc:
// 1 = Bài tập 4: Bộ lọc trung bình cộng (Average Filter) kèm kiểm tra độ biến thiên
// 2 = Bài tập 5: Bộ lọc trung vị (Median Filter)
#define FILTER_MODE 1

#define NUM_SAMPLES 5
#define SAMPLE_INTERVAL_MS 100
#define TEMP_VARIATION_THRESHOLD 2.0f  // Ngưỡng biến thiên nhiệt độ (°C)
#define HUMI_VARIATION_THRESHOLD 5.0f  // Ngưỡng biến thiên độ ẩm (%)

DHT20 dht20;
LiquidCrystal_I2C lcd(33, 16, 2);

// Hàm sắp xếp tăng dần (dùng cho Median Filter - Bài tập 5)
static void sortArray(float arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                float tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void temp_humi_monitor(void *pvParameters) {
    lcd.begin();         // Khởi tạo LCD
    lcd.backlight();     // Bật đèn nền LCD
    Wire.begin(11, 12);
    Serial.begin(115200);
    dht20.begin();

    while (1) {
        float tempSamples[NUM_SAMPLES];
        float humiSamples[NUM_SAMPLES];
        bool readError = false;

        // Lấy 5 mẫu dữ liệu, mỗi mẫu cách nhau 100ms
        for (int i = 0; i < NUM_SAMPLES; i++) {
            dht20.read();
            float t = dht20.getTemperature();
            float h = dht20.getHumidity();

            if (isnan(t) || isnan(h)) {
                readError = true;
            }
            tempSamples[i] = t;
            humiSamples[i] = h;

            if (i < NUM_SAMPLES - 1) {
                vTaskDelay(pdMS_TO_TICKS(SAMPLE_INTERVAL_MS));
            }
        }

        if (readError) {
            Serial.println("Failed to read from DHT sensor!");
            glob_temperature = -1;
            glob_humidity = -1;
        } else {
#if FILTER_MODE == 1
            // BÀI TẬP 4: Bộ lọc trung bình cộng & kiểm tra độ biến thiên
            float tSum = 0, hSum = 0;
            float tMin = tempSamples[0], tMax = tempSamples[0];
            float hMin = humiSamples[0], hMax = humiSamples[0];

            for (int i = 0; i < NUM_SAMPLES; i++) {
                tSum += tempSamples[i];
                hSum += humiSamples[i];
                if (tempSamples[i] < tMin) tMin = tempSamples[i];
                if (tempSamples[i] > tMax) tMax = tempSamples[i];
                if (humiSamples[i] < hMin) hMin = humiSamples[i];
                if (humiSamples[i] > hMax) hMax = humiSamples[i];
            }

            float tVariation = tMax - tMin;
            float hVariation = hMax - hMin;

            if (tVariation < TEMP_VARIATION_THRESHOLD && hVariation < HUMI_VARIATION_THRESHOLD) {
                glob_temperature = tSum / NUM_SAMPLES;
                glob_humidity = hSum / NUM_SAMPLES;
            } else {
                Serial.println("Sensor variation exceeded threshold, keeping previous values.");
            }
#elif FILTER_MODE == 2
            // BÀI TẬP 5: Bộ lọc trung vị (Median Filter)
            sortArray(tempSamples, NUM_SAMPLES);
            sortArray(humiSamples, NUM_SAMPLES);
            glob_temperature = tempSamples[NUM_SAMPLES / 2];
            glob_humidity = humiSamples[NUM_SAMPLES / 2];
#endif
        }

        // Hiển thị kết quả lên LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temperature: ");
        lcd.print(glob_temperature);
        lcd.print("C");

        lcd.setCursor(0, 1);
        lcd.print("Humidity: ");
        lcd.print(glob_humidity);
        lcd.print(" %");

        // In kết quả ra Serial Monitor
        Serial.print("Humidity: ");
        Serial.print(glob_humidity);
        Serial.print("%  Temperature: ");
        Serial.print(glob_temperature);
        Serial.println("°C");

        // Tổng chu kỳ ~5s (đã mất 400ms cho 4 khoảng nghỉ giữa 5 mẫu)
        vTaskDelay(pdMS_TO_TICKS(4600));
    }
}

