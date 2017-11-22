# pwsat-droptower

This project is a part of PWSat2 Drop Tower tests campaign.

This project consist of few parts:
* Arduino UNO board + Arduino protoshield - it is called `MainController`.
* `BackupLogger` which logs all transmitted data via an UART by the Arduino board.
* An external electromagnet which is controlled via the `MainController`.
* Separation switch which detects DropTES-PWSat2 separation. Checked by the `MainController`.
* Redundant battery pack composed of lead-acid batteries.

# Arduino board modes
There are two modes of the Arduino board which are selected with a jumper:
* **Free Fall triggering** - it detects free fall, next it waits between 20-300ms and it cuts-off an electromagnet.
* **Switch triggering** - it detects change the switch's state, next it waits 100-500ms and it cuts-off an electromagnet.

In case of accelerometers' failure it automatically change the current mode to switch triggering mode. Any microSD failure doesn't affects on Arduino board mode.

## Detecting Free Fall

The ADXL345 accelerometer is responsible for free fall detection.

The ADXL345 library installation and testing:
* For free fall detection with ADXL345 clone the library from [GitHub](https://github.com/sparkfun/SparkFun_ADXL345_Arduino_Library) into your Arduino Lib folder.
* Upload the following into the board: [code](https://github.com/sparkfun/SparkFun_ADXL345_Arduino_Library/blob/master/examples/SparkFun_ADXL345_Example/SparkFun_ADXL345_Example.ino)
* Drop the board from min 1-1.5 meters and see the messages in the serial monitor.

## Controlling a MOSFET
The MOSFET transistor is responsible for switching on/off the 12V for an electromagnet.
It is controller via the `D7` port:
* 0 -> electromagnet is ON,
* 1 -> electrmagnet is OFF.

Arduino reset doesn't affect on the stwich.

## Writing to SD Card on Arduino board
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

