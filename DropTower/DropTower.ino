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
#include <SPI.h>                      // SD writing capability
#include <SD.h>                       // SD writing capability

/*
 * !!! IMPORTANT !!!
 * Done tuse relay 1 and 4 while the SD card reader with chip select 4 is in use.
 */
#define RELAY_1  4 //cannot be used together with SD Card reader
#define RELAY_2  7
#define RELAY_3  8
#define RELAY_4  12  //cannot be used together with SD Card reader

ADXL345 adxl = ADXL345();             
const int CHIP_SELECT = 4;             //needed to set up SPI communication on the ethernet shield
const int FREE_FALL_THRESHOLD = 7;      // (5 - 9) recommended - 62.5mg per increment
const int FREE_FALL_DURATION = 30;      // (20 - 70) recommended - 5ms per increment   
const String LOG_FILE_NAME = "datalog2.txt";
bool freeFallDetected = false;          // Free Fall detection flag


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
  String dataString = "";                 //data to be written to sd card
   // Accelerometer Readings
  int x,y,z;   
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z

  dataString += String(x);
  dataString += ";";
  dataString += String(y);
  dataString += ";";
  dataString += String(z);
  dataString += ";";
  dataString += String(freeFallDetected);
  dataString += ";";
  dataString += String(millis());

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
//  File dataFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
//  if (dataFile) {
//    dataFile.println(dataString);
//    dataFile.close();
//    // print to the serial port too:
//    Serial.println(dataString);
//  }
  
  ADXL_ISR();
  
  if(freeFallDetected){
    relay_2_On();
  }
}

void setUpSDDevice() {
  if (!SD.begin(CHIP_SELECT)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
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
   * Done tuse relay 1 and 4 while the SD card reader with chip select 4 is in use.
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
  // LEFT FOR TESTING PURPOSES ONLY
  // Tap Detection
//  if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP)){
//    Serial.println("*** TAP ***");
//    freeFallDetected = true;
//  } 
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

