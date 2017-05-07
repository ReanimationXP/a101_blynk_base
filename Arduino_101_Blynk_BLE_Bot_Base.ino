#define BLYNK_PRINT Serial //Blynk Stuff
#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h> //Intel Stuff
#include <Wire.h> //Arduino Stuff
#include <Adafruit_MotorShield.h> //Adafruit Motorshield Stuff
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Get your Auth Token from the Blynk App in Project Settings (nut icon).
char auth[] = "AUTH TOKEN GOES HERE. DON'T FORGET TO CHANGE YOUR BOT NAME BELOW TOO!";

BLEPeripheral  blePeripheral;

BlynkTimer blynkTimer1; // Optional, create a Timer object.

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Define 'ports' M1, M2, M3, M4.
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);

//######### SETUP ######################################
void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Serial connection initialized.");

  // The name your bluetooth service will show up as, customize this.
  blePeripheral.setLocalName("DrewBot"); //CHANGE YOUR BOT NAME HERE
  blePeripheral.setDeviceName("DrewBot"); //THIS ONE TOO
  blePeripheral.setAppearance(384);
  
  Blynk.begin(auth, blePeripheral);
  blePeripheral.begin();
  Serial.println("Waiting for connections...");
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz

  //optional, we'll use this later to expose D13 (the LED) to Blynk V13
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); //turn it off for now

  //optional, tell timer to call sendUptime every 200ms, keep an eye on connection
  blynkTimer1.setInterval(100L, sendUptime);
  
  Serial.println("Adafruit Motorshield v2 - DC Motor");
  // Set the speed to start, from 0 (off) to 255 (max speed)
  motor1->setSpeed(255);
  motor2->setSpeed(255);
  motor3->setSpeed(255);
  motor4->setSpeed(255);  

}


//########## LOOP ######################################
void loop() {
  Blynk.run();
  blePeripheral.poll();
  
  //optional, fire timer if it's got work to do (sending uptime)
  blynkTimer1.run();
}



//######### Subroutines ################################

/*-------- Not used atm -------
BLYNK_READ(V5) // Widget in the app READs Virtal Pin V5 with the certain frequency
{
  // This command writes Arduino's uptime in seconds to Virtual Pin V5
  Blynk.virtualWrite(5, millis() / 1000);
}
*/

//optional, sends uptime to Blynk to keep an eye on the connection.
void sendUptime()
{
  // This function sends Arduino uptime every 100ms to Virtual Pin (V10)
  // In the app, Widget's reading frequency should be set to PUSH.
  // You can send anything with any interval using this construction
  // Don't send more that 10 values per second
  digitalWrite(LED_BUILTIN, HIGH);  //pin 13
  Blynk.virtualWrite(V10, millis());
  digitalWrite(LED_BUILTIN, LOW);  //pin 13
}

// This function will set the speed
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V0 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.println("Changing speed... ");
  motor1->setSpeed(pinValue);
  motor2->setSpeed(pinValue); 
  motor3->setSpeed(pinValue);
  motor4->setSpeed(pinValue);
  Serial.print("Speed (V0) is now: ");
  Serial.println(pinValue);   
}

// Left Motors Forward
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("Left Motor(s) Forward - ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motor1->run(FORWARD);
    motor3->run(FORWARD);
  }
  if(pinValue == 0) {
    motor1->run(RELEASE);
    motor3->run(RELEASE);    
  }
}


// Right Motors Forward
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V2 to a variable
  Serial.print("Right Motor(s) Forward - ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motor2->run(FORWARD);
    motor4->run(FORWARD);   
  }
  if(pinValue == 0) {
    motor2->run(RELEASE);
    motor4->run(RELEASE);
  }
}

// Left Motors Backward
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V3 to a variable
  Serial.print("Left Motor(s) Backward - ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motor1->run(BACKWARD);
    motor3->run(BACKWARD);    
  }
  if(pinValue == 0) {
    motor1->run(RELEASE);
    motor3->run(RELEASE);    
  }
}

// Right Motors Backward
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V4 to a variable
  Serial.print("Right Motor(s) Backward - ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motor2->run(BACKWARD);
    motor4->run(BACKWARD);
  }
  if(pinValue == 0) {
    motor2->run(RELEASE);
    motor4->run(RELEASE);    
  }
}

// LED Button - Useful way of checking if communication is working (besides uptime)
BLYNK_WRITE(V13)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V13 to a variable
  
  if(pinValue == 1) {
    Serial.print("LED on - ");
    digitalWrite(LED_BUILTIN, HIGH); //pin 13
  }
  if(pinValue == 0) {
    Serial.print("LED off - ");
    digitalWrite(LED_BUILTIN, LOW);  //pin 13
  }
  Serial.println(pinValue);
}


