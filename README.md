LoRa Rain Gauge with RTC (Real-Time Clock) and SD Card Logger
=============================================================

This repository contains an Arduino sketch for a rain gauge system that utilizes LoRa communication, RTC for timestamping, and an SD card for data logging.

Overview
--------

The system includes the following components:

*   **LoRa Communication:** Utilizes the MKRWAN library for sending rain gauge data via LoRaWAN to The Things Network (TTN).
*   **Real-Time Clock (RTC):** Integrated DS3231 RTC to timestamp rain data accurately.
*   **SD Card Logging:** Records rain gauge data locally on an SD card for backup and storage purposes.
*   **Rain Measurement:** Utilizes a reed switch to count pulses, converting them into measured rainfall.

Hardware Requirements
---------------------

*   Arduino MKR board with LoRa connectivity
*   DS3231 RTC module
*   SD card module
*   Reed switch for rainfall measurement

Getting Started
---------------

1.  **Connections:** Wire up the components according to the provided pin configurations.
2.  **Libraries:** Install the necessary libraries (Wire, RTClib, MKRWAN, ArduinoLowPower, SPI, SD) in the Arduino IDE.
3.  **Secrets File:** Create an `arduino_secrets.h` file for storing TTN AppEUI and AppKey.
4.  **Upload Sketch:** Upload the provided Arduino sketch to your MKR board.

Functionality
-------------

### RTC Setup

*   Sets up and initializes the DS3231 RTC module for accurate timekeeping.
*   Handles RTC power loss scenarios by setting the time using the sketch compilation time if power is lost.

### Rain Measurement

*   Monitors rainfall using a reed switch and software debouncing to count pulses accurately.
*   Resets accumulated rainfall at midnight to start afresh.

### LoRa Communication and Data Logging

*   Sends rain gauge data via LoRaWAN to TTN at regular intervals.
*   Logs data (date, time, accumulated rainfall) onto an SD card in a CSV format for local storage.

Usage
-----

*   Monitor the Serial Monitor to observe real-time data and system status.
*   Access the `rain.txt` file on the SD card for detailed logged rain gauge data.

Troubleshooting
---------------

*   If encountering issues during LoRa connection, ensure proper positioning for signal reception.
*   Verify wiring connections and component compatibility.

Contributing
------------

Feel free to contribute by forking this repository, making enhancements, and creating a pull request.

License
-------

This project is licensed under [MIT License](LICENSE).

Acknowledgements
----------------

*   Arduino community
*   Libraries used: RTClib, MKRWAN, ArduinoLowPower, SD
