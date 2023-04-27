/*
  24-101: Introduction to Mechanical Engineering
  Course Instructors: Professor Haidar and Professor Gomez

  Challenge Control Systems Starter Code
*/

#include <NewPing.h>
#include <Servo.h>

// Edit on day of challenge
const int numLine = 7;

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

  Serial.begin(9600);
}

void loop()
{
  // Driving forward until the left wall ends.
  while (true) //(getDistance() < 25)
  {
    // Controller to maintain roughly 10 cm from left wall
    int deviation = getDistance() - 12;

    if (deviation > 2)
    { // too far
      bankLeft(64, 30);
    }
    else if (deviation < -2)
    { // too close
      bankRight(64, 30);
    }
    else
      forward(64);
  }
  stop();

  // (continue the rest of the course)
  forward(64);
  delay(1250);
  stop();

  turnRight();

  backward(64);
  while (!lineDetected())
    continue;

  delay(0);

  while (true)
  {
    stop();
  }
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

void bankRight(int speed, int diff)
{
  digitalWrite(MOTOR_OUTPUT, HIGH);
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTORS, speed - diff);
  analogWrite(RIGHT_MOTORS, speed + diff);
}

void bankLeft(int speed, int diff)
{
  digitalWrite(MOTOR_OUTPUT, HIGH);
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTORS, speed + diff);
  analogWrite(RIGHT_MOTORS, speed - diff);
}

void backward(int speed)
{
  digitalWrite(MOTOR_OUTPUT, HIGH);
  digitalWrite(LEFT_MOTOR_DIR, LOW);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTORS, speed);
  analogWrite(RIGHT_MOTORS, speed);
}

void stop()
{
  forward(0);
}

void turnRight()
{
  digitalWrite(MOTOR_OUTPUT, HIGH);
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTORS, 127);
  analogWrite(RIGHT_MOTORS, 127);

  delay(500);
  stop();
}

void turnLeft()
{
  digitalWrite(MOTOR_OUTPUT, HIGH);
  digitalWrite(LEFT_MOTOR_DIR, LOW);
  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTORS, 127);
  analogWrite(RIGHT_MOTORS, 127);

  delay(500);
  stop();
}

int getDistance()
{
  return (sonar.ping_cm() == 0) ? 30 : sonar.ping_cm();
}

void driveUntilLines(int lines)
{
  int linesCounted = 0;
  bool irPrevState = false;
  bool irCurrentState = false;

  forward(64);
  while (lines - linesCounted > 0)
  {
    irPrevState = irCurrentState;
    irCurrentState = lineDetected();
    if (!irPrevState && irCurrentState)
    {
      linesCounted++;
      Serial.println(linesCounted);
    }
  }
  stop();
}