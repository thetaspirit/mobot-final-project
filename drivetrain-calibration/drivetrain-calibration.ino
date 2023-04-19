const int LEFT_MOTORS = 5;     // pin that controls the speed of the 2 left side motors - PWM pin
const int RIGHT_MOTORS = 6;    // pin that controls the speed of the 2 right side motors - PWM pin
const int LEFT_MOTOR_DIR = 8;  // pin that controls the polarity, or direction, of the 2 left side motors (HIGH = forward, LOW = backward)
const int RIGHT_MOTOR_DIR = 7; // pin that controls the polarity, or direction, of the 2 left side motors (HIGH = forward, LOW = backward)
const int MOTOR_OUTPUT = 3;    // controls whether the motors are allowed to run or not

void setup()
{
    pinMode(MOTOR_OUTPUT, OUTPUT);
    pinMode(LEFT_MOTORS, OUTPUT);
    pinMode(RIGHT_MOTORS, OUTPUT);
    pinMode(LEFT_MOTOR_DIR, OUTPUT);
    pinMode(RIGHT_MOTOR_DIR, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    forward(127);
    // turnLeft();
    // delay(1000);
    // turnLeft();
    // delay(1000);
    // turnRight();
    // delay(1000);
    // turnRight();
    // delay(1000);
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

void turnLeft()
{
    digitalWrite(MOTOR_OUTPUT, HIGH);
    digitalWrite(LEFT_MOTOR_DIR, HIGH);
    digitalWrite(RIGHT_MOTOR_DIR, LOW);
    analogWrite(LEFT_MOTORS, 127);
    analogWrite(RIGHT_MOTORS, 127);

    delay(525);
    stop();
}

void turnRight()
{
    digitalWrite(MOTOR_OUTPUT, HIGH);
    digitalWrite(LEFT_MOTOR_DIR, LOW);
    digitalWrite(RIGHT_MOTOR_DIR, HIGH);
    analogWrite(LEFT_MOTORS, 127);
    analogWrite(RIGHT_MOTORS, 127);

    delay(575);
    stop();
}
