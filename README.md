# pwsat-droptower

This project is a part of PWSat2 Drop Tower tests.

## Detecting Free Fall

### ADXL345

1. For free fall detection with ADXL345 clone the library from [GitHub](https://github.com/sparkfun/SparkFun_ADXL345_Arduino_Library) into your Arduino Lib folder.
2. Upload the following into the board: [code](https://github.com/sparkfun/SparkFun_ADXL345_Arduino_Library/blob/master/examples/SparkFun_ADXL345_Example/SparkFun_ADXL345_Example.ino)
3. Drop the board from min 1-1.5 meters and see the messages in the serial monitor.

## Controlling Relays

### Arduino 4 Relay Shield

The shield can be bought from [botland.com.pl](https://botland.com.pl/arduino-shield-ekspandery-wyprowadzen/4243-arduino-4-relay-shield-4-przekazniki-30v2a.html)

Responsible pins for controlling the relays are:

* Relay 1 (REL 1)	D4
* Relay 2 (REL 2)	D7
* Relay 3 (REL 3)	D8
* Relay 4  (REL 4)	D12

Example file `relay-control.ino` switches on and off all the relays simultaneously.

## Writing to SD Card

Executed tests with writing short strings into sd card:

|  x    |    y   |  z   |  d |  t  | 
|-------|--------|------|----|-----| 
| 3.434 | 5.2342 | 2.22 | -- | 10  | 
| 3.434 | 5.2342 | 2.22 | -- | 33  | 
| 3.434 | 5.2342 | 2.22 | -- | 57  | 
| 3.434 | 5.2342 | 2.22 | -- | 83  | 
| 3.434 | 5.2342 | 2.22 | -- | 107 | 
| 3.434 | 5.2342 | 2.22 | -- | 131 | 
| 3.434 | 5.2342 | 2.22 | FF | 155 | 
| 3.434 | 5.2342 | 2.22 | FF | 179 | 
| 3.434 | 5.2342 | 2.22 | FF | 202 | 

* x,y,z - readings from accelerometer
* d - additional data where FF indicates the point where the Free Fall was detected
* t - time in miliseconds since the program started (used just for testing purposes)

**Important!!**

While writing to SD card the maximum frequency achieved during tests as about 27 ms, which means that only about 3 writes per second are possible. 
This needs additional testing with faster SD cards, smaller data to write and/or same caching of the data before writing it down to SD card.
