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

***Important***

Please not that using ethernet shield (for SD card capabilities) and Relay Shield together the relay 1 nad 4 needs to be switched off as the pins streering them are used by the ethernet shield itself.


## Writing to SD Card

For writing to SD the extnal Arduino library was used. The library is called [SdFat](https://github.com/greiman/SdFat) and its much faster compared to the standard one available out of the box.

Executed tests with writing short strings into sd card:

| milis | x    | y    | z    | state |
|-------|------|------|------|-------|
| 1058  | 7    | -35  | 6    | 0     |
| 1067  | 8    | -37  | 5    | 0     |
| 1076  | 8    | -35  | 6    | 0     |
| 1085  | 9    | -33  | 5    | 0     |

* x,y,z - readings from accelerometer
* state - additional data where FF indicates the point where the Free Fall was detected
* milis - time in miliseconds since the program started (used just for testing purposes)

Experiment data: 

16.9 MB after 2.5 h of work

Example data can be found in the repo itslef under `example-data.csv`
