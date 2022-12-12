#include <Enes100.h>
#include <Tank.h>
#include <math.h>

int arucoID = 0;
int txPin = 0;
int rxPin = 1;
int START_ZONE;
const int ZONE_A = 0;
const int ZONE_B = 1;
const int CYCLE_INPUT_PIN = 6;
const int MAG_SENSOR_PIN = 7;
const int FRONT_SENSOR = 1;
float x;
float y;
float theta;

void setup() {
  //collect mission data
  beginMission();
  approachMissionSite();
  printLocation();
  
  /*
   * mission stuff
   */
  float dutyCycle = readDutyCycle();
  int percent = roundDutyCycle(dutyCycle);
  
  Enes100.print("Duty Cycle: ");
  Enes100.print(percent);
  Enes100.print("%");
  Enes100.println("");  
  //Enes100.mission(CYCLE, percent);
  
  Enes100.print("Magnetism: ");
  Enes100.print(isMagnetic() ? "MAGNETIC" : "NOT MAGNETIC");
  Enes100.println("");
  //Enes100.mission(MAGNETISM, isMagnetic() ? MAGNETIC : NOT_MAGNETIC);
  
  /*
   * leave the mission site and prepare to navigate obstacles
   */
  leaveMissionSite();
  //lower the arm?
}

void loop() {
  //navigate course
  updateLocation();
  setAngle(0);
    
  if (x < 2.8) {
    //move forward until an obstacle is encountered
    while (readFrontSensors() > 0.2 && x < 2.8) {
      updateLocation();
      moveForward(255);
      delay(10);
    }
    Tank.turnOffMotors();
    
    //turn perpendicular to the obstacle, towards whichever side is further
    if (y > 1) {
      setAngle(-M_PI / 2);
    } else {
      setAngle(M_PI / 2);
    }
    
    //move forward until no longer blocked by the obstacle
    moveForward(255);
    delay(1000);
    Tank.turnOffMotors();
    setAngle(0);
  }
  
  updateLocation();
  Tank.turnOffMotors(); 
}

void beginMission() {
  //pinMode(MAG_SENSOR_PIN, INPUT);
  
  Enes100.begin("DATA!!!!", DATA, arucoID, txPin, rxPin);
  Tank.begin();
  
  updateLocation();
  delay(10);
  if (y > 1) {
    Enes100.println("Zone A");
    START_ZONE = ZONE_A;
  } else {
    Enes100.println("Zone B");
    START_ZONE = ZONE_B;
  }
}

void leaveMissionSite() {
  updateLocation();
  if (START_ZONE == ZONE_A) {
    while (y < 0.8) {
      updateLocation();
      delay(10);
      moveBackward(255);
    }
  } else {
    while (y > 1.2) {
      updateLocation();
      delay(10);
      moveBackward(255);
    }
  }
  Tank.turnOffMotors();
}

void approachMissionSite() {
  updateLocation();
  if (START_ZONE == ZONE_A) {
    setAngle(-M_PI / 2);
    while (y > 0.7) {
      updateLocation();
      delay(10);
      moveForward(255);
    }
  } else {
    setAngle(M_PI / 2);
    while (y < 1.3) {
      updateLocation();
      delay(10);
      moveForward(255);
    }
  }
  Tank.turnOffMotors();
}

float readDutyCycle() {
  /*unsigned long highTime = pulseIn(CYCLE_INPUT_PIN, HIGH);
  unsigned long lowTime = pulseIn(CYCLE_INPUT_PIN, LOW);
  unsigned long totalTime = highTime + lowTime;
  float dutyCycle = (float)highTime / totalTime;

  return dutyCycle;*/
  return 64.2;
}

int roundDutyCycle(float cycle) {
  int result = 0;
  
  if (cycle <= 20)
    result = 1;
  else if (cycle <= 40)
    result = 3;
  else if (cycle <= 60)
    result = 5;
  else if (cycle <= 80)
    result = 7;
  else
    result = 9;
  
  return result;
}

bool isMagnetic() {
  //return digitalRead(MAG_SENSOR_PIN) == HIGH;
  return true;
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

void setAngle(float angle) {
  updateLocation();
  if (theta < angle) {
    while (theta < angle - 0.075) {
      updateLocation();
      delay(1);
      turnLeft(255);
    }
  } else {
    while (theta > angle + 0.075) {
      updateLocation();
      delay(1);
      turnRight(255);
    }
  }
  Tank.turnOffMotors();
}

float readFrontSensors() {  
  float leftSensor = Tank.readDistanceSensor(0);
  float rightSensor = Tank.readDistanceSensor(2);
  
  if (leftSensor < rightSensor) {
    return leftSensor;
  } else {
    return rightSensor;
  }
}
  
void turnRight(int speed) {
  Tank.setRightMotorPWM(-speed);
  Tank.setLeftMotorPWM(speed);
}
  
void turnLeft(int speed) {
  Tank.setRightMotorPWM(speed);
  Tank.setLeftMotorPWM(-speed);
}
  
void moveForward(int speed) {
  Tank.setRightMotorPWM(speed);
  Tank.setLeftMotorPWM(speed);
}

void moveBackward(int speed) {
  Tank.setRightMotorPWM(-speed);
  Tank.setLeftMotorPWM(-speed);
}

void updateLocation() {
  Enes100.updateLocation();
  x = Enes100.location.x;
  y = Enes100.location.y;
  theta = Enes100.location.theta;
}
