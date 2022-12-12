#include <AFMotor.h>
#include <Enes100.h>
#include <math.h>
#include <Servo.h>

#define ARUCO_ID 217
#define TX_PIN 3
#define RX_PIN 4
#define ARM_PIN 9
#define FRONT_TRIG 11
#define FRONT_ECHO 12
#define CYCLE_INPUT_PIN 8
#define HALL_SENSOR_D 7 //orange
#define HALL_SENSOR_A A0 //yellow

float x;
float y;
float theta;

AF_DCMotor leftMotor(3);
AF_DCMotor rightMotor(4);
Servo arm;

void setup() {
  // put your setup code here, to run once:
  Enes100.begin("SQUIRRELS!!!", DATA, ARUCO_ID, TX_PIN, RX_PIN);
  delay(1000);

  pinMode(FRONT_ECHO, INPUT);
  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(CYCLE_INPUT_PIN, INPUT);

  leftMotor.setSpeed(100);
  leftMotor.run(RELEASE);
  rightMotor.setSpeed(100);
  rightMotor.run(RELEASE);

  arm.attach(ARM_PIN);
  lowerArm();

  updateLocation();
  delay(500);

  Enes100.println("going to mission site");
  raiseArm();
  approachMissionSite();
  delay(1000);
  lowerArm();
  
  Enes100.println("doing mission stuff");
  delay(2000);
  Enes100.mission(CYCLE, readDutyCycle());
  Enes100.mission(MAGNETISM, magnetismResult());
  raiseArm();
  delay(500);
  
  leaveMissionSite();
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:

  // if in obstacle zone
  if (x < 2.8) {
    //move east until an obstacle is encountered
    setAngle(0);
    moveForward();
    while (readFrontSensor() > 0.2 && x < 2.8) {
      updateLocation();
      delayMicroseconds(10);
    }
    stopMotors();
    
    //move until no longer blocked by the obstacle
    if (x < 2.8) {
      if (y < 0.75) {
        moveToY(1.0);
      } else if (y > 0.75 && y < 1.25) {
        moveToY(1.5);
      } else {
        moveToY(0.5);
      }
    }
  }

  // if before limbo
  if (x >= 2.8 && x < 3.5) {
    moveToY(1.5);
    moveToX(3.7);
  }

  stopMotors();
}

void leaveMissionSite() {
  if (y < 1) {
    moveToY(1.5);
  } else {
    moveToY(0.5);
  }
}

void approachMissionSite() {
  if (y > 1) {
    moveToY(0.7);
    setAngle(-M_PI / 2);
  } else {
    moveToY(1.3);
    setAngle(M_PI / 2);
  }
}

int readDutyCycle() {
  int highCount = 0;
  int lowCount = 0;
  int totalCount = 1000;

  for(int i = 0; i < totalCount; i++) {
    if (digitalRead(CYCLE_INPUT_PIN) == HIGH) {
      highCount++;
    } else {
      lowCount++;
    }
    delay(1);
  }

  float percent = (float) highCount / totalCount;

  if (percent <= .2) {
    return 1;
  } else if (percent <= .4) {
    return 3;
  } else if (percent <= .6) {
    return 5;
  } else if (percent <= .8) {
    return 7;
  } else {
    return 9;
  }
}

bool magnetismResult() {
  if (analogRead(HALL_SENSOR_A) < 533 && HALL_SENSOR_D == LOW) {
    return NOT_MAGNETIC;
  } else {
    return MAGNETIC;
  }
}

void moveToY(float newY) {
  updateLocation();
  if (y < newY) {
    setAngle(M_PI/2);
    moveForward();
    while(y < newY) {
      updateLocation();
      delayMicroseconds(10);
    }
  } else {
    setAngle(-M_PI/2);
    moveForward();
    while(y > newY) {
      updateLocation();
      delayMicroseconds(10);
    }
  }
  stopMotors();
}

void moveToX(float newX) {
  updateLocation();
  if (x < newX) {
    setAngle(0);
    moveForward();
    while(x < newX) {
      updateLocation();
      delayMicroseconds(10);
    }
  } else {
    setAngle(M_PI);
    moveForward();
    while(x > newX) {
      updateLocation();
      delayMicroseconds(10);
    }
  }
  stopMotors();
}

void setAngle(float angle) {
  updateLocation();
  if (theta < angle) {
    turnCCW();
    while (theta < angle - 0.5) {
      updateLocation();
      delayMicroseconds(10);
    }
  } else {
    turnCW();
    while (theta > angle + 0.5) {
      updateLocation();
      delayMicroseconds(10);
    }
  }
  stopMotors();
}

float readFrontSensor() {
  long duration;
  int distance;

  digitalWrite(FRONT_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(FRONT_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(FRONT_TRIG, LOW);

  duration = pulseIn(FRONT_ECHO, HIGH);
  distance = duration * 0.034 / 2; //duration * speed of sound in cm/microsecond / 2

  return (float) distance / 100;
}

void raiseArm() {
  arm.write(180);
}

void lowerArm() {
  arm.write(0);
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
  leftMotor.run(BACKWARD);
  rightMotor.run(FORWARD);
}

void stopMotors() {
  leftMotor.run(RELEASE);
  rightMotor.run(RELEASE);
}

void updateLocation() {
  Enes100.updateLocation();
  x = Enes100.location.x;
  y = Enes100.location.y;
  theta = Enes100.location.theta;
}

void printLocation() {
  updateLocation();
  Enes100.print("x: ");
  Enes100.print(x);
  Enes100.print(", y: ");
  Enes100.print(y);
  Enes100.print(", theta: ");
  Enes100.print(theta / M_PI);
  Enes100.print("*pi");
  Enes100.println("");
}
