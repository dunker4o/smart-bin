#include <Thread.h>
#include <ThreadController.h>
#include <Stepper.h>


// defines pins numbers
const int enable1 = 12;
const int enable2 = 11;
const int lineA = 10;
const int lineB = 9;
const int echo1 = 8;
const int echo2 = 7;
const int echo3 = 6;
const int echo4 = 5;
const int echo5 = 4;

// defines variables
long duration[5];
int distance;


#define MAX_PULSE_WAIT 10000 // in microseconds
#define MAX_PULSE_DURATION 2942 // multiply by 0.017 to get actual distance [set for 50cm at 2942]
#define THREAD_INTERVAL 20


ThreadController tController = ThreadController();

Thread tSensor1 = Thread();
Thread tSensor2 = Thread();
Thread tSensor3 = Thread();
Thread tSensor4 = Thread();
Thread tSensor5 = Thread();

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library
// note switched pins 14 and 15
Stepper myStepper(stepsPerRevolution, 15, 14, 16, 17);
// enable stepper flag: 1 -- enabled
int bin_opened = 0;
int en_pin = 18;
float noRevs = 1;
int state;

#define BIN_RPM 380
#define BIN_DELAY_MS 500

void pollSensor1(){
  digitalWrite(lineA, LOW);
  digitalWrite(lineB, LOW);
  digitalWrite(enable1, LOW);
  
  delayMicroseconds(12);
  digitalWrite(enable1, HIGH);
  
  duration[0] = pulseIn(echo1, HIGH, MAX_PULSE_WAIT);
  if (duration[0] < MAX_PULSE_DURATION && duration[0] > 0){
    // Calculating the distance
    distance= duration[0]*0.017; // Original calculation was *0.034/2
    // Prints the distance on the Serial Monitor
    Serial.print("Distance-1: ");
    Serial.println(distance);
    open_accel(BIN_RPM, myStepper);
    delay(BIN_DELAY_MS);
    close_accel(380, myStepper);
  }
  
}

void pollSensor2(){
  digitalWrite(lineA, HIGH);
  digitalWrite(lineB, LOW);
  digitalWrite(enable1, LOW);
  
  delayMicroseconds(12);
  digitalWrite(enable1, HIGH);
  
  duration[1] = pulseIn(echo2, HIGH, MAX_PULSE_WAIT);
  if (duration[1] < MAX_PULSE_DURATION && duration[1] > 0){
    // Calculating the distance
    distance= duration[1]*0.017; // Original calculation was *0.034/2
    // Prints the distance on the Serial Monitor
    Serial.print("Distance-2: ");
    Serial.println(distance);
    open_accel(BIN_RPM, myStepper);
    delay(BIN_DELAY_MS);
    close_accel(380, myStepper);
  }

}


void pollSensor3(){
  digitalWrite(lineA, LOW);
  digitalWrite(lineB, HIGH);
  digitalWrite(enable1, LOW);
  
  delayMicroseconds(12);
  digitalWrite(enable1, HIGH);
  
  duration[2] = pulseIn(echo3, HIGH, MAX_PULSE_WAIT);
  if (duration[2] < MAX_PULSE_DURATION && duration[2] > 0){
    // Calculating the distance
    distance= duration[2]*0.017; // Original calculation was *0.034/2
    // Prints the distance on the Serial Monitor
    Serial.print("Distance-3: ");
    Serial.println(distance);
    open_accel(BIN_RPM, myStepper);
    delay(BIN_DELAY_MS);
    close_accel(380, myStepper);
  }

}

void pollSensor4(){
  digitalWrite(lineA, HIGH);
  digitalWrite(lineB, HIGH);
  digitalWrite(enable1, LOW);
  
  delayMicroseconds(12);
  digitalWrite(enable1, HIGH);
  
  duration[3] = pulseIn(echo4, HIGH, MAX_PULSE_WAIT);
  if (duration[3] < MAX_PULSE_DURATION && duration[3] > 0){
    // Calculating the distance
    distance= duration[3]*0.017; // Original calculation was *0.034/2
    // Prints the distance on the Serial Monitor
    Serial.print("Distance-4: ");
    Serial.println(distance);
    open_accel(BIN_RPM, myStepper);
    delay(BIN_DELAY_MS);
    close_accel(380, myStepper);
  }

}


void pollSensor5(){
  digitalWrite(lineA, LOW);
  digitalWrite(lineB, LOW);
  digitalWrite(enable1, HIGH);
  digitalWrite(enable2, LOW);
  
  delayMicroseconds(12);
  digitalWrite(enable2, HIGH);
  
  duration[4] = pulseIn(echo5, HIGH, MAX_PULSE_WAIT);
  if (duration[4] < MAX_PULSE_DURATION && duration[4] > 0){
    // Calculating the distance
    distance= duration[4]*0.017; // Original calculation was *0.034/2
    // Prints the distance on the Serial Monitor
    Serial.print("Distance-5: ");
    Serial.println(distance);
    open_accel(BIN_RPM, myStepper);
    delay(BIN_DELAY_MS);
    close_accel(380, myStepper);
  }

}

void enable_stepper(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  }

void disable_stepper(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  }



void open_accel(int max_rpm, Stepper stepper){

  Serial.println("opening");
  enable_stepper(en_pin);
   
  for (int i = 1; i<11; i++){
    stepper.setSpeed(i*max_rpm/11);
    stepper.step(4+i);
    }
   stepper.step(500);

  for (int i = 11; i>0; i--){
    stepper.setSpeed(i*max_rpm/11);
    stepper.step(4+i);
    }
    
  disable_stepper(en_pin);
  bin_opened = 1;
  
}

void close_accel(int max_rpm, Stepper stepper){

  Serial.println("closing");
  enable_stepper(en_pin);
  
  for (int i = 1; i<11; i++){
    stepper.setSpeed(i*max_rpm/10);
    stepper.step(-4-i);
    }

  stepper.step(-500);

  for (int i = 10; i>0; i--){
    stepper.setSpeed(i*max_rpm/10);
    stepper.step(-4-i);
    }
    
  disable_stepper(en_pin);
  bin_opened = 0;
  
}



void setup() {
  pinMode(enable1, OUTPUT); // Sets the trigPin as an Output
  pinMode(enable2, OUTPUT);
  pinMode(lineA, OUTPUT);
  pinMode(lineB, OUTPUT);
  
  pinMode(echo1, INPUT); // Sets the echoPin as an Input
  pinMode(echo2, INPUT);
  pinMode(echo3, INPUT);
  pinMode(echo4, INPUT);
  pinMode(echo5, INPUT);
  
  digitalWrite(enable1, HIGH);
  digitalWrite(enable2, HIGH);
  
  Serial.begin(9600); // Starts the serial communication

  Serial.println("Starting threads now...");
  
  tSensor1.onRun(pollSensor1);
  tSensor1.setInterval(THREAD_INTERVAL);
  
  tSensor2.onRun(pollSensor2);
  tSensor2.setInterval(THREAD_INTERVAL);
  
  tSensor3.onRun(pollSensor3);
  tSensor3.setInterval(THREAD_INTERVAL);
  
  tSensor4.onRun(pollSensor4);
  tSensor4.setInterval(THREAD_INTERVAL);

  tSensor5.onRun(pollSensor5);
  tSensor5.setInterval(THREAD_INTERVAL);
  
  tController.add(&tSensor1);
  tController.add(&tSensor2);
  tController.add(&tSensor3);
  tController.add(&tSensor4);
  tController.add(&tSensor5);

  // set the speed at 60 rpm:
  myStepper.setSpeed(100);

}

void loop() {
  tController.run();
}
