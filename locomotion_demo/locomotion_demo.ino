#include <AFMotor.h>

AF_DCMotor leftMotor(3);
AF_DCMotor rightMotor(4);

void setup() {
  // put your setup code here, to run once:
  leftMotor.setSpeed(175);
  leftMotor.run(RELEASE);
  rightMotor.setSpeed(200);
  rightMotor.run(RELEASE);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveForward();
}



void moveForward() {
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
}

void moveBackward() {
  leftMotor.run(BACKWARD);
  rightMotor.run(BACKWARD);
}

void turnCW() {
  leftMotor.run(FORWARD);
  rightMotor.run(BACKWARD);
}

void turnCCW() {
  leftMotor.run(FORWARD);
  rightMotor.run(BACKWARD);
}

void stopMotors() {
  leftMotor.run(RELEASE);
  rightMotor.run(RELEASE);
}
