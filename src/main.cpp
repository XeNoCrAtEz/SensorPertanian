#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>

// DEBUG definition for debugging purposes
#define DEBUG

const int SCREEN_WIDTH = 128;   // OLED display width, in pixels
const int SCREEN_HEIGHT = 64;   // OLED display height, in pixels
const int SCREEN_SDA = 5;
const int SCREEN_SCL = 4;
const int SCREEN_ADDR = 0x3C;

const int PROBE_SERIAL_BAUDRATE = 9600;
const int PROBE_RX = 2;
const int PROBE_TX = 14;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

HardwareSerial probe(1);        // Hardware serial on Serial1

// const byte code[]= {addressCode(0x01), functionCode(0x03), regStartAddr_L, regStartAddr_H, regLen_L, regLen_H, CRC_L, CRC_H};
const byte nitro[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C};
const byte phos[]  = {0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC};
const byte kali[]  = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};
const byte ph[]    = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
const byte temp[]  = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xCF};
const byte hum[]   = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F};
const byte ec[]    = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};
const byte CODE_SIZE = 8;
const byte RESPONSE_SIZE = 7;

// function declaration
int get_data(const byte code[]);
void display_splash_screen();
void display_data(
    const int& nitrogen, const int& phosphorus, const int& kalium,
    const float& pH, const float& temperature, const float& humidity,
    const int& EC
);

#ifdef DEBUG
void info_soil_data(
    const int& nitrogen, const int& phosphorus, const int& kalium,
    const float& pH, const float& temperature, const float& humidity,
    const int& EC
);

#endif

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

void loop()
{
    int nitrogen=0, phosphorus=0, kalium=0, EC=0;
    float humidity=0, temperature=0, pH=0;

    nitrogen = get_data(nitro);
    phosphorus = get_data(phos);
    kalium = get_data(kali);
    pH = get_data(ph) / (float) 100;
    temperature = get_data(temp) / (float) 10;
    humidity = get_data(hum) / (float) 10;
    EC = get_data(ec);

#ifdef DEBUG
    info_soil_data(
        nitrogen, phosphorus, kalium, pH,
        temperature, humidity, EC
    );
#endif

    delay(2000);

    display_data(
        nitrogen, phosphorus, kalium, pH,
        temperature, humidity, EC
    );
}


int get_data(const byte code[]) {
    // begin Serial for NPK Probe
    probe.begin(PROBE_SERIAL_BAUDRATE, SERIAL_8N1, PROBE_RX, PROBE_TX);

    if (probe.write(code, CODE_SIZE) != CODE_SIZE) {
        Serial.println("Error! Data request sent is not in the same amount");
        return 0;
    }

#ifdef DEBUG
    // tampilkan request bytes
    Serial.print("Sent bytes: ");
    for (byte i = 0; i < CODE_SIZE; i++) {
        Serial.print(code[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
#endif
    
    // tunggu respon data dri probe
    while(probe.available() < RESPONSE_SIZE)
        ;

    byte probeOutputBuffer[RESPONSE_SIZE];
    
    // read data
    for (byte i = 0; i < RESPONSE_SIZE; i++)
        probeOutputBuffer[i] = probe.read();

#ifdef DEBUG
    // tampilkan respond bytes
    Serial.print("Received bytes: ");
    for (byte i = 0; i < RESPONSE_SIZE; i++) {
        Serial.print(probeOutputBuffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
#endif

    probe.end();

    // gabungkan byte-3 (value_L) dan byte-4 (value_H)
    return (probeOutputBuffer[3] << 8) + probeOutputBuffer[4];
}


void display_splash_screen() {
    // display splash screen
    display.clearDisplay();    
    display.setCursor(25, 15);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(" NPK Sensor");
    display.setCursor(25, 35);
    display.setTextSize(1);
    display.print("Initializing");
    display.display();
}


void display_data(
        const int& nitrogen, const int& phosphorus, const int& kalium,
        const float& pH, const float& temperature, const float& humidity,
        const int& EC
    ) {
    const int DISPLAY_DELAY = 5000;
    // display NPK
    display.clearDisplay();

    display.setTextSize(2);
    display.setCursor(0, 5);
    display.print("N: ");
    display.print(nitrogen);
    display.setTextSize(1);
    display.print(" mg/kg");

    display.setTextSize(2);
    display.setCursor(0, 25);
    display.print("P: ");
    display.print(phosphorus);
    display.setTextSize(1);
    display.print(" mg/kg");

    display.setTextSize(2);
    display.setCursor(0, 45);
    display.print("K: ");
    display.print(kalium);
    display.setTextSize(1);
    display.print(" mg/kg");

    display.display();

    delay(DISPLAY_DELAY);

    // display pH, Temp, Hum
    display.clearDisplay();

    display.setTextSize(2);
    display.setCursor(0, 5);
    display.print("pH:   ");
    display.print(pH);
    display.setTextSize(1);
    display.print("");

    display.setTextSize(2);
    display.setCursor(0, 25);
    display.print("Tem: ");
    display.print(temperature);
    display.setTextSize(1);
    display.print(" C");

    display.setTextSize(2);
    display.setCursor(0, 45);
    display.print("Hum: ");
    display.print(humidity);
    display.setTextSize(1);
    display.print(" %");

    display.display();

    delay(DISPLAY_DELAY);

    // display EC
    display.clearDisplay();

    display.setTextSize(2);
    display.setCursor(0, 5);
    display.print("EC: ");
    display.print(EC);
    display.setTextSize(1);
    display.print(" us/cm");

    display.display();

    delay(DISPLAY_DELAY);
}


#ifdef DEBUG
void info_soil_data(
        const int& nitrogen, const int& phosphorus, const int& kalium,
        const float& pH, const float& temperature, const float& humidity,
        const int& EC
    ) {
    // output to USB Serial
    Serial.print("Nitrogen: ");
    Serial.print(nitrogen);
    Serial.println(" mg/kg");

    Serial.print("Phosphorous: ");
    Serial.print(phosphorus);
    Serial.println(" mg/kg");

    Serial.print("Potassium: ");
    Serial.print(kalium);
    Serial.println(" mg/kg");

    Serial.print("pH: ");
    Serial.println(pH);

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
    
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("EC: ");
    Serial.print(EC);
    Serial.println(" uS/cm");

    Serial.println();
}
#endif