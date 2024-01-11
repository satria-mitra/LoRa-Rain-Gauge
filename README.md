# <div style="text-align: center;" markdown="1">LoRa Rain Gauge</div>


This repository contains an Arduino sketch for a rain gauge system that utilizes LoRa communication, RTC for timestamping, and an SD card for data logging.

*CASA0016:MDB*\
Satria Mitra Utama

![Rain Gauge](<Assets/Pictures/Assembled devices.jpg>)
# Dependancies
## Hardware Requirements
1. Arduino MKR board with LoRa connectivity
2. DS3231 RTC module
3. SD card module
4. Reed switch for rainfall measurement
5. 5mm Neodymum magnet
6. Printed Rain Gauge
7. Lot of wires

### Optional Hardwares
1. Lipo Battery
2. Solar Panel
3. Adafruit Solar Charger

## Software
### Arduino Libraries
1. [RTCLib](https://www.arduino.cc/reference/en/libraries/rtclib/)
2. [MKRWAN](https://www.arduino.cc/reference/en/libraries/mkrwan/)
3. Wire
4. SD
5. SPI

### Node Red
1. [Node-Red Dashboard lib](https://flows.nodered.org/node/node-red-dashboard)

# Getting Started
1. **Connections**: Wire up the components according to the provided pin configurations. 

| No | Hardware | Hardware Pin | Arduino MKR Pin |
|----|----------|--------------|-------------|
|1   | RTC      |    VCC       |VCC          |
|    |          | GND          | GND         |
|    |          | SCL          | SCL/12      |
|    |          | SDA          | SDA/11 |
|2   | Sd card  |    VCC       |VCC          |
|    |          | GND          | GND         |
|    |          | MOSI         | 8/MOSI      |
|    |          | MISO         | 10/MISO     |
|    |          | SCK          | 7           |
|3   | Reed switch| SW1        | 6           |
|    |       | VCC          | VCC         |
|    |          | GND          | GND         |
|4   | Charger | JST +        | JST +        |
|    |          | JST-         | JST -       |


2. **Libraries**: Install the necessary libraries (Wire, RTClib, MKRWAN, SPI, SD) in the Arduino IDE.
3. **TTN Application**:
   - Sign up for The Things Network
   - Select your region and [create your application on TTN console](https://www.thethingsindustries.com/docs/integrations/adding-applications/)
   - Plug your Arduino MKRWAN and run a script from Arduino IDE>Examples>MKRWAN>First Configuration, take a note on your DevUI
   - [Register your End Device on TTN Console ](https://workshops.cetools.org/codelabs/CASA0016-Workshop-4/index.html?index=..%2F..casa0016#6)
   - Create a file named __arduino_secrets.h__ on the same folder of Arduino LoRa Transmitter scripts and copy paste your appEui and appKey from TTN Console

     ```
     const char *appEui = "0000000000000000";
     const char *appKey = "<APP_KEY>";
     ``` 
4. Upload the scripts of Transmitter and see live data on TTN Console
5. If you can see your data, congratulations, if not please do see Troubleshooting sections
6. [Install Node-Red](https://nodered.org/docs/getting-started/) on your Raspberry PI or on your PC
7. [Install Node-Red Dashboard library](https://flows.nodered.org/node/node-red-dashboard)
8. Import Node-Red json files configuration from Scripts>Node-Red
9. See the payload on the debug message on Node-red. You can modify or create your own dashboard. Done

# Functionality
### RTC Setup
- Sets up and initializes the DS3231 RTC module for accurate timekeeping.
- Handles RTC power loss scenarios by setting the time using the sketch compilation time if power is lost.

### Rain Measurement
- Monitors rainfall using a reed switch and software debouncing to count pulses accurately.
- Resets accumulated rainfall at midnight to start afresh.

### LoRa Communication and Data Logging
- Sends rain gauge data via LoRaWAN to TTN at regular intervals.
- Logs data (date, time, accumulated rainfall) onto an SD card in a CSV format for local storage.

### Usage
- Monitor the Serial Monitor to observe real-time data and system status.
- Access the `rain.txt` file on the SD card for detailed logged rain gauge data.

## Troubleshooting
- If encountering issues during LoRa connection, ensure proper positioning for signal reception.
- Check your appKey and appEui on TTN Console and your aduino_secrets.h
- Verify wiring connections and component compatibility.
- Dont forget to attach your antenna

## Future Work
- Use low power library
- Design a watertight enclosure
- Reflective learning
- Prototype and iterate to make it better
- Outdoor device needs a waterproof and watertight material

## License
This project is licensed under the MIT License.

## Acknowledgements
- Arduino community
- Libraries used: RTClib, MKRWAN, SD
- Node Red 
