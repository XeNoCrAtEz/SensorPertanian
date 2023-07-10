#include "main.h"

void setup() {
    // begin USB Serial
    Serial.begin(115200);
    // begin I2C for OLED
    Wire.begin(SCREEN_SDA, SCREEN_SCL);
    // begin OLED display
    // Address 0x3C for 128x64
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDR, false, false)) {
        Serial.println(F("SSD1306 allocation failed"));
        Serial.println("Restarting...");
        ESP.restart();
    }
    delay(2000);
    
    display_splash_screen();
}

void loop() {
    int nitrogen=0, phosphorus=0, kalium=0, EC=0;
    float humidity=0, temperature=0, pH=0;

    sample(nitrogen, phosphorus, kalium, pH, temperature, humidity, EC);
    
#ifdef DEBUG
    info_soil_data(nitrogen, phosphorus, kalium, pH, temperature, humidity, EC);
#endif

    display_data(nitrogen, phosphorus, kalium, pH, temperature, humidity, EC);
}
