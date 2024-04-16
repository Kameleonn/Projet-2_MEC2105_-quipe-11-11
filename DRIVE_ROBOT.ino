#include<SoftwareSerial.h>// SoftwareSerial for serial communication with HM10 bluetooth module.
#include<ArduinoBlue.h>// ArduinoBlue bluetooth library
#include<Wire.h>
#include<Adafruit_MotorShield.h>
#include"utility/Adafruit_MS_PWMServoDriver.h"
#include<Servo.h>// Library for servo motor

// shield -----------------------------------------------------------------------------------------------------
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *Rmotor = AFMS.getMotor(3);
Adafruit_DCMotor *Lmotor = AFMS.getMotor(4);


//bluetooth -----------------------------------------------------------------------------------------------------
const int speed = 255;
const int BLUETOOTH_TX = 2;
const int BLUETOOTH_RX = 3;
SoftwareSerial softSerial(BLUETOOTH_TX, BLUETOOTH_RX); // Object for serial communication to HM 10 bluetooth module using digital pins.
ArduinoBlue phone(softSerial);

//Servo declare -----------------------------------------------------------------------------------------------------
int servoPin_1 = 10;
int servoPin_2 = 9;
int angle, button , ID, sliderVal1, sliderVal2;
String  str;
Servo Servo1;
Servo Servo2;



// Driving function -----------------------------------------------------------------------------------------------------





void motorBrake() {
Rmotor -> run(RELEASE);
Lmotor -> run(RELEASE);
  // Do not write the motor speeds on this function. It simply configures the motor controller.
}
void motorSetForward() {
Rmotor -> run(FORWARD);
Lmotor -> run(FORWARD);
  // Do not write the motor speeds on this function. It simply configures the motor controller.
}
void motorSetBackward() {
Rmotor -> run(BACKWARD);
Lmotor -> run(BACKWARD);
  // Do not write the motor speeds on this function. It simply configures the motor controller.
}

void turn360Droite(){
Rmotor -> run(BACKWARD);
Lmotor -> run(FORWARD);
}

void turn360Gauche(){
Rmotor -> run(FORWARD);
Lmotor -> run(BACKWARD);
}

void driveControl() {
  // THROTTLE AND STEERING CONTROL
  // throttle values after subtracting 49:
  //     50 = max forward throttle
  //     0 = no throttle
  //     -49 = max reverse throttle
  // steering values after subtracting 49:
  //     50 = max right
  //     0 = straight
  //     -49 = max left
  int throttle = phone.getThrottle() - 49;
  int steering = phone.getSteering() - 49;
  if (throttle == 0 && steering == 0) {
    // If the throttle is zero, don't move.
    motorBrake();
    return;
  }
  // Determine forwards or backwards.
  if (throttle > 0) {
    // Forward
    motorSetForward();
  }
  else if (throttle < 0) {
    // Backward
    motorSetBackward();
  } else if (steering > 0) {
    turn360Droite();
   
  } else if (steering < 0) {
    turn360Gauche();
  }
}

//setup -----------------------------------------------------------------------------------------------------
void setup() {
 AFMS.begin();
 
 Rmotor -> setSpeed(100);
 Lmotor -> setSpeed (100);


 delay(500);
  // Start communication with HM10 bluetooth module.
  softSerial.begin(9600);
  Serial.begin(9600);
  Servo1.attach(servoPin_1);
  

  Servo1.write(0);
  delay(1000);
  Servo1.detach();

  Servo2.attach(servoPin_2);

  Servo2.write(25);
  delay(1000);
  Servo2.detach();
}




void loop() {
  // put your main code here, to run repeatedly:

// ############################### driving stuff  ##############################################
driveControl();
// ############################### get button and slider info  ##############################################







  int sliderId = phone.getSliderId();
  int sliderVal = phone.getSliderVal();
  int bouton = phone.getButton();
// store button info in a seperate value
  if (bouton != -1){
    button = bouton;
  }
  
  if(sliderId != -1){
    ID = sliderId;
    if(ID == 2){
      sliderVal1 = sliderVal;
      
    } else if (ID == 3){
      sliderVal2 = sliderVal;
      
    }
    
  }

  if (button == 4){
    Servo1.attach(servoPin_1);
    Servo1.write(map(sliderVal1, 0,100, 0 , 75));
    delay(200);
    Servo1.detach();
  }
  if(button == 5){
    Servo2.attach(servoPin_2);
    Servo2.write(map(sliderVal2, 0,100, 0 , 40));
    delay(200);
    Servo2.detach();
  }
  
  button = -1;




}