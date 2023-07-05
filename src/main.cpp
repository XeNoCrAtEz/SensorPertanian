#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>

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

// const byte code[]= {addressCode(0x01), functionCode(0x03), regStartAddr_L, regStartAddr_H, regLen_L, regLen_H, CRC_L, CRC_H};
const byte nitro[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C};
const byte phos[]  = {0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC};
const byte kali[]  = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};
const byte ph[]    = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
const byte temp[]  = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xCF};
const byte hum[]   = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F};
const byte ec[]    = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};

int read_data(const byte code[], int size);

void setup() {
    // begin USB Serial
    Serial.begin(115200);
    // begin I2C for OLED
    Wire.begin(SCREEN_SDA, SCREEN_SCL);
    // begin Serial for NPK Probe
    Serial1.begin(PROBE_SERIAL_BAUDRATE, SERIAL_8N1, PROBE_RX, PROBE_TX);
    // begin OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDR, false, false))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    delay(2000);
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

void loop()
{
    int nitrogen=0, phosphorus=0, kalium=0, EC=0;
    float humidity=0, temperature=0, pH=0;

    nitrogen = read_data(nitro, sizeof(nitro));
    phosphorus = read_data(phos, sizeof(phos));
    kalium = read_data(kali, sizeof(kali));
    pH = read_data(ph, sizeof(ph)) / (float) 100;
    temperature = read_data(temp, sizeof(temp)) / (float) 10;
    humidity = read_data(hum, sizeof(hum)) / (float) 10;
    EC = read_data(ec, sizeof(ec));

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
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
    Serial.print("pH: ");
    Serial.println(pH);
    Serial.print("EC: ");
    Serial.print(EC);
    Serial.println(" uS/cm");
    Serial.println();
    delay(2000);

    // output to OLED display
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
}


int read_data(const byte code[], int size) {
    byte probeOutputBuffer[7];

    if (Serial1.write(code, size) != size) return 0;

    // tampilkan request bytes
    Serial.print("Sent bytes: ");
    for (byte i = 0; i < 7; i++) {
        Serial.print(code[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    
    // tunggu respon data dri probe
    while(Serial1.available() < 7)
        ;
    
    // tampilkan respond bytes
    Serial.print("Received bytes: ");
    for (byte i = 0; i < 7; i++) {
        probeOutputBuffer[i] = Serial1.read();
        Serial.print(probeOutputBuffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    // gabungkan byte-3 dan byte-4
    return (probeOutputBuffer[3] << 8) + probeOutputBuffer[4];
}