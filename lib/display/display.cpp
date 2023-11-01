#include "display.h"


Display::Display(uint8_t sda, uint8_t scl, uint8_t w, uint8_t h, uint8_t addr, TwoWire *twi, int8_t rstpin)
        : m_disp(w, h, twi, rstpin),
          SCREEN_WIDTH{w}, SCREEN_HEIGHT{h}, SCREEN_ADDR{addr} {
    Wire.begin(sda, scl);

    if (!m_disp.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDR, false, false)) {
        Serial.println("SSD1306 Failed to Initialize!");
        m_status = DISPLAY_FAILED;
    }

    m_status = DISPLAY_OK;
}


Display::ErrorCodes Display::isOK() {
    return m_status;
}


Display::ErrorCodes Display::display_splash_screen() {
    // display splash screen
    m_disp.clearDisplay();    
    m_disp.setTextColor(WHITE);
    m_disp.setCursor(25, 15);
    m_disp.setTextSize(1);
    m_disp.println(" NPK Sensor");
    m_disp.setCursor(25, 35);
    m_disp.setTextSize(1);
    m_disp.print("Initializing");
    m_disp.display();

    return DISPLAY_OK;
}


Display::ErrorCodes Display::display_data(const SoilData& soilData) {
    const uint16_t DISPLAY_DELAY = 2000;

    const uint16_t& nitrogen = soilData.nitrogen;
    const uint16_t& phosphorus = soilData.phosphorus;
    const uint16_t& kalium = soilData.kalium;
    const float& pH = soilData.pH;
    const float& temperature = soilData.temperature;
    const float& humidity = soilData.humidity;
    const uint16_t& EC = soilData.EC;

    // display NPK
    m_disp.clearDisplay();

    m_disp.setTextColor(WHITE);

    m_disp.setTextSize(2);
    m_disp.setCursor(0, 5);
    m_disp.print("N: ");
    m_disp.print(nitrogen);
    m_disp.setTextSize(1);
    m_disp.print(" mg/kg");

    m_disp.setTextSize(2);
    m_disp.setCursor(0, 25);
    m_disp.print("P: ");
    m_disp.print(phosphorus);
    m_disp.setTextSize(1);
    m_disp.print(" mg/kg");

    m_disp.setTextSize(2);
    m_disp.setCursor(0, 45);
    m_disp.print("K: ");
    m_disp.print(kalium);
    m_disp.setTextSize(1);
    m_disp.print(" mg/kg");

    m_disp.display();

    delay(DISPLAY_DELAY);

    // display pH, Temp, Hum
    m_disp.clearDisplay();

    m_disp.setTextSize(2);
    m_disp.setCursor(0, 5);
    m_disp.print("pH:   ");
    m_disp.print(pH);
    m_disp.setTextSize(1);
    m_disp.print("");

    m_disp.setTextSize(2);
    m_disp.setCursor(0, 25);
    m_disp.print("Tem: ");
    m_disp.print(temperature);
    m_disp.setTextSize(1);
    m_disp.print(" C");

    m_disp.setTextSize(2);
    m_disp.setCursor(0, 45);
    m_disp.print("Hum: ");
    m_disp.print(humidity);
    m_disp.setTextSize(1);
    m_disp.print(" %");

    m_disp.display();

    delay(DISPLAY_DELAY);

    // display EC
    m_disp.clearDisplay();

    m_disp.setTextSize(2);
    m_disp.setCursor(0, 5);
    m_disp.print("EC: ");
    m_disp.print(EC);
    m_disp.setTextSize(1);
    m_disp.print(" us/cm");

    m_disp.display();

    delay(DISPLAY_DELAY);

    clear_display();

    return DISPLAY_OK;
}


Display::ErrorCodes Display::clear_display() {
    m_disp.clearDisplay();
    m_disp.display();

    return DISPLAY_OK;
}