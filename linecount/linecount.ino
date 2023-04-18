/*
  24-101: Introduction to Mechanical Engineering
  Course Instructors: Professor Haidar and Professor Gomez

  Challenge Control Systems Starter Code
*/

#include <NewPing.h>
#include <Servo.h>

// Edit on day of challenge
const int numLine = 7;
const int distVal = 12; // TODO calibrate this!!!

// Define all of the pins used.  These are NOT up to us, but rather what Elegoo decided.  Don't change.
// If your car doesn't function properly  with these pins, please double check your pin connections.
const int LEFT_MOTORS = 5;     // pin that controls the speed of the 2 left side motors - PWM pin
const int RIGHT_MOTORS = 6;    // pin that controls the speed of the 2 right side motors - PWM pin
const int LEFT_MOTOR_DIR = 8;  // pin that controls the polarity, or direction, of the 2 left side motors (HIGH = forward, LOW = backward)
const int RIGHT_MOTOR_DIR = 7; // pin that controls the polarity, or direction, of the 2 left side motors (HIGH = forward, LOW = backward)
const int MOTOR_OUTPUT = 3;    // controls whether the motors are allowed to run or not

// IR Detector Pins
const int RIGHT = A0;
const int MIDDLE = A1;
const int LEFT = A2;

// Ultrasonic Sensor Pins
const int ECHO = 12;
const int TRIG = 13;

// Servo Pin
const int SERVO_PIN = 11;

int linesCounted = 0;

bool irState = false;

bool done = false;

NewPing sonar(TRIG, ECHO, 30);
Servo servo;

void setup()
{
  pinMode(LEFT, INPUT);
  pinMode(MIDDLE, INPUT);
  pinMode(RIGHT, INPUT);

  pinMode(MOTOR_OUTPUT, OUTPUT);
  pinMode(LEFT_MOTORS, OUTPUT);
  pinMode(RIGHT_MOTORS, OUTPUT);
  pinMode(LEFT_MOTOR_DIR, OUTPUT);
  pinMode(RIGHT_MOTOR_DIR, OUTPUT);

  servo.attach(SERVO_PIN);

  servo.write(180);
  delay(250);

  Serial.begin(9600);
}

void loop()
{
  bool irCurrentState = lineDetected();
  if (numLine - linesCounted > 0)
  {
    if (!irState && irCurrentState)
    {
      linesCounted++;
      stop();
      servoSwivel();
      // note that there is a pause built into servoSwivel.
    }
    else
    {
      forward(127); // resume driving if stopped.
    }
    irState = irCurrentState;
  }
  else // we've passed enough lines
  {
    stop();
    delay(3000);
    servo.write(90);
    while (getDistance() > distVal && !done)
    {
      forward(90);
    }
    stop();
    done = true;
  }

  delay(10);
}

bool lineDetected()
{
  int leftData = analogRead(LEFT);
  int middleData = analogRead(MIDDLE);
  int rightData = analogRead(RIGHT);

  return leftData > 100 || middleData > 100 || rightData > 100;
}

void forward(int speed)
{
  digitalWrite(MOTOR_OUTPUT, HIGH);
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTORS, speed);
  analogWrite(RIGHT_MOTORS, speed);
}

void stop()
{
  forward(0);
}

void servoSwivel()
{
  servo.write(0);
  delay(250);
  servo.write(180);
}

unsigned long getDistance()
{
  return sonar.ping_cm() - 1;
}