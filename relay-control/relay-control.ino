/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

#define RELAY_1  4
#define RELAY_2  7
#define RELAY_3  8
#define RELAY_4  12


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
}

void relay_1_On() {
  digitalWrite(RELAY_1, HIGH);
}
void relay_1_Off() {
  digitalWrite(RELAY_1, LOW);
}

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

void relay_4_On() {
  digitalWrite(RELAY_4, HIGH);
}
void relay_4_Off() {
  digitalWrite(RELAY_4, LOW);
}
// the loop function runs over and over again forever

//demo
int counter = 0;
void loop() {
  if(counter == 0){
     relay_1_On();      
     counter = counter + 1;             
  }else if(counter == 1){
    relay_2_On();
    counter = counter + 1;
  }else if(counter == 2){
    relay_3_On();
    counter = counter + 1;
  }else if(counter == 3){
    relay_4_On();
    counter = counter + 1;
  }else if(counter == 4){
    relay_1_Off();
    relay_2_Off();
    relay_3_Off();
    relay_4_Off();
    counter = 0;
  }
  delay(1000);                      
}


