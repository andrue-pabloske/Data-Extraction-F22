Servo arm;

void setup() {
  // put your setup code here, to run once:
  arm.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  lowerArm();
  delay(1000);
  raiseArm();
  delay(1000);
}

void raiseArm() {
  arm.write(180);
}

void lowerArm() {
  arm.write(0);
}