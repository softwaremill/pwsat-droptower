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