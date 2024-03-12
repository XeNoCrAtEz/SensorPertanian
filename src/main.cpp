#include "main.h"


#include "LoRa.h"
#include "LoRa.h"

// define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

// define the frequencies for the two transmitters
#define frequency1 433E6
#define frequency2 433E6

int counter = 0;
bool useFrequency1 = true; // flag to switch between frequencies



void setup() {
    // begin USB Serial
    Serial.begin(115200);

log_i("Starting Sensor-%d", SENSOR_ID);
log_i("Logging will not start until RTC, Submitter, and TimeClass are initialized.");
    RTC rtc = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

#if defined(USE_WIFI)
    SubmitterWiFi submitter;
log_i("Using WiFi.");
#elif defined(USE_GSM)
    SubmitterGSM submitter(PIN_GSM_RX, PIN_GSM_TX);
log_i("Using GSM.");
#endif

// setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  // initialize the first frequency
  if (!LoRa.begin(frequency1)) {
    Serial.println("Error initializing LoRa");
    while (1);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");





    TimeClass timeClass = TimeClass(rtc, submitter);
    Logger logger = Logger(timeClass);

logger.log_I("Starting Sensor-" + String(SENSOR_ID));
if (!timeClass.availability() == TimeClass::RTC_UNAVAILABLE) logger.log_E("Error! RTC not available!");
if (!timeClass.availability() == TimeClass::NTP_UNAVAILABLE) logger.log_E("Error! Network time not available!");
if (!timeClass.availability() == TimeClass::NO_TIME) logger.log_E("Error! No time available!");

    BatteryMonitor battMon(PIN_VOLT_BAT, ESP32_REF_VOLTAGE, VOLT_MON_DIVIDER_RATIO, MIN_VOLT_LIPO, MAX_VOLT_LIPO);
    VoltageMonitor solarCellMon(PIN_VOLT_SC, ESP32_REF_VOLTAGE, VOLT_MON_DIVIDER_RATIO);
logger.log_I("Battery voltage: " + String(battMon.voltage()) + " mV (" + String(battMon.level()) + "%)");
logger.log_I("Solar cell voltage: " + String(solarCellMon.voltage()) + " mV");
    if (battMon.level() == 0) {
logger.log_E("Error! Battery depleted! Sleeping...");
        sleep(timeClass);
    }

#if defined(PROBE_DEFAULT)
    ProbeDefault probe(PIN_PROBE_RX, PIN_PROBE_TX);
logger.log_I("Initialized probe type: DEFAULT");
#elif defined(PROBE_KHDTK)
    ProbeKHDTK probe(PIN_PROBE_RX, PIN_PROBE_TX);
logger.log_I("Initialized probe type: KHDTK");
#elif defined(PROBE_NEW)
    ProbeNew probe(PIN_PROBE_RX, PIN_PROBE_TX);
logger.log_I("Initialized probe type: NEW");
#endif



// switch between frequencies in each iteration
  if (useFrequency1) {
    LoRa.setFrequency(frequency1);
  } else {
    LoRa.setFrequency(frequency2);
  }

  // try to parse packet
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    // received a packet
    if (useFrequency1) {
      Serial.print("Received packet from transmitter 1: '");
    } else {
      Serial.print("Received packet from transmitter 2: '");
    }
    
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
    }
    
    Serial.print("' with Kelompok TE 1 ");
    Serial.println(LoRa.packetRssi());
    
    // switch the flag for the next iteration
    useFrequency1 = !useFrequency1;
  }




    SoilData soilData;
    Probe::ErrorCodes probeErr = probe.sample(soilData);
if (probeErr == Probe::PROBE_ERROR) logger.log_E("Error! Internal probe error!");
else if (probeErr == Probe::NO_PROBE) logger.log_E("Error! No probe connected!");
else logger.log_I("Sampled soil data:\n" + soilData.toString());

    SoilReading currentReading = SoilReading(soilData, timeClass.get_date_time().TotalSeconds());

    SoilDataTable dataTable;
if (!dataTable.is_ready()) logger.log_E("Error! dataTable not ready!");
else logger.log_I("dataTable is initialized and ready!");

    if (dataTable.is_empty()) {
logger.log_I("Table is empty. Sending current sample");
        int responseCode = submitter.submit_reading(currentReading);
if (responseCode == HTTP_CODE_OK) logger.log_I("Data send successful!\n");
else logger.log_E("Error! Data send failed! Error code: " + String(responseCode));
    } else {
logger.log_I("Saved data available. Pushing sampled data...");
        auto pushErr = dataTable.push(currentReading);
if (pushErr != SoilDataTable::SUCCESS) logger.log_E("Error! Data push unsuccessful! Error code: " + String(pushErr));

        int responseCode = submitter.submit_reading(dataTable);
if (responseCode == HTTP_CODE_OK) logger.log_I("Data send successful!\n");
else logger.log_E("Error! Data send failed! Error code: " + String(responseCode));
}

    Display display(PIN_SCREEN_SDA, PIN_SCREEN_SCL);
if (!display.isOK()) logger.log_E("Error! Display is not ready!");
else logger.log_I("Display is ready!");

    display.display_splash_screen();
logger.log_I("Splash screen displayed.");

    display.display_data(soilData);
logger.log_I("Sampled soil data displayed.");

    display.clear_display();
logger.log_I("Display cleared.");

logger.log_I("Sleeping...");
    sleep(timeClass);
}


void loop()
{
}

