/*
Get data from accelerometer, log to SD card.

// Equipment:
- Accelerometer: GY-521 / MPU6050
- SD Card adapter: SPI SD adapter
*/

#include <SPI.h>
#include <SD.h>

// I2Cdev and MPU6050 libraries must be installed. 
// Downloadable from https://github.com/jrowberg/i2cdevlib/tree/master/Arduino
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

const int SdChipSelect = 4;

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;

// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO

//#define LED_PIN 13
//bool blinkState = false;

File dataFile;

#define LED_PIN 5
#define RELAY_PIN 3

bool blinkState = false;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(38400);

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(SdChipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  // Delete the file if it exists. Start with a clean file every time.
  SD.remove("datalog.txt");
  
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  digitalWrite(RELAY_PIN, true);
}

unsigned long time;
unsigned int interval;
unsigned long tmp;

unsigned long count = 0;

void loop() {
  //Serial.println("Opening the file. Entries: " + String(count));
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  
  if (!dataFile) {
    // if the file isn't open, pop up an error:
    Serial.println("error opening datalog.txt");
    Serial.println("Trying to initialize the SD again");
    
    if (!SD.begin(SdChipSelect)) {
      Serial.println("Card failed, or not present");
    }
  } else {
  
    for(int i = 0; i < 100; i++) {
      tmp = millis();
      interval = tmp - time;
      time = tmp;
  
      // make a string for assembling the data to log:
      String dataString = String(count++) + "\t" + String(tmp) + "\t" + String(interval);
    
      // read raw accel/gyro measurements from device
      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
      // these methods (and a few others) are also available
      //accelgyro.getAcceleration(&ax, &ay, &az);
      //accelgyro.getRotation(&gx, &gy, &gz);
      
#ifdef OUTPUT_READABLE_ACCELGYRO
      dataString = dataString
        + "\t" + ax
        + "\t" + ay
        + "\t" + az
        + "\t" + gx
        + "\t" + gy
        + "\t" + gz;
#endif
    
      dataFile.println(dataString);
      // print to the serial port too:
      // This takes roughly 8-10 millis overhead
      //Serial.println(dataString);
      
      // If the device is turned to the side, close relay
      if (ax > 14000) {
        digitalWrite(RELAY_PIN, false);
      } else {
        digitalWrite(RELAY_PIN, true);
      }
    } // end for loop
  
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    dataFile.close();
  }
}
