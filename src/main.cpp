#include "main.h"

int nitrogen=0, phosphorus=0, kalium=0, EC=0;
float humidity=0, temperature=0, pH=0;

void setup() {
    // begin USB Serial
    Serial.begin(115200);
    
    setup_display();

    delay(2000);
    
    display_splash_screen();
}

void loop() {
    sample(nitrogen, phosphorus, kalium, pH, temperature, humidity, EC);
    
#ifdef DEBUG
    info_soil_data(nitrogen, phosphorus, kalium, pH, temperature, humidity, EC);
#endif

    display_data(nitrogen, phosphorus, kalium, pH, temperature, humidity, EC);
}
