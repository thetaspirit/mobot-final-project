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

// Servo Pins
const int US_SERVO_PIN = 11;
const int LEVER_SERVO_PIN = 10;

NewPing sonar(TRIG, ECHO, 30);
Servo usServo;
Servo leverServo;

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

  usServo.attach(US_SERVO_PIN);
  usServo.write(180);

  leverServo.attach(LEVER_SERVO_PIN);
  leverServo.write(90);

  Serial.begin(9600);
}

void loop()
{
  driveUntilLines(numLine);

  turnLeft();

  forward(64);
  delay(250);

  while (getDistance() < 25)
  {
    forward(96);
  }
  stop();

  forward(64);
  delay(1250);
  stop();

  delay(1000); // give time for the wind to turn off???

  turnRight();

  reverseLeft(64);
  delay(750);

  backward(64);
  while (!lineDetected())
    continue;

  delay(100);
  stop();

  leverServo.write(190);

  while (true)
  {
    stop();
  }
}

int getDistance()
{
  return (sonar.ping_cm() == 0) ? 30 : sonar.ping_cm();
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

  delay(475);
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

void reverseLeft(int speed)
{
  digitalWrite(MOTOR_OUTPUT, HIGH);
  digitalWrite(LEFT_MOTOR_DIR, LOW);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTORS, speed + 20);
  analogWrite(RIGHT_MOTORS, speed - 20);
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
