#include <AFMotor.h>
#include <Enes100.h>
#include <Servo.h>

#define ARUCO_ID 217
#define TX_PIN 3
#define RX_PIN 4
#define ARM_PIN 9
#define CYCLE_INPUT_PIN 8
#define HALL_SENSOR_D 7 //orange
#define HALL_SENSOR_A A0 //yellow

Servo arm;

void setup() {
  // put your setup code here, to run once:
  Enes100.begin("SQUIRRELS!!!", DATA, ARUCO_ID, TX_PIN, RX_PIN);
  delay(1000);

  pinMode(CYCLE_INPUT_PIN, INPUT);

  arm.attach(ARM_PIN);
  lowerArm();
  
  Enes100.println("doing mission stuff");
  delay(1000);
  Enes100.mission(CYCLE, readDutyCycle());
  Enes100.mission(MAGNETISM, magnetismResult());
  raiseArm();
}

void loop() {
  // put your main code here, to run repeatedly:
  
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

void raiseArm() {
  arm.write(180);
}

void lowerArm() {
  arm.write(0);
}
