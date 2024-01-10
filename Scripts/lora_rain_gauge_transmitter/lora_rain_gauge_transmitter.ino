#include <Wire.h>
#include <RTClib.h>
#include <MKRWAN.h>
#include <ArduinoLowPower.h>
#include <SPI.h>
#include <SD.h>

// Secret file for storing AppEUI and Appkey of TTN App
#include "arduino_secrets.h"

#define debugSerial Serial

RTC_DS3231 rtc;

// SD card settings
const int chipSelect = 7 ;

// Reed Switch settings
const int reedSwitchPin = 6;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 150; // Adjusted according to pulse on Logic Analyzer

// Var for rain
const float rainTipVolume = 0.2; // mm
volatile int pulseCount = 0;
double   accumulatedRainfall = 0.0;

LoRaModem modem;

// Timer interval in milliseconds (60 seconds)
unsigned long previousMillis = 0;
const unsigned long interval = 60000;

// Variables to store the formatted date and time
char  formattedDate[11];
char  formattedTime[9];

File dataFile;

void setRTC() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2023, 12, 10, 20, 12, 0));
  }
  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
   //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  //rtc.adjust(DateTime(2023, 12, 10, 21, 16, 0));
}

void displayDateTime() {
  DateTime now = rtc.now();
  
  // Format date and time
  sprintf(formattedDate, "%04d-%02d-%02d", now.year(), now.month(), now.day());
  sprintf(formattedTime, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
     
  // Print JSON payload
  Serial.print("date :");
  Serial.println(formattedDate);
  Serial.print("time :");
  Serial.println(formattedTime);
}


void countPulse() {
  // Implement software debouncing
  if (millis() - lastDebounceTime > debounceDelay) {
    pulseCount++; // Increment the pulse count
    float rainCounted = pulseCount * rainTipVolume;
    pulseCount = 0;
    accumulatedRainfall += rainCounted;
    debugSerial.println(accumulatedRainfall);

  }
  lastDebounceTime = millis();
}

void resetRain() {
  DateTime now = rtc.now();
  if (now.hour() == 0 && now.minute() == 00 && now.second() == 0) {
  accumulatedRainfall = 0.0;
  }
}

void publishMessages() {
  DateTime now = rtc.now();
  // Format date and time
  sprintf(formattedDate, "%04d-%02d-%02d", now.year(), now.month(), now.day());
  sprintf(formattedTime, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());

  uint16_t rainInt = accumulatedRainfall*10;

  // Payload message
  byte payload[4];
  payload[0] = highByte(rainInt);
  payload[1] = lowByte(rainInt);
  //payload[2] = highByte(formattedDate);
  //payload[3] = lowByte(formattedDate);
  

  debugSerial.print("Rain: ");
  debugSerial.print(accumulatedRainfall);
  debugSerial.print("mm at: ");
  debugSerial.print(formattedTime);
  debugSerial.print(" on ");
  debugSerial.println(formattedDate);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open("rain.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.print(formattedDate);
    dataFile.print(",");
    dataFile.print(formattedTime);
    dataFile.print(",");
    dataFile.println(accumulatedRainfall);
    Serial.println("Data saved");

    dataFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening rain_gauge.txt");
  }


  // modem.beginPacket();
  // modem.write(payload, sizeof(payload));
  // int err = modem.endPacket(true);
  // if (err > 0) {
  //   Serial.println("Message sent correctly!");
  // } else {
  //   Serial.println("Error sending message");
  // }
}

void sendPayload() {
  DateTime now = rtc.now();
  if (now.second() == 0) {
    if (millis() - previousMillis >= interval || previousMillis == 0) {
      publishMessages();
      previousMillis = millis(); // Update the previousMillis variable
    }
  } else {
    previousMillis = 0; // Reset previousMillis if it's not at 00 seconds
  }
}
void initModem() {
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
  }

  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
  }
}

void initSDCard() {
  Serial.print("Initializing SD card...");

  if (!SD.begin(7)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void setup() {
  debugSerial.begin(9600);

    // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  initSDCard();
  //initModem();
  setRTC();
  displayDateTime(); 
  pinMode(reedSwitchPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(reedSwitchPin), countPulse, FALLING);


}

void loop() {
  // Your code might perform other tasks here, but for rain measurement, we'll focus on the interrupt
  sendPayload();
  resetRain();
  // We're relying on the interrupt to count rain pulses, so this loop might remain empty
}

