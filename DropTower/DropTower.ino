/*  *********************************************
 *  Softwaremill Drop Tower Module Software
 *
 *  Developers:
 *  krzysztof.grajek@softwaremill.com
 *  pawel.stawicki@softwaremill.com
 *
 *  Development Environment Specifics:
 *  Arduino 1.6.11
 *
 *  Hardware Specifications:
 *  SparkFun ADXL345
 *  Arduino Uno
 *  Relay shield
 *  Ethernet shield
 *  More info about the project on the github repo:
 *  https://github.com/softwaremill/pwsat-droptower
 *  *********************************************/


#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library
#include <SPI.h>
#include "SdFat.h"

/*
 * !!! IMPORTANT !!!
 * Don't use relay 1 and 4 while the SD card reader with chip select 4 is in use.
 */
#define RELAY_1  4 //cannot be used together with SD Card reader
#define RELAY_2  7
#define RELAY_3  8
#define RELAY_4  12  //cannot be used together with SD Card reader
#define FILE_BASE_NAME "Data"

ADXL345 adxl = ADXL345();
const uint8_t chipSelect = 4;            //needed to set up SPI communication on the ethernet shield
const int FREE_FALL_THRESHOLD = 7;      // (5 - 9) recommended - 62.5mg per increment
const int FREE_FALL_DURATION = 30;      // (20 - 70) recommended - 5ms per increment
const int MAX_TIME = 1000;
const uint32_t SAMPLE_INTERVAL_MS = 100;
bool freeFallDetected = false;          // Free Fall detection flag

// File system object.
SdFat sd;
// Log file.
SdFile file;
// Time in micros for next data record.
uint32_t logTime;

#define error(msg) sd.errorHalt(F(msg))
void setup() {
  Serial.begin(9600);                 // Start the serial terminal
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  setUpSDDevice();
  setUpAccDevice();
  setUpRelaysDevice();
}

void loop() {

  ADXL_ISR();

  if(freeFallDetected) {
    relay_2_On();
  }

  logData();

  // Force data to SD and update the directory entry to avoid data loss.
  if (!file.sync() || file.getWriteError()) {
    error("write error");
  }

  if (Serial.available()) {
    // Close file and stop.
    file.close();
    Serial.println(F("Done"));
    SysCall::halt();
  }
}

void logData() {
   // Accelerometer Readings
  int x,y,z;
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z

  file.print(String(millis()));
  file.write(',');
  file.print(x);
  file.write(',');
  file.print(y);
  file.write(',');
  file.print(z);
  file.write(',');
  file.print(String(freeFallDetected));
  file.println();
}

void setUpSDDevice() {
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME "00.csv";
  delay(1000);
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }
  if (BASE_NAME_SIZE > 6) {
    error("FILE_BASE_NAME too long");
  }
  while (sd.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      error("Can't create file name");
    }
  }
  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) {
    error("file.open");
  }
  Serial.print(F("Logging to: "));
  Serial.println(fileName);
  // Write data header.
  writeHeader();
  // Start on a multiple of the sample interval.
  logTime = micros()/(1000UL*SAMPLE_INTERVAL_MS) + 1;
  logTime *= 1000UL*SAMPLE_INTERVAL_MS;
  Serial.println("card initialized.");
}

void setUpAccDevice() {
  Serial.println("SparkFun ADXL345 Accelerometer");
  Serial.println();

  adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(16);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

  adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library

  adxl.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

  adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

  adxl.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)

  // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  adxl.setTapThreshold(50);           // 62.5 mg per increment
  adxl.setTapDuration(15);            // 625 μs per increment
  adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
  adxl.setDoubleTapWindow(200);       // 1.25 ms per increment

  // Set values for what is considered FREE FALL (0-255)
  adxl.setFreeFallThreshold(FREE_FALL_THRESHOLD);       // (5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(FREE_FALL_DURATION);       // (20 - 70) recommended - 5ms per increment

  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
  adxl.FreeFallINT(1);
  adxl.doubleTapINT(1);
  adxl.singleTapINT(1);
}

void setUpRelaysDevice() {
  /*
   * !!! IMPORTANT !!!
   * Don't use relay 1 and 4 while the SD card reader with chip select 4 is in use.
   */
  //  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  //  pinMode(RELAY_4, OUTPUT);
}

void ADXL_ISR() {
  // getInterruptSource clears all triggered actions after returning value
  // Do not call again until you need to recheck for triggered actions
  byte interrupts = adxl.getInterruptSource();
  // Free Fall Detection
  if(adxl.triggered(interrupts, ADXL345_FREE_FALL)){
    Serial.println("*** FREE FALL ***");
    freeFallDetected = true;
  }

  if(adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)){
    Serial.println("*** DOUBLE TAP ***");
    if(file.isOpen()) {
      file.close();
      Serial.println(F("Done"));
      Serial.println(F("Restarting SD card..."));
      setUpSDDevice();
      Serial.println(F("Restarted."));
    }
  }
}

// manage relays on/off state
void relay_2_On() {
  digitalWrite(RELAY_2, HIGH);
}
void relay_2_Off() {
  digitalWrite(RELAY_2, LOW);
}

void relay_3_On() {
  digitalWrite(RELAY_3, HIGH);
}
void relay_3_Off() {
  digitalWrite(RELAY_3, LOW);
}

//SD writing helpers
void writeHeader() {
  file.print(F("milis"));
  file.print(F(",x"));
  file.print(F(",y"));
  file.print(F(",z"));
  file.print(F(",state"));
  file.println();
}
