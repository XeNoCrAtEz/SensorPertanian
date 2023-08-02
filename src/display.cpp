#include "display.h"


Display::Display()
        : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1)
{
    // begin I2C for OLED
    Wire.begin(SCREEN_SDA, SCREEN_SCL);
    // begin OLED display
    // Address 0x3C for 128x64
    if (!begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDR, false, false)) {
        Serial.println(F("SSD1306 allocation failed"));
    }
}


void Display::display_wifi() {
}


void Display::display_splash_screen() {
    // display splash screen
    clearDisplay();
    setCursor(25, 15);
    setTextSize(1);
    setTextColor(WHITE);
    println(" NPK Sensor");
    setCursor(25, 35);
    setTextSize(1);
    print("Initializing");
    display();
}


void Display::display_data(const SoilData& soilData) {
    const int DISPLAY_DELAY = 2000;

    const int& nitrogen = soilData.nitrogen;
    const int& phosphorus = soilData.phosphorus;
    const int& kalium = soilData.kalium;
    const float& pH = soilData.pH;
    const float& temperature = soilData.temperature;
    const float& humidity = soilData.humidity;
    const int& EC = soilData.EC;

    // display NPK
    clearDisplay();

    setTextSize(2);
    setCursor(0, 5);
    print("N: ");
    print(nitrogen);
    setTextSize(1);
    print(" mg/kg");

    setTextSize(2);
    setCursor(0, 25);
    print("P: ");
    print(phosphorus);
    setTextSize(1);
    print(" mg/kg");

    setTextSize(2);
    setCursor(0, 45);
    print("K: ");
    print(kalium);
    setTextSize(1);
    print(" mg/kg");

    display();

    delay(DISPLAY_DELAY);

    // display pH, Temp, Hum
    clearDisplay();

    setTextSize(2);
    setCursor(0, 5);
    print("pH:   ");
    print(pH);
    setTextSize(1);
    print("");

    setTextSize(2);
    setCursor(0, 25);
    print("Tem: ");
    print(temperature);
    setTextSize(1);
    print(" C");

    setTextSize(2);
    setCursor(0, 45);
    print("Hum: ");
    print(humidity);
    setTextSize(1);
    print(" %");

    display();

    delay(DISPLAY_DELAY);

    // display EC
    clearDisplay();

    setTextSize(2);
    setCursor(0, 5);
    print("EC: ");
    print(EC);
    setTextSize(1);
    print(" us/cm");

    display();

    delay(DISPLAY_DELAY);
}